/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称：thread_comm.c
 * 文件标识：
 * 摘    要：控制信息结构体
 *
 * 当前版本：V1.0.0
 * 作    者：wilson
 * 完成日期：2012-10-09
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */
#include "thread_comm.h"
//#include "touch_screen/touch_screen_intercomm.h"

/*The declaration of mutex*/
pthread_mutex_t mutex_of_control_to_network_comm;/*the mutex between control thread and netwoek thread*/
pthread_mutex_t mutex_of_global_device_info;/*the mutex of global info struct*/

/*The declaration of semaphore*/
sem_t  sem_wakeup_bcu_mic_sample;
sem_t sem_demao;

#ifdef CONFIG_TEST_SND_IN_MULTI_THREAD
sem_t  sem_snd_card_self_test;
#endif

//控制线程与网络线程之间的通信结构体:主控线程-->网络线程
volatile threads_communication_type_t control_to_network_communication;

/*All device information*/
volatile global_device_info_state_t global_device_info_state =
{
		InitGlobalInfo,
		UpdateGlobalInfo,
		{{"0"}},
		{{"0"}},
		{{"0"}},
		{{"0"}}
};

/*Initial the mutex*/
void InitMutex()
{
	pthread_mutex_init(&mutex_of_control_to_network_comm,NULL);
	pthread_mutex_init(&mutex_of_global_device_info,NULL);
}

/*Initialize the semaphore*/
void InitSemaphore(sem_t *param_sem)
{
	sem_init(param_sem, 0, 0);
}

/*Initialize the destination of audio data*/
void InitAudioDataDestination(void)
{
	pthread_mutex_lock(&mutex_of_control_to_network_comm);
	strcpy((char *)control_to_network_communication.device_name,"");
	control_to_network_communication.device_no = 0;
	pthread_mutex_unlock(&mutex_of_control_to_network_comm);
}

/*Clear the destination of audio data*/
void ClearAudioDataDestination()
{
	InitAudioDataDestination();
}

/*Set the destination of audio data*/
void SetAudioDataDestination(char *device_name, unsigned char device_no)
{
	pthread_mutex_lock(&mutex_of_control_to_network_comm);
	strcpy((char *)control_to_network_communication.device_name,device_name);
	control_to_network_communication.device_no = device_no;
	pthread_mutex_unlock(&mutex_of_control_to_network_comm);
}

/*Initial all device information*/
void InitAllDeviceInfo()
{
	pthread_mutex_lock(&mutex_of_global_device_info);
	global_device_info_state.init();
	pthread_mutex_unlock(&mutex_of_global_device_info);
}

/*Update  device information*/
void UpdataDeviceInformation(send_infomation_t recv_package)
{
	pthread_mutex_lock(&mutex_of_global_device_info);
	global_device_info_state.setglobalinfo(recv_package);
	pthread_mutex_unlock(&mutex_of_global_device_info);
}

/*Initial the global device information struct*/
void InitGlobalInfo()///<modify by wilson 2014_01_03
{
	int i = 0;
	for(i = 1;i < CONFIG_BCU_NUM + 1;i ++)//bcu
	{
		global_device_info_state.device_bcu[i].devices_no = i;
		strcpy((char *)global_device_info_state.device_bcu[i].devices_name,"BCU");
		global_device_info_state.device_bcu[i].current_state_ann = ANN_IDLE;
		global_device_info_state.device_bcu[i].current_state_intercom = INTERCOM_IDLE;
		memset((void *)&(global_device_info_state.device_bcu[i].event_infomation_ann),0x00,sizeof(global_device_info_state.device_bcu[i].event_infomation_ann));
		memset((void *)&(global_device_info_state.device_bcu[i].event_infomation_intercom),0x00,sizeof(global_device_info_state.device_bcu[i].event_infomation_intercom));
	}
	for(i = 1;i < CONFIG_CCU_NUM + 1;i ++)//ccu
	{
		global_device_info_state.device_ccu[i].devices_no = i;
		strcpy((char *)global_device_info_state.device_ccu[i].devices_name,"CCU");
	}
	for(i = 1;i < CONFIG_EAMP_NUM + 1;i ++)//eamp
	{
		global_device_info_state.device_eamp[i].devices_no = i;
		strcpy((char *)global_device_info_state.device_eamp[i].devices_name,"EAMP");
		global_device_info_state.device_eamp[i].current_state = ANN_IDLE;
		memset((void *)&(global_device_info_state.device_eamp[i].event_infomation_ann),0x00,sizeof(global_device_info_state.device_eamp[i].event_infomation_ann));
	}
	for(i = 1;i < CONFIG_PECU_NUM + 1;i ++)//pcu
	{
		global_device_info_state.device_pcu[i].devices_no = i;
		strcpy((char *)global_device_info_state.device_pcu[i].devices_name,"PCU");
		global_device_info_state.device_pcu[i].current_state_intercom = INTERCOM_IDLE;
		memset((void *)&(global_device_info_state.device_pcu[i].event_infomation_intercom),0x00,sizeof(global_device_info_state.device_pcu[i].event_infomation_intercom));
	}
	ts_debug_print(("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"));
}

/*Update  global device information struct*/
void UpdateGlobalInfo(send_infomation_t receive_package)
{}

/*Adjust PCU request sequence*/
void AdjustPCURequestSequence(int param_request_sequence)
{
	ts_debug_print(("AdjustPCURequestSequence,param_request_sequence = %d\n",param_request_sequence));
	int i = 0;
	for(i = 1; i <= CONFIG_PECU_NUM;i ++)
	{
		ts_debug_print(("%d:%d,%d,%d\n",i,
				global_device_info_state.device_pcu[i].event_infomation_intercom.d2p_intercomm.d2p_intercomm_active,
				global_device_info_state.device_pcu[i].request_sequence,
				param_request_sequence	));
		if(global_device_info_state.device_pcu[i].event_infomation_intercom.d2p_intercomm.d2p_intercomm_active != 0 &&
		   global_device_info_state.device_pcu[i].request_sequence >= param_request_sequence &&
		   param_request_sequence != 0 )
		{
			global_device_info_state.device_pcu[i].request_sequence --;
		}

		/*if(global_device_info_state.device_pcu[i].event_infomation_intercom.d2p_intercomm.d2p_intercomm_active == 0)
		{
			global_device_info_state.device_pcu[i].request_sequence = 0;
		}*/
	}
	ts_debug_print(("PCU%d,global_request=%d\n",1,global_device_info_state.device_pcu[1].request_sequence));
	ts_debug_print(("PCU%d,global_request=%d\n",2,global_device_info_state.device_pcu[2].request_sequence));
	ts_debug_print(("PCU%d,global_request=%d\n",3,global_device_info_state.device_pcu[3].request_sequence));
}

/*Set the destination device.which is offer to network thread*/
void SetDSTDeviceNameAndNumber(char *device_name,int device_number)
{
	pthread_mutex_lock(&mutex_of_control_to_network_comm);
	control_to_network_communication.device_no = device_number;
	strcpy((char *)&control_to_network_communication.device_name,(char *)device_name);
	pthread_mutex_unlock(&mutex_of_control_to_network_comm);
}

/*Get the destination device's name and number*/
void DSTDeviceNameAndNumber(char *dst_device_name,int *dst_device_number)
{
	last_network_flag = network_flag;network_flag = 100;
	pthread_mutex_lock(&mutex_of_control_to_network_comm);
	last_network_flag = network_flag;network_flag = 101;
	*dst_device_number = control_to_network_communication.device_no;
	strcpy((char *)dst_device_name,(char *)control_to_network_communication.device_name);
	last_network_flag = network_flag;network_flag = 102;
	pthread_mutex_unlock(&mutex_of_control_to_network_comm);
	last_network_flag = network_flag;network_flag = 103;
}

/*Update global device information*/
void UpdataGlobalDeviceInfo(send_infomation_t received_information_backup)
{
	last_control_flag = control_flag;control_flag = 195;
	pthread_mutex_lock(&mutex_of_global_device_info);
	last_control_flag = control_flag;control_flag = 196;
	global_device_info_state.setglobalinfo(received_information_backup);
	pthread_mutex_unlock(&mutex_of_global_device_info);
	last_control_flag = control_flag;control_flag = 197;
	last_control_flag = control_flag;control_flag = 198;
}

void ClearGlobalDeviceInfo()
{
	pthread_mutex_lock(&mutex_of_global_device_info);

	global_device_info_state.init();

	pthread_mutex_unlock(&mutex_of_global_device_info);
}

/*Show pcu request*/
void ShowPCURequest()
{
	int i = 1;
	for(i = 1; i < 6; i ++)
	{
		debug_print(("PCU%d = %d,request_sequence = %d\n",i,global_device_info_state.device_pcu[i].event_infomation_intercom.d2p_intercomm.d2p_intercomm_active,
														   global_device_info_state.device_pcu[i].request_sequence));
		ts_debug_print(("PCU%d = %d,request_sequence = %d\n",i,global_device_info_state.device_pcu[i].event_infomation_intercom.d2p_intercomm.d2p_intercomm_active,
																   global_device_info_state.device_pcu[i].request_sequence));
	}
}

/*Copy global struct*/
void ScreenCopyGlobalDeviceInfo(global_device_info_state_t *global_device_info_state_backup)
{
	pthread_mutex_lock(&mutex_of_global_device_info);
	memcpy((void *)global_device_info_state_backup,(void *)&global_device_info_state,sizeof(global_device_info_state));
	pthread_mutex_unlock(&mutex_of_global_device_info);
}

/*Try Copy global struct*/
int TryScreenCopyGlobalDeviceInfo(global_device_info_state_t *global_device_info_state_backup)
{
	if ( pthread_mutex_trylock(&mutex_of_global_device_info) != 0 )
	{
	       return -1;
	}

	if( NULL != global_device_info_state_backup )
	{
		  memcpy((void *)global_device_info_state_backup, (void *)&global_device_info_state, sizeof(global_device_info_state));
	}
	pthread_mutex_unlock(&mutex_of_global_device_info);

	return 0;
}

/*BCU update PCU state*/
void BCUUpdataPCUDeviceInfo(send_infomation_t received_information_backup)
{
	int pcu_no;
	pthread_mutex_lock(&mutex_of_global_device_info);
	pcu_no = received_information_backup.event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no;
	global_device_info_state.device_pcu[pcu_no].event_infomation_intercom.d2p_intercomm.d2p_intercomm_active = 0;
	pthread_mutex_unlock(&mutex_of_global_device_info);
}
