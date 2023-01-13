# 1. 内核头文件

---

#### 作用

内核头文件的作用主要有两个：1）定义内核组件间的接口2）定义内核与用户空间的接口

内部模块：模块间的内部接口在linux/include/ 或 linux/arch/*/include/ 下都有定义。一个单独模块的源文件间的接口应该同模块源码置于同一目录下，避免污染全局头文件空间。

 外部模块：传统的内核源码安装在/usr/src/linux下，这不再支持外部模块的编译。相反，你的Makefile应该指向/lib/modules/${kver}/build，其中${kver}是内核确切的版本字符串，例如：对于当前正在运行的内核，就是“uname -r”的输出。

 用户空间程序：一般来说，用户空间程序是针对发行版提供的头文件编译的，通常源于glibc-devel、glibc-kernheaders 或 linux-libc-dev。这些头文件通常来源于旧版内核，并不能安全地在不重新编译glibc的情况下被替换。特别地，作为一个到/usr/src/linux/include或/lib/modules/*/build/include/linux的符号链接/usr/include/linux，是极不推荐使用的。因为它经常使重新编译的应用程序损坏。例如，旧内核使用include/asm-${arch}存放架构特定的头文件，而不是现在的arch/${arch}/include/asm ，且没有符号链接到架构特定的目录。

#### 什么时候需要

当你在编译一个设备驱动模块时，你需要在系统中安装内核头文件。内核头文件同样在你编译与内核直接链接的用户空间程序时需要。当你在这些情况下安装内核头文件时，你必须确保内核头文件精确地与你当前内核版本匹配（比如：3.13.0-24-generic）。

# 2. 插入模块命令

---

## insmod

## modprobe

> 在搞Linux驱动移植/开发的时候，对于编译出来的驱动可以选择手动insmod，但是感觉很土：1. 需要指定路径； 2. 如果碰到存在依赖的，就丑陋不堪了。
> 
> 但是modprobe可以很优雅的解决：直接$ modprobe XX_DRIVER_XX即可。
> 
> 那么问题来了：modprobe自动加载的时候，如何知道驱动的路径和信息呢？以及，我自己编译的驱动，又如何能够modprobe，而不是山寨的insmod呢？

### modprobe 的信息依据

modprobe依赖于/lib/modules/$(uname -r)/modules.dep 而且该文件时不需要手动修改，而使用depmod即可实现自动化操作。

### depmod

depmod执行不依赖于目录，效果是自动查找`/lib/modules/$(uname -r)/`下的驱动文件，以及分析彼此的依赖关系。

**因此，如果使modprobe找得到的话：**

**step 1:添加驱动至`/lib/modules/$(uname -r)/` 目录下**

**step 2: 终端运行命令`depmod` 更新modules.dep信息**

⚠️加载驱动时 使用命令`modprobe DRIVER_NAME`而不是`modprobe DRIVER_NAME.ko`

在Linux中，modprobe和insmod都可以用来加载module，不过现在一般都推荐使用modprobe而不是insmod了。 modprobe和insmod的区别是什么呢？

1) modprobe可以解决load module时的依赖关系，比如load moudleA就必须先load mouduleB之类的，它是通过/lib/modules//modules.dep文件来查找依赖关系的。而insmod不能解决依赖问题。

2) modprobe默认会去/lib/modules/目录下面查找module，而insmod只在给它的参数中去找module（默认在当前目录找）。 这样，有时insmod也有它的有用之处，举个例子吧。

有/root/my-mod.ko这个module，cd /root/，然后用insmod my-mod.ko(insmod /root/my-mod.ko)就可以insert这个module了，但是用modprobe my-mod.ko(modprobe /root/my-mod.ko)却提示"FATAL: Module my-mod.ko not found"，这就是因为modprobe是到/lib/modules/`uname -r`/下去找module的，如果没找到就是这样了。

原文链接：https://blog.csdn.net/yexiangCSDN/article/details/82828055

# 3. 模块之间的依赖

---

> Q1:在Linux驱动开发过程中，一个产品的功能可能由好几个驱动模块组成。模块之间存在依赖的关系。那么怎么处理这种依赖关系呢？
> 
> Q2:比如一个产品由A和B两个模块组成。在B模块里面调用了A模块的test()函数。那么编译的时候，是怎么实现的呢？
> 
> Q3: Module.symvers文件怎么用？怎么生成的？生成的内容是什么？
> 
> Q4: EXPORT_SYMBOL宏怎么用？
> 
> Q5:模块的所在路径怎么处理？
> 
> Q6：导出的符号重复怎么说？钩子?

**A1:**

类似于不同 `.c` 文件，`test1.c` 想用 `test2.c`中的函数，就需要引用`test2.h`同理B模块调用了A模块的test()函数，则需要“引用”A模块的Modules.symvers文件。

**A2：**

在编译模块的过程中，我们会发现有一个Module.symvers文件生成。这个文件，如果你的驱动代码里面没有调用EXPORT_SYMBOL这个宏，那么这个Module.symvers文件里面就是空的，如果调用了，那么里面就是对应的内容。

*方法一：手动使用extern*

```c
// 内核驱动导出符号实际就是将当前驱动的函数提供给其他驱动模块使用
// 内核里面，函数必须主动导出，别人才能使用


#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int num = 10;
static void show(void)
{
    printk(KERN_ALERT "show(), num=%d\r\n", num);
}

static int hello_init(void)
{
    printk(KERN_ALERT "Goodbye\r\n");
}
EXPORT_SYMBOL(show);


module_init(hello_init);
module_exit(hello_exit);

#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL")；
extern void show(void);
/* 在这个模块中调用hello模块中的show函数， 要在hello模块中使用EXPORT_SYMBOL
 * 1.要先编译被调用的模块， 将Module.symvers拷贝到b模块下
 * 2.编译b模块
 * 3.加载a模块，再加载b模块
 * 4.卸载b模块，再卸载a模块
*/

static int show_init(void)
{
    printk(KERN_ALERT "show_init\r\n");
    show();
    return 0;
}


static void show_exit(void)
{
    printk(KERN_ALERT "show_exit\r\n");
}

module_init(show_init);
module_exit(exit_init);

#gcc -C /lib/modules/$(uname -r)/build M=$(pwd) modules
#insmod hello1.ko
#insmod show.ko
#rmmod show.ko
#rmmod hello1.ko

#gcc -C /lib/modules/$(uname -r)/build M=$(pwd) clean
```

*方法二：通过配置环境变量*

> 在b模块的Makefile里面添加KBUILD_EXTRA_SYMBOLS=/mod_a/Module.symvers 就是模块a的Module.symvers文件路径

在编译过程中，编译器会先根据KBUILD_EXTRA_SYMBOLS和当前驱动代码里面的EXPORT_SYMBOL宏，生成对应的符号， 并将相关信息保存在Module.symvers中。在链接时，会使用Module.symvers文件里面的相关的符号表。

# 4. 内核模块出错：oops

使用锁时睡眠时引发死锁的元凶

# 5. Linux Watchdog 机制

# 6. tainted kernel

kernel bug 内部问题：属于轻微错误，比如spin_lock期间调用了sleep，导致潜在的死锁问题等。

kernel oops 可恢复错误：<mark>代表某一用户进程出现错误，需要杀死用户进程</mark>。这时如果用户进程占用了某些信号锁，这些信号锁将永远不会得到释放，就会导致系统潜在的不稳定。注意oops本身不会导致系统crash，只有打开Panic on oops 选项才会触发panic导致系统crash。

kernel Panic 不可恢复错误：是个严重错误，代表整个系统崩溃。

列出时说明kernel处于受污染状态，意思是kernel不信任这些模块，并将由有关的调试信息写入日志dmesg输出，也可以通过/proc/中的文件在运行时检查受污染状态。

## BUG、Oops或Panics消息中的污染标志

在顶部以「CPU：」开头的一行中可以找到受污染的状态，内核是否受污染和原因会显示在进程ID「PID：」和触发事件命令的名称「Comm：」

Tainted： P      W O

## 用户态call trace

##### Linux oops时，会进入traps.c中的die函数。

int die(const char *str, struct pt_regs *regs, long err)

       ... ...

       show_regs(regs);

##### 用户程序可以在以下情形call trace，以方便调试：

1）程序崩溃时，都会收到一个信号。Linux系统接收到某些信号时会自动打印call trace。

2）在用户程序设计中添加检查点，类似于assert机制，如果检查点的条件不满足，就执行call trace。

用户态的call trace与内核态相同，同样满足EABI标准，原理如下：

在GNU标准中，有一个内建函数__builtin_frame_address。这个函数可以返回当前执行上下文的栈底（Frame Header）指针（同时也是指向Back Chain Word的指针），通过这个指针得到当前调用栈。而这个调用栈中，会有上一级调用函数的栈底指针，通过这个指针再回溯到上一级的调用栈。以此类推完成整个 call dump过程。

得到函数的地址后，可以通过符号表得到函数名字。如果是动态库中定义的函数，还可以通过扩展函数dladdr得到这个函数的动态库信息。

# 7. Linux内核死锁

死锁就是多个进程（线程）因为等待别的进程已占有的自己所需要的资源而陷入阻塞的一种状态，死锁状态一旦形成，进程本身是解决不了的，需要外在的推动，才能解决，最重要的是死锁不仅仅影响进程业务，而且还会占用系统资源，影响其他进程。所以内核中设计了死锁监测机制，一旦发现死锁进程，就重启OS，快刀斩乱麻解决问题。「？之所以使用重启招数，还是在于分布式系统中可以容忍单点崩溃，不能容忍单点进程计算异常，否则进行死锁检测重启OS就得不偿失了」

内核提供自旋锁、信号量等锁形式的工具。

LInux内核死锁主要分为两种：D状态死锁和R状态死锁

**D状态死锁**：进程长时间处于`TASK_UNINTERRUPTIBLE`而不恢复的状态，不响应其他信号（kill -9），保证一些内核原子操作不被意外中断。但这种状态时间长就表示进程异常了，需要处理。

**R状态死锁**：进程长时间处于TASK_RUNNING 状态抢占CPU而不发生切换，一般是，进程关抢占后一直执行任务，或者进程关抢占后处于死循环或者睡眠，此时往往会导致多个CPU互锁，整个系统异常。

补充：lockdep不是所谓的死锁

内核R状态死锁检测机制就是lockdep机制，入口即是lockup_detector_init函数。

内核R状态死锁检测机制就是lockdep机制，入口即是`lockup_detector_init`函数。

1.通过cpu_callback函数调用`watchdog_enable`，在每个CPU core上创建`SCHED_FIFO`级别的实时线程watchdog，其中使用了`hrtimer`定时器，控制检查周期。

2.`hrtimer`定时器调用`watchdog_timer_fn`进行清狗的时间检查，而线程则每次重置清狗时间，如果`watchdog_timer_fn`发现狗的重置时间已经和当前时间差出危险值，则根据开关进行panic处理。

## Linux睡眠函数

**内核态睡眠  **
`msleep()  `
`msleep_interruptible()  `
`ssleep()  `
以上三个睡眠函数都是基于内核定时器(timer_list)  

**用户态睡眠  **
`sleep()  `

`usleep()`
`nanosleep()  `
以上三个睡眠函数都是基于内核定时器(timer_list)

# 8. 驱动模块无法卸载

# 9.Makefile文件

```makefile
obj-m+=hello.o
all:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

这个makefile的作用就是编译hello.c文件，最终生成hello.ko文件。

`obj-m+=hello.o` 显式将`hello.o`编译成`hello.ko`,而`hello.o`则是由make的自动推导功能便已`hello.c`文件生成。

obj-m表示编译生成可加在模块，obj-y表示直接将模块编译内核。

### **all、clean**

这一类是makefile中的伪目标，伪目标并不是一个真正的目标，它代表着一系列你想要执行命令的集合，通常一个makefile会对应多个操作，例如编译，清除编译结果，安装，就可以使用这些伪目标。如下，当make后不带参数时，默认执行第一个伪目标的操作。

```c
make
make clean
```

### `make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules`

标准make指令是`make -C $KDIR M=$(PWD) [target]`

**-C**选项：此选项指定内核源码的位置， make在编译时会进入内核源码目录，执行编译，编译完成时返回。

**$KDIR**： `/lib/modules/(shell uname -r)/build/`指定内核源码的位置

直接在目标板上编译时，内核头文件默认存放在`/lib/modules/(shell uname -r)/build/`中，这个`build`目录是一个软链接，链接到源码头文件的安装位置。而内核真正的源码库则直接引用正在运行的内核镜像。

**M=$(PWD)**：需要编译的模块源文件地址

## 编译多个源文件

```makefile
obj-m +=hello.o
hello-y := a.o\
           b.o\
           c.o
```

hello.o目标文件依赖于`a.o, b.o, c.o`,这里的`a.o`虽然没有指定源文件，根据推导规则就是依赖文件`a.c`。

除了`hello-y`， 同时也可以用`hello-objs`，实现效果都是一样的。

## 同时编译多个可加载模块

kbuild支持同时编译多个可加载模块，就是生成多个`.ko`文件，格式如下：

```makefile
obj-m := foo.o bar.o
foo-y := <foo_srcs>
bar-y := <bar_srcs>
```

## 坑

### \$(shell pwd) 和 \$（pwd）

makefile里面获取相对路径必须在`pwd`前面加`shell`， 然后把`shell pwd`当成一个变量来引用

```makefile
INCDIR := $(shell pwd)
all:
    echo $INCDIR
```

所以，修改某些开源代码时首先要确定当前是**Makefile**还是**Shell**脚本，以免把`shell pwd`和`pwd`混用。

# 内核线程

```c
// 函数定义在include/linux/kthread.h中

struct task_struct kthread_run(int (*threadfn)(void* data), void *data, constchar namefmt[],);
int kthread_stop(struct task_struct * k);
int kthread_should_stop(void);
```

`kthread_run()`负责内核线程的<mark>创建并运行</mark>，参数包括回调函数名`threadfn`，参数`data`，创建的内核线程名称`namefmt`

其实`kthread_run()`实际是一个**宏定义**，由`kthread_create()`和`wake_up_process()`两部分组成。前者创建线程，后者启动线程。



# 内核模块调试

linux内核模块作为操作系统调试起来有一定特殊性， 不能直接在本地gdb调试，需要一个host和一个target远程调试。

远程调试内核主要有三种方式，一种是内置派，通过Kgdb这种内核内置的模块。一种是虚拟派，通过把内核跑在类似qemu的虚拟机上。最后一种是硬件派，这是嵌入式设备上最常见的调试方式，通过连接jtag调试。

前两种多少会受到软件的限制，而最后一种方式由硬件来实现，最为直接。
