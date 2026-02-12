#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_DESCRIPTION("Hello World module");
MODULE_AUTHOR("Nicolas");
MODULE_LICENSE("GPL");

static char *whom = "";
module_param(whom, charp, 0660);
MODULE_PARM_DESC(whom, "Name of the person to greet");

static int howmany = 1;
module_param(howmany, int, 0000);
MODULE_PARM_DESC(howmany, "Number of times to greet");

static int __init hello_init(void) {
    for (int i = 0; i < howmany; i++) {
        pr_info("Hello ! %s\n", whom);
    }

    return 0;
}

module_init(hello_init);

static void __exit hello_exit(void) {
        for (int i = 0; i < howmany; i++) {
        pr_info("Goodbye ! %s\n", whom);
    }
}

module_exit(hello_exit);