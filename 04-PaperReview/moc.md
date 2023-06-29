### Background

multiplayer online game(MOG) using Named Data Networking (NDN) as the primary communication mechanism.

Taxonomy: Modern MOGs were also examined in order to develop a taxonomy of the data generated in MOGs. Each of the categories identified by the taxonomy have **different characteristics and require different networking solutions** to provide a consistent view of the game world to all players. 

`GitHub：github.com/stefano-lupo/ndn-thesis.`

### Orin tech

#### PACKAGE

<img src="/home/bailey/图片/Screenshot_20230627_112517.png" alt="Screenshot_20230627_112517" style="zoom:55%;" />

###### INTEREST PACKAGE

**CanBePrefix:** Indicates whether this Interest can be satisfied by a Data packet with a name such that the Interest packet’s Name field is a prefix of the Data packet’s Name field. **This is useful when consumers do not know the exact name of a Data packet they require.** If this field is omitted, the Name of the Data packet must exactly match the Name of the Interest packet.

指示此兴趣能否被具有名称的数据包满足，使得该兴趣数据包的Name字段是该数据包Name字段的前缀。当消费者不知道他们需要的数据包的确切名称时，这很有用。如果省略此字段，则数据包的名称必须与兴趣包的名称完全匹配。

**MustBeFresh:** Indicates whether this Interest packet can be satisfied by a content store (CS) entry whose FreshnessPeriod has expired

**ForwardingHint: **This defines where the packet should be forwarded if there is no corresponding entry in the forwarding information base (FIB). Due to the limited capacity of the FIB, there is a finite number of name prefixes that can be stored. This field typically represents an ISP prefix and is used to tackle the routing scalability issues present in NDN .

**Nonce: **A randomly generated 4-octet long byte string. The combination of the Name and Nonce should uniquely identify an Interest packet. This is used to detect looping Interests.

一个随机生成的4 - 8进制长字节字符串。Name和Nonce的组合应该唯一地标识一个Interest包。这被用来检测**循环兴趣**。

**InterestLifetime: **The length of time in milliseconds before the Interest packet times out. This is defined on a hop-by-hop basis, meaning that an Interest packet will time out at an intermediate node InterestLifetime milliseconds after reaching that node.

兴趣包超时前以毫秒为单位的时间长度。这是在逐跳的基础上定义的，这意味着一个兴趣包在到达该节点后会在一个中间节点InterestLifetime毫秒处超时。

###### **DATA** **PACKAGE**

**ContentType: **Defines the type of the content in the packet. This field is an enumeration of four possible values: BLOB, LINK, KEY or NACK. LINK and NACK represent NDN implementation packets, while BLOB and KEY represent actual content packets and cryptographic keys respectively.

定义包中内容的类型。该字段列举了4种可能的值：BLOB、LINK、KEY或NACK。LINK和NACK表示NDN实现数据包，BLOB和KEY分别表示实际内容数据包和密钥。

**FreshnessPeriod: **This represents the length of time in milliseconds that the Data packet should be considered fresh for. As Data packets are cached in the CS, **this field is used to approximately specify how long this packet should be considered the newest content available for the given Name.** Consumers can use the MustBeFresh field of the Interest packets to specify whether they will accept potentially stale cached copies of a piece of Data and the "staleness" of the Data is defined using the FreshnessPeriod field.

这代表了数据包应该被认为是新鲜的毫秒的时间长度。由于数据包缓存在CS中，因此该字段用于近似指定该数据包在多长时间内被认为是给定名称可用的最新内容。消费者可以使用兴趣包的MustBeFresh字段来指定他们是否会接受一段数据的潜在过期缓存副本，并且使用FreshnessPeriod字段来定义数据的"过期"。

**FinalBlockId: **This is used to identify the ID of the final block of data which has been fragmented.

该方法用于识别已被分片的最终数据块的ID。

**Content: **This is an arbitrary sequence of bytes which contains the actual data being transported.

这是一个任意的字节序列，其中包含正在传输的实际数据。

**Signature: **This contains the cryptographic signature of the Data packet.

#### BASIC OPERATION

The **FIB** is used to determine which interface(s) an incoming Interest should be forwarded upstream through. This is similar to FIB used on IP routers, however, NDN supports multipath forwarding (see section 2.1.5), enabling a single Interest to be sent upstream through multiple interfaces.

FIB用于确定传入Interest应通过哪个接口( s )向上游转发。这与IP路由器上使用的FIB类似，但是NDN支持多路径转发(见2.1 . 5节)，使得单个Interest可以通过多个接口向上游发送。

The PIT stores the names of Interests and the interface on which the Interest was received, for Interests which have been forwarded upstream, but not yet had any Data returned.

PIT存储了兴趣点的名称和接收兴趣点的接口，对于已经被转发到上游，但尚未返回任何数据的兴趣点。

The CS is used to cache Data packets received in response to Interests expressed. The CS allows any NDN node to satisfy an interest if it has the corresponding Data packet, even if it is not the producer itself. As with all caches, the CS is subject to a replacement policy, which is typically Least Recently Used (LRU)

CS用于缓存接收到的响应兴趣表达的数据包。CS允许任何NDN节点满足兴趣，如果它有相应的数据包，即使它不是生产者本身。与所有缓存一样，CS受到替换策略的约束，该策略通常为最近最少使用( LRU )策略。

NDN also uses a Face abstraction. An NDN Face is a link over which NDN Interest and Data packets can flow. A Face can represent a physical interface such as a network card, or a logical interface such as an application producing data under a certain namespace

NDN也使用了Face抽象。NDN Face是NDN兴趣包和数据包可以在其上流动的链路。Face可以表示网卡等物理接口，也可以表示在某个命名空间下产生数据的应用程序等逻辑接口.

<img src="/home/bailey/图片/Screenshot_20230627_114820.png" alt="Screenshot_20230627_112517" style="zoom:65%;" />

<img src="/home/bailey/图片/Screenshot_20230627_115404.png" alt="Screenshot_20230627_115404" style="zoom:65%;" />

description:

On receipt of an Interest, the CS is checked to see if there is a cached copy of the Data corresponding to the name in the Interest. If a copy exists with the appropriate freshness, the Data packet can be sent back over the requesting Face and the Interest packet is satisfied. If there is no cached copy of the Data in the CS, the PIT is then checked. If a PIT entry containing the Interest name exists, this indicates that an equivalent Interest packet has already been seen and forwarded upstream. In this case, the Interest packet is not forwarded upstream a second time. Instead, the requesting face is added to the list of downstream faces in the PIT entry. This list of faces represents the downstream links which are interested in a copy of the Data. If there is no PIT entry, the FIB is then queried to extract the next hop information for the given Interest. If there is no next hop information, a NACK is typically returned. In some implementations, the Interest could also be forwarded based on the ForwardingHint if one is present. If an FIB entry is present, a PIT entry for the given Interest is created and the packet is forwarded upstream, using the next hop contained in the FIB entry. The operation of an NDN node on receipt of a Data packet is shown in figure 2.3.

#### NAMING

The names of Data packets can be more specific than the names of the Interest packets which solicit them. That is, the Interest name may be a **prefix** of the returned Data name. For example, a producer of sequenced data may respond to Interests of the form /ndn/test/<sequence-number>. In this case, the producer would register the prefix /com/test in order to receive all Interests, regardless of the sequence number requested. However,a consumer may not know what the current sequence number is. Thus,a convention could be agreed upon such that a consumer can express an interest for /com/test and the Data packet that will be returned will be named /com/test/<next-producer-

sequence-number>, allowing the consumer to catch up to the current sequence number. This method is used in the synchronization protocol outlined in section 4.4.

数据包的名称可以比征集它们的兴趣包的名称更具体。即兴趣名可能是返回的数据名的前缀。例如，一个测序数据的生产者可能会对形式为/ ndn / test /的Interests做出回应。在这种情况下，生产商会注册前缀/ com / test以接收所有的兴趣，而不管请求的序列号是多少。然而，消费者可能不知道当前的序列号是什么。因此，可以约定一个约定，使消费者可以表达对/ com / test的兴趣，并将要返回的数据包命名为/ com / test /，从而使消费者能够赶上当前的序列号。该方法在4.4节所述的同步协议中得到了应用。

#### SYNC   

怎么同步数据集？

A common requirement in distributed, P2P environments is for nodes to read and write to a shared dataset. An example of a shared dataset is a chat room in which all participants can send messages to all other participants. In order to provide all participants with a common view of the messages sent to the chat room, the underlying dataset must be synchronized by a dataset synchronization protocol (DSP). The importance of DSPs is amplified in an NDN context as most applications are developed with a distributed P2P architecture in mind. This is done to enable high scalability through the exploitation of the features offered by NDN such as in-network caching and native multicast. As such, a lot of research into the area of DS in NDN has been conducted and one of the goals of the research is to abstract away the need for NDN application developers to consider DS。

在分布式P2P环境中，一个常见的需求是节点对共享数据集进行读写操作。共享数据集的一个例子是一个聊天室，在这个聊天室中，所有参与者都可以向所有其他参与者发送消息。为了给所有参与者提供对发送到聊天室的消息的共同视图，底层数据集必须通过数据集同步协议( DSP )进行同步。在NDN环境下，DSP的重要性被放大，因为大多数应用程序都是基于分布式P2P架构开发的。这样做的目的是通过利用NDN提供的特性，如网内缓存和本地多播，实现高可扩展性。因此，对NDN中的DS领域进行了大量的研究，研究的目标之一是抽象出NDN应用开发人员考虑DS的必要性。

Traditionally, IP based solutions for DS take one of two approaches - centralized or decentralized. Centralized approaches require a centralized node which becomes the authoritative source on the state of the dataset. All nodes communicate directly with this node and updates to the dataset are sent through this node. This simplifies the problem considerably at the cost of creating a bottleneck in the system.

传统上，基于IP的DS解决方案采取两种方式之一- -集中式或分散式。集中式方法需要一个中心化的节点，该节点成为数据集状态的权威来源。所有节点直接与该节点通信，并通过该节点发送对数据集的更新。这在很大程度上简化了问题，代价是在系统中创建了一个瓶颈。

Alternatively, a decentralized approach can be taken in which all nodes communicate with one and other. In an IP based solution, **this requires each node to maintain n − 1 connections to every other node,** for example using a TCP socket. This approach mitigates the problem of having a bottleneck in the system, resulting in a more scalable solution, a**t the cost of requiring a considerably more complex protocol in order to maintain a consistent view of the dataset amongst all nodes.**

或者，可以采取一种去中心化的方法，所有节点相互通信。在基于IP的解决方案中，**这要求每个节点与其他节点保持n - 1个连接**，例如使用TCP套接字。这种方法缓解了系统中存在瓶颈的问题，从而产生了一个更可扩展的解决方案，**代价是需要一个相当复杂的协议来在所有节点之间保持一致的数据集视图。**

The scalability of the decentralized approach is limited by the connection oriented abstraction of IP, as the number of connections required scales quadratically with the number of nodes. The data oriented abstraction of NDN overcomes this issue as nodes are no longer concerned with who they communicate with and are instead concerned with producing and consuming named pieces of data, which can be fetched from and published to the network.

去中心化方法的可扩展性受限于IP的面向连接的抽象，因为所需的连接数与节点数呈二次方关系。NDN面向数据的抽象化克服了这一问题，因为节点不再关心与谁通信，而是关心产生和消费命名数据，这些数据可以从网络中获取并发布到网络中。

NDN can achieve distributed DS by synchronizing the namespace of the shared dataset among a group of distributed nodes [33]. Several protocols have been developed to achieve this including CCNx Sync 1.0 [34], iSync [35], ChronoSync [36], RoundSync [37] and PSync [38]. However, the most relevant protocols at the time of writing are ChronoSync, RoundSync and PSync.

NDN可以通过在一组分布式节点之间同步共享数据集的命名空间来实现分布式DS [ 33 ]。为了实现这一目标，人们提出了多种协议，包括CCNx Sync 1.0 [ 34 ]、iSync [ 35 ]、ChronoSync [ 36 ]、RoundSync [ 37 ]和PSync [ 38 ]。然而，在编写时最相关的协议是ChronoSync、RoundSync和PSync。

#### ChronoSync

The primary step in any DS protocol is a mechanism for determining that the dataset has been updated. ChronoSync datasets are organized such that each node’s data is maintained separately. Each node has a name prefix, representing the name the node’s data and a sequence number, representing the latest version of that data. The hash of the combination of a node’s name prefix and sequence number forms the node’s digest. Finally, the combination of all nodes’ digests forms the state digest which succinctly represents the state of the dataset at a snapshot in time. An example of a ChronoSync state digest tree in which Alice, Bob and Ted are interested in the synchronized dataset is shown in figure 2.7.

任何DS协议中的首要步骤都是确定数据集已更新的机制。对ChronoSync数据集进行组织，使得每个节点的数据被单独维护。每个节点有一个名称前缀，表示该节点数据的名称，有一个序列号，表示该数据的最新版本。节点名称前缀与序列号组合的哈希构成节点摘要。最后，所有节点摘要的组合形成状态摘要，简洁地表示数据集在时间快照上的状态。一个Alice、Bob和Ted对同步数据集感兴趣的ChronoSync状态摘要树的例子如图2.7所示。

When a node locally inserts a new piece of data into the dataset, the node recomputes the state digest, which will now be different to the previous state digest. At this point, the ChronoSync library will satisfy the outstanding SyncInterest using a SyncReply, which is a standard NDN Data packet. The Data packet used to satisfy the SyncInterest contains the name of the data which has been updated as the content. The name of the Data packet will simply be the name of the Interest it satisfies.

当节点在本地向数据集中插入新的数据时，该节点重新计算状态摘要，该状态摘要将不同于之前的状态摘要。此时，ChronoSync库将使用一个标准的NDN数据包SyncReply来满足杰出的SyncInterest。用于满足SyncInterest的数据包包含已更新为内容的数据的名称。数据包的名称将简单地表示为它所满足的兴趣的名称。

<img src="/home/bailey/图片/Screenshot_20230627_163511.png" alt="Screenshot_20230627_163511" style="zoom:55%;" />

For example, consider the case in which the current outstanding SyncInterest is /ndn/chatroom/sync/a73e6cb and Alice’s latest sequence number is 5. If Alice inserts a new piece of data into the dataset, **Alice will satisfy the SyncInterest with a SyncReply packet named /ndn/chatroom/sync/a73e6cb which contains /ndn/chatroom/alice/6 as the content**. **Alice will then recompute her state digest and express a new SyncInterest.**

例如，考虑当前突出的SyncInterest为/ ndn /聊天房间/ sync / a73e6cb且Alice的最新序列号为5的情况。如果Alice在数据集中插入新的数据，Alice将满足SyncInterest，其内容为包含/ ndn /聊天房间/ alice / 6的SyncReply包/ ndn /聊天房间/ sync / a73e6cb。爱丽丝将重新计算她的状态摘要，并表达一个新的SyncInterest。

All nodes will receive this SyncReply packet and have the option to express a standard NDN Interest to fetch Alice’s new data. The other nodes will also recompute the state digest using Alice’s new sequence number, and express a new SyncInterest, which will match the SyncInterest expressed by Alice, returning the system to the steady state.

所有节点都会收到这个SyncReply数据包，并且可以选择表达一个标准的NDN兴趣来获取Alice的新数据。其他节点也将使用Alice的新序列号重新计算状态摘要，并表示一个新的SyncInterest，它将与Alice表示的SyncInterest相匹配，使系统恢复到稳态。

The ChronoSync protocol exploits the Interest aggregation mechanism provided by NDN, meaning that when the dataset is synchronized, there will only be at most one outstanding SyncInterest on each link in the network.

ChronoSync协议利用了NDN提供的兴趣聚合机制，即当数据集同步时，网络中每条链路上最多只有一个突出的SyncInterest。

As a single Interest can only return a single Data packet in NDN, if two nodes produce two different SyncReplies for the same SyncInterest, only one SyncReply will reach a given node. To overcome this, ChronoSync re-expresses the same SyncInterest on receipt of a SyncReply. The second SyncInterest uses an exclude filter set to the hash of the content in the SyncReply. This means the same SyncReply will not be returned for the second SyncInterest and the second SyncReply can be obtained. This repeats until a subsequent SyncInterest incurs a timeout. ChronoSync also contains features for reconciliation in the event of network partitioning.

由于NDN中单个Interest只能返回单个数据包，如果两个节点对同一个SyncInterest产生两个不同的SyncReply，则只有一个SyncReply到达给定的节点。为了克服这个问题，ChronoSync在收到一个SyncReply时重新表达了相同的SyncInterest。第二个SyncInterest使用一个排除过滤器集合来对SyncReply中的内容进行哈希。这意味着对于第二个SyncInterest不会返回相同的SyncReply，可以得到第二个SyncReply。如此反复，直到后续的SyncInterest发生超时。ChronoSync还包含用于在网络分区时进行协调的功能。

#### RoundSync

RoundSync was developed to address the shortcomings of ChronoSync, namely the issues which arise when sync states diverge due to simultaneous data generation. As previously discussed, ChronoSync requires an expensive state reconciliation process when sync states diverge. The shortcoming of ChronoSync was determined to be the fact that ChronoSync uses a SyncInterest to serve two different purposes: (1) SyncInterests enable a node to retrieve updates as soon as they are produced by any other nodes, and (2) it lets each node detect whether its knowledge about the shared dataset conflicts with anyone else in the sync group [37].

RoundSync是为了解决ChronoSync的缺点，即同步状态因同时产生数据而发散时出现的问题而开发的。如前所述，当同步状态发生偏移时，ChronoSync需要一个昂贵的状态协调过程。ChronoSync的缺点被确定为ChronoSync使用SyncInterest服务于两个不同的目的：( 1 ) SyncInterests使节点能够在其他任何节点产生更新时立即检索更新，( 2 )让每个节点检测其关于共享数据集的知识是否与同步组中的任何人发生冲突[ 37 ]。

RoundSync uses a monotonically increasing round number and limits the number of times a node can produce an update to once per round. The key aspect here is that data synchronization is independent for each round. This means nodes can continue to publish and receive further updates, while trying to reconcile issues which occurred in previous rounds. RoundSync accomplishes this by splitting up ChronoSync’s SyncInterest into a Data Interest which is used for fetching updates generated by a node, and RoundSync’s own Sync Interest which is used solely for detecting inconsistent states within a round [37].

RoundSync使用单调递增的轮数，并将节点产生更新的次数限制为每轮一次。这里的关键之处在于数据同步对于每一轮都是独立的。这意味着节点可以继续发布和接收进一步的更新，同时试图调和前几轮中出现的问题。RoundSync通过将ChronoSync的SyncInterest拆分为Data Interest (数据兴趣)和RoundSync自己的Sync Interest (同步兴趣)来实现，Data Interest用于获取节点产生的更新，RoundSync自己的Sync Interest仅用于检测一轮中的不一致状态[ 37 ]。

Although RoundSync appears to offer several benefits over ChronoSync, the only available implementation of the protocol is for use with ndnSIM only [39].

尽管RoundSync似乎比ChronoSync提供了一些好处，但该协议唯一可用的实现是仅与ndnSIM一起使用[ 39 ]。

#### PSync

PSync was developed as a protocol to allow consumers to subscribe to a subset of a large dataset being published by a producer. Data generated by producers is organized into data streams, which are sets of data which have the same name but different sequence numbers. Consumers can subscribe to certain data streams of a producer by maintaining a subscription list.

PSync被开发为一种协议，允许消费者订阅由生产商发布的大型数据集的子集。生产者产生的数据被组织成数据流，数据流是具有相同名称但序列号不同的数据集合。消费者可以通过维护订阅列表来订阅某个生产商的某些数据流。

Two accomplish this efficiently, PSync uses two key data structures - a Bloom Filter (BF) and an Invertible Bloom Filter (IBF). BFs are memory efficient probabilistic data structures which can rapidly determine if an element is not present in a set. However, BFs cannot say for certain that an element is present in a set. BFs use several hash functions to hash the element of interest, resulting in a list of indices into a bit array (one for each hash function).

PSync使用了两个关键的数据结构- - Bloom Filter ( BF )和可逆布鲁姆过滤器( IBF )。BFs是一种内存有效的概率数据结构，它可以快速确定一个元素是否不存在于一个集合中。但是，BFs不能确定一个元素是否存在于一个集合中。BFs使用多个哈希函数对感兴趣的元素进行哈希运算，将索引列表转化为一个位数组(每个哈希函数一个)。

To insert into a BF, the bits at the corresponding indices provided by hashing the element with each of the hash functions are all set to 1. To determine if an element is not in the set, the incoming element is hashed using each of the hash functions, again producing a list of indices. If any of the bits in the array at the list of indices are 0, the element is definitely not in the set, otherwise the element may be in the set. IBFs are an extension to standard BFs which allow elements to be inserted and deleted from the IBF. Elements can also be retrieved from the IBF, but the retrieval may fail, depending on the state of the IBF. The operation of an IBF is outlined in appendix A. IBFs also support a set difference operation, allowing for the determination of elements in one set but not in another.

为了插入到BF中，使用每个哈希函数对元素进行哈希所提供的相应索引处的比特都设置为1。为了确定一个元素是否不在集合中，使用每个哈希函数对传入的元素进行哈希，再次生成索引列表。如果索引列表中数组中的任意一位为0，则该元素肯定不在集合中，否则该元素可能在集合中。IBFs是对标准BFs的扩展，允许从IBF中插入和删除元素。元素也可以从IBF中检索，但检索可能会失败，这取决于IBF的状态。IBF的操作在附录A中进行了概述，IBF还支持集合差分操作，允许在一个集合中确定元素，但不允许在另一个集合中确定元素。

PSync uses BFs to store the subscription list of subscribers and IBFs to maintain producers’ latest datasets, known as the producer state. The producer state represents the latest dataset of a producer and contains a single data name for each of the producer’s data streams. These data names contain the data stream’s name prefix and the latest sequence number of that data.

PSync使用BF存储订阅者订阅列表和IBF来维护生产者的最新数据集，称为生产者状态。生产者状态表示生产者的最新数据集，并为每个生产者的数据流包含单一的数据名称。这些数据名称包含数据流的名称前缀和该数据的最新序列号。

Producers in PSync maintain no state regarding their consumers and instead store a single IBF for all consumers, providing scalability under a large number of consumers [38]. Consumers express long standing SyncInterests which contain an encoded copy of the BF representing their subscription list and an encoded copy of an IBF representing the last producer state they received. The producer can determine if any new data names have been produced by subtracting the current producer state from the producer state contained in the SyncInterest, using set difference operator for IBFs. The producer can then determine whether or not the consumer is actually subscribed to any of these data names using the provided subscriber list.Finally, the producer will either send back the new data names through a SyncReply, or if there is no new data, store the Interest until new data is generated. Consumers receiving the SyncReply can then fetch the new data using standard NDN Interests and update their latest producer state accordingly.

PSync中的生产者不会维护关于其消费者的状态，而是为所有消费者存储一个单独的IBF，以便在大量消费者的情况下实现可扩展性[38]。消费者表达长期的SyncInterests，其中包含代表其订阅列表的BF的编码副本和代表它们收到的最后一个生产者状态的IBF的编码副本。生产者可以通过从SyncInterest中包含的生产者状态中减去当前生产者状态来确定是否产生了任何新的数据名称，使用IBF的集合差异运算符。生产者然后可以使用提供的订阅者列表确定消费者是否实际上订阅了这些数据名称中的任何一个。最后，生产者要么通过SyncReply将新的数据名称发送回来，要么如果没有新数据，将兴趣存储起来，直到生成新数据。接收到SyncReply的消费者可以使用标准的NDN Interests获取新的数据，并相应地更新其最新的生产者状态。

### Motivation

As the data generated in MOGs is so varied, one of the main goals of this research was to determine how well NDN can support all of these different data types, some of which fit nicely into a content-oriented abstraction, and some of which are entirely host oriented. The other major challenge associated with MOGs is that they require extremely highperformance networking solutions that provide high bandwidth and low latency.

由于在MOGs中生成的数据是如此的多样化，本研究的主要目标之一是确定NDN能多好地支持所有这些不同的数据类型，其中一些类型非常适合于面向内容的抽象，而其中一些类型则完全面向主机。与MOGs相关的另一个主要挑战是它们需要提供高带宽和低延迟的高性能网络解决方案。

### Work

The research led to the development of a novel protocol for synchronizing remote game objects across all players in the game using NDN. This protocol was designed to exploit the three main benefits of NDN - interest aggregation, native multicast and in-network caching.

这项研究导致开发了一个新的协议，用于在游戏中使用NDN在所有玩家之间同步远程游戏对象。该协议旨在利用NDN的三个主要优势- -兴趣聚合、原生多播和网内缓存。

**Frontend:** The results of this research led to the design and implementation of a 2D MOG in order to facilitate the testing and evaluation of an NDN based backend for the game. The game allows players to move around a virtual game world, shoot projectiles at other players and to place blocks which can be used as cover. These game mechanics were chosen as they provide a good coverage of the categories of data identified by the taxonomy.

**Backend:** A P2P NDN based backend was developed for the game which uses existing NDN software, as well as a novel protocol for synchronizing remote game objects over NDN, to provide a high-performance networking solution required by a fast paced MOG.    

**System: **A system for automating game players was created to facilitate the testing of the game. This was done by creating a Docker container for an automated player and deploying these containers onto an Amazon Web Services cluster using a Docker swarm. Finally, the performance of the game was evaluated under a variety of network topologies and scenarios by analysing the data produced during the testing period. 
**Result:** The results showed that the features of NDN such as interest aggregation and native multicast enabled the game easily support 16 concurrent players. Several networking optimizations were also developed to further reduce the network traffic generated by the game, further increasing scalability.

创建了一个用于自动化游戏玩家的系统，以方便游戏的测试。这是通过为自动播放器创建一个Docker容器，并使用Docker Swarm将这些容器部署到Amazon Web Services集群来完成的。最后，通过对测试阶段产生的数据进行分析，在多种网络拓扑和场景下评估了游戏的性能。结果表明，NDN的兴趣聚合、原生多播等特性使得游戏可以轻松支持16个并发玩家。为了进一步减少游戏产生的网络流量，还开发了一些网络优化，进一步增加了可扩展性。

### Design

### Implementation

### Conclusion