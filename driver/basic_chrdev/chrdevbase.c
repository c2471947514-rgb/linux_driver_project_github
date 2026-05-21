#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define CHRDEVBASE_NAME "chrdevbase"

/* 设备号 */
static dev_t dev_num;

/* class / device */
static struct class *chrdev_class;
static struct device *chrdev_device;

/* buffer */
static char readbuf[100];
static char writebuf[100];
static char kerneldata[] = "kernel data!";

/* file_operations 先声明 */
static int chrdevbase_open(struct inode *inode, struct file *filp);
static ssize_t chrdevbase_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt);
static ssize_t chrdevbase_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt);
static int chrdevbase_release(struct inode *inode, struct file *filp);

/* file_operations */
static struct file_operations chrdevbase_fops = {
    .owner   = THIS_MODULE,
    .open    = chrdevbase_open,
    .read    = chrdevbase_read,
    .write   = chrdevbase_write,
    .release = chrdevbase_release,
};

/* open */
static int chrdevbase_open(struct inode *inode, struct file *filp)
{
    printk("chrdevbase open\n");
    return 0;
}

/* read */
static ssize_t chrdevbase_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    int ret;

    memcpy(readbuf, kerneldata, sizeof(kerneldata));

    ret = copy_to_user(buf, readbuf, cnt);

    if (ret == 0) {
        printk("kernel send ok\n");
    } else {
        printk("kernel send failed\n");
    }

    return cnt;
}

/* write */
static ssize_t chrdevbase_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
    int ret;

    ret = copy_from_user(writebuf, buf, cnt);

    if (ret == 0) {
        printk("kernel recv: %s\n", writebuf);
    } else {
        printk("kernel recv failed\n");
    }

    return cnt;
}

/* release */
static int chrdevbase_release(struct inode *inode, struct file *filp)
{
    printk("chrdevbase release\n");
    return 0;
}

/* init */
static int __init chrdevbase_init(void)
{
    int ret;

    printk("chrdevbase init\n");

    /* 注册字符设备 */
    ret = register_chrdev(239, CHRDEVBASE_NAME, &chrdevbase_fops);
    if (ret < 0) {
        printk("register chrdev failed\n");
        return ret;
    }

    dev_num = MKDEV(239, 0);

    /* 创建类 */
    chrdev_class = class_create(CHRDEVBASE_NAME);
    if (IS_ERR(chrdev_class)) {
        unregister_chrdev(239, CHRDEVBASE_NAME);
        return PTR_ERR(chrdev_class);
    }

    /* 创建设备 */
    chrdev_device = device_create(chrdev_class,
                                  NULL,
                                  dev_num,
                                  NULL,
                                  CHRDEVBASE_NAME);

    if (IS_ERR(chrdev_device)) {
        class_destroy(chrdev_class);
        unregister_chrdev(239, CHRDEVBASE_NAME);
        return PTR_ERR(chrdev_device);
    }

    printk("device create ok\n");

    return 0;
}

/* exit */
static void __exit chrdevbase_exit(void)
{
    device_destroy(chrdev_class, dev_num);
    class_destroy(chrdev_class);

    unregister_chrdev(239, CHRDEVBASE_NAME);

    printk("chrdevbase exit\n");
}

module_init(chrdevbase_init);
module_exit(chrdevbase_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cerole");
MODULE_DESCRIPTION("Simple chrdev driver (Linux 6.x)");


