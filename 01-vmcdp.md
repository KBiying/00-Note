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

*如何更新reward表？*

*设计点是什么：对比于传统广播的传输效率*

*其中的延迟是否需要考虑？：比如我网络拥塞了，来不及更新信息参数。导致你用了旧参数进行了一次多余的迭代。*

#### Why ndn?

#### C. what about we transmit a wrong data?

数据的完整性

### D. Strategy

强化学习 需要隔一段时间去迭代,**需要时间参数**每次迭代完成后,会生成相应的梯度和参数,这些数据RL中是矩阵的形式存在,但是为了方便传输,可以转化为向量形式.

另外,有一个虚拟总控制,来规定哪个"好学生"带哪些"坏学生".(1-n).**可以设计为不同的组.**每个需要加强训练的智能体如何知道自己不够好,以及需要接收的数据是什么? 在RL层面就由全局变量[0,1,0,1,1]来规定,哪些智能体还需要训练.在网络传输方面

- They are assigned to mobile devices based on the availability of resources and features of the data available to the mobile device. 他们根据 资源利用率和可用数据的特征 分配数据给边缘设备
  - **那就是说我们同时需要考虑边缘设备的资源？还是说一开始就已经固定哪个边缘设备要做什么task？然后永久不变？或是说，一开始根据边缘设备能力去分配task？然后一直保持固定任务不变？**

- They are used to build evolving data mining models and consult mining agents for a final collaborative decision when required by one or more users. 
  - 当被多个agent请求数据的时候， 建立data mining agent 模型以及咨询mining agent最后的决策

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





