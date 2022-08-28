# H_TS

#### 介绍
一个线程调度器。也可以算作一个RTOS

目前支持:

  1.Cortex-M3 Cortex-M4F Cortex-M7 (在STM32F103、STM32F411、STM32F730、STM32H750上正常运行)
  
  2.基于riscv的Bumblebee (在GD32VF103上正常运行)



H_TS特性:

  1.H_TS运行过程中不会屏蔽中断，保证了中断的实时性。
  
  2.可创建的线程数量无上限(只要你内存够)
  
  3.线程结束很方便，线程的函数返回后即线程结束(return)，资源自动释放。如果想在其他地方退出线程(例如在线程调用的方法内部)，可显式调用一个线程结束方法来结束线程
  
  4.线程堆栈，句柄等内存全部都由动态内存获取
  
  5.H_TS本身不实现动态内存(但仓库提供了一个动态内存算法可以使用)，底层可配置内存管理算法
  
  6.能以很低的性能损耗来实现所有线程的CPU占用统计, 只需要占用一个硬件计数器。每一个线程都可以得到其的CPU占用率。
  
  
H_TS具有:

  1.线程有多种创建方式，可在创建后立即加入就绪队列，也可暂时不运行
  
  2.线程可睡眠(陷入阻塞态一段时间，此段时间内可供其他就绪线程运行)
  
  3.锁(仅线程之间使用) 具有优先级继承，仅限线程之间使用的锁，用于保护资源的互斥访问
  
  4.信号量( 线程之间使用时: 一个信号量只能有一个线程获取同一个信号量; 线程与中断之间使用时: 一个信号量只能由一个中断释放,一个信号量只能有一个线程获取同一个信号量)
  
  5.二值信号量，只能记录有无的信号量
  
  6.消息队列，一次固定传2个指针(限制同信号量)
  
  7.邮箱，它传输的是消息实体(将要传输的数据复制到其内部的FIFO), 方便了中断向线程发送消息的操作。并且其没有信号量的访问限制，除了接收还是只能同一个线程接收之外。

 
#### 架构

 **H_TS的运行需要占用：** 
 
 1.一个可由软件触发的中断，该中断要能处于最低优先级,并且在该中断正在运行时该中断可以以相同方式再次触发(悬起)，使得中断退出之后再次进入该中断。该中断被H_TS完全占用 (STM32中的PendSV)

 2.一个定时器中断，用于给H_TS提供时基。该中断可以不被H_TS完全占用，只需要在中断中调用H_TS的Tick处理函数即可 (STM32中的Systick)
 
H_TS只需要使用上面两个硬件资源，如果需要使能CPU占用率计算功能 还需要一个计时器(使得H_TS能获取一个时间)

H_Lib文件夹包含了H_TS的一些依赖，其中H_Type.h为必须的文件 其他的文件不是必须(例如H_Malloc负责内存管理 如果用户自己实现了内存管理则可以不需要H_Malloc)
H_TS\H_ThreadScheduler下有两个文件夹 API是用户接口，其中的代码无需修改。LL是底层接口，移植时需处理

#### 移植
将H_TS下的H_ThreadScheduler添加到工程，并且也要将H_Lib\H_Type.h、H_Lib\H_Malloc.c和H_Lib\H_Malloc.h添加到工程
将LL目录下的文件做相应修改

#### 使用
  H_ThreadScheduler.h中包含了H_TS使用过程中的所有API 只需要让使用它的文件包含它即可，并且内部有各个API的说明

  例程已在H_TS_Example中，包含了STM32F4，STM32F1和GD32VF103的例程

  更多的见说明文档。

#### 速度测试

  STM32F103, GD32VF103, STM32F411 例程都包含极限调度测试的代码，通过串口发送测试结果

  \H_TS_Example\H_TS_Example_xxx 内部包含了调度极限测试的代码
  
  \H_TS_Example\H_TS_Example_xxx\Code\APP\app_main.c 下包含了详细的测试代码
  


芯片条件: stm32f411 96MHz Flash配置: 指令预取开 指令缓存开 数据缓存开

  ARM编译器V5
  
  **配置条件:**
  
    阻塞链表数量vH_TS_BlockThreadListNum==1
	
    占位线程数量Stress_IsExtThread==0

                                       -O3优化+时间优化
    			
    Thread(Stress_IsTestISR==0)    avg:31375.51/100ms 3.18us
    
    SVC_ISR(Stress_IsTestISR!=0)   avg:27373.31/100ms 3.65us
	
	*后面us为单位的为每次调度所用的时间 100000us/cnt*
	
	
	
芯片条件: stm32f103 72MHz Flash使能预取

  ARM编译器V5

  配置条件:
    阻塞链表数量vH_TS_BlockThreadListNum==1
    占位线程数量Stress_IsExtThread==0

                                       -O3优化+时间优化                  -O0优化

    Thread(Stress_IsTestISR==0)    avg:19122.60/100ms 5.23us     avg:11200.14/100ms 8.93us
    
    SVC_ISR(Stress_IsTestISR!=0)   avg:16534.39/100ms 6.05us     avg:9994.09/100ms 10.0us
	
    *后面us为单位的为每次调度所用的时间 100000us/cnt*



芯片条件: gd32vf103cbt 108MHz 

  配置条件:
    阻塞链表数量vH_TS_BlockThreadListNum==1
    占位线程数量Stress_IsExtThread==0

                                          Ofast优化                      Og优化

    Thread(Stress_IsTestISR==0)    avg:36144.78/100ms 2.77us     avg:34336.73/100ms 2.91us
    
    SVC_ISR(Stress_IsTestISR!=0)   avg:26381.21/100ms 3.79us     avg:25270.41/100ms 3.96us
	
    *后面us为单位的为每次调度所用的时间 100000us/cnt*

