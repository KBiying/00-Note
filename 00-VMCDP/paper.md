### DESIGN

#### A. Design Goals

​	we has three goals: large file transmission, retransmission mechanism and package loss simulation. 

​	Large File Transmission. 一个兴趣包最多只能请MTU大小的数据，因此请求大文件的时候需要进行多次请求。

​	Large File Transmission: The goal is to support the transmission of large files. Since an Interest packet can request a maximum of MTU size data, multiple requests are required to retrieve a large file. To achieve this, we need to implement a mechanism to handle sequential requests for different segments of the file and assemble them at the consumer end.

​	Retransmmision Mechanism.为了保证稳定传输，获取完整的数据

​	Package Loss. 在模拟器上模拟丢包

#### B. Large File Transmission.

画一个流程图，讲一下流程

在ndn网络中，两者之间的沟通是通过发送兴趣包和回复数据包通信的，每个被分片的文件也要标记不同的seq值。拆分文件的时候，得先把文件放到内存中，以方便后续进行。

因此在消费者端，需要先获取文件的大小，与生产者商量好MTU值的大小，并将文件分片，分成N个MTU size 。然后每段请求数据。从小到大发送视频片段请求，如果期间有兴趣包没有被满足，则认为是包丢了，则使用重传机制，重新传输丢掉的对应seq值的请求。如果接收到相应的seq值数据包，则将其暂存在数组array[seq]当中。知道收到视频最大的seq值，将内存中接收到的视频片段合并起来，输出到本地文件当中。

在生产者端，需要提前将文件拆分成MTU大小的分片，每个分片的seq值从大到小按顺序赋值。在app运行时间内，等待数据请求，即等待兴趣包。接收到兴趣包之后，分析seq是否在合法的范围值之内，如果是则返回请求，即返回对应seq值的数据包，否则就丢弃该兴趣包。

#### C.Retransmmision Mechanism.

#### D.Package Loss.

###  IMPLEMENTATION

A. Custom class

自定义consumer类， 自定义producer类

B.

### EVALUATION

A. 有线和无线的对比

丢包率 

B. bestroute和mutilcast路由的对比

丢包率

### DESIGN

#### A. Design Goals

In this design, we have identified three goals: large file transmission, retransmission mechanism, and package loss simulation.

1. Large File Transmission: The goal is to support the transmission of large files. Since an Interest packet can request a maximum of 1500 bytes of data, multiple requests are required to retrieve a large file. To achieve this, we need to implement a mechanism to handle sequential requests for different segments of the file and assemble them at the consumer end.

2. Retransmission Mechanism: To ensure reliable and stable transmission, we need to implement a retransmission mechanism. This mechanism will detect and handle packet loss or transmission errors. When a packet loss or error occurs, the missing or erroneous packet will be retransmitted to ensure that the complete data is received at the consumer end.

3. Package Loss: It is important to simulate packet loss in the simulator environment. This can be achieved by introducing a packet loss model or mechanism that randomly drops packets during the simulation. The packet loss simulation should be configurable, allowing us to control the packet loss rate and pattern according to our specific requirements.

#### B. Proposed Solution

To address the design goals mentioned above, we propose the following solution:

1. Large File Transmission:
   - Implement a segmentation mechanism at the producer side to divide the large file into smaller segments.
   - The consumer sends sequential Interest packets for each segment of the file, requesting the data.
   - The producer responds with Data packets containing the requested segments of the file.
   - The consumer assembles the received segments to reconstruct the complete file.

2. Retransmission Mechanism:
   - Implement a reliable transport protocol, such as TCP, to ensure reliable delivery of packets.
   - Track acknowledgments for received packets at the consumer side.
   - In case of missing or erroneous packets, the consumer sends a retransmission request for those packets.
   - The producer retransmits the requested packets, ensuring their reliable delivery.

3. Package Loss:
   - Implement a packet loss model that randomly drops packets during the simulation.
   - Introduce configurable parameters to control the packet loss rate and pattern.
   - Track the dropped packets at the consumer side and trigger retransmission requests for the missing packets.

By implementing the proposed solution, we can achieve large file transmission by handling sequential requests, ensure reliable transmission through a retransmission mechanism, and simulate package loss in the simulator environment. These design elements work together to provide a comprehensive solution for the specified goals.





> ## Consumer Process App Design ##
>
> In an NDN network, communication between the consumer and producer is achieved through the exchange of Interest and Data packets. For the transmission of segmented files, each segment is assigned a unique sequence value. To facilitate further processing, the file is first loaded into memory at the consumer's end. Therefore, in the consumer process, the following steps need to be performed:
>
> 1. Determine File Size and Negotiate MTU:
>    - The consumer initiates a request to the producer to obtain the size of the file.
>    - Based on the file size, the consumer negotiates with the producer to determine the Maximum Transmission Unit (MTU) size, which represents the maximum payload size that can be transmitted in a single Data packet.
>
> 2. File Segmentation:
>    - The consumer divides the file into multiple segments, each with a size equal to the negotiated MTU size.
>    - Each segment is assigned a unique sequence value to enable proper ordering and identification during the reassembly process.
>
> 3. Sending Interest Packets:
>    - The consumer starts sending Interest packets for each segment in ascending order of their sequence values.
>    - If any Interest packet is not satisfied within a specified timeout period, it is assumed that the packet was lost, and the consumer triggers a retransmission request for the corresponding sequence value.
>
> 4. Retransmission Mechanism:
>    - When a retransmission request is received by the producer, it retransmits the Data packet for the requested sequence value.
>    - The consumer keeps track of the received Data packets in an array, where each position corresponds to a sequence value.
>    - If multiple retransmission requests are triggered for different sequence values, the producer handles them independently and sends the requested packets.
>
> 5. Data Reassembly:
>    - The consumer continues receiving Data packets for different sequence values until it receives the packet with the highest sequence value, indicating the end of the file transmission.
>    - The received segments are temporarily stored in memory, using the array mentioned earlier.
>    - Once all the segments are received, the consumer combines them in the correct order to reconstruct the complete file.
>    - Finally, the reconstructed file is saved to the local file system.
>
> By following this consumer process app design, the consumer can successfully request and receive segmented video files from the producer. The retransmission mechanism ensures the reliability of the transmission, and the data reassembly process allows the consumer to reconstruct the complete video file from the received segments.

> The exchange of Interest and Data packets allows for consumer and producer interactions in an NDN network. Each segment of a segmented file is given a distinct sequence value for transmission. The file is initially loaded into memory at the consumer's end to aid in subsequent processing. Therefore, the following actions must be taken during the consumer process:
>
> 1. Determine File Size and Negotiate MTU: The customer contacts the producer to request information about the file size.
>    - The consumer and producer negotiate the Maximum Transmission Unit (MTU) size, which denotes the largest payload that may be transferred in a single Data packet, based on the file size.
>
> 2. File Segmentation: 
>
>    - The consumer separates the file into a number of segments, each of which is the same size as the agreed-on MTU size. In order to facilitate appropriate ordering and identification throughout the reassembly process, each segment is given a distinct sequence value.
>
> 3. Sending Interest Packets: The consumer initiates the sending of Interest packets for each segment in ascending order of their sequence values. Consumers initiate retransmission requests for the associated sequence value if any Interest packets that are not met within a given delay period are believed to have been lost packets.
>
> 4. Re-transmission Mechanism: - The producer re-transmits the Data packet for the required sequence value when it receives a re-transmission request.
>    - The producer retransmits the Data packet for the required sequence value in response to a retransmission request.
>    - The consumer maintains a list of all the data packets they have received in an array, where each position represents a sequence value.
>    - The producer handles each request separately and transmits the desired packets if several retransmission requests come for various sequence values.
>
> 5. Data Reassembly: - Until the consumer receives the packet with the highest sequence value, signaling the conclusion of the file transmission, it continues to receive Data packets with lower sequence values.
>    - The array stated before is used to temporarily store the received segments in memory.
>    - Once each segment has been received, it is combined in the proper order by the consumer to reassemble the entire file.
>    - The constructed file is then saved to the local directory.
>
>    The customer can successfully request and get segmented video files from the producer by using this consumer process app design. The data reassembly procedure enables the consumer to reassemble the entire video file from the received segments, and the retransmission mechanism ensures that reliability of the transmission.

> ## Producer Process App Design ##
>
> In the Producer process of the NDN network, the following steps are involved:
>
> 1. File Segmentation:
>    - Before running the app, the Producer pre-processes the file by dividing it into segments of the MTU size.
>    - Each segment is assigned a unique sequence value, with the sequence values assigned in descending order.
>
> 2. Handling Data Requests:
>    - The Producer app waits for Interest packets from the Consumer, indicating the Consumer's request for specific segments of the file.
>    - Upon receiving an Interest packet, the Producer app analyzes the sequence value requested in the Interest packet.
>    - If the sequence value is within the valid range of sequence values for the segmented file, the Producer app generates a Data packet containing the requested segment and sends it back as a response to the Consumer.
>    - If the sequence value is not within the valid range, indicating an invalid or out-of-order request, the Producer app discards the Interest packet.
>
> 3. Data Transmission:
>    - The Producer app continues to receive Interest packets and process data requests as long as the app is running.
>    - Whenever a valid Interest packet is received, the Producer app retrieves the corresponding segment based on the requested sequence value.
>    - The Producer app encapsulates the segment data in a Data packet and sends it back to the Consumer as a response.
>    - This process continues until all requested segments have been transmitted or until the app receives a termination signal.
>
> By following this Producer process app design, the Producer prepares the file by segmenting it into MTU-sized fragments with assigned sequence values. During the app's runtime, it waits for Interest packets from the Consumer and responds with the requested segments if the sequence value is valid. This design ensures that the Producer app effectively handles data requests and only returns the appropriate Data packets, while discarding invalid or out-of-order Interest packets.

> 100intps
>
> send_interest: 27660   recieve_data: 27460 responding with Data13225.0
> 1024 bytes 的包个数27456 interest_number27660 timeOut_number200
> goodput: 5.850907565720957Mb/s
>
> 200intps
>
> send_interest: 27860   recieve_data: 27460 responding with Data12720.0
> 1024 bytes 的包个数27456 interest_number27860 timeOut_number400
> goodput: 10.944443559666695Mb/s
>
> 1000intps
>
> send_interest: 29334   recieve_data: 27460 responding with Data8734.0
> 1024 bytes 的包个数27456 interest_number29334 timeOut_number1905
> goodput: 13.838080225111197Mb/s



> IMPLEMETATION
>
> 使用传输策略： strategy/multicast配置应用程序：配置 AppHelper
>
> 自定义Custom_Consumer类， 继承AppHelper
>
> Consumer::SendPacket()请求数据包，发送请求视频片段的兴趣包Consumer::OnData(shared_ptr<const Data> data)处理收到数据。根据seq合并接收到的数据包分片，存到本地中
>
> 自定义Custom_Producer类， 继承AppHelper
>
> Producer::StartApplication()在实例化producer的时候， 使用readFile() 和 splitFile() 实现对本地视频的分片，存储到数组中。Producer::OnInterest(shared_ptr<const Interest> interest)接收到兴趣包后，返回对应片段的数据包。



> ## Custom Consumer Implementation
>
> In order to optimize the transmission efficiency of video segments, we have employed a multicast transmission strategy, namely strategy/multicast. Our application is configured based on the NDN architecture, with the AppHelper serving as the base class for implementing the custom consumer functionality.
>
> We have introduced a custom class called Custom_Consumer, which inherits from the AppHelper class. This class primarily implements the following two methods:
>
> 1. **SendPacket()**: This method is responsible for sending interest packets to request specific video segments. When the consumer needs to retrieve a particular segment of video data, it invokes the SendPacket() method to send the corresponding interest packet.
>
> 2. **OnData(shared_ptr<const Data> data)**: This method handles the received data. When the consumer receives a data packet, it invokes the OnData() method to process the data. In our implementation, we merge the received data packet fragments based on their sequence number (seq) and store the combined data locally.
>
> By utilizing this custom consumer implementation, we can request and process video segment data. The adoption of the multicast transmission strategy allows for enhanced transmission efficiency, enabling seamless retrieval of video segments.
>
> ## Custom Producer Implementation
>
> Within the custom producer implementation, we focus on the StartApplication() and OnInterest(shared_ptr<const Interest> interest) methods to enable efficient data retrieval and transmission.
>
> Upon the instantiation of the producer, the StartApplication() method is invoked. This method facilitates the reading of the local video file using the readFile() function, which retrieves the video content. Subsequently, the splitFile() function is employed to divide the video into segments, storing them within an array for easy access.
>
> The OnInterest(shared_ptr<const Interest> interest) method is responsible for handling incoming interest packets from consumers. When an interest packet is received, this method extracts the corresponding segment's sequence number (seq) and retrieves the corresponding data packet from the array of video fragments. This data packet is then returned to the requesting consumer, satisfying their interest.
>
> Through the implementation of the custom producer class, efficient retrieval and transmission of video segments are facilitated. The combination of reading and splitting the local video file allows for seamless data organization and delivery, ensuring an optimal consumer experience.
>
> By employing both the custom consumer and custom producer implementations, we establish an efficient and reliable video streaming system. The multicast transmission strategy enhances data transmission efficiency, while the custom consumer and producer implementations enable seamless retrieval, processing, and delivery of video segment data.

有线和无线拓扑设为4个consumer节点和1个producer节点之间通过一个路由器相连

> 测试使用的文件大小是6MB，MTU大小是1024。总共分成了6583个包。分别进行3轮有线实验，第一轮中将发送请求设置为每秒钟发送100个兴趣包，第二轮是每秒钟发送200个兴趣包，第三轮是每秒钟发送1000个兴趣包。分别计算该场景下consumer总共发送的兴趣包个数， producer个总共发送的数据包个数以及timeout的包个数， 总共花的时间
>
> In the experiment, a file of size 6MB is used with an MTU (Maximum Transmission Unit) size of 1024. This results in a total of 6583 packets. The experiment is conducted in three rounds, each focusing on wired communication. 
>
> In the first round, the Consumer sends 100 Interest packets per second. In the second round, the Consumer increases the rate to 200 Interest packets per second. In the third round, the Consumer further increases the rate to 1000 Interest packets per second. 
>
> The following metrics are calculated for each round:
>
> 1. Total number of Interest packets sent by the Consumer: This represents the cumulative number of Interest packets generated by the Consumer during the experiment.
>
> 2. Total number of Data packets sent by the Producer: This indicates the overall count of Data packets transmitted by the Producer in response to the received Interests.
>
> 3. Number of timeout packets: These are packets for which the Consumer did not receive a corresponding Data packet within a specified timeout period.
>
> 4. Total elapsed time: This is the time taken for the entire experiment, including the time spent in generating and transmitting packets, processing delays, and any timeout durations.
>
> These metrics provide insights into the performance of the network under different sending rates and help evaluate the efficiency and reliability of the communication process.



A。在有限传输中，使用每秒发送兴趣包的个数，来检测网络拥堵，并找到当前链路中，较为合适的每秒发送兴趣包的个数。

数据如下表

在每秒钟发送兴趣包个数不同的情况下，通过比对吞吐量来检测网络拥堵状况。

B。链路模拟实验

5-7秒的时候，断开consumer node0 与producer node 5之间的链路。效果显著，这个期间node0发送的兴趣包都超时了，据统计，总共超时400个包

