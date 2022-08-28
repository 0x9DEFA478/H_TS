/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-03 14:30:17
 * @LastEditTime: 2021-11-28 21:01:12
 * @LastEditors: 0x9DEFA478
 * @Description: 定义了一些链表方法 由H_TS内部调用
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
#include "./H_ThreadScheduler_Core.h"




extern H_TS* H_TS_Core;




void* H_TS_Semaphore_Create(Hsize MaxCount){
  struct _H_TS_Semaphore* r;

  r=H_TS_Core->Memory.Malloc(sizeof(struct _H_TS_Semaphore));
  if(r==NULL){
    return NULL;
  }

  r->I=0;
  r->O=0;
  r->Max=MaxCount;

  return r;
}

void H_TS_Semaphore_Delete(void* Semaphore){

  H_TS_Core->Memory.Free(Semaphore);
}




void* H_TS_MessageFIFO_Create(Hsize NumOfMessages){
  struct _H_TS_MessageFIFO* r;

  r=H_TS_Core->Memory.Malloc(sizeof(struct _H_TS_MessageFIFO));
  if(r==NULL){
    return NULL;
  }

  r->Messages=H_TS_Core->Memory.Malloc(NumOfMessages*2*sizeof(void*));
  if(r->Messages==NULL){
    H_TS_Core->Memory.Free(r);
    return NULL;
  }
  
  r->I=0;
  r->O=0;
  r->NumOfMessages=NumOfMessages;

  return r;
}

void H_TS_MessageFIFO_Delete(void* MessageFIFO){
  H_TS_Core->Memory.Free((void*)(((struct _H_TS_MessageFIFO*)MessageFIFO)->Messages));
  H_TS_Core->Memory.Free(MessageFIFO);
}




void* H_TS_Mailbox_Create(Hsize MaxNumOfMail,Hsize MailSize){
  struct _H_TS_Mailbox* r;

  r=H_TS_Core->Memory.Malloc(sizeof(struct _H_TS_Mailbox));
  if(r==NULL){
    return NULL;
  }

  r->Mails=H_TS_Core->Memory.Malloc(MailSize*MaxNumOfMail);
  if(r->Mails==NULL){
    H_TS_Core->Memory.Free(r);
    return NULL;
  }
  
  r->I=0;
  r->O=0;
  r->MailSize=MailSize;
  r->Max=MaxNumOfMail;

  return r;
}

void H_TS_Mailbox_Delete(void* Mailbox){
  H_TS_Core->Memory.Free((void*)(((struct _H_TS_Mailbox*)Mailbox)->Mails));
  H_TS_Core->Memory.Free(Mailbox);
}



