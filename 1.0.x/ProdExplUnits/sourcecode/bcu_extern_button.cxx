/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： bcu_extern_button.c
 * 文件标识：
 * 摘    要： BCU extern button control function
 *
 * 当前版本： V1.0.0
 * 作    者： wilson
 * 完成日期：2013-12-30
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */
#include "bcu_extern_button.h"

unsigned char whether_live_button_is_pressed = 0;//口播按钮是否按下
unsigned char whether_d2p_button_is_pressed = 0;//紧急对讲按钮是否按下
unsigned char whether_d2d_button_is_pressed = 0;//司机对讲按钮是否按下
unsigned char whether_monitor_button_is_pressed = 0;//监听按钮是否按下
unsigned char ppt_state_flag = 0;
unsigned char pa_state_flag = 0;
unsigned char pc_state_flag = 0;
unsigned char cc_state_flag = 0;
unsigned char mr_state_flag = 0;
unsigned char out_3d5_state_flag = 0;
extern int last_control_flag;
extern int control_flag;
extern int last_sample_flag;
extern int sample_flag;
extern int thread_change_flag;
extern int last_thread_change_flag;
extern int network_flag;
extern int last_network_flag;


int read_cc_key()
{
	
	
	int __ret;
	__ret=hal_gpio_get_pin(KEY_3_CC_BUTTON_DOWN);
	
} 

int GetLiveExternButtonState()
{///<获取口播按钮状态
	int ret = 3;
	if(pa_state_flag >= 2 && whether_live_button_is_pressed == 0)
	{
		whether_live_button_is_pressed = 1;
		ret = 1;
	}
	else if(pa_state_flag < 2 && whether_live_button_is_pressed == 1)
	{
		
		whether_live_button_is_pressed = 0;
		ret=0;
	}
	return ret;
}

int GetMRExternButtonState()
{///<获取MS外部按钮状态
	int ret = 0;
	if(mr_state_flag >= 2 && whether_monitor_button_is_pressed == 0)
	{
		whether_monitor_button_is_pressed = 1;
		ret = 1;
	}
	else if(mr_state_flag < 2 && whether_monitor_button_is_pressed == 1)
	{
		whether_monitor_button_is_pressed = 0;
	}
	return ret;
}


int GetD2PExternButtonState()
{///<获取D2P外部按钮状态
	int ret = 0;
	if(pc_state_flag >= 2 && whether_d2p_button_is_pressed == 0)
	{
		whether_d2p_button_is_pressed = 1;
		ret = 1;
	}
	else if(pc_state_flag < 2 && whether_d2p_button_is_pressed == 1)
	{
		whether_d2p_button_is_pressed = 0;
	}
	return ret;
}

int GetD2DExternButtonState()
{///<获取D2D外部按钮状态
	int ret = 0;
	if(cc_state_flag >= 2 && whether_d2d_button_is_pressed == 0)
	{
		whether_d2d_button_is_pressed = 1;
		ret = 1;
	}
	else if(cc_state_flag < 2 && whether_d2d_button_is_pressed == 1)
	{
		whether_d2d_button_is_pressed = 0;
	}
	return ret;
}

int GetPTTState()
{
	int ret = 0;
	diag_printf("ppt pass flag=%d\n",ppt_state_flag);
	if(ppt_state_flag >= 2)
	{
		ret =  1;
	}
	return ret;
}

void GetPAAllOuterButtonState()
{

	if(bcu_read_key_1_status() == 0)//pa 按下
	{
		if(pa_state_flag >= 100)
		{
			pa_state_flag = 100;
		}
		else
		{
			pa_state_flag++;
		}
	}
	else
	{
		pa_state_flag = 0;
	}

	if(bcu_read_key_2_status() == 0)//pc 按下
	{
		diag_printf("pc pass\n");
		if(pc_state_flag >= 100)
		{
			pc_state_flag = 100;
		}
		else
		{
			pc_state_flag++;
		}
	}
	else
	{
		pc_state_flag = 0;
	}
	last_control_flag = control_flag;control_flag = 188;
	if(read_cc_key() == 0)//cc 按下
	{
		if(cc_state_flag >= 100)
		{
			cc_state_flag = 100;
		}
		else
		{
			cc_state_flag++;
		}
	}
	else
	{
		cc_state_flag = 0;
	}
	if(bcu_read_key_4_status() == 0)//mt 按下
	{
		if(mr_state_flag >= 100)
		{
			mr_state_flag = 100;
		}
		else
		{
			mr_state_flag++;
		}
	}
	else
	{
		mr_state_flag = 0;
	}
	if(bcu_read_hd_status() == 1)
	{
		if(out_3d5_state_flag >= 100)
		{
			out_3d5_state_flag = 100;
		}
		else
		{
			out_3d5_state_flag ++;
		}
	}
	else
	{
		out_3d5_state_flag = 0;
	}
	last_control_flag = control_flag;control_flag = 190;
}
void GetPAAllOuterButtonState2()
{
	if(hal_gpio_get_pin( CYGHWR_HAL_KINETIS_PIN(A, 24, 1, KINETIS_PIN_PULLUP) ) == 0)//mic ppt
		{

			if(ppt_state_flag >= 100)
			{
				diag_printf("PPT =%d\n",ppt_state_flag);
				ppt_state_flag = 100;
			}
			else
			{
				ppt_state_flag++;
			}
		}
		else
		{
			ppt_state_flag = 0;
		}


	last_sample_flag = sample_flag;sample_flag = 5;
	if(bcu_read_key_1_status() == 0)//pa 按下
	{
		if(pa_state_flag >= 100)
		{
			pa_state_flag = 100;
		}
		else
		{
			pa_state_flag++;
		}
	}
	else
	{
		pa_state_flag = 0;
	}
	last_sample_flag = sample_flag;sample_flag = 6;
	if(bcu_read_key_2_status() == 0)//pc 按下
	{
		if(pc_state_flag >= 100)
		{
			pc_state_flag = 100;
		}
		else
		{
			pc_state_flag++;
		}
	}
	else
	{
		pc_state_flag = 0;
	}
	last_sample_flag = sample_flag;sample_flag = 7;
	if(read_cc_key() == 0)//cc 按下
	{
		if(cc_state_flag >= 100)
		{
			cc_state_flag = 100;
		}
		else
		{
			cc_state_flag++;
		}
	}
	else
	{
		cc_state_flag = 0;
	}
	if(bcu_read_key_4_status() == 0)//mt 按下
	{
		if(mr_state_flag >= 100)
		{
			mr_state_flag = 100;
		}
		else
		{
			mr_state_flag++;
		}
	}
	else
	{
		mr_state_flag = 0;
	}
	if(bcu_read_hd_status() == 1)
	{
		if(out_3d5_state_flag >= 100)
		{
			out_3d5_state_flag = 100;
		}
		else
		{
			out_3d5_state_flag ++;
		}
	}
	else
	{
		out_3d5_state_flag = 0;
	}
	last_sample_flag = sample_flag;sample_flag = 9;
}
