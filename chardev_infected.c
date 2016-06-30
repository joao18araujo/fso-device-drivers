#include "chardev.h"

int init_module(void){
	major = register_chrdev(0, DEVICE_NAME, &fops);

	if (major < 0) {
		printk(KERN_ALERT "Registering char device failed with %d\n", major);
		return major;
	}

	return 0;
}

void cleanup_module(void){
	unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file){
	static int counter = 0;

	if(device_opened)
		return -EBUSY;

	device_opened = TRUE;

	sprintf(msg, "Infected device, opened %d times\n", counter++);
	msg_ptr = msg;
	try_module_get(THIS_MODULE);

	return 0;
}

static int device_release(struct inode *inode, struct file *file){
	device_opened = FALSE;

	module_put(THIS_MODULE);

	return 0;
}

static ssize_t device_read(struct file *filp, char *buffer,	size_t length, loff_t * offset){
	int bytes_read = 0;

	if (*msg_ptr == 0)
		return 0;

	while (length && *msg_ptr){
		put_user(*(msg_ptr++), buffer++);
		
		length--;
		bytes_read++;
	}

	return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off){
	printk(KERN_INFO "[%s][%c][%c]", buff, 0, 0);

	sprintf(msg, "CPF valido\n");
	msg_ptr = msg;
	printk(KERN_INFO "CPF valido\n");

	return strlen(msg);
}
