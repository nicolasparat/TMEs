
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched/task.h>
#include <linux/pid.h>

MODULE_DESCRIPTION("Hello ioctl module");
MODULE_AUTHOR("Nicolas");
MODULE_LICENSE("GPL");

static int pid;
module_param(pid, int, 0660);
MODULE_PARM_DESC(pid, "PID to monitor");

struct task_monitor {
    struct pid *pid_struct; 
};

static struct task_monitor monitor;
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

int monitor_fn(void *arg) {
    while (!kthread_should_stop()) {
        struct task_struct *tasks = get_pid_task(monitor.pid_struct, PIDTYPE_PID);
        if (!tasks) break;
        
        if(pid_alive(tasks)) {
            printk(KERN_INFO "pid %d usr %llu sys %llu\n", pid, (unsigned long long)tasks->utime, (unsigned long long)tasks->stime);
        }
        
        put_task_struct(tasks);
        ssleep(1);
    }

    return 0;
}

static int __init hello_init(void) {
    int ret = monitor_pid(pid);
    if (ret) {
        return ret;
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
    if (monitor_thread) {
        kthread_stop(monitor_thread);
    }

    if (monitor.pid_struct) {
        put_pid(monitor.pid_struct);
    }

    printk(KERN_INFO "Module unloaded\n");
}

module_exit(hello_exit);