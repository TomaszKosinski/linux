#include<linux/module.h>
#include<linux/fs.h>        /* alloc_chrdev_region() */
#include<linux/cdev.h>      /* cdev_init(), cdev_add() */
#include<linux/device.h>    /* class_create() device_create() */
#include<linux/kdev_t.h>    /* MAJOR(), MINOR() */
#include<linux/uaccess.h>
#include<linux/platform_device.h>
#include"platform.h"


static int pcd_open(struct inode *inode, struct file *filep)
{
    pr_info("%s", __func__);
    return 0;
}

static loff_t pcd_llseek(struct file *filep, loff_t offset, int whence)
{
    pr_info("%s", __func__);

    return 0;
}

static ssize_t pcd_read(struct file *filep, char __user *buffer, size_t count, loff_t *f_pos)
{
    pr_info("%s %zu bytes", __func__, count);
    return 0;
}

static ssize_t pcd_write(struct file *filep, const char __user *buffer, size_t count, loff_t *f_pos)
{
    pr_info("%s %zu bytes", __func__, count);
    return 0;
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


int pcd_platform_driver_probe(struct platform_device *pdev)
{
    pr_info("device is detected\n");
    return 0;
}
int pcd_platform_driver_remove(struct platform_device *pdev)
{
    pr_info("device is removed\n");
    return 0;
}

struct platform_driver pcd_platform_driver = 
{
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    .driver = {
        .name = "pseudo-char-device"    /* same name as in device setup*/
    }
};


/* __init macro to puc code into init section, which can be cleaed after init is over */
static int __init pcd_platform_driver_init(void)
{
    platform_driver_register(&pcd_platform_driver);
    pr_info("pcd platform driver loaded");
    return 0;
}

static void __exit pcd_paltform_driver_cleanup(void)
{
    platform_driver_unregister(&pcd_platform_driver);
    pr_info("pcd platform driver unloaded");
}


module_init(pcd_platform_driver_init);
module_exit(pcd_paltform_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tomasz Kosinski");
MODULE_DESCRIPTION("A simple pseudo char device (pcd) kernel module");


