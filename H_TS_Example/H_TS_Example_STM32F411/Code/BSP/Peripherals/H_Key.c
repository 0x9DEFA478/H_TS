/*
 * @Author: 0x9DEFA478
 * @Date: 2021-04-29 00:46:10
 * @LastEditTime: 2021-04-29 00:50:31
 * @LastEditors: 0x9DEFA478
 * @Description: 按键抽象的一些方法 此文件无实例
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
#include "H_Key.h"



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
  ,void (*EventCallback)(void*,Huint32),void* v_EventCallback){
  H_Key_Root->Root=NULL;
  H_Key_Root->ChangeTickNum=ChangeTickNum;
  H_Key_Root->MaxKeepTick=MaxKeepTick;
  H_Key_Root->EventCallback=EventCallback;
  H_Key_Root->v_EventCallback=v_EventCallback;
}

/**
 * @brief 初始化按键
 * @param H_Key 按键对象(已分配内存的对象)
 * @param KeyID 按键ID 0~65535
 * @param LongPressTick 长按事件触发时长 0代表无长按事件
 * @param isPress 获取按键状态方法 返回 0:未按下 其他:按下
 * @return 无
 */
void H_Key_Init(H_Key_Def* H_Key,Huint32 KeyID,int LongPressTick,int (*isPress)(Huint32)){
  H_Key->ChangeTick=0;
  H_Key->isPress=isPress;
  H_Key->KeepTick=0;
  H_Key->KeyID=KeyID;
  H_Key->LongPressTick=LongPressTick;
}

/**
 * @brief 添加按键到节点
 * @param H_Key_Root 根节点对象
 * @param H_Key 初始化好的按键对象
 * @return 0:成功 其他:失败
 */
int H_Key_Add(H_Key_Root_Def* H_Key_Root,H_Key_Def* H_Key){
  H_Key_Def* p;

  if(H_Key->KeyID&(vH_Key_Event_isPress|vH_Key_Event_Type)){
    return -1;
  }

  H_Key->KeyID&=vH_Key_Event_KeyID;

  if(H_Key_Root->Root==NULL){
    H_Key->Next=NULL;
    H_Key_Root->Root=H_Key;
  }else{
    p=H_Key_Root->Root;

    if((p->KeyID&vH_Key_Event_KeyID)==H_Key->KeyID){
      return -2;
    }

    while(p->Next!=NULL){
      p=p->Next;
      if((p->KeyID&vH_Key_Event_KeyID)==H_Key->KeyID){
        return -2;
      }
    }

    H_Key->Next=NULL;
    p->Next=H_Key;

  }
  
  return 0;
}

/**
 * @brief 设置长按事件触发时长
 * @param H_Key_Root 按键根节点
 * @param KeyID 键值
 * @param LongPressTick 触发Tick数 0表示不使用长按事件
 * @return 0:成功 -1:未找到对应的按键
 */
int H_Key_SetLongPressTick(H_Key_Root_Def* H_Key_Root,Huint32 KeyID,int LongPressTick){

  H_Key_Def* p;

  p=H_Key_Root->Root;

  while (p!=NULL)
  {
    if((p->KeyID&vH_Key_Event_KeyID)==KeyID){
      p->LongPressTick=LongPressTick;
      return 0;
    }
    p=p->Next;
  }
  
  return -1;
}

/**
 * @brief 按键处理循环 随便找个地方循环调用就行 建议每隔一段时间调用一次
 * @param H_Key_Root 根节点对象
 * @return 无
 */
void H_Key_Loop(H_Key_Root_Def* H_Key_Root){
  H_Key_Def* p;
  int Status;


  if (H_Key_Root->Root==NULL)
  {
    return;
  }

  p=H_Key_Root->Root;
  
  while(p!=NULL)
  {
    Status=p->isPress(p->KeyID&vH_Key_Event_KeyID);

    if(p->KeyID&vH_Key_Event_isPress){
      if(Status){
        //都为按下

        p->ChangeTick=0;
        if(p->LongPressTick>0){
          if(p->KeepTick<H_Key_Root->MaxKeepTick){
            p->KeepTick++;
          }
          if(p->KeepTick==p->LongPressTick){
            H_Key_Root->EventCallback(H_Key_Root->v_EventCallback,p->KeyID|vH_Key_Event_LongPress);//长按事件
          }
        }

      }else{
        //新状态 抬起

        p->ChangeTick++;
        if(p->ChangeTick>H_Key_Root->ChangeTickNum){
          p->KeyID&=(~vH_Key_Event_isPress);
          p->ChangeTick=0;
          H_Key_Root->EventCallback(H_Key_Root->v_EventCallback,p->KeyID|vH_Key_Event_Up);//抬起事件
        }
      }
    }else{
      if(Status){
        //新状态 按下

        p->KeepTick=0;

        p->ChangeTick++;
        if(p->ChangeTick>H_Key_Root->ChangeTickNum){
          p->KeyID|=vH_Key_Event_isPress;
          p->ChangeTick=0;
          H_Key_Root->EventCallback(H_Key_Root->v_EventCallback,p->KeyID|vH_Key_Event_Press);//按下事件
        }
      }else{
        //都为抬起
        p->ChangeTick=0;
      }
    }

    p=p->Next;
  }
}

