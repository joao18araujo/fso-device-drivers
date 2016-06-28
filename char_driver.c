#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "projeto-fso"

typedef struct _fake_device {
	char data[100];
	struct semaphore sem;
} fake_device;

struct cdev * mcdev;

int major_number;
int ret;

dev_t dev_num;

int device_open(struct inode * inode, struct file *filp){
	if(down_interruptible(&virtual_device.sem) != 0){
		printk(KERN_ALERT "Could not lock device during open\n");
		return -1;
	}

	printk(KERN_INFO "Device opened\n");
	return 0;
}
ssize_t device_read(struct file * filp, char * buffer_store_data, size_t buffer_count, loff_t * cur_offset){
	printk(KERN_INFO "Reading from device\n");
}


int device_close(){

}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_close,
	.write = device_write,
	.read = device_read
};

static int driver_entry(void){
	ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if(ret < 0){
		printk(KERN_ALERT "Alloc fail\n");
		return ret;
	}

	major_number = MAJOR(dev_num);
	printk(KERN_INFO "Major number is %d\n", major_number);
	
	mcdev = cdev_alloc();
	mcdev->ops = &fops;
	mcdev->owner = THIS_MODULE;
	
	ret = cdev_add(mcdev, dev_num, 1);
	if(ret < 0){
		printk(KERN_ALERT "Unable to add cdev to kernel\n");
		return ret;
	}

	sema_init(&virtual_device.sem, 1);

	return 0;
}
































