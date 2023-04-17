 

data包本身不需要进行路由转发， 它只是简单地沿着Interest包被传输的相反路径返回。

在NDN的通信是由接收端驱动的. 例如消费者请求数据时发出INTEREST包,它带有标识所需的**数据名称**.例如"/reward/worker",路由器记住请求传入的接口.然后通过**FIB**查找名称转发Interest包

#### Naming 

NDN采用符号“/”对名字进行层级划分,如一段视频数据的名称可能为"/prac/videos/widget.mpg".结构化的名字能让网络中的数据包携带更多的信息,同时能更好地满足移动性要求.各种应用可根据实际需求设计出符合自身情形的具体命名模式,如一个会议系统可以利用前缀"/sample/conference"来限定数据包的路由范围,同一个应用程序甚至可以利用不同的前缀来区分不同的处理模块.

每一跳路由节点在请求包经过时保存相应的状态信息,数据包根据这些信息回溯至请求方,即内容消费者,在回溯过程中每个数据包在每一条消耗一个相应的请求包.每个NDN节点包含三张表:转发信息表(FIB,Forwarding information base), 内容仓库(CS, content store)和转发请求表(PIT,pending interesting table).

现在我的问题是前缀放在哪个结构中

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

NDN路由器将interest和数据都保存一段时间,当多个请求相同的数据的interest从下游收到数据, 则只发送第一个interest到数据源
