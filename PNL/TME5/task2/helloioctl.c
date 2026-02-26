
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#include "helloioctl.h"

MODULE_DESCRIPTION("Hello ioctl module");
MODULE_AUTHOR("Nicolas");
MODULE_LICENSE("GPL");

static unsigned int major;

static long my_fun(struct file *file, unsigned int cmd, unsigned long param) {
    struct hello_data my_data;
    int rc = 0;

    if (cmd == HELLO) {
        strncpy(my_data.message, "Hello ioctl!", sizeof(my_data.message));
        my_data.message[sizeof(my_data.message)-1] = '\0';

        rc = copy_to_user((void __user *)param, &my_data, sizeof(my_data));
        if (rc > 0) {
            return -EFAULT;
        }
    } else {
        return -ENOTTY;
    }

    return 0;
}

static const struct file_operations fops = {
    .unlocked_ioctl = my_fun
};

static int __init hello_init(void) {
    major = register_chrdev(0, "hello", &fops);
    printk("%u\n", major);
    if (major < 0) {
        return major;
    }

    return 0;
}

module_init(hello_init);

static void __exit hello_exit(void) {
    unregister_chrdev(major, "hello");
}

module_exit(hello_exit);