
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/dcache.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define BUF_SIZE 128

extern struct hlist_bl_head *dentry_hashtable;
extern unsigned int d_hash_shift;

MODULE_DESCRIPTION("Weasel module");
MODULE_AUTHOR("Nicolas");
MODULE_LICENSE("GPL");

static ssize_t whoami_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    char message[] = "I'm a weasel!\n";
    return simple_read_from_buffer(buf, count, ppos, message, sizeof(message));
}

static const struct proc_ops proc_file_ops = {
    .proc_read = whoami_read,
};

static int __init weasel_init(void) {
    struct proc_dir_entry *dir;

    dir = proc_mkdir("weasel", NULL);
    proc_create("whoami", 0, dir, &proc_file_ops);

    printk(KERN_INFO "Weasel loaded\n");
    printk(KERN_INFO "dentry_hashtable address: %px\n", dentry_hashtable);
    printk(KERN_INFO "Hash size: %u\n", 1 << d_hash_shift);

    unsigned int i;
    unsigned long total = 0;
    unsigned long max_len = 0;

    for (i = 0; i < (1 << d_hash_shift); i++) {
        struct hlist_bl_head *head = &dentry_hashtable[i];
        struct dentry *d;
        unsigned long len = 0;

        // Checker que cette fonction existe
        hlist_bl_for_each_entry(d, head, d_hash) {
            len++;
        }

        total += len;
        if (len > max_len)
            max_len = len;
    }

    printk(KERN_INFO "Total dentries: %lu\n", total);
    printk(KERN_INFO "Max bucket length: %lu\n", max_len);

    return 0;
}

module_init(weasel_init);

static void __exit weasel_exit(void) {
    printk(KERN_INFO "Weasel unloaded\n");
}

module_exit(weasel_exit);