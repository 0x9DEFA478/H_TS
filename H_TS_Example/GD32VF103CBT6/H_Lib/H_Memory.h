/*
 * @Author: 0x9DEFA478
 * @Date: 2021-11-17 19:01:49
 * @LastEditTime: 2021-11-19 22:08:14
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
#ifndef __H_Memory_H_
#define __H_Memory_H_
#include "H_Type.h"


#define H_MemoryCopy_SizeType                                      Huint32


/**
 * @brief ????????????
 * @param Dst ????????????
 * @param Src ?????????
 * @param Length ????????????
 * @return ???
 */
void H_MemoryCopy_byte(void* Dst,void* Src,H_MemoryCopy_SizeType Length);

/**
 * @brief ????????????
 * @param Dst ????????????
 * @param Src ?????????
 * @param Length ????????????
 * @return ???
 */
void H_MemoryCopy_uint16(void* Dst,void* Src,H_MemoryCopy_SizeType Length);

/**
 * @brief ????????????
 * @param Dst ????????????
 * @param Src ?????????
 * @param Length ????????????
 * @return ???
 */
void H_MemoryCopy_uint32(void* Dst,void* Src,H_MemoryCopy_SizeType Length);

/**
 * @brief ????????????
 * @param Dst ????????????
 * @param Src ?????????
 * @param Length ????????????
 * @return ???
 */
void H_MemoryCopy_uint64(void* Dst,void* Src,H_MemoryCopy_SizeType Length);

/**
 * @brief ???????????????
 * @param p ????????????????????????
 * @param Length ?????? ??????:??????
 * @return ???
 */
void H_MemorySetZero(void* p,H_MemoryCopy_SizeType Length);




#endif //__H_Memory_H_
