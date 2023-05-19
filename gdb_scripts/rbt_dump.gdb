
####################################################################
#  用于在gdb中调试红黑树结构  add by 张鹏20646 @20230519
####################################################################

define rb_first
  set $root = $arg0
  set $n = $root->rb_node

  if !$n
    printf "NULL\n"
    return 0
  end

  while $n->rb_left
    set $n = $n->rb_left
  end

  return $n
end

define rb_next
  set $node = $arg0
  set $parent = ((struct rb_node *)(($node)->rb_parent_color & ~3))

  if $parent == $node
    return 0
  end

  if $node->rb_right
    set $node = $node->rb_right
    while $node->rb_left
      set $node = $node->rb_left
    end
    return $node
  end

  while $parent && $node == $parent->rb_right
    set $node = $parent
    set $parent = ((struct rb_node *)(($node)->rb_parent_color & ~3))
  end

  return $parent
end
define rbt_dump
  set $rbt = $arg0
  set $rbnode = 0
  set $iter = rb_first(&$rbt->root)

  if !$iter
    printf "iter is null\n"
    return 0
  end

  #由于node节点在结构体第一个位置，因此这种强转也可以使用
  #set $f = (rbt_node_t*)$iter
  #printf "key: %lu, private_data: %p, destroy_fn:%p dump_fn:%p\n", $f->key, $f->private_data, $f->destroy_fn, $f->dump_fn
  set $count = 0
  printf "----------------------begin dump rbt:%p---------------------\n", $rbt
  while $iter
   set $count = $count + 1
    printf "####################### count: %d #########################\n", $count
    # 为了安全起见，还是使用这种方式
    set $rbnode = (rbt_node_t *)((char *)$iter - ((long)&((rbt_node_t *)0)->node))
    printf "node:  key: %lu, private_data: %p, destroy_fn:%p dump_fn:%p\n", $rbnode->key, $rbnode->private_data, $rbnode->destroy_fn, $rbnode->dump_fn
    #printf "                                                 \n"
    #p *(pthread_res_t*)$rbnode->private_data
    #printf "                                                 \n"
    #printf "                                                 \n"
    #if $rbnode->dump_fn
    #  call $rbnode->dump_fn($rbnode->private_data)
    #end
    set $iter = rb_next($iter)
  end

  printf "------------------------end dump rbt:%p---------------------\n", $rbt
end
