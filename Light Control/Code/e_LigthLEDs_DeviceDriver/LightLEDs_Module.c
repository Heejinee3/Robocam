// 	Template of Module Program LightLEDs_Module.c
//
// A simple character driver with two custom LEDs for Light Control
// utilizing GPIO0_30 & GPIO0_31.
//

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <asm/io.h>

#include <linux/init.h>
#include <linux/device.h>
#include <linux/semaphore.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/moduleparam.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/slab.h>

// AM33xx specific register definitions
#include "./am33xx.h"
#include "./gpio.h"

#define LightLEDs_MAJOR	0  	// Auto select major number

// LightLEDs File operations table
struct file_operations LightLEDs_fops;

// LightLEDs usage flag
static int LightLEDs_usage      = 0;
int major;		// Device major number in init/cleamup_module

// Global data
// GPIO0 physical/virtual address
unsigned long gpio0_start = AM33XX_GPIO0_BASE;	// GPIO0 Start address
unsigned long gpio0_len = 0x1000;		// Length of GPIO0 addresses
void __iomem * gpio0_vbase = NULL;		// GPIO0 base virtual address
int gpio0_chk;


// 1. LightLEDs init_module
static int __init LightLEDs_init_module (void)
{
	// A. Register LightLEDs as a character device 
	major = register_chrdev( LightLEDs_MAJOR, "LightLEDs", &LightLEDs_fops );
	if (major < 0) {
		printk(KERN_WARNING "Error registering LightLEDs fops %d\n", major);
		return major;
	}
	printk("    A. Init LightLEDs: The major number is %d\n", major);

	// Return success
	return 0; 
}


// 2. LightLEDs open function for open() call
static int LightLEDs_open(struct inode *inode, struct file *filp)
{
	unsigned int oe0, oe0new;
	unsigned int state, state1;

// B0. Check & set LightLEDs GPIO0 usage flag
	if( LightLEDs_usage != 0 ) return -EBUSY;
  	LightLEDs_usage = 1;    
	
// memory mapping for GPIO0 control
	// B1. Check memory region and then request - GPIO0
	gpio0_chk = check_mem_region(gpio0_start, gpio0_len);
	if (gpio0_chk < 0) {
		printk("  B1. Warning: LightLEDs GPIO0 check_mem_region failed...\n");
		//- return(result);	// Just warn & continue.
	} else {
		request_mem_region (gpio0_start, gpio0_len, "gpio0");
		// printk("  B1. GPIO0 request_mem_region.\n");
	}

	// B2. Physical to virtual memory mapping - GPIO0
	gpio0_vbase = ioremap(gpio0_start, gpio0_len);
	if (!gpio0_vbase) {
		printk("  B2. LightLEDs GPIO0 ioremap failed.\n");
		// -B1. Release memory region
		if (gpio0_chk >= 0) 
			release_mem_region (gpio0_start, gpio0_len);

		return(-2);
	}
	// printk("  B2. LightLEDs open: ioremap gpio0_vbase= %08x\n", (unsigned int)gpio0_vbase);

// C. Set MUXes
	// Assumed to be done by Kernel correctly.

// D. Set Directions of GPIO 30 & 31 to output
	// First set GPIO0 30 - 31 direction to output ('0')
	// Fill-in read, modify, and write OMAP4_GPIO_OE

	state = ioread32(ioremap(gpio0_start+OMAP4_GPIO_OE,gpio0_len));
	state1 = (state)&(~(1<<31 | 1<<30));
	iowrite32(state1, ioremap(gpio0_start+OMAP4_GPIO_OE,gpio0_len));
	printk("  D. LightLEDs opened.\n");

	return 0;           
}


// 3. LightLEDs_write function for write() call
static ssize_t LightLEDs_write (struct file *filp, const char *wbuf, size_t wcount, loff_t *f_pos)
{
	unsigned int mdata;
	unsigned int oe0, oe0new;
	unsigned int dout0, dout0new;
	
	// E. Get user_data from app program
	get_user(mdata, wbuf);		// Args (To var, Src addr)
	printk("%d\n", mdata); 
	// F. Read GPIO0, modify, and write bits 30 & 31 according to user_data
	// Fill-in read, modify, and write OMAP4_GPIO_DATAOUT
	dout0 = ioread32(gpio0_vbase+OMAP4_GPIO_DATAOUT);
	dout0new = dout0 & (~(1<<31 | 1<<30));
	iowrite32(dout0new, gpio0_vbase+OMAP4_GPIO_DATAOUT);
	dout0 = ioread32(gpio0_vbase+OMAP4_GPIO_DATAOUT);
	if (mdata == 0){
		dout0new = dout0 & ~(1 << 30 | 1 << 31);
	}
	else if (mdata == 1){
		dout0new = dout0 | (1 << 30);
	}
	else if (mdata == 2){
		dout0new = dout0 | (1 << 31);
	}
	else if (mdata == 3){
		dout0new = dout0 | (1 << 30) | (1 << 31);
	}
	iowrite32(dout0new, gpio0_vbase+OMAP4_GPIO_DATAOUT); 
	// Can check printout via dmesg
	printk("LightLEDs GPIO0_DOUT: %08x to %08x\n", dout0, dout0new);

  	return (0);
}



// 4. LightLEDs release function for release() call
static int LightLEDs_release(struct inode *inode, struct file *filp)
{
	unsigned int oe0, oe0new;
	unsigned int state, state1; 
// -D. Set Directions of GPIO 30 & 31 to input
	// First set GPIO0 30 - 31 direction to input ('1')
	// Fill-in read, modify, and write OMAP4_GPIO_OE
	state = ioread32(gpio0_vbase+OMAP4_GPIO_OE);
	state1 = (state) | (1<<31 | 1<<30);
	iowrite32(state1, gpio0_vbase+OMAP4_GPIO_OE); 
// -C. ReSet MUXes
	// None since Mux are set by Kernel.

// -Reverse handling GPIO0 memory map
	// -B2. Unmap ioaddress
	iounmap(gpio0_vbase);
	// -B1. Release memory region
	if (gpio0_chk >= 0)
		release_mem_region (gpio0_start, gpio0_len);

// -B0. Clear GPIO0 usage flag
	LightLEDs_usage = 0;             

	printk("  -D. LightLEDs released.\n");

	return 0;
}


// 5. LightLEDs cleanup_module
static void __exit LightLEDs_cleanup_module (void)
{
	// -A. Unregister LightLEDs device
	unregister_chrdev( LightLEDs_MAJOR, "LightLEDs" ); 
	printk("  -A. Cleanup LightLEDs: Unregistered.\n");
}

// Define file operations table for LightLEDs driver: After functions!
struct file_operations LightLEDs_fops = 
{
	owner:		THIS_MODULE,
	open:		LightLEDs_open,
	write:		LightLEDs_write,
	release:	LightLEDs_release,
};


// Define module init and cleanup functions
module_init(LightLEDs_init_module);
module_exit(LightLEDs_cleanup_module);

MODULE_AUTHOR("Byung Kook Kim");
MODULE_DESCRIPTION("LightLEDs Driver");
MODULE_LICENSE("GPL");

