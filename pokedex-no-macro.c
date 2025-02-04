#include <linux/module.h>
#include <linux/printk.h>
// You should NOT include <linux/list.h>

/*
 * Undefine commonly used macros -- DO NOT MODIFY
 * Please also do not use other macros and functions defined in <linux/list.h>,
 * such as INIT_LIST_HEAD, list_add, list_add_tail, list_del, list_empty,
 * or any of the internal helper functions beginning with __.
 */
#undef LIST_HEAD
#undef LIST_HEAD_INIT
#undef list_entry
#undef list_for_each_entry
#undef list_for_each_entry_safe
#undef container_of
#undef offsetof
#undef READ_ONCE
#undef WRITE_ONCE

struct pokemon {
	char name[32];
	int dex_no;
	struct list_head list;
};

static size_t offset = (size_t) &((struct pokemon *)0)->list;

void print_pokemon(struct pokemon *p)
{
	pr_info("%s: National Dex No. #%d\n", p->name, p->dex_no);
}

/* TODO: declare a single static struct list_head, named pokedex */
static struct list_head pokedex = { &pokedex, &pokedex };

void add_pokemon(char *name, int dex_no)
{
	/* TODO: write your code here */
	struct pokemon *new = kmalloc(sizeof(struct pokemon), GFP_KERNEL);
	if (!new) {
		printk(KERN_INFO "kmalloc failed.");
	}
	strcpy(new->name, name);
        new->dex_no = dex_no;

	struct list_head *tail = pokedex.prev;
	tail->next = &new->list;
	new->list.prev = tail;
	new->list.next = &pokedex;
	printk(KERN_INFO "%s", new->name); //FOR DEBUGGING

}

void print_pokedex(void)
{
	/* TODO: write your code here, using print_pokemon() */
	struct list_head *cursor = pokedex.next;
	while (cursor != &pokedex) {
		struct pokemon *pmon = (struct pokemon *) (char *)cursor - offset;
		print_pokemon(pmon);
		cursor = cursor->next;
	}
}

void delete_pokedex(void)
{
	/* TODO: write your code here */
	if (pokedex.next == &pokedex) {
		return;
	}
	struct list_head *cursor = pokedex.next;
	struct list_head *nxt;
	while (cursor != &pokedex) {
		struct pokemon *pmon = (struct pokemon *) (char *)cursor - offset;
		nxt = cursor->next;
		cursor->prev->next = nxt;
		nxt->prev = cursor->prev;	
		kfree(pmon);
		cursor = nxt;
	}	
}

int pokedex_nom_init(void)
{
	pr_info("Loading Module\n");

	add_pokemon("Porygon-Z", 474);
	add_pokemon("Exeggutor", 103);
	add_pokemon("Poliwhirl", 61);
	add_pokemon("Infernape", 392);

	print_pokedex();

	return 0;
}

void pokedex_nom_exit(void)
{
	pr_info("Removing Module\n");

	print_pokedex();

	delete_pokedex();
}

module_init(pokedex_nom_init);
module_exit(pokedex_nom_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A handy Pokedex module, without list macros");
MODULE_AUTHOR("SGG");
