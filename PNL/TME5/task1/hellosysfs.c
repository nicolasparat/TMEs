#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_DESCRIPTION("Hello sysfs module");
MODULE_AUTHOR("Nicolas");
MODULE_LICENSE("GPL");

#define MAX_LEN 32
static char custom_val[MAX_LEN] = "sysfs";

static ssize_t hello_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return snprintf(buf, PAGE_SIZE, "Hello %s !\n", custom_val);
}

static ssize_t hello_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    size_t len;

    /* Remove trailing newline if present */
    len = strcspn(buf, "\n");

    if (len >= MAX_LEN)
        return -EINVAL;

    strscpy(custom_val, buf, len + 1);

    return count;
}

static const struct kobj_attribute myattr = __ATTR_RW(hello);

static int __init hello_init(void) {
    int retval = sysfs_create_file(kernel_kobj, &myattr.attr);
    return retval;
}

module_init(hello_init);

static void __exit hello_exit(void) {
    sysfs_remove_file(kernel_kobj, &myattr.attr);
}

module_exit(hello_exit);