#include "key.h"

void Key_Init()
{
  rcu_periph_clock_enable(RCU_GPIOE);
  rcu_periph_clock_enable(RCU_AF);
  
  /*KEY1:PE14  KEY2:PE11  KEY3:PE13  KEY4:PE15*/
  gpio_init(GPIOE, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_11 | GPIO_PIN_3 | GPIO_PIN_14 | GPIO_PIN_15);
}

uint8_t Key_Scan()
{
  uint8_t key_left,key_right,key_enter,key_esc,Key_value_sum;
	uint8_t key_value;
	static uint8_t old_Key_value_sum;
  
  key_left = gpio_input_bit_get(GPIOE, GPIO_PIN_14);
  key_right = gpio_input_bit_get(GPIOE, GPIO_PIN_11);
  key_enter = gpio_input_bit_get(GPIOE, GPIO_PIN_13);
  key_esc = gpio_input_bit_get(GPIOE, GPIO_PIN_15);
  
  Key_value_sum = (key_left<<3) + (key_right<<2) + (key_enter<<1) + key_esc;
	if((Key_value_sum == old_Key_value_sum)&&(Key_value_sum != 0x0f))
	{
		switch(Key_value_sum)
		{
			case 0x07:
				key_value = 2;
				break;
			case 0x0b:
				key_value = 1;
				break;
			case 0x0d:
				key_value = 5;
				break;
			case 0x0e:
				key_value = 6;
				break;
			default:
				break;
		}
	}else{
		key_value = 0;
	}
	old_Key_value_sum = Key_value_sum;
 
  return key_value;
}


