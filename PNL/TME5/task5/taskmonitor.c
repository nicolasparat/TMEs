
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched/task.h>
#include <linux/pid.h>

#define MAX_LEN 32

MODULE_DESCRIPTION("Hello ioctl module");
MODULE_AUTHOR("Nicolas");
MODULE_LICENSE("GPL");

static int pid;
module_param(pid, int, 0660);
MODULE_PARM_DESC(pid, "PID to monitor");

struct task_monitor {
    struct pid *pid_struct; 
};

struct task_sample {
    u64 utime;
    u64 stime;
};

static struct task_monitor monitor;
static struct task_sample sample;
static struct task_struct *monitor_thread;

int monitor_pid(pid_t my_pid) {
    struct pid * pid_struct = find_get_pid(my_pid);
    if (!pid_struct) {
        printk(KERN_INFO "PID %d not found\n", my_pid);
        return -ESRCH;
    }

    monitor.pid_struct = pid_struct;
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
    }
    
    put_task_struct(tasks);
    return is_alive;
}

int monitor_fn(void *arg) {
    // NB : Ce serait plus propre d'utiliser la valeur de arg plutôt que le monitor global, mais le résultat est le même (et j'ai la flemme).
    while (!kthread_should_stop()) {
        if(get_sample(&monitor, &sample)) {
            printk(KERN_INFO "pid %d usr %llu sys %llu\n", pid, (unsigned long long)sample.utime, (unsigned long long)sample.stime);
        }
        
        ssleep(1);
    }

    return 0;
}

static ssize_t taskmonitor_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    get_sample(&monitor, &sample);
    return snprintf(buf, PAGE_SIZE, "pid %d usr %llu sys %llu\n", pid, (unsigned long long)sample.utime, (unsigned long long)sample.stime);
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

static int __init hello_init(void) {
    int ret = monitor_pid(pid);
    if (ret) {
        return ret;
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
    sysfs_remove_file(kernel_kobj, &myattr.attr);

    if (monitor_thread) {
        kthread_stop(monitor_thread);
        monitor_thread = NULL;
    }

    if (monitor.pid_struct) {
        put_pid(monitor.pid_struct);
    }

    printk(KERN_INFO "Module unloaded\n");
}

module_exit(hello_exit);