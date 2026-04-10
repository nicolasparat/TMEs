#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched/task.h>
#include <linux/pid.h>
#include <linux/slab.h>
#include <linux/shrinker.h>

#define MAX_LEN 32

MODULE_DESCRIPTION("Hello ioctl module");
MODULE_AUTHOR("Nicolas");
MODULE_LICENSE("GPL");

static int pid;
module_param(pid, int, 0660);
MODULE_PARM_DESC(pid, "PID to monitor");

struct task_monitor {
    struct pid *pid_struct; 
    struct list_head head;
    struct mutex mtx;
    int count;
};

struct task_sample {
    u64 utime;
    u64 stime;
    struct list_head list;
    unsigned long total_vm;
    unsigned long stack_vm;
    unsigned long data_vm;
};

static struct task_monitor monitor;
static struct task_struct *monitor_thread;
static struct kmem_cache *sample_cache;

int monitor_pid(pid_t my_pid) {
    struct pid * pid_struct = find_get_pid(my_pid);
    if (!pid_struct) {
        printk(KERN_INFO "PID %d not found\n", my_pid);
        return -ESRCH;
    }

    monitor.pid_struct = pid_struct;
    monitor.count = 0;
    mutex_init(&monitor.mtx);
    INIT_LIST_HEAD(&monitor.head);

    printk(KERN_INFO "monitor_pid: Monitoring PID %d\n", pid);

    return 0;
}

bool get_sample(struct task_monitor *tm, struct task_sample *smpl) {
    struct task_struct *tasks = get_pid_task(tm->pid_struct, PIDTYPE_PID);
    if (!tasks) return false;
    
    bool is_alive = pid_alive(tasks);
    if(is_alive) {
        smpl->utime = tasks->utime;
        smpl->stime = tasks->stime;

        struct mm_struct *mm = get_task_mm(tasks);
        if (mm) {
            smpl->total_vm = mm->total_vm;
            smpl->stack_vm = mm->stack_vm;
            smpl->data_vm = mm->data_vm;
            mmput(mm);
        }
    }
    
    put_task_struct(tasks);
    return is_alive;
}

int save_sample(void) {
    struct task_sample *my_sample = kmem_cache_alloc(sample_cache, GFP_KERNEL);
    if (!my_sample) {
        printk(KERN_ERR "kmalloc failed\n");
        return -ENOMEM;
    }

    if (!get_sample(&monitor, my_sample)) {
        kmem_cache_free(sample_cache, my_sample);
        return -ESRCH;
    }

    mutex_lock(&monitor.mtx);
    list_add(&my_sample->list, &monitor.head);
    monitor.count++;
    mutex_unlock(&monitor.mtx);

    return 0;
}

int monitor_fn(void *arg) {
    // NB : Ce serait plus propre d'utiliser la valeur de arg plutôt que le monitor global, mais le résultat est le même (et j'ai la flemme).
    while (!kthread_should_stop()) {
        if (save_sample() == -ESRCH) { break; }
        ssleep(1);
    }

    return 0;
}

static ssize_t taskmonitor_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    struct task_sample *entry;
    int counter = 0;
    
    mutex_lock(&monitor.mtx);
    list_for_each_entry(entry, &monitor.head, list) {
        counter += scnprintf(
            buf + counter, PAGE_SIZE - counter, "pid %d usr %llu sys %llu total %lu stack %lu data %lu \n", 
            pid, (unsigned long long)entry->utime, (unsigned long long)entry->stime, entry->total_vm, entry->stack_vm, entry->data_vm
        );

        if (counter >= PAGE_SIZE) break;
    }
    mutex_unlock(&monitor.mtx);

    return counter;
}

static ssize_t taskmonitor_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    size_t len;

    /* Remove trailing newline if present */
    len = strcspn(buf, "\n");

    if (len >= MAX_LEN) {
        return -EINVAL;
    }

    char tmp[MAX_LEN];
    strncpy(tmp, buf, len);
    tmp[len] = '\0';

    if((strcmp(tmp, "stop") == 0) && monitor_thread) {
        kthread_stop(monitor_thread);
        monitor_thread = NULL;
    }

    if((strcmp(tmp, "start") == 0) && !monitor_thread) {
        monitor_thread = kthread_run(monitor_fn, &monitor, "task_monitor_thread");
        if (IS_ERR(monitor_thread)) {
            printk(KERN_ERR "Failed to create kthread\n");
        }
    }

    return count;
}

static const struct kobj_attribute myattr = __ATTR_RW(taskmonitor);

static unsigned long tm_count(struct shrinker *s, struct shrink_control *sc) {
    unsigned long ret;    
    mutex_lock(&monitor.mtx);
    ret = monitor.count;
    mutex_unlock(&monitor.mtx);
    return ret;
}

static unsigned long tm_scan(struct shrinker *s, struct shrink_control *sc) {
    if (!sc->nr_to_scan) { return 0; }

    struct task_sample *smp, *tmp;
    unsigned long freed = 0;

    mutex_lock(&monitor.mtx);

    list_for_each_entry_safe(smp, tmp, &monitor.head, list) {
        list_del(&smp->list);
        kmem_cache_free(sample_cache, smp);
        monitor.count--;
        freed++;

        if (freed >= sc->nr_to_scan) { break; }
    }

    mutex_unlock(&monitor.mtx);
    printk("Shrinked : %lu", freed);
    return freed;
}

static struct shrinker tm_shrinker = {
    .count_objects = tm_count,
    .scan_objects = tm_scan,
    .seeks = DEFAULT_SEEKS,
};

static int __init hello_init(void) {
    // NB : en cas de crash, on ne désalloue pas tout correctement mais c'est pas grave pour un module trivial comme celui-ci.

    int ret = monitor_pid(pid);
    if (ret) {
        return ret;
    }

    sample_cache = KMEM_CACHE(task_sample, 0);
    if (!sample_cache) {
        printk(KERN_ERR "Failed to create task_sample cache\n");
        return -ENOMEM;
    }

    int retshrinker = register_shrinker(&tm_shrinker, "task_monitor");
    if (retshrinker) {
        return retshrinker;
    }

    int retval = sysfs_create_file(kernel_kobj, &myattr.attr);
    if (retval) {
        return retval;
    }

    monitor_thread = kthread_run(monitor_fn, &monitor, "task_monitor_thread");
    if (IS_ERR(monitor_thread)) {
        printk(KERN_ERR "Failed to create kthread\n");
        put_pid(monitor.pid_struct);
        return PTR_ERR(monitor_thread);
    }

    return 0;
}

module_init(hello_init);

static void __exit hello_exit(void) {
    unregister_shrinker(&tm_shrinker);
    sysfs_remove_file(kernel_kobj, &myattr.attr);

    if (monitor_thread) {
        kthread_stop(monitor_thread);
        monitor_thread = NULL;
    }

    if (monitor.pid_struct) {
        put_pid(monitor.pid_struct);
    }

    struct task_sample *entry, *tmp;
    mutex_lock(&monitor.mtx);
    list_for_each_entry_safe(entry, tmp, &monitor.head, list) {
        list_del(&entry->list);
        kmem_cache_free(sample_cache, entry);
    }
    mutex_unlock(&monitor.mtx);

    kmem_cache_destroy(sample_cache);

    printk(KERN_INFO "Module unloaded\n");
}

module_exit(hello_exit);