/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-06 18:01:17
 * @LastEditTime: 2021-11-10 01:43:20
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

static void SSD1306_Init(OLED* _this){
  _this->LL_Function.Reset();
  _this->LL_Function.SendSequence((void*)OLED_SSD1306_InitSequence_CMD,vOLED_SSD1306_InitSequence_CMD_Length,NULL,0,-1);
}

static void CH1115_Init(OLED* _this){
  _this->LL_Function.Reset();
  _this->LL_Function.SendSequence((void*)OLED_CH1115_InitSequence_CMD,vOLED_CH1115_InitSequence_CMD_Length,NULL,0,-1);
}


static void SSD1306_CH1115_SetDirection(OLED* _this,int Direction){

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

static void SSD1306_CH1115_SetContrast(OLED* _this,int Contrast){
  Hbyte c;

  c=(Contrast*255)/100;

  ((volatile Hbyte*)_this->CmdSequenceBuffer)[0]=0x81U;
  ((volatile Hbyte*)_this->CmdSequenceBuffer)[1]=c;
  _this->LL_Function.SendSequence(_this->CmdSequenceBuffer,2,NULL,0,-1);
}

static void SSD1306_CH1115_SetIsDisplay(OLED* _this,int IsDisplay){

  if(IsDisplay==0){
    ((volatile Hbyte*)_this->CmdSequenceBuffer)[0]=0xAEU;
  }else{
    ((volatile Hbyte*)_this->CmdSequenceBuffer)[0]=0xAFU;
  }

  _this->LL_Function.SendSequence(_this->CmdSequenceBuffer,1,NULL,0,-1);

}


static void SSD1306_Display(OLED* _this,void* Bitmap){
  
  ((volatile Hbyte*)_this->CmdSequenceBuffer)[0]=0x21U;
  ((volatile Hbyte*)_this->CmdSequenceBuffer)[1]=0;
  ((volatile Hbyte*)_this->CmdSequenceBuffer)[2]=_this->bytesNumOfPage-1;
  ((volatile Hbyte*)_this->CmdSequenceBuffer)[3]=0x22U;
  ((volatile Hbyte*)_this->CmdSequenceBuffer)[4]=0;
  ((volatile Hbyte*)_this->CmdSequenceBuffer)[5]=_this->NumOfPage-1;
  
  _this->LL_Function.SendSequence(_this->CmdSequenceBuffer,6,Bitmap,_this->NumOfPage*_this->bytesNumOfPage,0);

}

static void CH1115_Display(OLED* _this,void* Bitmap){
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
 * @brief 设置显示方向
 * @param _this 要操作的对象
 * @param Direction 显示方向 vOLED_Direction_0 vOLED_Direction_180 可选
 * @return 无
 */
void OLED_SetDirection(OLED* _this,int Direction){

  _this->LL_Function.Lock();
  _this->SetDirection(_this,Direction);
  _this->LL_Function.UnLock();
}

/**
 * @brief 设置亮度
 * @param _this 要操作的对象
 * @param Contrast 0~100的亮度值
 * @return 无
 */
void OLED_SetContrast(OLED* _this,int Contrast){

  _this->LL_Function.Lock();
  _this->SetContrast(_this,Contrast);
  _this->LL_Function.UnLock();
}

/**
 * @brief 设置是否开显示
 * @param _this 要操作的对象
 * @param IsDisplay 0:关显示 其他:开显示
 * @return 无
 */
void OLED_SetIsDisplay(OLED* _this,int IsDisplay){
  _this->LL_Function.Lock();
  _this->IsDisplay=IsDisplay;
  _this->SetIsDisplay(_this,IsDisplay);
  _this->LL_Function.UnLock();
}

/**
 * @brief 显示一帧图像
 * @param _this 要操作的对象
 * @param Bitmap 要显示的位图
 * @return 无
 */
void OLED_Display(OLED* _this,void* Bitmap){
  _this->LL_Function.Lock();
  if(_this->IsDisplay!=0){
    _this->Display(_this,Bitmap);
  }
  _this->LL_Function.UnLock();
}

/**
 * @brief 初始化显示屏
 * @param _this 要操作的对象
 * @return 无
 */
void OLED_ScreenInit(OLED* _this){
  _this->LL_Function.Lock();
  _this->Init(_this);
  _this->Direction=vOLED_Direction_0;
  _this->IsDisplay=-1;
  _this->LL_Function.UnLock();
}

/**
 * @brief 反初始化屏幕 恢复到初始状态
 * @param _this 要操作的对象
 * @return 无
 */
void OLED_ScreenDeInit(OLED* _this){
  _this->LL_Function.Lock();
  _this->IsDisplay=0;
  _this->LL_Function.Reset();
  _this->LL_Function.UnLock();
}

/**
 * @brief 使得_this拥有操作底层的能力 不会对显示屏进行真正的操作
 * @param _this 要初始化的空对象
 * @param LL_Function 底层调用方法
 * @param CmdSequenceBuffer 命令队列缓存 vOLED_CmdSequenceBufferLength字节大小(或者更大)
 * @param bytesNumOfPage 每页的字节数量
 * @param NumOfPage 屏幕的页数
 * @param Type OLED类型
 * @return 0:成功 -1:无效参数
 */
int OLED_Init(OLED* _this,OLED_LL_Function* LL_Function,void* CmdSequenceBuffer,int bytesNumOfPage,int NumOfPage,int Type){

  _this->Direction=vOLED_Direction_0;
  _this->IsDisplay=0;
  _this->CmdSequenceBuffer=CmdSequenceBuffer;
  _this->bytesNumOfPage=bytesNumOfPage;
  _this->NumOfPage=NumOfPage;
  _this->Type=Type;
  _this->LL_Function=LL_Function[0];

  switch(Type){
    case vOLED_Type_SSD1306:
      _this->Display=SSD1306_Display;
      _this->SetContrast=SSD1306_CH1115_SetContrast;
      _this->SetDirection=SSD1306_CH1115_SetDirection;
      _this->SetIsDisplay=SSD1306_CH1115_SetIsDisplay;
      _this->Init=SSD1306_Init;
      break;
    case vOLED_Type_CH1115:
      _this->Display=CH1115_Display;
      _this->SetContrast=SSD1306_CH1115_SetContrast;
      _this->SetDirection=SSD1306_CH1115_SetDirection;
      _this->SetIsDisplay=SSD1306_CH1115_SetIsDisplay;
      _this->Init=CH1115_Init;
      break;
  
    default:
      return -1;
  }

  

  return 0;
}






