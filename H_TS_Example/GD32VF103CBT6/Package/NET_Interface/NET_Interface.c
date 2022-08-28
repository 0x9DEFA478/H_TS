/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-15 22:24:53
 * @LastEditTime: 2021-12-05 18:12:02
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
void NET_Interface_ReceiveDoneCallback(void* net_interface,int IsISR){
  if(IsISR==0){
    H_TS_BinarySemaphoreGive(((NET_Interface*)net_interface)->ReceiveDoneSem);
  }else{
    H_TS_BinarySemaphoreGive_ISR(((NET_Interface*)net_interface)->ReceiveDoneSem);
  }
}

//被底层调用 用于告知发送已完成
void NET_Interface_TransmitDoneCallback(void* net_interface,int IsISR){
  if(IsISR==0){
    H_TS_BinarySemaphoreGive(((NET_Interface*)net_interface)->TransmitDoneSem);
  }else{
    H_TS_BinarySemaphoreGive_ISR(((NET_Interface*)net_interface)->TransmitDoneSem);
  }
}





/**
 * @brief 发送数据
 * @param net_interface 网络句柄
 * @param WriteCallback 写数据回调  如果操作无问题, 此段将被调用写入数据 (返回:成功:写入长度 失败:负数  句柄v, 数据指针, 最大数据长度)
 * @param v WriteCallback被调用时的句柄v
 * @param Timeout 等待缓存超时时间 小于0永久等待
 * @return 0:成功 -1:超时 -2:要写入的数据过长 -3:外设未启动
 */
int NET_Interface_Transmit(NET_Interface* net_interface,int (*WriteCallback)(void*,void*,int),void* v,int Timeout){
  int r;

  if(net_interface->IsStart==0){
    return -3;
  }

  H_TS_BinarySemaphoreTryTake(net_interface->TransmitDoneSem);
Start:
  r=net_interface->LL_Function.TransmitBuffer(net_interface->LL_Function.v,WriteCallback,v);

  if(r!=0){
    if(r==-1){

      if(Timeout==0){
        return -1;
      }

      //等待信号量(等待有数据发送完成)
      if(Timeout>0){
        r=H_TS_BinarySemaphoreTake_Time(net_interface->TransmitDoneSem,Timeout);
      }else{
        H_TS_BinarySemaphoreTake(net_interface->TransmitDoneSem);
        r=0;
      }
      
      if(r!=0){
        return -1;
      }else{
        goto Start;
      }

    }else{
      return -2;
    }
  }

  return 0;
}

/**
 * @brief 接收数据
 * @param net_interface 网络句柄
 * @param ReadCallback 读数据回调  如果操作无问题, 此段将被调用, 传入接收到的数据 (  句柄v, 数据指针, 数据长度)
 * @param v 网络句柄
 * @param Timeout 等待缓存超时时间 小于0永久等待
 * @return 0:成功 其他:超时 -3:外设未启动
 */
int NET_Interface_Receive(NET_Interface* net_interface,void (*ReadCallback)(void*,void*,int),void* v,int Timeout){
  int r;

  if(net_interface->IsStart==0){
    return -3;
  }

  H_TS_BinarySemaphoreTryTake(net_interface->ReceiveDoneSem);
Start:
  r=net_interface->LL_Function.ReceiveBuffer(net_interface->LL_Function.v,ReadCallback,v);

  if(r!=0){

    if(Timeout==0){
      return -1;
    }

    //等待信号量(等待有数据接收完成)
    if(Timeout>0){
      r=H_TS_BinarySemaphoreTake_Time(net_interface->ReceiveDoneSem,Timeout);
    }else{
      H_TS_BinarySemaphoreTake(net_interface->ReceiveDoneSem);
      r=0;
    }
    
    if(r!=0){
      return -1;
    }else{
      goto Start;
    }

  }

  return 0;
}




