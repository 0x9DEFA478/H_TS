/*
 * @Author: 0x9DEFA478
 * @Date: 2021-06-06 19:31:31
 * @LastEditTime: 2021-11-10 01:42:22
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



const unsigned char OLED_SSD1306_InitSequence_CMD[vOLED_SSD1306_InitSequence_CMD_Length]={

  0xAEU,//关显示
  0x40U,//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  0x81U,//设置对比度指令
  0xFFU,//对比度值
  0xA4U,//0xA4/0xA5 正常/全充满
  0xA6U,//0xA6/0xA7 正常/反相显示
  0x2EU,//关闭滚屏
  0xA0U,//0xA1左右反置 0xA0正常
  0xC0U,//0xC8上下反置 0xC0正常
  0xA8U,//set multiplex ratio
  0x3FU,//默认值
  0xD5U,//设置分频指令
  0xF0U,//设置值
  0xD9U,//充放电周期
  0x22U,//设置值 如果为0则无效
  0xDBU,//Vcomh反压
  0x20U,//0x00U:0.65Vcc 0x20U:0.77Vcc 0x30U:0.83Vcc
  0x20U,// -Set Page Addressing Mode 
  0x00U,//设置值 (0x00/0x01/0x02)00 水平 01垂直 02 单页
  0x8DU,//电荷泵
  0x14U,//0x14:启动 0x10:关闭
  0xAFU,//显示开


};

const unsigned char OLED_CH1115_InitSequence_CMD[vOLED_CH1115_InitSequence_CMD_Length]={

  0xAEU,//关显示
  0x40U,//设置开始行
  0x81U,//设置对比度
  0xFFU,//对比度值
  0x82U,//参考电阻设置
  0x05U,//使用内部310K电阻 不使用Iseg调节
  0xA0U,//自适应节能 0xA0:关 0xA1:开
  0xC0U,
  0xA2U,//硬件配置 0xA2 0xA3
  0xA4U,//是否全显示 0xA4:关 0xA5:开
  0xA6U,//是否反色   0xA6:关 0xA7:开
  0xA8U,//设置复用率
  0x3FU,//复用率
  0xADU,//设置电源策略 0x8A:外部电源 0x8B:DC变换器
  0x8BU,
  0xD3U,//设置偏移
  0x00U,
  0xD5U,//设置振荡器频率与分频
  0x52U,
  0xD9U,//预充电周期设置
  0x2FU,
  0xDBU,
  0x35U,
  0x31U,//设置内部DC输出电压 0x30U:10V 0x31U:7.4V 0x32U:8V 0x33U:9V
  0x00U,//设置列地址低位
  0x10U,//设置列地址高位
  0xB0U,//设置页地址
  0xAFU,//显示开


};


