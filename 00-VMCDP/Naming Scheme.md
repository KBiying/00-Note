### Naming Scheme

The process of ICN multicast, including infinite state machine and explanation
(need mention inter-process communication)

简单介绍ERL系统大概是要做什么：首先ERL进行本地训练，得出性能指标。agents相互请求性能指标数据。每个agent根据全局性能指标来评判自己的学习效果，以及评估出比自己学习效果好的agent，并向之请求数据用于自身训练。



为此，在传输层中分两个阶段满足ERL的数据传输需求。

第一阶段：agents相互请求性能指标数据，因此传输层需要广播请求neigbour的性能指标

第二阶段：请求数据用于自身训练

根据这两个阶段的需求，我们设计了naming scheme

第一阶段multicast：/broadcast/performanceIndex。各个agent向网络发送该interest包，其他agents收到interest包后返回自己的performance index。收到来自各agent的performanceindex。当然啦，由于网络不稳定性，有可能收不齐。此时就需要重传。

第二阶段根据subset请求。/parameters/<>/<>/<identifier> identifier具有唯一性，标识每次迭代训练的编号，。请求回来的数据是什么

两个阶段要请求的数据内容存放到payload当中，简要介绍需要共享的数据内容：performance index which is a float type 和parameters which is a list contains multi float type number. 

我们通过进程间实现状态的转化。

描述有限状态机：We implement state transitions through inter-process. The finite-state machine (FSM) definitions for the ERL and ICN are shown in Fig. 1. The FSM in figure 1.a defines the operation of the ERL, while the FSM in figure 1.b defines the operation of the ICN. The arrows in the FSM description represent the protocol's movement from one state to another. The dashed arrow represents the FSM's initial state, assumed to be S0. Besides, these transitions represent cyclical behavior, where the machine loops back to S0 after reaching the final state.

The ERL FSMs in Fig. 1 have 6 states, which will be denoted as S0, S1, S2, S3, S4, and S5. Each state is distinct and identifiable. At the beginning, the ERL application is waiting for the start signal issued by ICN. It enters state S1 after receiving the start signal to train a neural network. Following training, ERL generates a performance index that indicates the training performance of this iteration and then sends it to ICN. After transmission, it enters S2 and waits for ICN to collect performance indices accompanied by an identifier from surrounding agents. Compare the collected performance indices in state S3. If the performance of local is so good that it doesn't need to learn from others, ERL will send an empty subset and optimized parameters of local to ICN, then finally return to state S0. Otherwise, ERL will send a subset containing identifiers indicating the learning object and optimized local parameters to ICN, then wait for collected parameters to be collected through ICN in state S4. Finally, ERL gathers the collected parameters from ICN and merges the paras for better and more accurate predictions or decisions. Then roll back to state S0, waiting for a new round of training.

Likewise, the ICN FSMs in Fig. 1(b) have 5 states. Initially in state S0, the FSM awaits its own performance index from the Emssemble Reinfoce Learning(ERL). It moves to state S1 after obtaining the performance index, where it gathers performance indices from neighboring agents. After returning the collecting results to ERL, in state S2, the ICN awaits for its own parameters and subset . If ERL returns an empty subset which means this agent learns very well, so it does not need to learn from others and then returns to S0 to await the next ERL performance index. If ERL return a non-empty subset, it advances to state S3 and begins collecting parameters from neighboring agents. After gathering the required parameters, the ICN advances to state S4 to begin the next round by delivering the collected parameters. Finally, roll back to state S0 by sending start signal.



Similarly, the ICN FSMs in Fig. 1(b) contain five states. The FSM is initially in state S0, awaiting its own performance index from the Emssemble Reinfoce Learning (ERL). After acquiring the performance index, it proceeds to state S1, where we collect performance indices from nearby agents through ICN.In stage S2, the ICN awaits its own parameters and subset after submitting the collecting results to ERL. If ERL returns an empty subset, this agent has learned very well and does not need to learn from others, and it returns to S0 to await the next ERL performance index. If ERL returns a non-empty subset, the process moves to S3 and continues collecting parameters from surrounding agents. The ICN advances to state S4 to start the following round by sending the appropriate parameters gathered. Finally, transmit the start signal to return to state S0.



开始命令是ICN发出的，标志着ICN已经准备好接收ERL的数据了：The initiation command, which is issued by Information-Centric Networking (ICN), serves as a pivotal signal indicating that ICN is now prepared to receive data from the ERL (External Resource Location). 

书本对状态机的描述：The finite-state machine (FSIM) definitions for the rdt1. O sender and receiver are shown in Figure 3.9. The FSM in Figure 3.9(a) defines the operation of the sender, while the FSM in Figure 3.9(b) defines the operation of the receiver. It is important to note that there are separate FSMs for the sender and for the receiver. The sender and receiver FSMs in Figure 3.9 each have just one state. The arrows in the FSM description indicate the transition of the protocol from one state to another. (Since each FSM in Figure 3.9 has just one state, a transition is necessarily from the one state back to itself; we'll see more complicated state diagrams shortly.) The event causing the transition is shown above the horizontal line labeling the transition, and the actions taken when the event occurs are shown below the horizontal line. When no action is taken on an event, or no event occurs and an action is taken, we'll use the symbol A below or above the horizontal, respectively, to explicitly denote the lack ot an action or event. The initial state of the FSM is indicated by the dashed arrow. Although the FSMs in Figure 3.9 have but one state, the FSMs we will see shortly have multiple states, so it will be important to identify the initial state of each FSM.

今天的任务是把流程和naming理顺，写出论文描述

实验设计

jetson跑pi做传输的事情一句话带过，主要强调的是我们开的是进程处理这样的事情。

Thurther work：

1.cs

### FLARE: Federated Active Learning Assisted by Naming for Responding to Emergencies

#### 3.1 Name-based Pub/Sub in FLARE

The communication between entities in FLARE is name-based (to enable information centricity [33]), used both for disseminating instructions and reports between first responders, and also learning/inference-related information sharing among SMEs. 

他们之间的communication通过name-based， 用这个来disseminating instructions and reports.

A unified namespace, such as the one in Fig. 2, guides this informationcentric framework, and is a common interface that various primary actors and components (i.e., first responders, SMEs, and Incident Coordinator but not all the other social media users) have a local copy of, and **can use to indicate which subset of the incident namespace they are interested in (i.e., subscribed to), and want to deliver to**. First responders, based on their assigned task (either based on the incident or as part of their organizational hierarchy), **subscribe to a prefix in the namespace. For example, a firefighter FF1 assigned with tasks related to “Fire Engine 1” will subscribe to “/IncidentX/LA/Fire/Firefighting/FireEngine1”.**

 The subscription can be at any desired granularity: e.g., another firefighter, let us call him/her FF2, who manages all fire engines in LA, will subscribe to “/IncidentX/LA/Fire/Firefighting”. **Any publication 𝑃1 with a prefix** “/IncidentX/LA/Fire/Firefighting” **will be delivered to** FF2. Additionally, FLARE expands the delivery according to the name hierarchy to reach all relevant recipients.

简单介绍一下一些subscription

 **To provide that capability in a flexible and efficient manner, here we use an NDN architecture that is enhanced with recipient-based pub/sub logic (as proposed in [8]).** This forwarding logic matches an incoming packet with all entries that contain the packet’s name as a prefix. Thus, 𝑃1 will be delivered to the subscribers of names with the prefix containing “/IncidentX/LA/Fire/Firefighting”, including FF1 as well. In addition to the prefix, the publication names in FLARE can **include other attributes as well,** such as tags and an arbitrary number of input parameters. The named-SMP (NSMPs) containing instructions for first responders **will be published (typically by SMEs) with the following name format:** “/[prefix]/tag=instr/[params]”. **The prefix** (e.g., “/IncidentX/LA/Fire/Firefighting”) will be used to forward the publication to the right recipients. **The tag** is used to indicate the type of the message payload (e.g., instr). **The params** are input parameters used by the recipients to process the received information, as in “.../𝑝𝑎𝑟𝑎𝑚1 = 𝑣𝑎𝑙𝑢𝑒1/𝑝𝑎𝑟𝑎𝑚2 = 𝑣𝑎𝑙𝑢𝑒2/...”. FLARE also uses name-based delivery for message exchange among SMEs **in its Federated Learning-based procedures**, integrating the namespace with different classifiers. Each SME is organizationally in charge of processing and manually labeling SMPs associated with certain prefixes, which they subscribe to. E.g., 𝑆𝑀𝐸1 in Fig. 1 subscribes to “/IncidentX/LA/Fire/SME”. The dispatcher with 𝑆𝑀𝐸1 has specialized domain knowledge to label SMPs regarding “Fire” and its sub-categories, e.g., “SurvivalSearch”, etc.

 There are three types of SME-related messages: 1) SME-to-SME (S2S) messaging: An SME asking any SME or a particular SME with the prefix 𝑝 to process an SMP (for calculating, labeling, etc.): “/[𝑝]/tag=proc/[𝑝𝑎𝑟𝑎𝑚𝑠]”, **where 𝑝𝑎𝑟𝑎𝑚𝑠 can include SMP ID, confidence values, etc.** The payload of this message type is SMP content (e.g., a tweet json). 2) SME-to-Incident Coordinator (S2I) messaging: An SME sends its processing result from training to incident coordinator for aggregation: “/IncidentX/Coord/ tag=result/[𝑝𝑎𝑟𝑎𝑚𝑠]”, where 𝑝𝑎𝑟𝑎𝑚𝑠 can include which classifier it includes the result it is associated with (whether C1 or C2). The payload of this message type is the calculation result, e.g., weights. 3) I2S messaging: The incident coordinator distributing fully trained models to a set of SMEs under prefix 𝑝 for synchronization: “/[𝑝]/tag=model/[𝑝𝑎𝑟𝑎𝑚𝑠]”, where 𝑝𝑎𝑟𝑎𝑚𝑠 can include the classifier, version number, etc. The payload of this message is the most recent, fully trained model, after the incident coordinator’s aggregation procedure is completed. The namespace has a hierarchical structure (as a prefix tree). The hierarchical levels are divided into three levels, namely root, static, and dynamic levels to capture both how the namespace nodes are managed, and their relationship with the SME classification procedures. The root identifies the name of the namespace. The static levels follow the organizational/incident command structure, based on a template, created at the beginning, when the response to the incident is initiated and organized. They remain static during the disaster’s management. The dynamic levels represent more fine-grained roles under the static levels. **Name nodes within the dynamic levels can be created, modified or removed as the disaster unfolds, based on command decisions.**

介绍有三种情况的nameing



We have 3 type of interest name.

1) Discover performance index from neighbour agents in phase 1: /pfm/discovery.

2) Request specific parameters from neighbour agents in phase 2: /paras/<identifier>

3) Collect parameters from neighbour agents in phase 3: /paras/discovery



第一阶段的内容：更新自己的pfm后， 每次训练都会产生唯一的identifier和pfm以及paras。训练的性能指标pfm在一定程度上表示该agent训练的情况是优还是劣。去发现附近智能体的pfm，收集起来之后通过分析pfm，来确定哪个pfm值得学习。要完成的数据传输目标是：通过pub/sub来发现和收集附近智能体的pfm，请求兴趣包的名称为/pfm/discovery。当agent收到该兴趣包的时候，就会publish自己的最新迭代的pfm和标志该迭代的identifier。

The task of first phase: In Emssemble learning, each trainning iteration will generate a sole identifier, perforamance index and parameters. After one round trainning, we update performance index which can indicate the trainning performance to some extend. So each agent get updated performance now, we need to share the performance to figure out which one we can learn from. Therefore, The transmittion task is to discover and collect performance index of neigbor agents through pub/sub protocol. The interest name is designed as /pfm/discovery. While someone receive this interest package, they will publish/response with their latest performance index and corresponding identifier. 

第二阶段的内容：确定好需要学习的对象之后，向该对象请求训练参数，好的参数可以更快地提高自己的训练能力。由于每次训练都会产生唯一的identifier和pfm以及paras， 因此可通过唯一值，去获取identifier相对应的那轮参数。要完成的数据传输目标是：通过pub/sub来发现和收集paras of well-trained agent. 由于pub/sub规定，请求数据的名称要与内容相关，而与位置或生产该数据的对象无关。因此我们通过请求某一轮的identifier来获取到该paras，请求的兴趣包名称为/paras/<identifier>。identifier为一个64bit的hash值，可以唯一标识某次迭代训练。当拥有该paras的agent收到该兴趣包后，会返回对应的paras。

The task of second phase: After collecting the performance index, ERL will choose the best one or two performance index that represents well-trained and is better than self performance. Then we will request the trainning parameters of well-trained agent through corresponding identifier. The interest name is designed as  /paras/<identifier>. Identifier is a 64bit hashed number, which can identify the iteration of trainning. The agent who own this parameter will response the  package.

在第三阶段，称为总验证阶段，选取1个agent， 获取所有agent的最后一次迭代的paras，做融合。目的是获取集合之后的模型参数。要完成的数据传输目标是：通过pub/sub来请求邻近所有agent的训练成果。（万一没有训练完呢？）作参数融合，可综合出最后的模型参数。

此方案，不仅可以高效完成通过多播的数据传输 以及 应对动态加入的情况。

我们要完成的目标就是为ERL share infomation 提供数据传输平台

The communication between agents in XXX is name-based (to enable information centricity[xx]), This method is applied for the dissemination of performance indices as well as the sharing of parameters between agents. In this case, we employ an NDN architecture supplemented with recipient-based pub/sub logic (as presented in [xx]).

The objective of the first phase: In Ensemble Learning, each training iteration generates a unique identifier, performance index, and parameters. After one training round, the performance index is updated, which can indicate the training performance to some extent. As each agent receives updated performance, it is necessary to share the performance to determine which one can be learned from. Thus, the transmission task involves discovering and collecting the performance index of neighbouring agents through the pub/sub logic. The name of the interest is designated as /pfm/discovery. When an individual receives this interest package, they will publish a response that includes their latest performance index and corresponding identifier.

In the second phase of the project: Once the performance indices have been collected, ERL will select one or two performance indices that accurately represent well-trained models and outperform self-performance. Subsequently, the training parameters of the well-trained agent will be obtained through the relevant identifier. The identifier corresponds to the interest name /paras/<identifier>. The identifier is a 64-bit hashed number that is capable of identifying the training iteration. The agent that possesses these parameters will then respond to the package.

