#include <linux/module.h>
#include <linux/major.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");

static int sk_major =0, sk_minor=0;
static int result;
static dev_t sk_dev;

//static struct file_operations sk_fops;
static struct cdev sk_cdev;

static int sk_register_cdev(void);

static int sk_open(struct inode *inode, struct file *filp);
static int sk_release(struct inode *inode, struct file *filp); 

static int sk_open(struct inode *inode, struct file *filp)
{
	printk("Device has been opened...\n");
	return 0;
}

static int sk_release(struct inode *inode, struct file *filp)
{
	printk("Device has been closed...\n");
	return 0;
}

struct file_operations sk_fops={
	.open = sk_open,
	.release = sk_release,
};

static int __init sk_init(void)
{
	printk("SK Module is up...\n");
	if((result = sk_register_cdev())<0)
	{
	return result;
	}
	return 0;
}

static void __exit sk_exit(void)
{
	printk("The module is down...\n");
	
	cdev_del(&sk_cdev);
	unregister_chrdev_region(sk_dev,1);
}

static int sk_register_cdev(void)
{
	int error;

	if(sk_major){
	sk_dev= MKDEV(sk_major, sk_minor);
	error = register_chrdev_region(sk_dev, 1, "sk");
	}
	else{
	error = alloc_chrdev_region(&sk_dev, sk_minor, 1, "sk");
	sk_major = MAJOR(sk_dev);
	}
	
	if(error<0){
	printk(KERN_WARNING "sk:cant't get major %d\n",sk_major);
	return result;
	}

	printk("major number=%d\n", sk_major);
	
	cdev_init(&sk_cdev, &sk_fops);
	sk_cdev.owner = THIS_MODULE;
	sk_cdev.ops = &sk_fops;
	error = cdev_add(&sk_cdev, sk_dev, 1);

	if(error)
	printk(KERN_NOTICE "sk Register Error %d\n", error);
	
	return 0;
}

module_init(sk_init);
module_exit(sk_exit);

