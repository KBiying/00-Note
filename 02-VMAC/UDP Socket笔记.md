## UDP Socket笔记

1. 内核中的 `socket` 概览图：

   ![img](https://img-blog.csdn.net/20150415092843904?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1eGluZ2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

   *`Socket(domain, type, protocol)`*函数中的三个参数其实就是把抽象的*`socket`*具体化的条件，*`domain`*参数决定了图中所示的第二层通信域，*`type`*决定了第三层的通信模式，*`protocol`*决定了第四层真正的通信协议。

   

   ![](https://image-static.segmentfault.com/104/445/1044455728-5fc988aaec930)

   

2. 应用进程往一个 `UDP` 套接字写入一个消息，该消息随后被封装（`encapsulating`）到一个 `UDP` 数据报，该 `UDP` 数据报进而又被封装到一个 `IP` 数据报中，然后发送到目的地

3. 如果一个数据报到达了其最终目的地，但是校验和检测发现有错误，或者该数据报在网络传输中被丢弃了，它就无法被投递给 `UDP` 套接字，也不会被源端自动重传。**如果想要确保一个数据报到达其目的地，可以往应用程序中添置一大堆的特性：来自对端的确认、本端的超时与重传等**

4. `UDP` 不提供可靠性。`UDP` 本身不提供确认、序列号、超市和重传等机制。如果一个 `UDP` 数据报在网络中被复制，两份副本就可能都递送到接收端的主机。同样地，如果一个 `UDP` 客户发送两个数据报到同一个目的地，它们可能被网络重新排序，颠倒顺序后到达目的地。`UDP` 应用必须处理所有这些情况（`22.5节`）

5. 用户程序使用 `socket()` 创建一个 `UDP` 套接字后，调用 `sendto() recvfrom()` 来发送和接收目的地址的数据。

   >  一个可能性是需要去 `sendto()` 源码查看是如何继续使用套接字发送下去的。

6. 对于面向数据报的理解：

   > 1. 应用层交给UDP多长的报文, UDP原样发送, 既不会拆分, 也不会合并；
   >
   > 2. 用UDP传输100个字节的数据：如果发送端调用一次`sendto`, 发送100个字节, 那么接收端也必须调用对应的一次`recvfrom`, 接收100个字节; 而不能循环调用10次`recvfrom`, 每次接收10个字节

7. UDP的发送和接收缓冲区：

   > 1. UDP没有真正意义上的发送缓冲区，**调用`sendto`会直接交给内核，由内核将数据传给网络层协议进行后续的传输动作**
   > 2. UDP具有接收缓冲区，但是这个接收缓冲区不能保证收到的UDP报的顺序和发送UDP报的顺序一致; 如果缓冲区满了, 再到达的UDP数据就会被丢弃
   > 3. 如果没有接收缓冲区，那么就要求上层及时将获取到的报文读取上去，如果没有及时被读取，那么新获取的报文数据就会被迫丢弃；UDP会维护接收缓冲区将接收到的报文暂时的保存起来，供上层读取
   > 4. 对于UDP，如果还没有上层交付数据，UDP的报文长度仍然在，同时UDP是一个一个把数据交付给应用层的，有很明确的数据边界，站在应用层的角度，使用UDP的时候，要么收到完整的UDP报文，要么不收，不会出现“半个”的情况

8. UDP如何将有效载荷交付给上层协议：

   > 获取到一个报文后从该报文的前8个字节中提取出对应的目的端口号，通过目的端口号找到对应的上层应用层进程进行交付

9. UDP是全双工通信：

   > UDP的socket在读(写)的同时也能进行写(读)，也就是说不会像管道这种半双工通信读写是阻塞进行的(无法同时进行)

10. UDP使用注意事项：

   > 1. UDP协议首部中有一个16位的最大长度，也就是说一个UDP能传输的数据最大长度是`64K`(包含UDP首部)
   > 2. 如果我们需要传输的数据超过`64K`, 就需要在应用层手动的分包，多次发送，并在接收端手动拼装



### *Socket(domain, type, protocol)* 函数



#### *Domain(family)* 参数

*`Domain`*参数指定了通信的*”*域*”(*在后文中会用*`family`*替代*`domain`)*，我们是在*`IPv4`*还是*`IPv6`*这个范围内通信，也就决定了我们通信的地址是*`IPv4`*格式还是*`IPv6`*格式。通常可选的定义如下：

| 名称                | 目的           |
| ------------------- | -------------- |
| *AF_UNIX, AF_LOCAL* | 本地通信       |
| *AF_INET*           | *IPv4*网络通信 |
| *AF_INET6*          | *IPv6*网络通信 |
| *AF_PACKET*         | 链路层通信     |

> 在*Linux*系统中**AF_**和**PF_**是等价的。

在内核源码中*net*目录下面有*`Af_`*开头的一系列文件*(*如*:`Af_inet.c`*、*`Af_inet6.c`*、*`Af_unix.c`*等*)*，**每一个文件分别代表了一种协议族**。

在*`Net.h`*中定义了一个结构体：

```c
//net_proto_family结构体定义了每一个协议族的新建socket句柄

struct net_proto_family {

    int     family;

    int     (*create)(struct net *net, struct socket *sock,

                  int protocol, int kern);

    struct module   *owner;

};

//Af_inet.c中的PF_INET domain的定义，是实际实现的接口

static const struct net_proto_family inet_family_ops = {

    .family = PF_INET,

    .create = inet_create,

    .owner  = THIS_MODULE,

};
```

> 在协议栈初始化时通过 `sock_register(const struct net_proto_family *ops)(socket.c)` 函数把协议栈支持的协议族 `family` 加入 `net_families` 数组中。
>
>     当我们通过socket系统调用创建套接字的时候流程走到__sock_create函数(SYSCALL_DEFINE3->sock_create->__sock_create)的时候根据family在net_families数组中取得对应协议族的create句柄，所以对于PF_INET协议族的套接字就是调用inet_create来新建socket。
>
>     通过上面分析可知family这个参数决定了调用哪个协议族create函数来新建socket，说得可能不准确点就是决定了你使用net/目录下面的哪个Af_*.c文件中的函数。



#### *Type* 参数

*`Type`* 就是 *`socket`* 的类型（**套接字的类型**），对于 *`AF_INET`* 协议族而言有流套接字 *(`SOCK_STREAM`)*、数据包套接字 *(`SOCK_DGRAM`)*、原始套接字 *(`SOCK_RAW`)*。

> *`Type`* 和 *`Domain`* 是有组合的，只有某些特定的组合该 *`socket`* 才会有效

在**具体的内核协议栈**中有一个很重要的结构体 `xxx_protosw`，定义了每一个协议族中 *`套接字`* **在传输层的操作集合**：

> *PS*：这里我理解传输层包括了**套接字类型*(`STREAM,DGRAM`)*和具体的传输层协议*(`TCP,UDP`)***

```c
/* This is used to register socket interfaces for IP protocols.  */

struct inet_protosw {

    struct list_head list;

    /* These two fields form the lookup key.  */

    unsigned short   type;     /* This is the 2nd argument to socket(2). */

    unsigned short   protocol; /* This is the L4 protocol number.  */

    struct proto     *prot;  // 具体协议的操作集合

    const struct proto_ops *ops; // 套接字类型的操作集合

    unsigned char    flags;      /* See INET_PROTOSW_* below.  */
};
```

*`struct proto_ops`* 结构体定义了每一种**套接字类型** (*`SOCK_STREAM、SOCK_DGRAM、SOCK_RAW`*)的操作集合，在 *`Af_inet.c`* 中分别定义了 *`inet_stream_ops、inet_dgram_ops、inet_sockraw_ops`* 这三种类型的 *`proto_ops`*：

```c
const struct proto_ops inet_stream_ops = {
    .family        = PF_INET,
    .owner         = THIS_MODULE,
    .release       = inet_release,
    .bind          = inet_bind,
    .connect       = inet_stream_connect,
    .socketpair    = sock_no_socketpair,
    .accept        = inet_accept,
    .getname       = inet_getname,
    .poll          = tcp_poll,
    .ioctl         = inet_ioctl,
    .listen        = inet_listen,
    .shutdown      = inet_shutdown,
    .setsockopt    = sock_common_setsockopt,
    .getsockopt    = sock_common_getsockopt,
    .sendmsg       = inet_sendmsg,
    .recvmsg       = inet_recvmsg,
    .mmap          = sock_no_mmap,
    .sendpage      = inet_sendpage,
    .splice_read       = tcp_splice_read,

#ifdef CONFIG_COMPAT
    .compat_setsockopt = compat_sock_common_setsockopt,
    .compat_getsockopt = compat_sock_common_getsockopt,
    .compat_ioctl      = inet_compat_ioctl,

#endif
};
```

我们新建 *`socket`* 的时候指定了 *`type`* 为 *`SOCK_STREAM`* 那么在后面的套接字操作中*(*如 *`connect`)*会调用对应的 *`inet_stream_ops`* 中对应的函数*(*如*`inet_stream_connect`)*。



#### *Protocol* 参数

上文中我们通过 *`family`* 和 *`type`* 已经基本确定了新建的 *`socket`* 具体是什么类型的套接字，最后一步通过 *`protocol`* 来确定 *`socket`* 到底支持的哪个协议 *(`TCP?UDP?`)*。

在 *`inet_protosw`* 结构体中我们已经解释过 *`proto_ops`* 对应的是每一种套接字类型的操作集合，那么可知 *`struct proto`* 对应的就是具体协议的操作集合。在*`Tcp_ipv4.c`* 中定义 *`TCP`* 协议的 *`struct proto tcp_prot`*：

```c
struct proto tcp_prot = {
    .name           = "TCP",
    .owner          = THIS_MODULE,
    .close          = tcp_close,
    .connect        = tcp_v4_connect,
    .disconnect     = tcp_disconnect,
    .accept         = inet_csk_accept,
    .ioctl          = tcp_ioctl,
    .init           = tcp_v4_init_sock,
    .destroy        = tcp_v4_destroy_sock,
    .shutdown       = tcp_shutdown,
    .setsockopt     = tcp_setsockopt,
    .getsockopt     = tcp_getsockopt,
    .recvmsg        = tcp_recvmsg,
    .sendmsg        = tcp_sendmsg,
    .sendpage       = tcp_sendpage,
    .backlog_rcv        = tcp_v4_do_rcv,
    .release_cb     = tcp_release_cb,
    .hash           = inet_hash,
    .unhash         = inet_unhash,
    .get_port       = inet_csk_get_port,
    .enter_memory_pressure  = tcp_enter_memory_pressure,
    .stream_memory_free = tcp_stream_memory_free,
    .sockets_allocated  = &tcp_sockets_allocated,
    .orphan_count       = &tcp_orphan_count,
    .memory_allocated   = &tcp_memory_allocated,
    .memory_pressure    = &tcp_memory_pressure,
    .sysctl_mem     = sysctl_tcp_mem,
    .sysctl_wmem        = sysctl_tcp_wmem,
    .sysctl_rmem        = sysctl_tcp_rmem,
    .max_header     = MAX_TCP_HEADER,
    .obj_size       = sizeof(struct tcp_sock),
    .slab_flags     = SLAB_DESTROY_BY_RCU,
    .twsk_prot      = &tcp_timewait_sock_ops,
    .rsk_prot       = &tcp_request_sock_ops,
    .h.hashinfo     = &tcp_hashinfo,
    .no_autobind        = true,

#ifdef CONFIG_COMPAT
    .compat_setsockopt  = compat_tcp_setsockopt,
    .compat_getsockopt  = compat_tcp_getsockopt,
#endif

#ifdef CONFIG_MEMCG_KMEM
    .init_cgroup        = tcp_init_cgroup,
    .destroy_cgroup     = tcp_destroy_cgroup,
    .proto_cgroup       = tcp_proto_cgroup,
#endif

};
```

**所以对于*`TCP socket`*当执行*`connect`*连接的时候经过的流程大致是`SYSCALL_DEFINE3 connect(系统调用)->inet_stream_connect(inet_stream_ops中定义)->tcp_v4_connect(tcp_prot中定义)`。**



#### *Family&type&protocol* 结合

*`struct inet_protosw`* 结构体在内核中是如何初始化的呢？

```c
/* Upon startup we insert all the elements in inetsw_array[] into
 * the linked list inetsw. 在启动时，我们将inetsw_array[]中的所有元素插入到链表inetsw中。
 * 很明显，这里列出来的是 inet 这个协议族所支持的跟 type 和 protocol 的组合
 */
static struct inet_protosw inetsw_array[] =
{
    {
        .type =       SOCK_STREAM,
        .protocol =   IPPROTO_TCP,
        .prot =       &tcp_prot,
        .ops =        &inet_stream_ops,
        .flags =      INET_PROTOSW_PERMANENT | INET_PROTOSW_ICSK,
    },
    {
        .type =       SOCK_DGRAM,
        .protocol =   IPPROTO_UDP,
        .prot =       &udp_prot,
        .ops =        &inet_dgram_ops,
        .flags =      INET_PROTOSW_PERMANENT,
       },
       {
        .type =       SOCK_DGRAM,
        .protocol =   IPPROTO_ICMP,
        .prot =       &ping_prot,
        .ops =        &inet_dgram_ops,
        .flags =      INET_PROTOSW_REUSE,
       },
       {
           .type =       SOCK_RAW,
           .protocol =   IPPROTO_IP,    /* wild card */
           .prot =       &raw_prot,
           .ops =        &inet_sockraw_ops,
           .flags =      INET_PROTOSW_REUSE,
       }
};
```

在 *`Af_inet.c`* 中定义了 *`PF_INET`* 协议族的四个初始化的 *`inet_protosw`* 结构体，在内核协议栈初始化的时候通过 *`inet_register_protosw`* 函数将这些结构体按照类型 *`(type)hash`* 到**全局**的 *`inetsw`* 数组中。





## SK_BUFF 结构

其字段可粗略划分为下列几种类型：

- 布局（`layout`）
- 通用（`General`）
- 功能专用（`Feature-specific`）
- 管理函数（`Management functions`）

当结构从一个分层传到另一个分层时，其不同的字段会随之发生变化。

1. `L4` 在传给 `L3` 之前会附加一个报头，附加报头比起把数据从一个分层拷贝到另一个分层更有效率。由于要在一个缓冲区开端新增空间（**也就是要改变指向该缓冲区的变量**）是一种复杂的运算，内核提供了 `skb_reserve` 函数来执行这一操作。所以，当缓冲区往下传经每个分层时，每层的协议首先做的就是调用 `skb_reserve` 函数，为该协议的报头预留空间。
2. 当缓冲区往上传经各个网络分层时，每个源自于旧分层的报头就不再有用处。不过，并没有把 `L2` 的报头从缓冲区删除，而是把指向**有效载荷**开端的指针向前移到 `L3` 报头的开端，这样做只需要很少的 CPU 周期。



### 布局字段

1. `struct sk_buff_head`，由于 `sk_buff` 是一个双向链表，所以该头结构也有 `next prev` 指针分别指向第一个元素和最后一个元素，里面还有一个成员叫做 `qlen`，记录双向链表的表长。每个 `sk_buff` 结构都包含一个指针，指向专一的 `sk_buff_head`，名为 `list` 
2. `struct sock *sk`，该指针指向拥有此缓冲区的套接字的 `sock` 数据结构。当数据在本地产生或者正由本地进程接收时，就需要这个指针，因为该数据以及套接字相关的信息会有 TCP / UDP 以及用户应用程序使用。当缓冲区只是被转发时（也就是说本地机器不是来源地也不是目的地），该指针就是 NULL



### 通用字段



## NetLink

### *`nlmsghdr`* 结构体

```c
struct nlmsghdr {   
    __u32       nlmsg_len;  /* Length of message including header */
    __u16       nlmsg_type; /* Message content */
    __u16       nlmsg_flags;    /* Additional flags */ 
     __u32       nlmsg_seq;  /* Sequence number */
     __u32       nlmsg_pid;  /* Sending process port ID */
 };
```



1. `nlmsg_len`：整个`netlink`消息的长度（**包含消息头**）；

2. `nlmsg_type`：消息状态，内核在`include/uapi/linux/netlink.h`中定义了以下4种通用的消息类型，它们分别是：

   ```c
   NLMSG_NOOP：不执行任何动作，必须将该消息丢弃；
   NLMSG_ERROR：消息发生错误；
   NLMSG_DONE：标识分组消息的末尾；
   NLMSG_OVERRUN：缓冲区溢出，表示某些消息已经丢失。
   NLMSG_MIN_TYPEK：预留
   
   nlmsg_type 的可取值如下：
   
   NLM_F_REQUEST	用于表示消息是一个请求，所有应用首先发起的消息都应设置该标志  
   NLM_F_MULTI 	用于指示该消息是一个多部分消息的一部分，后续的消息可以通过宏NLMSG_NEXT来获得  
   NLM_F_ACK		表示该消息是前一个请求消息的响应，顺序号与进程ID可以把请求与响应关联起来  
   NLM_F_ECHO	表示该消息是相关的一个包的回传  
   NLM_F_ROOT 	被许多 netlink 协议的各种数据获取操作使用，该标志指示被请求的数据表应当整体返回用户应用，
   			而不是一个条目一个条目地返回。有该标志的请求通常导致响应消息设置 NLM_F_MULTI标志。
   			注意，当设置了该标志时，请求是协议特定的，因此，需要在字段 nlmsg_type 中指定协议类型  
   NLM_F_MATCH 	表示该协议特定的请求只需要一个数据子集，数据子集由指定的协议特定的过滤器来匹配  
   NLM_F_ATOMIC 	指示请求返回的数据应当原子地收集，这预防数据在获取期间被修改  
   NLM_F_DUMP 	未实现  
   NLM_F_REPLACE 	用于取代在数据表中的现有条目  
   NLM_F_EXCL_ 	用于和 CREATE 和 APPEND 配合使用，如果条目已经存在，将失败  
   NLM_F_CREATE 	指示应当在指定的表中创建一个条目  
   NLM_F_APPEND 	指示在表末尾添加新的条目 
   ```

   

3. `nlmsg_flags`：消息标记，它们用以表示消息的类型，如下

   ```c
   /* Flags values */
   
   #define NLM_F_REQUEST       1   /* It is request message.   */
   #define NLM_F_MULTI     2   /* Multipart message, terminated by NLMSG_DONE */
   #define NLM_F_ACK       4   /* Reply with ack, with zero or error code */
   #define NLM_F_ECHO      8   /* Echo this request        */
   #define NLM_F_DUMP_INTR     16  /* Dump was inconsistent due to sequence change */
   
   /* Modifiers to GET request */
   #define NLM_F_ROOT  0x100   /* specify tree root    */
   #define NLM_F_MATCH 0x200   /* return all matching  */
   #define NLM_F_ATOMIC    0x400   /* atomic GET       */
   #define NLM_F_DUMP  (NLM_F_ROOT|NLM_F_MATCH)
   
   /* Modifiers to NEW request */
   #define NLM_F_REPLACE   0x100   /* Override existing        */
   #define NLM_F_EXCL  0x200   /* Do not touch, if it exists   */
   #define NLM_F_CREATE    0x400   /* Create, if it does not exist */
   #define NLM_F_APPEND    0x800   /* Add to end of list       */
   ```

   

4. `nlmsg_seq`：消息序列号，用以将消息排队，有些类似TCP协议中的序号（不完全一样），但是`netlink`的这个字段是可选的，不强制使用；

5. `nlmsg_pid`：发送端口的ID号，对于内核来说该值就是0，**对于用户进程来说就是其socket所绑定的ID号**。



### *`msghdr`* 结构体

```c
struct iovec {                    /* Scatter/gather array items */
     void  *iov_base;              /* Starting address */
     size_t iov_len;               /* Number of bytes to transfer */
 };
  /* iov_base: iov_base指向数据包缓冲区，即参数buff，iov_len是buff的长度。msghdr中允许一次传递多个buff，
    以数组的形式组织在 msg_iov中，msg_iovlen就记录数组的长度 （即有多少个buff）
  */
 struct msghdr {
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



### *`Netlink`* 常用宏

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
#define NLMSG_PAYLOAD(nlh,len) ((nlh)->nlmsg_len - NLMSG_SPACE((len)))
```



### 几个结构体关系图

<img src="https://yqfile.alicdn.com/7eb46431a86d05b6a61b62f3317ee3a53302252b.png" style="zoom: 50%;" />



![](https://ask.qcloudimg.com/http-save/yehe-1158208/6yybxzkisi.png?imageView2/2/w/1620)





## Tips

### [memset()与初始化](https://www.cnblogs.com/cyx-b/p/12515910.html)





```
        // 数据太大了，则需要分片处理
        if (frame_size < 63)
        {
            int times = (frame_size / 1024 / 63) + 1;
            // printf("the frame needs to be divided by %d times\n", times);
            for (int i = 0; i < times; i++)
            {
                /* 构建 nlmsghdr */
                struct nlmsghdr *message_header;
                message_header = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD + sizeof(struct nlmsghdr)));
                memset(message_header, 0, sizeof(struct nlmsghdr));
                struct iovec message_iov;
                memset(&message_iov, 0, sizeof(struct iovec));
                if (i == times - 1)
                {
                    message_header->nlmsg_len = frame_size - (i)*MAX_PAYLOAD + sizeof(struct nlmsghdr);
                    message_header->nlmsg_type = NLMSG_DONE;
                }
                else
                {
                    message_header->nlmsg_len = MAX_PAYLOAD + sizeof(struct nlmsghdr);
                    message_header->nlmsg_type = NLM_F_MULTI;
                }
                message_header->nlmsg_flags = 0;
                message_header->nlmsg_seq = 0;
                message_header->nlmsg_pid = getpid();
                memcpy(NLMSG_DATA(message_header), frame + (i * MAX_PAYLOAD), message_header->nlmsg_len - sizeof(struct nlmsghdr));
                /* 构造发送信息结构体 */
                message_iov.iov_len = message_header->nlmsg_len;
                message_iov.iov_base = (void *)message_header;

                vmac_priv.msg.msg_iov = &message_iov;
                vmac_priv.msg.msg_iovlen = 1;
                sendmsg(vmac_priv.sock_fd, &vmac_priv.msg, 0);
                free(message_header);
            }
        }


        /* 构建 nlmsghdr */
        struct nlmsghdr *message_header;
        message_header = (struct nlmsghdr *)malloc(NLMSG_SPACE(frame_size + sizeof(struct nlmsghdr)));
        memset(message_header, 0, sizeof(struct nlmsghdr));
        struct iovec message_iov;
        memset(&message_iov, 0, sizeof(struct iovec));
        message_header->nlmsg_len = frame_size + sizeof(struct nlmsghdr);
        message_header->nlmsg_type = NLMSG_DONE;
        message_header->nlmsg_flags = 0;
        message_header->nlmsg_seq = 0;
        message_header->nlmsg_pid = getpid();
        memcpy(NLMSG_DATA(message_header), frame, frame_size);
        /* 构造发送信息结构体 */
        message_iov.iov_len = message_header->nlmsg_len;
        message_iov.iov_base = (void *)message_header;

        vmac_priv.msg.msg_iov = &message_iov;
        vmac_priv.msg.msg_iovlen = 1;
        sendmsg(vmac_priv.sock_fd, &vmac_priv.msg, 0);
        free(message_header);

```









