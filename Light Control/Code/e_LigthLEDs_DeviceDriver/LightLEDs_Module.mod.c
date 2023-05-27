#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xae4b0ceb, "module_layout" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xcd7317e5, "__register_chrdev" },
	{ 0x28118cb6, "__get_user_1" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0xadf42bd5, "__request_region" },
	{ 0xb8aa2342, "__check_region" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x27e1a049, "printk" },
	{ 0x9bce482f, "__release_region" },
	{ 0x788fe103, "iomem_resource" },
	{ 0xc2165d85, "__arm_iounmap" },
	{ 0xac8f37b2, "outer_cache" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "78E055447AA76FC9C26FB64");
