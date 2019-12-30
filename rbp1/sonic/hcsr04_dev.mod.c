#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xf230cadf, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xe549e5f5, "__register_chrdev" },
	{ 0x91715312, "sprintf" },
	{ 0xb43f9365, "ktime_get" },
	{ 0x8e865d3c, "arm_delay_ops" },
	{ 0xedc03953, "iounmap" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xe97c4103, "ioremap" },
	{ 0x7c32d0f0, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "6133DE236B4EB2C666D0692");