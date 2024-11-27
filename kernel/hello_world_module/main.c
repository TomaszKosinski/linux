#include<linux/module.h>

/* __init macro to puc code into init section, which can be cleaed after init is over */
static int __init helloworld_init(void)
{
   pr_info("Hello world\n");
   return 0;
}

static void __exit helloworld_cleanup(void)
{
  pr_info("Good bye world\n");
}

module_init(helloworld_init);
module_exit(helloworld_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tomasz Kosinski");
MODULE_DESCRIPTION("A simple hello world kernel module");
