/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-14 19:34:06
 * @LastEditTime: 2021-12-07 21:36:06
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
#include "H_FIFO.h"


#define H_FIFO_uint64_Type              Huint64


#if   H_FIFO_SizeOfSizeType == 2
#define cH_FIFO_uint64_GetPacketSize(Size,Buffer)                 do{\
                                                                    ((Huint16*)&(Size))[0]=((Huint16*)(Buffer))[0];\
                                                                  }while(0)
#define cH_FIFO_uint64_SetPacketSize(Buffer,Size)                 do{\
                                                                    ((Huint16*)(Buffer))[0]=((Huint16*)&(Size))[0];\
                                                                  }while(0)
#elif H_FIFO_SizeOfSizeType == 4
#define cH_FIFO_uint64_GetPacketSize(Size,Buffer)                 do{\
                                                                    ((Huint32*)&(Size))[0]=((Huint32*)(Buffer))[0];\
                                                                  }while(0)
#define cH_FIFO_uint64_SetPacketSize(Buffer,Size)                 do{\
                                                                    ((Huint32*)(Buffer))[0]=((Huint32*)&(Size))[0];\
                                                                  }while(0)
#elif H_FIFO_SizeOfSizeType == 8
#define cH_FIFO_uint64_GetPacketSize(Size,Buffer)                 do{\
                                                                    ((H_FIFO_uint64_Type*)&(Size))[0]=(Buffer)[0];\
                                                                  }while(0)
#define cH_FIFO_uint64_SetPacketSize(Buffer,Size)                 do{\
                                                                    (Buffer)[0]=((H_FIFO_uint64_Type*)&(Size))[0];\
                                                                  }while(0)
#else
#error "????????????????????????"
#endif



int H_FIFO_uint64_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length){
  volatile H_FIFO_uint64_Type* Buffer;
  H_FIFO_SizeType InId;
  H_FIFO_SizeType OutId;
  H_FIFO_SizeType BufferSize;
  H_FIFO_SizeType SizeSize;
  H_FIFO_SizeType Empty;
  H_FIFO_uint64_Type* src;
  H_FIFO_SizeType i;


  InId=fifo->FIFO.InId;
  OutId=fifo->FIFO.OutId;
  BufferSize=fifo->FIFO.BufferSize;
  SizeSize=fifo->FIFO.SizeSize;

  Length+=SizeSize;
  
  if(InId<OutId){
    Empty=OutId-InId;
  }else{
    Empty=BufferSize-InId;
    if(Empty<OutId){
      Empty=OutId;
    }
  }

  if(Empty<=Length){//??????????????? ??????????????????????????? I O ??????0 ????????????????????????FIFO??????
    return -1;
  }

  //??????????????? ????????????????????? ?????????0???????????????fifo->Buffer_InId??????

  //????????????
  //?????????0???????????????fifo->Buffer_InId??????
  i=BufferSize-InId;
  if(i<Length){
    //???0??????

    if(i>=SizeSize){
      //??????
      Buffer=fifo->FIFO.Buffer;
      Buffer=&Buffer[InId];
      i=0;
      cH_FIFO_uint64_SetPacketSize(Buffer,i);
    }

    Buffer=fifo->FIFO.Buffer;
    cH_FIFO_uint64_SetPacketSize(Buffer,Length);
    Buffer=&Buffer[SizeSize];

    InId=0;
    
  }else{
    //???I??????

    Buffer=fifo->FIFO.Buffer;
    Buffer=&Buffer[InId];
    cH_FIFO_uint64_SetPacketSize(Buffer,Length);
    Buffer=&Buffer[SizeSize];

  }

  src=Data;
  i=Length-SizeSize;
  while(i>7){
    i-=8;

    Buffer[0]=src[0];
    Buffer[1]=src[1];
    Buffer[2]=src[2];
    Buffer[3]=src[3];
    Buffer[4]=src[4];
    Buffer[5]=src[5];
    Buffer[6]=src[6];
    Buffer[7]=src[7];

    Buffer=&Buffer[8];
    src=&src[8];
  }
  while(i>1){
    i-=2;

    Buffer[0]=src[0];
    Buffer[1]=src[1];

    Buffer=&Buffer[2];
    src=&src[2];
  }
  if(i>0){
    Buffer[0]=src[0];
  }

  if((InId+Length)==BufferSize){
    fifo->FIFO.InId=0;
  }else{
    fifo->FIFO.InId=InId+Length;
  }

  return 0;
}

int H_FIFO_uint64_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length){
  volatile H_FIFO_uint64_Type* Buffer;
  H_FIFO_SizeType InId;
  H_FIFO_SizeType OutId;
  H_FIFO_SizeType BufferSize;
  H_FIFO_SizeType SizeSize;
  H_FIFO_SizeType Empty;
  H_FIFO_uint64_Type* src;
  H_FIFO_SizeType i;


  InId=fifo->FIFO.InId;
  OutId=fifo->FIFO.OutId;
  BufferSize=fifo->FIFO.BufferSize;
  SizeSize=fifo->FIFO.SizeSize;

  Length+=SizeSize+1;
  
  if(InId<OutId){
    Empty=OutId-InId;
  }else{
    Empty=BufferSize-InId;
    if(Empty<OutId){
      Empty=OutId;
    }
  }

  if(Empty<=Length){//??????????????? ??????????????????????????? I O ??????0 ????????????????????????FIFO??????
    return -1;
  }

  //??????????????? ????????????????????? ?????????0???????????????fifo->Buffer_InId??????

  //????????????
  //?????????0???????????????fifo->Buffer_InId??????
  i=BufferSize-InId;
  if(i<Length){
    //???0??????

    if(i>=SizeSize){
      //??????
      Buffer=fifo->FIFO.Buffer;
      Buffer=&Buffer[InId];
      i=0;
      cH_FIFO_uint64_SetPacketSize(Buffer,i);
    }

    Buffer=fifo->FIFO.Buffer;
    cH_FIFO_uint64_SetPacketSize(Buffer,Length);
    Buffer=&Buffer[SizeSize];
    Buffer[0]=((H_FIFO_uint64_Type*)Data0)[0];
    Buffer=&Buffer[1];

    InId=0;
    
  }else{
    //???I??????

    Buffer=fifo->FIFO.Buffer;
    Buffer=&Buffer[InId];
    cH_FIFO_uint64_SetPacketSize(Buffer,Length);
    Buffer=&Buffer[SizeSize];
    Buffer[0]=((H_FIFO_uint64_Type*)Data0)[0];
    Buffer=&Buffer[1];

  }

  src=Data;
  i=Length-SizeSize-1;
  while(i>7){
    i-=8;

    Buffer[0]=src[0];
    Buffer[1]=src[1];
    Buffer[2]=src[2];
    Buffer[3]=src[3];
    Buffer[4]=src[4];
    Buffer[5]=src[5];
    Buffer[6]=src[6];
    Buffer[7]=src[7];

    Buffer=&Buffer[8];
    src=&src[8];
  }
  while(i>1){
    i-=2;

    Buffer[0]=src[0];
    Buffer[1]=src[1];

    Buffer=&Buffer[2];
    src=&src[2];
  }
  if(i>0){
    Buffer[0]=src[0];
  }

  if((InId+Length)==BufferSize){
    fifo->FIFO.InId=0;
  }else{
    fifo->FIFO.InId=InId+Length;
  }

  return 0;
}

void* H_FIFO_uint64_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr){
  H_FIFO_uint64_Type* Buffer;
  H_FIFO_SizeType OutId;
  H_FIFO_SizeType BufferSize;
  H_FIFO_SizeType SizeSize;
  H_FIFO_SizeType Size;


  OutId=fifo->FIFO.OutId;
  BufferSize=fifo->FIFO.BufferSize;
  SizeSize=fifo->FIFO.SizeSize;

  if(oH_FIFO_isEmpty(fifo)){

    if((BufferSize-OutId)<SizeSize){
      OutId=0;
      fifo->FIFO.OutId=0;

      Buffer=&((H_FIFO_uint64_Type*)fifo->FIFO.Buffer)[OutId];
      cH_FIFO_uint64_GetPacketSize(Size,Buffer);
    }else{
      Buffer=&((H_FIFO_uint64_Type*)fifo->FIFO.Buffer)[OutId];
      cH_FIFO_uint64_GetPacketSize(Size,Buffer);

      if(Size<=0){//??????????????????????????????
        OutId=0;
        fifo->FIFO.OutId=0;

        Buffer=&((H_FIFO_uint64_Type*)fifo->FIFO.Buffer)[OutId];
        cH_FIFO_uint64_GetPacketSize(Size,Buffer);
      }
    }

    LengthPtr[0]=Size-SizeSize;
    return &((H_FIFO_uint64_Type*)fifo->FIFO.Buffer)[OutId+SizeSize];
  }
  else{
    LengthPtr[0]=0;
    return NULL;
  }
}

void H_FIFO_uint64_Next(H_FIFO* fifo){
  H_FIFO_uint64_Type* Buffer;
  H_FIFO_SizeType OutId;
  H_FIFO_SizeType BufferSize;
  H_FIFO_SizeType SizeSize;
  H_FIFO_SizeType Size;

  
  OutId=fifo->FIFO.OutId;
  BufferSize=fifo->FIFO.BufferSize;
  SizeSize=fifo->FIFO.SizeSize;

  if((BufferSize-OutId)<SizeSize){
    OutId=0;

    Buffer=&((H_FIFO_uint64_Type*)fifo->FIFO.Buffer)[OutId];
    cH_FIFO_uint64_GetPacketSize(Size,Buffer);
  }else{
    Buffer=&((H_FIFO_uint64_Type*)fifo->FIFO.Buffer)[OutId];
    cH_FIFO_uint64_GetPacketSize(Size,Buffer);

    if(Size<=0){//??????????????????????????????
      OutId=0;

      Buffer=&((H_FIFO_uint64_Type*)fifo->FIFO.Buffer)[OutId];
      cH_FIFO_uint64_GetPacketSize(Size,Buffer);
    }
  }

  OutId+=Size;
  if(OutId==BufferSize){
    fifo->FIFO.OutId=0;
  }else{
    fifo->FIFO.OutId=OutId;
  }

}



