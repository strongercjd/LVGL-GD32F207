#include "LCD_disp.h"


/*120Mhz时钟时，当ulCount为1时，函数耗时3个时钟，延时=3*1/120us=1/40us*/
/*
SystemCoreClock=120000000

us级延时,延时n微秒
SysCtlDelay(n*(SystemCoreClock/3000000));

ms级延时,延时n毫秒
SysCtlDelay(n*(SystemCoreClock/3000));

m级延时,延时n秒
SysCtlDelay(n*(SystemCoreClock/3));
*/

#if defined   (__CC_ARM) /*!< ARM Compiler */
__asm void
SysCtlDelay(unsigned long ulCount)
{
  subs    r0, #1;
  bne     SysCtlDelay;
  bx      lr;
}
#elif defined ( __ICCARM__ ) /*!< IAR Compiler */
void
SysCtlDelay(unsigned long ulCount)
{
  __asm("    subs    r0, #1\n"
        "    bne.n   SysCtlDelay\n"
          "    bx      lr");
}

#elif defined (__GNUC__) /*!< GNU Compiler */
void __attribute__((naked))
SysCtlDelay(unsigned long ulCount)
{
  __asm("    subs    r0, #1\n"
        "    bne     SysCtlDelay\n"
          "    bx      lr");
}

#elif defined  (__TASKING__) /*!< TASKING Compiler */                           
/*无*/
#endif /* __CC_ARM */

/**
* @brief  初始化函数
* @param  无
* @retval 无
*/
void LCD_Init ( void )
{
  LMT028_GPIO_Config ();
  LMT028_EXMC_Config ();
  
  timer7_init();
  
  LMT028_Rst ();
  LMT028_REG_Config ();
}


void LMT028_GPIO_Config ()
{
  rcu_periph_clock_enable(RCU_EXMC);
  rcu_periph_clock_enable(RCU_GPIOC);
  rcu_periph_clock_enable(RCU_GPIOD);
  rcu_periph_clock_enable(RCU_GPIOE);
  rcu_periph_clock_enable(RCU_AF);
  
  /* 配置控制线
  * PD14  :D0
  * PD15  :D1
  * PD0   :D2
  * PD1   :D3
  * PE7   :D4
  * PE8   :D5
  * PE9   :D6
  * PE10  :D7
  */
  gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_14 | GPIO_PIN_15);
  gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
  
  /* 配置控制线
  * PD4-FSMC_NOE   :LCD-RD
  * PD5-FSMC_NWE   :LCD-WR
  * PD7-FSMC_NE1   :LCD-CS
  * PD11-FSMC_A16  :LCD-DC
  */
  gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_11);
  
  /* 配置LCD复位RST控制用脚 PC6*/     
  gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
  
  
  /* 配置LCD背光BK控制用脚 PC7*/  
  gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
//	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
//  gpio_bit_set(GPIOC,GPIO_PIN_7);
  
}


/**
* @brief  LCD  亮度设置
* @param  value：0~99  0是最暗，99是最亮
* @retval 无
*/
void set_bk_value(uint16_t value)
{
  timer_counter_value_config(TIMER7,value);
}


/**
* @brief  LCD  FSMC 模式配置
* @param  无
* @retval 无
*/
void LMT028_EXMC_Config ( void )
{
  exmc_norsram_timing_parameter_struct  norsram_timing_init_struct;
  exmc_norsram_parameter_struct  norsram_init_struct;
  
  rcu_periph_clock_enable(RCU_EXMC);
  
  /* Step 1 : configure NORSRAM timing registers --------------------------------*/
  norsram_timing_init_struct.asyn_address_setuptime = 2U;
  norsram_timing_init_struct.asyn_address_holdtime = 2U; 
  norsram_timing_init_struct.asyn_data_setuptime = 6U;
  norsram_timing_init_struct.bus_latency = 0U;
  norsram_timing_init_struct.syn_clk_division = 0U;
  norsram_timing_init_struct.syn_data_latency = 0U;
  norsram_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A; 

  
  /* step 2 : configure NORSRAM control registers ---------------------------------*/
  norsram_init_struct.norsram_region = EXMC_BANK0_NORSRAM_REGION0;
  norsram_init_struct.write_mode = EXMC_ASYN_WRITE;
  norsram_init_struct.extended_mode = DISABLE;
  norsram_init_struct.asyn_wait = DISABLE;
  norsram_init_struct.nwait_signal = DISABLE;
  norsram_init_struct.memory_write = ENABLE;
  norsram_init_struct.nwait_config = EXMC_NWAIT_CONFIG_BEFORE;
  norsram_init_struct.wrap_burst_mode = DISABLE;
  norsram_init_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;
  norsram_init_struct.burst_mode = DISABLE;
  norsram_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_8B;
  norsram_init_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;
  norsram_init_struct.address_data_mux = DISABLE;
  norsram_init_struct.read_write_timing = &norsram_timing_init_struct;
  norsram_init_struct.write_timing = &norsram_timing_init_struct;
  
  exmc_norsram_init(&norsram_init_struct);
  exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION0);
}


/**
* @brief  作为LCD的背光输出PWM.
* @param  None
* @retval None
*/
void timer7_init(void)
{
	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(RCU_TIMER7);

	timer_deinit(TIMER7);

	/* TIMER7 configuration */
	timer_initpara.prescaler         = (uint16_t) (SystemCoreClock / 100000) - 1;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = 100-1;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER7,&timer_initpara);

	/* CH1configuration in PWM mode1 */
	timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
	timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
	timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
	timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
	timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
	timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

	timer_channel_output_config(TIMER7,TIMER_CH_1,&timer_ocintpara);

	/* CH1 configuration in PWM mode1*/
	timer_channel_output_pulse_value_config(TIMER7,TIMER_CH_1,90);
	timer_channel_output_mode_config(TIMER7,TIMER_CH_1,TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER7,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);

	/*TIMER7 primary ouput function enable*/
	timer_primary_output_config(TIMER7,ENABLE);
	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER7);
	/* auto-reload preload enable */
	timer_enable(TIMER7);
}


/**
* @brief  LMT028 软件复位
* @param  无
* @retval 无
*/
void LMT028_Rst ( void )
{			
  gpio_bit_reset(GPIOC,GPIO_PIN_6);
  Delay1ms(80);				   
  	 	 
  gpio_bit_set(GPIOC,GPIO_PIN_6);
  Delay1ms(80);		
}


/**
* @brief 设置坐标
* @param  X/Y:起始坐标 
* @retval 无
*/
void set_coordinate(uint16_t x,uint16_t y )
{
  uint8_t x_l,x_h,y_l,y_h;
  
  x_l=(x&0xff);
  x_h=((x>>8)&0xff);
  y_l=(y&0xff);
  y_h=((y>>8)&0xff);
  
  LMT028_Write_Cmd(0x2a); 
  LMT028_Write_Data(x_h);
  LMT028_Write_Data(x_l);
  
  LMT028_Write_Cmd(0x2b); 
  LMT028_Write_Data(y_h);
  LMT028_Write_Data(y_l); 
}


/**
* @brief  初始化LMT028寄存器
* @param  无
* @retval 无
*/
void LMT028_REG_Config ( void )
{
  // LMT028_Rst();
  LMT028_Write_Cmd ( 0x11  );
  Delay1ms  (150);
  
  //--------------------------------ST7789S Frame rate setting----------------------------------//
  LMT028_Write_Cmd ( 0xB2  );
  LMT028_Write_Data ( 0x0C  );
  LMT028_Write_Data ( 0x0C  );
  LMT028_Write_Data ( 0x00 );
  LMT028_Write_Data ( 0x33 );
  LMT028_Write_Data ( 0x33 );
  LMT028_Write_Data ( 0xB7 );
  LMT028_Write_Data ( 0x35 );
  
  ////---------------------------------ST7789S Fmark setting--------------------------------------//
  //        
  //        LMT028_Write_Cmd ( 0xB3  );
  //	LMT028_Write_Data ( 0x00  );
  //	LMT028_Write_Data ( 0x0f  );
  //	LMT028_Write_Data ( 0x0f );
  //        
  ////        
  //        SysCtlDelay(500); 
  //        LMT028_Write_Cmd ( 0xC6  );
  //	LMT028_Write_Data ( 0x1f);
  //	
  //---------------------------------ST7789S Power setting--------------------------------------//
  SysCtlDelay(500); 
  LMT028_Write_Cmd ( 0xBB);
  LMT028_Write_Data ( 0x28 );
  
  SysCtlDelay(500); 
  // mx,my,bgr,rev,ss,mh,mv,gs        
  LMT028_Write_Cmd ( 0xC0  );
  LMT028_Write_Data ( 0x2C );
  
  LMT028_Write_Cmd ( 0xC2  );
  LMT028_Write_Data ( 0x0f );
  
  SysCtlDelay(500); 
  //VRHS
  LMT028_Write_Cmd ( 0xC3  );
  LMT028_Write_Data ( 0x25);
  
  SysCtlDelay(500); 
  //VDV SET    
  LMT028_Write_Cmd ( 0xC4  );
  LMT028_Write_Data ( 0x20);
  
  SysCtlDelay(500); 
  // FRCTR2    
  LMT028_Write_Cmd ( 0xC6 );
  LMT028_Write_Data ( 0x0f);
  
  SysCtlDelay(500); 
  //regsel2 
  LMT028_Write_Cmd (0xCA);
  LMT028_Write_Data (0x0F );
  
  SysCtlDelay(500); 
  //regsel1
  LMT028_Write_Cmd ( 0xC8);
  LMT028_Write_Data ( 0x08 );
  
  SysCtlDelay(500); 
  // CABC
  LMT028_Write_Cmd ( 0x55);
  LMT028_Write_Data ( 0x90 );
  
  SysCtlDelay(500); 
  //PWCTRL1
  LMT028_Write_Cmd ( 0xd0);
  LMT028_Write_Data ( 0xa4 );
  LMT028_Write_Data ( 0xa1);
  
  SysCtlDelay(500); 
  //--------------------------------ST7789S gamma setting---------------------------------------//
  LMT028_Write_Cmd(0xe0);
  LMT028_Write_Data(0xd0);
  LMT028_Write_Data(0x03);
  LMT028_Write_Data(0x08);
  LMT028_Write_Data(0x0b);
  LMT028_Write_Data(0x0f);
  LMT028_Write_Data(0x2c);
  LMT028_Write_Data(0x41);
  LMT028_Write_Data(0x54);
  LMT028_Write_Data(0x4e);
  LMT028_Write_Data(0x07);
  LMT028_Write_Data(0x0e);
  LMT028_Write_Data(0x0c);
  LMT028_Write_Data(0x1e);
  LMT028_Write_Data(0x23); 
  
  SysCtlDelay(500); 
  LMT028_Write_Cmd(0xe1);
  LMT028_Write_Data(0xd0);
  LMT028_Write_Data(0x03);
  LMT028_Write_Data(0x09);
  LMT028_Write_Data(0x0b);
  LMT028_Write_Data(0x0d);
  LMT028_Write_Data(0x19);
  LMT028_Write_Data(0x3c);
  LMT028_Write_Data(0x54);
  LMT028_Write_Data(0x4f);
  LMT028_Write_Data(0x0e);
  LMT028_Write_Data(0x1d);
  LMT028_Write_Data(0x1c);
  LMT028_Write_Data(0x20);
  LMT028_Write_Data(0x22); 
  
  SysCtlDelay(500); 
  //65k色，16bit
  LMT028_Write_Cmd(0x3a);
  LMT028_Write_Data(0x55);
  
  SysCtlDelay(500); 
  //设置: MY=1, MX=1, MV=1(RowColEx), ML=0(normal), RGB=0(RGB)
  LMT028_Write_Cmd(0x36);
  //LMT028_Write_Data(0x60);  //信号线在右边  从左上到右下
  LMT028_Write_Data(0x60);
  
  SysCtlDelay(500); 
  //开启撕裂影响线
  LMT028_Write_Cmd(0x35);
  LMT028_Write_Data(0x00);
  
  SysCtlDelay(500); 
  
  //显示开
  LMT028_Write_Cmd(0x29);
  
  SysCtlDelay(500);
	clear_lcd_area(0,0,320,240);  
}

/**
* @brief 设置窗体
* @param  X/Y:起始坐标   width：区域宽度  height;区域高度  
* @retval 无
*/

void set_windows(uint16_t X,uint16_t Y,uint16_t width,uint16_t height)
{
  uint8_t X_l,X_h,Y_l,Y_h;
  uint8_t width_l,width_h,height_l,height_h;
  
  X_l=(X&0xff);
  X_h=((X>>8)&0xff);
  Y_l=(Y&0xff);
  Y_h=((Y>>8)&0xff);
  
  width_l=((width+X)&0xff);
  width_h=(((width+X)>>8)&0xff);
  height_l=((height+Y)&0xff);
  height_h=(((height+Y)>>8)&0xff);
  
  // set column range as full screen
  LMT028_Write_Cmd(0x2a); 
  LMT028_Write_Data(X_h);
  LMT028_Write_Data(X_l); 
  LMT028_Write_Data(width_h);
  LMT028_Write_Data(width_l); 
  
  // set row range as full screen
  LMT028_Write_Cmd(0x2b); 
  LMT028_Write_Data(Y_h);
  LMT028_Write_Data(Y_l); 
  LMT028_Write_Data(height_h);
  LMT028_Write_Data(height_l);
}

/**
* @brief 清除LMT028区域数据
* @param  X/Y:起始坐标   width：区域宽度  height;区域高度 
* @retval 无
*/
void clear_lcd_area(uint16_t X,uint16_t Y,uint16_t width,uint16_t height)
{
  uint32_t dp_y;
  uint32_t num;
  set_windows(X,Y,width,height);
  LMT028_Write_Cmd(0x2c);  
  num = width * height;
  for(dp_y = 0;dp_y < num;dp_y++){
    LMT028_Write_Data(0x00);
    LMT028_Write_Data(0x00);
  }
}
/**
* @brief  向LMT028写入命令
* @param  usData :要写入的命令（表寄存器地址）
* @retval 无
*/
void LMT028_Write_Cmd ( uint8_t usCmd )
{
  LCD->LCD_CMD=usCmd;
}


/**
* @brief  向LMT028写入数据
* @param  usData :要写入数据（表寄存器地址）
* @retval 无
*/	
void LMT028_Write_Data ( uint8_t usData )
{
  LCD->LCD_DATA = usData;
}


/*!
* @brief	画点
* @param 	x     坐标x
* @param 	y     坐标y
* @param        color 点的颜色
* @return	NONE
* @note	       NONE
*/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
  uint8_t color_h,color_l;
  
  color_h=(color>>8)&0xff;
  color_l=color&0xff;
  
  set_coordinate(x,y);//设置光标位置
  
  LMT028_Write_Cmd(0x2c); 
  LMT028_Write_Data(color_h);
  LMT028_Write_Data(color_l);
}

/**
* @brief  延时1us
* @param  usData :要写入的命令（表寄存器地址）
* @retval 无
*/

void  Delay_1us(uint16_t k){
  uint32_t i;
  for (i=0;i<1*k;i++){
    SysCtlDelay(120000000/3300000);  //1us
  }
}


/**
* @brief  延时1ms
* @param  usData :要写入的命令（表寄存器地址）
* @retval 无
*/

void  Delay1ms(uint16_t k)
{
  uint32_t i;
  for (i=0;i<k;i++){
    Delay_1us(1000);  //1us
  }
}


//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
  uint16_t height,width;
  uint16_t i,j;
  width=ex-sx+1; 			//得到填充的宽度
  height=ey-sy+1;			//高度
  for(i=0;i<height;i++)
  {
    for(j=0;j<width;j++)
      LCD_DrawPoint(sx+j,sy+i,color[i*width+j]);//画点
  }		  
}

