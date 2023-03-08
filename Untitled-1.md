openocd

openocd版本

https://github.com/xpack-dev-tools/openocd-xpack/releases/download/v0.10.0-13/xpack-openocd-0.10.0-13-linux-x64.tgz

更改为更新的

[Release xPack OpenOCD v0.10.0-14 · xpack-dev-tools/openocd-xpack · GitHub](https://github.com/xpack-dev-tools/openocd-xpack/releases/tag/v0.10.0-14)

新下载了东西，若显示没有，则重新打开终端

# vmac userspace

重要数据结构

```c
/**
 * @brief      vmac frame information, buffer and interest name with their lengths, respectively.
 */
struct vmac_frame
{
    char* buf;    
    uint16_t len;
    char* InterestName;
    uint16_t name_len;
};


/**
 * @brief      meta data to provide guidance to V-MAC configurations
 */
struct meta_data
{
    uint8_t type;
    uint16_t seq;
    double rate; //影响到什么
    uint64_t enc;
};
/**
 * @brief      V-MAC userspace's library internal use configurations. V-MAC 用户空间的库内部使用配置。
 */
struct vmac_lib_priv
{
    struct hash* names;
    struct sockaddr_nl src_addr,dest_addr;
    /* TX structs */
    struct nlmsghdr *nlh;
    struct iovec iov;
    struct msghdr msg;

    /* RX structs */
    struct nlmsghdr *nlh2;
    struct iovec iov2;
    struct msghdr msg2;

    uint64_t digest64; 
    uint8_t fixed_rate;
    void (*cb)();
    char msgy[2000]; /* buffer to store frame */
    int sock_fd;
    pthread_t thread;
    char key[16];    
};
```

注册时就创建接收线程recvcmac

pthread_create(&vmac_priv.thread, NULL, recvvmac, (void*)0);

vmac_priv结构体中存了数据和一些控制信息以及callback funtion调用地址。问题是怎么callback？？？

main函数调用run_vmac。如果是producer仅仅把producer=1，如果是consumer

设置running2=1， 并且创建发送兴趣帧的线程

pthread_create(&consumerth, NULL, vmac_send_interest, (void *)0);

vmac_send_interest函数

设置数据的名字，兴趣包的buf内容

vmac_frame 兴趣包的数据帧设置buffer，len，数据名称，数据名称长度

meta_data 控制设置type为兴趣帧， 速率为6.5。 不明白速率的值怎么规定。

调用send_vmac(&frame, &meta);

使用数据结构 struct timespec 获取时间

打印new run （每发一次兴趣包就是一轮。）

打印时间戳，方便后续计算时延。时间戳不需要放到控制结构meta_data里面

recvvmac函数

作用: 从内核中获取数据, 换成frame和meta形式，调用callback函数，进行数据分析

callback函数

作用：分析

若是接收到的是兴趣帧，打印类型、序列号(seq)、时间戳、data内容。以及创建线程 pthread_create(&sendth, NULL, vmac_send_data, (void *)0);

若是接收到的是数据帧，打印 type 、seq 以及 计算打印googdput、 timediff、loss

free掉frame和meta的内存空间

vmac_send_data函数

作用：创建数据帧并发送给请求者

running2 =1            ??? 不为1会如何

sleep（15）

创建frame和meta 赋值frame数据 

发送数据50000份

    调用send_vmac(&frame, &meta);

running2 = 0

### **send_vmac函数**

作用：发送vmac frame 到 vmac内核模块

数据率映射到rix表

帧名字哈希

将meta的类type、seq以及哈希值和速率放到control结构体中

并将control、data依次放到nlh中，所以nlh里面包含control和data，取data的时候需要注意，从control处开始

memcpy(NLMSG_DATA(vmac_priv.nlh), &txc, sizeof(struct control)); // 头部信息

memcpy(NLMSG_DATA(vmac_priv.nlh) + sizeof(struct control), frame->buf, frame->len); // 消息体信息

设置nlh、iov长度，发送msg//注意这里的nlh iov msg的关系

sendmsg(vmac_priv.sock_fd, &vmac_priv.msg, 0);

TODO：+=100调试

TODO：去掉15sleep

? running2 

！skb->data指向什么？用户端传下去的是msg类型。 nlh = (struct nlmsghdr *) skb->data;因此skb->data指向nlh，nlmsghdr类型

Bug：前三位乱码

1为什么%c没有打印出来， 因为打印方式错误，指错地方了

2传abc的ascii码为什么还是0 ，因为打印方式错误，指错地方了

3tx的数据都就直接变成h a    传下来的时候就是ha开头了

! 0的ascii是48 

?发送的时候怎么获取nlh数据

# 

# NETLINK

老子数到三，再分心，老子掐了你

Netlink 相对于**系统调用**，**ioctl** 以及 **/proc文件系统**而言具有以下优点：

1. netlink使用简单，只需要在include/linux/netlink.h中增加一个新类型的 netlink 协议定义即可,(如 **#define NETLINK_TEST 20** 然后，内核和用户态应用就可以立即通过 socket API 使用该 netlink 协议类型进行数据交换);

2. netlink是一种异步通信机制，在内核与用户态应用之间传递的消息保存在<mark>socket缓存队列中，发送消息只是把消息保存在接收者的socket的接收队列，而不需要等待接收者收到消息；</mark>

3. 使用 netlink 的内核部分可以采用模块的方式实现，使用 netlink 的应用部分和内核部分没有编译时依赖;

4. netlink 支持多播，内核模块或应用可以把消息多播给一个netlink组，属于该neilink 组的任何内核模块或应用都能接收到该消息，内核事件向用户态的通知机制就使用了这一特性；

5. 内核可以使用 netlink 首先发起会话;

**struct net **网络设备命名空间指针

**struct sock**是套接口再网络层的表示

**struct sk_buff**结构是linux网络代码中主要的数据结构，它管理和控制接受或发送数据包的信息。

**struct sockadrr_nl**是netlink通信地址

![](/home/bailey/图片/1.webp)

**struct netlink_kernel_cfg netlink**是<mark>配置结构体</mark>

**struct nlmsghdr**是netlink提供的协议头， netlink协议是**面向消息**的****，***需要定义自己的协议。自定义协议按照协议头格式填充协议头内容，并定义自己的payload，通常自定义的协议体包含自定义协议头和额外的属性。***

![](/home/bailey/图片/2.webp)

</mark>

对于该结构，我们更需要关注的是前三个变量参数，对于netlink数据包来说其中msg_name指向的就是目的sockaddr_nl地址结构实例的首地址，iov_base指向的就是消息实体中的nlmsghdr消息头的地址，而iov_len赋值为nlmsghdr中的nlmsg_len即可（消息头+实际数据）。

**struct msghdr**

```c
struct msghdr {
    void        *msg_name;    /* ptr to socket address structure */
    int        msg_namelen;    /* size of socket address structure */
    struct iov_iter    msg_iter;    /* data */
    void        *msg_control;    /* ancillary data */
    __kernel_size_t    msg_controllen;    /* ancillary data buffer length */
    unsigned int    msg_flags;    /* flags on received message */
    struct kiocb    *msg_iocb;    /* ptr to iocb for async requests */
};
```

（1）msg_name：指向数据包的目的地址；

（2）msg_namelen：目的地址数据结构的长度；

（3）msg_iov：消息包的实际数据块，定义如下：

（4）msg_control：消息的辅助数据；

（5）msg_controllen：消息辅助数据的大小；

（6）msg_flags：接收消息的标识。

**struct iovec**

```c
1 struct iovec
2 {
3     void *iov_base;    /* BSD uses caddr_t (1003.1g requires void *) */
4     __kernel_size_t iov_len; /* Must be size_t (1003.1g) */
5 };
```

(1)iov_base：消息包实际载荷的首地址；

(2)iov_len：消息实际载荷的长度。



**struct sockaddr_nl**

```text
struct sockaddr_nl
{
    sa_family_t nl_family; /*该字段总是为AF_NETLINK */
    unsigned short nl_pad; /* 目前未用到，填充为0*/
    __u32 nl_pid; /* process pid */
    __u32 nl_groups; /* multicast groups mask */
};
```

**struct nlmsghdr**

Netlink的报文由消息头和消息体构成，struct nlmsghdr即为消息头。消息头定义在文件里，由结构体nlmsghdr表示：

```text
struct nlmsghdr
{
    __u32 nlmsg_len; /* Length of message including header */
    __u16 nlmsg_type; /* Message content */
    __u16 nlmsg_flags; /* Additional flags */
    __u32 nlmsg_seq; /* Sequence number */
    __u32 nlmsg_pid; /* Sending process PID */
};
```

**nlmsg_len**：整个消息的长度， 按字节计算。包括了Netlink消息头本身。

**nlmsg_type**：消息的类型，即是数据还是控制消息。目消息状态，内核在include/uapi/linux/netlink.h中定义以下4种通用的消息类型，如下：

a) NLMSG_NOOP-空消息，什么也不做；

b) NLMSG_ERROR-指明该消息中包含一个错误；

c) NLMSG_DONE-如果内核通过Netlink队列返回了多个消息，那么队列的最后一条消息的类型为NLMSG_DONE，其余所有消息的nlmsg_flags属性都被设置NLM_F_MULTI位有效。

d) NLMSG_OVERRUN-暂时没用到。

**nlmsg_flags：**附加在消息上的额外说明信息。如上面提到的NLM_F_MULTI。

**nlmsg_pid：**：发送端口的ID号，内核的ID号为0，对于用户进程就是其socket所绑定的ID号

```c
struct netlink_kernel_cfg {
    unsigned int    groups;
    unsigned int    flags;
    void        (*input)(struct sk_buff *skb);-------------input回调函数
    struct mutex    *cb_mutex;
    int        (*bind)(struct net *net, int group);
    void        (*unbind)(struct net *net, int group);
    bool        (*compare)(struct net *net, struct sock *sk);
};
```

netlink消息处理常用宏

```c
#define NLMSG_ALIGNTO    4U
#define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )------------------用于得到不小于len且字节对齐的最小数值
#define NLMSG_HDRLEN     ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))----------------------netlink头部长度
#define NLMSG_LENGTH(len) ((len) + NLMSG_HDRLEN)-------------------------------------------计算消息数据len的真实消息长度，消息体+消息头
#define NLMSG_SPACE(len) NLMSG_ALIGN(NLMSG_LENGTH(len))------------------------------------返回不小于NLMSG_LENGTH(len)且字节对齐的最小数值
#define NLMSG_DATA(nlh)  ((void*)(((char*)nlh) + NLMSG_LENGTH(0)))-------------------------用于取得消息的数据部分的首地址，设置和读取消息数据部分时需要使用该宏
#define NLMSG_NEXT(nlh,len)     ((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), \
                  (struct nlmsghdr*)(((char*)(nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))------用于得到下一个消息的首地址，同时len变为剩余消息的长度
#define NLMSG_OK(nlh,len) ((len) >= (int)sizeof(struct nlmsghdr) && \
               (nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && \
               (nlh)->nlmsg_len <= (len))--------------------------------------------------判断消息是否>len
#define NLMSG_PAYLOAD(nlh,len) ((nlh)->nlmsg_len - NLMSG_SPACE((len)))---------------------用于返回payload的长度
```

```c
//netlink消息常用API
int netlink_unicast(struct sock *ssk, struct sk_buff *skb,
            u32 portid, int nonblock)-----------------------------------------------用来发送单播信息。
ssk：netlink socketskb：skb buff指针portid：通信端口号nonblock：表示该函数是否为非阻塞，如果为1，该函数将在没有接收缓存可利用时立即返回；如果为0，该函数在没有接收缓存可利用时定是睡眠。
int netlink_broadcast(struct sock *ssk, struct sk_buff *skb, u32 portid,
              u32 group, gfp_t allocation)------------------------------------------用来发送多播信息。ssk：netlink socket，netlink_kernel_create()返回值skb：内核skb buffportid：通信端口号group：是所有目标多播组对应掩码的OR操作的合值allocation：指定内核内存分配方式，通常GFP_ATOMIC用于中断上下文，而GFP_KERNEL用于其他场合。这个参数的存在是因为该API可能需要分配一个或多个缓冲区来对多播消息进行clone。

  static inline struct nlmsghdr *nlmsg_hdr(const struct sk_buff *skb)----------------从sk_buff->data获取struct nlmsghdr数据结构。  {      return (struct nlmsghdr *)skb->data;  }static inline struct sk_buff *nlmsg_new(size_t payload, gfp_t flags)---------------创建len大小的struct sk_buff。
{
    return alloc_skb(nlmsg_total_size(payload), flags);
}

static inline struct nlmsghdr *nlmsg_put(struct sk_buff *skb, u32 portid, u32 seq,
                     int type, int payload, int flags)----------------------------将一个新的netlink消息加入到skb中。如果skb无法存放消息则返回NULL。
static inline void nlmsg_free(struct sk_buff *skb)--------------------------------释放nlmsg_new()创建的skb。
{
    kfree_skb(skb);
}

static inline void *nlmsg_data(const struct nlmsghdr *nlh)------------------------根据nlmsghdr指针获取对应的payload。
{
    return (unsigned char *) nlh + NLMSG_HDRLEN;
}

static inline struct nlmsghdr *nlmsg_next(const struct nlmsghdr *nlh, int *remaining)---获取消息流中下一个netlink消息。

static inline void nlmsg_end(struct sk_buff *skb, struct nlmsghdr *nlh)
```

**用户空间Netlink socket API

```c
int socket(int domain, int type, int protocol);------------------该函数用来创建一个套接字，并返回一个描述符，该描述符可以用来访问该套接字。protocol参数设置为0表示使用默认协议。

int bind( int socket, const struct sockaddr *address, size_t address_len);--把通过socket()创建的套接字命名，从而让它可以被其他进程使用。

int sendto(int sockfd, void *buffer, size_t len, int flags, struct sockaddr *to, socklen_t tolen);----把缓冲区buffer中的信息送给制定的IP端口程序，buffer存放将要发送的数据，len是buffer长度，to是要发送数据到的程序IP端口，tolen是to参数长度。

int recvfrom(int sockfd, void *buffer, size_t len，int flags, struct sockaddr *src_from, socklen_t *src_len); --把发送给程序的信息存储在缓冲区buffer中，并记录数据来源的程序IP端口。buffer存放接收的数据，len是buffer长度，src_from是数据来源程序IP端口，src_len是src_from长度。
```

1、创建socket

fd = socket(PF_NETLINK, SOCK_RAW, VMAC_USER);

2、绑定socket //绑定源地址和fd，那目的地址怎么用

bind( vmac_priv.sock_fd, (struct sockaddr *)&vmac_priv.src_addr, sizeof(vmac_priv.src_addr))；

3、发送netlink消息

sendmsg(vmac_priv.sock_fd, &vmac_priv.msg, 0);  //参数是套接字和消息体

4、接收netlink消息

recvmsg(vmac_priv.sock_fd, &vmac_priv.msg2, 0); //参数依旧是套接字和消息体

**内核空间Netlink socket API**

1、创建netlink socket

```c
struct netlink_kernel_cfg cfg = {.input = nl_recv};
nl_sk =netlink_kernel_create(&init_net, VMAC_USER, &cfg);
```

```text
struct sock *netlink_kernel_create(struct net *net,
                                   int unit，
                                   void (*input)(struct sk_buff *skb),);
```

参数说明：

(1) net：是一个网络名字空间namespace，在不同的名字空间里面可以有自己的转发信息库，有自己的一套net_device等等。默认情况下都是使用 init_net这个全局变量。

(2) unit：表示netlink协议类型，如NETLINK_TEST、NETLINK_SELINUX，也可以是自定义的协议类型。

(3) groups：多播地址。

(4) input：为内核模块定义的netlink消息处理函数，当有消 息到达这个netlink socket时，该input函数指针就会被引用，且只有此函数返回时，调用者的sendmsg才能返回。

2、发送消息 

        -单播netlink_unicast

```c
//使用案例
nlmsg_unicast(nl_sk, skb_out, pidt); //其实nlmsg_unicast调用了netlink_unicast
```

```c
//函数定义
int netlink_unicast（struct sock *ssk, struct sk_buff *skb, u32 pid, int nonblock）
```

参数说明：

(1) ssk：为函数 netlink_kernel_create()返回的socket。

(2) skb：存放消息，它的data字段指向要发送的netlink消息结构，而 skb的控制块保存了消息的地址信息，宏NETLINK_CB(skb)就用于方便设置该控制块。

(3) pid：为接收此消息进程的pid，即目标地址，如果目标为组或内核，它设置为 0。

(4) nonblock：表示该函数是否为非阻塞，如果为1，该函数将在没有接收缓存可利用时立即返回；而如果为0，该函数在没有接收缓存可利用定时睡眠。

        

        -多播netlink_broadcast

```c
int netlink_broadcast(struct sock *ssk, struct sk_buff *skb, u32 pid, u32 group, gfp_t allocation)
```

3、释放netlink socket

```c
//使用案例
netlink_kernel_release(nl_sk);
```

**内核接收应用层的消息**

当进程有数据发送过来时，内核部分会接收数据，上送的包是struct sk_buff *skb,我们可以通过netlink提供的一系列操作函数来获取消息头以及数据。  

消息头 = nlmsg_hdr(skb)；  

消息数据 = NLMSG_DATA(nlh)；



# 内存分配

1字节对齐，为了不降低取数据速率，能够一次性取到

2分配内存的函数收集

？为什么struct nlmsghdr *nlh;要定义成指针

```c
struct sockaddr_nl saddr, daddr; // sockaddr 地址

memset(&saddr, 0, sizeof(saddr));
memset(&daddr, 0, sizeof(daddr));

struct nlmsghdr *nlh = NULL;     // nlh
nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PLOAD));
memset(nlh, 0, sizeof(struct nlmsghdr));
//umsg 是用户层消息数据
memcpy(NLMSG_DATA(nlh), umsg, strlen(umsg));
```

# vmac

再分？

### ieee80211_rx_vmac

作用：接收驱动模块传来的消息，并过滤出vmac帧

将接收到的skb抽去ieee头部
