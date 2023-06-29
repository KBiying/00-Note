# 下一代互联网

## 要求

> **`NDN`模拟**
>
> - 模拟`NDN`网络环境, 实现无线网络中的, 无连接的,面向数据内容的传输
>
> **使用`ndnSIM`进行模拟**
>
> 	- 基于`NS3`进行构建
>
> **目标:**
>
> 	- 实现大文件(*加密)稳定传输
>	- 模拟丢包
> 	- 设计重传机制

## nsnSIM目录结构

（1）`ns-3`，仿真之后产生的`txt`仿真结果数据文件会在这个目录下。在命令行中通过`./waf`来编译运行仿真的`waf`编译工具也在这个文件夹里。

（2）`ns-3/src/ndnSIM`，这个是真正的`ndnSIM`源代码目录。

（3）`ns-3/src/ndnSIM/apps`，这里面放着多种内容请求者`（consumer)`和内容提供者`（producer)`的头文件（`.hpp`，用于声明和其他文件的继承关系和一些虚函数等）和真正的代码文件（`.cpp`），它们彼此的继承关系是：`ndn-consumer-cbr`等特定属性和自己定制的内容请求者继承`ndn-consumer`，自己编写的特定定制的其他`producer`应该继承`ndn-producer`，而`ndn-consumer`与`ndn-producer`都继承了`ndn-app`（在各角色应用的`.hpp`文件开头会提到），如下所示：

（4）`ns-3/src/ndnSIM/examples`，这里面存放着很多`ndnSIM`官网的`simulation example`源代码，即仿真场景`（scenario）`文件所在的地方。如果自己编写了定制的仿真场景文件.

（5）`ns-3/src/ndnSIM/NFD/daemon`，这个是`ndnSIM`中集成了`NFD`模块的地方，主要负责`NDN`兴趣包和数据包的转发等功能。其中`ns-3/src/ndnSIM/NFD/daemon/fw`文件夹里面存放着各种转发策略（forwarding strategy)



## Frame

要使用`ndnSIM`仿真器来模拟`NDN（Named Data Networking）`网络环境，实现无线网络中的无连接、面向数据内容的传输，可以按照以下步骤进行操作：

1. 安装`ndnSIM`：首先，需要在计算机上安装`ndnSIM`仿真器。`ndnSIM`是基于NS-3网络仿真器的一个扩展，因此需要先安装NS-3。你可以参考`ndnSIM`的官方文档（https://ndnsim.net/current/）中的安装指南，按照步骤进行安装。
2. 创建仿真场景：一旦`ndnSIM`安装完毕，你可以**创建一个仿真场景**来模拟无线网络中的`NDN`环境。在仿真场景中，你可以定义节点、拓扑结构、应用程序等。
3. 定义节点和拓扑结构：使用ndnSIM提供的API，你可以在仿真场景中定义NDN节点和网络拓扑结构。你可以创建无线节点，并在节点之间建立连接。你还可以指定节点的位置、移动模式和属性，以模拟真实的无线网络环境。
4. 配置应用程序：使用ndnSIM的应用程序模型，你可以配置数据生成器、消费者和路由器等。这些应用程序将模拟NDN网络中的数据传输。你可以定义数据生成器来生成数据，并将其发布到网络中，然后使用消费者节点来请求数据。路由器节点将负责转发数据请求和传输数据。
5. 运行仿真：一旦场景配置完成，你可以运行仿真并观察仿真结果。ndnSIM会模拟节点之间的数据传输、数据缓存和路由选择等过程。你可以根据需要进行多次仿真，并分析结果以评估网络性能。

需要注意的是，ndnSIM是一个强大的工具，使用它进行仿真需要一定的网络和编程知识。你可以参考ndnSIM的官方文档和示例代码，以获得更详细的指导和帮助。



要使用ndnSIM实现大文件（加密）的稳定传输，可以按照以下步骤进行操作：

1. 定义文件加密机制：首先，你需要确定所需的文件加密机制。选择适合你的需求的加密算法，并确保你具有相应的加密和解密功能。
2. /home/bailey/ndnSIM/ns-3/src/ndnSIM/examples/loss.cpp创建NDN节点：使用ndnSIM提供的API，在仿真场景中创建NDN节点。你可以使用`ndn::Node`类来创建节点，并在节点之间建立连接以构建网络拓扑。
3. 实现数据生成器：创建一个数据生成器，用于生成大文件的内容。你可以使用C++编程语言编写自定义的数据生成器，根据需要生成加密或非加密的文件内容。
4. 实现数据消费者：创建一个数据消费者节点，用于接收和处理来自数据生成器的数据。你可以定义一个自定义的消费者应用程序，用于接收和解密传输的数据，并将其写入文件。
5. 实现数据传输：使用ndnSIM提供的API，将数据从数据生成器传输到数据消费者。你可以通过编写自定义的应用程序逻辑，将加密或非加密的数据封装成NDN数据包，并使用NDN路由器节点进行传输。
6. 运行仿真：一旦你的NDN节点、数据生成器和数据消费者都准备好了，你可以运行ndnSIM仿真并观察仿真结果。确保文件能够稳定传输并正确解密。

需要注意的是，以上步骤只是一个基本的指导，你可能需要根据具体的需求进行进一步的定制和调整。此外，对于大文件传输，还可能需要考虑数据分片、流量控制和传输可靠性等问题，以确保稳定的传输。

请参考ndnSIM的官方文档和示例代码，以获取更详细的指导和帮助。同时，对于加密相关的实现，建议参考相关的密码学和安全性资料，并确保实现的安全性和保密性。

在ndnSIM中，没有直接内置的传统重传机制，因为NDN（Named Data Networking）采用了一种基于内容的路由和缓存策略，与传统的TCP/IP网络中的端到端重传机制不同。但是，ndnSIM提供了一些机制和策略，用于处理数据传输中的丢包和恢复。

以下是一些在ndnSIM中处理丢包的机制和策略：

1. 内容感知路由：NDN路由器根据内容名称来路由数据包，而不是根据IP地址。当一个数据包丢失时，请求方可以通过发送新的请求来获取丢失的数据包，路由器会根据名称匹配，将数据包从缓存中转发给请求方。
2. 兴趣包重传：当兴趣包没有得到满足时，可以在一定时间后进行重传。在ndnSIM中，可以设置兴趣包的超时时间，如果在超时时间内没有收到数据包，则可以选择重新发送相同的兴趣包。
3. NACK（Negative Acknowledgement）机制：NDN中的消费者可以向生产者发送 ，表示对某个数据包的请求失败或数据包损坏。生产者可以根据收到的NACK信息，选择重传丢失或损坏的数据包。
4. FEC（Forward Error Correction）机制：在一些特殊情况下，NDN可以使用前向纠错编码（FEC）来增强数据的可靠性。FEC会将冗余数据添加到数据包中，以允许接收方通过冗余数据纠正部分丢失的数据。

需要注意的是，ndnSIM是一个灵活的仿真平台，你可以根据需要自定义和实现特定的重传机制。你可以通过编写自定义的应用程序逻辑，利用ndnSIM提供的API和事件处理机制，实现自己的重传策略。

此外，ndnSIM还提供了各种示例和文档，可以帮助你更好地理解和使用ndnSIM的机制和策略。你可以参考ndnSIM的官方文档和示例代码，以获取更详细的信息和指导。

在ndnSIM中，可以创建各种不同类型的仿真场景来模拟NDN（Named Data Networking）网络。下面是几个常见的NDN仿真场景示例：

1. 简单的点对点通信：创建两个节点，一个作为数据生产者，另一个作为数据消费者。生产者节点生成数据，消费者节点发送兴趣包请求数据，并接收和处理数据包的响应。
2. 多节点拓扑：创建多个节点并在它们之间建立连接，以模拟更复杂的网络拓扑结构。可以使用ns-3网络仿真器的拓扑生成器来创建各种类型的拓扑结构，如星型、树状、网格等。
3. 缓存策略评估：在仿真场景中，可以使用不同的缓存策略（如最近最少使用、最久未使用等）来评估缓存对网络性能和数据传输效率的影响。可以根据节点的缓存大小和替换策略，观察缓存命中率、延迟和带宽利用率等指标。
4. 网络拥塞控制：通过在节点之间引入网络拥塞，可以评估NDN中的拥塞控制机制的效果。可以模拟网络中的链路拥塞或节点负载过高，观察NDN的拥塞控制算法如何调整数据传输速率和窗口大小。
5. 移动场景：考虑节点的移动性，创建一个移动场景来模拟无线网络环境。可以定义节点的移动模型、速度和方向，以评估NDN在移动网络中的性能和适应性。

这些只是一些示例场景，你可以根据具体的研究需求和实验目标来设计自己的NDN仿真场景。ndnSIM提供了一系列的API和示例代码，以帮助你创建和定制仿真场景。你可以参考ndnSIM的官方文档和示例代码，了解更多关于仿真场景创建和配置的详细信息。

https://blog.csdn.net/qq1187239259/article/details/115296651

https://blog.csdn.net/Ximerr/article/details/115434760



Question:

1.producer怎么传输自定义文件,怎么确认consumer收到了文件

2.路由器设置功能有哪些? 

​	`Default`

​	`ndnGlobalRoutingHelper`是全局路由器管理

3.传输策略有哪些? 

​	`StrategyChoiceHelper::InstallAll`

​	两个`BestRoute`和`Multicast`

### AppHelper

> #### ConsumerCbr
>
> 这是一个生成兴趣流量的应用程序，具有预定义的模式（常量频率、以随机均匀分布、指数分布等方式生成的兴趣包之间的间隔）。

- `Frequency` 

  兴趣包的生成频率

  - default:1.0

- `Randomize`(不清楚作用) 

  Specify whether to do randomization for inter-Interest gap or not. The following variants are currently supported

  - none
  - uniform
  - exponential

> ConsumerZipfMandelbrot
>
> 这是一个应用程序，根据Zipf-Mandelbrot分布（内容频率分布的数量）请求内容（请求中的名称）。这个类是ConsumerCbr的子类。

- `NumberOfContents`

  应用程序将请求的不同内容（序列号）的数量

  - default:100

> ConsumerBatches
>
> 这是一个开关式应用程序，在仿真的指定时间点生成指定数量的兴趣包。

- Batches

  - default:Empty

    - ```
      // Set attribute using the app helper
      consumerHelper.SetAttribute("Batches", StringValue("1s 1 2s 5 10s 2"));
      ```

> ConsumerWindow
>
> 这是一个生成可变速率兴趣流量的应用程序。它实现了一个基于滑动窗口的简单兴趣生成机制。

- Window

  - default:1
    - 最初将发送的兴趣包数量，在等待数据之前（即未响应的兴趣包数量）。

- PayloadSize

  - default:1040
    - 数据有效负载的预期大小(仅指定了size的时候才需要)

- Size

  - default:-1

    - 要请求的数据量（在接收到指定数据量后停止发出兴趣包）。

      如果Size设置为-1，则会一直请求兴趣包，直到仿真结束。

> ConsumerPcon
>
> 是一个生成可变速率兴趣流量的应用程序.它实现了基于窗口的速率控制,使用K.Schneifer等人开发的PCON拥塞控制机制.是从ConsumerWindow派生而来的。

- The application has the same attributes as [:ndnsim:`ConsumerWindow`](https://ndnsim.net/current/applications.html#id17), in addition to the following:

- CcAlgorithm

  使用哪种窗口自适应算法（AIMD、BIC 或 CUBIC）

  - default:AIMD

- Beta

  TCP 乘法减少因子                          

  - default:0.5

- CubicBeta

  - default:0.8

- AddRttSupress

  窗口减少之间的最小 RTT 数（1 + 此因子）

  - default:0.5

- ReactToCongestionMarks

  如果为真，则进程收到拥塞标记；否则，忽略它们

  - default: `true`

- UseCwa

  如果为真，则使用保守窗口自适应

  - default: `true`

- UseCubicFastConvergence

  如果为真，则使用 TCP CUBIC 快速收敛           

  - default: `true`

> Producer
>
> 这是一个简单的兴趣汇应用程序，它会以指定的大小和与兴趣包相同的名称，回复每个收到的兴趣包。

> ​    Consumer函数调用顺序
>
> Consumer()
>
> StartApplication()
>
> SendPacket()
>
> ```c
> +0.000000000s 0 ndn.Consumer:Consumer()
> +0.000000000s 0 ndn.Consumer:Consumer()
> +0.000000000s 0 ndn.Consumer:StartApplication()
> +0.000000000s 0 ndn.Consumer:SendPacket()
> +0.000000000s 0 ndn.Consumer:SendPacket(): [INFO ] > Interest for 0
> +0.000000000s 0 ndn.Consumer:WillSendOutInterest(): [DEBUG] Trying to add 0 with +0ns. already 0 items
> +0.000000000s 0 ndn.Consumer:StartApplication()
> +0.000000000s 0 ndn.Consumer:OnNack(): [INFO ] NACK received for: /dst1/seq=0, reason: NoRoute
> +0.000000000s 0 ndn.Consumer:SendPacket()
> +0.000000000s 0 ndn.Consumer:SendPacket(): [INFO ] > Interest for 0
> +0.000000000s 0 ndn.Consumer:WillSendOutInterest(): [DEBUG] Trying to add 0 with +0ns. already 0 items
> +0.000000000s 0 ndn.Consumer:OnNack(): [INFO ] NACK received for: /dst3/seq=0, reason: NoRoute //接收了NACK就打印log，但不会重新发送interest包，因此这里加上重发interest包
> +0.097648000s 0 ndn.Consumer:OnData(0x563362464800, 0x5633624df6c0)
> +0.097648000s 0 ndn.Consumer:OnData(): [INFO ] < DATA for 1
> +0.097648000s 0 ndn.Consumer:OnData(): [DEBUG] Hop count: 3
> 
> 
> ```
>
> 

> hijacker里面没有说producer怎么接受兴趣包，
>
> consumer发送了兴趣包，producer接收到兴趣包，然后发送data包
>
> example 里都没有说producer接收兴趣包，只是设置了拓扑结构，还有prefix就可以进行通信了。
>
> ```
> +0.000000000s 4 ndn.Producer:Producer()
> +0.000000000s 4 ndn.Producer:Producer()
> +0.000000000s 4 ndn.Producer:StartApplication()
> +0.000000000s 4 ndn.Producer:StartApplication()
> +0.040307200s 4 ndn.Producer:OnInterest(0x5633622a37d0, 0x5633623827e0)
> +0.040307200s 4 ndn.Producer:OnInterest(): [INFO ] node(4) responding with Data: /dst1/seq=1
> +0.040819200s 4 ndn.Producer:OnInterest(0x56336246dac0, 0x5633624b7900)
> +0.040819200s 4 ndn.Producer:OnInterest(): [INFO ] node(4) responding with Data: /dst3/seq=1
> +0.050307200s 4 ndn.Producer:OnInterest(0x5633622a37d0, 0x5633624c66f0)
> +0.050307200s 4 ndn.Producer:OnInterest(): [INFO ] node(4) responding with Data: /dst1/seq=2
> +0.060819200s 4 ndn.Producer:OnInterest(0x56336246dac0, 0x5633624b4b20)
> +0.060819200s 4 ndn.Producer:OnInterest(): [INFO ] node(4) responding with Data: /dst3/seq=3
> ```
>
> 最长前缀匹配

> 代码框架
>
> 1.构建拓扑结构，创建节点和链接或直接读取拓扑文件
>
> 2.在所有节点安装NDN栈
>
> 3.配置转发策略
>
> 4.为各个节点安装消费者应用或生产者应用，用于生产或请求NDN网络中的流量数据
>
> 5.手动或用GlobalRoutingHelper生成FIB表
>
> 6.设置仿真时间
>
> 7.使用trace helper 等工具获取仿真结果
>
> 8.开始仿真，仿真结束后销毁
>
> 

> ## Tracer
>
> ndnSIM支持可以进行多种记录仿真结果的多个类，例如：
>
> （1）`Packet-level trace helpers` 用于跟踪节点字节速率和该节点转发的兴趣包/数据包的每秒数量
>
> ```c++
> ndn::L3RateTracer::InsterAll("rate-trace.txt", Second(1.0));
> Simulator::Run();
> ```
>
> - Time 
> - Node
> - FaceId
> - Type
> - Packets
> - Kilobytes
> - PacketsRaw
> - KilobytesRaw
>
> (2) `L2Tracer`该跟踪器类似于`ndn::L3RateTracer`,但它仅踪第二层上的数据包丢包(例如,由于传输队列溢出)
>
> ```c++
> L2RateTracer::InstallAll("drop-trace.txt",Second(0.5));
> Simulator::Run();
> ```
>
> - Time
> - Node
> - Interface
> - Type
> - Packets
> - Kilobytes
> - PacketsRaw
> - KilobytesRaw
>
> (3)`Content styoretrace helper` 其中`ndn::CsTracer`可以统计缓存命中率进而缓存未命中率
>
> ```c++
> CsTracer::InstallAll("cs-trace.txt", Second(1));
> Simlator::Run();
> ```
>
> - Time :仿真产生该条数据的时刻点
> - Node: 节点id, 在仿真中唯一的
> - Type: 在该时刻记录的计数器的类别，分为两种：（1）CacheHits，即被本地缓存所满足的兴趣包的数量。（2）CacheMisses，即没有被本地缓存所满足的兴趣包的数量。
> - Packets: 在该时间间隔记录到的包数量，这个数量指的是两种类别中的哪个基于第三列。
>
> (4) `Application-leveltrace helper`:获得有关发出Interest到接收相应数据包之间延迟的数据.
>
> ```c++
> AppDelayTracer::InstallAll("app-delays-trace.txt");
> Simulator::Run()
> ```
>
> - Time
> - Node
> - AppId
> - SeqNo: seq number of the Interest-Data
> - Type
>   - full delay:表示第一个Interest包发送到收到Data包的时间
>   - last delay: 表示上一个Interest包发送到收到Data包的时间
> - DelayS
> - DelayUS
> - RetxCount:兴趣包重传的个数
> - HopCount

