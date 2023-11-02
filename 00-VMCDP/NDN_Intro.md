 

data包本身不需要进行路由转发， 它只是简单地沿着Interest包被传输的相反路径返回。

在NDN的通信是由接收端驱动的. 例如消费者请求数据时发出INTEREST包,它带有标识所需的**数据名称**.例如"/reward/worker",路由器记住请求传入的接口.然后通过**FIB**查找名称转发Interest包

#### Naming 

NDN采用符号“/”对名字进行层级划分,如一段视频数据的名称可能为"/prac/videos/widget.mpg".结构化的名字能让网络中的数据包携带更多的信息,同时能更好地满足移动性要求.各种应用可根据实际需求设计出符合自身情形的具体命名模式,如一个会议系统可以利用前缀"/sample/conference"来限定数据包的路由范围,同一个应用程序甚至可以利用不同的前缀来区分不同的处理模块.

每一跳路由节点在请求包经过时保存相应的状态信息,数据包根据这些信息回溯至请求方,即内容消费者,在回溯过程中每个数据包在每一条消耗一个相应的请求包.每个NDN节点包含三张表:转发信息表(FIB,Forwarding information base), 内容仓库(CS, content store)和转发请求表(PIT,pending interesting table).

> Q1：前缀放在哪个结构中
>
> Q2：还有前缀的命名规则，斜杠不属于data name的一部分，那什么时候加上斜杠？
>
> 

消费者可以结合第一个数据包得到的信息以及同生产商协定好命名约定，制定命名的后半部分，发出请求。

命名唯一性，

namemingspace | Application name | Destination  | Function parameters



#### FIB

**作用:**用来将请求数据包路由至潜在的匹配数据源

**结构:**<prefix, face list> face list中记录了入口以及出口

**用法:**采用最长匹配进行查询.,*NDN支持一条路由项同时指向多个潜在的匹配接口*,即允许从多个源获得数据,实行并行查询.FIB中的信息通过手动配置,或基于名字的路由协议进行更新.

#### CS

**作用:** 类似于IP路由中的内存缓存, 在IP缓存机制中, 由于采用点对点对话,数据传输表现为流的形式,中间路由器只进行路由转发,当一个数据包发送出去之后即失效,缓存区的内容无法重复利用.而NDN路由器端可通过名字识别内容并做处理,也可通过内容名字对数据进行复用,因此尽可能长时间地保存接收到的数据,最大化共享,降低带宽消耗.

**结构:**<name, data>

**用法:**内存有限的情况下,如何设置规则去替换内容



#### PIT

**作用:** 保存兴趣包的上行信息,以保证当接收到数据包时能将其正确地返回给请求方. PIT表可以实现请求数据包的聚合

**结构:**<prefix, face>

**用法:** 数据发送至请求接口后需要删除与该数据匹配的PIT表项,对于长时间未得到返回的数据表项则在超时后清除.来自不同接口的相同请求将在PIT中进行合并.

#### LET（VMAC）

数据通过ICN层提供的data name来过滤帧。然而ICN的数据名称很长，不适合直接嵌入到有限大小的报头中。我们使用hash函数把名字哈希到固定长度.LP层把ICN层的大数据包分成MAC MTU长度大小的数据单元。然后VMAC重组成大数据包

对于每个发出去的兴趣帧，V-MAC都会将其添加到LET表中，用来记录我请求过什么数据。当有数据到来的时候，需要将数据名对比LET表项，若存在，则pass to LP层重组，否则drop掉。

我们对编码匹配做出两点评论：一个encoding用来配对多个frame。但是在NDN中配对完data就移除PIT中的条目。由于许多数据包是多个MTU大小，一个“滞留”的编码条目避免了相同兴趣帧的重复传输，从而增加了数据帧的传输时间。

LET中的每一个编码条目都有一个到期时间，到期后将被删除。理想情况下，当条目过期时，所有（或大多数）匹配的数据都应该返回。到期时间应根据这一估计进行设定。如果某些数据帧还没有回来，则数据包不能完全重新组装。LP可能在编码到期时再次发送Interest。这重置了LET条目超时，以允许跟多的匹配数据帧到来。

### 数据传输过程:

NDN路由器将interest和数据都保存一段时间,当多个请求相同的数据的interest从下游收到数据, 则只发送第一个interest到数据源。

NDN路由器将interest 和数据都保存一段时间， 当多个请求相同数据的interest从下游收到数据， 则只发送第一个interest到数据源，然后路由器将其存储在PIT表中，并记录传入的接口集合。当数据包到达时，路由器查找到匹配的PIT条目，并将数据从这些接口转发出去，然后删除相应的条目。并将数据缓存到CS中。

# NDN-Hippo流媒体点播系统的设计

SM：资源管理服务器：负责管理媒体资源在全网的调度（上线、下线和拷贝）和查询

RM：区域管理服务器：按区域划分，每个RM负责管理区域所有资源和节点的调度，RM不仅关心区域内有哪些资源，而且还关心这些资源分布在哪些节点，RM之间是平等分治的，彼此不存在直接的交互

Speer和Dpeer位于底层节点，会自动向归属的RM上报自己的资源信息， 节点的整个生命过程中通过发送心跳包和RM保持联系。

NDN-Hippo没有原来Hippo系统中的RM服务器，利用NDN协议的底层路由机制完成数据资源的定位。

### 命名前缀

1.为了区分interest包的就收响应者， 把interest包分为两种不同的类型， 并标记不同的目标前缀部分。

第一类是没有特定的接收者，一种原因是发送方不知道有哪些用户满足条件的内容，所以只能通过这种方式进行搜索式的获取，interest包可以表达谁含有这个数据块的需求。另一个原因是想从多个用户获取数据，所以要进行多播式的获取。

第二类与第一类相反， 节点在发送这样的interest包之前就已经知道interest会到达某个网络节点并被那里处理。NDN-Hippo中，这样的应用主要出现在服务器之间的交互中，完成特定过程的数据交互。类似于TCP/IP网络下的TCP连接。

2.功能部分：每个interest包的本体部分是因功能而异的。但都符合同一个样式“/功能名/参数列表”,其中参数列表也使用/作为分隔符以方便应用程序进行处理.在客户端应用程序中, 用户收到一个interest后先去除前缀部分,**然后提取功能名字段已选择对应的处理线程.**

### 数据预处理

启动interest机制将这些分片发布到整个系统中

数据流组织:

根据媒体文件的时钟基准进行切分打包.

根据NDN协议的原则, 本文将一个媒体文件上线之前,处理成一下三个格式的数据。meta、data、index

#### data包设计：

包头设计

- post字段：合并标识符，表示这个数据包中是否有一帧buffer跨越了下一个数据包；
- timestamp：表示这个数据包开始的第一帧数据的时间戳。
- offset：表示数据包的第一帧数据的开始位置
- count：表示数据包有多少帧的数据

#### 数据源服务器设计

node：预处理服务器， 节目目录服务器， 客户

在系统中分布式添加数据源服务器时，首先需要向预处理服务器注册，让预处理服务器保存一个前缀记录。新配置的存储服务器开始向预处理服务器发送一个interest包。 

节预处理服务器可以给新服务器分配存储节点的一个注册号。

类TCP/IPsocket库函数 API
