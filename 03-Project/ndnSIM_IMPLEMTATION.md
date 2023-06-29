> ### 实现大文件传输
>
> 1.使用数据生成器，如何生成大数据呢？还是直接加载本地文件
>
> producer接收到兴趣包处后，发送数据处理。
>
> 2.producer如何接收兴趣包 Simulator:: Schedule
>
> 由于ndnSIM中的名称匹配是基于最长前缀匹配的原则，可以让消费者在请求数据时只提供一部分名称，而不必提供完整的名称。这个部分名称与数据名称的最长前缀匹配，就可以获取到相对全面的数据。以视频为例，只需要提供“/video/mp4/1080p/1001”这部分前缀，就可以获取ID为1001的所有分段视频,也就是说,通过一次请求,就可以获取完整的视频内容,而不必逐个请求视频的每一段内容，从而减少了请求的数据量，降低网络流量。

> ### 重传机制两种解决方法：
>
> 1.当兴趣包没有得到满足时，可以在一定时间后进行重传。在ndnSIM中，可以设置兴趣包的超时时间，如果在超时时间内没有收到数据包，则可以选择重新发送相同的兴趣包。（机制内已实现）
>
> 2.NACK（Negative Acknowledgement）机制：NDN中的消费者可以向生产者发送 ，表示对某个数据包的请求失败或数据包损坏。生产者可以根据收到的NACK信息，选择重传丢失或损坏的数据包。（机制内已实现）

> ### 模拟丢包
>
> 1.wire设置丢包率方法是否可行
>
> 2.若不可行：link-failure
>
> 而有些场景需要在某些时候在NDN节点之间断开某些链接。NS-3不提供实际“断开”节点之间链接的能力。但是，它提供了设置损失模型的功能，以模拟信道中的数据包丢失。通过在点对点链路两侧使用正确建立的损耗模型，可以模拟链路断开。为了简化这些操作，ndnSIM包含[**ndn::LinkControlHelper**](http://ndnsim.net/2.3/doxygen/classns3_1_1ndn_1_1LinkControlHelper.html)，它允许调度链路故障和故障恢复：
>
> ```c++
> #inlude "ns3/ndnSIM/helper/ndn-link-control-helper.hpp"
> 
> Simulator::Schedule(Seconds(10.0),ndn::LinkControlHelper::FailLink, node1, node2);
> Simulator::Schedule(Seconds(15.0), ndn::LinkControlHelper::Uplink, node1, node2);
> ```
>
> 

不需要新增任何字段

> 搭环境，scenario?
>
> 不，直接在example上开发，scenario后期再做。



> 1.拓扑图 4-1,做多播
>
> 2.链路丢包策略
>
> 3.300MB视频下载
>
> 4.实验设计