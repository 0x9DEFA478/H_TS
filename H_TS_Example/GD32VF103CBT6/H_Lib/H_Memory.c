/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 19:01:37
 * @LastEditTime: 2021-11-24 17:24:03
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
#include "H_Memory.h"




/**
 * @brief ????????????
 * @param Dst ????????????
 * @param Src ?????????
 * @param Length ????????????
 * @return ???
 */
void H_MemoryCopy_byte(void* Dst,void* Src,H_MemoryCopy_SizeType Length){

  Hbyte* dst;
  Hbyte* src;

  dst=Dst;
  src=Src;

  while(Length>7){
    Length-=8;

    dst[0]=src[0];
    dst[1]=src[1];
    dst[2]=src[2];
    dst[3]=src[3];
    dst[4]=src[4];
    dst[5]=src[5];
    dst[6]=src[6];
    dst[7]=src[7];

    src=&src[8];
    dst=&dst[8];
  }
  while(Length>1){
    Length-=2;

    dst[0]=src[0];
    dst[1]=src[1];

    src=&src[2];
    dst=&dst[2];
  }
  if(Length>0){
    dst[0]=src[0];
  }

}

/**
 * @brief ????????????
 * @param Dst ????????????
 * @param Src ?????????
 * @param Length ????????????
 * @return ???
 */
void H_MemoryCopy_uint16(void* Dst,void* Src,H_MemoryCopy_SizeType Length){

  Huint16* dst;
  Huint16* src;

  dst=Dst;
  src=Src;

  while(Length>7){
    Length-=8;

    dst[0]=src[0];
    dst[1]=src[1];
    dst[2]=src[2];
    dst[3]=src[3];
    dst[4]=src[4];
    dst[5]=src[5];
    dst[6]=src[6];
    dst[7]=src[7];

    src=&src[8];
    dst=&dst[8];
  }
  while(Length>1){
    Length-=2;

    dst[0]=src[0];
    dst[1]=src[1];

    src=&src[2];
    dst=&dst[2];
  }
  if(Length>0){
    dst[0]=src[0];
  }

}

/**
 * @brief ????????????
 * @param Dst ????????????
 * @param Src ?????????
 * @param Length ????????????
 * @return ???
 */
void H_MemoryCopy_uint32(void* Dst,void* Src,H_MemoryCopy_SizeType Length){

  Huint32* dst;
  Huint32* src;

  dst=Dst;
  src=Src;

  while(Length>7){
    Length-=8;

    dst[0]=src[0];
    dst[1]=src[1];
    dst[2]=src[2];
    dst[3]=src[3];
    dst[4]=src[4];
    dst[5]=src[5];
    dst[6]=src[6];
    dst[7]=src[7];

    src=&src[8];
    dst=&dst[8];
  }
  while(Length>1){
    Length-=2;

    dst[0]=src[0];
    dst[1]=src[1];

    src=&src[2];
    dst=&dst[2];
  }
  if(Length>0){
    dst[0]=src[0];
  }

}

/**
 * @brief ????????????
 * @param Dst ????????????
 * @param Src ?????????
 * @param Length ????????????
 * @return ???
 */
void H_MemoryCopy_uint64(void* Dst,void* Src,H_MemoryCopy_SizeType Length){

  Huint64* dst;
  Huint64* src;

  dst=Dst;
  src=Src;

  while(Length>7){
    Length-=8;

    dst[0]=src[0];
    dst[1]=src[1];
    dst[2]=src[2];
    dst[3]=src[3];
    dst[4]=src[4];
    dst[5]=src[5];
    dst[6]=src[6];
    dst[7]=src[7];

    src=&src[8];
    dst=&dst[8];
  }
  while(Length>1){
    Length-=2;

    dst[0]=src[0];
    dst[1]=src[1];

    src=&src[2];
    dst=&dst[2];
  }
  if(Length>0){
    dst[0]=src[0];
  }

}

/**
 * @brief ???????????????
 * @param p ????????????????????????
 * @param Length ?????? ??????:??????
 * @return ???
 */
void H_MemorySetZero(void* p,H_MemoryCopy_SizeType Length){

  Hbyte* dst;

  dst=p;

  while(Length>7){
    Length-=8;

    dst[0]=0;
    dst[1]=0;
    dst[2]=0;
    dst[3]=0;
    dst[4]=0;
    dst[5]=0;
    dst[6]=0;
    dst[7]=0;

    dst=&dst[8];
  }
  while(Length>1){
    Length-=2;

    dst[0]=0;
    dst[1]=0;

    dst=&dst[2];
  }
  if(Length>0){
    dst[0]=0;
  }

}




