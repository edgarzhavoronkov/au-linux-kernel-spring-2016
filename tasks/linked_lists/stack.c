#include "stack.h"
#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/errno.h>

stack_entry_t* create_stack_entry(void *data)
{
    stack_entry_t* entry = kmalloc(sizeof(stack_entry_t), GFP_KERNEL);
    if (!entry)
    {
        //handle error somehow
        return NULL;
    }
    entry->data = data;
    INIT_LIST_HEAD(&entry->lh);
    return entry;
}

void delete_stack_entry(stack_entry_t *entry)
{
    kfree(entry);
}

void stack_push(struct list_head *stack, stack_entry_t *entry)
{
    list_add(&(entry->lh), stack);
}

stack_entry_t* stack_pop(struct list_head *stack)
{
    stack_entry_t* entry = NULL;
    if (stack_empty(stack))
    {
        //handle error somehow
        return NULL;
    }
    entry = list_entry(stack->next, stack_entry_t, lh);
    list_del_init(stack->next);
    return entry;
}
