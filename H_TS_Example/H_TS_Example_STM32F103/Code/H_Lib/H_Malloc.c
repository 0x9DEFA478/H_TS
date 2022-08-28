/*
 * @Author: 0x9DEFA478
 * @Date: 2021-03-27 18:55:04
 * @LastEditTime: 2021-11-07 20:53:51
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




//占用标记掩码
#define H_Memory_BlockOccupyMsk    (((H_Malloc_SizeType)0x1U)<<(sizeof(H_Malloc_SizeType)*8-1))



typedef struct{
  H_Malloc_SizeType* StartBlock_Ptr;//用于存放第一个内存块首地址
  H_Malloc_SizeType* EntryBlock_Ptr;//下次malloc时开始搜索的地址
  H_Malloc_SizeType MemoryAlign;//用于生成对齐字节数 对齐字节数=2^MemoryAlign
  void* EndMemoryAddr;//边界地址 为(内存池首指针+内存池大小)
}H_Malloc_Header;



/**
 * @brief 初始化内存池
 * @param MemoryAddr 作为内存池数组的指针
 * @param MemorySize 数组大小(字节) 必须为MemoryAlign的倍数
 * @param MemoryAlign 用于生成对齐字节数 对齐字节数=2^MemoryAlign 并且不小于两倍sizeof(H_Malloc_SizeType)
 * @return 无
 */
void H_Malloc_Init(void* MemoryAddr,H_Malloc_SizeType MemorySize,H_Malloc_SizeType MemoryAlign){
  
  H_Malloc_Header* Header;
  H_Malloc_SizeType AlignSize;
  H_Malloc_SizeType Offset;
  H_Malloc_ByteType* Ptr;

  AlignSize=((H_Malloc_SizeType)0x1U)<<MemoryAlign;

  if(AlignSize<(2*sizeof(H_Malloc_SizeType))){
    for(;;){
      //卡死把你 瞎传数据
    }
  }

  Header=(H_Malloc_Header*)MemoryAddr;
  Header->MemoryAlign=MemoryAlign;

  Offset=sizeof(H_Malloc_Header);
  //循环到合适的地方
  while((Offset%AlignSize)!=0){
    Offset++;
  }
  Ptr=(H_Malloc_ByteType*)MemoryAddr;
  Header->StartBlock_Ptr=(H_Malloc_SizeType*)&Ptr[Offset];
  Header->EntryBlock_Ptr=Header->StartBlock_Ptr;//入口块默认为第一个块
  Header->StartBlock_Ptr[0]=(MemorySize-Offset-AlignSize)>>MemoryAlign;//块大小 单位AlignSize字节
  Header->StartBlock_Ptr[1]=0;//上一个块的大小 0表示没有上一个块 (不会产生0大小的块)
  Header->EndMemoryAddr=(void*)&Ptr[MemorySize];
}

/**
 * @brief 向指定内存池申请内存
 * @param MemoryAddr 内存池地址
 * @param Size 要申请的内存大小
 * @return 申请到的内存指针 如果为NULL则为失败
 */
void* H_Malloc(void* MemoryAddr,H_Malloc_SizeType Size){
  H_Malloc_Header* Header;
  H_Malloc_SizeType MemoryAlign;
  H_Malloc_SizeType AlignSize;
  H_Malloc_SizeType tSize;
  H_Malloc_SizeType* tBlock_Ptr;
  H_Malloc_SizeType* EntryBlock_Ptr;
  H_Malloc_SizeType* NextBlock_Ptr;
  H_Malloc_SizeType* NewBlock_Ptr;
  H_Malloc_ByteType* r;


  if(Size==0){
    return NULL;
  }

  Header=(H_Malloc_Header*)MemoryAddr;
  MemoryAlign=Header->MemoryAlign;
  AlignSize=((H_Malloc_SizeType)0x1U)<<MemoryAlign;
  EntryBlock_Ptr=Header->EntryBlock_Ptr;

  //将大小转化成以AlignSize字节单位
  if((Size&(AlignSize-1U))==0U){
    Size=Size>>Header->MemoryAlign;
  }else{
    Size=(Size>>Header->MemoryAlign)+1U;
  }
  
  tBlock_Ptr=EntryBlock_Ptr;
  do{
    tSize=tBlock_Ptr[0];//读取tBlock_Ptr指向的块大小

    if(tSize&H_Memory_BlockOccupyMsk){
      //已占用 不做操作

    }else{
      //未占用

      if(tSize>=Size){
        //大小足够 尝试分隔

        //如果要分隔 需要额外多出 2*AlignSize 字节的空间 (1个用来保存大小信息 另一个用于分配)

        if((tSize-Size)<2U){
          //大小不足以分割 全部用于分配

          tBlock_Ptr[0]=tSize|H_Memory_BlockOccupyMsk;//只需要直接标记就好

          //获取块分配的地址
          r=&((H_Malloc_ByteType*)tBlock_Ptr)[AlignSize];

          //开始更新EntryBlock_Ptr
          EntryBlock_Ptr=(H_Malloc_SizeType*)&((H_Malloc_ByteType*)tBlock_Ptr)[(tSize+1)<<MemoryAlign];
          if(Header->EndMemoryAddr==EntryBlock_Ptr){
            //现在的指针超过了内存范围(EndMemoryAddr用于界定范围, 只要内存没有被破坏, 就一定能指到这个)
            EntryBlock_Ptr=Header->StartBlock_Ptr;//指向开头 重新开始
          }
          Header->EntryBlock_Ptr=EntryBlock_Ptr;//更新EntryBlock_Ptr
          return (void*)r;
        }else{
          //可分割

          //获取下一块的指针
          NextBlock_Ptr=(H_Malloc_SizeType*)&((H_Malloc_ByteType*)tBlock_Ptr)[(tSize+1)<<MemoryAlign];

          //获取新块指针(这个块要即将从旧块中分隔出来)
          NewBlock_Ptr=(H_Malloc_SizeType*)&((H_Malloc_ByteType*)tBlock_Ptr)[(Size+1)<<MemoryAlign];
          
          //块大小信息更新
          NewBlock_Ptr[0]=tSize-Size-1U;
          NewBlock_Ptr[1]=Size;
          tBlock_Ptr[0]=Size|H_Memory_BlockOccupyMsk;

          if((void*)NextBlock_Ptr!=Header->EndMemoryAddr){
            //NextBlock_Ptr为有效块的指针
            NextBlock_Ptr[1]=NewBlock_Ptr[0];//更新大小信息
          }

          Header->EntryBlock_Ptr=NewBlock_Ptr;

          //获取块分配的地址
          r=&((H_Malloc_ByteType*)tBlock_Ptr)[AlignSize];

          return (void*)r;
        }
      }
    }

    tBlock_Ptr=(H_Malloc_SizeType*)&((H_Malloc_ByteType*)tBlock_Ptr)[((tSize&(~H_Memory_BlockOccupyMsk))+1)<<MemoryAlign];
    if(Header->EndMemoryAddr==tBlock_Ptr){
      //现在的指针超过了内存范围(EndMemoryAddr用于界定范围, 只要内存没有被破坏, 就一定能指到这个)
      tBlock_Ptr=Header->StartBlock_Ptr;//指向开头 重新开始
    }

  }while(tBlock_Ptr!=EntryBlock_Ptr);//如果回到开始的块 表示操作结束

  return NULL;
}

/**
 * @brief 将内存释放回内存池
 * @param MemoryAddr 内存池地址
 * @param Ptr 要释放的内存指针
 * @return 无
 */
void H_Free(void* MemoryAddr,void* Ptr){
  H_Malloc_Header* Header;
  H_Malloc_SizeType MemoryAlign;
  H_Malloc_SizeType AlignSize;
  H_Malloc_SizeType tSize;
  H_Malloc_SizeType* tBlock_Ptr;
  H_Malloc_SizeType* LastBlock_Ptr;
  //H_Malloc_SizeType* EntryBlock_Ptr;
  H_Malloc_SizeType* NextBlock_Ptr;
  H_Malloc_SizeType* NextNextBlock_Ptr;


  if(Ptr==NULL){
    return;
  }

  Header=(H_Malloc_Header*)MemoryAddr;
  MemoryAlign=Header->MemoryAlign;
  AlignSize=((H_Malloc_SizeType)0x1U)<<MemoryAlign;
  //EntryBlock_Ptr=Header->EntryBlock_Ptr;
  
  //获取对应的块
  tBlock_Ptr=(H_Malloc_SizeType*)((H_Malloc_ByteType*)Ptr-(H_Malloc_ByteType*)AlignSize);
  //获取大小
  tSize=tBlock_Ptr[0]&(~H_Memory_BlockOccupyMsk);

  if(tBlock_Ptr[1]==0U){
    //没有上一块内存

    NextBlock_Ptr=(H_Malloc_SizeType*)&((H_Malloc_ByteType*)tBlock_Ptr)[(tSize+1)<<MemoryAlign];

    if((void*)NextBlock_Ptr!=Header->EndMemoryAddr){
      //下一个存在

      NextNextBlock_Ptr=(H_Malloc_SizeType*)&((H_Malloc_ByteType*)NextBlock_Ptr)[((NextBlock_Ptr[0]&(~H_Memory_BlockOccupyMsk))+1)<<MemoryAlign];

      if(NextBlock_Ptr[0]&H_Memory_BlockOccupyMsk){
        //下一个被占用

        Header->EntryBlock_Ptr=tBlock_Ptr;//直接让下一次分配此块 本块

        tBlock_Ptr[0]=tSize;//取消标记
      }else{
        //都无占用

        Header->EntryBlock_Ptr=tBlock_Ptr;//直接让下一次分配此块 本块

        tBlock_Ptr[0]=tSize+NextBlock_Ptr[0]+1U;//取消标记 并且更新大小
        if((void*)NextNextBlock_Ptr!=Header->EndMemoryAddr){
          //下一个的下一个存在 更新它
          NextNextBlock_Ptr[1]=tBlock_Ptr[0];
        }
      }
    }else{
      //无下一个

      Header->EntryBlock_Ptr=tBlock_Ptr;//直接让下一次分配此块 本块

      tBlock_Ptr[0]=tSize;//取消标记
    }
  }else{
    //有上一块内存

    LastBlock_Ptr=(H_Malloc_SizeType*)((H_Malloc_ByteType*)tBlock_Ptr-(H_Malloc_ByteType*)((tBlock_Ptr[1]+1)<<MemoryAlign));
    NextBlock_Ptr=(H_Malloc_SizeType*)&((H_Malloc_ByteType*)tBlock_Ptr)[(tSize+1)<<MemoryAlign];

    if((void*)NextBlock_Ptr!=Header->EndMemoryAddr){
      //有下一个

      NextNextBlock_Ptr=(H_Malloc_SizeType*)&((H_Malloc_ByteType*)NextBlock_Ptr)[((NextBlock_Ptr[0]&(~H_Memory_BlockOccupyMsk))+1)<<MemoryAlign];

      if((LastBlock_Ptr[0]&H_Memory_BlockOccupyMsk)&&(NextBlock_Ptr[0]&H_Memory_BlockOccupyMsk)){
        //都为已占用

        Header->EntryBlock_Ptr=tBlock_Ptr;//直接让下一次分配此块 本块

        tBlock_Ptr[0]=tSize;//取消标记
      }else if(LastBlock_Ptr[0]&H_Memory_BlockOccupyMsk){
        //仅上一个占用 合并下一个

        Header->EntryBlock_Ptr=tBlock_Ptr;//直接让下一次分配此块 本块

        tBlock_Ptr[0]=tSize+NextBlock_Ptr[0]+1U;//取消标记 并且更新大小
        if((void*)NextNextBlock_Ptr!=Header->EndMemoryAddr){
          //下一个的下一个存在 更新它
          NextNextBlock_Ptr[1]=tBlock_Ptr[0];
        }
      }else if(NextBlock_Ptr[0]&H_Memory_BlockOccupyMsk){
        //仅下一个占用 合并到上一个

        Header->EntryBlock_Ptr=LastBlock_Ptr;//直接让下一次分配此块 上一个块
        
        LastBlock_Ptr[0]=LastBlock_Ptr[0]+tSize+1;//更新大小
        NextBlock_Ptr[1]=LastBlock_Ptr[0];
      }else{
        //都无占用 全合并

        Header->EntryBlock_Ptr=LastBlock_Ptr;//直接让下一次分配此块 上一个块

        LastBlock_Ptr[0]=LastBlock_Ptr[0]+tSize+NextBlock_Ptr[0]+2U;//更新大小
        if((void*)NextNextBlock_Ptr!=Header->EndMemoryAddr){
          NextNextBlock_Ptr[1]=LastBlock_Ptr[0];
        }
      }
    }else{
      //无下一个

      if(LastBlock_Ptr[0]&H_Memory_BlockOccupyMsk){
        //上一个被占用

        Header->EntryBlock_Ptr=tBlock_Ptr;//直接让下一次分配此块 本块

        tBlock_Ptr[0]=tSize;//取消标记
      }else{
        //都无占用

        Header->EntryBlock_Ptr=LastBlock_Ptr;//直接让下一次分配此块 上一个块

        LastBlock_Ptr[0]=LastBlock_Ptr[0]+tSize+1U;//更新大小
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
void H_Malloc_GetInfo(void* MemoryAddr,H_Malloc_Info* info){
  H_Malloc_Header* Header;
  H_Malloc_SizeType MemoryAlign;
  //H_Malloc_SizeType AlignSize;
  H_Malloc_SizeType tSize;
  H_Malloc_SizeType* tBlock_Ptr;
  H_Malloc_SizeType* LastBlock_Ptr;
  //H_Malloc_SizeType* EntryBlock_Ptr;
  H_Malloc_SizeType* NextBlock_Ptr;


  Header=(H_Malloc_Header*)MemoryAddr;
  MemoryAlign=Header->MemoryAlign;
  //AlignSize=((H_Malloc_SizeType)0x1U)<<MemoryAlign;
  //EntryBlock_Ptr=Header->EntryBlock_Ptr;


  info->UseSize=0;
  info->FreeSize=0;
  info->OccupySize=0;
  info->NoOccupySize=0;

  LastBlock_Ptr=Header->StartBlock_Ptr;

  tSize=LastBlock_Ptr[0]&(~H_Memory_BlockOccupyMsk);
  if(LastBlock_Ptr[0]&H_Memory_BlockOccupyMsk){
    info->UseSize+=tSize<<MemoryAlign;
    info->OccupySize+=(tSize+1)<<MemoryAlign;
  }else{
    info->FreeSize+=tSize<<MemoryAlign;
    info->NoOccupySize+=(tSize+1)<<MemoryAlign;
  }

  if(LastBlock_Ptr[1]!=0){

    info->Result=vH_Malloc_Info_Result_ExistsLast;
    info->ErrorPtr=(void*)LastBlock_Ptr;
    return;
  }

  tBlock_Ptr=(H_Malloc_SizeType*)&((H_Malloc_ByteType*)LastBlock_Ptr)[(tSize+1)<<MemoryAlign];
  
  while(tBlock_Ptr!=Header->EndMemoryAddr){
    if((void*)tBlock_Ptr>Header->EndMemoryAddr){
      //正常情况下 不会出界
      info->Result=vH_Malloc_Info_Result_BlockNoAlign;
      info->ErrorPtr=(void*)tBlock_Ptr;
      return;
    }

    tSize=tBlock_Ptr[0]&(~H_Memory_BlockOccupyMsk);
    
    if(tBlock_Ptr[1]==0){
      //没有上一块内存
      
      info->Result=vH_Malloc_Info_Result_InexistentLast;
      info->ErrorPtr=(void*)tBlock_Ptr;
      return;
    }else{
      //有上一块内存
      
      LastBlock_Ptr=(H_Malloc_SizeType*)((H_Malloc_ByteType*)tBlock_Ptr-(H_Malloc_ByteType*)((tBlock_Ptr[1]+1)<<MemoryAlign));
      NextBlock_Ptr=(H_Malloc_SizeType*)&((H_Malloc_ByteType*)tBlock_Ptr)[(tSize+1)<<MemoryAlign];

      if((void*)NextBlock_Ptr!=Header->EndMemoryAddr){
        //有下一个

        if(NextBlock_Ptr[1]!=tSize){
          info->Result=vH_Malloc_Info_Result_NextBlockError;
          info->ErrorPtr=(void*)tBlock_Ptr;
          return;
        }
        
        if((LastBlock_Ptr[0]&(~H_Memory_BlockOccupyMsk))!=tBlock_Ptr[1]){
          info->Result=vH_Malloc_Info_Result_LastBlockError;
          info->ErrorPtr=(void*)tBlock_Ptr;
          return;
        }
        
      }else{
        //无下一个

        if((LastBlock_Ptr[0]&(~H_Memory_BlockOccupyMsk))!=tBlock_Ptr[1]){
          info->Result=vH_Malloc_Info_Result_LastBlockError;
          info->ErrorPtr=(void*)tBlock_Ptr;
          return;
        }
      }
    
    }
    
    if(tBlock_Ptr[0]&H_Memory_BlockOccupyMsk){
      info->UseSize+=tSize<<MemoryAlign;
      info->OccupySize+=(tSize+1)<<MemoryAlign;
    }else{

      if((LastBlock_Ptr[0]&H_Memory_BlockOccupyMsk)==0){
        //相邻空闲块都为未占用
        info->Result=vH_Malloc_Info_Result_IdleBlockIncontnuity;
        info->ErrorPtr=(void*)LastBlock_Ptr;
        return;
      }

      info->FreeSize+=tSize<<MemoryAlign;
      info->NoOccupySize+=(tSize+1)<<MemoryAlign;
    }

    if((LastBlock_Ptr[0]&(~H_Memory_BlockOccupyMsk))!=tBlock_Ptr[1]){
      info->Result=vH_Malloc_Info_Result_LastBlockError;
      info->ErrorPtr=(void*)LastBlock_Ptr;
      return;
    }

    LastBlock_Ptr=tBlock_Ptr;

    tBlock_Ptr=(H_Malloc_SizeType*)&((H_Malloc_ByteType*)LastBlock_Ptr)[(tSize+1)<<MemoryAlign];
  }
  
  info->Result=vH_Malloc_Info_Result_None;
}




