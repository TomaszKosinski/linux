#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

static int device_value = 0;
static struct kobject *example_kobject;

static ssize_t device_value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", device_value);
}

static ssize_t device_value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &device_value);
    return count;
}

// cat /sys/kernel/example_device/device_value 
// echo "10" > /sys/kernel/example_device/device_value 
static struct kobj_attribute device_value_attribute = __ATTR(device_value, 0666, device_value_show, device_value_store);

static int __init my_module_init(void)
{
    int error;

    // Tworzymy nowy obiekt kobject w sysfs
    example_kobject = kobject_create_and_add("example_device", kernel_kobj);
    if (!example_kobject)
        return -ENOMEM;

    // Tworzymy atrybut w tym kobject
    error = sysfs_create_file(example_kobject, &device_value_attribute.attr);
    if (error)
        kobject_put(example_kobject);

    return error;
}

static void __exit my_module_exit(void)
{
    // Usuwamy atrybut i kobject po zakończeniu działania modułu
    kobject_put(example_kobject);
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Example of Sysfs Communication");
