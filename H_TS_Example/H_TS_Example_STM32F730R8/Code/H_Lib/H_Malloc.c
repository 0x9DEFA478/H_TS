/*
 * @Author: 0x9DEFA478
 * @Date: 2021-03-27 18:55:04
 * @LastEditTime: 2021-08-01 20:53:00
 * @LastEditors: 0x9DEFA478
 * @Description: Malloc主体实现
 * 实现了一个内存管理算法 这是一个可以快速获取/释放内存的内存管理算法 
 * 无内存表 不需要每次都遍历内存表(或者内存标记) 而是从上次申请到内存的位置接着向后查找
 * 内存标记使用的是内存大小 标记在申请到的内存前方 使得释放极其迅速 
 * 
 * 此内存管理算法速度优先 例如对齐大小为4字节 每个申请的内存都会额外消耗4字节的内存来标记大小
 * 若8字节对齐 则会使用8字节来标记内存大小 即使用的字节数与对齐字节数相同 (尽管用不到那么大的长度,但为了保证效率,牺牲了利用率)
 * 此算法适合申请内存数目较低 需要频繁申请/释放的场合中
 * 
 * 使用方法:
 *  数组作为内存池 用H_Malloc_Init()进行初始化 数组即被初始化为内存池 可以被H_Malloc()来申请内存与H_Free()来释放内存
 *  H_Malloc_GetInfo()可以用于判断内存池是否出现异常
 * 
 * 
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


#include "H_Malloc.h"









typedef struct
{
  volatile H_Malloc_Size* volatile StartSize_Ptr;//用于存放第一个内存池首地址
  volatile H_Malloc_Size* volatile EntrySize_Ptr;//下次malloc时开始搜索的地址
  void* EndAddr;//边界地址 为(内存池首指针+内存池大小)
}H_Malloc_Header;



/**
 * @brief 初始化内存池
 * @param MemAddr 作为内存池数组的指针
 * @param MemSize 数组大小(字节)
 * @return 无
 */
void H_Malloc_Init(void* MemAddr,H_Malloc_Size MemSize){
	
  H_Malloc_Header* Header;
  H_Malloc_Size offset;
  Hbyte_ptr ptr;

  Header=(H_Malloc_Header*)MemAddr;

  

  offset=sizeof(H_Malloc_Header);
  while(((offset+H_Malloc_Align)%H_Malloc_Align)!=0){
    offset++;
  }
  ptr=(Hbyte_ptr)MemAddr;
  Header->StartSize_Ptr=(H_Malloc_Size*)&ptr[offset];
  Header->EntrySize_Ptr=Header->StartSize_Ptr;
  Header->StartSize_Ptr[0]=MemSize-offset-H_Malloc_Align;
  Header->StartSize_Ptr[1]=0;
  Header->EndAddr=(void*)&ptr[MemSize];
}

/**
 * @brief 向指定内存池申请内存
 * @param MemAddr 内存池地址
 * @param Size 要申请的内存大小
 * @return 申请到的内存指针 如果为NULL则为失败
 */
void* H_Malloc(void* MemAddr,H_Malloc_Size Size){
  H_Malloc_Header* Header;
  H_Malloc_Size tSize;
  volatile H_Malloc_Size* EntrySize_Ptr;
  volatile H_Malloc_Size* After_Ptr;
  volatile H_Malloc_Size* New_Ptr;
  H_Malloc_Size Msk;
  Hbyte_ptr ptr;
  Hbyte_ptr r;


  if(Size==0)
  {
    return NULL;
  }

  Msk=((H_Malloc_Size)0x1)<<(sizeof(H_Malloc_Size)*8-1);
  Header=(H_Malloc_Header*)MemAddr;
  EntrySize_Ptr=Header->EntrySize_Ptr;

  //将大小整和成H_Malloc_Align的倍数
  Size+=H_Malloc_Align-1;
  Size-=Size%H_Malloc_Align;
  
  do{
    tSize=EntrySize_Ptr[0];

    if(Msk&tSize){
      //已占用

    }else{
      //未占用

      if(tSize>Size){
        //大小不等

        if(tSize-Size<(H_Malloc_Align*2)){
          //不可分割

          *EntrySize_Ptr=tSize|Msk;
          ptr=(Hbyte_ptr)EntrySize_Ptr;
          r=&ptr[H_Malloc_Align];
          EntrySize_Ptr=(H_Malloc_Size*)&ptr[tSize+H_Malloc_Align];
          if(Header->EndAddr==EntrySize_Ptr){
            EntrySize_Ptr=Header->StartSize_Ptr;
          }
          Header->EntrySize_Ptr=EntrySize_Ptr;
					return (void*)r;
        }else{
          //可分割

          ptr=(Hbyte_ptr)EntrySize_Ptr;
          After_Ptr=(H_Malloc_Size*)&ptr[tSize+H_Malloc_Align];
          New_Ptr=(H_Malloc_Size*)&ptr[Size+H_Malloc_Align];
          

          New_Ptr[0]=tSize-Size-H_Malloc_Align;
          New_Ptr[1]=Size;
          EntrySize_Ptr[0]=Size|Msk;

          if((void*)After_Ptr!=Header->EndAddr){
            //有下一个
            After_Ptr[1]=New_Ptr[0];
          }

          ptr=(Hbyte_ptr)EntrySize_Ptr;
          Header->EntrySize_Ptr=New_Ptr;
          return (void*)&ptr[H_Malloc_Align];
        }
      }else if(tSize==Size){
        //大小相同

        *EntrySize_Ptr=tSize|Msk;
        ptr=(Hbyte_ptr)EntrySize_Ptr;
        r=&ptr[H_Malloc_Align];
        EntrySize_Ptr=(H_Malloc_Size*)&ptr[tSize+H_Malloc_Align];
        if(Header->EndAddr==EntrySize_Ptr){
          EntrySize_Ptr=Header->StartSize_Ptr;
        }
        Header->EntrySize_Ptr=EntrySize_Ptr;
        return (void*)r;
      }
    }

    ptr=(Hbyte_ptr)EntrySize_Ptr;
    EntrySize_Ptr=(H_Malloc_Size*)&ptr[(tSize&(~Msk))+H_Malloc_Align];
    if(Header->EndAddr==EntrySize_Ptr){
      EntrySize_Ptr=Header->StartSize_Ptr;
    }

  }while(EntrySize_Ptr!=Header->EntrySize_Ptr);

  return NULL;
}

/**
 * @brief 将内存释放回内存池
 * @param MemAddr 内存池地址
 * @param Ptr 要释放的内存指针
 * @return 无
 */
void H_Free(void* MemAddr,void* Ptr){

  H_Malloc_Header* Header;
  H_Malloc_Size tSize;
  volatile H_Malloc_Size* Last_Ptr;
  volatile H_Malloc_Size* EntrySize_Ptr;
  volatile H_Malloc_Size* After_Ptr;
  volatile H_Malloc_Size* After_After_Ptr;
  H_Malloc_Size Msk;
  Hbyte_ptr ptr;

  if(Ptr==NULL)
  {
    return;
  }
  

  Msk=((H_Malloc_Size)0x1)<<(sizeof(H_Malloc_Size)*8-1);
  Header=(H_Malloc_Header*)MemAddr;
  EntrySize_Ptr=(H_Malloc_Size*)((Hbyte_ptr)Ptr-(Hbyte_ptr)H_Malloc_Align);
  tSize=EntrySize_Ptr[0]&(~Msk);

  if(EntrySize_Ptr[1]==0){
    //没有上一块内存

    ptr=(Hbyte_ptr)EntrySize_Ptr;
    After_Ptr=(H_Malloc_Size*)&ptr[tSize+H_Malloc_Align];

    if((void*)After_Ptr!=Header->EndAddr){
      //有下一个

      ptr=(Hbyte_ptr)After_Ptr;
      After_After_Ptr=(H_Malloc_Size*)&ptr[(After_Ptr[0]&(~Msk))+H_Malloc_Align];

      if(After_Ptr[0]&Msk){
      //下一个被占用

        EntrySize_Ptr[0]&=(~Msk);//取消标记
      }else{
        //都无占用

        if(After_Ptr==Header->EntrySize_Ptr){
          //下次Malloc是即将要合并的位置

          Header->EntrySize_Ptr=EntrySize_Ptr;
        }
        EntrySize_Ptr[0]=tSize+After_Ptr[0]+H_Malloc_Align;//取消标记 并且更新大小
        if((void*)After_After_Ptr!=Header->EndAddr){
          After_After_Ptr[1]=EntrySize_Ptr[0];
        }
      }
    }else{
      //无下一个

      EntrySize_Ptr[0]&=(~Msk);//取消标记
    }
  }else{
    //有上一块内存

    Last_Ptr=(H_Malloc_Size*)((Hbyte_ptr)EntrySize_Ptr-(Hbyte_ptr)(EntrySize_Ptr[1]+(H_Malloc_Size)H_Malloc_Align));
    ptr=(Hbyte_ptr)EntrySize_Ptr;
    After_Ptr=(H_Malloc_Size*)&ptr[tSize+H_Malloc_Align];

    if((void*)After_Ptr!=Header->EndAddr){
      //有下一个

      ptr=(Hbyte_ptr)After_Ptr;
      After_After_Ptr=(H_Malloc_Size*)&ptr[(After_Ptr[0]&(~Msk))+H_Malloc_Align];

      if((Last_Ptr[0]&Msk)&&(After_Ptr[0]&Msk)){
        //都为已占用

        EntrySize_Ptr[0]&=(~Msk);//取消标记
      }else if(Last_Ptr[0]&Msk){
        //仅上一个占用 合并下一个

        if(After_Ptr==Header->EntrySize_Ptr){
          //下次Malloc是即将要合并的位置

          Header->EntrySize_Ptr=EntrySize_Ptr;
        }
        EntrySize_Ptr[0]=tSize+After_Ptr[0]+H_Malloc_Align;//取消标记 并且更新大小
        if((void*)After_After_Ptr!=Header->EndAddr){
          After_After_Ptr[1]=EntrySize_Ptr[0];
        }
      }else if(After_Ptr[0]&Msk){
        //仅下一个占用 合并到上一个

        if(EntrySize_Ptr==Header->EntrySize_Ptr){
          //下次Malloc是即将要合并的位置

          Header->EntrySize_Ptr=Last_Ptr;
        }
        Last_Ptr[0]=Last_Ptr[0]+tSize+H_Malloc_Align;//并且更新大小
        After_Ptr[1]=Last_Ptr[0];
      }else{
        //都无占用 全合并

        if((EntrySize_Ptr==Header->EntrySize_Ptr)||(After_Ptr==Header->EntrySize_Ptr)){
          //下次Malloc是即将要合并的位置

          Header->EntrySize_Ptr=Last_Ptr;
        }
        Last_Ptr[0]=Last_Ptr[0]+tSize+After_Ptr[0]+H_Malloc_Align*2;//并且更新大小
        if((void*)After_After_Ptr!=Header->EndAddr){
          After_After_Ptr[1]=Last_Ptr[0];
        }
      }
    }else{
      //无下一个

      if(Last_Ptr[0]&Msk){
        //上一个被占用

        EntrySize_Ptr[0]&=(~Msk);//取消标记
      }else{
        //都无占用

        if(EntrySize_Ptr==Header->EntrySize_Ptr){
          //下次Malloc是即将要合并的位置

          Header->EntrySize_Ptr=Last_Ptr;
        }
        Last_Ptr[0]=Last_Ptr[0]+tSize+H_Malloc_Align;//并且更新大小
      }
    }
  }
}

/**
 * @brief 获取内存池状态
 * @param MemAddr 内存池地址
 * @param info 用于存放信息的指针
 * @return 无
 */
void H_Malloc_GetInfo(void* MemAddr,H_Malloc_Info* info){

  H_Malloc_Header* Header;
  volatile H_Malloc_Size* EntrySize_Ptr;
  volatile H_Malloc_Size* After_Ptr;
  //volatile H_Malloc_Size* After_After_Ptr;
  volatile H_Malloc_Size* Last_Ptr;
  H_Malloc_Size Msk;
  H_Malloc_Size tSize;
  Hbyte_ptr ptr;

  Msk=((H_Malloc_Size)0x1)<<(sizeof(H_Malloc_Size)*8-1);
  Header=(H_Malloc_Header*)MemAddr;


  info->UseSize=0;
  info->FreeSize=0;
  info->OccupySize=0;
  info->NoOccupySize=0;

  Last_Ptr=Header->StartSize_Ptr;

  tSize=Last_Ptr[0]&(~Msk);
  if(Last_Ptr[0]&Msk){
    info->UseSize+=tSize;
    info->OccupySize+=tSize+H_Malloc_Align;
  }else{
    info->FreeSize+=tSize;
    info->NoOccupySize+=tSize+H_Malloc_Align;
  }

  if(Last_Ptr[1]!=0){

    info->Result=-1;
    info->ErrPtr=(void*)Last_Ptr;
    return;
  }

  ptr=(Hbyte_ptr)Last_Ptr;
  EntrySize_Ptr=(H_Malloc_Size*)&ptr[tSize+H_Malloc_Align];
	

  while(EntrySize_Ptr!=Header->EndAddr)
  {
    if((void*)EntrySize_Ptr > Header->EndAddr){
      info->Result=-2;
      info->ErrPtr=(void*)EntrySize_Ptr;
      return;
    }


    tSize=EntrySize_Ptr[0]&(~Msk);
		
		
		if(EntrySize_Ptr[1]==0){
			//没有上一块内存
			
			ptr=(Hbyte_ptr)EntrySize_Ptr;
			After_Ptr=(H_Malloc_Size*)&ptr[tSize+H_Malloc_Align];
			
			if((void*)After_Ptr!=Header->EndAddr){
				//有下一个

				ptr=(Hbyte_ptr)After_Ptr;
				//After_After_Ptr=(H_Malloc_Size*)&ptr[(After_Ptr[0]&(~Msk))+H_Malloc_Align];

				if(After_Ptr[1]!=tSize){
					info->Result=-5;
					info->ErrPtr=(void*)EntrySize_Ptr;
					return;
				}
			}else{
				//无下一个
			}
		}else{
			//有上一块内存
			
			
			Last_Ptr=(H_Malloc_Size*)((Hbyte_ptr)EntrySize_Ptr-(Hbyte_ptr)(EntrySize_Ptr[1]+(H_Malloc_Size)H_Malloc_Align));
			ptr=(Hbyte_ptr)EntrySize_Ptr;
			After_Ptr=(H_Malloc_Size*)&ptr[tSize+H_Malloc_Align];

			if((void*)After_Ptr!=Header->EndAddr){
				//有下一个

				ptr=(Hbyte_ptr)After_Ptr;
				//After_After_Ptr=(H_Malloc_Size*)&ptr[(After_Ptr[0]&(~Msk))+H_Malloc_Align];

				if(After_Ptr[1]!=tSize){
					info->Result=-5;
					info->ErrPtr=(void*)EntrySize_Ptr;
					return;
				}
				
				if((Last_Ptr[0]&(~Msk))!=EntrySize_Ptr[1]){
					info->Result=-6;
					info->ErrPtr=(void*)EntrySize_Ptr;
					return;
				}
				
			}else{
				//无下一个

				if((Last_Ptr[0]&(~Msk))!=EntrySize_Ptr[1]){
					info->Result=-6;
					info->ErrPtr=(void*)EntrySize_Ptr;
					return;
				}
			}
		
		}
		
    if(EntrySize_Ptr[0]&Msk){
      info->UseSize+=tSize;
      info->OccupySize+=tSize+H_Malloc_Align;
    }else{
      if((Last_Ptr[0]&Msk)==0){
        //都为未占用
        info->Result=-3;
        info->ErrPtr=(void*)Last_Ptr;
        return;
      }

      info->FreeSize+=tSize;
      info->NoOccupySize+=tSize+H_Malloc_Align;
    }

    if((Last_Ptr[0]&(~Msk))!=EntrySize_Ptr[1]){
      info->Result=-4;
      info->ErrPtr=(void*)Last_Ptr;
      return;
    }

    Last_Ptr=EntrySize_Ptr;

    ptr=(Hbyte_ptr)Last_Ptr;
    EntrySize_Ptr=(H_Malloc_Size*)&ptr[tSize+H_Malloc_Align];
  }
	
	info->Result=0;
}




