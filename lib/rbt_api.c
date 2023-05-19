#include "rbt_api.h"

#define DBG_ERR(fmt, ...)  //printf("[ERR] " fmt, ##__VA_ARGS__)
#define DBG_INFO(fmt, ...) //printf("[INFO] " fmt, ##__VA_ARGS__)

#define SAFE_FREE(ptr) do { if (ptr != NULL) { free(ptr); ptr=NULL; } } while(0)

#define container_of(ptr, type, member) \
	(type *)((char *)(ptr) - (char *) &((type *)0)->member)

#define	rb_entry(ptr, type, member) container_of(ptr, type, member)


//初始化红黑树
rbt_root_t* rbt_init(void)
{
	rbt_root_t *rbt = calloc(1, sizeof(rbt_root_t));
	if (rbt == NULL) {
		DBG_ERR("rbt: calloc rbt failed.\n");
		return NULL;
	}

	pthread_rwlock_init(&rbt->rw_lock, NULL);
	rbt->root.rb_node = NULL;
	DBG_INFO("rbt: rbt_init successed, rbt:%p.\n", rbt);
	return rbt;
}

//删除之前的旧节点
static void __rbt_node_delete(rbt_root_t* rbt, rbt_node_t *rbnode)
{
	if(!rbnode){
		return;
	}
	rb_erase(&(rbnode->node), &(rbt->root));
	if(rbnode->destroy_fn){
		rbnode->destroy_fn(rbnode->private_data);
	}
	DBG_INFO("rbt: delete key:%lu, private_data:%p rbnode:%p, rbt:%p\n",
			 rbnode->key, rbnode->private_data, rbnode, rbt);
	SAFE_FREE(rbnode);
}

//向红黑树插入一个节点
static void __rbt_node_insert(rbt_root_t* rbt, rbt_node_t* rbnode)
{
	struct rb_node *parent = NULL;
	struct rb_node **new_node = &(rbt->root.rb_node);
	rbt_node_t *tmp_rbnode;

	/*找出放置新节点的位置*/
	pthread_rwlock_wrlock(&rbt->rw_lock);
	while (*new_node) {
		tmp_rbnode = rb_entry(*new_node, rbt_node_t, node);
		parent = *new_node;

		if (rbnode->key < tmp_rbnode->key){
			new_node = &((*new_node)->rb_left);
		}
		else if (rbnode->key > tmp_rbnode->key){
			new_node = &((*new_node)->rb_right);
		}
		else{
			DBG_ERR("rbt: __rbt_node_insert failed, already exist key:%lu, private_data:%p rbnode:%p, rbt:%p\n",
						tmp_rbnode->key, tmp_rbnode->private_data, tmp_rbnode, rbt);
			//TODO: 存在相同key的节点, 删除之前的节点
			__rbt_node_delete(rbt, tmp_rbnode);
		}
	}

	rb_link_node(&rbnode->node, parent, new_node);
	rb_insert_color(&rbnode->node, &rbt->root);
	pthread_rwlock_unlock(&rbt->rw_lock);
	DBG_INFO("rbt: __rbt_node_insert success, key:%lu, private_data:%p rbnode:%p, rbt:%p\n",
			 rbnode->key, rbnode->private_data, rbnode, rbt);
}

//添加资源到rbt节点
rbt_node_t* rbt_add(rbt_root_t* rbt, const unsigned long key, 
					void* private_data, handle_t destroy_fn, handle_t dump_fn)
{
	rbt_node_t *rbnode = calloc(1, sizeof(rbt_node_t));
	if (rbnode == NULL) {
		DBG_ERR("rbt: calloc rbnode failed.\n");
		return NULL;
	}

	rbnode->key = key;
	rbnode->private_data = private_data;
	rbnode->destroy_fn = destroy_fn;
	rbnode->dump_fn = dump_fn;

	__rbt_node_insert(rbt, rbnode);
	DBG_INFO("rbt: rbt_add successed, key:%lu private_data:%p, rbt:%p\n",key, private_data, rbt);
	return rbnode;
}

//根据key从红黑树中查找node节点
static rbt_node_t* __rbt_node_search(rbt_root_t* rbt, const unsigned long key)
{
	rbt_node_t *found_rbnode = NULL;
	rbt_node_t *tmp_rbnode = NULL;
	struct rb_node *node_ptr = rbt->root.rb_node;

	pthread_rwlock_rdlock(&rbt->rw_lock);
	while (node_ptr) {
		tmp_rbnode = rb_entry(node_ptr, rbt_node_t, node);
		if (key < tmp_rbnode->key){
			node_ptr = node_ptr->rb_left;
		}
		else if (key > tmp_rbnode->key){
			node_ptr = node_ptr->rb_right;
		}
		else{
			found_rbnode = tmp_rbnode;
			break;
		}
	}
	pthread_rwlock_unlock(&rbt->rw_lock);
	return found_rbnode;
}

//根据key获取节点的值
void* rbt_find(rbt_root_t* rbt, const unsigned long key)
{
	rbt_node_t *rbnode = __rbt_node_search(rbt, key);
	if(!rbnode){
		DBG_ERR("rbt: key:%lu, __rbt_node_search falied in rbt:%p\n", key, rbt);
		return NULL;
	}

	DBG_INFO("rbt: key:%lu, private_data:%p, rbt:%p\n", key, rbnode->private_data, rbt);
	return rbnode->private_data;
}

//根据key 删除节点
void rbt_delete(rbt_root_t* rbt, const unsigned long key)
{
	rbt_node_t *rbnode = NULL;
	rbnode = __rbt_node_search(rbt, key);
	if(!rbnode){
		DBG_ERR("rbt: key:%lu, __rbt_node_search falied in rbt:%p\n", key, rbt);
		return;
	}
	pthread_rwlock_wrlock(&rbt->rw_lock);
	rb_erase(&(rbnode->node), &(rbt->root));
	pthread_rwlock_unlock(&rbt->rw_lock);

	DBG_INFO("rbt: delete frome rbt, key:%lu private_data:%p rbnode:%p.\n", key, rbnode->private_data, rbnode);

	if(rbnode->destroy_fn){
		rbnode->destroy_fn(rbnode->private_data);
	}

	SAFE_FREE(rbnode);
}

//dump 红黑树的所有节点信息
void rbt_dump(rbt_root_t* rbt)
{
	rbt_node_t *rbnode = NULL;
	struct rb_node *iter;
	pthread_rwlock_rdlock(&rbt->rw_lock);
	DBG_INFO("rbt: ----------------------begin dump rbt:%p---------------------\n",rbt);
	for (iter = rb_first(&rbt->root); iter != NULL; iter = rb_next(iter)) {
		rbnode = rb_entry(iter, rbt_node_t, node);
		if(rbnode->dump_fn){
			rbnode->dump_fn(rbnode->private_data);
		}
	}
	DBG_INFO("rbt: ------------------------end dump rbt:%p---------------------\n",rbt);
	pthread_rwlock_unlock(&rbt->rw_lock);
}

//销毁红黑树，释放内存
void rbt_destroy(rbt_root_t* rbt)
{
	//1.判断红黑树上是否没有节点
	if(rbt->root.rb_node){
		DBG_ERR("rbt: rbt:%p not empty\n", rbt);
		return;
	}
	//2.删除空树
	pthread_rwlock_destroy(&rbt->rw_lock);
	DBG_INFO("rbt: rbt_destroy successed, rbt:%p.\n",rbt);
	SAFE_FREE(rbt);
}