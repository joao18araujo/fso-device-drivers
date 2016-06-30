#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define TRUE 1
#define FALSE 0
#define DEVICE_NAME "projeto-3-fso"	
 
#define DEVICE_AUTHOR_1 "Joao Vitor Araujo"
#define DEVICE_AUTHOR_2 "Mateus Manuel"
#define DEVICE_AUTHOR_3 "Victor Matias"

#define DEVICE_DESCRIPTION "Device driver that prints the number of times that has been read"
#define BUF_LEN 256		

static int major;		
static int device_opened = FALSE;
						
static char msg[BUF_LEN];
static char *msg_ptr;

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

 MODULE_LICENSE("GPL");
 MODULE_AUTHOR(DEVICE_AUTHOR_1);
 MODULE_AUTHOR(DEVICE_AUTHOR_2);
 MODULE_AUTHOR(DEVICE_AUTHOR_3);
 MODULE_DESCRIPTION(DEVICE_DESCRIPTION);
