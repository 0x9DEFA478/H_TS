/*
 * @Author: 0x9DEFA478
 * @Date: 2022-01-14 15:36:43
 * @LastEditTime: 2022-01-14 15:58:07
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
#ifdef __CC_ARM
#include <stdio.h>


#pragma import(__use_no_semihosting_swi)


struct __FILE{
  void* _keep;
};
FILE __stdout;
FILE __stdin;


int fputc(int c,FILE* f) {
  return 0;
}

int fgetc(FILE* f) {
  return '\n';
}

int ferror(FILE* f) {
  return EOF;
}

void _ttywrch(int c) {
  (void)c;
}

void _sys_exit(int return_code) {
  for(;;){
  }
}

#else

#include <stdio.h>

int fputc(int c,FILE* f) {
  return 0;
}

int fgetc(FILE* f) {
  return '\n';
}

int __backspace(FILE *stream){
  return 0;
}

void __aeabi_assert (const char *expr, const char *file, int line){

  for(;;){
  }
}

void _ttywrch(int c) {
  (void)c;
}

#endif