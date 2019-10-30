#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#define BUFFER_SIZE 128
#define PROC_NAME "pid"

ssize_t proc_write(struct file *file, char __user *usr_buf, 
        size_t count, loff_t *pos){
    int rv = 0;
    char *k_mem;
    k_mem = kmalloc(count, GFP_KERNEL);
    copy_from_user(k_mem, usr_buf, count);
    printk(KERN_INFO "%s\n", k_mem);
    kfree(k_mem);

    return count;
}
ssize_t proc_read(struct file *file, char __user *usr_buf, 
        size_t count, loff_t *pos){
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if(completed){
        completed = 0;
        return 0;
    }

    completed = 1;

    rv = sprintf(buffer, "pid proc Hello\n");
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
    .write = proc_write,
};

int proc_init(void){
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    return 0;
}

void proc_exit(void){
    remove_proc_entry(PROC_NAME, NULL);
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("pid module");
MODULE_AUTHOR("SSG");



