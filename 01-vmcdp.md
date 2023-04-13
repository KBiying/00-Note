#  vmcdp

## Design

#### A. Modeling Action according to State

First, we need to investigate how users determine their actions according to their states. An action is whether or not the agent accepts a relay request from other agents. The agent's state includes remaining battery charge, data quality, and relationship with requester.

Each agent completely executes the same actions (accept, reject, and cancel) according their states as the corresponding subject acted through the emulation.

#### B.Model parallel training. 

Model parallel training is another popular distributed training paradigm. Our current implementation supports data parallel traininbbbbbbbbb but model parallel training can be supported similarly as data parallel training. Specifically, for the forward propagation, each worker has three stages, i.e., receiving intermediate data from the previous worker, computing, and sending intermediate data to the next worker. The first worker replaces the first stage with loading data and preprocessing, while the last worker replaces the last stage with synchronizing gradients. Different stages mainly use different resource types considering the full-duplex network. For the backward propagation, each worker has three stages that mainly use different resource types as well. Therefore, Muri can support model parallel training by (𝑖) interleaving stages in one model parallel training job with stages of the same propagation direction in other jobs, and (𝑖𝑖) adjusting the interleaving efficiency for the Blossom-based scheduling algorithm.

> 模型并行训练。模型并行训练是另一种流行的分布式训练范式。我们当前的实现支持数据并行训练，但模型并行训练可以类似于数据并行训练进行支持。具体来说，对于前向传播，每个工作人员有三个阶段，即从前一个工作人员接收中间数据，计算和发送中间数据到下一个工作人员。第一个工作人员用加载数据和预处理替换第一个阶段，而最后一个工作人员用同步梯度替换最后一个阶段。不同阶段主要使用不同的资源类型考虑全双工网络。对于反向传播，每个工作人员也有三个主要使用不同资源类型的阶段。因此，Muri可以通过（𝑖）将一个模型并行训练作业中的阶段与其他作业中具有相同传播方向的阶段交错，并（𝑖𝑖）调整基于Blossom的调度算法的交错效率来支持模型并行训练。

 

#### Why ndn?

#### B. what about we transmit a wrong data?



### Experiment

1. How far and how fast can our vmac transmit data.

2. Compare: IEEE802.11a multicast, adhoc
3. Metric that indicate the advantage of our scheme. e.g. we can evaluate the average bit rate of cumulative distribution of all agents.





