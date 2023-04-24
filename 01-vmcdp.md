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

