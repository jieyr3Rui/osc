#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>

struct typedef{
    int day;
    int month;
    int year;
    struct list_head list;
}birthday;

static LIST_HEAD(birthday_list);
birthday *persion;
persion = kmalloc(sizeof(birthday), GFP_KERNEL);
persion->day = 2;
persion->month = 8;
persion->year = 1995;
INIT_LIST_HEAD(&persion->list);

struct birthdat *ptr, *next;


/* this function is called when the module is loaded. */
int birthday_init(void){
    printk(KERN_INFO "birthday module loading...\n");    
    return 0;
}

/* this function is called when the module is removed. */
void birthday_exit(void){
    printk(KERN_INFO "birthday module removing...\n");
}

/* Macro for registering module entry and exit points */
module_init(birthday_init);
module_exit(birthday_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("birthday module");
MODULE_AUTHOR("SGG");

