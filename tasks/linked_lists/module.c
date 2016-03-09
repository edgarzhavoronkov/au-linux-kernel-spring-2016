#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/errno.h>

#include "stack.h"
#include "assert.h"

static void __init test_stack(void)
{
    LIST_HEAD(data_stack);
    stack_entry_t *tos = NULL;
    const char *tos_data = NULL;
    const char* test_data[] = { "1", "2", "3", "4" };
    long i = 0;

    pr_alert("Testing basic stack");

    for (i = 0; i != ARRAY_SIZE(test_data); ++i) {
        stack_push(&data_stack,
            create_stack_entry((void*)test_data[i])
        );
    }

    for (i = ARRAY_SIZE(test_data) - 1; i >= 0; --i) {
        tos = stack_pop(&data_stack);
        tos_data = STACK_ENTRY_DATA(tos, const char*);
        delete_stack_entry(tos);
        printk(KERN_ALERT "%s == %s\n", tos_data, test_data[i]);
        assert(!strcmp(tos_data, test_data[i]));
    }

    assert(stack_empty(&data_stack));
}

static int __init print_processes_backwards(void)
{
    struct task_struct* task;
    LIST_HEAD(tasks_stack);
    stack_entry_t *entry = NULL;
    char *filename = NULL;
    int result = 0;

    for_each_process(task)
    {
        filename = (char*)kmalloc(sizeof(task->comm), GFP_KERNEL);
        if (!filename)
        {
            result = -ENOMEM;
            break;
        }
        get_task_comm(filename, task);

        entry = create_stack_entry((void*)filename);
        if (!entry)
        {
            kfree(filename);
            result = -ENOMEM;
            break;
        }

        stack_push(&tasks_stack, entry);
    }

    while (!stack_empty(&tasks_stack))
    {
        entry = stack_pop(&tasks_stack);
        filename = STACK_ENTRY_DATA(entry, char*);
        printk("%s \n", filename);
        delete_stack_entry(entry);
        kfree(filename);
    }

    return result;
}

static int __init ll_init(void)
{
    printk(KERN_ALERT "Hello, linked_lists\n");
    test_stack();
    print_processes_backwards();
    return 0;
}

static void __exit ll_exit(void)
{
    printk(KERN_ALERT "Goodbye, linked_lists!\n");
}

module_init(ll_init);
module_exit(ll_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linked list exercise module");
MODULE_AUTHOR("Kernel hacker!");
