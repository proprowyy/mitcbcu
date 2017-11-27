/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： bcu_ann.c
 * 文件标识：
 * 摘    要：The source file of bcu announcement
 *
 * 当前版本： V1.1.0
 * 作    者： wilson
 * 完成日期：2013-01-11
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */

#include "bcu_ann.h"

//#define NEW_ENV
//extern cyg_io_handle_t bcu_audio_handle;
#define SET_PA_AUDIO 1
#define SET_TALK_AUDIO 0

extern pthread_t thread_of_control;/*the handle of control thread*/
send_infomation_t recv_cmd_info_of_ann,recv_cmd_info_of_intercom;
unsigned char wherther_send_sync_signal = 0;//是否需要发送设备同步信号
state_process_t idle_ann_state =
{
		IdleAnnEnter,
		IdleAnnExit,
		IdleAnnProcess,
		ANN_IDLE
};

state_process_t mic_outer_3d5_ann_state =
{
		Outer3D5AnnEnter,
		Outer3D5AnnExit,
		Outer3D5AnnProcess,
		MIC_3D5_OUTER_EVENT
};

state_process_t auto_annoucement_state =
{
		AutoAnnEnter,
		AutoAnnExit,
		AutoAnnProcess,
		TMS_AUTO_ANN_EVENT
};

state_process_t live_annoucement_state =
{
		LiveAnnEnter,
		LiveAnnExit,
		LiveAnnProcess,
		LIVE_ANN_EVENT
};

state_process_t occ_state =
{
		OCCEnter,
		OCCExit,
		OCCProcess,
		OCC_EVENT
};

state_process_t emergency_annoucement_state =
{
		EmergAnnEnter,
		EmergAnnExit,
		EmergAnnProcess,
		EMERG_ANN_EVENT
};

state_process_t manual_annoucement_state =
{
		ManualAnnEnter,
		ManualAnnExit,
		ManualAnnProcess,
		MANUAL_ANN_EVENT
};

cyg_handle_t  counter_handle_sync;
cyg_handle_t alarm_handle_sync;
cyg_alarm alarm_object_sync;

void IdleAnnEnter(send_infomation_t *send_information_idle)
{
	StartOrBrokeBroadcastPcuRequestAlarmAudioData();
	debug_print(("I am idle ann enter\n"));

	bcu_state.bcu_info.current_state_ann = ANN_IDLE;
	bcu_state.bcu_active_ann_state->state_id  = ANN_IDLE;

	SendCmd(&send_information_idle,"EAMP",MUL_DST_NO);
}

void IdleAnnExit()
{
	debug_print(("I am idle ann exit\n"));

//	CloseSndCard(&bcu_audio_handle,"playback","finish");
}

void IdleAnnProcess(send_infomation_t *send_information_idle)
{
	SendCmd(&send_information_idle,"EAMP",MUL_DST_NO);
}

void AutoAnnEnter(send_infomation_t *send_information_tms_auto)
{
	debug_print(("I am auto ann enter\n"));

	SendCmd(&send_information_tms_auto,"EAMP",MUL_DST_NO);

	/*Start timer*/
//	StartAutoSimulateTimer();
}

void AutoAnnExit()
{
	debug_print(("I am auto ann exit\n"));

	/*Close timer*/
}

void AutoAnnProcess(send_infomation_t *send_information_tms_auto)
{
//	diag_printf("ann process\n");
	SendCmd(&send_information_tms_auto,"EAMP",MUL_DST_NO);
}

void LiveAnnEnter(send_infomation_t *send_information_live)
{
	SendCmd(&send_information_live,"EAMP",MUL_DST_NO);///<2013-12-3
	CloseMonitorSpeaker();
	bcu_state.live_button_state = 1;

	debug_print(("I am live ann enter\n"));
	diag_printf("I am live ann enter\n");
	CloseAudioSampleTimer();

	SetAudioDataDestination("EAMP",LIVE_DST_NO);

	bcu_state.mic_owner = ANNOUNCE;

	StartAudioSampleTimer();///<2013-10-24

	EnableLiveMonitorTimer();

	bcu_state.bcu_info.current_state_ann = LIVE_ANN_EVENT;

	CharBufferClear(bcu_state.audio_data_send_buffer_id);
	CharBufferClear(bcu_state.pending_buffer_id);
	CharBufferClear(bcu_state.audio_data_recv_buffer_id);
	CharBufferClear(bcu_state.local_audio_data_recv_buffer_id);
	MicAdPcmWavheader(bcu_state.local_audio_data_recv_buffer_id);

	///<BCU 音频输出到模拟总线
	bcu_audio_talk_pa_choose(2);

	ShowCurrentTime(3);
	bcu_state.bcu_have_the_initiative_of_cmu = 0;
	if(bcu_state.bcu_mcu_connect_state == 0 && bcu_state.bcu_cmu_connect_car_type != 3)
	{///3号线老车不支持PA
		bcu_state.bcu_have_the_initiative_of_cmu = 1;
		cyg_flag_setbits(&bcu_cmd_to_cmu ,0x1);
	}
	BCU_LED_BUTTON1_ON;
}

void LiveAnnExit()
{
	bcu_state.live_button_state = 0;
	diag_printf("I am live ann exit\n");

	DisableLiveMonitorTimer();

	bcu_state.mic_owner = NONE;

//	EndSaveAudioData(whether_save_live_audio_data,"LIVE");

	CloseAudioSampleTimer();

	ClearAllAudioDataBuffer();//清空发送音频buf

	ClearAudioDataDestination();

	///<BCU 音频输出到扬声器
	bcu_audio_talk_pa_choose(SET_TALK_AUDIO);

//	CloseSndCard(&bcu_audio_handle,"codec","finish");
	whether_have_begin_broadcast_alarm_audio_data = 0;

	if(bcu_state.bcu_have_the_initiative_of_cmu != 0  && bcu_state.bcu_cmu_connect_car_type != 3)
	{
		bcu_state.bcu_have_the_initiative_of_cmu = 0;
		cyg_flag_setbits(&bcu_cmd_to_cmu ,0x8);
	}
	bcu_state.bcu_mcu_connect_state = 0;
	OpenMonitorSpeaker();
	BCU_LED_BUTTON1_DIS;
}

void LiveAnnProcess(send_infomation_t *send_information_live)
{

	SendCmd(&send_information_live,"EAMP",MUL_DST_NO);

}
void OCCEnter(send_infomation_t *send_information_occ)
{
	debug_print(("I am occ ann enter\n"));
	bcu_state.bcu_info.current_state_ann = OCC_EVENT;

	AlarmTSToChangeScreen(21);

}
void OCCExit()
{
	debug_print(("I am occ ann exit\n"));
	bcu_state.bcu_info.current_state_ann = ANN_IDLE;

	AlarmTSToChangeScreen(22);

	cyg_thread_delay(100);
}
void OCCProcess(send_infomation_t *send_information)
{
//	SendCmd(&send_information,"EAMP",MUL_DST_NO);
}
void EmergAnnEnter(send_infomation_t *send_information_emerg)
{
	debug_print(("I am emerg ann enter\n"));

	memcpy((void *)&recv_cmd_info_of_ann,(void *)send_information_emerg,sizeof(recv_cmd_info_of_ann));
	send_information_emerg->event_info_ann.emerg_announce.emerg_begin_or_over = 0;
	RestartSYNCTimer();///<重置同步定时器
	SendCmd(&send_information_emerg,"EAMP",MUL_DST_NO);
}

void EmergAnnExit()
{
	debug_print(("I am emerg ann exit\n"));
	SendEmergExitToCCU();///<告知CCU,BCU退出Emerg
}

void EmergAnnProcess(send_infomation_t *send_information_emerg)
{
	send_infomation_t send_information_emerg_temp,*p_send_information_emerg_temp;
	p_send_information_emerg_temp = &send_information_emerg_temp;
	memcpy((void *)p_send_information_emerg_temp,(void *)send_information_emerg,sizeof(*send_information_emerg));
	SendCmd(&send_information_emerg,"EAMP",MUL_DST_NO);///<告知EAMP
	SendCmd(&p_send_information_emerg_temp,"CCU",MUL_DST_NO);///<告知CCU
}
void ManualAnnEnter(send_infomation_t *send_information_manual)
{
	debug_print(("I am manual ann enter\n"));

	memcpy((void *)&recv_cmd_info_of_ann,(void *)send_information_manual,sizeof(recv_cmd_info_of_ann));
	send_information_manual->event_info_ann.manual_annnounce.manual_begin_or_over = 0;
	RestartSYNCTimer();///<重置同步定时器
	SendCmd(&send_information_manual,"EAMP",MUL_DST_NO);///<2013-12-05
}
void ManualAnnExit()
{
	debug_print(("I am manual ann exit\n"));
}

void ManualAnnProcess(send_infomation_t *send_information_manual)
{
#ifdef TEST_FOR_SHENGYANG
	send_infomation_t pis_send_information;
	memcpy((void *)&pis_send_information,(void *)send_information_manual,sizeof(*send_information_manual));
	BcuNoticePisStationCode_Manual(bcu_state.send_info_to_pis,&pis_send_information);
#endif
	SendCmd(&send_information_manual,"EAMP",MUL_DST_NO);
}


void Outer3D5AnnEnter(send_infomation_t *send_information_live)
{

	debug_print(("I am outer 3d5 ann enter\n"));
	CloseAudioSampleTimer();///<关闭音频采样定时器

	AlarmTSToChangeScreen(19);


	/*音频输入切为外接3D5*/
	bcu_audio_in_ctrl(1);

	bcu_state.mic_owner = OCC_OR_3D5;
	SetAudioDataDestination("EAMP",LIVE_DST_NO);

//	OpenSndCard(&bcu_audio_handle,"codec");///<2013-11-29
	///<写入音频头
	MicAdPcmWavheader(bcu_state.audio_data_recv_buffer_id);

	///<音频输出到模拟总线
	bcu_audio_talk_pa_choose(SET_PA_AUDIO);

	StartAudioSampleTimer();///<2013-10-24

	bcu_state.bcu_info.current_state_ann = MIC_3D5_OUTER_EVENT;
	SendCmd(&send_information_live,"EAMP",MUL_DST_NO);///<2013-12-3
}

void Outer3D5AnnExit()
{
	debug_print(("I am outer 3d5 ann exit\n"));

	bcu_state.mic_owner = NONE;

	CloseAudioSampleTimer();
	bcu_state.bcu_info.current_state_ann = ANN_IDLE;


	AlarmTSToChangeScreen(20);
	if(bcu_state.tcmns_effective_flag == 1)
	{
		AlarmTSToChangeScreen(14);
	}
	/*音频输入切为PPT*/
	bcu_audio_in_ctrl(0);

	///<音频输出到BCU的扬声器
	bcu_audio_talk_pa_choose(SET_TALK_AUDIO);

	ClearAllAudioDataBuffer();//清空发送音频buf
//	ClearAudioDataDestination();

	whether_have_begin_broadcast_alarm_audio_data = 0;
	cyg_thread_delay(100);
}

void Outer3D5AnnProcess(send_infomation_t *send_information_live)
{

	SendCmd(&send_information_live,"EAMP",MUL_DST_NO);
}

void alarm_func_handle_sync(cyg_handle_t counter_handle, cyg_addrword_t data)
{
	wherther_send_sync_signal = 1;
	//sync_timer_flag++;
}

void CreateSYNCTimer()
{///<创建同步定时器
	cyg_clock_to_counter(cyg_real_time_clock(),&counter_handle_sync);
	cyg_alarm_create(counter_handle_sync,alarm_func_handle_sync,0,&alarm_handle_sync,&alarm_object_sync);
	cyg_alarm_initialize(alarm_handle_sync,cyg_current_time()+400,400);
	cyg_alarm_disable(alarm_handle_sync);
	diag_printf("CreateSYNCTimer\n");
}
void RestartSYNCTimer()
{///<重启同步定时器
	cyg_alarm_initialize(alarm_handle_sync,cyg_current_time()+400,400);
	cyg_alarm_disable(alarm_handle_sync);
	diag_printf("RestartSYNCTimer\n");
}
void CloseSYNCTimer()
{///<关闭同步定时器
	cyg_alarm_disable(alarm_handle_sync);
	diag_printf("CloseSYNCTimer\n");
}

void SendSYNCPackageToEamp()
{///<发送同步信息包给EAMP
	int ret = 0,write_counts = 0;
	network_send_package_t sync_info = {"\0",0};
	sync_info.dst_devices_no =  MUL_DST_NO;
	strcpy((char *)sync_info.dst_devices_name,"EAMP");
	sync_info.send_information.src_devices_no =  bcu_state.bcu_info.devices_no;
	strcpy((char *)sync_info.send_information.src_devices_name,bcu_state.bcu_info.devices_name);
	sync_info.send_information.event_type_ann = AUTO_EAMP_SYNC_SIGNAL;
	sync_info.send_information.event_type_intercom = INTERCOM_IDLE;
	last_control_flag = control_flag;control_flag = 192;
	do
	{
		ret = BlockBufferWrite(bcu_state.cmd_send_buffer_id,&sync_info,sizeof(sync_info));
		write_counts ++;
		if(ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(ret <= 0 && write_counts < 2);
	last_control_flag = control_flag;control_flag = 193;
}

void SendEmergExitToCCU()
{///<发送命令包给CCU，告知其EMERG
	int ret = 0,write_counts = 0;
	network_send_package_t sync_info = {"\0",0};
	sync_info.dst_devices_no =  MUL_DST_NO;
	strcpy((char *)sync_info.dst_devices_name,"CCU");
	sync_info.send_information.src_devices_no =  bcu_state.bcu_info.devices_no;
	strcpy((char *)sync_info.send_information.src_devices_name,bcu_state.bcu_info.devices_name);
	sync_info.send_information.event_type_ann = EMERG_ANN_EVENT;
	sync_info.send_information.event_type_intercom = INTERCOM_IDLE;
	sync_info.send_information.event_info_ann.emerg_announce.emerg_active = 0;
	sync_info.send_information.event_info_ann.emerg_announce.emerg_begin_or_over = 0;
	do
	{
		ret = BlockBufferWrite(bcu_state.cmd_send_buffer_id,&sync_info,sizeof(sync_info));
		write_counts ++;
		if(ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(ret <= 0 && write_counts < 2);
}

void DevSyncHandle()
{
	
	if(wherther_send_sync_signal == 1)
	{///<当前是否需要发送同步信号
		
		wherther_send_sync_signal = 0;
		SendSYNCPackageToEamp();
		CloseSYNCTimer();
		
	}
	
}

void Outer3D5Handle()
{
	if(bcu_state.this_bcu_is_active == 1 &&
//			bcu_state.tcmns_effective_flag == 0 &&
			BCUGetDataFromCCUState() == 3)
	{///<外接3D5触发与结束控制
		last_control_flag = control_flag;control_flag = 53;
		JudgeWhetherHaveOuter3D5Triger();
		last_control_flag = control_flag;control_flag = 54;
	}
	last_control_flag = control_flag;control_flag = 55;
}
