目录结构
├── gdb_scripts
│   └── rbt_dump.gdb  # gdb dump 整个红黑树的脚本命令
├── lib
│   ├── makefile    # 将红黑树实现编译成动态库代码
│   ├── rbtree.c    #从linux内核移植到用户态的红黑树
│   ├── rbtree.h    #从linux内核移植到用户态的红黑树头文件
│   ├── rbt_api.c   #在此基础上又封装了一层api，用户态代码可以很方便的使用
│   └── rbt_api.h   #抽象的api接口
└── test
    ├── makefile    #编译测试程序
    └── test.c      #简单的测试代码



