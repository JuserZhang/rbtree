
#ifndef _RBT_API_H_
#define _RBT_API_H_
#include <pthread.h>
#include "rbtree.h"

/*
 * 封装一层通用的红黑树接口，20230518@张鹏20646
 */

typedef void (*handle_t) (void* private_data); 

typedef struct rbtree_root {
	pthread_rwlock_t rw_lock;
	struct rb_root root;
} rbt_root_t;

typedef struct data_node {
	struct rb_node node;		//红黑树节点
	unsigned long  key;			//红黑树key
	void *private_data;			//私有数据
	handle_t destroy_fn; 		//释放private_data 的函数指针
	handle_t dump_fn; 			//解析private_data 的函数指针
} rbt_node_t;

//初始化红黑树
rbt_root_t* rbt_init(void);
//初始化一个红黑树节点
rbt_node_t* rbt_add(rbt_root_t* rbt, const unsigned long key,
					 void* private_data, handle_t destroy_fn, handle_t dump_fn);
//根据key获取节点的私有资源数据
void* rbt_find(rbt_root_t* rbt, const unsigned long key);
//根据key 删除节点
void rbt_delete(rbt_root_t* rbt, const unsigned long key);
//dump 红黑树的所有节点信息
void rbt_dump(rbt_root_t* rbt);
//销毁红黑树，释放内存
void rbt_destroy(rbt_root_t* rbt);

#endif