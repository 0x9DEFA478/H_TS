/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-20 00:02:54
 * @LastEditTime: 2021-12-05 22:06:49
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
#include "I_DebugPort.h"
#include "Peripherals_Instence.h"
#include "usart.h"




H_TS_Lock* I_DebugPort_Write_Lock;
H_TS_Lock* I_DebugPort_Read_Lock;
static volatile int I_DebugPort_IsValid;


//阻塞写入数据
void I_DebugPort_Write(void* Buffer,int Length){
  int r;

  if(I_DebugPort_IsValid==0){
    return;
  }

  H_TS_LockLock(I_DebugPort_Write_Lock);
  do{
    r=H_Stream_Wirte(usart2_Handle->Stream,Buffer,Length,16);
  }while((r!=0)&&(I_DebugPort_IsValid!=0));
  H_TS_LockUnlock(I_DebugPort_Write_Lock);

}

//阻塞读取数据 返回-1代表失败
int I_DebugPort_Read(void* Buffer,int MaxLength){
  int r;

  if(I_DebugPort_IsValid==0){
    return -1;
  }

  H_TS_LockLock(I_DebugPort_Read_Lock);
  do{
    r=H_Stream_Read(usart2_Handle->Stream,Buffer,MaxLength,16);
  }while((r<0)&&(I_DebugPort_IsValid!=0));
  H_TS_LockUnlock(I_DebugPort_Read_Lock);

  return r;
}

//设置调试端口是否有效
void I_DebugPort_SetValid(int IsValid){

  I_DebugPort_IsValid=IsValid;

  H_TS_LockLock(I_DebugPort_Read_Lock);
  H_TS_LockLock(I_DebugPort_Write_Lock);
  H_TS_LockUnlock(I_DebugPort_Write_Lock);
  H_TS_LockUnlock(I_DebugPort_Read_Lock);
}

//进行调试端口初始化
void I_DebugPort_Init(){

  I_DebugPort_IsValid=0;

  I_DebugPort_Write_Lock=new_H_TS_Lock();
  I_DebugPort_Read_Lock=new_H_TS_Lock();

  if(cH_IsNull(I_DebugPort_Write_Lock)||cH_IsNull(I_DebugPort_Read_Lock)){
    for(;;){
    }
  }
}


