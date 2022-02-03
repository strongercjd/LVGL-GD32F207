/*!
\file    main.c
\brief   led spark with systick, USART print and key example

\version 2015-07-15, V1.0.0, firmware for GD32F20x
\version 2017-06-05, V2.0.0, firmware for GD32F20x
\version 2018-10-31, V2.1.0, firmware for GD32F20x
\version 2020-09-30, V2.2.0, firmware for GD32F20x
*/

/*
Copyright (c) 2020, GigaDevice Semiconductor Inc.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this 
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors 
may be used to endorse or promote products derived from this software without 
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f20x.h"
#include <stdio.h>
#include "main.h"
#include "LCD_disp.h"
#include "key.h"
#include "lvgl_app.h"
void uart_init()
{    
  /* enable GPIO clock */
  rcu_periph_clock_enable(RCU_GPIOA);
  
  /* enable USART clock */
  rcu_periph_clock_enable(RCU_USART0);
  
  /* connect port to USARTx_Tx */
  gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
  
  /* connect port to USARTx_Rx */
  gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
  
  /* USART configure */
  usart_deinit(USART0);
  usart_baudrate_set(USART0, 115200U);
  usart_receive_config(USART0, USART_RECEIVE_ENABLE);
  usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
  usart_enable(USART0);
}
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
  usart_data_transmit(USART0, (uint8_t)ch);
  while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
  return ch;
}
/*!
\brief      main function
\param[in]  none
\param[out] none
\retval     none
*/
int main(void)
{
  /* enable the led clock */
  rcu_periph_clock_enable(RCU_GPIOE);
  /* configure led GPIO port */ 
  gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_4);
  SysTick_Config(SystemCoreClock / 1000);
  
  gpio_bit_reset(GPIOE,GPIO_PIN_4);
  LCD_Init();
  Key_Init();
  uart_init();
  printf("\r\n======================================================================");
  printf("\r\n=               (C) COPYRIGHT 2022                                   =");
  printf("\r\n=                                                                    =");
  printf("\r\n=                GD32F20x LVGVL                                      =");
  printf("\r\n=                                                                    =");
  printf("\r\n=                                           By Firefly               =");
  printf("\r\n======================================================================");
  printf("\r\n\r\n");
  lv_init(); //lvgl 系统初始化
  lv_port_disp_init(); //lvgl 显示接口初始化,放在 lv_init()的后面
  lv_port_indev_init(); //lvgl 输入接口初始化,放在 lv_init()的后面
  lvgl_app_create();
  while(1){
    gpio_bit_reset(GPIOE,GPIO_PIN_4);
    SysCtlDelay(100*(SystemCoreClock/3000));
    gpio_bit_set(GPIOE,GPIO_PIN_4);
    SysCtlDelay(100*(SystemCoreClock/3000));
  }
}

