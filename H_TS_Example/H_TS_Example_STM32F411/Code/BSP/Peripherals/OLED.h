/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-06 18:01:25
 * @LastEditTime: 2021-11-10 00:35:56
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
#ifndef __OLED_H_
#define __OLED_H_
#include "H_Type.h"

//???????????? SSD1306
#define vOLED_Type_SSD1306   1

//???????????? CH1115
#define vOLED_Type_CH1115    2



//???????????? 0???
#define vOLED_Direction_0    0

//???????????? 180???
#define vOLED_Direction_180  180

//??????????????????CmdSequenceBuffer?????????
#define vOLED_CmdSequenceBufferLength   6


typedef struct{
  void (*Lock)(void);//???????????? ?????????????????????????????????
  void (*UnLock)(void);//???????????? ?????????????????????????????????
  void (*Reset)(void);//????????????
  void (*SendSequence)(void*,int,void*,int,int);//???????????? (????????????,??????????????????,????????????,?????????????????? ????????????(0:?????? ??????:??????))
}OLED_LL_Function;


typedef struct _OLED
{
  //????????????
  int Type;

  //????????????
  int Direction;

  //???????????????
  volatile int IsDisplay;

  int bytesNumOfPage;
  int NumOfPage;

  void* CmdSequenceBuffer;//????????????

  void (*Init)(struct _OLED*);
  void (*SetDirection)(struct _OLED*,int);
  void (*SetContrast)(struct _OLED*,int);
  void (*SetIsDisplay)(struct _OLED*,int);
  void (*Display)(struct _OLED*,void*);

  //??????????????????
  OLED_LL_Function LL_Function;


}OLED;





/**
 * @brief ??????????????????
 * @param _this ??????????????????
 * @param Direction ???????????? vOLED_Direction_0 vOLED_Direction_180 ??????
 * @return ???
 */
void OLED_SetDirection(OLED* _this,int Direction);

/**
 * @brief ????????????
 * @param _this ??????????????????
 * @param Contrast 0~100????????????
 * @return ???
 */
void OLED_SetContrast(OLED* _this,int Contrast);

/**
 * @brief ?????????????????????
 * @param _this ??????????????????
 * @param IsDisplay 0:????????? ??????:?????????
 * @return ???
 */
void OLED_SetIsDisplay(OLED* _this,int IsDisplay);

/**
 * @brief ??????????????????
 * @param _this ??????????????????
 * @param Bitmap ??????????????????
 * @return ???
 */
void OLED_Display(OLED* _this,void* Bitmap);

/**
 * @brief ??????????????????
 * @param _this ??????????????????
 * @return ???
 */
void OLED_ScreenInit(OLED* _this);

/**
 * @brief ?????????????????? ?????????????????????
 * @param _this ??????????????????
 * @return ???
 */
void OLED_ScreenDeInit(OLED* _this);

/**
 * @brief ??????_this??????????????????????????? ???????????????????????????????????????
 * @param _this ????????????????????????
 * @param LL_Function ??????????????????
 * @param CmdSequenceBuffer ?????????????????? 6????????????(????????????)
 * @param bytesNumOfPage ?????????????????????
 * @param NumOfPage ???????????????
 * @param Type OLED??????
 * @return 0:?????? -1:????????????
 */
int OLED_Init(OLED* _this,OLED_LL_Function* LL_Function,void* CmdSequenceBuffer,int bytesNumOfPage,int NumOfPage,int Type);







#define vOLED_SSD1306_InitSequence_CMD_Length   22
#define vOLED_CH1115_InitSequence_CMD_Length    28

#endif //__OLED_H_
