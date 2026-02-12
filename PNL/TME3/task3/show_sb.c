#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

MODULE_DESCRIPTION("Hello World module");
MODULE_AUTHOR("Nicolas");
MODULE_LICENSE("GPL");

static void print_infos(struct super_block *sb, void *data) {
    if (!sb) return;
    printk("uuid=%pU type=%s\n", &sb->s_uuid, sb->s_type->name);
}

static int __init hello_init(void) {
    iterate_supers(print_infos, NULL);
    return 0;
}

module_init(hello_init);