/*
 * @Author: 0x9DEFA478
 * @Date: 2021-04-29 00:46:19
 * @LastEditTime: 2021-04-29 00:49:39
 * @LastEditors: 0x9DEFA478
 * @Description: 按键抽象头文件 定义了按键的抽象
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
#ifndef __H_Key_H_
#define __H_Key_H_
#include "H_Type.h"




#define vH_Key_Event_isPress     0x80000000U
#define vH_Key_Event_Type        0x7FFF0000U
#define vH_Key_Event_KeyID       0x0000FFFFU

#define vH_Key_Event_Press       0x00010000U
#define vH_Key_Event_Up          0x00020000U
#define vH_Key_Event_LongPress   0x00040000U


typedef struct _H_Key_Def
{
  Huint32 KeyID;//[15:0]按键ID [30:16]事件类型 Bit31 按键状态 0:抬起 1:按下
  int KeepTick;//状态已保持时间
  int ChangeTick;//延迟计时
  int LongPressTick;//长按事件触发计数

  int (*isPress)(Huint32);

  struct _H_Key_Def* Next;
}H_Key_Def;

typedef struct
{
  H_Key_Def* Root;
  int ChangeTickNum;//状态改变延迟
  int MaxKeepTick;//最大保持时间 仅为KeepTick的最大值 这是为了防止KeepTick溢出
  void (*EventCallback)(void*,Huint32);
  void* v_EventCallback;
}H_Key_Root_Def;


/**
 * @brief 初始化按键根节点
 * @param H_Key_Root 根节点(已分配内存的对象)
 * @param ChangeTickNum 状态改变延迟
 * @param MaxKeepTick 最大保持时间 仅为KeepTick的最大值 这是为了防止KeepTick溢出
 * @param EventCallback 按键事件回调
 * @param v_EventCallback 按键事件回调传入参数
 * @return 无
 */
void H_Key_Root_Init(H_Key_Root_Def* H_Key_Root,int ChangeTickNum,int MaxKeepTick
  ,void (*EventCallback)(void*,Huint32),void* v_EventCallback);

/**
 * @brief 初始化按键
 * @param H_Key 按键对象(已分配内存的对象)
 * @param KeyID 按键ID 0~65535
 * @param LongPressTick 长按事件触发时长 0代表无长按事件
 * @param isPress 获取按键状态方法 返回 0:未按下 其他:按下
 * @return 无
 */
void H_Key_Init(H_Key_Def* H_Key,Huint32 KeyID,int LongPressTick,int (*isPress)(Huint32));

/**
 * @brief 添加按键到节点
 * @param H_Key_Root 根节点对象
 * @param H_Key 初始化好的按键对象
 * @return 0:成功 其他:失败
 */
int H_Key_Add(H_Key_Root_Def* H_Key_Root,H_Key_Def* H_Key);

/**
 * @brief 设置长按事件触发时长
 * @param H_Key_Root 按键根节点
 * @param KeyID 键值
 * @param LongPressTick 触发Tick数 0表示不使用长按事件
 * @return 0:成功 -1:未找到对应的按键
 */
int H_Key_SetLongPressTick(H_Key_Root_Def* H_Key_Root,Huint32 KeyID,int LongPressTick);

/**
 * @brief 按键处理循环 随便找个地方循环调用就行 建议每隔一段时间调用一次
 * @param H_Key_Root 根节点对象
 * @return 无
 */
void H_Key_Loop(H_Key_Root_Def* H_Key_Root);




#endif //__H_Key_H_
