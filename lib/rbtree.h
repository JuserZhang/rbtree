#ifndef	_RBTREE_H_
#define	_RBTREE_H_
#include <stdio.h>
#include <stdlib.h>

struct rb_node
{
	unsigned long  rb_parent_color;
	struct rb_node *rb_right;
	struct rb_node *rb_left;
};

struct rb_root
{
	struct rb_node *rb_node;
};


#define RB_ROOT	(struct rb_root) { NULL, }

#if 0
#define	rb_entry(ptr, type, member) container_of(ptr, type, member)
#endif

void rb_insert_color(struct rb_node *, struct rb_root *);
void rb_erase(struct rb_node *, struct rb_root *);

/* Find logical next and previous nodes in a tree */
struct rb_node *rb_next(struct rb_node *);
struct rb_node *rb_prev(struct rb_node *);
struct rb_node *rb_first(struct rb_root *);
struct rb_node *rb_last(struct rb_root *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
extern void rb_replace_node(struct rb_node *victim, struct rb_node *new_node,
			    struct rb_root *root);

void rb_link_node(struct rb_node * node, struct rb_node * parent,
		  struct rb_node ** rb_link);

#endif
