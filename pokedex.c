#include <linux/module.h>
#include <linux/list.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/sys.h>
#include <linux/slab.h>

struct pokemon {
	char name[32];
	int dex_no;
	struct list_head list;
};

void print_pokemon(struct pokemon *p)
{
	pr_info("%s: National Dex No. #%d\n", p->name, p->dex_no);
}

/* TODO: declare a single static struct list_head, named pokedex */
static LIST_HEAD(pokedex);

void add_pokemon(char *name, int dex_no)
{
	/* TODO: write your code here */
	struct pokemon *new = kmalloc(sizeof(struct pokemon), GFP_KERNEL);
	if (!new) {
		printk(KERN_INFO "kmalloc failed.");
	}	
	strcpy(new->name, name);
        new->dex_no = dex_no;
	list_add_tail(&new->list, &pokedex); 
}

void print_pokedex(void)
{
	/* TODO: write your code here, using print_pokemon() */
	struct pokemon *cursor;
	list_for_each_entry(cursor, &pokedex, list) {
	      print_pokemon(cursor);
	}	      
}

void delete_pokedex(void)
{
	/* TODO: write your code here */
	if (list_empty(&pokedex)) {
		return;
	}
	struct pokemon *cursor, *tmp;
	list_for_each_entry_safe(cursor, tmp, &pokedex, list) {
		kfree(cursor);
		list_del(&cursor->list);
	}	
}

int pokedex_init(void)
{
	pr_info("Loading Module\n");

	add_pokemon("Porygon-Z", 474);
	add_pokemon("Exeggutor", 103);
	add_pokemon("Poliwhirl", 61);
	add_pokemon("Infernape", 392);

	print_pokedex();

	return 0;
}

void pokedex_exit(void)
{
	pr_info("Removing Module\n");

	print_pokedex();

	delete_pokedex();
}

module_init(pokedex_init);
module_exit(pokedex_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A handy Pokedex module");
MODULE_AUTHOR("SGG");
