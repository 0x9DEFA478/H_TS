/*
 * @Author: 0x9DEFA478
 * @Date: 2021-08-14 17:19:35
 * @LastEditTime: 2021-12-17 16:02:48
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



extern int H_FIFO_byte_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length);
extern int H_FIFO_byte_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length);
extern void* H_FIFO_byte_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr);
extern void H_FIFO_byte_Next(H_FIFO* fifo);

extern int H_FIFO_uint16_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length);
extern int H_FIFO_uint16_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length);
extern void* H_FIFO_uint16_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr);
extern void H_FIFO_uint16_Next(H_FIFO* fifo);

extern int H_FIFO_uint32_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length);
extern int H_FIFO_uint32_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length);
extern void* H_FIFO_uint32_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr);
extern void H_FIFO_uint32_Next(H_FIFO* fifo);

extern int H_FIFO_uint64_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length);
extern int H_FIFO_uint64_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length);
extern void* H_FIFO_uint64_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr);
extern void H_FIFO_uint64_Next(H_FIFO* fifo);



/**
 * @brief ?????????FIFO
 * @param fifo ???????????????FIFO??????
 * @param Type FIFO??????/????????????????????? vH_FIFO_Type_byte vH_FIFO_Type_uint16 vH_FIFO_Type_uint32 vH_FIFO_Type_uint64
 * @param Buffer ??????????????????????????????????????????
 * @param BufferSize ????????????, ?????????Type???????????????????????????
 * @return ???
 */
void H_FIFO_Init(H_FIFO* fifo,H_FIFO_SizeType Type,void* Buffer,H_FIFO_SizeType BufferSize){
  
  fifo->FIFO.InId=0;
	fifo->FIFO.OutId=0;

	fifo->FIFO.Buffer=Buffer;
	fifo->FIFO.BufferSize=BufferSize;

  switch(Type){
    case vH_FIFO_Type_byte:
    
#if   H_FIFO_SizeOfSizeType == 2
      fifo->FIFO.SizeSize=2;
#elif H_FIFO_SizeOfSizeType == 4
      fifo->FIFO.SizeSize=4;
#elif H_FIFO_SizeOfSizeType == 8
      fifo->FIFO.SizeSize=8;
#else
#error "????????????????????????"
#endif

      fifo->Write=H_FIFO_byte_Write;
      fifo->Write_Data0=H_FIFO_byte_Write_Data0;
      fifo->Read=H_FIFO_byte_Read;
      fifo->Next=H_FIFO_byte_Next;
      break;
    case vH_FIFO_Type_uint16:

#if   H_FIFO_SizeOfSizeType == 2
      fifo->FIFO.SizeSize=1;
#elif H_FIFO_SizeOfSizeType == 4
      fifo->FIFO.SizeSize=2;
#elif H_FIFO_SizeOfSizeType == 8
      fifo->FIFO.SizeSize=4;
#else
#error "????????????????????????"
#endif

      fifo->Write=H_FIFO_uint16_Write;
      fifo->Write_Data0=H_FIFO_uint16_Write_Data0;
      fifo->Read=H_FIFO_uint16_Read;
      fifo->Next=H_FIFO_uint16_Next;
      break;
    case vH_FIFO_Type_uint32:

#if   H_FIFO_SizeOfSizeType == 2
      fifo->FIFO.SizeSize=1;
#elif H_FIFO_SizeOfSizeType == 4
      fifo->FIFO.SizeSize=1;
#elif H_FIFO_SizeOfSizeType == 8
      fifo->FIFO.SizeSize=2;
#else
#error "????????????????????????"
#endif

      fifo->Write=H_FIFO_uint32_Write;
      fifo->Write_Data0=H_FIFO_uint32_Write_Data0;
      fifo->Read=H_FIFO_uint32_Read;
      fifo->Next=H_FIFO_uint32_Next;
      break;
    case vH_FIFO_Type_uint64:

#if   H_FIFO_SizeOfSizeType == 2
      fifo->FIFO.SizeSize=1;
#elif H_FIFO_SizeOfSizeType == 4
      fifo->FIFO.SizeSize=1;
#elif H_FIFO_SizeOfSizeType == 8
      fifo->FIFO.SizeSize=1;
#else
#error "????????????????????????"
#endif

      fifo->Write=H_FIFO_uint64_Write;
      fifo->Write_Data0=H_FIFO_uint64_Write_Data0;
      fifo->Read=H_FIFO_uint64_Read;
      fifo->Next=H_FIFO_uint64_Next;
      break;
    default:
      for(;;){
        //??????????????????
      }
  }
}

/**
 * @brief FIFO???????????????
 * @param fifo FIFO??????
 * @return 0:FIFO??? ??????:FIFO?????????
 */
int H_FIFO_isEmpty(H_FIFO* fifo){
  if(oH_FIFO_isEmpty(fifo)){
    return -1;
  }

  return 0;
}

/**
 * @brief ??????FIFO??????????????????????????????
 * @param fifo FIFO??????
 * @return ????????????????????????????????????(???fifo????????????????????????????????????)
 */
H_FIFO_SizeType H_FIFO_GetEmptySize(H_FIFO* fifo){
  H_FIFO_SizeType InId;
  H_FIFO_SizeType OutId;
  H_FIFO_SizeType BufferSize;
  H_FIFO_SizeType EmptySize;


  InId=fifo->FIFO.InId;
  OutId=fifo->FIFO.OutId;
  BufferSize=fifo->FIFO.BufferSize;

  //????????????
  if(InId<OutId){
    EmptySize=OutId-InId;
  }else{
    EmptySize=BufferSize-InId;
    if(EmptySize<OutId){
      EmptySize=OutId;
    }
  }

  EmptySize-=fifo->FIFO.SizeSize;//?????????????????????????????????
  if(EmptySize<0){
    EmptySize=0;
  }

  return EmptySize;
}

/**
 * @brief ???FIFO????????????
 * @param fifo FIFO??????
 * @param Data ??????????????????
 * @param Length ??????(???fifo????????????????????????????????????)
 * @return 0:?????? ??????:?????? ???????????????????????????
 */
int H_FIFO_Write(H_FIFO* fifo,void* Data,H_FIFO_SizeType Length){
  return fifo->Write(fifo,Data,Length);
}

/**
 * @brief ???FIFO????????????(????????????????????????)
 * @param fifo FIFO??????
 * @param Data0 ?????????????????????(?????????fifo???????????????????????????)
 * @param Data ??????????????????
 * @param Length ??????(???fifo????????????????????????????????????)
 * @return 0:?????? ??????:?????? ???????????????????????????
 */
int H_FIFO_Write_Data0(H_FIFO* fifo,void* Data0,void* Data,H_FIFO_SizeType Length){
  return fifo->Write_Data0(fifo,Data0,Data,Length);
}

/**
 * @brief ???????????????????????? ?????????????????? ??????H_FIFO_Next()??????????????????????????????
 * @param fifo FIFO??????
 * @param LengthPtr ???????????????????????????
 * @return ????????????????????? ?????????NULL((void*)0)?????????????????????
 */
void* H_FIFO_Read(H_FIFO* fifo,H_FIFO_SizeType* LengthPtr){
  return fifo->Read(fifo,LengthPtr);
}

/**
 * @brief ???????????????????????????????????? (?????????FIFO?????????????????????, ???H_FIFO_Read()???????????????????????????)
 * @param fifo FIFO??????
 * @return ???
 */
void H_FIFO_Next(H_FIFO* fifo){
  return fifo->Next(fifo);
}



