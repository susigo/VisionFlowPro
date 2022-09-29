# VisionFlowPro
NodeEditor base on qt halcon c++ ,thanks to this great project https://github.com/paceholder/nodeeditor . I had added some custom feature,and include some halcon nodes.
In the near future i might make it a industrial machine vision software like VisionMaster.finger cross!
by susigo@foxmail.com 

## 一、项目启发
自从去年用了海康的VisionMaster后，做一款自己的拖拉拽机器视觉框架的想法就一直萦绕在我心中。所以在后续的时间里我在C#与C++中纠结，接触了大神开源的STNode后，被winform的难搞的绘图方式劝退。Qt有它独特的QGraphicsView框架，图元可以很方便地作为节点进行移动。因为找到的资料很少，恶心的CSDN什么都收费，当时自己摸索做了一个，使用void*指针传值，也可以勉强完成节点间传输数据的任务。后来C++水平提升，终于看懂了大神开源的框架。于是兴奋地通宵上传这个工程。本着开源精神，希望能够抛砖引玉。
如有指教可联系qq:2576662787

## 二、更新日志
***
1.更新了选区节点，现在可以坐下一步操作了。如绘制区域训练片训练匹配模型。
![reducedomain showcase](https://github.com/susigo/VisionFlowPro/blob/master/NodeEditorPro/showcase/ReduceDomain.gif)

2022/09/29
***
1. 可以保存项目与读取项目
2. 添加了SelectRegion的节点、添加了Connection节点。

![调通了halcon节点保存的逻辑](https://github.com/susigo/VisionFlowPro/blob/master/NodeEditorPro/showcase/showcase4.gif)

2022/09/23

***

