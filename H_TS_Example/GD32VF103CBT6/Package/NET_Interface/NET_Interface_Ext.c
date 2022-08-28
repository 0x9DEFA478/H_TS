/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-15 22:24:53
 * @LastEditTime: 2021-11-20 23:33:18
 * @LastEditors: 0x9DEFA478
 * @Description: 实现了网络接口封装 底层与协议栈的连接
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
#include "NET_Interface.h"



//被底层调用 用于传入数据
extern void NET_Interface_ReceiveDoneCallback(void* net_interface,int IsISR);

//被底层调用 用于告知发送已完成
extern void NET_Interface_TransmitDoneCallback(void* net_interface,int IsISR);




/**
 * @brief 初始化网络句柄 (初始化之后将不能反初始化 只能停止外设)
 * @param net_interface 要初始化的句柄
 * @param LL_Function 底层方法
 * @return 无
 */
void NET_Interface_Init(NET_Interface* net_interface,NET_Interface_LL_Function* LL_Function){

  net_interface->LL_Function=LL_Function[0];

  net_interface->IsStart=0;

  net_interface->TransmitDoneSem=new_H_TS_BinarySemaphore();
  net_interface->ReceiveDoneSem=new_H_TS_BinarySemaphore();

}

/**
 * @brief 启动网络外设
 * @param net_interface 网络句柄
 * @return 无
 */
void NET_Interface_Start(NET_Interface* net_interface){
  net_interface->LL_Function.Lock(net_interface->LL_Function.v);

  if(net_interface->IsStart==0){
    net_interface->LL_Function.Start(net_interface->LL_Function.v,NET_Interface_TransmitDoneCallback,NET_Interface_ReceiveDoneCallback,net_interface);
    net_interface->IsStart=-1;
  }
  
  net_interface->LL_Function.Unlock(net_interface->LL_Function.v);
}

/**
 * @brief 停止网络外设
 * @param net_interface 网络句柄
 * @return 无
 */
void NET_Interface_Stop(NET_Interface* net_interface){
  net_interface->LL_Function.Lock(net_interface->LL_Function.v);

  if(net_interface->IsStart!=0){
    net_interface->LL_Function.Stop(net_interface->LL_Function.v);
    net_interface->IsStart=0;
  }
  
  net_interface->LL_Function.Unlock(net_interface->LL_Function.v);
}

/**
 * @brief 检查连接状态
 * @param net_interface 网络句柄
 * @return 0:无连接 其他:已连接
 */
int NET_Interface_IsLink(NET_Interface* net_interface){
  int r;

  net_interface->LL_Function.Lock(net_interface->LL_Function.v);

  r=net_interface->LL_Function.IsLink(net_interface->LL_Function.v);

  net_interface->LL_Function.Unlock(net_interface->LL_Function.v);

  return r;
}

/**
 * @brief 获取MAC地址
 * @param net_interface 网络句柄
 * @param MACAddr 6字节的MAC地址
 * @return 无
 */
void NET_Interface_GetMACAddr(NET_Interface* net_interface,Hbyte* MACAddr){
  net_interface->LL_Function.GetMACAddr(net_interface->LL_Function.v,MACAddr);
}




