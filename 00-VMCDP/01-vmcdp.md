#  vmcdp

## Design

#### A. Modeling Action according to State

First, we need to investigate how users determine their actions according to their states. An action is whether or not the agent accepts a relay request from other agents. The agent's state includes remaining battery charge, data quality, and relationship with requester.

Each agent completely executes the same actions (accept, reject, and cancel) according their states as the corresponding subject acted through the emulation.

#### B.Model parallel training. 

Model parallel training is another popular distributed training paradigm. Our current implementation supports data parallel traininbbbbbbbbb but model parallel training can be supported similarly as data parallel training. Specifically, for the forward propagation, each worker has three stages, i.e., receiving intermediate data from the previous worker, computing, and sending intermediate data to the next worker. The first worker replaces the first stage with loading data and preprocessing, while the last worker replaces the last stage with synchronizing gradients. Different stages mainly use different resource types considering the full-duplex network. For the backward propagation, each worker has three stages that mainly use different resource types as well. Therefore, Muri can support model parallel training by (𝑖) interleaving stages in one model parallel training job with stages of the same propagation direction in other jobs, and (𝑖𝑖) adjusting the interleaving efficiency for the Blossom-based scheduling algorithm.

> 模型并行训练。模型并行训练是另一种流行的分布式训练范式。我们当前的实现支持数据并行训练，但模型并行训练可以类似于数据并行训练进行支持。具体来说，对于前向传播，每个工作人员有三个阶段，即从前一个工作人员接收中间数据，计算和发送中间数据到下一个工作人员。第一个工作人员用加载数据和预处理替换第一个阶段，而最后一个工作人员用同步梯度替换最后一个阶段。不同阶段主要使用不同的资源类型考虑全双工网络。对于反向传播，每个工作人员也有三个主要使用不同资源类型的阶段。因此，Muri可以通过（𝑖）将一个模型并行训练作业中的阶段与其他作业中具有相同传播方向的阶段交错，并（𝑖𝑖）调整基于Blossom的调度算法的交错效率来支持模型并行训练。

**模型分配任务（1）：**可以考虑设备异构性

**向谁请求信息:**根据reward表(需要更新)得出决策表(需要更新)，可以决策出分组情况。组内坏学生向好学生发请求/好学生也会主动发出信息。

**时间节点：** 200步数后做一次迭代，此时需要更新reward表，然后DL决策，更新决策表(如果决策是根据reward表加随机概率去分组的话，不一定需要DL)

**agent个数动态变化（2）：**

*如何更新reward表？* 最长匹配进行查询。

*设计点是什么：对比于传统广播的传输效率*

*其中的延迟是否需要考虑？：比如我网络拥塞了，来不及更新信息参数。导致你用了旧参数进行了一次多余的迭代。*

#### Why ndn?

#### C. what about we transmit a wrong data?

数据的完整性

### D. Strategy

- 强化学习 需要隔一段时间去迭代,**需要时间参数**每次迭代完成后,会生成相应的梯度和参数,这些数据RL中是矩阵的形式存在,但是为了方便传输,可以转化为向量形式.

- 另外,有一个虚拟总控制,来规定哪个"好学生"带哪些"坏学生".(1-n).**可以设计为不同的组.**每个需要加强训练的智能体如何知道自己不够好,以及需要接收的数据是什么? 在RL层面就由全局变量[0,1,0,1,1]来规定,哪些智能体还需要训练.在网络传输方面

- They are assigned to mobile devices based on the availability of resources and features of the data available to the mobile device. 他们根据 资源利用率和可用数据的特征 分配数据给边缘设备
  - **那就是说我们同时需要考虑边缘设备的资源？还是说一开始就已经固定哪个边缘设备要做什么task？然后永久不变？或是说，一开始根据边缘设备能力去分配task？然后一直保持固定任务不变？**

- They are used to build evolving data mining models and consult mining agents for a final collaborative decision when required by one or more users. 
  - 当被多个agent请求数据的时候， 建立data mining agent 模型以及咨询mining agent最后的决策


#### E. 

**strategy network:** 

Each agent broadcast request of reward and then update reward table. The time for update is 10 seconds. The prerequisite of broadcast is fixed, about 200 steps after the moving of player

According to the reward table, they **decide** which agent to learn from. And then send the request of data to the given agent.

The decision is random.(二项分布生成)

#### naming:

先假设每个node都有自己唯一特定的node-id，each node发送name为 “/reward” 的*interest package*

收到reward类型的node，返回name为“/reward/node-id”的*data package.*

更新reward表的时间为10秒（这么秒数暂定）<这个过程就可以知道这个网里有哪些node>

根据reward表，随机选择要学习的对象，发送name为"/data/destination-node-id/source-id"的*interest package*

node收到含有自己名称且含有data请求的interest包后，返回name为“/data/destination-name/source-name”的*data package*。

#### IMPLEMENTATION:

#### **userspace**

- **struct reward**
  - workerID int
  - reward number int

- **registeID()**
  - 为该node节点分配id
- **process()**
  - 处理vmac层传来的数据
  - 如果是interest帧,如果是data帧
- **recvfromVmac()**
  - 接收从vmac层传来的数据包
- **sendtoVmac()**
  - 发送数据包给vmac
- **addToRewardTable()**
  - 向reward表中填写数据
  - reward表是一个链表

- **delFromRewardTable()**
  - 从reward表中删除数据
- **iterationTime()**
  - 迭代计时器
- **getData()**
  - 读写文件获取训练数据

#### **vmac层**

- **recv()**
  - 接收帧,并按照规则过滤帧
- send()
  - 封装帧,发送数据

ATTENTION:开线程并行

##### register server：

为了动态加入节点，且保证节点id的唯一性

> 如何做到非常有效的[分布式执行](https://www.zhihu.com/search?q=分布式执行&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A1055207335})（decentralized execution），训练的时候可以任意作弊（centralized training），可以用全信息，可以知道其它agent的内部状态，可以知道其它agent的行动目的，等等；但在测试的时候，我们希望每个agent只用自己所见进行决策，同时还能相互配合着完成任务。这是目前一个比较火的方向。如果是走这个方向。我们就需要加一个

### Experiment

1. How far and how fast can our vmac transmit data.
2. Compare: IEEE802.11a multicast, adhoc
3. Metric that indicate the advantage of our scheme. e.g. we can evaluate the average bit rate of cumulative distribution of all agents.

### Task1--coding

1. 实现使用netlink传输文件中的结构体

2. 结构体数据包含时间,向量,控制组

3. vmac里面有control结构体,以及seq

### Task2--Hardware

1. nano安装操作系统

2. run vamc

3. config rl env & run rl modules



### 应用场景

机器人足球比赛，交通灯控制，多机器人走迷宫

# Multi-Agent Reinforcement Learning

## Setting

> 多智能体强化学习通常有四种设定
>
> - **Fully cooperative**
>   **完全合作关系，这种设定里面，Agents的利益一致，获得的奖励相同，有共同的目标.**
> - Fully competitive
>   完全竞争关系，一方的收益是另一方的损失。典型的代表就是0和博弈，双方获得的奖励的总和为0.
> - Mixed Cooperative & competitive
>   既有竞争也有合作。例如：足球机器人踢球，两支球队，一方获得的奖励就是另一方的损失，但是球队内部成员是合作关系。
> - Self-interested
>   利己主义。系统中有多个Agents，一个Agent的动作会改变环境的状态，可能让别人受益或者受损。每个Agent只想最大化自身利益，至于让别人受损还是受益它不管。

## Architectures

> - Fully decentralized(去中心化)：
>
>   每个Agent都是独立的个体， 每个Agent独立和环境交互，并且用自己的观测和奖励来更新自己的策略。Agent彼此之间不通信，不和环境交流。前面我们介绍了去中心化，并且分析了缺点。
>
> - Fully centralized(完全中心化)：
>
>   所有Agent都把信息传送给中央控制器，中央控制器知道所有Agent的观测、动作以及奖励。Agent上没有策略网络，Agent自己不做决策，决策都是由中央控制器做的，Agent只是执行。
>
> - Centralized training with decentralized execution(中心化训练，去中心化执行)：
>
>   Agent各自有各自的策略网络，训练的时候有一个中央控制器，这个中央控制器会收集所有Agent的观测，动作以及奖励。中央控制器帮助Agent训练策略网络。训练结束之后就不再用中央控制器了，每个Agent根据自己的观测用自己的策略网络来做决策，不需要跟中央控制器通信。

大背景

technical intro：强化学习

1.提高学习精确度

2.并降低每个设备的能耗

通过share信息互通，更好地训练模型。

为什么需要多播而不是广播？因为多样性，部分子集需要需求。

网络延迟是否需要考虑？传输时间，delay

与传统多播相比，我们的多播的优势？我们不需要加入多播组enter access；

广播阶段+多播阶段

传输内容：最大4KB 60000*64bit

分组？每一次都选择新一组

文章writing重点：

实验

1.纯广播（两个阶段都用广播）需要多少时间，浪费多少时间。

2.vmac experience有限和无线和NDN simulation。NDN有线协议和无线协议模拟



 #  强化学习peer to peer multi agent learning

> 
> Peer-to-peer multi-agent learning in the context of reinforcement learning refers to a scenario where multiple agents interact and learn from each other in a decentralized manner, without relying on a central authority or global coordination.
>
> In traditional reinforcement learning, there is typically a single agent that interacts with an environment and learns from the feedback received. However, in peer-to-peer multi-agent learning, there are multiple agents, and each agent acts as both a learner and a decision-maker. These agents interact with the environment, receive feedback, and also learn from the experiences of other agents.
>
> In this setting, agents can communicate and share information with each other to improve their learning and decision-making processes. This communication can occur directly between agents or indirectly through a shared environment. Agents can exchange learned policies, value functions, or other relevant information to enhance their individual learning processes.
>
> The key motivations and benefits of peer-to-peer multi-agent learning in reinforcement learning include:
>
> 1. Distributed Knowledge: Agents can leverage the collective knowledge and experiences of other agents to improve their learning. By learning from each other, agents can discover better policies or strategies that may be difficult to find individually.
> 2. Exploration and Exploitation: Peer-to-peer multi-agent learning allows agents to explore a wider range of actions and strategies. Agents can learn from each other's exploratory behaviors and collectively exploit the knowledge gained from various experiences.
> 3. Robustness and Adaptability: Agents in a peer-to-peer learning system can adapt to changes or perturbations more effectively. If one agent fails or encounters a new situation, other agents can provide guidance or knowledge to help adapt and overcome challenges.
> 4. Scalability: Peer-to-peer multi-agent learning can scale well to large and complex environments. Agents can collaborate and learn in parallel, enabling efficient learning in settings with a high number of agents or a large state and action space.
>
> It's worth noting that peer-to-peer multi-agent learning introduces additional challenges such as coordination, communication, and stability. Designing effective communication protocols, managing exploration and exploitation trade-offs, and ensuring convergence are important considerations in developing successful peer-to-peer multi-agent learning algorithms and systems.
>
> 在强化学习中，点对点多智能体学习（Peer-to-peer multi-agent learning）是指多个智能体以去中心化的方式相互交互和学习的场景，而无需依赖中央控制机构或全局协调。
>
> 在传统的强化学习中，通常只有一个智能体与环境进行交互，并从接收到的反馈中学习。然而，在点对点多智能体学习中，存在多个智能体，每个智能体同时充当学习者和决策者的角色。这些智能体与环境交互，接收反馈，并且还从其他智能体的经验中学习。
>
> 在这种设置下，智能体可以相互通信和共享信息，以改进它们的学习和决策过程。这种通信可以直接在智能体之间进行，也可以通过共享的环境间接进行。智能体可以交换学习到的策略、值函数或其他相关信息，以增强各自的学习过程。
>
> 点对点多智能体学习在强化学习中的关键动机和优势包括：
>
> 1. 分布式知识：智能体可以利用其他智能体的集体知识和经验来改进学习。通过相互学习，智能体可以发现更好的策略或战略，这些可能在个体学习中很难获得。
> 2. 探索与利用：点对点多智能体学习使智能体能够探索更广泛的行动和策略。智能体可以从彼此的探索行为中学习，并从各种经验中共同利用所获得的知识。
> 3. 鲁棒性和适应性：点对点学习系统中的智能体可以更有效地适应变化或干扰。如果一个智能体失败或遇到新情况，其他智能体可以提供指导或知识，帮助适应并克服挑战。
> 4. 可扩展性：点对点多智能体学习可以适用于大型和复杂的环境。智能体可以并行协作和学习，在智能体数量众多或状态和动作空间庞大的情况下实现高效学习。
>
> 需要注意的是，点对点多智能体学习也带来了额外的挑战，如协调、通信和稳定性。设计有效的通信协议，管理探索与利用的权衡，确保收敛性是开发成功的点对点多智能体学

##  关于点对点多智能体学习之间的通信协议或通信规则怎么设计

> 点对点多智能体学习中的通信协议或通信规则的设计是一项重要且具有挑战性的任务。下面是一些设计这些通信方案时需要考虑的关键因素：
>
> 1. 信息共享：确定智能体之间需要共享哪些信息，如经验、策略、价值函数等。这些信息的共享可以通过消息传递、共享内存或其他形式实现。
> 2. 通信拓扑：确定智能体之间的通信拓扑结构，即哪些智能体可以直接通信，哪些需要经过中间代理或路由器进行通信。可以选择全互联（fully connected）、局部邻居（local neighbors）或者其他特定的拓扑结构。
> 3. 通信频率：确定智能体之间的通信频率，即多久发送一次通信或更新信息。频率的选择需要权衡通信开销和信息同步的速度。
> 4. 信息格式：定义信息的格式和编码方式，以确保不同智能体之间的信息交流是准确、一致和可解释的。常见的格式可以是向量、矩阵、JSON等。
> 5. 协议同步：确保智能体之间的通信和信息同步是有序和一致的。可以使用同步协议来确保消息的顺序和一致性，例如时间步同步、事件触发同步等。
> 6. 通信安全性：考虑智能体之间的通信安全性，防止信息被篡改、泄露或伪造。可以使用加密、身份验证和数字签名等安全机制来保护通信。
> 7. 可扩展性：设计通信协议时需要考虑系统的可扩展性，以支持大规模的多智能体系统。通信方案应该能够适应增加或减少智能体数量的变化。
>
> 设计通信协议时需要结合具体的应用场景和问题要求。可以通过模拟、实验和调整来优化通信协议，以提高系统性能和学习效果。此外，还可以参考已有的研究成果和通信协议的最佳实践，以获得更好的设计思路和指导。

