/*
 * @Author: 0x9DEFA478
 * @Date: 2021-07-24 18:15:23
 * @LastEditTime: 2021-12-12 21:15:57
 * @LastEditors: 0x9DEFA478
 * @Description: 提供了一些位图绘制方法
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
#include "H_Graphics.h"


#define __weak __attribute__((weak))


/**
 * @brief 图形复制 低位靠近原点垂直分布 转 低位靠近原点垂直分布 (OLED)
 * @param Param 操作的参数
 * @return 无
 */
__weak void H_Graphics_L1Vertical_CopyTo_L1Vertical(H_Graphics_Area_Param* Param){
  int sx;
  int sy;
  int dx;
  int dy;
  int ox;
  int mx;
  int oy;
  union{
    int sob;
    unsigned char smsk;
  }sparam;
  union{
    int dob;
    unsigned char dmsk;
  }dparam;
  int tb;
  unsigned char* tSAddr;
  unsigned char* tDAddr;
  
  sx=Param->sX;
  sy=Param->sY;
  dx=Param->dX;
  dy=Param->dY;
  oy=0;
  while(oy<Param->Height){

    int dShift;
    unsigned char msk;

    tSAddr=Param->S->Bitmap;
    tDAddr=Param->D->Bitmap;

    tSAddr=&tSAddr[sx + (sy/8)*Param->S->Width];
    tDAddr=&tDAddr[dx + (dy/8)*Param->D->Width];

    //两个位图的字节位移
    sparam.sob=sy%8;
    dparam.dob=dy%8;

    //计算本次x循环复制的y方向的bit数
    if(sparam.sob>dparam.dob){
      tb=8-sparam.sob;
    }else{
      tb=8-dparam.dob;
    }
    if((oy+tb)>Param->Height){
      tb=Param->Height-oy;
    }

    msk=(0x01U<<tb)-1U;

    //计算相对位移(src要左移多少)
    dShift=dparam.dob-sparam.sob;

    //计算掩码
    sparam.smsk=msk<<sparam.sob;
    dparam.dmsk=~(msk<<dparam.dob);

    mx=Param->Width;
    ox=0;
    if(dShift<0){
      dShift=-dShift;

      for (ox = 0;ox<mx;ox++){
        tDAddr[ox]=(tDAddr[ox]&dparam.dmsk)|((tSAddr[ox]&sparam.smsk)>>dShift);
      }
    }else if (dShift==0){
      for (ox = 0;ox<mx;ox++)
      {
        tDAddr[ox]=(tDAddr[ox]&dparam.dmsk)|(tSAddr[ox]&sparam.smsk);
      }
    }else{
      for (ox = 0;ox<mx;ox++){
        tDAddr[ox]=(tDAddr[ox]&dparam.dmsk)|((tSAddr[ox]&sparam.smsk)<<dShift);
      }
    }

    oy+=tb;
    dy+=tb;
    sy+=tb;
  }
}

/**
 * @brief 图形复制 低位靠近原点垂直分布 转 低位靠近原点垂直分布 反色 (OLED)
 * @param Param 操作的参数
 * @return 无
 */
__weak void H_Graphics_L1Vertical_CopyTo_L1Vertical_Reverse(H_Graphics_Area_Param* Param){
  int sx;
  int sy;
  int dx;
  int dy;
  int ox;
  int mx;
  int oy;
  union{
    int sob;
    unsigned char smsk;
  }sparam;
  union{
    int dob;
    unsigned char dmsk;
  }dparam;
  int tb;
  unsigned char* tSAddr;
  unsigned char* tDAddr;
  
  sx=Param->sX;
  sy=Param->sY;
  dx=Param->dX;
  dy=Param->dY;
  oy=0;
  while(oy<Param->Height){

    int dShift;
    unsigned char msk;

    tSAddr=Param->S->Bitmap;
    tDAddr=Param->D->Bitmap;

    tSAddr=&tSAddr[sx + (sy/8)*Param->S->Width];
    tDAddr=&tDAddr[dx + (dy/8)*Param->D->Width];

    //两个位图的字节位移
    sparam.sob=sy%8;
    dparam.dob=dy%8;

    //计算本次x循环复制的y方向的bit数
    if(sparam.sob>dparam.dob){
      tb=8-sparam.sob;
    }else{
      tb=8-dparam.dob;
    }
    if((oy+tb)>Param->Height){
      tb=Param->Height-oy;
    }

    msk=(0x01U<<tb)-1U;

    //计算相对位移(src要左移多少)
    dShift=dparam.dob-sparam.sob;

    //计算掩码
    sparam.smsk=msk<<sparam.sob;
    dparam.dmsk=~(msk<<dparam.dob);

    mx=Param->Width;
    ox=0;
    if(dShift<0)
    {
      dShift=-dShift;

      for (ox = 0;ox<mx;ox++)
      {
        tDAddr[ox]=(tDAddr[ox]&dparam.dmsk)|(((~tSAddr[ox])&sparam.smsk)>>dShift);
      }
    }else if (dShift==0)
    {
      for (ox = 0;ox<mx;ox++)
      {
        tDAddr[ox]=(tDAddr[ox]&dparam.dmsk)|((~tSAddr[ox])&sparam.smsk);
      }
    }else{
      for (ox = 0;ox<mx;ox++)
      {
        tDAddr[ox]=(tDAddr[ox]&dparam.dmsk)|(((~tSAddr[ox])&sparam.smsk)<<dShift);
      }
    }

    oy+=tb;
    dy+=tb;
    sy+=tb;
  }

}

/**
 * @brief 填充颜色 低位靠近原点垂直分布
 * @param bitmap 要填充的位图
 * @param Color 颜色 0:黑色 其他:白色
 * @param X 坐标X
 * @param Y 坐标Y
 * @param Width 宽度
 * @param Height 高度
 * @return 无
 */
__weak void H_Graphics_L1Vertical_Fill(H_Graphics_Bitmap* bitmap,int Color,int X,int Y,int Width,int Height){
  int dx;
  int dy;
  int ox;
  int mx;
  int oy;
  union{
    int dob;
    unsigned char dmsk;
  }dparam;
  int tb;
  unsigned char* tDAddr;
  
  dx=X;
  dy=Y;
  oy=0;
  while(oy<Height){

    unsigned char msk;

    tDAddr=bitmap->Bitmap;

    tDAddr=&tDAddr[dx + (dy/8)*bitmap->Width];

    //两个位图的字节位移
    dparam.dob=dy%8;

    //计算本次x循环复制的y方向的bit数
    tb=8-dparam.dob;
    if((oy+tb)>Height){
      tb=Height-oy;
    }

    msk=(0x01U<<tb)-1U;

    //计算掩码
    dparam.dmsk=msk<<dparam.dob;

    mx=Width;
    ox=0;
    if(Color==0)
    {
      dparam.dmsk=~dparam.dmsk;
      for (ox = 0;ox<mx;ox++)
      {
        tDAddr[ox]=tDAddr[ox]&dparam.dmsk;
      }
    }else{
      for (ox = 0;ox<mx;ox++)
      {
        tDAddr[ox]=tDAddr[ox]|dparam.dmsk;
      }
    }

    oy+=tb;
    dy+=tb;
  }
}



