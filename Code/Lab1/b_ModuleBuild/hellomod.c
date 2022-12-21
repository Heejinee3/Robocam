#include <linux/module.h> /* Needed by all modules */
static int __init hellomod_init(void) // init_module()
{
	printk("Hello, hellomod!\n");
	return 0;
}
static void __exit hellomod_exit(void) // cleanup_module()
{
	printk("Goodbye, hellomod!\n");
}
module_init(hellomod_init);
module_exit(hellomod_exit);
MODULE_AUTHOR("Christoper Hallinan");
MODULE_DESCRIPTION("Hello Module Example");
MODULE_LICENSE("GPL");
