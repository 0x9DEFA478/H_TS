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

//驱动类型 SSD1306
#define vOLED_Type_SSD1306   1

//驱动类型 CH1115
#define vOLED_Type_CH1115    2



//显示方向 0度
#define vOLED_Direction_0    0

//显示方向 180度
#define vOLED_Direction_180  180

//初始化时参数CmdSequenceBuffer的长度
#define vOLED_CmdSequenceBufferLength   6


typedef struct{
  void (*Lock)(void);//互斥锁定 可用于保证底层操作互斥
  void (*UnLock)(void);//互斥解锁 可用于保证底层操作互斥
  void (*Reset)(void);//复位调用
  void (*SendSequence)(void*,int,void*,int,int);//发送序列 (指令序列,指令序列长度,数据序列,数据序列长度 传输速度(0:高速 其他:低速))
}OLED_LL_Function;


typedef struct _OLED
{
  //屏幕类型
  int Type;

  //显示方向
  int Direction;

  //是否开显示
  volatile int IsDisplay;

  int bytesNumOfPage;
  int NumOfPage;

  void* CmdSequenceBuffer;//命令缓存

  void (*Init)(struct _OLED*);
  void (*SetDirection)(struct _OLED*,int);
  void (*SetContrast)(struct _OLED*,int);
  void (*SetIsDisplay)(struct _OLED*,int);
  void (*Display)(struct _OLED*,void*);

  //底层方法集合
  OLED_LL_Function LL_Function;


}OLED;





/**
 * @brief 设置显示方向
 * @param _this 要操作的对象
 * @param Direction 显示方向 vOLED_Direction_0 vOLED_Direction_180 可选
 * @return 无
 */
void OLED_SetDirection(OLED* _this,int Direction);

/**
 * @brief 设置亮度
 * @param _this 要操作的对象
 * @param Contrast 0~100的亮度值
 * @return 无
 */
void OLED_SetContrast(OLED* _this,int Contrast);

/**
 * @brief 设置是否开显示
 * @param _this 要操作的对象
 * @param IsDisplay 0:关显示 其他:开显示
 * @return 无
 */
void OLED_SetIsDisplay(OLED* _this,int IsDisplay);

/**
 * @brief 显示一帧图像
 * @param _this 要操作的对象
 * @param Bitmap 要显示的位图
 * @return 无
 */
void OLED_Display(OLED* _this,void* Bitmap);

/**
 * @brief 初始化显示屏
 * @param _this 要操作的对象
 * @return 无
 */
void OLED_ScreenInit(OLED* _this);

/**
 * @brief 反初始化屏幕 恢复到初始状态
 * @param _this 要操作的对象
 * @return 无
 */
void OLED_ScreenDeInit(OLED* _this);

/**
 * @brief 使得_this拥有操作底层的能力 不会对显示屏进行真正的操作
 * @param _this 要初始化的空对象
 * @param LL_Function 底层调用方法
 * @param CmdSequenceBuffer 命令队列缓存 6字节大小(或者更大)
 * @param bytesNumOfPage 每页的字节数量
 * @param NumOfPage 屏幕的页数
 * @param Type OLED类型
 * @return 0:成功 -1:无效参数
 */
int OLED_Init(OLED* _this,OLED_LL_Function* LL_Function,void* CmdSequenceBuffer,int bytesNumOfPage,int NumOfPage,int Type);







#define vOLED_SSD1306_InitSequence_CMD_Length   22
#define vOLED_CH1115_InitSequence_CMD_Length    28

#endif //__OLED_H_
