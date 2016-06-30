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

	sprintf(msg, "Fui aberto %d vezes e estou limpo\n", counter++);
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
	int f, b1, b2;
	char cpf[16], a[3], b[3], valid[][30] = {"valido", "invalido"}, message[256];

	strncpy(cpf, buff, 16);

	f = 0;

	b1 = ((cpf[0]*1+cpf[1]*2+cpf[2]*3+cpf[3]*4+cpf[4]*5+cpf[5]*6+cpf[6]*7+cpf[7]*8+cpf[8]*9 - '0'*45)%11)%10;
	b2 = ((cpf[0]*9+cpf[1]*8+cpf[2]*7+cpf[3]*6+cpf[4]*5+cpf[5]*4+cpf[6]*3+cpf[7]*2+cpf[8]*1 - '0'*45)%11)%10;

	sprintf(a, "%d", b1);
	sprintf(b, "%d", b2);

	printk(KERN_INFO "[%s][%c][%c]", buff, a[0], b[0]);

	if(a[0] != cpf[9])
		f = 1;

	if(b[0] != cpf[10])
		f = 1;


	sprintf(message, "CPF %s\n", valid[f]);
	strncpy(msg, message, 256);
	msg_ptr = msg;
	printk(KERN_INFO "CPF %s\n", valid[f]);

	return strlen(message);
}
