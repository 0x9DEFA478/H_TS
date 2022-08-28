/*
 * @Author: 0x9DEFA478
 * @Date: 2021-03-11 17:53:34
 * @LastEditTime: 2021-11-06 17:03:34
 * @LastEditors: 0x9DEFA478
 * @Description: 自定义字符串操作
 * ============================================================================================================================================
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
 * ============================================================================================================================================
 * 
 */
#include "H_String.h"
#include <stdarg.h>


#ifndef NULL
#define NULL  ((void*)0)
#endif // !NULL



typedef struct{
  int MinNumberOfString;//最短长度 (正负代表对齐)
  int NumberOfMantissa;//保留小数位数
  int isD;//是否有前导D/d
  char Sign;//指向数据类型字符
}H_String_Function_MarkParam;

typedef int (*H_String_Function_Def)(H_String_8BitCharType*,H_String_Function_MarkParam*,va_list*);


static int String_Function_E(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param);
static int String_Function_F(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param);
static int String_Function_I(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param);
static int String_Function_X(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param);
static int String_Function_x(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param);
static int String_Function_S(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param);
static int String_Function_123(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param);//AsciiCode 123 '{'
static int String_Function_125(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param);//AsciiCode 125 '}'
static int String_Function_37(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param);//AsciiCode 37 '%'




static const H_String_Function_Def String_Functions[128]={



  

  [0]=NULL,//0
  [1]=NULL,//1
  [2]=NULL,//2
  [3]=NULL,//3
  [4]=NULL,//4
  [5]=NULL,//5
  [6]=NULL,//6
  [7]=NULL,//7
  [8]=NULL,//8
  [9]=NULL,//9

  [10]=NULL,//10
  [12]=NULL,//12
  [13]=NULL,//13
  [14]=NULL,//14
  [15]=NULL,//15
  [16]=NULL,//16
  [17]=NULL,//17
  [18]=NULL,//18
  [19]=NULL,//19

  [20]=NULL,//20
  [21]=NULL,//21
  [22]=NULL,//22
  [23]=NULL,//23
  [24]=NULL,//24
  [25]=NULL,//25
  [26]=NULL,//26
  [27]=NULL,//27
  [28]=NULL,//28
  [29]=NULL,//29

  [30]=NULL,//30
  [31]=NULL,//31
  [' ']=NULL,//32
  [33]=NULL,//33
  [34]=NULL,//34
  [35]=NULL,//35
  [36]=NULL,//36
  ['%']=String_Function_37,//37
  [38]=NULL,//38
  [39]=NULL,//39

  [40]=NULL,//40
  [41]=NULL,//41
  [42]=NULL,//42
  [43]=NULL,//43
  [44]=NULL,//44
  [45]=NULL,//45
  [46]=NULL,//46
  [47]=NULL,//47
  [48]=NULL,//48
  [49]=NULL,//49

  [50]=NULL,//50
  [51]=NULL,//51
  [52]=NULL,//52
  [53]=NULL,//53
  [54]=NULL,//54
  [55]=NULL,//55
  [56]=NULL,//56
  [57]=NULL,//57
  [58]=NULL,//58
  [59]=NULL,//59

  [60]=NULL,//60
  [61]=NULL,//61
  [62]=NULL,//62
  [63]=NULL,//63
  [64]=NULL,//64
  ['A']=NULL,//65
  ['B']=NULL,//66
  ['C']=NULL,//67
  ['D']=NULL,//68
  ['E']=String_Function_E,//69

  ['F']=String_Function_F,//70
  ['G']=NULL,//71
  ['H']=NULL,//72
  ['I']=String_Function_I,//73
  ['J']=NULL,//74
  ['K']=NULL,//75
  ['L']=NULL,//76
  ['M']=NULL,//77
  ['N']=NULL,//78
  ['O']=NULL,//79

  ['P']=NULL,//80
  ['Q']=NULL,//81
  ['R']=NULL,//82
  ['S']=String_Function_S,
  ['T']=NULL,//84
  ['U']=NULL,//85
  ['V']=NULL,//86
  ['W']=NULL,//87
  ['X']=String_Function_X,//88
  ['Y']=NULL,//89

  ['Z']=NULL,//90
  [91]=NULL,//91
  [92]=NULL,//92
  [93]=NULL,//93
  [94]=NULL,//94
  [95]=NULL,//95
  [96]=NULL,//96
  ['a']=NULL,//97
  ['b']=NULL,//98
  ['c']=NULL,//99

  ['d']=NULL,//100
  ['e']=String_Function_E,//101
  ['f']=String_Function_F,//102
  ['g']=NULL,//103
  ['h']=NULL,//104
  ['i']=String_Function_I,//105
  ['j']=NULL,//106
  ['k']=NULL,//107
  ['l']=NULL,//108
  ['m']=NULL,//109

  ['n']=NULL,//110
  ['o']=NULL,//111
  ['p']=NULL,//112
  ['q']=NULL,//113
  ['r']=NULL,//114
  ['s']=String_Function_S,//115
  ['t']=NULL,//116
  ['u']=NULL,//117
  ['v']=NULL,//118
  ['w']=NULL,//119

  ['x']=String_Function_x,//120
  ['y']=NULL,//121
  ['z']=NULL,//122
  ['{']=String_Function_123,//123
  [124]=NULL,//124
  ['}']=String_Function_125,//125
  [126]=NULL,//126
  [127]=NULL//127

};


#define H_String_S8                                                char
#define H_String_U8                                                unsigned char
#define H_String_S32                                               long
#define H_String_U32                                               unsigned long
#define H_String_S64                                               long long
#define H_String_U64                                               unsigned long long
#define H_String_BitSizeOf(Type)                                   (8*((int)sizeof(Type)))

#define H_String_BigInteger_BaseType                               H_String_U64
#define H_String_BaseValType                                       H_String_U64
#define H_String_ExpValType                                        H_String_S32

#define H_String_DivMul_Exp7                                       7
#define H_String_DivMul_Exp16                                      16


//选择大整型乘除法的倍数 10^7 和 10^17 可选 分别使用 long 或 long long 进行大整型计算 根据机器选择合适的可以获得更好的性能
#define H_String_DivMul_Exp                                        H_String_DivMul_Exp7



#if H_String_DivMul_Exp==H_String_DivMul_Exp7
#define H_String_DivMul_Factor                                     10000000UL
#define H_String_DivMul_Type                                       H_String_U32
#elif H_String_DivMul_Exp==H_String_DivMul_Exp16
#define H_String_DivMul_Factor                                     10000000000000000ULL
#define H_String_DivMul_Type                                       H_String_U64
#else
#error "请选择正确的H_String_DivMul_Exp"
#endif




//长整型缓存大小 单位:字节 144已经足够浮点的计算(double)
#define vString_BigInteger_ByteLength                              144
#define vString_BigInteger_BufferSize                              ((vString_BigInteger_ByteLength+sizeof(H_String_BigInteger_BaseType)-1)/sizeof(H_String_BigInteger_BaseType))

//格式化方法(H_StringFormat())使用的大整型结构体是否为全局内存(如果使用全局内存, H_StringFormat()就不能重入)
#define BigInteger_IsStatic                                        0


typedef struct{
  int First;//第一个非0
  int Last;//最后一个非0
  H_String_BigInteger_BaseType BigInteger_Buffer[vString_BigInteger_BufferSize];//用于储存长整数
}H_String_BigInteger;

#define vDoubleFloatDecode_Result_PositiveInf       2
#define vDoubleFloatDecode_Result_Positive          1
#define vDoubleFloatDecode_Result_Nagative          -1
#define vDoubleFloatDecode_Result_NagativeInf       -2
#define vDoubleFloatDecode_Result_QNAN              -3
#define vDoubleFloatDecode_Result_SNAN              -4

/**
 * @brief 解码双精度浮点 base*(2^expVal)*(Val?-1:1)
 * @param Val 指向要解析的数
 * @param expVal 指数
 * @param baseVal 基数
 * @return 1:正数 -1:负数 2:正无穷 -2:负无穷 -3:QNAN -4:SNAN
 */
static int DoubleFloatDecode(double* Val,H_String_ExpValType* expVal,H_String_BaseValType* baseVal){

  H_String_U64 srcVal;
  H_String_BaseValType rawbase;
  H_String_ExpValType exp;
  H_String_BaseValType base;
  
  
  srcVal=((H_String_U64*)Val)[0];

  if((srcVal&(~0x8000000000000000ULL))==0ULL){
    expVal[0]=0LL;
    baseVal[0]=0LL;
    goto Return;
  }

  rawbase=srcVal&0xFFFFFFFFFFFFFULL;

  exp=(H_String_ExpValType)((srcVal>>52)&0x7FFULL);
  if(exp==(H_String_ExpValType)(0x7FFLL)){
    //无穷大或nan
    if(rawbase==0ULL){
      //无穷大
      if((srcVal&0x8000000000000000ULL)!=0ULL){
        return vDoubleFloatDecode_Result_NagativeInf;
      }
      return vDoubleFloatDecode_Result_PositiveInf;
    }else{
      if((rawbase&0x8000000000000ULL)==0ULL){
        return vDoubleFloatDecode_Result_SNAN;
      }else{
        return vDoubleFloatDecode_Result_QNAN;
      }
    }
  }else{

    if(exp==0){
      exp-=((H_String_ExpValType)0x3FFLL)-1LL;
      exp-=52LL;
      base=(H_String_BaseValType)rawbase;
    }else{
      exp-=0x3FFLL;
      exp-=52LL;
      base=(H_String_BaseValType)(0x10000000000000ULL|rawbase);
    }
  }

  baseVal[0]=base;
  expVal[0]=exp;

Return:
  if((srcVal&0x8000000000000000ULL)!=0ULL){
    return vDoubleFloatDecode_Result_Nagative;
  }

  return vDoubleFloatDecode_Result_Positive;
}

//清零BigInteger
static void H_String_BigInteger_Set0(H_String_BigInteger* BigInteger){
  H_String_BigInteger_BaseType* dst;
  int i;

  dst=BigInteger->BigInteger_Buffer;
  i=vString_BigInteger_BufferSize;
  while(i>=8){
    i-=8;

    dst[0]=0ULL;
    dst[1]=0ULL;
    dst[2]=0ULL;
    dst[3]=0ULL;
    dst[4]=0ULL;
    dst[5]=0ULL;
    dst[6]=0ULL;
    dst[7]=0ULL;

    dst=&dst[8];
  }
  while(i>0){
    i-=1;

    dst[0]=0ULL;

    dst=&dst[1];
  }

}

//浮点的整数部分转大整型 小端模式
static void GenerateBigInteger_Integer(H_String_BigInteger* BigInteger,H_String_ExpValType* expVal,H_String_BaseValType* baseVal){
  H_String_ExpValType ExpVal;
  H_String_BaseValType BaseVal;
  int LShfit;
  int LByteLength;
  int i;
  H_String_U8* dst;
  H_String_U8 last;
  H_String_U8 H;
  H_String_U8 L;

  //清零
  H_String_BigInteger_Set0(BigInteger);

  ExpVal=expVal[0];
  BaseVal=baseVal[0];

  if(ExpVal<0){
    //有小数部分

    if(ExpVal<(-(H_String_BitSizeOf(H_String_BaseValType)-1))){
      BigInteger->First=vString_BigInteger_ByteLength;
      BigInteger->Last=-1;
      return;//整数部分为0
    }else{
      //部分小数部分整数
      BaseVal>>=(-ExpVal);
      ExpVal=0;
    }
  }

  LShfit=(int)(ExpVal&0x7ULL);
  LByteLength=(int)(ExpVal>>3);

  dst=&((H_String_U8*)BigInteger->BigInteger_Buffer)[LByteLength];
  last=0;

  for(i=0;i<sizeof(H_String_BaseValType);i++){
    H=(H_String_U8)(BaseVal>>(i*8));

    L=H<<LShfit;

    dst[0]=last|L;
    last=H>>(8-LShfit);

    dst=&dst[1];

  }
  dst[0]=last;

  dst=(H_String_U8*)BigInteger->BigInteger_Buffer;

  i=LByteLength;
  while((i<vString_BigInteger_ByteLength)&&(dst[i]==0)){
    i++;
  }
  BigInteger->First=i;

  i=LByteLength+8;
  while((i>=0)&&(dst[i]==0)){
    i--;
  }
  BigInteger->Last=i;
}

//浮点的小数部分转大整型 大端模式
static void GenerateBigInteger_Decimal(H_String_BigInteger* BigInteger,H_String_ExpValType* expVal,H_String_BaseValType* baseVal){
  H_String_ExpValType ExpVal;
  H_String_BaseValType BaseVal;
  int RShfit;
  int RByteLength;
  int i;
  H_String_U8* dst;
  H_String_U8 last;
  H_String_U8 H;
  H_String_U8 L;

  //清零
  H_String_BigInteger_Set0(BigInteger);

  ExpVal=expVal[0];
  BaseVal=baseVal[0];

  if(ExpVal>(-H_String_BitSizeOf(H_String_BaseValType))){
    //有整数部分

    if(ExpVal<0){
      //部分小数部分整数
      BaseVal<<=(H_String_BitSizeOf(H_String_BaseValType)+ExpVal);
      ExpVal=-H_String_BitSizeOf(H_String_BaseValType);
    }
    else{
      BigInteger->First=vString_BigInteger_ByteLength;
      BigInteger->Last=-1;
      return;//小数部分为0
    }
  }

  ExpVal=-ExpVal;

  ExpVal-=H_String_BitSizeOf(H_String_BaseValType);

  RShfit=(int)(ExpVal&0x7ULL);
  RByteLength=(int)(ExpVal>>3);

  dst=&((H_String_U8*)BigInteger->BigInteger_Buffer)[RByteLength];
  last=0;

  for(i=0;i<sizeof(H_String_BaseValType);i++){
    L=(H_String_U8)(BaseVal>>((sizeof(H_String_BaseValType)-1-i)*8));

    H=L>>RShfit;

    dst[0]=H|last;
    last=L<<(8-RShfit);

    dst=&dst[1];

  }
  dst[0]=last;

  dst=(H_String_U8*)BigInteger->BigInteger_Buffer;

  i=RByteLength;
  while((i<vString_BigInteger_ByteLength)&&(dst[i]==0)){
    i++;
  }
  BigInteger->First=i;

  i=RByteLength+8;
  while((i>=0)&&(dst[i]==0)){
    i--;
  }
  BigInteger->Last=i;
}

//大整型除法, 除以H_String_DivMul_Factor, 除法的商将存储在输入的BigInteger中, 返回余数, 用于GenerateBigInteger_Integer()生成的大整型
static H_String_DivMul_Type BigInteger_Division(H_String_BigInteger* BigInteger){
  int i;
  H_String_DivMul_Type rem;
  H_String_U8 read;
  H_String_U8 quotient;
  H_String_U8* src;
  int Last;
  
  Last=BigInteger->Last;
  if(Last<0){
    return 0;
  }
  src=(H_String_U8*)BigInteger->BigInteger_Buffer;
  rem=0;
  i=Last+1;
  while(i>0){
    i--;

    quotient=0;

    read=src[i];
    rem=(rem<<(H_String_BitSizeOf(H_String_U8)))+read;
    quotient=(H_String_U8)(rem/H_String_DivMul_Factor);
    rem=rem%H_String_DivMul_Factor;

    src[i]=quotient;

  }

  while(src[Last]==0){
    Last--;
    BigInteger->Last=Last;
  }

  return rem;
}

//大整型乘法, 乘以H_String_DivMul_Factor, 乘法的积将存储在输入的BigInteger中, 返回溢出的值(如果无溢出,返回0), 用于GenerateBigInteger_Decimal()生成的大整型
static H_String_DivMul_Type BigInteger_Multiply(H_String_BigInteger* BigInteger){
  int i;
  H_String_DivMul_Type cnt;
  H_String_U8 read;
  H_String_U8* src;
  int First;
  int Last;


  Last=BigInteger->Last;
  if(Last<0){
    return 0;
  }
  First=BigInteger->First;
  src=(H_String_U8*)BigInteger->BigInteger_Buffer;
  cnt=0;
  i=Last+1;
  while(i>First){
    i--;

    read=src[i];

    cnt+=(H_String_DivMul_Type)read*H_String_DivMul_Factor;
    src[i]=(H_String_U8)cnt&0xFFU;
    cnt>>=(H_String_BitSizeOf(H_String_U8));

  }

  while(src[Last]==0){
    Last--;
    BigInteger->Last=Last;
  }

  while((cnt>0)&&(First>0)){
    First--;
    src[First]=(H_String_U8)cnt&0xFFU;
    cnt>>=(H_String_BitSizeOf(H_String_U8));
    BigInteger->First=First;
  }

  return cnt;
}

#if BigInteger_IsStatic != 0
static H_String_BigInteger BigInteger;
#endif




//e E
static int String_Function_E(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param){
#if BigInteger_IsStatic == 0
  H_String_BigInteger BigInteger;
#endif
  double Val_d;
  int IntegerLength;
  H_String_DivMul_Type Remainder;
  H_String_DivMul_Type Product;
  int e;
  H_String_8BitCharType* IntegerPtr;

  int rLen;
  int realLen;
  H_String_8BitCharType* t_wrPtr;

  int MinNumberOfString;
  int MinNumberOfStringIsNegative;
  int NumberOfMantissa;
  int i;
  int j;
  H_String_8BitCharType tc;

  H_String_ExpValType expVal;
  H_String_BaseValType baseVal;
  int DecodeResult;


  MinNumberOfString=MarkParam->MinNumberOfString;
  if(MinNumberOfString<0){
    MinNumberOfString=-MinNumberOfString;
    MinNumberOfStringIsNegative=-1;
  }else{
    MinNumberOfStringIsNegative=0;
  }

  NumberOfMantissa=MarkParam->NumberOfMantissa;

  Val_d=va_arg(param[0],double);
  DecodeResult=DoubleFloatDecode(&Val_d,&expVal,&baseVal);

  t_wrPtr=wrPtr;

  IntegerLength=0;
  realLen=0;
  switch(DecodeResult){
    case vDoubleFloatDecode_Result_Nagative:

      t_wrPtr[0]='-';
      t_wrPtr=&t_wrPtr[1];
      realLen++;

    case vDoubleFloatDecode_Result_Positive:

      IntegerPtr=t_wrPtr;
      if(baseVal==0LL){
        t_wrPtr[0]='0';
        t_wrPtr=&t_wrPtr[1];
        IntegerLength++;

        e=0;

        if(NumberOfMantissa>0){
          t_wrPtr[0]='.';
          t_wrPtr=&t_wrPtr[1];
          IntegerLength++;//小数点也计入数量

          i=NumberOfMantissa;
          while(i>0){
            i--;
            t_wrPtr[0]='0';
            t_wrPtr=&t_wrPtr[1];
            IntegerLength++;
          }
        }
      }else{

        GenerateBigInteger_Integer(&BigInteger,&expVal,&baseVal);

        do{
          Remainder=BigInteger_Division(&BigInteger);

          j=H_String_DivMul_Exp;
          while(j>0){
            j--;
            t_wrPtr[0]=(Remainder%10)+'0';
            t_wrPtr=&t_wrPtr[1];
            IntegerLength++;
            Remainder/=10;
          }
        }while(BigInteger.Last>=0);

        while((IntegerLength>1)&&(IntegerPtr[IntegerLength-1]=='0')){
          IntegerLength--;
        }
        t_wrPtr=&IntegerPtr[IntegerLength];

        i=0;
        j=IntegerLength-1;
        while(i<j){
          tc=IntegerPtr[i];
          IntegerPtr[i]=IntegerPtr[j];
          IntegerPtr[j]=tc;
          j--;
          i++;
        }

        GenerateBigInteger_Decimal(&BigInteger,&expVal,&baseVal);

        if(IntegerPtr[0]=='0'){
          //此时整数部分一定为0 (如果整数部分不为0, 开头的字符就不会是'0')

          e=0;//指数暂且为0
          IntegerLength=0;

          do{
            Product=BigInteger_Multiply(&BigInteger);
            e-=H_String_DivMul_Exp;
          }while(Product==0);

          t_wrPtr=IntegerPtr;
          
          while(Product>0){
            IntegerLength++;
            t_wrPtr[0]=(Product%10)+'0';
            t_wrPtr=&t_wrPtr[1];
            Product/=10;
          }

          i=0;
          j=IntegerLength-1;
          while(i<j){
            tc=IntegerPtr[i];
            IntegerPtr[i]=IntegerPtr[j];
            IntegerPtr[j]=tc;
            j--;
            i++;
          }

          e+=IntegerLength-1;

        }else{
          //整数部分不为0 此时可确定指数
          e=IntegerLength-1;
        }
        
        //除了首位都后移一个字符
        if(NumberOfMantissa>0){
          i=IntegerLength<(NumberOfMantissa+2)?IntegerLength:(NumberOfMantissa+2);
          while(i>1){
            IntegerPtr[i]=IntegerPtr[i-1];
            i--;
          }
          IntegerPtr[1]='.';
          IntegerLength++;//连小数点一起算上

          i=NumberOfMantissa-(IntegerLength-2);

          if(i>0){
            //小数位不足

            while(i>0){
              i-=H_String_DivMul_Exp;

              Product=BigInteger_Multiply(&BigInteger);

              j=H_String_DivMul_Exp;
              while(j>0){
                j--;
                IntegerPtr[IntegerLength+j]=(Product%10)+'0';
                Product/=10;
              }
              IntegerLength+=H_String_DivMul_Exp;
            }
          }
          IntegerLength+=i;
        }else{
          IntegerLength=1;
        }
        
        t_wrPtr=&IntegerPtr[IntegerLength];//更新指针
      }

      realLen+=IntegerLength;
     
      t_wrPtr[0]=MarkParam->Sign;
      t_wrPtr=&t_wrPtr[1];
      realLen++;

      if(e==0){
        t_wrPtr[0]='0';
        t_wrPtr=&t_wrPtr[1];
        realLen++;
      }
      else{

        if(e<0){
          e=-e;
          t_wrPtr[0]='-';
          t_wrPtr=&t_wrPtr[1];
          realLen++;
        }

        IntegerPtr=t_wrPtr;
        IntegerLength=0;

        while(e>0){
          IntegerLength++;
          t_wrPtr[0]=(e%10)+'0';
          t_wrPtr=&t_wrPtr[1];
          e/=10;
        }
        realLen+=IntegerLength;

        i=0;
        j=IntegerLength-1;
        while(i<j){
          tc=IntegerPtr[i];
          IntegerPtr[i]=IntegerPtr[j];
          IntegerPtr[j]=tc;
          j--;
          i++;
        }
      }

      break;
    case vDoubleFloatDecode_Result_NagativeInf:

      t_wrPtr[0]='-';
      t_wrPtr=&t_wrPtr[1];
      realLen++;

    case vDoubleFloatDecode_Result_PositiveInf:

      t_wrPtr[0]='I';
      t_wrPtr[1]='n';
      t_wrPtr[2]='f';
      t_wrPtr=&t_wrPtr[3];
      realLen+=3;

      break;
    case vDoubleFloatDecode_Result_QNAN:

      t_wrPtr[0]='Q';
      t_wrPtr[1]='N';
      t_wrPtr[2]='A';
      t_wrPtr[3]='N';
      t_wrPtr=&t_wrPtr[4];
      realLen+=4;

      break;
    case vDoubleFloatDecode_Result_SNAN:

      t_wrPtr[0]='S';
      t_wrPtr[1]='N';
      t_wrPtr[2]='A';
      t_wrPtr[3]='N';
      t_wrPtr=&t_wrPtr[4];
      realLen+=4;

      break;
    default:
      for(;;){
      }
  }

  rLen=realLen;
  if(MinNumberOfStringIsNegative==0){
    i=MinNumberOfString-realLen;
    if(i>0){
      j=realLen;
      while(j>0){
        j--;
        wrPtr[j+i]=wrPtr[j];
      }
      while(i>0){
        i--;
        wrPtr[i]=' ';
      }

      rLen=MinNumberOfString;
    }
  }else{
    while(rLen<MinNumberOfString){
      rLen++;
      t_wrPtr[0]=' ';
      t_wrPtr=&t_wrPtr[1];
    }
  }

  return rLen;
}

//f F
static int String_Function_F(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param){
#if BigInteger_IsStatic == 0
  H_String_BigInteger BigInteger;
#endif
  double Val_d;
  int IntegerLength;
  H_String_DivMul_Type Remainder;
  H_String_DivMul_Type Product;
  H_String_8BitCharType* IntegerPtr;

  int rLen;
  int realLen;
  H_String_8BitCharType* t_wrPtr;
  
  int MinNumberOfString;
  int MinNumberOfStringIsNegative;
  int NumberOfMantissa;
  int i;
  int j;
  H_String_8BitCharType tc;

  H_String_ExpValType expVal;
  H_String_BaseValType baseVal;
  int DecodeResult;


  MinNumberOfString=MarkParam->MinNumberOfString;
  if(MinNumberOfString<0){
    MinNumberOfString=-MinNumberOfString;
    MinNumberOfStringIsNegative=-1;
  }else{
    MinNumberOfStringIsNegative=0;
  }

  NumberOfMantissa=MarkParam->NumberOfMantissa;
  
  Val_d=va_arg(param[0],double);
  DecodeResult=DoubleFloatDecode(&Val_d,&expVal,&baseVal);

  t_wrPtr=wrPtr;

  IntegerLength=0;
  realLen=0;
  switch(DecodeResult){
    case vDoubleFloatDecode_Result_Nagative:

      t_wrPtr[0]='-';
      t_wrPtr=&t_wrPtr[1];
      realLen++;

    case vDoubleFloatDecode_Result_Positive:

      IntegerPtr=t_wrPtr;
      if(baseVal==0LL){
        t_wrPtr[0]='0';
        t_wrPtr=&t_wrPtr[1];
        IntegerLength++;
      }else{

        GenerateBigInteger_Integer(&BigInteger,&expVal,&baseVal);

        do{
          Remainder=BigInteger_Division(&BigInteger);

          j=H_String_DivMul_Exp;
          while(j>0){
            j--;
            t_wrPtr[0]=(Remainder%10)+'0';
            t_wrPtr=&t_wrPtr[1];
            IntegerLength++;
            Remainder/=10;
          }
        }while(BigInteger.Last>=0);

        while((IntegerLength>1)&&(IntegerPtr[IntegerLength-1]=='0')){
          IntegerLength--;
        }
        t_wrPtr=&IntegerPtr[IntegerLength];

        i=0;
        j=IntegerLength-1;
        while(i<j){
          tc=IntegerPtr[i];
          IntegerPtr[i]=IntegerPtr[j];
          IntegerPtr[j]=tc;
          j--;
          i++;
        }
      }

      realLen+=IntegerLength;

      if(NumberOfMantissa>0){
        t_wrPtr[0]='.';
        t_wrPtr=&t_wrPtr[1];
        realLen++;
      }

      if(baseVal==0){
        i=NumberOfMantissa;
        while(i>0){
          i--;
          t_wrPtr[0]='0';
          t_wrPtr=&t_wrPtr[1];
        }
      }else{
        GenerateBigInteger_Decimal(&BigInteger,&expVal,&baseVal);

        if(NumberOfMantissa>0){
          IntegerLength=0;
          i=NumberOfMantissa;
          while(i>0){
            i-=H_String_DivMul_Exp;

            Product=BigInteger_Multiply(&BigInteger);

            j=H_String_DivMul_Exp;
            while(j>0){
              j--;
              t_wrPtr[IntegerLength+j]=(Product%10)+'0';
              Product/=10;
            }
            IntegerLength+=H_String_DivMul_Exp;
          }
          IntegerLength+=i;
          t_wrPtr=&t_wrPtr[IntegerLength];
        }
      }

      realLen+=NumberOfMantissa;

      break;
    case vDoubleFloatDecode_Result_NagativeInf:

      t_wrPtr[0]='-';
      t_wrPtr=&t_wrPtr[1];
      realLen++;

    case vDoubleFloatDecode_Result_PositiveInf:

      t_wrPtr[0]='I';
      t_wrPtr[1]='n';
      t_wrPtr[2]='f';
      t_wrPtr=&t_wrPtr[3];
      realLen+=3;

      break;
    case vDoubleFloatDecode_Result_QNAN:

      t_wrPtr[0]='Q';
      t_wrPtr[1]='N';
      t_wrPtr[2]='A';
      t_wrPtr[3]='N';
      t_wrPtr=&t_wrPtr[4];
      realLen+=4;

      break;
    case vDoubleFloatDecode_Result_SNAN:

      t_wrPtr[0]='S';
      t_wrPtr[1]='N';
      t_wrPtr[2]='A';
      t_wrPtr[3]='N';
      t_wrPtr=&t_wrPtr[4];
      realLen+=4;

      break;
    default:
      for(;;){
      }
  }
  
  rLen=realLen;
  if(MinNumberOfStringIsNegative==0){
    i=MinNumberOfString-realLen;
    if(i>0){
      j=realLen;
      while(j>0){
        j--;
        wrPtr[j+i]=wrPtr[j];
      }
      while(i>0){
        i--;
        wrPtr[i]=' ';
      }

      rLen=MinNumberOfString;
    }
  }else{
    while(rLen<MinNumberOfString){
      rLen++;
      t_wrPtr[0]=' ';
      t_wrPtr=&t_wrPtr[1];
    }
  }

  return rLen;
}

//i I
static int String_Function_I(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param){

  int rLen;
  int realLen;
  H_String_8BitCharType* t_wrPtr;

  int sVar_i;
  long long sVar_l;
  int MinNumberOfString;
  int MinNumberOfStringIsNegative;
  int i;
  int j;
  H_String_8BitCharType tc;

  if(MarkParam->isD!=0){
    sVar_l=va_arg(param[0],long long);
  }
  else{
    sVar_i=va_arg(param[0],int);
    sVar_l=sVar_i;
  }

  MinNumberOfString=MarkParam->MinNumberOfString;
  if(MinNumberOfString<0){
    MinNumberOfString=-MinNumberOfString;
    MinNumberOfStringIsNegative=-1;
  }else{
    MinNumberOfStringIsNegative=0;
  }
  
  t_wrPtr=wrPtr;

  if(sVar_l==0){

    t_wrPtr[0]='0';

    t_wrPtr=&t_wrPtr[1];
    realLen=1;

  }else if(((unsigned long long)sVar_l)==0x8000000000000000ULL){
    //-9223372036854775808==0x8000000000000000

    t_wrPtr[0]    ='-';
    t_wrPtr[1]    ='9';
    t_wrPtr[2]    ='2';
    t_wrPtr[3]    ='2';
    t_wrPtr[4]    ='3';
    t_wrPtr[5]    ='3';
    t_wrPtr[5]    ='3';
    t_wrPtr[6]    ='7';
    t_wrPtr[7]    ='2';
    t_wrPtr[8]    ='0';
    t_wrPtr[9]    ='3';
    t_wrPtr[10]   ='6';
    t_wrPtr[11]   ='8';
    t_wrPtr[12]   ='5';
    t_wrPtr[13]   ='4';
    t_wrPtr[14]   ='7';
    t_wrPtr[15]   ='7';
    t_wrPtr[16]   ='5';
    t_wrPtr[17]   ='8';
    t_wrPtr[18]   ='0';
    t_wrPtr[19]   ='8';

    t_wrPtr=&t_wrPtr[20];
    realLen=20;

  }else{
    if(sVar_l<0){
      sVar_l=-sVar_l;

      realLen=0;
      while(sVar_l!=0){
        realLen++;
        t_wrPtr[0]=(sVar_l%10)+'0';
        t_wrPtr=&t_wrPtr[1];
        sVar_l/=10;
      }
      realLen++;
      t_wrPtr[0]='-';
      t_wrPtr=&t_wrPtr[1];
    }else{
      realLen=0;
      while(sVar_l!=0){
        realLen++;
        t_wrPtr[0]=(sVar_l%10)+'0';
        t_wrPtr=&t_wrPtr[1];
        sVar_l/=10;
      }  
    }
  }

  i=0;
  j=realLen-1;
  while(i<j){
    tc=wrPtr[i];
    wrPtr[i]=wrPtr[j];
    wrPtr[j]=tc;
    j--;
    i++;
  }

  rLen=realLen;
  if(MinNumberOfStringIsNegative==0){
    i=MinNumberOfString-realLen;
    if(i>0){
      j=realLen;
      while(j>0){
        j--;
        wrPtr[j+i]=wrPtr[j];
      }
      while(i>0){
        i--;
        wrPtr[i]=' ';
      }

      rLen=MinNumberOfString;
    }
  }else{
    while(rLen<MinNumberOfString){
      rLen++;
      t_wrPtr[0]=' ';
      t_wrPtr=&t_wrPtr[1];
    }
  }

  return rLen;
}

//X
static int String_Function_X(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param){

  int rLen;
  int i;
  unsigned int val;
  unsigned int sVal_i;
  unsigned long long sVal_l;


  rLen=MarkParam->MinNumberOfString;
  if(rLen<0){
    rLen=-rLen;
  }

  if(MarkParam->isD!=0){

    i=rLen;
    sVal_l=va_arg(param[0],unsigned long long);
    while(i>0){
      i--;
      val=(sVal_l>>(i*4))&0xF;

      if(val<10){
        wrPtr[0]='0'+val;
      }
      else{
        wrPtr[0]='A'-10+val;
      }
      wrPtr=&wrPtr[1];
    }
  }else{

    i=rLen;
    sVal_i=va_arg(param[0],unsigned int);
    while(i>0){
      i--;
      val=(sVal_i>>(i*4))&0xF;

      if(val<10){
        wrPtr[0]='0'+val;
      }
      else{
        wrPtr[0]='A'-10+val;
      }
      wrPtr=&wrPtr[1];
    }
  }

  return rLen;
}

//x
static int String_Function_x(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param){
  int rLen;
  int i;
  unsigned int val;
  unsigned int sVal_i;
  unsigned long long sVal_l;
  

  rLen=MarkParam->MinNumberOfString;
  if(rLen<0){
    rLen=-rLen;
  }

  if(MarkParam->isD!=0){

    i=rLen;
    sVal_l=va_arg(param[0],unsigned long long);
    while(i>0){
      i--;
      val=(sVal_l>>(i*4))&0xF;

      if(val<10){
        wrPtr[0]='0'+val;
      }
      else{
        wrPtr[0]='a'-10+val;
      }
      wrPtr=&wrPtr[1];
    }
  }else{

    i=rLen;
    sVal_i=va_arg(param[0],unsigned int);
    while(i>0){
      i--;
      val=(sVal_i>>(i*4))&0xF;

      if(val<10){
        wrPtr[0]='0'+val;
      }
      else{
        wrPtr[0]='a'-10+val;
      }
      wrPtr=&wrPtr[1];
    }
  }

  return rLen;
}

//s S
static int String_Function_S(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param){
  int rLen;
  H_String_8BitCharType* src_str;

  rLen=0;
  src_str=va_arg(param[0],H_String_8BitCharType*);

  while(src_str[0]!='\0'){
    wrPtr[0]=src_str[0];

    src_str=&src_str[1];
    wrPtr=&wrPtr[1];
    rLen++;
  }

  return rLen;
}

//{
static int String_Function_123(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param){
  wrPtr[0]='{';
  return 1;
}

//}
static int String_Function_125(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param){
  wrPtr[0]='}';
  return 1;
}

//%
static int String_Function_37(H_String_8BitCharType* wrPtr,H_String_Function_MarkParam* MarkParam,va_list* param){
  wrPtr[0]='%';
  return 1;
}




//H格式
static void GetMarkParam_H(H_String_8BitCharType* Sign,H_String_Function_MarkParam* MarkParam){
  H_String_8BitCharType c;
  int MinNumberOfString;
  int NumberOfMantissa;
  int IsNegative;

  c=Sign[0];
  if((c=='D')||(c=='d')){
    MarkParam->Sign=Sign[1];
    Sign=&Sign[2];
    MarkParam->isD=-1;
  }else{
    MarkParam->Sign=c;
    Sign=&Sign[1];
    MarkParam->isD=0;
  }

  MinNumberOfString=0;
  IsNegative=0;
  if(Sign[0]=='-'){
    Sign=&Sign[1];
    IsNegative=-1;
  }
  c=Sign[0];
  while((c>='0')&&(c<='9')){
    MinNumberOfString*=10;
    MinNumberOfString+=(int)c-'0';
    Sign=&Sign[1];
    c=Sign[0];
  }
  if(IsNegative!=0){
    MinNumberOfString=-MinNumberOfString;
  }

  if(Sign[0]=='.'){
    Sign=&Sign[1];

    NumberOfMantissa=0;
    c=Sign[0];
    while((c>='0')&&(c<='9')){
      NumberOfMantissa*=10;
      NumberOfMantissa+=(int)c-'0';
      Sign=&Sign[1];
      c=Sign[0];
    }
  }else{
    NumberOfMantissa=6;
  }

  MarkParam->MinNumberOfString=MinNumberOfString;
  MarkParam->NumberOfMantissa=NumberOfMantissa;

}

//标准格式
static void GetMarkParam_Std(H_String_8BitCharType* Sign,H_String_Function_MarkParam* MarkParam){
  H_String_8BitCharType c;
  int MinNumberOfString;
  int NumberOfMantissa;
  int IsNegative;

  int L_cnt;

  MinNumberOfString=0;
  IsNegative=0;
  if(Sign[0]=='-'){
    Sign=&Sign[1];
    IsNegative=-1;
  }
  c=Sign[0];
  while((c>='0')&&(c<='9')){
    MinNumberOfString*=10;
    MinNumberOfString+=(int)c-'0';
    Sign=&Sign[1];
    c=Sign[0];
  }
  if(IsNegative!=0){
    MinNumberOfString=-MinNumberOfString;
  }

  if(Sign[0]=='.'){
    Sign=&Sign[1];

    NumberOfMantissa=0;
    c=Sign[0];
    while((c>='0')&&(c<='9')){
      NumberOfMantissa*=10;
      NumberOfMantissa+=(int)c-'0';
      Sign=&Sign[1];
      c=Sign[0];
    }
  }else{
    NumberOfMantissa=6;
  }

  MarkParam->MinNumberOfString=MinNumberOfString;
  MarkParam->NumberOfMantissa=NumberOfMantissa;

  L_cnt=0;
  c=Sign[0];
  while((c=='L')||(c=='l')){
    L_cnt++;
    Sign=&Sign[1];
    c=Sign[0];
  }

  if(L_cnt>1){
    MarkParam->isD=-1;
  }else{
    MarkParam->isD=0;
  }

  if((c=='D')||(c=='d')){
    MarkParam->Sign='I';
  }else{
    MarkParam->Sign=c;
  }

}




/**
 * @brief 格式化字符串到字节数组
 * @param Buffer 所使用的缓存
 * @param Format 格式
 * {i} {I} 整型(int) {iyyy} {Iyyy} yyy为10进制数字 表示最短长度(yyy长度任意 负数代表左对齐)
 * {X} {x} 16进制(unsigned int){xyyy} {Xyyy} yyy为10进制数字 表示最短长度(yyy长度任意) 
 * {Diyyy} {dxyyy} {i},{I},{x},{X}的长整型模式
 * {f} {F} 浮点数(double) {Fyyy.xxx} {fyyy.xxx} {F.xxx} {f.xxx} yyy为10进制数字, 为总字符最小长度,负数代表左对齐. xxx为10进制数字, 保留小数位数
 * {e} {E} 浮点数(double) 科学计数法 {eyyy.xxx} {Eyyy.xxx} yyy为10进制数字, 为总字符最小长度,负数代表左对齐. xxx为10进制数字, 保留小数位数
 * {s} {S} 以'\0'结尾的字符串
 * @param ... 参数
 * @return 生成的字符串长度, 这个长度不包括结束符
 */
int H_StringFormat(void* Buffer,H_String_8BitCharType* Format,...){
  va_list Params;

  H_String_Function_MarkParam MarkParam;
  int rSize;
  H_String_8BitCharType* sBuffer;
  H_String_8BitCharType* format;
  H_String_8BitCharType* sPtr;
  int sSize;
  H_String_8BitCharType c;


  va_start(Params,Format);

  sBuffer=Buffer;
  format=Format;
  
  rSize=0;
  while(format[0]!='\0'){

    //寻找参数标记
    while((format[0]!='{')&&(format[0]!='\0')){
      //复制一般的字符
      sBuffer[rSize]=format[0];
      format=&format[1];
      rSize++;
    }
    if(format[0]=='\0'){
      break;
    }else{
      format=&format[1];
    }
    //format已指向标记的第一个字符

    sPtr=format;

    //计算参数长度
    sSize=0;
    c=format[0];
    while((c!='}')&&(c!='\0')){
      sSize++;
      format=&format[1];
      c=format[0];
    }
    
    if(format[0]=='\0'){
      break;
    }else{
      format=&format[1];
    }

    if(sSize>0){

      GetMarkParam_H(sPtr,&MarkParam);
      c=MarkParam.Sign;
      if(String_Functions[(int)((H_String_8BitCharType)c)]!=NULL){
        rSize+=String_Functions[(int)((H_String_8BitCharType)c)](&sBuffer[rSize],&MarkParam,&Params);
      }

    }

  }
  va_end(Params);

  sBuffer[rSize]='\0';

  return rSize;
}

/**
 * @brief 格式化字符串到字节数组
 * @param Buffer 所使用的缓存
 * @param Format 格式 与sprintf()的格式化字符串一致
 * @param ... 参数
 * @return 生成的字符串长度, 这个长度不包括结束符
 */
int H_sprintf(void* Buffer,H_String_8BitCharType* Format,...){
  va_list Params;

  H_String_Function_MarkParam MarkParam;
  int rSize;
  H_String_8BitCharType* sBuffer;
  H_String_8BitCharType* format;
  H_String_8BitCharType* sPtr;
  int sSize;
  H_String_8BitCharType c;


  va_start(Params,Format);

  sBuffer=Buffer;
  format=Format;
  
  rSize=0;
  while(format[0]!='\0'){

    //寻找参数标记
    while((format[0]!='%')&&(format[0]!='\0')){
      //复制一般的字符
      sBuffer[rSize]=format[0];
      format=&format[1];
      rSize++;
    }
    if(format[0]=='\0'){
      break;
    }else{
      format=&format[1];
    }
    //format已指向标记的第一个字符

    sPtr=format;

    //计算参数长度
    sSize=1;
    c=format[0];
    while((String_Functions[(int)((H_String_8BitCharType)c)]==NULL)&&((c!='D')&&(c!='d'))&&(c!='\0')){
      sSize++;
      format=&format[1];
      c=format[0];
    }
    
    if(format[0]=='\0'){
      break;
    }else{
      format=&format[1];
    }

    if(sSize>0){

      GetMarkParam_Std(sPtr,&MarkParam);
      c=MarkParam.Sign;
      if(String_Functions[(int)((H_String_8BitCharType)c)]!=NULL){
        rSize+=String_Functions[(int)((H_String_8BitCharType)c)](&sBuffer[rSize],&MarkParam,&Params);
      }

    }

  }
  va_end(Params);

  sBuffer[rSize]='\0';

  return rSize;
}

/**
 * @brief 获取字符串长度
 * @param string 字符串
 * @return 字符串长度
 */
int H_StringGetLength(H_String_8BitCharType* Str){
  int rLen;

  rLen=0;
  while(Str[0]!='\0'){
    rLen++;
    Str=&Str[1];
  }
  return rLen;
}

/**
 * @brief 复制字符串到数组
 * @param dst 要复制到的数组
 * @param string 字符串
 * @return 无
 */
void H_StringCopy(H_String_ByteType* Dst,H_String_8BitCharType* Str){
  while(Str[0]!='\0'){
    Dst[0]=Str[0];
    Dst=&Dst[1];
    Str=&Str[1];
  }
  Dst[0]='\0';
}

/**
 * @brief UTF8解析方法 每次调用将获取到一个unicode 并且将字符串指针向后移动一个字符
 * @param StringPtr 指向一个字符串指针
 * @param Unicode 用于返回unicode
 * @return 如果不为0, 代表解析出错 一般由字符串不合法引发
 */
int H_UTF8_Decode(H_String_8BitCharType** StringPtr,H_String_16BitCharType* Unicode){

  H_String_8BitCharType* String;
  H_String_8BitCharType c;
  H_String_8BitCharType c1;
  H_String_8BitCharType c2;
  H_String_16BitCharType unicode;

  String=StringPtr[0];
  c=String[0];

  if((c&0x80U)!=0){
    if((c&0xC0U)==0xC0U){
      if((c&0x20U)==0x20U){
      
        if((c&0x10U)==0x10U){
          //不支持过长的UTF8(此时已经超过16Bit)
          String=&String[4];

          StringPtr[0]=String;
          return -1;
        }else{
          //1110 16Bit

          c1=String[1];
          if((c1&0xC0U)!=0x80U){
            //第二个字节错误
            String=&String[3];
            
            StringPtr[0]=String;
            return -2;
          }

          c2=String[2];
          if((c2&0xC0U)!=0x80U){
            //第三个字节错误
            String=&String[3];
            
            StringPtr[0]=String;
            return -3;
          }

          unicode=(((H_String_16BitCharType)c)&0x0FU)<<12;
          unicode|=(((H_String_16BitCharType)c1)&0x3FU)<<6;
          unicode|=c2&0x3FU;
          String=&String[3];
        }

      }else{
        //110 11Bit

        c1=String[1];

        if((c1&0xC0U)!=0x80U){
          //第二个字节错误
          String=&String[2];
          
          StringPtr[0]=String;
          return -4;
        }

        unicode=(((H_String_16BitCharType)c)&0x1FU)<<6;
        unicode|=c1&0x3FU;

        String=&String[2];
      }
    }else{
      //无效unicode字符
      String=&String[1];
      
      StringPtr[0]=String;
      return -5;
    }
  }else{
    //0 7bit
    unicode=c;

    String=&String[1];
  }

  Unicode[0]=unicode;
  StringPtr[0]=String;
  return 0;
}


