# 

skb结构体包含缓冲区的地址和长度。他还为内核中不同层对数据的操纵提供了良好的支持；实现了众多的接口，如，不同网络层首部的插入和去除等。整个数据的发送和接收过程都会用到这个结构。

在创建套接字时就与制定的协议绑定了，相关的协议便会负责相关层的数据传输。

用户态和内核态通信方式：

# 1、使用netlink进行用户空间和内核空间的数据交互

netlink 用于在用户空间和内核空间传递数据， 它提供了内核/用户空间的双向通信方案

## netlink套接字的用户空间建立和使用

netlink包含用户空间的标准套接字接口和用于构建内核模块的内核API。

> Q1：内核API是指什么？
> 
> Q2: 结构体里面函数是怎么使用和定义
> 
> Q3：netlink地址类型？
> 
> Q4： 协议簇是什么？有哪些？干什么用的？
> 
> Q5： 多播组群？
> 
> Q6:：socket里面做了啥，为什么就返回一个int类型的
> 
> Q7：绑定bind函数里面做了啥，怎么就bind了？
> 
> Q8：nlmsg的消息体怎么放上去的？

socket():

bind()：将netlink套接字和netlink地址类型进行绑定

sendmsg()：由用户空间向内核空间发送数据

recvmsg()：从内核或其他进程接收信息

close()

### 套接字建立

> int s = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE)

**AF_NETLINK**为协议簇

**SOCK_RAW**为套接字类型，是一种数据包套接字类型的服务，可以是SOCK_RAW或SOCK_DGRAM

**NETLINK_ROUTE**为协议类型，可以是系统定义的某种类型也可以是用户自定义的类型

网络协议类型有：

**NETLINK_ROUTE**：接收路由更新并且可以用于修改ipv4路由表、网络路由、IP地址、连接参数、邻居设置、队列丢弃、负载类型、数据包分类等

**NETLINK_FIREWALL**：接收IPv4防火墙发送的数据包

**NETLINK_ARPD**：用于用户空间管理arp表

**NETLINK_ROUTE6**：接收和发送IPv6路由表更新

### 套接字绑定

> bind ( s, (struct sockaddr*)&nladdr, sizeof(nladdr) );

作用：将一个套接字文件描述符和地址结构绑定在一起。地址结构如下

```c
struct sockaddr_nl
{
    sa_family_t     nl_family;        /*协议簇*/
    unsigned short  nl_pad;            
    __u32           nl_pid;            /*进程ID号*/
    __u32           nl_groups;        /*多播组掩码*/
}
```

### netlink消息发送

> sendmsg ( s, &msg, 0 );

使用sendmsg（）函数向内核或其他进程发送消息。向其他进程发送消息需要填充**目的进程**的sockaddr_nl结构，这种情况下与UDP协议的sendmsg（）函数使用情况相同。如果向内核发送消息，则结构sockaddr_nl中的成员nl_pid和nl_groups均需要设置为0.

> 如果消息是一个单播，则消息的目的进程就是目的进程的PID，并填充到nl_pid在，同时nl_groups设置为0.
> 
> 如果消息是一个多播，则nl_groups需要设置多播掩码位。

然后将sockadrr_nl结构填充到消息结构中

```c
 struct   {
     void         *msg_name;       /* optional address */
     socklen_t     msg_namelen;    /* size of address */
     struct iovec *msg_iov;        /* scatter/gather array */
     size_t        msg_iovlen;     /* # elements in msg_iov */
     void         *msg_control;    /* ancillary data, see below */
     size_t        msg_controllen; /* ancillary data buffer len */
     int           msg_flags;      /* flags on received message */
 };
 /* msg_name： 数据的目的地址，网络包指向sockaddr_in, netlink则指向sockaddr_nl;
    msg_namelen: msg_name 所代表的地址长度
    msg_iov: 指向的是缓冲区数组
    msg_iovlen: 缓冲区数组长度
    msg_control: 辅助数据，控制信息(发送任何的控制信息)
    msg_controllen: 辅助信息长度
    msg_flags: 消息标识
 */
```

```c
struct msghdr mhdr; /*消息头*/
mhdr.msg_name = (void *)&nladdr; /*消息名称指向netlink的地址结构*/
mhdr.msg_namelen = sizeof(nladdr); /*消息名称长度为netlink地址的大小*/
```

在netlink套接字中有自己的消息头，在实际的实现过程中是将netlink自己的消息头部包含在通用的消息中，在通用消息的基础上构造netlink的自由消息。netlink规定每个消息中必须含有netlink私有消息，其私有消息的结构原型如下：

```c
struct nlmsghdr
{
    __u32 nlmsg_len;    /*消息长度*/
    __u16 nlmsg_type;   /*消息类型*/
    __u16 nlmsg_flag;   /*附加信息*/
    __u32 nlmsg_seq;    /*序列号*/
    __u32 nlmsg_pid;    /*发送方的进程ID*/
}nlmh;
```

**nlmsg_len**表示消息的长度， 它包括消息头部

**nlmsg_type**仅用于应用程序，对于netlink核心是透明的

**nlmsg_flags**给出额外的控制信息

**nlmsg_seq**和**nlmsg_pid**是应用程序程序用于跟踪消息使用，对于netlink核心是透明的

netlink的消息包含nlmsghdr和消息的负载，例如：

```c
struct iovec vec;              /*向量*/
iov.iov_base = (void*)nlmh;    /*向量的数据指针为netlink消息的头部*/
iov.iov_len = nlmh->nlmsg_len; /*向量的长度为netlink消息的长度*/
msg.msg_iov = &vec;            /*消息的向量*/
msg.msg_iovlen = 1;            /*消息向量的个数*/
```

填充完毕就可以发消息了

```
sendmsg ( s, &msg, 0 );
```

### netlink消息接收

> recv ( s, &msg, 0 )

recvmsg()函数用于接收内核和其他应用程序发送的数据。接收消息的缓冲区要足够包含netlink消息头部和负载。接受消息代码如下

```c
struct sockaddr_nl nladdr;
struct msghdr msgh;
struct nlmsghdr &nlmh;
struct iovec iov;

msg.msg_name = (void*)&nladdr;
msg.msg_namelen = sizeof(nladdr);
msg.msg_iov = &iov;
msg.msg_iovlen = 1;
iov.iov_base = (void*)&nlmh;
iov.iov_len = MAX_NL_MSG_LEN;
recvmsg(s, &msg, 0);
```

当正确接收到消息之后，指针nlmh指向刚刚接收netlink消息的头部。nladdr中保存的为接收到消息的目的地止，包含着进程的pid和多播群组。宏NLMSG_DATA(nlmh)返回指向负载部分的指针，在头文件<netlink.h>中定义。

**netlink常用宏**

```c
#define NLMSG_ALIGNTO   4U
/* 宏NLMSG_ALIGN(len)用于得到不小于len且字节对齐的最小数值 */
#define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )

/* Netlink 头部长度 */
#define NLMSG_HDRLEN     ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))

/* 计算消息数据len的真实消息长度（消息体 +　消息头）*/
#define NLMSG_LENGTH(len) ((len) + NLMSG_HDRLEN)

/* 宏NLMSG_SPACE(len)返回不小于NLMSG_LENGTH(len)且字节对齐的最小数值 */
#define NLMSG_SPACE(len) NLMSG_ALIGN(NLMSG_LENGTH(len))

/* 宏NLMSG_DATA(nlh)用于取得消息的数据部分的首地址，设置和读取消息数据部分时需要使用该宏 */
#define NLMSG_DATA(nlh)  ((void*)(((char*)nlh) + NLMSG_LENGTH(0)))

/* 宏NLMSG_NEXT(nlh,len)用于得到下一个消息的首地址, 同时len 变为剩余消息的长度 */
#define NLMSG_NEXT(nlh,len)  ((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), \
                  (struct nlmsghdr*)(((char*)(nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))

/* 判断消息是否 >len */
#define NLMSG_OK(nlh,len) ((len) >= (int)sizeof(struct nlmsghdr) && \
               (nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && \
               (nlh)->nlmsg_len <= (len))

/* NLMSG_PAYLOAD(nlh,len) 用于返回payload的长度*/
#define NLMSG_PAYLOAD(nlh,len) ((nlh)->nlmsg_len - NLMSG_SPACE((len))
```

## 如何在内核中进行netlink的程序设计

内核空间的netlink API 与应用程序之间的API之间有很多的不同，netlink内核API在文件net/core/af_netlink.c中实现。内核netlinkAPI可以用于访问内核模块的netlink套接字，并和用户空间的应用程序进行通信。如果想添加用户自己定义的协议类型，需要自己增加协议类型，如定义一下的协议类型：

```c
#define NETLINK_TEST 17
```

### netlink的内核套接字建立

在用户层中，socket()函数用于建立netlink套接字，其中的协议类型应该也为NETLINK_TEST。内核建立次套接字的函数为

> struct sock *
> 
> netlink_kernel_create(int unit, void (*input)(struct sock *sk, int len));

**unit**是netlink协议类型，例如`NETLINK_TEST`

函数指针**input**是一个接收用户空间消息后的回调函数，是netlink内核套接字的主要处理函数。

建立netlink套接字函数成功时，返回一个structure sock指针类型的值，之后可以用这个值对netlink套接字进行处理；当返回值为NULL时，套接字建立失败，需要进行容错处理，进行资源释放等。

### netlink接收应用层数据

当内核使用`netlink_kernel_create()`函数建立一个NEILINK_TEST类型的协议之后，当用户空间想内核空间通过之前的netlink套接字发送消息时，`netlink_kernel_create()`函数注册的回调函数`input()`会被调用，下面是`input()`函数的实现代码：

```c
void input(struct sock* sk, int len)
{
    struct sk_buff *skb;
    struct nlmsghdr *nlh = NULL;
    u8 *payload = NULL;

    /*从内核接收链中摘除网络数据*/
    while ((skb = skb_dequeue(&sk->receive_queue)) != NULL) 
    {
        nlh = (struct nlmsghdr *)skb->data;/*Netlink消息头部*/
        payload = NLMSG_DATA(nlh);    /*获得其中的负载数据*/
    }
}
```

> 如果input（）函数处理过秤占用很长时间，则需要使用内核线程来处理。具体详情请看《Linux网络编程》p635

### netlink的内核数据发送

netlink在内核中发送数据应与应用程序发送数据一样，需要设置netlink的源地址和目的netlink地址。需要发送的netlink消息数据在结构struct sk_buff* skb中，则本地的地址可以使用如下设置：

```c
/*本地的地址设置*/
NETLINK_CB(skb).groups = local_groups;
NETLINK_CB(skb).pid = 0;        /*from kernel*/
```

netlink的目的地止的设置如下：

```c
/*m目的地址设置*/
NETLINK_CB(skb).dst_groups = dst_groups;
NETLINK_CB(skb).dst_pid = dst_pid;
```

上述消息没有存在`skb->data`中，而是存放在`netlink`协议的套接字缓冲区控制块`skb`中。

发送一个单播消息，使用netlink_unicase()函数，其原型如下：

```c
int
netlink_unicast(struct sock* ssk, struct sk_buff* skb, 
                u32 pid, int nonblock);
```

**ssk**是由`netlink_kernel_create()`函数返回的

**skb->data**指向需要发送的netlink消息

**pid**是应用层接受数据的pid

**nonblock**用于设置当接受缓冲区不可用的时候是阻塞等待知道缓冲区可用，还是直接返回失败

发送多播消息使用

```c
int
netlink_broadcast(struct sock* ssk, struct sk_buff* skb, 
                  u32 pid, u32 group, int allocation)
```

**group**是**OR**运算组成的接收数据的多播群ID号

**allocation**是内核内存申请类型，例如：`GFP_ATOMIC`在终端上下文使用，`GFP_KERNEL`在其他状态下使用。要申请内存的原因是这个函数在发送消息时可能需要申请多个套接字缓冲区，用于复制多播消息。

### netlink的套接字关闭

> void sock_release(struct socket * sock);

`netlink_kernel_create()`函数建立成功套接字返回值为`struct sock`，`sock_release`函数释放套接字是传入的类型是`struct socket`类型，类型`struct socket`是`struct sock`的一员，因此释放如下

```c
sock_release(nl_sk->socket);
```

# 2、ath9k usb wifi 网卡驱动浅析

参考网站：[(65条消息) ath9k usb wifi 网卡驱动浅析_Linux学习之路的博客-CSDN博客](https://blog.csdn.net/lizuobin2/article/details/53678299)

**nl80211**：用于对无线设备进行配置管理，它是一个基本**Netlink的用户态协议**(User态)
**cfg80211**：用于对无线设备进行配置管理。与FullMAC, mac80211和nl80211一起工作。(Kernel态)*「无线子系统」*
**mac80211**：是一个driver开发者可用于为SoftMAC无线设备写驱动的**框架** (Kernel态)。

## 数据传递过程

mac80211和 wifi设备驱动程序息息相关， 但是它是内核抽象出来的**设备无关层**，每一个注册进来的wifi设备，它都帮我们注册一个net_device，并且使用统一的ops函数。那么网络协议上层传递过来的数据包首先经过mac80211层，然后才会传递到设备驱动层，再由硬件发送出去。

### 数据包发送过程
