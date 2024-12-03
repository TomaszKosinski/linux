#include<linux/module.h>
#include<linux/fs.h>        /* alloc_chrdev_region() */
#include<linux/cdev.h>      /* cdev_init(), cdev_add() */
#include<linux/device.h>    /* class_create() device_create() */
#include<linux/kdev_t.h>    /* MAJOR(), MINOR() */
#include<linux/uaccess.h>

#define NO_OF_DEVICES 4
#define DEV_MEM_SIZE 1024
char device_buffer_pcdev1[DEV_MEM_SIZE];
char device_buffer_pcdev2[DEV_MEM_SIZE];
char device_buffer_pcdev3[DEV_MEM_SIZE];
char device_buffer_pcdev4[DEV_MEM_SIZE];

/* Device private data */
struct pcdev_orivate_data
{
    char *buffer;
    unsigned size;
    const char *serial_number;
    int perm;
    struct cdev cdev;
}

/* driver private data structire */
struct pcdrv_private_data
{
    int total_devices;
    struct pcdev_orivate_data pcdev_data[NO_OF_DEVICES];
}

dev_t device_number;
unsigned base_minor = 0;
unsigned minor_numbers_count = 1;
/* this is not a device name */
char *name_of_device_number = "pcd_devices";

struct cdev pcd_cdev;

static int pcd_open(struct inode *inode, struct file *filep)
{
    pr_info("%s", __func__);
    return 0;
}

static loff_t pcd_llseek(struct file *filep, loff_t offset, int whence)
{
    loff_t temp;

    pr_info("%s", __func__);
    pr_info("Initial file position = %lld\n", filep->f_pos);
    switch(whence)
    {
        case SEEK_SET:
            if (offset > DEV_MEM_SIZE || offset < 0) {
                return -EINVAL;
            }
            filep->f_pos = offset;
            break;
        case SEEK_CUR:
            temp = filep->f_pos + offset;
            if (temp > DEV_MEM_SIZE || temp < 0) {
                return -EINVAL;
            }
            filep->f_pos = temp;
            break;
        case SEEK_END:
            if (temp > DEV_MEM_SIZE || temp < 0) {
                return -EINVAL;
            }
            filep->f_pos = temp;
            break;
        default:
            return -EINVAL;
    }
    pr_info("Updated file position = %lld\n", filep->f_pos);
    return filep->f_pos;
}

static ssize_t pcd_read(struct file *filep, char __user *buffer, size_t count, loff_t *f_pos)
{
    pr_info("%s %zu bytes", __func__, count);
    pr_info("Initial file position = %lld\n", *f_pos);

    /* Adjust the "count" */
    if (*f_pos + count > DEV_MEM_SIZE)
    {
        count = DEV_MEM_SIZE - *f_pos;
    }
    /* copy to user */
    if (copy_to_user(buffer, &device_buffer[*f_pos], count)) {
        return -EFAULT;
    }

    /* update current file position */
    *f_pos += count;

    pr_info("Number of bytes read = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);
    return count;
}

static ssize_t pcd_write(struct file *filep, const char __user *buffer, size_t count, loff_t *f_pos)
{
    pr_info("%s %zu bytes", __func__, count);
    pr_info("Initial file position = %lld\n", *f_pos);

    /* Adjust the "count" */
    if (*f_pos + count > DEV_MEM_SIZE)
    {
        count = DEV_MEM_SIZE - *f_pos;
    }
    if(count == 0) {
        pr_err("No space left on the device\n");
        return -ENOMEM;
    }
    
    /* copy from user */
    if (copy_from_user(&device_buffer[*f_pos], buffer, count)) {
        return -EFAULT;
    }

    /* update current file position */
    *f_pos += count;

    pr_info("Number of bytes written = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);
    return count;
}

static int pcd_release(struct inode *inode, struct file *filep)
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
    int ret;
    
    /* dynamically allocate a device number -> device_number:base_minor...minor_numbers_count-1 */
    ret = alloc_chrdev_region(&device_number, base_minor, minor_numbers_count, name_of_device_number);
    if (ret < 0) {
        goto out;
    }
    /* initialize cdev structure (once for every device) */
    cdev_init(&pcd_cdev, &pcd_fops);
    /* register cdev structure with the kernel VFS */
    pcd_cdev.owner = THIS_MODULE;
    ret = cdev_add(&pcd_cdev, device_number, minor_numbers_count);
    if (ret < 0) {
        goto unreg_chrdev;
    }
    /* create device class under /sys/class */
    class_pcd = class_create("pcd_class");
    if (IS_ERR(class_pcd)) {
        pr_err("Class creation failed \n");
        /* in a case of error, class_create converts error code to an *void address instead of null using ERR_PTR()
        that is why we have to use pointer to error to check the error codeL PTR_ERR */
        ret = PTR_ERR(class_pcd);
        goto cdev_del;
    }
    /* populate sysfs with device information */
    device_pcd = device_create(class_pcd, NULL, device_number, NULL, "pcd");
    if (IS_ERR(device_pcd)) {
        pr_err("device create failed\n");
        ret = PTR_ERR(device_pcd);
        goto class_del;
    }
    pr_info("%s device number created, major:%i minor:%i\n",__func__, MAJOR(device_number), MINOR(device_number));
    return 0;

class_del:
    class_destroy(class_pcd);
cdev_del:
    cdev_del(&pcd_cdev);
unreg_chrdev:
    unregister_chrdev_region(device_number, minor_numbers_count);
out :
    pr_info("Module insertion failed\n");
    return ret;

}

static void __exit pcd_cleanup(void)
{
    /* reverse order of pcd_init */
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


