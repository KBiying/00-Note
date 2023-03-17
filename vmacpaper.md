LET表的使用

consumer

用户层向内核发送兴趣包后，内核会将兴趣包的名字加入到let表中，作用是记录要请求的数据，并设置有expiration time.

vmac_tx逻辑

如果是兴趣包

先在let表里查找是否有encode，

    如果没有，则新新创建一个entry；

    如果有，则延长expiration time。

vmac头部type设置为interest，添加vmac头部。

如果是数据包

现在let表里查找是否有encode，

    如果没有，则新创建一个entry，（操作同兴趣帧）

    如果有，设置重传？？

    

producer

驱动将接收到的数据传输给vmac，vmac将兴趣包的名字加入let表中，作用是    

# 借鉴点

### Challenges. The V-MAC design must address the following questions: 1) how to eliminate address discovery and group formation for a data-centric MAC layer; 2) how to fit variable sized, possibly long hierarchical data names in limited size frames, while achieving fast matching against large numbers of data names; 3) how to find a suitable representative to send aggregated feedback without prior knowledge or explicit coordination between consumers, and how to ensure efficient air time utilization despite varying numbers of receivers and intensities of background traffic. We eliminate beacons and use a hash based encoding to address questions 1 and 2 (Section III-B), and use a backoff mechanism for question 3 (Section III-C).

> vmac面临的挑战有：1怎么消除发现IP地址以及MAC层的组信息。2如何把编码名字，到一个固定长度3如何找一个合适的代表发feedback
>
> vmac的解决方法是1消除beacons。2别人的hash算法。3backoff mechanism



# 改进点

### 1、实现ICN层

### 2、vmac速度

### 3、“The ICN layer may need to send out Interests periodically to discover nearby data.”





# 实验点

### 1、分包测试

> 论文中提到a link protocol (LP) between ICN and V-MAC breaks it into multiple MTU size units, and V-MAC converts each into a Data frame, carrying the same data name but a different sequence number denoting its position within the packet.意思是分包的功能层就是LP层。VMAC层就将数据包转化为数据帧，并且带有不同的seq number。
>
> The LP reassembles such frames into one Data packet when needed. 接收端的LP层把包合起来

### 2、打印let表

### 3、打印b_seq 是什么

#### 4、consumer发送名字不同的数据包，producer发送不同名字的数据包

### 5、producer拥有两个资源



# A.Design

### 1、健壮性：设计了DACK以及每一个帧使用各使用一个PHY前导码

# B. Data-Centric Frame Filtering

In this section, we describe how we achieve Beaconless Design by leveraging V-MAC frames to carry the beacon’s functionalities and how the Lingering Encoding Table works to support 1:m mapping of Interest/Data frames (instead of 1:1 Internet/Data packet mapping in NDN).

## Beaconless Design.

Wifi有周期性发beacon信号，用来发现节点和成组

vmac header包括了兴趣名以及发送速率

## Lingering Encoding Table.

> Q1: How the Lingering Encoding Table works to support 1:m mapping of Interest/Data frames (instead of 1:1 Internet/Data packet mapping in NDN).
>
> 翻译：关于LET表怎么实现1:m兴趣帧/数据帧映射？NDN里面是1:1映射。什么是mapping？
>
> Q2：The LP layer breaks a long ICN packet with one name into multiple MAC MTU size units, then V-MAC packages them into frames carrying the same encoding but different sequence numbers.“这句没有实现吧，因为vamc设定就是收到一个兴趣帧，就发送数据帧，自己造的数据。如果自己造不了，需要别人提供的数据。这部分没有实现
>
> 翻译：LP层分包，然后vmac封装成帧。
>
> 问题：这是在哪里做的？没找到在哪里分包诶
>
> Q3：For each outgoing Interest frame, V-MAC adds its encoding in a lingering encoding table (LET) to record what data are requested. Each incoming Data frame is compared against the encodings in the LET. If a match is found, the frame will be passed to the LP for reassembly; otherwise it will be dropped.” 
>
> 翻译：每个请求者发送兴趣帧时，都会将请求的数据名存到LET表中。以记录请求的是什么数据，然后当有数据来的时候就查看一下LET表，match就往上传给LP层，合并帧。
>
> 李姐：LET表中存的是需要请求的数据的名称
>
> 问题：使用什么数据来match？encoded name？LP层合并帧的动作在哪里？
>
> Q4:If some Data frames still have not come back, the Data packet cannot be fully reassembled. The LP may send the Interest again right before the encoding expiration.
>
> 翻译：如果有些数据帧还没有到达，LP层就重新发送兴趣帧
>
> 

LET 条目的大小 = ？

encoding size 64bit，用一个hash map存储

哪里做了分包和resemble？



# DACK

“DACK is agnostic to the type of filtering, and can work in both data-centric and address-based networks” 

过滤类型不知道DACK，DACK都可以在data-centric and address-based networks运作。

"After a burst, each receiver prepares a DACK frame."

DACK size < 70Bytes

<img src="/Users/bailey/Library/Application Support/typora-user-images/image-20230220102637367.png" alt="image-20230220102637367" style="zoom:40%;" />

> DACK Frame Format. Figure 2 shows the format of the DACK header, which comes after the PHY (PLCP) header.3 The first byte is FC (frame control), denoting the V-MAC frame type: Interest, Data, or Control (e.g., DACK). In a DACK, the next 8 bytes are the encoding of the packet’s data name, informing the sender which packet’s frames are missing when multiple packets are transmitted concurrently. The b seq field denotes the sequence number of the most recent burst heard by this receiver, used to prevent redundant retransmissions (explained later). The next byte is the number of “holes” (frames missing in a row), then come pairs of left edge and right edge (LE, RE), denoting frame sequence numbers before and after each hole. Data/Interest frames have only a sequence number seq after encoding. A DACK contains holes in a sliding window of multiple bursts. This allows receivers to request missing frames beyond the most recent burst, when they did not have a chance to request them immediately or successfully receive retransmissions. The size of this window is set comparable to and slightly smaller than the retransmission buffer, whose size is based on available memory and how long frames should be kept for retransmission.
>
> 翻译：DACK帧格式：DACK头部在PHY头部后面。第一个字节是control，用来表明帧类型；下个8字节是encoding，表明数据是什么；b_seq表示某次burst的最新seq number，用来防止多余重传。下一个字节是holes，一组连续的丢失的帧的数量。LE和RE是指连续组的头和尾seq。sliding window大小应该就是burst的大小。

?什么时候会触发DACK

当一个接收方丢失了一整串帧时，可能不会立即触发DACK。

？round number（not sequence number）

# retransmission

根据sliding window，缺失了哪个连续seq就重传

Q：重传整个帧还是只是丢失的那一部分帧？如果是后者，那么怎么实现？因为请求的时候是根据数据名字请求的，这表示，请求的是一整个文件，期间丢失的帧，怎么请求？而且重传的时候，每个接收者都会收到重传的帧，但有一些接受者并不需要这些帧的时候，怎么处理。这时候就是window发挥作用了

### Window size

"The window size is multiple bursts, thus allowing missing frames be requested and transmitted more than once to counter DACK or retransmission losses. The window size is limited by available kernel memory, and set to 10 bursts empirically in the prototype."



# backoff mechanism

1、Before transmitting its DACK, each receiver waits for a backoff time T = ατ， where α is the slot size. τ是burst成功传输的帧数量。

2、When other receivers overhear a DACK, they cancel their own backoff timers, preventing excessive DACKs. 当其他接受者监听到DACK帧，那就去萧条自己的backoff timers，为了避免过多的DACK在空中飞来飞去。

3、slot size计算方法：we leverage the sender’s back to back Data frame transmissions to measure the slot size: a receiver uses the time difference between receiving two frames of consecutive sequence numbers, minus the Data frame transmission time (dividing the frame size by data rate, while accounting for preamble size and rate). This depicts how quickly the sender can grab the medium.

接受者使用接受两个连续seq numbers的时间差，减去数据帧传输时间（用帧的大小除以数据率）

？alphe的动态适应算法

？vmac怎么计算数据率

？common start of backoff

？DACK Cancellation Policy，时间消除那块看看

# 内核链表

### list_head <内核链表的核心> 

```c
struct list_head {
   struct list_head *next, *prev;
};
```

### INIT_LIST_HEAD <初始化链表>

```c
//用例
struct person
{
    struct list_head list;
    int age;
};

struct person person1;
struct list_head *pos;
INIT_LIST_HEAD(&person1.list); // 初始化链表

```

前面说了 list_head 只有指针域，没有数据域，如果只是这样就没有什么意义了。所以我们需要创建一个宿主结构，然后再再此结构包含 list 字段，宿主结构，也有其他字段（进程描述符，页面管理结构等都是采用这种方法创建链表的）。假设定义如下：

```c
struct my_data_list {
    int data ;
    struct list_head list; /* list head , 这个至关重要，后期遍历通过container_of 解析my_data_list 地址 */
};
```

### list_entry <获得含链表入口的结构体首地址>

### list_add <从链表头插入>

```c
struct person
{
    struct list_head list;
    int age;
};
struct person list;
struct person person1;
list_add(&p.list, &person1.list);
```



### list_add_tail <从链表尾插入>



### list_for_each <遍历链表>

因此之遍历链表不删除结点时，可以使用list_for_each()，而当由删除结点操作时，则要使用list_for_each_safe()

### list_for_each_safe <遍历链表>

### list_del_init <删除节点>







# 	内核内存分配

### kmalloc

# Printk

树莓派上可以通过/var/log/kern.log查看demsg中的信息

