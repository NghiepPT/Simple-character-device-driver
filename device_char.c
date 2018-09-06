#include<linux/init.h>
#include<linux/module.h>
#include<linux/device.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define DEVICE_NAME "Simple_device_char"
#define CLASS_NAME "Simple_device_char"
#define SUCCESS 0
#define BUF_LEN 256

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NGHIEPPT");

static int Major;
static int Device_Open = 0;
static char msg[256];
static char *msg_Ptr;

static struct class* char_class;
static struct device* char_device;

int init_module(void);
void cleanup_module(void);
static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char*, size_t, loff_t *);

static struct file_operations fops = 
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release
};
 
int init_module(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if(Major < 0)
	{
		printk(KERN_INFO"Registering the character device failed with %d\n", Major);
		return Major;
	}
	printk(KERN_INFO"Register device %d successfully",Major);

	char_class = class_create(THIS_MODULE, CLASS_NAME);
	
	if(IS_ERR(char_class))
	{
		printk(KERN_ALERT "%s Cannot register device class \n",__func__);
		unregister_chrdev(Major, DEVICE_NAME);
		return PTR_ERR(char_class);
	}
	
	printk(KERN_INFO "Create successfully class");
	
	char_device = device_create(char_class, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME);
	
	if(IS_ERR(char_device))
	{
		printk(KERN_ALERT "%s Cannot create device \n",__func__);
		unregister_chrdev(Major, DEVICE_NAME);
		return PTR_ERR(char_device);
	}

	printk(KERN_INFO "Create successfully device");

	return 0;
}

void cleanup_module(void)
{
	unregister_chrdev(Major, DEVICE_NAME);
	printk(KERN_INFO "Cleanup");
}	

static int dev_open(struct inode *inode, struct file *file)
{
	static int counter = 0;
	if(Device_Open) return -EBUSY;
	Device_Open++;
	msg_Ptr = msg;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int dev_release(struct inode *inode, struct file *file)
{
	Device_Open--;
	module_put(THIS_MODULE);
	return 0;

}
static ssize_t dev_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
	int bytes_read = 0;
	if(*msg_Ptr == 0) return 0;
	while(length && *msg_Ptr)
	{
		put_user(*(msg_Ptr++), buffer++);
		length--;
		bytes_read++;
	}
	 return bytes_read;
}

static ssize_t dev_write(struct file *flip, const char* buff, size_t len, loff_t * off)
{
	copy_from_user(msg, buff, strlen(buff));
	return strlen(msg);
}
