/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-15 22:25:42
 * @LastEditTime: 2021-11-21 00:08:22
 * @LastEditors: 0x9DEFA478
 * @Description: 
 * QQ:2652450237
 * ============================================================================================================================================
 * 
 * 
 * 
 *                                                                                               ************      ****************************
 *                                                                                             ************      ****************************  
 *                                                                                           ************      ****************************    
 *                                                                                         ************      ****************************      
 *                                                                                       ************      ************                        
 *                                                                                     ************      ************                          
 *                                                                                   ************      ************                            
 *                                                                                 ************      ************                              
 *                                                                               ************      ************                                
 *                                                                             ************      ************                                  
 *                                                                           ************      ************                                    
 *                                                                         ************      ************                                      
 *                                                                       ************      ************                                        
 *                                                                     ************      ************                                          
 *                                                                   ************      ************                                            
 *                                                                 ************      ************                                              
 *                                                               ************      ************                                                
 *                                                             ************      ************                                                  
 *                                                           ************      ************                                                    
 *                                                         ************      ************                                                      
 *                                                       ************      ************                                                        
 *                                                     ************      ************                                                          
 *                                                   ************      ************                                                            
 *                                                 ************      ************                                                              
 *                                               ************      ************                                                                
 *                                             ************      ************                                                                  
 *                                           ************      ************                                                                    
 *                                         ************      ************                                                                      
 *                                       ************      ************                                                                        
 *                                     ************      ************                                                                          
 *                                   ************      ************                                                                            
 *                                 ************      ************                                                                              
 *                               ************      ************                                                                                
 *                             ************      ************                                                                                  
 *                           ************      ************                                                                                    
 *                         ************      ************                                                                                      
 *       ****************************      ************                                                                                        
 *     ****************************      ************                                                                                          
 *   ****************************      ************                                                                                            
 * ****************************      ************                                                                                              
 * 
 * 
 * 
 * ============================================================================================================================================
 * 
 */
#ifndef __Package_NET_Interface_H_
#define __Package_NET_Interface_H_
#include "H_Type.h"
#include "H_ThreadScheduler.h"

typedef struct{
  void* v;//底层句柄

  void (*Lock)(void* v);//锁定底层
  void (*Unlock)(void* v);//解锁底层
  void (*GetMACAddr)(void* v,unsigned char* MACAddr);//获取MAC地址
  void (*Start)(void* v,void (*TransmitDoneCallbcak)(void* net_interface,int IsISR),void (*ReceiveDoneCallbcak)(void* net_interface,int IsISR),void* net_interface);//启动底层外设
  void (*Stop)(void* v);//停止底层外设
  int (*TransmitBuffer)(void* v,int (*WriteCallback)(void* Callback_v,void* Data,int MaxLength),void* Callback_v);//发送数据 返回: 0:成功 -1:缓存不足 -2:写入的数据过长
  int (*ReceiveBuffer)(void* v,void (*ReadCallback)(void* Callback_v,void* Data,int DataLength),void* Callback_v);//读取接收缓冲区 返回: 0:成功 其他:缓存不足
  int (*IsLink)(void* v);//检测连接状态 0:未连接 其他:连接成功

}NET_Interface_LL_Function;


typedef struct{
  
  volatile int IsStart;

  H_TS_BinarySemaphore* TransmitDoneSem;
  H_TS_BinarySemaphore* ReceiveDoneSem;


  NET_Interface_LL_Function LL_Function;

}NET_Interface;







/**
 * @brief 初始化网络句柄 (初始化之后将不能反初始化 只能停止外设)
 * @param net_interface 要初始化的句柄
 * @param LL_Function 底层方法
 * @return 无
 */
void NET_Interface_Init(NET_Interface* net_interface,NET_Interface_LL_Function* LL_Function);

/**
 * @brief 启动网络外设
 * @param net_interface 网络句柄
 * @return 无
 */
void NET_Interface_Start(NET_Interface* net_interface);

/**
 * @brief 停止网络外设
 * @param net_interface 网络句柄
 * @return 无
 */
void NET_Interface_Stop(NET_Interface* net_interface);

/**
 * @brief 检查连接状态
 * @param net_interface 网络句柄
 * @return 0:无连接 其他:已连接
 */
int NET_Interface_IsLink(NET_Interface* net_interface);

/**
 * @brief 获取MAC地址
 * @param net_interface 网络句柄
 * @param MACAddr 6字节的MAC地址
 * @return 无
 */
void NET_Interface_GetMACAddr(NET_Interface* net_interface,Hbyte* MACAddr);





/**
 * @brief 发送数据
 * @param net_interface 网络句柄
 * @param WriteCallback 写数据回调  如果操作无问题, 此段将被调用写入数据 (返回:成功:写入长度 失败:负数  句柄v, 数据指针, 最大数据长度)
 * @param v WriteCallback被调用时的句柄v
 * @param Timeout 等待缓存超时时间 小于0永久等待
 * @return 0:成功 -1:超时 -2:要写入的数据过长 -3:外设未启动
 */
int NET_Interface_Transmit(NET_Interface* net_interface,int (*WriteCallback)(void*,void*,int),void* v,int Timeout);

/**
 * @brief 接收数据
 * @param net_interface 网络句柄
 * @param ReadCallback 读数据回调  如果操作无问题, 此段将被调用, 传入接收到的数据 (  句柄v, 数据指针, 数据长度)
 * @param v 网络句柄
 * @param Timeout 等待缓存超时时间 小于0永久等待
 * @return 0:成功 其他:超时 -3:外设未启动
 */
int NET_Interface_Receive(NET_Interface* net_interface,void (*ReadCallback)(void*,void*,int),void* v,int Timeout);



#endif //__Package_NET_Interface_H_
