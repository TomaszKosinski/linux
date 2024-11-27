#include<linux/module.h>
#include<linux/fs.h>        // alloc_chrdev_region()
#include<linux/cdev.h>      // cdev_init(), cdev_add()
#include<linux/device.h>    // class_create() device_create()
#include<linux/kdev_t.h>    // MAJOR(), MINOR()

#define DEV_MEM_SIZE 1024
char device_buffer[DEV_MEM_SIZE];

dev_t device_number;
unsigned base_minor = 0;
unsigned minor_numbers_count = 1;
// this is not a device name
char *name_of_device_number = "pcd_devices";

struct cdev pcd_cdev;

int pcd_open(struct inode *inode, struct file *filep)
{
    pr_info("%s", __func__);
    return 0;
}

loff_t pcd_llseek(struct file *filep, loff_t offset, int whence)
{
    pr_info("%s", __func__);
    return 0;
}

ssize_t pcd_read(struct file *filep, char __user *buffer, size_t count, loff_t *f_pos)
{
    pr_info("%s %zu bytes", __func__, count);
    return 0;
}

ssize_t pcd_write(struct file *filep, const char __user *buffer, size_t count, loff_t *f_pos)
{
    pr_info("%s %zu bytes", __func__, count);
    return 0;
}

int pcd_release(struct inode *inode, struct file *filep)
{
    pr_info("%s", __func__);
    return 0;
}

struct file_operations pcd_fops = 
{
    .open = pcd_open,
    .write = pcd_write,
    .read = pcd_read,
    .llseek = pcd_llseek,
    .release = pcd_release,
    .owner = THIS_MODULE
};

struct class *class_pcd;
struct device *device_pcd;

/* __init macro to puc code into init section, which can be cleaed after init is over */
static int __init pcd_init(void)
{
    // dynamically allocate a device number
    alloc_chrdev_region(&device_number, base_minor, minor_numbers_count, name_of_device_number); // device_number:base_minor...minor_numbers_count-1
    // initialize cdev structure (once for every device)
    cdev_init(&pcd_cdev, &pcd_fops);
    // register cdev structure with the kernel VFS
    pcd_cdev.owner = THIS_MODULE;
    cdev_add(&pcd_cdev, device_number, minor_numbers_count);
    // create device class under /sys/class
    class_pcd = class_create(THIS_MODULE, "pcd_class");
    // populate sysfs with device information
    device_pcd = device_create(class_pcd, NULL, device_number, NULL, "pcd");
    
    pr_info("%s device number created, major:%i minor:%i\n",__func__, MAJOR(device_number), MINOR(device_number));
    return 0;
}

static void __exit pcd_cleanup(void)
{
    // reverse order of pcd_init
    device_destroy(class_pcd, device_number);
    class_destroy(class_pcd);
    cdev_del(&pcd_cdev);
    unregister_chrdev_region(device_number, minor_numbers_count);
    pr_info("%s Module unloaded\n",__func__);
}

module_init(pcd_init);
module_exit(pcd_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tomasz Kosinski");
MODULE_DESCRIPTION("A simple pseudo char device (pcd) kernel module");


