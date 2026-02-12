#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/utsname.h>
#include <asm/segment.h>
#include <asm/paravirt.h>

MODULE_DESCRIPTION("Hello World module");
MODULE_AUTHOR("Nicolas");
MODULE_LICENSE("GPL");

char my_release[__NEW_UTS_LEN];

static int __init hello_init(void) {
    printk(KERN_INFO "Original uname: %s\n", init_uts_ns.name.release);

    // Copy old version number
    strncpy(my_release, init_uts_ns.name.release, __NEW_UTS_LEN - 1);

    // Change version number
    strncpy(init_uts_ns.name.release, "6.5.7-modified", __NEW_UTS_LEN - 1);
    init_uts_ns.name.release[__NEW_UTS_LEN-1] = '\0';

    printk(KERN_INFO "New uname: %s\n", init_uts_ns.name.release);

    return 0;
}

module_init(hello_init);

static void __exit hello_exit(void) {
    // Restore version number
    strncpy(init_uts_ns.name.release, my_release, __NEW_UTS_LEN - 1);
    init_uts_ns.name.release[__NEW_UTS_LEN-1] = '\0';

    printk(KERN_INFO "Restored uname: %s\n", init_uts_ns.name.release);
}

module_exit(hello_exit);