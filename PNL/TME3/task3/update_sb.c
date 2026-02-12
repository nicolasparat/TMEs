#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ktime.h>

MODULE_DESCRIPTION("Hello World module");
MODULE_AUTHOR("Nicolas");
MODULE_LICENSE("GPL");

static char *partition_name = "";
module_param(partition_name, charp, 0660);
MODULE_PARM_DESC(partition_name, "Type of partition to display");

static void print_infos(struct super_block *sb, void *data) {
    if (!sb) return;

    printk("uuid=%pU type=%s time=%lld\n", &sb->s_uuid, sb->s_type->name, sb->s_custom_time);

    // Update timestamp
    ktime_t now = ktime_get();
    sb->s_custom_time = now;
}

static int __init hello_init(void) {
    struct file_system_type *fst = get_fs_type(partition_name);
    if (!fst) {
        return -ENOENT;
    }

    iterate_supers_type(fst, print_infos, NULL);

    // Must be paired with get_fs_type(), similar to free() with malloc()
    put_filesystem(fst);

    return 0;
}

module_init(hello_init);

static void __exit hello_exit(void) {
    pr_info("Goodbye, cruel world\n");
}

module_exit(hello_exit);