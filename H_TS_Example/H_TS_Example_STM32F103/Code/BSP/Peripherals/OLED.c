/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-06 18:01:17
 * @LastEditTime: 2021-09-21 17:43:59
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
#include "OLED.h"





extern const unsigned char OLED_SSD1306_InitSequence_CMD[];
extern const unsigned char OLED_CH1115_InitSequence_CMD[];

static void ssd1306_Init(OLED* _this){
  _this->LL_Function.Reset();
  _this->LL_Function.SendSequence((void*)OLED_SSD1306_InitSequence_CMD,vOLED_SSD1306_InitSequence_CMD_Length,NULL,0,-1);
}

static void ch1115_Init(OLED* _this){
  _this->LL_Function.Reset();
  _this->LL_Function.SendSequence((void*)OLED_CH1115_InitSequence_CMD,vOLED_CH1115_InitSequence_CMD_Length,NULL,0,-1);
}


static void ssd1306_ch1115_SetDirection(OLED* _this,int Direction){

  if(_this->Direction==Direction){
    return;
  }

  _this->Direction=Direction;

  switch(Direction)
  {
    case vOLED_Direction_0:
      ((volatile Hbyte*)_this->CmdSequenceBuffer)[0]=0xA0U;
	    ((volatile Hbyte*)_this->CmdSequenceBuffer)[1]=0xC0U;
      break;
    case vOLED_Direction_180:
      ((volatile Hbyte*)_this->CmdSequenceBuffer)[0]=0xA1U;
	    ((volatile Hbyte*)_this->CmdSequenceBuffer)[1]=0xC8U;
    break;
  
  default:
    return;
  }

  _this->LL_Function.SendSequence(_this->CmdSequenceBuffer,2,NULL,0,-1);
}

static void ssd1306_ch1115_SetContrast(OLED* _this,int Contrast){
  Hbyte c;

  c=(Contrast*255)/100;

  ((volatile Hbyte*)_this->CmdSequenceBuffer)[0]=0x81U;
  ((volatile Hbyte*)_this->CmdSequenceBuffer)[1]=c;
  _this->LL_Function.SendSequence(_this->CmdSequenceBuffer,2,NULL,0,-1);
}

static void ssd1306_ch1115_SetIsDisplay(OLED* _this,int IsDisplay){

  if(IsDisplay==0){
    ((volatile Hbyte*)_this->CmdSequenceBuffer)[0]=0xAEU;
  }else{
    ((volatile Hbyte*)_this->CmdSequenceBuffer)[0]=0xAFU;
  }

  _this->LL_Function.SendSequence(_this->CmdSequenceBuffer,1,NULL,0,-1);

}


static void ssd1306_Display(OLED* _this,void* Bitmap){
  
  ((volatile Hbyte*)_this->CmdSequenceBuffer)[0]=0x21U;
	((volatile Hbyte*)_this->CmdSequenceBuffer)[1]=0;
	((volatile Hbyte*)_this->CmdSequenceBuffer)[2]=_this->bytesNumOfPage-1;
	((volatile Hbyte*)_this->CmdSequenceBuffer)[3]=0x22U;
	((volatile Hbyte*)_this->CmdSequenceBuffer)[4]=0;
	((volatile Hbyte*)_this->CmdSequenceBuffer)[5]=_this->NumOfPage-1;
  
  _this->LL_Function.SendSequence(_this->CmdSequenceBuffer,6,Bitmap,_this->NumOfPage*_this->bytesNumOfPage,0);

}

static void ch1115_Display(OLED* _this,void* Bitmap){
  int NumOfPage;
  int i;
  Hbyte* p;

  p=Bitmap;
  NumOfPage=_this->NumOfPage;

  for(i=0;i<NumOfPage;i++)
  {
    ((volatile Hbyte*)_this->CmdSequenceBuffer)[0]=0x00U;
    ((volatile Hbyte*)_this->CmdSequenceBuffer)[1]=0x10U;
    ((volatile Hbyte*)_this->CmdSequenceBuffer)[2]=0xB0U+(i&0x7U);

    _this->LL_Function.SendSequence(_this->CmdSequenceBuffer,3,p,_this->bytesNumOfPage,0);

    p=&p[_this->bytesNumOfPage];
  }
}





/**
 * @brief ??????????????????
 * @param _this ??????????????????
 * @param Direction ???????????? vOLED_Direction_0 vOLED_Direction_180 ??????
 * @return ???
 */
void OLED_SetDirection(OLED* _this,int Direction){

  _this->SetDirection(_this,Direction);
}

/**
 * @brief ????????????
 * @param _this ??????????????????
 * @param Contrast 0~100????????????
 * @return ???
 */
void OLED_SetContrast(OLED* _this,int Contrast){

  _this->SetContrast(_this,Contrast);
}

/**
 * @brief ?????????????????????
 * @param _this ??????????????????
 * @param IsDisplay 0:????????? ??????:?????????
 * @return ???
 */
void OLED_SetIsDisplay(OLED* _this,int IsDisplay){
  _this->IsDisplay=IsDisplay;
  _this->SetIsDisplay(_this,IsDisplay);
}

/**
 * @brief ??????????????????
 * @param _this ??????????????????
 * @param Bitmap ??????????????????
 * @return ???
 */
void OLED_Display(OLED* _this,void* Bitmap){
  if(_this->IsDisplay!=0){
    _this->Display(_this,Bitmap);
  }
}

/**
 * @brief ?????????OLED
 * @param _this ????????????????????????
 * @param LL_Function ??????????????????
 * @param CmdSequenceBuffer ?????????????????? vOLED_CmdSequenceBufferLength????????????(????????????)
 * @param bytesNumOfPage ?????????????????????
 * @param NumOfPage ???????????????
 * @param Type OLED??????
 * @return 0:?????? -1:????????????
 */
int OLED_Init(OLED* _this,OLED_LL_Function* LL_Function,void* CmdSequenceBuffer,int bytesNumOfPage,int NumOfPage,int Type){

  _this->Direction=vOLED_Direction_0;
  _this->IsDisplay=-1;
  _this->CmdSequenceBuffer=CmdSequenceBuffer;
  _this->bytesNumOfPage=bytesNumOfPage;
  _this->NumOfPage=NumOfPage;
  _this->Type=Type;
  _this->LL_Function=*LL_Function;

  switch (Type)
  {
    case vOLED_Type_SSD1306:
      _this->Display=ssd1306_Display;
      _this->SetContrast=ssd1306_ch1115_SetContrast;
      _this->SetDirection=ssd1306_ch1115_SetDirection;
      _this->SetIsDisplay=ssd1306_ch1115_SetIsDisplay;
      ssd1306_Init(_this);
      break;
    case vOLED_Type_CH1115:
      _this->Display=ch1115_Display;
      _this->SetContrast=ssd1306_ch1115_SetContrast;
      _this->SetDirection=ssd1306_ch1115_SetDirection;
      _this->SetIsDisplay=ssd1306_ch1115_SetIsDisplay;
      ch1115_Init(_this);
      break;
  
    default:
      return -1;
  }

  

  return 0;
}






