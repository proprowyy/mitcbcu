/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称：bcu_function.c
 * 文件标识：
 * 摘    要：bcu处理的函数的库函数
 *
 * 当前版本：V1.0.0
 * 作    者：wilson
 * 完成日期：2012-10-24
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 * */

#include "bcu_function.h"
int current_is_being_d2d = 0;
unsigned char whether_show_pcu_info = 0;
unsigned char whether_updata_db = 0;
extern pthread_t thread_of_control;
extern pthread_t thread_of_screen;
unsigned char device_recv_failure[9];
unsigned char device_failure[9];
/*----------------begin of bcu beta 1.1----------------------*/
/*PPT state information*/
common_package_t network_send_package_ptt_release_to_bcu;
common_package_t network_send_package_ptt_press_to_bcu;
common_package_t network_send_package_ptt_release_to_pcu;
common_package_t network_send_package_ptt_press_to_pcu;

common_big_package_t bcu_send_log_info_to_ccu;
int last_control_flag = 0;
int control_flag = 0;
int last_sample_flag = 0;
int sample_flag = 0;
int thread_change_flag = 0;
int last_thread_change_flag = 0;
int network_flag = 0;
int last_network_flag = 0;
unsigned char snd_timer_flag = 0;
unsigned char deamon_timer_flag = 0;
unsigned char key_timer_flag = 0;
unsigned char sync_timer_flag = 0;
unsigned char network_count = 0;


///<BCU发送给其他设备的命令包
common_package_t dev_vol_to_eamp,dev_vol_to_pcu,dev_vol_to_bcu,recv_dev_col_bcu,dev_vol_to_ccu;

int package_sequence = 0;
int ntwork_thread_prio = 0;//网络线程优先级
char *blank_audio_data = NULL;//空白数据首地址
int whether_could_be_finished_d2d = 0;//是否结束司机对讲
int whether_eant_to_delay_finished_d2d = 0;//判断是否延时结束司机对讲
int whether_have_begin_broadcast_alarm_audio_data = 0;//是否已经开始播放PCU请求提示音
unsigned char whether_get_out3d5_button_state=0;//是否获取外接3D5按钮状态
unsigned char last_out3d5_button_state=0;//上一次3D5状态
unsigned char whether_ask_key_info = 0;//是否发送钥匙信息查询包
unsigned char whether_send_to_pcu = 0;//定时发送信息包给PCU，解决长久PCU没有网络通信，首次PCU请求无法接通问题

char ts_key_info_buffer[30] = "请 插 入 钥 匙";

/*司机对讲延迟定时器，目前尚未使用*/
cyg_handle_t  counter_handle_delay_d2d;
cyg_handle_t alarm_handle_delay_d2d;
cyg_alarm alarm_object_delay_d2d;

/*钥匙信号查询定时器*/
cyg_handle_t  counter_handle_key_info;
cyg_handle_t alarm_handle_key_info;
cyg_alarm alarm_object_key_info;

/*外接3D5检测定时器，目前尚未使用*/
cyg_handle_t  counter_handle_get_out3d5_button_state;
cyg_handle_t alarm_handle_get_out3d5_button_state;
cyg_alarm alarm_object_get_out3d5_button_state;

int bcu_test_for_ts = 0;



station_info_t station[2][32+4] =
{
	 {   //station code of line 3
		   {0,"\0"},
		   {0,"太平园",0,3,3},
		   {0,"簇锦",0,3,3},
		   {0,"华兴",0,3,3},
		   {1,"金花",0,3,3},
		   {0,"空港一站",0,3,3},
		   {0,"空港二站",0,3,3},
		   {1,"延安西路",0,3,3},
		   {2,"中山公园",0,3,3},
		   {0,"金沙江路",0,3,3},
		   {1,"曹杨路",0,3,3},
		   {0,"镇坪路",0,3,3},
		   {0,"中潭路",0,3,3},
		   {0,"上海火车站",0,3,3},
		   {0,"宝山路",0,3,3},
		   {0,"东宝兴路",0,3,3},
		   {0,"虹口足球场",0,3,3},
		   {0,"赤峰路",0,3,3},
		   {0,"大柏树",0,3,3},
		   {0,"江湾镇",0,3,3},
		   {0,"殷高西路",0,3,3},
		   {0,"长江南路",0,3,3},
		   {0,"淞发路",0,3,3},
		   {0,"张华浜",0,3,3},
		   {0,"淞滨路",0,3,3},
		   {1,"水产路",0,3,3},
		   {0,"宝杨路",0,3,3},
		   {0,"友谊路",0,3,3},
		   {0,"铁力路",0,3,3},
		   {0,"江杨北路",0,3,3}
	 },
	 {  //station code of line 4
		   {0,"\0"},
		   {0,"火车北站",0,3,3}, ///< 1
		   {0,"驷马桥",0,3,3},  ///< 2
		   {0,"府青路",0,3,3},  ///< 3
		   {0,"八里庄",0,3,3},  ///< 4
		   {0,"二仙桥",0,3,3},  ///< 5
		   {0,"曹杨路",0,3,3}, ///< 6
		   {0,"镇坪路",0,3,3}, ///< 7
		   {0,"中潭路",0,3,3}, ///< 8
		   {0,"上海火车站",0,3}, ///< 9
		   {0,"宝山路",0,3,3}, ///< 10
		   {0,"海伦路",0,3,3}, ///< 11
		   {0,"临平路",0,3,3}, ///< 12
		   {0,"大连路",0,3,3}, ///< 13
		   {0,"杨树浦路",0,3,3}, ///< 14
		   {0,"浦东大道",0,3,3}, ///< 15
		   {0,"世纪大道",0,3,3}, ///< 16
		   {0,"浦电路",0,3,3}, ///< 17
		   {0,"蓝村路",0,3,3}, ///< 18
		   {0,"塘桥",0,3,3}, ///< 19
		   {0,"南浦大桥",0,3,3}, ///< 20
		   {0,"西藏南路",0,3,3}, ///< 21
		   {0,"鲁班路",0,3,3}, ///< 22
		   {0,"大木桥路",0,3,3}, ///< 23
		   {0,"东安路",0,3,3}, ///< 24
		   {0,"上海体育场",0,3,3}, ///< 25
		   {0,"上海体育馆",0,3,3}, ///< 26
		   {0,"南浦大桥",0,3,3}, ///< 27
		   {0,"西藏南路",0,3,3}, ///< 28
		   {0,"鲁班路",0,3,3}, ///< 29
		   {0,"大木桥路",0,3,3}, ///< 30
		   {0,"东安路",0,3,3}, ///< 31
		   {0,"东安路",0,3,3}, ///< 32
	 }
};

/*station information*/
station_information_t station_information[2][MAX_STATION_NUMBER] =
{
	{
 		 {0,0,0,"",""},
		 {4,1,1,"火车北站", "North Railway Station"},
		 {4,2,1,"驷马桥", "Simaqiao"},
		 {4,3,1,"府青路", "Fuqing Road"},
		 {4,4,1,"八里庄", "Balizhuang"},
		 {4,5,1,"二仙桥", "Erxianqiao Bridge"},
		 {4,6,1,"理工大学", "Chengdu University of Technology"},
		 {4,7,1,"崔家店", "Cuijiadian"},
		 {4,8,1,"双店路", "Shuangdian Road"},
		 {4,9,1,"槐树店", "Huaishudian"},
		 {4,10,1,"迎晖路", "Yinhui Road"},
		 {4,11,1,"成都东客站", "East Chengdu Railway Station"},
		 {4,12,1,"大观", "Daguan"},
		 {4,13,1,"狮子山", "Shizishan"},
		 {4,14,1,"四川师大", "Sichuan Normal University"},
		 {4,15,1,"琉璃场", "Liulichang"},
		 {4,16,1,"三瓦窑", "Sanwayao"},
		 {4,17,1,"火车南站", "South Railway Station"},
		 {4,18,1,"神仙树", "Shenxianshu"},
		 {4,19,1,"高朋大道", "Gaopeng Avenue"},
		 {4,20,1,"太平园", "Taipingyuan"},
		 {4,21,1,"武侯大道", "Wuhou Avenue"},
		 {4,22,1,"龙爪堰", "Longzhuayan"},
		 {4,23,1,"东坡路", "Dongpo Road"},
		 {4,24,1,"文化宫", "Culture Palace"},
		 {4,25,1,"金沙博物馆", "Jinsha Site Museum"},
		 {4,26,1,"一品天下", "Yipintianxia"},
		 {4,27,1,"茶店子", "Chadianzi"},
		 {4,28,1,"花照壁", "Huazhaobi"},
		 {4,29,1,"西南交大", "Southwest Jiaotong University"},
		 {4,30,1,"九里堤", "Jiulidi"},
		 {4,31,1,"北站西二路", "2nd Beizhan Road West"}
	},
	{
		 {0,0,0,"",""},
		 {4,1,1,"火车北站", "North Railway Station"},
		 {4,2,1,"驷马桥", "Simaqiao"},
		 {4,3,1,"府青路", "Fuqing Road"},
		 {4,4,1,"八里庄", "Balizhuang"},
		 {4,5,1,"二仙桥", "Erxianqiao Bridge"},
		 {4,6,1,"理工大学", "Chengdu University of Technology"},
		 {4,7,1,"崔家店", "Cuijiadian"},
		 {4,8,1,"双店路", "Shuangdian Road"},
		 {4,9,1,"槐树店", "Huaishudian"},
		 {4,10,1,"迎晖路", "Yinhui Road"},
		 {4,11,1,"成都东客站", "East Chengdu Railway Station"},
		 {4,12,1,"大观", "Daguan"},
		 {4,13,1,"狮子山", "Shizishan"},
		 {4,14,1,"四川师大", "Sichuan Normal University"},
		 {4,15,1,"琉璃场", "Liulichang"},
		 {4,16,1,"三瓦窑", "Sanwayao"},
		 {4,17,1,"火车南站", "South Railway Station"},
		 {4,18,1,"神仙树", "Shenxianshu"},
		 {4,19,1,"高朋大道", "Gaopeng Avenue"},
		 {4,20,1,"太平园", "Taipingyuan"},
		 {4,21,1,"武侯大道", "Wuhou Avenue"},
		 {4,22,1,"龙爪堰", "Longzhuayan"},
		 {4,23,1,"东坡路", "Dongpo Road"},
		 {4,24,1,"文化宫", "Culture Palace"},
		 {4,25,1,"金沙博物馆", "Jinsha Site Museum"},
		 {4,26,1,"一品天下", "Yipintianxia"},
		 {4,27,1,"茶店子", "Chadianzi"},
		 {4,28,1,"花照壁", "Huazhaobi"},
		 {4,29,1,"西南交大", "Southwest Jiaotong University"},
		 {4,30,1,"九里堤", "Jiulidi"},
		 {4,31,1,"北站西二路", "2nd Beizhan Road West"}
	}
};


bcu_state_t bcu_state =
{
	BCURuleInit,
	BCUAddRule,
	BCURemoveRule,
	BCUDisableRule,
	BCUEnableRule,
	{0},
	{0},
	{{{0}}},
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};


/*station information*/
station_information_t station_information1[2][MAX_STATION_NUMBER];///<这个只用作越战信息了，有点浪费

/*Save the audio data*/
char src_device_info[10] = "\0";
char this_device_info[10] = "\0";
char audio_data_log_file_name[60] = "\0";
FILE *fp_audio_data_log_file = NULL;
unsigned char whether_save_live_audio_data = 0;
unsigned char whether_save_d2d_audio_data = 0;
unsigned char whether_save_d2p_audio_data = 0;

void InitBCUState()
{
	  bcu_state.pp_skip_station_info = station_information;
	  bcu_state.system_language = 0;
	  bcu_state.live_button_state = 0;
	  bcu_state.d2d_button_state = 0;
	  bcu_state.d2p_button_state = 0;
	  bcu_state.tcmns_effective_flag = 1;
	  bcu_state.bcu_mcu_connect_state = 0;
	  bcu_state.bcu_cmu_connect_car_type = 0;//3:old3 4:old 4 0:new
	  bcu_state.bcu_have_the_initiative_of_cmu = 0;
	  bcu_state.bcu_current_tcms_cmu_state = 0;
	  bcu_state.ts_current_state = 1;
	  int i = 0;
	  for(i = 0;i < 2;i++)
	  {
		  bcu_state.dev_falut_info.bcu_info[i].online = 1;
		  bcu_state.dev_falut_last_info.bcu_info[i].online = 1;
	  }
	  for(i = 0;i < 2;i++)
	  {
		  bcu_state.dev_falut_info.ccu_info[i].online = 1;
		  bcu_state.dev_falut_last_info.ccu_info[i].online = 1;
		  bcu_state.dev_falut_info.ccu_info[i].ccu_per_dev_failure_info.fdu_failure_info = 0;
		  bcu_state.dev_falut_last_info.ccu_info[i].ccu_per_dev_failure_info.fdu_failure_info = 0;
	  }
	  for(i = 0;i < 12;i++)
	  {
		  bcu_state.dev_falut_info.pcu_info[i].online = 1;
		  bcu_state.dev_falut_last_info.pcu_info[i].online = 1;
	  }
	  for(i = 0;i < 6;i++)
	  {
		  bcu_state.dev_falut_info.eamp_info[i].online = 1;
		  bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.dm_failure_info = 0;
		  bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led1_failure_info = 0;
		  bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led2_failure_info = 0;
		  bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.nd1_failure_info = 0;
		  bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.nd2_failure_info = 0;
		  bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led1_failure_info = 0;
		  bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led2_failure_info = 0;

		  bcu_state.dev_falut_last_info.eamp_info[i].online = 1;
		  bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.dm_failure_info = 0;
		  bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led1_failure_info = 0;
		  bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led2_failure_info = 0;
		  bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.nd1_failure_info = 0;
		  bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.nd2_failure_info = 0;
		  bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led1_failure_info = 0;
		  bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led2_failure_info = 0;
	  }

	  for(i = 0;i < 12;i++)
	  {
		  bcu_state.pcu_request_info_without_ts[i] = 0;
	  }
	  bcu_state.device_volume.intercomm_volume = 4;
	  bcu_state.device_volume.d2d_volume = 3;
}

int BcuGetNetTxCmdBuffId(void)
{
     return bcu_state.cmd_send_buffer_id;
}

int InitBuffer(void)
{
	int network_send_audio_buffer;
	int network_receive_audio_buffer;
	int network_receive_control_buffer;//网络线程接受控制线程的buffer
	int network_send_control_buffer;//网络线程发送控制线程的buffer
	int control_receive_cmd_from_screen;//主控线程与触摸屏线程之间的buffer
	int send_info_to_pis_buffer; //发送控制信息给PIS
	int recv_info_to_pis_buffer; //接受控制信息给PIS
	int local_audio_data_buffer;
	int alarm_audio_data_buffer;
	int ptt_state_send_buffer_id;
	int ptt_state_recv_buffer_id;
	int network_udp_common_send_buffer;
	int network_udp_common_recv_buffer;
	int control_recv_ts_buffer;
	int dev_vol_info_buffer;
	int network_send_shell_buffer;
	int network_recv_shell_buffer;


    int gd_net_cmd_rx_buffer_id = BlockBufferCreate(8, sizeof(common_big_package_t), "bcu-gd-net-cmd-Rx"); ///< 8 packages: 4K Bytes
	if(0 == gd_net_cmd_rx_buffer_id )
	{
		return -1;
	}
	control_recv_ts_buffer = BlockBufferCreate(2, sizeof(ts_dev_volume_info_t), "bcu-control-recv-ts-info");
	if (0 == control_recv_ts_buffer )
	{
		return -1;
	}


	network_send_audio_buffer = CharBufferCreate( (DEFAULT_AUDIO_BUFFER_SIZE),"bcu-network-audio-tx");//用于音频发送
	if (0 == network_send_audio_buffer )
	{
		return -1;
	}
	CharBufferCreate((3 * 1024 + 64),"bcu-network-audio-pendig");

	network_receive_audio_buffer = CharBufferCreate((DEFAULT_AUDIO_BUFFER_SIZE + 3 * 1024),"bcu-network-audio-rx"); //用于音频接收(对讲)
	network_receive_audio_buffer = CharBufferCreate((DEFAULT_AUDIO_BUFFER_SIZE + 3 * 1024),"bcu-local-audio-rx"); //用于音频接收(对讲)
	if ( 0 == network_receive_audio_buffer )
	{
		 CharBufferDelete(network_send_audio_buffer);
		 return -1;
	}

	network_receive_control_buffer = BlockBufferCreate(DEFAULT_BUFFER_TOTAL_BLOCKS, sizeof(network_send_package_t), "bcu-network-cmd-rx"); //用于网络接收响应,块缓冲
	if ( 0 == network_receive_control_buffer )
	{
	   CharBufferDelete(network_send_audio_buffer);
	   CharBufferDelete(network_receive_audio_buffer);
	   return -1;
	}

	network_send_control_buffer = BlockBufferCreate(5, sizeof(network_send_package_t),"bcu-network-cmd-tx"); //用于主控线程接收响应,块缓冲
	if ( 0 == network_send_control_buffer )
	{
	   CharBufferDelete(network_send_audio_buffer);
	   CharBufferDelete(network_receive_audio_buffer);
	   BlockBufferDelete(network_receive_control_buffer);
	   return -1;
	}

	control_receive_cmd_from_screen = BlockBufferCreate(5, sizeof(send_infomation_t),"bcu-screen-cmd-tx"); //用于接收响应,块缓冲
	if ( 0 == control_receive_cmd_from_screen )
	{
	   CharBufferDelete(network_send_audio_buffer);
	   CharBufferDelete(network_receive_audio_buffer);
	   BlockBufferDelete(network_receive_control_buffer);
	   BlockBufferDelete(network_send_control_buffer);
	   return -1;
	}

	alarm_audio_data_buffer = CharBufferCreate((128 + (1024 * 11) ),"bcu-alarm-audio-data");

	if ( 0 == alarm_audio_data_buffer )
	{
		 CharBufferDelete(network_send_audio_buffer);
		 CharBufferDelete(network_receive_audio_buffer);
		 BlockBufferDelete(network_receive_control_buffer);
		 BlockBufferDelete(network_send_control_buffer);
		 BlockBufferDelete(control_receive_cmd_from_screen);
		 BlockBufferDelete(send_info_to_pis_buffer);
		 BlockBufferDelete(recv_info_to_pis_buffer);
		 BlockBufferDelete(local_audio_data_buffer);
		 return -1;
	}


	ptt_state_send_buffer_id = BlockBufferCreate(2, sizeof(common_package_t),"bcu-cmu-car-type-rx");
	if ( 0 == ptt_state_send_buffer_id )
	{
		 CharBufferDelete(network_send_audio_buffer);
		 CharBufferDelete(network_receive_audio_buffer);
		 BlockBufferDelete(network_receive_control_buffer);
		 BlockBufferDelete(network_send_control_buffer);
		 BlockBufferDelete(control_receive_cmd_from_screen);
		 BlockBufferDelete(send_info_to_pis_buffer);
		 BlockBufferDelete(recv_info_to_pis_buffer);
		 BlockBufferDelete(local_audio_data_buffer);
		 CharBufferDelete(alarm_audio_data_buffer);
		 return -1;
	}

	ptt_state_recv_buffer_id = BlockBufferCreate(3, sizeof(common_package_t),"bcu-ptt-state-rx");
	if ( 0 == ptt_state_recv_buffer_id )
	{
		 CharBufferDelete(network_send_audio_buffer);
		 CharBufferDelete(network_receive_audio_buffer);
		 BlockBufferDelete(network_receive_control_buffer);
		 BlockBufferDelete(network_send_control_buffer);
		 BlockBufferDelete(control_receive_cmd_from_screen);
		 BlockBufferDelete(send_info_to_pis_buffer);
		 BlockBufferDelete(recv_info_to_pis_buffer);
		 BlockBufferDelete(local_audio_data_buffer);
		 CharBufferDelete(alarm_audio_data_buffer);
		 BlockBufferDelete(ptt_state_send_buffer_id);
		 return -1;
	}//bcu-ptt-state-tx

	network_udp_common_send_buffer = BlockBufferCreate(5, sizeof(common_package_t), "network-udp-common-send");
	if (0 == network_udp_common_send_buffer )
	{
		CharBufferDelete(network_send_audio_buffer);
		CharBufferDelete(network_receive_audio_buffer);
		BlockBufferDelete(network_receive_control_buffer);
		BlockBufferDelete(network_send_control_buffer);
		BlockBufferDelete(control_receive_cmd_from_screen);
		BlockBufferDelete(send_info_to_pis_buffer);
		BlockBufferDelete(recv_info_to_pis_buffer);
		BlockBufferDelete(local_audio_data_buffer);
		CharBufferDelete(alarm_audio_data_buffer);
		BlockBufferDelete(ptt_state_send_buffer_id);
		BlockBufferDelete(ptt_state_recv_buffer_id);
	   return -1;
	}


	dev_vol_info_buffer = BlockBufferCreate(2, sizeof(common_package_t), "network-udp-dev_vol-recv");
	if(dev_vol_info_buffer == -1)
	{
		return -1;
	}

	network_udp_common_recv_buffer = BlockBufferCreate(3, sizeof(common_package_t), "network-udp-common-recv"); //用于网络接收响应,块缓冲
   if ( 0 == network_udp_common_recv_buffer )
	{
		CharBufferDelete(network_send_audio_buffer);
		CharBufferDelete(network_receive_audio_buffer);
		BlockBufferDelete(network_receive_control_buffer);
		BlockBufferDelete(network_send_control_buffer);
		BlockBufferDelete(control_receive_cmd_from_screen);
		BlockBufferDelete(send_info_to_pis_buffer);
		BlockBufferDelete(recv_info_to_pis_buffer);
		BlockBufferDelete(local_audio_data_buffer);
		CharBufferDelete(alarm_audio_data_buffer);
		BlockBufferDelete(ptt_state_send_buffer_id);
		BlockBufferDelete(ptt_state_recv_buffer_id);
		BlockBufferDelete(network_udp_common_send_buffer);
	   return -1;
   }
   BlockBufferCreate(3, sizeof(common_big_package_t), "network-udp-common-big-send");
   BlockBufferCreate(2, sizeof(common_big_package_t), "network-udp-common-big-recv");
	int ret = 0;
	ret= BlockBufferCreate(3, sizeof(network_heart_package_t), "network-udp-heart-recv");
	if ( 0 == ret )
	{
		return -1;
	}
	ret= BlockBufferCreate(3, sizeof(network_heart_package_t), "network-udp-heart-send");
	if ( 0 == ret )
	{
		return -1;
	}

	ret= BlockBufferCreate(2, sizeof(common_big_package_t), "bcu-recv-all-dev-state");
	if ( 0 == ret )
	{
		return -1;
	}

	ret = BlockBufferCreate(3, sizeof(common_big_package_t), "network-udp-common-big-recv-version-check");
	if(ret == 0)
	{
		return -25;
	}
	ret = BlockBufferCreate(2, sizeof(common_package_t),"bcu-cmu-ack-state-rx");
	if ( 0 == ret )
	{
		return -26;
	}
	ret = BlockBufferCreate(2, sizeof(common_package_t),"network-udp-common-TCMS-cmu-state");
	if ( 0 == ret )
	{
		return -27;
	}

	ret = BlockBufferCreate(2, sizeof(network_shell_package_t),"network-udp-shell-recv");
	if ( 0 == ret )
	{
		return -28;
	}
	ret = BlockBufferCreate(2, sizeof(network_shell_package_t),"network-udp-shell-send");
	if ( 0 == ret )
	{
		return -29;
	}
	return 0;
}

/*Intialize the station information*/
void InitStationInformation()
{
	int i = 0;
	int j = 0;
	for(i = 0 ; i < 2;i ++)
	{
		for(j = 0; j < MAX_STATION_NUMBER; j++)
		{
			station_information[i][j].effective = 1;
		}
	}
}

/*This function is used to clear all audio data buffer*/
void ClearAllAudioDataBuffer()
{
	/*Clear aduio data send buffer*/
	CharBufferClear(bcu_state.audio_data_send_buffer_id);

	/*Clear aduio data reveive buffer*/
	CharBufferClear(bcu_state.audio_data_recv_buffer_id);

	return ;
}


/*Set PPT state information*/
void SetPTTStateInfoPackage()
{
	/*send to opposite bcu*/
	network_send_package_ptt_release_to_bcu.pkg_type = COMMON_PACKAGE_TYPE_PTT_STATE;
	strcpy(network_send_package_ptt_release_to_bcu.src_dev_name,bcu_state.bcu_info.devices_name);
	network_send_package_ptt_release_to_bcu.src_dev_number = bcu_state.bcu_info.devices_no;

	strcpy(network_send_package_ptt_release_to_bcu.dst_dev_name,bcu_state.bcu_info.devices_name);
	network_send_package_ptt_release_to_bcu.dst_dev_number = bcu_state.opposite_bcu_no;

	network_send_package_ptt_release_to_bcu.common_data_u.comm_ptt_state.src_ptt = 0;
	network_send_package_ptt_release_to_bcu.common_data_u.comm_ptt_state.dst_ptt = 0;


	network_send_package_ptt_press_to_bcu.pkg_type = COMMON_PACKAGE_TYPE_PTT_STATE;
	strcpy(network_send_package_ptt_press_to_bcu.src_dev_name,bcu_state.bcu_info.devices_name);
	network_send_package_ptt_press_to_bcu.src_dev_number = bcu_state.bcu_info.devices_no;

	strcpy(network_send_package_ptt_press_to_bcu.dst_dev_name,bcu_state.bcu_info.devices_name);
	network_send_package_ptt_press_to_bcu.dst_dev_number = bcu_state.opposite_bcu_no;

	network_send_package_ptt_press_to_bcu.common_data_u.comm_ptt_state.src_ptt = 1;
	network_send_package_ptt_press_to_bcu.common_data_u.comm_ptt_state.dst_ptt = 0;


	/*send to opposite pcu*/
	network_send_package_ptt_release_to_pcu.pkg_type = COMMON_PACKAGE_TYPE_PTT_STATE;
	strcpy(network_send_package_ptt_release_to_pcu.src_dev_name,"BCU");
	network_send_package_ptt_release_to_pcu.src_dev_number = bcu_state.bcu_info.devices_no;

	strcpy(network_send_package_ptt_release_to_pcu.dst_dev_name,"PCU");
	network_send_package_ptt_release_to_pcu.dst_dev_number = 0;

	network_send_package_ptt_release_to_pcu.common_data_u.comm_ptt_state.src_ptt = 0;
	network_send_package_ptt_release_to_pcu.common_data_u.comm_ptt_state.dst_ptt = 0;


	network_send_package_ptt_press_to_pcu.pkg_type = COMMON_PACKAGE_TYPE_PTT_STATE;
	strcpy(network_send_package_ptt_press_to_pcu.src_dev_name,"BCU");
	network_send_package_ptt_press_to_pcu.src_dev_number = bcu_state.bcu_info.devices_no;

	strcpy(network_send_package_ptt_press_to_pcu.dst_dev_name,"PCU");
	network_send_package_ptt_press_to_pcu.dst_dev_number = 0;

	network_send_package_ptt_press_to_pcu.common_data_u.comm_ptt_state.src_ptt = 1;
	network_send_package_ptt_press_to_pcu.common_data_u.comm_ptt_state.dst_ptt = 0;

	return ;
}

/*Init BCU rule*/
void BCURuleInit()
{
	int i = 0;
	for(i = 0;i < RECORD_OF_RULES;i ++)
	{
		memset((void *)&bcu_state.bcu_state_rule_table[i].xrule,0,sizeof(bcu_state.bcu_state_rule_table[i].xrule));
		bcu_state.bcu_state_rule_table[i].xrule.is_empty = 1;
		bcu_state.bcu_state_rule_table[i].rule_enable = FALSE;
	}
}

/*Add bcu rule*/
void BCUAddRule(state_machine_rule_t xRule)
{
	int i = 0;
	for(i = 0;i < RECORD_OF_RULES;i ++)
	{
		if(bcu_state.bcu_state_rule_table[i].xrule.is_empty == 1)
		{
			memcpy((void *)&bcu_state.bcu_state_rule_table[i].xrule ,(void *)&xRule,sizeof(xRule));
			bcu_state.bcu_state_rule_table[i].xrule.is_empty = 0;
			bcu_state.bcu_state_rule_table[i].rule_enable = TRUE;
			break;
		}
	}
}

/*remove bcu rule*/
void BCURemoveRule(state_machine_rule_t xRule)
{
	int i = 0;
	for(i = 0;i < RECORD_OF_RULES;i ++)
	{
		if(bcu_state.bcu_state_rule_table[i].xrule.is_empty == 0)
		{
			if(bcu_state.bcu_state_rule_table[i].xrule.psrc_state->state_id == xRule.psrc_state->state_id)
			{
				if(bcu_state.bcu_state_rule_table[i].xrule.dest_state->state_id == xRule.dest_state->state_id)
				{
					if(bcu_state.bcu_state_rule_table[i].xrule.trigger_event == xRule.trigger_event)
					{
						memset((void *)&bcu_state.bcu_state_rule_table[i].xrule,'0',sizeof(bcu_state.bcu_state_rule_table[i].xrule));
						bcu_state.bcu_state_rule_table[i].xrule.is_empty = 0;
						bcu_state.bcu_state_rule_table[i].rule_enable = FALSE;
						break;
					}
				}
			}
		}
	}
}

/*disable bcu rule*/
void BCUDisableRule(state_machine_rule_t xRule)
{
	int i = 0;
	for(i = 0;i < RECORD_OF_RULES;i ++)
	{
		if(bcu_state.bcu_state_rule_table[i].xrule.is_empty == 0)
		{
			if(bcu_state.bcu_state_rule_table[i].xrule.psrc_state->state_id == xRule.psrc_state->state_id)
			{
				if(bcu_state.bcu_state_rule_table[i].xrule.dest_state->state_id == xRule.dest_state->state_id)
				{
					if(bcu_state.bcu_state_rule_table[i].xrule.trigger_event == xRule.trigger_event)
					{
						bcu_state.bcu_state_rule_table[i].rule_enable = FALSE;
						break;
					}
				}
			}
		}
	}
}

/*enable bcu rule*/
void BCUEnableRule(state_machine_rule_t xRule)
{
	int i = 0;
	for(i = 0;i < RECORD_OF_RULES;i ++)
	{
		if(bcu_state.bcu_state_rule_table[i].xrule.is_empty == 0)
		{
			if(bcu_state.bcu_state_rule_table[i].xrule.psrc_state->state_id == xRule.psrc_state->state_id)
			{
				if(bcu_state.bcu_state_rule_table[i].xrule.dest_state->state_id == xRule.dest_state->state_id)
				{
					if(bcu_state.bcu_state_rule_table[i].xrule.trigger_event == xRule.trigger_event)
					{
						bcu_state.bcu_state_rule_table[i].rule_enable = TRUE;
						break;
					}
				}
			}
		}
	}
}

/*Set rule*/
state_machine_rule_t RuleSet(state_process_t *src_state,state_process_t *dst_state,int trigger_event)
{
	state_machine_rule_t xRule;
	xRule.psrc_state = src_state;
	xRule.dest_state = dst_state;
	xRule.trigger_event = trigger_event;
	return xRule;
}

/*Initial bcu rule table*/
void BCURuleTableInit()
{
	bcu_state.RuleInit();

	bcu_state.state_process_ann.auto_announcement = &auto_annoucement_state;
	bcu_state.state_process_ann.idle =  &idle_ann_state;
	bcu_state.state_process_ann.live_announcement =  &live_annoucement_state;
	bcu_state.state_process_ann.occ =  &occ_state;
	bcu_state.state_process_ann.emergency_announcement =  &emergency_annoucement_state;
	bcu_state.state_process_ann.manual_announcement =  &manual_annoucement_state;

	bcu_state.state_process_intercom.idle = &idle_intercom_state;
	bcu_state.state_process_intercom.d2d_intercom = &d2d_intercomm_state;
	bcu_state.state_process_intercom.d2p_intercom = &d2p_intercomm_state;
	bcu_state.state_process_intercom.d2d_hang_up_d2p = &intercomm_hang_up_state;

	//idle intercom-2
	bcu_state.AddRule(RuleSet(&idle_intercom_state,&d2d_intercomm_state,EVENT_PTT_OR_DRIVER_CALL));
	bcu_state.AddRule(RuleSet(&idle_intercom_state,&d2p_intercomm_state,EVENT_CONFIRM_PCU_CALL));

	//D2D-1
	bcu_state.AddRule(RuleSet(&d2d_intercomm_state,&idle_intercom_state,EVENT_PTT_RELEASE_AND_DRIVER_RELEASE));

	//D2P-2
	bcu_state.AddRule(RuleSet(&d2p_intercomm_state,&idle_intercom_state,EVENT_CONFIRM_RELEASE_PCU));
	bcu_state.AddRule(RuleSet(&d2p_intercomm_state,&intercomm_hang_up_state,EVENT_TRANSFER_TO_D2D_OR_DRIVER_CALL));

	//D2D hang up D2P-2
	bcu_state.AddRule(RuleSet(&intercomm_hang_up_state,&d2d_intercomm_state,EVENT_TIME_OUT));
	bcu_state.AddRule(RuleSet(&intercomm_hang_up_state,&d2p_intercomm_state,EVENT_CONFIRM_TRANSFER_TO_D2P));
}


void RegisterCommonBufferRecvBuffer()
{
	ZhwBufferRegister(1,COMMON_PACKAGE_TYPE_PTT_STATE,1, bcu_state.ppt_state_recv_buffer_id);
	///<注册设备音量、监听扬声器音量、所有设备状态接收缓存区
	ZhwBufferRegister(1,COMMON_PACKAGE_TYPE_CMU_CAR_TYPE,1, bcu_state.bcu_cmu_car_type_buffer_id);
	ZhwBufferRegister(1,COMMON_PACKAGE_TYPE_DEVICE_INFO,1, bcu_state.udp_common_recv_dev_colueme);
	ZhwBufferRegister(1,COMMON_PACKAGE_TYPE_MONITOR_SPEAKER_CONTROL,1, bcu_state.udp_common_recv_dev_colueme);
	ZhwBufferRegister(2,COMMON_PACKAGE_TYPE_ALL_DEV_STATE,1, bcu_state.all_dev_state_buffer_id);
	ZhwBufferRegister(2,COMMON_PACKAGE_TYPE_SOFTWARE_VERSION,1,bcu_state.software_version_buffer_id);
	ZhwBufferRegister(1,COMMON_PACKAGE_TYPE_ACK_STATE,1, bcu_state.bcu_cmu_ack_state_buffer_id);
	ZhwBufferRegister(1,COMMON_PACKAGE_TYPE_TCMS_CMU_STATE,1, bcu_state.bcu_recv_tcms_cmu_state_buffer_id);


}
/*Initialize the bcu control thread data buffer*/
void BCUControlMainBufferInit()
{
	int network_send_audio_buffer; // 发送音频数据缓冲区，每次从这里取出数据发送
	int network_receive_audio_buffer; // 接收音频数据缓冲区，每次收到音频数据，存放在此
	int network_receive_cmd_buffer; // 接受响应缓冲区(块)，每次收到响应信息，存放在此
	int control_receive_cmd_from_screen;//主控线程与触摸屏线程之间的buffer
	int control_send_cmd_to_network;//主控线程发送网络线程之间buffer
	int local_audio_data_buffer;
	int bcu_alarm_audio_data;
	int ppt_state_recv_buffer_id;


	bcu_state.bcu_recv_tcms_cmu_state_buffer_id = BlockBufferOpen("network-udp-common-TCMS-cmu-state");
	bcu_state.comm_cmd_recv_big_buffer_id = BlockBufferOpen("network-udp-common-big-recv");
	bcu_state.comm_cmd_send_big_buffer_id = BlockBufferOpen("network-udp-common-big-send");
	bcu_state.comm_cmd_send_buffer_id = BlockBufferOpen("network-udp-common-send");
	bcu_state.comm_cmd_recv_buffer_id = BlockBufferOpen("network-udp-common-recv");
	bcu_state.udp_common_recv_dev_colueme = BlockBufferOpen("network-udp-dev_vol-recv");
	bcu_state.software_version_buffer_id = BlockBufferOpen("network-udp-common-big-recv-version-check");
	network_send_audio_buffer = CharBufferOpen("bcu-network-audio-tx");
	network_receive_audio_buffer = CharBufferOpen("bcu-network-audio-rx");
	bcu_state.local_audio_data_recv_buffer_id = CharBufferOpen("bcu-local-audio-rx");
	diag_printf("bcu_state.local_audio_data_recv_buffer_id = %d\n",bcu_state.local_audio_data_recv_buffer_id);
	network_receive_cmd_buffer = BlockBufferOpen("bcu-network-cmd-rx");
	control_receive_cmd_from_screen = BlockBufferOpen("bcu-screen-cmd-tx");
	control_send_cmd_to_network = BlockBufferOpen("bcu-network-cmd-tx");
	local_audio_data_buffer = CharBufferOpen("bcu-local-audio-data");
	bcu_alarm_audio_data = CharBufferOpen("bcu-alarm-audio-data");

	ppt_state_recv_buffer_id = BlockBufferOpen("bcu-ptt-state-rx");

	bcu_state.dev_vol_info_buffer_id = BlockBufferOpen("bcu-control-recv-ts-info");

	bcu_state.cmd_send_buffer_id = control_send_cmd_to_network;
	bcu_state.cmd_recv_buffer_id = network_receive_cmd_buffer;
	bcu_state.audio_data_send_buffer_id = network_send_audio_buffer;
	bcu_state.audio_data_recv_buffer_id = network_receive_audio_buffer;
	bcu_state.recv_cmd_from_touch = control_receive_cmd_from_screen;
	bcu_state.local_audio_data_buffer_id = local_audio_data_buffer;
	bcu_state.alarm_audio_data_buffer_id = bcu_alarm_audio_data;
	bcu_state.bcu_cmu_car_type_buffer_id  = BlockBufferOpen("bcu-cmu-car-type-rx");
	bcu_state.bcu_cmu_ack_state_buffer_id  = BlockBufferOpen("bcu-cmu-ack-state-rx");

	bcu_state.ppt_state_recv_buffer_id = ppt_state_recv_buffer_id;

	bcu_state.pending_buffer_id = CharBufferOpen("bcu-network-audio-pendig");
	bcu_state.all_dev_state_buffer_id = BlockBufferOpen("bcu-recv-all-dev-state");

	bcu_state.shell_recv_buffer_id = BlockBufferOpen("network-udp-shell-recv");
	bcu_state.shell_send_buffer_id = BlockBufferOpen("network-udp-shell-send");
}

/*initialize the original state of bcu*/
void BCUStateInit()
{///<初始化BCU 状态
#if 1
	bcu_state.bcu_info.current_state_ann = ANN_IDLE;
	bcu_state.bcu_info.current_state_intercom = INTERCOM_IDLE;

	bcu_state.bcu_active_ann_state =  &idle_ann_state;
	bcu_state.bcu_active_intercom_state = &idle_intercom_state;
	bcu_state.mic_owner = NONE;
	bcu_state.this_line_no = DEFAULT_LINE_NO;
	bcu_state.other_bcu_ptt_state = 0;
	bcu_state.active_bcu_no = 0;//bcu_state.bcu_info.devices_no;///< 2014-01-09 wilson
#endif
}

/*set d2p over cmd package*/
void SetD2POverCMD(send_infomation_t *param_cmd_info)
{
#if 0
	param_cmd_info->event_type_ann = ANN_IDLE;
	param_cmd_info->event_type_intercom = D2P_INTERCOM_PENDING;
	memset((void *)&param_cmd_info->event_info_ann,0,sizeof(param_cmd_info->event_info_ann));
	param_cmd_info->event_info_intercom.d2p_intercomm.d2p_intercomm_active = 0;
#endif
	return ;
}

/*D2D hangup D2P timeout handle*/
void D2DHangUpD2PTimeOutHandle()
{
	last_control_flag = control_flag;control_flag = 31;
	if(whether_d2d_hangup_d2p_timeout == 1)
	{///<目前没有使用，司机对讲打断乘客对讲之后，如果司机对讲退出，自动切换到与之前的乘客继续进行对讲
		last_control_flag = control_flag;control_flag = 194;
		whether_d2d_hangup_d2p_timeout = 0;

		send_infomation_t d2p_over_cmd;

		SetD2POverCMD(&d2p_over_cmd);

		UpdataGlobalDeviceInfo(d2p_over_cmd);/*Update global device information*/
		last_control_flag = control_flag;control_flag = 199;
		StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_TIME_OUT,&d2p_over_cmd);
		last_control_flag = control_flag;control_flag = 200;
	}
	last_control_flag = control_flag;control_flag = 201;
}

/*Change bcu state*/
void StateMachineExchange(state_process_t **state_process,int trigger_signal,send_infomation_t *param_send_info)
{
#if 1
	int i = 0;
	debug_print(("(*state_process)->state_id = %d,trigger_signal = %d\n",(*state_process)->state_id,trigger_signal));
	diag_printf("(*state_process)->state_id = %d,trigger_signal = %d\n",(*state_process)->state_id,trigger_signal);
	for(i = 0;i < RECORD_OF_RULES;i++)
	{
		if(bcu_state.bcu_state_rule_table[i].rule_enable == TRUE &&
		   (*state_process)->state_id == bcu_state.bcu_state_rule_table[i].xrule.psrc_state->state_id &&
		   trigger_signal == bcu_state.bcu_state_rule_table[i].xrule.trigger_event)
		{
			(*state_process)->exit();
			*state_process = bcu_state.bcu_state_rule_table[i].xrule.dest_state;
			cyg_thread_delay(10);
			(*state_process)->enter(param_send_info);
			break;
		}
	}
	if(i == RECORD_OF_RULES )
	{
		intercomm_debug_print(("aaaaa,error request!\n"));
	}
#endif
	return ;
}

/*Chaneg thread's priority*/
int PthreadPriorityChangeForSchedRr(pthread_t  target_thread_id, int  new_prior )
{
	 int a,b;
	last_thread_change_flag = thread_change_flag;thread_change_flag = 1;
	  if(target_thread_id == thread_of_control )
	  {
		  control_priority = new_prior;
	  }
	  else if(target_thread_id == thread_of_screen )
	  {
		  ts_priority = new_prior;
	  }
	  else if(target_thread_id == thread_of_network )
	  {
		  network_priority = new_prior;
	  }
	  struct sched_param param_temp;
	  if(target_thread_id == thread_of_network )
	  {
		  pthread_getschedparam(target_thread_id, &a, &param_temp);
		  b = param_temp.sched_priority;
	  }
	  last_thread_change_flag = thread_change_flag;thread_change_flag = 2;
	  param_temp.sched_priority = new_prior;
	  pthread_setschedparam(target_thread_id, SCHED_RR, &param_temp);
	  last_thread_change_flag = thread_change_flag;thread_change_flag = 3;
	  pthread_getschedparam(target_thread_id, &a, &param_temp);
	  last_thread_change_flag = thread_change_flag;thread_change_flag = 10;
	  sched_yield();
	  last_thread_change_flag = thread_change_flag;thread_change_flag = 4;
	  if(param_temp.sched_priority != new_prior && target_thread_id == thread_of_network)
	  {
		  debug_print(("thread priority change error-%d-%d-%d\n",param_temp.sched_priority,new_prior,b));
	  }
	  return 0;
}

/*Judge whether have finish d2d*/
void JudgeWhetherD2DHaveFinished()
{///<判断D2D是否已经完成
	if(bcu_state.bcu_active_intercom_state->state_id == D2D_INTERCOMM_EVENT ||
	   bcu_state.bcu_active_intercom_state->state_id == D2D_HANGUP_D2P_EVENT)
	{
		last_control_flag = control_flag;control_flag = 204;
		if(GetD2PExternButtonState() == 1 && bcu_state.d2d_button_state == 1)
		{
			last_control_flag = control_flag;control_flag = 205;
			send_infomation_t d2d_over_package;

			d2d_over_package.src_devices_no = bcu_state.bcu_info.devices_no;
			strcpy(d2d_over_package.src_devices_name,bcu_state.bcu_info.devices_name);

			d2d_over_package.event_type_ann = ANN_IDLE;
			d2d_over_package.event_type_intercom = D2D_INTERCOMM_EVENT;

			d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_active = 0;
			d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over = 0;
			d2d_over_package.event_info_intercom.d2d_intercomm.d2d_ppt_state = 0;
			d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_bcu_device_no = bcu_state.opposite_bcu_no;

//			UpdataGlobalDeviceInfo(d2d_over_package);/*Update global device information*/

			if(bcu_state.bcu_active_intercom_state->state_id == D2D_HANGUP_D2P_EVENT)//pending
			{
				send_infomation_t rt_d2d_bk_d2p,*p_d2d_bk_d2p;
				p_d2d_bk_d2p = &rt_d2d_bk_d2p;
				memset((void *)p_d2d_bk_d2p,0x00,sizeof(rt_d2d_bk_d2p));
				p_d2d_bk_d2p->event_type_ann = ANN_IDLE;
				p_d2d_bk_d2p->event_type_intercom = D2P_INTERCOMM_EVENT;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_active = 1;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_pending = 0;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_bcu_device_no = bcu_state.bcu_info.devices_no;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no = bcu_state.pcu_request_info.recept_pcu_no;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_request_or_over = 1;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_response = 0;
				p_d2d_bk_d2p->src_devices_no = bcu_state.bcu_info.devices_no;
				strcpy((char *)p_d2d_bk_d2p->src_devices_name,(char *)bcu_state.bcu_info.devices_name);
				last_control_flag = control_flag;control_flag = 209;
				StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_CONFIRM_TRANSFER_TO_D2P,&rt_d2d_bk_d2p);
				last_control_flag = control_flag;control_flag = 210;

				current_is_being_d2d = 0;

				last_control_flag = control_flag;control_flag = 211;
				AlarmTSToChangeScreen(31);
				last_control_flag = control_flag;control_flag = 212;
			}
			else if(bcu_state.bcu_active_intercom_state->state_id == D2D_INTERCOMM_EVENT)//idle
			{
				debug_print(("We will finish the D2D\n"));
				diag_printf("We will finish the D2D\n");
				last_control_flag = control_flag;control_flag = 213;
				StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_PTT_RELEASE_AND_DRIVER_RELEASE,&d2d_over_package);
				last_control_flag = control_flag;control_flag = 214;
			}
			last_control_flag = control_flag;control_flag = 215;
		}
		last_control_flag = control_flag;control_flag = 216;
	}
}

void ExitD2DAccordingToCMU()
{///<CMU 使得BCU 退出D2D
	if(bcu_state.bcu_active_intercom_state->state_id == D2D_INTERCOMM_EVENT ||
	   bcu_state.bcu_active_intercom_state->state_id == D2D_HANGUP_D2P_EVENT)
	{
		last_sample_flag = sample_flag;sample_flag = 300;
		if(bcu_state.d2d_button_state == 1 && bcu_state.bcu_mcu_connect_state == 1)
		{
			send_infomation_t d2d_over_package;

			d2d_over_package.src_devices_no = bcu_state.bcu_info.devices_no;
			strcpy(d2d_over_package.src_devices_name,bcu_state.bcu_info.devices_name);

			d2d_over_package.event_type_ann = ANN_IDLE;
			d2d_over_package.event_type_intercom = D2D_INTERCOMM_EVENT;

			d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_active = 0;
			d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over = 0;
			d2d_over_package.event_info_intercom.d2d_intercomm.d2d_ppt_state = 0;
			d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_bcu_device_no = bcu_state.opposite_bcu_no;
			last_sample_flag = sample_flag;sample_flag = 301;
			UpdataGlobalDeviceInfo(d2d_over_package);/*Update global device information*/
			if(bcu_state.bcu_active_intercom_state->state_id == D2D_HANGUP_D2P_EVENT)//pending
			{
				last_sample_flag = sample_flag;sample_flag = 302;
				send_infomation_t rt_d2d_bk_d2p,*p_d2d_bk_d2p;
				p_d2d_bk_d2p = &rt_d2d_bk_d2p;
				memset((void *)p_d2d_bk_d2p,0x00,sizeof(rt_d2d_bk_d2p));
				p_d2d_bk_d2p->event_type_ann = ANN_IDLE;
				p_d2d_bk_d2p->event_type_intercom = D2P_INTERCOMM_EVENT;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_active = 1;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_pending = 0;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_bcu_device_no = bcu_state.bcu_info.devices_no;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no = bcu_state.pcu_request_info.recept_pcu_no;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_request_or_over = 1;
				p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_response = 0;
				p_d2d_bk_d2p->src_devices_no = bcu_state.bcu_info.devices_no;
				strcpy((char *)p_d2d_bk_d2p->src_devices_name,(char *)bcu_state.bcu_info.devices_name);
				StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_CONFIRM_TRANSFER_TO_D2P,&rt_d2d_bk_d2p);
				last_sample_flag = sample_flag;sample_flag = 303;
				current_is_being_d2d = 0;
				last_sample_flag = sample_flag;sample_flag = 304;
				AlarmTSToChangeScreen(31);
				last_sample_flag = sample_flag;sample_flag = 305;
			}
			else if(bcu_state.bcu_active_intercom_state->state_id == D2D_INTERCOMM_EVENT)//idle
			{
				last_sample_flag = sample_flag;sample_flag = 306;
				debug_print(("We will finish the D2D\n"));
				last_sample_flag = sample_flag;sample_flag = 307;
				StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_PTT_RELEASE_AND_DRIVER_RELEASE,&d2d_over_package);
				last_sample_flag = sample_flag;sample_flag = 308;
			}
		}
	}
}

void judgeWheteherExitD2DEnterD2P()
{///<判断是否退出D2D再进入D2P
	if(bcu_state.bcu_active_intercom_state->state_id == D2D_HANGUP_D2P_EVENT)
	{
		send_infomation_t d2d_over_package;

		d2d_over_package.src_devices_no = bcu_state.bcu_info.devices_no;
		strcpy(d2d_over_package.src_devices_name,bcu_state.bcu_info.devices_name);

		d2d_over_package.event_type_ann = ANN_IDLE;
		d2d_over_package.event_type_intercom = D2D_INTERCOMM_EVENT;

		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_active = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_ppt_state = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_bcu_device_no = bcu_state.opposite_bcu_no;
		last_sample_flag = sample_flag;sample_flag = 309;
		UpdataGlobalDeviceInfo(d2d_over_package);/*Update global device information*/
		last_sample_flag = sample_flag;sample_flag = 310;
		if(bcu_state.bcu_active_intercom_state->state_id == D2D_HANGUP_D2P_EVENT)//pending
		{
			send_infomation_t rt_d2d_bk_d2p,*p_d2d_bk_d2p;
			p_d2d_bk_d2p = &rt_d2d_bk_d2p;
			memset((void *)p_d2d_bk_d2p,0x00,sizeof(rt_d2d_bk_d2p));
			p_d2d_bk_d2p->event_type_ann = ANN_IDLE;
			p_d2d_bk_d2p->event_type_intercom = D2P_INTERCOMM_EVENT;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_active = 1;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_pending = 0;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_bcu_device_no = bcu_state.bcu_info.devices_no;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no = bcu_state.pcu_request_info.recept_pcu_no;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_request_or_over = 1;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_response = 0;
			p_d2d_bk_d2p->src_devices_no = bcu_state.bcu_info.devices_no;
			strcpy((char *)p_d2d_bk_d2p->src_devices_name,(char *)bcu_state.bcu_info.devices_name);

			last_sample_flag = sample_flag;sample_flag = 310;
			StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_CONFIRM_TRANSFER_TO_D2P,&rt_d2d_bk_d2p);
			last_sample_flag = sample_flag;sample_flag = 311;
			current_is_being_d2d = 0;
#if 0
			PthreadPriorityChangeForSchedRr(thread_of_control, BCU_PRIORIT+1);
			ChangeIntercommLabelState(current_is_being_d2d);
			Fl::wait();
			wd_touch_screen->show();
			cyg_thread_delay(10);
			PthreadPriorityChangeForSchedRr(thread_of_control, BCU_PRIORIT);
#else
			last_sample_flag = sample_flag;sample_flag = 312;
			AlarmTSToChangeScreen(31);
			last_sample_flag = sample_flag;sample_flag = 313;
#endif
		}
	}
}

/*Judge whether have finish d2d*/
void NetFinishD2D()
{///<网络结束D2D

	if(bcu_state.bcu_active_intercom_state->state_id == D2D_INTERCOMM_EVENT ||
	   bcu_state.bcu_active_intercom_state->state_id == D2D_HANGUP_D2P_EVENT)
	{
		if(bcu_state.d2d_button_state != 1)
		{
			return ;
		}
		send_infomation_t d2d_over_package;

		d2d_over_package.src_devices_no = bcu_state.bcu_info.devices_no;
		strcpy(d2d_over_package.src_devices_name,bcu_state.bcu_info.devices_name);

		d2d_over_package.event_type_ann = ANN_IDLE;
		d2d_over_package.event_type_intercom = D2D_INTERCOMM_EVENT;

		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_active = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_ppt_state = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_bcu_device_no = bcu_state.opposite_bcu_no;
		last_sample_flag = sample_flag;sample_flag = 314;
		UpdataGlobalDeviceInfo(d2d_over_package);/*Update global device information*/
		last_sample_flag = sample_flag;sample_flag = 315;
		if(bcu_state.bcu_active_intercom_state->state_id == D2D_HANGUP_D2P_EVENT)//pending
		{
			send_infomation_t rt_d2d_bk_d2p,*p_d2d_bk_d2p;
			p_d2d_bk_d2p = &rt_d2d_bk_d2p;
			memset((void *)p_d2d_bk_d2p,0x00,sizeof(rt_d2d_bk_d2p));
			p_d2d_bk_d2p->event_type_ann = ANN_IDLE;
			p_d2d_bk_d2p->event_type_intercom = D2P_INTERCOMM_EVENT;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_active = 1;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_pending = 0;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_bcu_device_no = bcu_state.bcu_info.devices_no;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no = bcu_state.pcu_request_info.recept_pcu_no;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_request_or_over = 1;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_response = 0;
			p_d2d_bk_d2p->src_devices_no = bcu_state.bcu_info.devices_no;
			strcpy((char *)p_d2d_bk_d2p->src_devices_name,(char *)bcu_state.bcu_info.devices_name);
			last_sample_flag = sample_flag;sample_flag = 316;
			StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_CONFIRM_TRANSFER_TO_D2P,&rt_d2d_bk_d2p);
			last_sample_flag = sample_flag;sample_flag = 317;
			current_is_being_d2d = 0;
#if 0
			ChangeIntercommLabelState(current_is_being_d2d);
#else
			last_sample_flag = sample_flag;sample_flag = 318;
			AlarmTSToChangeScreen(31);
			last_sample_flag = sample_flag;sample_flag = 319;
#endif
		}
		else if(bcu_state.bcu_active_intercom_state->state_id == D2D_INTERCOMM_EVENT)//idle
		{
			last_sample_flag = sample_flag;sample_flag = 320;
			debug_print(("We will finish the D2D\n"));
			last_sample_flag = sample_flag;sample_flag = 321;
			StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_PTT_RELEASE_AND_DRIVER_RELEASE,&d2d_over_package);
			last_sample_flag = sample_flag;sample_flag = 322;
		}
	}
}

void JudgeWhetherHaveOuter3D5Triger()
{///<检测当前是否有外接3D5触发
	if(bcu_state.bcu_active_ann_state->state_id == OCC_EVENT)
	{
			return ;
	}
	///<获取外接3D5状态
	bcu_state.this_bcu_outer_3d5_button_state = out_3d5_state_flag >= 2?1:0;
	if(bcu_state.this_bcu_outer_3d5_button_state== 1 &&
		bcu_state.bcu_active_ann_state->state_id != MIC_3D5_OUTER_EVENT &&
		bcu_state.mic_owner != INTERCOM)
	{
		last_control_flag = control_flag;control_flag = 228;
		send_infomation_t send_infomation_3d5;
		strcpy(send_infomation_3d5.src_devices_name,bcu_state.bcu_info.devices_name);
		send_infomation_3d5.src_devices_no = bcu_state.bcu_info.devices_no;
		send_infomation_3d5.event_type_ann = MIC_3D5_OUTER_EVENT;
		send_infomation_3d5.event_type_intercom = INTERCOM_IDLE;
		send_infomation_3d5.event_info_ann.outer_3d5_ann.outer_3d5_active = 1;
		send_infomation_3d5.event_info_ann.outer_3d5_ann.outer_3d5_begin_or_over = 0;
		send_infomation_3d5.event_info_ann.outer_3d5_ann.outer_3d5_response = 0;

		BlockBufferWrite(bcu_state.recv_cmd_from_touch, &send_infomation_3d5, sizeof(send_infomation_3d5));
	}
	last_control_flag = control_flag;control_flag = 229;
	if( bcu_state.bcu_active_ann_state->state_id == MIC_3D5_OUTER_EVENT &&
			bcu_state.this_bcu_outer_3d5_button_state == 0 )
	{
		send_infomation_t send_infomation_3d5;
		last_control_flag = control_flag;control_flag = 230;
		strcpy(send_infomation_3d5.src_devices_name,bcu_state.bcu_info.devices_name);
		send_infomation_3d5.src_devices_no = bcu_state.bcu_info.devices_no;
		send_infomation_3d5.event_type_ann = MIC_3D5_OUTER_EVENT;
		send_infomation_3d5.event_type_intercom = INTERCOM_IDLE;
		send_infomation_3d5.event_info_ann.outer_3d5_ann.outer_3d5_active = 0;
		send_infomation_3d5.event_info_ann.outer_3d5_ann.outer_3d5_begin_or_over = 0;
		send_infomation_3d5.event_info_ann.outer_3d5_ann.outer_3d5_response = 0;

		BlockBufferWrite(bcu_state.recv_cmd_from_touch, &send_infomation_3d5, sizeof(send_infomation_3d5));
		last_control_flag = control_flag;control_flag = 231;
	}
	last_control_flag = control_flag;control_flag = 232;
}

void JudgeWhetherOCCTriger(network_send_package_t recv_network_info_from_network)
{///<当前是否有OCC触发
	if(strcmp(recv_network_info_from_network.send_information.src_devices_name,"CCU") != 0)
	{
//		diag_printf("This occ is not send by ccu,%s\n",recv_network_info_from_network.send_information.src_devices_name);
		return ;
	}

	if(recv_network_info_from_network.send_information.event_type_ann == OCC_EVENT)
	{
		diag_printf("This occ is  send by %s\n",recv_network_info_from_network.send_information.src_devices_name);
		send_infomation_t send_infomation_occ;
		strcpy(send_infomation_occ.src_devices_name,bcu_state.bcu_info.devices_name);
		send_infomation_occ.src_devices_no = bcu_state.bcu_info.devices_no;
		send_infomation_occ.event_type_ann = OCC_EVENT;
		send_infomation_occ.event_type_intercom = INTERCOM_IDLE;
		if(recv_network_info_from_network.send_information.event_info_ann.occ_announce.occ_active == 1)
		{
			send_infomation_occ.event_info_ann.occ_announce.occ_active = 1;
			send_infomation_occ.event_info_ann.occ_announce.occ_begin_or_over = 0;
			send_infomation_occ.event_info_ann.occ_announce.occ_response = 0;
		}
		else
		{
			send_infomation_occ.event_info_ann.occ_announce.occ_active = 0;
			send_infomation_occ.event_info_ann.occ_announce.occ_begin_or_over = 0;
			send_infomation_occ.event_info_ann.occ_announce.occ_response = 0;
			if((bcu_state.bcu_active_ann_state->state_id == MIC_3D5_OUTER_EVENT ||
					bcu_state.bcu_active_ann_state->state_id == LIVE_ANN_EVENT) &&
					bcu_state.bcu_active_intercom_state->state_id == INTERCOM_IDLE)
			{
				CloseAudioSampleTimer();

				ClearAllAudioDataBuffer();//清空发送音频buf
			}
		}
		BlockBufferWrite(bcu_state.recv_cmd_from_touch, &send_infomation_occ, sizeof(send_infomation_occ));
	}

}
/*Judge whether want to enter D2D state*/
void JudegWhetherRequestD2D()
{
	if(bcu_state.bcu_active_intercom_state->state_id == INTERCOM_IDLE &&
			GetD2PExternButtonState() == 1 && bcu_state.mic_owner == NONE && bcu_state.d2d_button_state == 0)
	{
		debug_print(("I want to enter D2D state\n"));

		last_control_flag = control_flag;control_flag = 217;

		send_infomation_t D2D_request_cmd_package;

		strcpy(D2D_request_cmd_package.src_devices_name,"BCU");
		D2D_request_cmd_package.src_devices_no = bcu_state.bcu_info.devices_no;

		D2D_request_cmd_package.event_type_ann = ANN_IDLE;
		D2D_request_cmd_package.event_type_intercom = D2D_INTERCOMM_EVENT;

		D2D_request_cmd_package.event_info_intercom.d2d_intercomm.d2d_intercomm_active = 1;
		D2D_request_cmd_package.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over = 0;
		D2D_request_cmd_package.event_info_intercom.d2d_intercomm.d2d_intercomm_response = 0;
		D2D_request_cmd_package.event_info_intercom.d2d_intercomm.d2d_intercomm_bcu_device_no = bcu_state.opposite_bcu_no;
		D2D_request_cmd_package.event_info_intercom.d2d_intercomm.d2d_ppt_state = 1;

		last_control_flag = control_flag;control_flag = 218;
		UpdataGlobalDeviceInfo(D2D_request_cmd_package);/*Update global device information*/
		bcu_state.this_bcu_request = 1;
		last_control_flag = control_flag;control_flag = 219;
		/*idle->d2d*/
		StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_PTT_OR_DRIVER_CALL,&D2D_request_cmd_package);
		last_control_flag = control_flag;control_flag = 220;
	}
	last_control_flag = control_flag;control_flag = 221;
}

void SendD2DAccordingToCMU()
{
	if(1)
	{
		debug_print(("I want to enter D2D state\n"));

//		SendBCUPTTPressStateTOBCU();

		send_infomation_t D2D_request_cmd_package;

		strcpy(D2D_request_cmd_package.src_devices_name,"BCU");
		D2D_request_cmd_package.src_devices_no = bcu_state.bcu_info.devices_no;

		D2D_request_cmd_package.event_type_ann = ANN_IDLE;
		D2D_request_cmd_package.event_type_intercom = D2D_INTERCOMM_EVENT;

		D2D_request_cmd_package.event_info_intercom.d2d_intercomm.d2d_intercomm_active = 1;
		D2D_request_cmd_package.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over = 0;
		D2D_request_cmd_package.event_info_intercom.d2d_intercomm.d2d_intercomm_response = 0;
		D2D_request_cmd_package.event_info_intercom.d2d_intercomm.d2d_intercomm_bcu_device_no = bcu_state.opposite_bcu_no;
		D2D_request_cmd_package.event_info_intercom.d2d_intercomm.d2d_ppt_state = 1;

		UpdataGlobalDeviceInfo(D2D_request_cmd_package);/*Update global device information*/
		bcu_state.this_bcu_request = 1;
		/*idle->d2d*/
		StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_PTT_OR_DRIVER_CALL,&D2D_request_cmd_package);
	}
}

/*Update Line number*/
int BcuUpdateLineNumber( send_infomation_t *pcmd_from_touch )
{
	 if( NULL ==pcmd_from_touch )
	 {
		debug_print(("BcuUpdateLineNumber: null pointer\n"));
		return -1;
	 }

	 if( pcmd_from_touch->event_info_ann.line_info.line_no != 0 )
	 {
		bcu_state.this_line_no = pcmd_from_touch->event_info_ann.line_info.line_no; //update line no.
		debug_print(("BcuUpdateLineNumber: new line no=%d,region = %d\n", bcu_state.this_line_no,pcmd_from_touch->event_info_ann.line_info.region_no));

		network_send_package_t network_send_information = {"0"};
		network_send_information.dst_devices_no = MUL_DST_NO;

		strcpy((char *)network_send_information.send_information.src_devices_name,bcu_state.bcu_info.devices_name);
		network_send_information.send_information.src_devices_no = bcu_state.bcu_info.devices_no;

		network_send_information.send_information.event_info_ann.line_info.line_no = pcmd_from_touch->event_info_ann.line_info.line_no;
		network_send_information.send_information.event_info_ann.line_info.region_no = pcmd_from_touch->event_info_ann.line_info.region_no;
		network_send_information.send_information.event_info_ann.line_info.get_or_set = 0;
		network_send_information.send_information.event_info_ann.line_info.region_update_type = 0;

		network_send_information.send_information.event_type_ann = LINE_NUMBER_CHANGE_EVENT;
		network_send_information.send_information.event_type_intercom = INTERCOM_IDLE;
		network_send_information.send_information.event_ptt_state = GetPTTState();
		last_control_flag = control_flag;control_flag = 247;
		DisplayNetworkCmd(network_send_information);
		last_control_flag = control_flag;control_flag = 248;
		int ret = 0,write_counts = 0;
		strcpy((char *)network_send_information.dst_devices_name,"CCU");
		do
		{
			ret = BlockBufferWrite(bcu_state.cmd_send_buffer_id, (const void *)&network_send_information, sizeof(network_send_information));
			write_counts ++;
			if(ret <= 0)
			{
				cyg_thread_delay(1);
			}
		}while(ret <= 0 && write_counts < 2);
		last_control_flag = control_flag;control_flag = 249;
		strcpy((char *)network_send_information.dst_devices_name,"EAMP");
		write_counts = 0;
		do
		{
			ret = BlockBufferWrite(bcu_state.cmd_send_buffer_id, (const void *)&network_send_information, sizeof(network_send_information));
			if(ret <= 0)
			{
				cyg_thread_delay(1);
			}
		}while(ret <= 0 && write_counts < 2);
		last_control_flag = control_flag;control_flag = 250;
		strcpy((char *)network_send_information.dst_devices_name,"BCU");
		network_send_information.dst_devices_no = bcu_state.opposite_bcu_no;
		write_counts = 0;
		do
		{
			ret = BlockBufferWrite(bcu_state.cmd_send_buffer_id, (const void *)&network_send_information, sizeof(network_send_information));
			write_counts ++;
			if(ret <= 0)
			{
				cyg_thread_delay(1);
			}
		}while(ret <= 0 && write_counts < 2);
		last_control_flag = control_flag;control_flag = 251;
		pcmd_from_touch->src_devices_no = 0;
		strcpy(pcmd_from_touch->src_devices_name,"");
	 }
	 last_control_flag = control_flag;control_flag = 252;
	 return 0;
}

/*Update skip station information*/
void UpdateSkipStationInformation(send_infomation_t *param_cmd_info)
{

	if(param_cmd_info->event_type_ann == SKIP_STATION &&
	   param_cmd_info->event_type_intercom == INTERCOM_IDLE)
	{
		last_control_flag = control_flag;control_flag = 240;
		int ret = 0,write_counts = 0;
		station_information[param_cmd_info->event_info_ann.station_information.line_number - 3][param_cmd_info->event_info_ann.station_information.station_no].effective = param_cmd_info->event_info_ann.station_information.effective;
		bcu_state.this_line_no = param_cmd_info->event_info_ann.station_information.line_number;
		network_send_package_t network_send_package_skip_station_info = {"\0",0};
		memcpy((void *)&network_send_package_skip_station_info.send_information,(void *)param_cmd_info,sizeof(*param_cmd_info));
		network_send_package_skip_station_info.dst_devices_no =  MUL_DST_NO;

		strcpy((char *)network_send_package_skip_station_info.dst_devices_name,"EAMP");
		do
		{
			ret = BlockBufferWrite(bcu_state.cmd_send_buffer_id,&network_send_package_skip_station_info,sizeof(network_send_package_skip_station_info));
			write_counts++;
			if(ret <= 0)
			{
				cyg_thread_delay(10);
			}
		}while(ret <= 0 && write_counts < 2);
		last_control_flag = control_flag;control_flag = 241;
		strcpy((char *)network_send_package_skip_station_info.dst_devices_name,"CCU");
		write_counts = 0;
		do
		{
			ret = BlockBufferWrite(bcu_state.cmd_send_buffer_id,&network_send_package_skip_station_info,sizeof(network_send_package_skip_station_info));
			write_counts ++;
			if(ret <= 0)
			{
				cyg_thread_delay(10);
			}
		}while(ret <= 0 && write_counts < 2);
		last_control_flag = control_flag;control_flag = 242;
		param_cmd_info->event_type_ann  = ANN_IDLE;
		param_cmd_info->event_type_intercom = INTERCOM_IDLE;
		param_cmd_info->event_info_ann.line_info.line_no = 0;
		ShowStationInformation(bcu_state.this_line_no);
	}
}

/*Change line no between 3 and 4*/
void ChangeLineNumber(send_infomation_t *param_cmd_info)
{
	if(param_cmd_info->event_type_ann == LINE_NUMBER_CHANGE_EVENT &&  param_cmd_info->event_type_intercom == INTERCOM_IDLE)
	{
		last_control_flag = control_flag;control_flag = 243;
		BcuUpdateLineNumber(param_cmd_info);/*Update Line number*/
		last_control_flag = control_flag;control_flag = 244;
		ClearSkipStationInfo(param_cmd_info->event_info_ann.line_info.line_no);
		last_control_flag = control_flag;control_flag = 245;
	}
}

/*Handle announcement event*/
void ANNStateHangle(send_infomation_t *param_recv_cmd_info_of_ann,send_infomation_t *param_cmd_info)/*BCU announcement handle*/
{
	last_control_flag = control_flag;control_flag = 254;
	if(param_cmd_info->event_type_ann != ANN_IDLE)
	{
		last_control_flag = control_flag;control_flag = 256;
		if(WhetherWantToExchangeState(*param_cmd_info))
		{
			debug_print(("I want to exchange state\n"));
			last_control_flag = control_flag;control_flag = 257;
			if(param_cmd_info->event_type_ann == LIVE_ANN_EVENT && bcu_state.mic_owner != NONE &&
					param_cmd_info->event_info_ann.live_announce.live_active == 1)
			{
				debug_print(("we want to change to live,but the mic have been occupied\n"));
				param_cmd_info->src_devices_no = 0;
				strcpy((char *)param_cmd_info->src_devices_name,"");
			}
			else
			{
				debug_print(("ANN State change\n"));
				last_control_flag = control_flag;control_flag = 258;
				BcuDynamicStateExchange(&bcu_state.bcu_active_ann_state,param_cmd_info);
				last_control_flag = control_flag;control_flag = 259;
			}
		}
		else
		{
			memcpy((void *)param_recv_cmd_info_of_ann,(void *)param_cmd_info,sizeof(*param_cmd_info));
		}
	}
	last_control_flag = control_flag;control_flag = 255;
}


/*Handle intercom event*/
void IntercomStateHangle(send_infomation_t *param_recv_cmd_info_of_intercom,send_infomation_t *param_cmd_info)/*BCU announcement handle*/
{
	/*refuse the request of PCU*/
	if(param_cmd_info->event_type_intercom != INTERCOM_IDLE)
	{
		if(param_cmd_info->event_type_intercom == D2P_INTERCOMM_EVENT &&
		   param_cmd_info->event_info_intercom.d2p_intercomm.d2p_intercomm_active == 0)
		{
		//	BCUUpdataPCUDeviceInfo(*param_cmd_info);
//			CharBufferBrush(bcu_state.alarm_audio_data_buffer_id);
		}

		if(param_cmd_info->event_type_intercom == D2P_INTERCOMM_EVENT &&
		   param_cmd_info->event_info_intercom.d2p_intercomm.d2p_intercomm_active == 1 &&
		   param_cmd_info->event_info_intercom.d2p_intercomm.d2p_intercomm_request_or_over == 0)
		{
			bcu_state.pcu_request_info.recept_pcu_no =
					param_cmd_info->event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no;

			debug_print(("active_pcu_no = %d\n",bcu_state.pcu_request_info.recept_pcu_no));
		}

		if(bcu_state.bcu_active_ann_state->state_id != OCC_EVENT)///<wilson 2013-12-22
		{
			if(WhetherWantToExchangeState(*param_cmd_info))
			{
				intercomm_debug_print(("Intercomm State change\n"));
				last_control_flag = control_flag;control_flag = 261;
				StateMachineExchange(&bcu_state.bcu_active_intercom_state,GetEventType(bcu_state.bcu_active_intercom_state,*param_cmd_info),param_cmd_info);
				last_control_flag = control_flag;control_flag = 262;
			}
			else
			{
				memcpy((void *)param_recv_cmd_info_of_intercom,(void *)param_cmd_info,sizeof(*param_cmd_info));
			}
		}
		else
		{
			param_recv_cmd_info_of_intercom->src_devices_no = 0;
			strcpy(param_recv_cmd_info_of_intercom->src_devices_name,"");
		}
		last_control_flag = control_flag;control_flag = 263;
	}
}

/*judge whether want to exchange state*/
int WhetherWantToExchangeState(send_infomation_t send_information)
{
	int ret = 0;
	int current_event = 0;
	if(send_information.event_type_ann != ANN_IDLE)
	{
		current_event = send_information.event_type_ann;
	}
	else if(send_information.event_type_intercom != INTERCOM_IDLE)
	{
		current_event = send_information.event_type_intercom;
	}
	debug_print(("current_event = %d\n",current_event));
	switch(current_event)
	{
		case OCC_EVENT:
			if(bcu_state.bcu_active_ann_state->state_id != OCC_EVENT)
			{
				if(	send_information.event_info_ann.occ_announce.occ_active == 0)
				{
					ret = 0;
				}
				else
				{
					ret = 1;
				}
			}
			else if(send_information.event_info_ann.occ_announce.occ_active == 0)
			{
				ret = 1;
			}
			else
			{
				ret = 0;
			}
			break;
		case LIVE_ANN_EVENT:
			if(bcu_state.bcu_active_ann_state->state_id != LIVE_ANN_EVENT)
			{
				if(	send_information.event_info_ann.live_announce.live_active == 0)
				{
					ret = 0;
				}
				else
				{
					ret = 1;
				}
			}
			else if(send_information.event_info_ann.live_announce.live_active == 0)
			{
				ret = 1;
			}
			else
			{
				ret = 0;
			}
			break;
		case MIC_3D5_OUTER_EVENT:
				if(bcu_state.bcu_active_ann_state->state_id != MIC_3D5_OUTER_EVENT)
				{
					if(	send_information.event_info_ann.outer_3d5_ann.outer_3d5_active == 0)
					{
						ret = 0;
					}
					else
					{
						ret = 1;
					}
				}
				else if(send_information.event_info_ann.outer_3d5_ann.outer_3d5_active == 0)
				{
					ret = 1;
				}
				else
				{
					ret = 0;
				}
				break;
		case EMERG_ANN_EVENT:
			if(bcu_state.bcu_active_ann_state->state_id != EMERG_ANN_EVENT)
			{
				if(	send_information.event_info_ann.emerg_announce.emerg_active == 0)
				{
					ret = 0;
				}
				else
				{
					ret = 1;
				}
			}
			else if(send_information.event_info_ann.emerg_announce.emerg_active == 0)
			{
				ret = 1;
			}
			else
			{
				ret = 0;
			}
			break;
		case MANUAL_ANN_EVENT:
//			diag_printf("AAA:%d,MANUAL_ANN_EVENT = %d\n",bcu_state.bcu_active_ann_state->state_id,MANUAL_ANN_EVENT);
			if(bcu_state.bcu_active_ann_state->state_id != MANUAL_ANN_EVENT)
			{
				if(	send_information.event_info_ann.manual_annnounce.manual_active == 0)
				{
					ret = 0;
				}
				else
				{
					ret = 1;
				}

			}
			else if(send_information.event_info_ann.manual_annnounce.manual_active == 0)
			{
				ret = 1;
			}
			else
			{
				ret = 0;
			}
//			diag_printf("ret = %d\n",ret);
			break;
		case TMS_AUTO_ANN_EVENT:
			if(bcu_state.bcu_active_ann_state->state_id != TMS_AUTO_ANN_EVENT)
			{
				if(	send_information.event_info_ann.tms_auto_announce.tms_auto_active == 0)
				{
					ret = 0;
				}
				else
				{
					ret = 1;
				}
			}
			else if(send_information.event_info_ann.tms_auto_announce.tms_auto_active == 0)
			{
				ret = 1;
			}
			else
			{
				ret = 0;
			}
			debug_print(("TMS_AUTO_ANN_EVENT ret = %d\n",ret));
			break;
		case D2D_INTERCOMM_EVENT:
			if(bcu_state.bcu_active_intercom_state->state_id != D2D_INTERCOMM_EVENT &&
				bcu_state.bcu_active_intercom_state->state_id != D2D_HANGUP_D2P_EVENT)
			{
				ret = 1;
			}
			else if(send_information.event_info_intercom.d2d_intercomm.d2d_intercomm_active == 0)
			{
				ret = 1;
			}
			else
			{
				ret = 0;
			}
			break;
		case D2P_INTERCOMM_EVENT:
			if(bcu_state.bcu_active_intercom_state->state_id != D2P_INTERCOMM_EVENT)
			{
				ret = 1;
			}
			else if(send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_active == 0)
			{
				ret = 1;
			}
			else
			{
				ret = 0;
			}
			if(bcu_state.bcu_active_intercom_state->state_id != D2P_INTERCOMM_EVENT &&
			   send_information.event_type_intercom == D2P_INTERCOMM_EVENT &&
			   send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_active == 0)
			{
				ret = 0;
			}

			break;
		default:
			ret = 0;
			break;
		}
	return ret;
}

/*Judge whether have D2D request from network*/
void JudgeWhetherHaveD2DRequest(send_infomation_t *param_cmd_info,network_send_package_t *param_network_cmd_info)
{
	send_infomation_t param_send_infomation;
	if(param_network_cmd_info->send_information.event_type_intercom == D2D_INTERCOMM_EVENT &&
	   param_network_cmd_info->send_information.event_info_intercom.d2d_intercomm.d2d_intercomm_active ==1 &&
	   param_network_cmd_info->send_information.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over == 0 &&
	   param_network_cmd_info->send_information.event_info_intercom.d2d_intercomm.d2d_intercomm_response == 0)
	{
		///<begin 2013-12-18
		//bcu_state.other_bcu_ptt_state = param_network_cmd_info->send_information.event_info_intercom.d2d_intercomm.d2d_ppt_state;

		///<over 2013-12-18
		if((bcu_state.bcu_active_ann_state->state_id == LIVE_ANN_EVENT ||
				bcu_state.bcu_active_ann_state->state_id == MIC_3D5_OUTER_EVENT||
				bcu_state.bcu_active_ann_state->state_id == OCC_EVENT) &&
				bcu_state.bcu_active_intercom_state->state_id != D2P_INTERCOMM_EVENT)//提示仲裁界面
		{
			last_control_flag = control_flag;control_flag = 271;
			//ShowArbitratePage(bcu_state.bcu_active_ann_state->state_id);
			//上海业主要求直接切换-2015-6-13
			if(bcu_state.bcu_active_ann_state->state_id == LIVE_ANN_EVENT)
			{
				network_send_package_t a;
				a.send_information.event_type_ann = 2;
				a.send_information.event_type_intercom = 255;
				strcpy(a.send_information.src_devices_name,"BCU");
				a.send_information.src_devices_no=bcu_state.bcu_info.devices_no;
				a.send_information.event_info_ann.live_announce.live_active = 0;
				a.send_information.event_info_ann.live_announce.live_begin_or_over =0;
				a.send_information.event_info_ann.live_announce.live_response =0;
				bcu_state.bcu_active_ann_state->exit();
				bcu_state.bcu_active_ann_state = &idle_ann_state;
				bcu_state.bcu_active_ann_state->enter(&(a.send_information));
			}

			last_control_flag = control_flag;control_flag = 272;
		}
		if(bcu_state.bcu_active_intercom_state->state_id == D2P_INTERCOMM_EVENT)	/*There have driver request*/
		{
			/*Current state is D2P_INTERCOMM_EVENT*/
			last_control_flag = control_flag;control_flag = 273;
			if(WhetherWantToExchangeState(param_network_cmd_info->send_information))//系统转换成司机对讲状态，并且使的PCU进入Pending状态
			{
				last_control_flag = control_flag;control_flag = 274;
				memcpy((void *)&param_send_infomation,(void *)&param_network_cmd_info->send_information,sizeof(param_network_cmd_info->send_information));
				param_send_infomation.event_info_intercom.d2d_intercomm.d2d_intercomm_response = 1;


				param_send_infomation.init = NULL;/*need to modify*/
				param_send_infomation.setinformation = NULL;
				last_control_flag = control_flag;control_flag = 275;
				StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_TRANSFER_TO_D2D_OR_DRIVER_CALL,&param_send_infomation);
				last_control_flag = control_flag;control_flag = 276;
			}
			else
			{
				strcpy((char *)param_cmd_info,(char *)&param_network_cmd_info->send_information);
			}
		}
		else/*Current state is IDLE_INTERCOMM*/
		{
			last_control_flag = control_flag;control_flag = 277;
			if(WhetherWantToExchangeState(param_network_cmd_info->send_information))
			{
				last_control_flag = control_flag;control_flag = 278;
				memcpy((void *)&param_send_infomation,(void *)&param_network_cmd_info->send_information,sizeof(param_network_cmd_info->send_information));
				param_send_infomation.event_info_intercom.d2d_intercomm.d2d_intercomm_response = 1;

				param_send_infomation.init = NULL;/*need to modify*/

				param_send_infomation.setinformation = NULL;
				last_control_flag = control_flag;control_flag = 279;
				StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_PTT_OR_DRIVER_CALL,&param_send_infomation);
				last_control_flag = control_flag;control_flag = 280;
			}
			else
			{
				strcpy((char *)param_cmd_info,(char *)&param_network_cmd_info->send_information);
			}
		}
	}
	else if(param_network_cmd_info->send_information.event_type_intercom == D2D_INTERCOMM_EVENT &&
		   param_network_cmd_info->send_information.event_info_intercom.d2d_intercomm.d2d_intercomm_active ==0)
		{
			if(bcu_state.bcu_active_intercom_state->state_id == D2D_INTERCOMM_EVENT)
			{
				bcu_state.other_bcu_ptt_state =  0;
				last_control_flag = control_flag;control_flag = 281;
				NetFinishD2D();
				last_control_flag = control_flag;control_flag = 282;
			}
			else if(bcu_state.bcu_active_intercom_state->state_id == D2D_HANGUP_D2P_EVENT)
			{
				bcu_state.other_bcu_ptt_state =  0;
				last_control_flag = control_flag;control_flag = 283;
				judgeWheteherExitD2DEnterD2P();
				last_control_flag = control_flag;control_flag = 284;
			}
			last_control_flag = control_flag;control_flag = 285;
//			HideArbitratePage();
			last_control_flag = control_flag;control_flag = 286;
		}
}
void SendPTTStateToPCU()
{///<发送当前BCU的PTT状态给PCU，只有在当前PTT状态与上次检测不同的情况下才会发送
	int current_ptt_state = GetPTTState();
	if(bcu_state.this_bcu_ptt_state != current_ptt_state)
	{
		last_sample_flag = sample_flag;sample_flag = 182;
		bcu_state.this_bcu_ptt_state = current_ptt_state;
		common_package_t common_package_bcu_ppt_state;
		common_package_bcu_ppt_state.pkg_type = COMMON_PACKAGE_TYPE_PTT_STATE;
		strcpy(common_package_bcu_ppt_state.src_dev_name,bcu_state.bcu_info.devices_name);
		common_package_bcu_ppt_state.src_dev_number = bcu_state.bcu_info.devices_no;

		strcpy(common_package_bcu_ppt_state.dst_dev_name,"PCU");
		common_package_bcu_ppt_state.dst_dev_number = bcu_state.pcu_request_info.recept_pcu_no;

		common_package_bcu_ppt_state.common_data_u.comm_ptt_state.dst_ptt = bcu_state.this_bcu_ptt_state;
		common_package_bcu_ppt_state.common_data_u.comm_ptt_state.src_ptt = bcu_state.this_bcu_ptt_state;
		diag_printf("!-%d-%d\n",bcu_state.this_bcu_ptt_state,common_package_bcu_ppt_state.dst_dev_number);
		int ret = 0,write_counts = 0;
		last_sample_flag = sample_flag;sample_flag = 183;
		do
		{
			ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id,&common_package_bcu_ppt_state,sizeof(common_package_bcu_ppt_state));
			write_counts ++;
			if(ret <= 0)
			{
				cyg_thread_delay(10);
			}
		}while(ret <= 0 && write_counts < 2);
		last_sample_flag = sample_flag;sample_flag = 184;
	}
}

void SendCurrentBCUPTTStateToPCU()
{///<发送当前BCU的PTT状态给PCU
	int current_ptt_state = GetPTTState();
	bcu_state.this_bcu_ptt_state = current_ptt_state;
	common_package_t common_package_bcu_ppt_state;
	common_package_bcu_ppt_state.pkg_type = COMMON_PACKAGE_TYPE_PTT_STATE;
	strcpy(common_package_bcu_ppt_state.src_dev_name,bcu_state.bcu_info.devices_name);
	common_package_bcu_ppt_state.src_dev_number = bcu_state.bcu_info.devices_no;

	strcpy(common_package_bcu_ppt_state.dst_dev_name,"PCU");
	common_package_bcu_ppt_state.dst_dev_number = bcu_state.pcu_request_info.recept_pcu_no;

	common_package_bcu_ppt_state.common_data_u.comm_ptt_state.dst_ptt = bcu_state.this_bcu_ptt_state;
	common_package_bcu_ppt_state.common_data_u.comm_ptt_state.src_ptt = bcu_state.this_bcu_ptt_state;
	diag_printf("!-%d\n",bcu_state.this_bcu_ptt_state);
	int ret = 0,write_counts = 0;
	do
	{
		ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id,&common_package_bcu_ppt_state,sizeof(common_package_bcu_ppt_state));
		write_counts ++;
		if(ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(ret <= 0 && write_counts < 2);
}

/*send ptt state to bcu*/
void SendPTTStateToBCU()
{///<发BCU的PTT状态
	int current_ptt_state = GetPTTState();
	last_sample_flag = sample_flag;sample_flag = 191;
	if(bcu_state.this_bcu_ptt_state != current_ptt_state)
	{
		last_sample_flag = sample_flag;sample_flag = 192;
		bcu_state.this_bcu_ptt_state = current_ptt_state;
		if(bcu_state.this_bcu_ptt_state == 0)//PTT松开
		{
			last_sample_flag = sample_flag;sample_flag = 193;
			debug_print(("ppt release to bcu\n"));
			BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id,&network_send_package_ptt_release_to_bcu,sizeof(network_send_package_ptt_release_to_bcu));
		}
		else//PTT摁下
		{
			last_sample_flag = sample_flag;sample_flag = 194;
			debug_print(("ppt press to bcu,%d\n",network_send_package_ptt_press_to_bcu.common_data_u.comm_ptt_state.src_ptt));
			BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id,&network_send_package_ptt_press_to_bcu,sizeof(network_send_package_ptt_press_to_bcu));
		}
		last_sample_flag = sample_flag;sample_flag = 195;
		bcu_state.this_bcu_ptt_state = current_ptt_state;
		last_sample_flag = sample_flag;sample_flag = 196;
	}
	last_sample_flag = sample_flag;sample_flag = 197;
}
void SendPTTStateToBCU_Once()
{///<只发一次BCU的PTT状态
	int current_ptt_state = GetPTTState();
		bcu_state.this_bcu_ptt_state = current_ptt_state;
		if(bcu_state.this_bcu_ptt_state == 0)//PTT松开
		{
			debug_print(("ppt release to bcu\n"));
			BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id,&network_send_package_ptt_release_to_bcu,sizeof(network_send_package_ptt_release_to_bcu));
		}
		else//PTT摁下
		{
			debug_print(("ppt press to bcu\n"));
			BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id,&network_send_package_ptt_press_to_bcu,sizeof(network_send_package_ptt_press_to_bcu));
		}
}
/*Acquire other bcu ptt state*/
void ReadOtherBCUPttState()
{///<获取对端BCU的PTT状态
	common_package_t temp_ptt_state;
	if(BlockBufferRead(bcu_state.ppt_state_recv_buffer_id,&temp_ptt_state,sizeof(temp_ptt_state)) > 0)
	{
		last_control_flag = control_flag;control_flag= 302;
		debug_print(("!!!!!ReadOtherBCUPttState!!!!!!RRRRRR!!!!!-%d\n",temp_ptt_state.common_data_u.comm_ptt_state.src_ptt));
		bcu_state.other_bcu_ptt_state = temp_ptt_state.common_data_u.comm_ptt_state.src_ptt;
		debug_print(("ReadOtherBCUPttState:other_bcu_ptt_state = %d\n",bcu_state.other_bcu_ptt_state));
	}
	last_control_flag = control_flag;control_flag = 303;
	return ;
}


/*Acquire event type*/
int GetEventType(state_process_t *state_process,send_infomation_t screen_cmd)
{
#if 1
    int sig = 0;
	int event_type_ann = screen_cmd.event_type_ann;
	int event_type_intercom = screen_cmd.event_type_intercom;

	debug_print(("event_type_ann = %d,event_type_intercom = %d\n",event_type_ann,event_type_intercom));

	if (event_type_ann != ANN_IDLE )
	{
		switch( event_type_ann )
		{
			case OCC_EVENT:
				if( screen_cmd.event_info_ann.occ_announce.occ_active == 1 )
				{
					sig = EVENT_OCC_COMMAND;
				}
				else
				{
					sig = EVENT_OCC_RELEASE;
				}
				break;
			case LIVE_ANN_EVENT:
				if( screen_cmd.event_info_ann.live_announce.live_active == 1 )
				{
					sig = EVENT_LIVE_BUTTON_PRESS;
					return sig;
				}
				else
				{
					sig = EVENT_RETURN_BUTTON_PRESS;
					return sig;
				}
				break;
           case EMERG_ANN_EVENT:
				if( screen_cmd.event_info_ann.emerg_announce.emerg_active == 1 )
				{
					sig = EVENT_EMERGENCY_BUTTON_PRESS;
					return sig;
				}
				else
				{
					sig = EVENT_RETURN_BUTTON_PRESS;
					return sig;
				}
				break;
           case MANUAL_ANN_EVENT:
        	   if(state_process->state_id == ANN_IDLE)
        	   {
					if( screen_cmd.event_info_ann.manual_annnounce.manual_active == 1 )
					{
						sig = EVENT_MANUAL_MODE;
					}
					else
					{
						sig = EVENT_RETURN_BUTTON_PRESS;
					}
        	   }
        	   else if(state_process->state_id == TMS_AUTO_ANN_EVENT)
        	   {
	  				if( screen_cmd.event_info_ann.manual_annnounce.manual_begin_or_over == 0 )
					{
						sig = EVENT_TRANSFER_TO_MANUAL;
					}
					else
					{
						sig = EVENT_RETURN_BUTTON_PRESS;
					}
        	   }
        	   else if(state_process->state_id == MANUAL_ANN_EVENT)
        	   {
        		   if( screen_cmd.event_info_ann.manual_annnounce.manual_active == 1 )
					{
						sig = EVENT_MANUAL_MODE;
					}
					else
					{
						sig = EVENT_RETURN_BUTTON_PRESS;
					}
        	   }
        	   else
        	   {

        	   }
				break;
           case TMS_MANUAL_ANN_EVENT:
				if( screen_cmd.event_info_ann.tms_manual_announce.tms_manual_active == 1 )
				{
					sig = 100;
				}
				else
				{
					sig = 100;
				}
				break;
           case TMS_AUTO_ANN_EVENT:
        	   debug_print(("TMS_AUTO_ANN_EVENT,tms_auto_active = %d\n", screen_cmd.event_info_ann.tms_auto_announce.tms_auto_active));
           	 if( screen_cmd.event_info_ann.tms_auto_announce.tms_auto_active == 1 )
				 {
				    	sig = EVENT_TMS_SIGNAL;
			     }
				 else
				 {
				    	sig = EVENT_TMS_SIGNAL_OVER;
				 }
           	 break;
           default:
           	 sig = EVENT_MAX;
           	 break;
		}
	}
	if (event_type_intercom != INTERCOM_IDLE )
	{
		switch(event_type_intercom)
		{
			case D2D_INTERCOMM_EVENT:
				if(state_process->state_id == D2P_INTERCOMM_EVENT)
				{
					if( screen_cmd.event_info_intercom.d2d_intercomm.d2d_intercomm_active == 1 )
					{
						sig = EVENT_TRANSFER_TO_D2D_OR_DRIVER_CALL;
						return sig;
					}
					else
					{
						sig = EVENT_CONFIRM_TRANSFER_TO_D2P;
					}
				}
				else
				{
					if( screen_cmd.event_info_intercom.d2d_intercomm.d2d_intercomm_active == 1 )
					{
						sig = EVENT_PTT_OR_DRIVER_CALL;
					}
					else
					{
						sig = EVENT_CONFIRM_TRANSFER_TO_D2P;
					}
				}
				break;
			case D2P_INTERCOMM_EVENT:
				if( screen_cmd.event_info_intercom.d2p_intercomm.d2p_intercomm_active == 1 )
				{
					sig = EVENT_CONFIRM_PCU_CALL;
				}
				else
				{
					sig = EVENT_CONFIRM_RELEASE_PCU;
				}
				break;
			default:
				sig = EVENT_MAX;
				break;
		}
	}
	return sig;
#else
	return 0;
#endif
}

/*BCU send response cmd to BCU*/
void SendCmd(send_infomation_t **send_cmd_info,char *dst_device_name,int dst_device_no)
{
#if 1
	if(send_cmd_info ==  NULL)///<2013-12-31
	{
		return ;
	}
	if((*send_cmd_info)->src_devices_no == 0 && strcmp((*send_cmd_info)->src_devices_name,"") == 0)
	{
		return ;
	}
	else
	{
		network_send_package_t network_send_information;
		(*send_cmd_info)->src_devices_no = bcu_state.bcu_info.devices_no;
		strcpy((*send_cmd_info)->src_devices_name,bcu_state.bcu_info.devices_name);

		diag_printf("dst:%s,%d\n",dst_device_name,dst_device_no);
		memcpy((void *)&network_send_information.send_information,(void *)&(**send_cmd_info),sizeof(*(*send_cmd_info)));
	  diag_printf("--------------------\n");
	   strcpy(network_send_information.dst_devices_name,dst_device_name);
	   network_send_information.dst_devices_no = dst_device_no;
	   int ret = -1,write_counts = 0;
	   do
	   {
		ret = BlockBufferWrite(bcu_state.cmd_send_buffer_id,&network_send_information,sizeof(network_send_information));
		write_counts ++;
		if(ret <= 0)
		{
			cyg_thread_delay(10);
		}
		diag_printf("SendCmd:%s_%d\n",network_send_information.dst_devices_name,network_send_information.dst_devices_no);
	   }while(ret <= 0 && write_counts < 2);

		(*send_cmd_info)->src_devices_no = 0;
		strcpy((char *)&(*send_cmd_info)->src_devices_name,"");
		(*send_cmd_info)->event_type_ann = ANN_IDLE;
		(*send_cmd_info)->event_type_intercom = INTERCOM_IDLE;
		memset((void *)&(*send_cmd_info)->event_info_ann,0,sizeof((*send_cmd_info)->event_info_ann));
		memset((void *)&(*send_cmd_info)->event_info_intercom,0,sizeof((*send_cmd_info)->event_info_intercom));
	}
#endif
	return ;
}

/*Show the station information*/
void ShowStationInformation(int line_number_of_skip_station)
{///<显示站信息
#if 1
	int i = 0;
	for(i = 0; i < MAX_STATION_NUMBER; i++)
	{
		if(station_information[line_number_of_skip_station - 3][i].effective != 1)
		{
			debug_print(("line:%d_%d:%d\n",line_number_of_skip_station,i,station_information[line_number_of_skip_station - 3][i].effective));//
		}
	}
#endif
}

void SetSrcDevInfo(char name[],unsigned char number)
{
	sprintf(src_device_info,"%s%d",name,number);
}

void SetThisDevInfo(char name[],unsigned char number)
{
	sprintf(this_device_info,"%s%d",name,number);
}
///<获取配置的录音功能模块
void UpdataSettingSaveLiveAudioData(unsigned char param_data)
{
	whether_save_live_audio_data = param_data;
}
void UpdataSettingSaveD2DAudioData(unsigned char param_data)
{
	whether_save_d2d_audio_data = param_data;
}

void UpdataSettingSaveD2PAudioData(unsigned char param_data)
{
	whether_save_d2p_audio_data = param_data;
}

void StartSaveAudioData(int param_attribution,send_infomation_t *param_send_info,char param_event_type[])
{
#if 0
	if(strcmp((char *)param_event_type,"LIVE") == 0)
	{
		if(param_attribution == 1)
		{
			SetSrcDevInfo(param_send_info->src_devices_name,param_send_info->src_devices_no);

			SetAudioDataLogFileName(audio_data_log_file_name,src_device_info,this_device_info,param_event_type);

			fp_audio_data_log_file = OpenAudioDataFile(audio_data_log_file_name,"w");

		}
	}
	else if(strcmp((char *)param_event_type,"D2D") == 0)
	{
		if(param_attribution == 1)
		{
			SetSrcDevInfo(param_send_info->src_devices_name,param_send_info->src_devices_no);

			SetAudioDataLogFileName(audio_data_log_file_name,src_device_info,this_device_info,"D2P");

			fp_audio_data_log_file = OpenAudioDataFile(audio_data_log_file_name,"w");
		}
	}
	else if(strcmp((char *)param_event_type,"D2P") == 0)
	{
		if(param_attribution == 1)
		{
			SetSrcDevInfo(param_send_info->src_devices_name,param_send_info->src_devices_no);

			SetAudioDataLogFileName(audio_data_log_file_name,src_device_info,this_device_info,"D2P");

			fp_audio_data_log_file = OpenAudioDataFile(audio_data_log_file_name,"w");
		}
	}
	else
	{
	}

//	WriteAudioDataToLog(fp_audio_data_log_file,RIFFHeader0);
#endif
}

void EndSaveAudioData(int param_attribution,char param_event_type[])
{
#if 0
	if(strcmp((char *)param_event_type,"LIVE") == 0)
		{
			if(param_attribution == 1)
			{
				CloseAudioDataFile(fp_audio_data_log_file);

				memset(audio_data_log_file_name,0x00,sizeof(audio_data_log_file_name));
			}
		}
		else if(strcmp((char *)param_event_type,"D2D") == 0)
		{
			if(param_attribution == 1)
			{
				CloseAudioDataFile(fp_audio_data_log_file);

				memset(audio_data_log_file_name,0x00,sizeof(audio_data_log_file_name));
			}
		}
		else if(strcmp((char *)param_event_type,"D2P") == 0)
		{
			if(param_attribution == 1)
			{
				CloseAudioDataFile(fp_audio_data_log_file);

				memset(audio_data_log_file_name,0x00,sizeof(audio_data_log_file_name));
			}
		}
#endif
}

void JudgeWhetherHaveFinishedAcquireData()
{///<判断当前是否完成数据库数据获取
	diag_printf("Have finished update\n");

	BCUSetDataFromCCUState(0);

//	UpdateDeviceVolumePageInfo(255);

	///<显示请插入钥匙界面
#ifndef NEW_TS_STRATEGE
//	Fl::wait();
#endif
	//title_wait_for_get_data->label("");///<2014-09-04
	//title_wait_for_get_data->show();///<2014-09-04
	//gp_wait_for_get_data->hide();///<2014-09-04
//	wz_window_view->value(gp_static_show);


	//wz_select_window->deactivate();///<2014-09-04
#ifndef NEW_TS_STRATEGE
//	title_please_input_key->label((char *)ts_key_info_buffer);
//	title_please_input_key->show();
#endif

#ifndef NEW_TS_STRATEGE
//	Fl::wait();
#endif
	//wd_touch_screen->show();///<2014-09-04
}
void ClearProcessPackage(send_infomation_t *p_send_info)
{///<清空状态处理包
#if 1
	if(p_send_info->src_devices_no == 0 && strcmp(p_send_info->src_devices_name,"") == 0)
	{
		return ;
	}
	p_send_info->src_devices_no = 0;
	strcpy((char *)p_send_info->src_devices_name,"");
	p_send_info->event_type_ann = ANN_IDLE;
	p_send_info->event_type_intercom = INTERCOM_IDLE;
	memset((void *)&(p_send_info->event_info_ann),0,sizeof(p_send_info->event_info_ann));
	memset((void *)&(p_send_info->event_info_intercom),0,sizeof(p_send_info->event_info_intercom));
#endif
}
void SendBCUPTTPressStateTOBCU()
{///<发送BCU，本端BCU的PTT 已经按下
#if 1
	debug_print(("This BCU PTT is pressed\n"));
	bcu_state.this_bcu_ptt_state = 1;
	BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id,&network_send_package_ptt_press_to_bcu,sizeof(network_send_package_ptt_press_to_bcu));
#endif
}
void ClearSkipStationInfo(int line_number_of_skip_station)
{///<清除所有越站信息
#if 1
	int i = 0;
	for(i = 0; i < MAX_STATION_NUMBER; i++)
	{
		if(station_information[line_number_of_skip_station - 3][i].effective != 2)
		{
			station_information[line_number_of_skip_station - 3][i].effective = 1;
		}
	}
#endif
}
void SendBCUPTTReleaseStateTOBCU()
{///<告知BCU，本端BCU的PTT 已经松开
#if 1
	debug_print(("This BCU PTT is released\n"));
	bcu_state.this_bcu_ptt_state = 0;
	BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id,&network_send_package_ptt_release_to_bcu,sizeof(network_send_package_ptt_release_to_bcu));
#endif
}
void ControlCommomPackageInit(common_package_t *p_to_eamp,common_package_t *p_to_pcu,common_package_t *p_to_bcu,common_package_t *p_to_ccu)
{///<通用控制命令包初始化
#if 1
	p_to_eamp->dst_dev_number = MUL_SYNC_DST_NO;
	strcpy(p_to_eamp->dst_dev_name,"EAMP");
	p_to_eamp->pkg_type = COMMON_PACKAGE_TYPE_DEVICE_INFO;
	strcpy(p_to_eamp->src_dev_name,bcu_state.bcu_info.devices_name);
	p_to_eamp->src_dev_number = bcu_state.bcu_info.devices_no;

	p_to_pcu->dst_dev_number = MUL_SYNC_DST_NO;
	strcpy(p_to_pcu->dst_dev_name,"PCU");
	strcpy(p_to_pcu->common_data_u.ts_dev_volume_info.device_name,"PCU");
	p_to_pcu->pkg_type = COMMON_PACKAGE_TYPE_DEVICE_INFO;
	strcpy(p_to_pcu->src_dev_name,bcu_state.bcu_info.devices_name);
	p_to_pcu->src_dev_number = bcu_state.bcu_info.devices_no;
//	p_to_pcu->common_data_u.ts_dev_volume_info.device_adjust_volume = 10;
	p_to_pcu->common_data_u.ts_dev_volume_info.device_no_recept_timeout = 10;

	p_to_bcu->dst_dev_number = bcu_state.opposite_bcu_no;
	strcpy(p_to_bcu->dst_dev_name,"BCU");
	p_to_bcu->pkg_type = COMMON_PACKAGE_TYPE_DEVICE_INFO;
	strcpy(p_to_bcu->src_dev_name,bcu_state.bcu_info.devices_name);
	p_to_bcu->src_dev_number = bcu_state.bcu_info.devices_no;

	p_to_ccu->dst_dev_number = MUL_SYNC_DST_NO;
	strcpy(p_to_ccu->dst_dev_name,"CCU");
	p_to_ccu->pkg_type = COMMON_PACKAGE_TYPE_DEVICE_INFO;
	strcpy(p_to_ccu->src_dev_name,bcu_state.bcu_info.devices_name);
	p_to_ccu->src_dev_number = bcu_state.bcu_info.devices_no;
#endif
}


int GetBCUNetworkState()
{///<获取网络状态
	return 0;
}

/*----------------end of bcu beta 1.1----------------------*/
void OpenMonitorSpeaker()
{///<打开监听扬声器
	common_package_t speaker_control_info;
	strcpy(speaker_control_info.dst_dev_name,"EAMP");
	speaker_control_info.dst_dev_number = MUL_DST_NO;
	speaker_control_info.pkg_type = COMMON_PACKAGE_TYPE_MONITOR_SPEAKER_CONTROL;
	strcpy(speaker_control_info.src_dev_name,bcu_state.bcu_info.devices_name);
	speaker_control_info.src_dev_number = bcu_state.bcu_info.devices_no;
	speaker_control_info.common_data_u.monitor_speaker_control.monitor_speaker_state = 3;
	BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &speaker_control_info, sizeof(speaker_control_info));
}

void CloseMonitorSpeaker()
{///<关闭监听扬声器
	common_package_t speaker_control_info;
	strcpy(speaker_control_info.dst_dev_name,"EAMP");
	speaker_control_info.dst_dev_number = MUL_DST_NO;
	speaker_control_info.pkg_type = COMMON_PACKAGE_TYPE_MONITOR_SPEAKER_CONTROL;
	strcpy(speaker_control_info.src_dev_name,bcu_state.bcu_info.devices_name);
	speaker_control_info.src_dev_number = bcu_state.bcu_info.devices_no;
	speaker_control_info.common_data_u.monitor_speaker_control.monitor_speaker_state = 0;
	BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &speaker_control_info, sizeof(speaker_control_info));
}

void TouchScreenShowLockInfo(void)
{
	if(bcu_state.this_bcu_is_active  == 0)
	{
		lock->hide();
		lock->label("\345\267\262\351\224\201\345\256\232");
		lock->redraw();
		lock->show();
	}
	else if(bcu_state.this_bcu_is_active  == 1)
	{
		lock->hide();
		lock->label("\345\267\262\350\247\243\351\224\201");
		lock->redraw();
		lock->show();
	}
	
}

void TouchScreenShowPCUInfo()
{
	char *str_temp[13] = {"","TC1-1","TC1-2","MP1-1","MP1-2","M1-1","M1-2","M2-1","M2-2",
			"MP2-1","MP2-2","TC2-1","TC2-2"};
	if(whether_show_pcu_info == 0)
	{
		whether_show_pcu_info = 1;
		pcu_recept->label("PCU");
		pcu_recept->redraw();
		pcu_value->label(str_temp[bcu_state.pcu_request_info.recept_pcu_no]);
		pcu_value->redraw();
	}
	else if(whether_show_pcu_info == 1)
	{
		whether_show_pcu_info = 0;
		pcu_recept->label("");
		pcu_recept->redraw();
		pcu_value->label("");
		pcu_value->redraw();
	}
}


void AlarmTSToChangeScreen(int param)
{///<通知触摸屏，其他线程需要改变触摸屏界面
	switchFlag = param;
	while(switchFlag == param)
	{
		diag_printf("touch screen is not response-01-%d\n",param);
		cyg_thread_delay(10);
		diag_printf("touch screen is not response-02\n");
	}
}

void TouchScreenShowStationInfo(int flag)
{
	unsigned char station_no;
	diag_printf("$$$$ start_station_no %d\n",bcu_state.region_station_info.
os_begin_station_no);
	diag_printf("$$$$ over_station_no %d\n",bcu_state.region_station_info.
os_terminal_station_no);	

	station_no = bcu_state.region_station_info.os_begin_station_no;
	start_value->label(station_information[1][station_no].station_chinese_name);
	start_value->redraw();
	
	station_no = bcu_state.region_station_info.os_terminal_station_no;
	end_value->label(station_information[1][station_no].station_chinese_name);
	end_value->redraw();

	station_no = bcu_state.region_station_info.os_current_station_no;
	if(station_no == 0)
	{
		current_value->label("----");
	}
	else
	{
		current_value->label(station_information[1][station_no].station_chinese_name);
	}
	current_value->redraw();
	
	if(0==bcu_state.region_station_info.os_door_side)
	{
		open_value->label("左门");
	}
	else if(1==bcu_state.region_station_info.os_door_side)
	{
		open_value->label("右门");
	}
	else if(2==bcu_state.region_station_info.os_door_side)
	{
		open_value->label("双侧车门");
	}
	open_value->redraw();

	station_no = bcu_state.region_station_info.os_next_station_no;
	if(station_no == 0)
	{
		next_value->label("----");
	}
	else
	{
		next_value->label(station_information[1][station_no].station_chinese_name);
	}
	next_value->redraw();

	if(flag == 0)
	{
		current_value->label("");
		current_value->redraw();
		next_value->label("");
		next_value->redraw();
		open_value->label("");
		open_value->redraw();
	}

}

required_station_info_t station_info_from_network;

void ShowCurrentTime(int param_no)
{
	struct timeval tp_log;
	gettimeofday(&tp_log,NULL);
	diag_printf("%d:%d.%d\n",param_no,tp_log.tv_sec,(tp_log.tv_usec));
}
void ShowRegionInfo(int param_line_no)
{

	diag_printf("line%d:region_no = %d\n",
			param_line_no,
			bcu_state.bcu_region_info[param_line_no - 3].current_region_info.region_no);
	diag_printf("line%d:start_station_no = %d\n",
			param_line_no,
			bcu_state.bcu_region_info[param_line_no - 3].current_region_info.start_station_no);
	diag_printf("line%d:over_station_no = %d\n",param_line_no,bcu_state.bcu_region_info[param_line_no - 3].current_region_info.over_station_no);
	diag_printf("line%d:running_direction = %d\n",param_line_no,bcu_state.bcu_region_info[param_line_no - 3].current_region_info.running_direction);
	diag_printf("line%d:whether_the_cycle_bus = %d\n",param_line_no,bcu_state.bcu_region_info[param_line_no - 3].current_region_info.whether_the_cycle_bus);
}
void ShowAllRegionInfo(int param_line_no)
{///<显示所有区间信息
	int k = 0;
	diag_printf("AA-%d\n",bcu_state.bcu_region_info[param_line_no - 3].total_regions_number);
	for(k = 0;k < bcu_state.bcu_region_info[param_line_no - 3].total_regions_number;k++)
	{
		diag_printf("line%d:region_no = %d\n",
				param_line_no,(bcu_state.bcu_region_info[param_line_no - 3].p_regin_info + k)->region_no);
		diag_printf("line%d:start_station_no = %d\n",
						param_line_no,(bcu_state.bcu_region_info[param_line_no - 3].p_regin_info + k)->start_station_no);
		diag_printf("line%d:over_station_no = %d\n",
						param_line_no,(bcu_state.bcu_region_info[param_line_no - 3].p_regin_info + k)->over_station_no);
		diag_printf("line%d:running_direction = %d\n",
						param_line_no,(bcu_state.bcu_region_info[param_line_no - 3].p_regin_info + k)->running_direction);
		diag_printf("line%d:whether_the_cycle_bus = %d\n",
						param_line_no,(bcu_state.bcu_region_info[param_line_no - 3].p_regin_info + k)->whether_the_cycle_bus);
	}
	diag_printf("@@@@@@@@@@@@@@@@\n");
}

void alarm_func_handle_delay_d2d(cyg_handle_t counter_handle, cyg_addrword_t data)
{
	whether_could_be_finished_d2d = 1;
}
void alarm_func_handle_get_out3d5_button_state(cyg_handle_t counter_handle, cyg_addrword_t data)
{
	whether_get_out3d5_button_state = 1;
}

void CreateDelayD2DTimer()
{
	cyg_clock_to_counter(cyg_real_time_clock(),&counter_handle_delay_d2d);
	cyg_alarm_create(counter_handle_delay_d2d,alarm_func_handle_delay_d2d,0,
			          &alarm_handle_delay_d2d,&alarm_object_delay_d2d);
	cyg_alarm_initialize(alarm_handle_delay_d2d,cyg_current_time()+200,200);
	cyg_alarm_disable(alarm_handle_delay_d2d);
}

void CreateGetOut3D5ButtonStateTimer()
{
	cyg_clock_to_counter(cyg_real_time_clock(),&counter_handle_get_out3d5_button_state);
	cyg_alarm_create(counter_handle_get_out3d5_button_state,alarm_func_handle_get_out3d5_button_state,0,
			          &alarm_handle_get_out3d5_button_state,&alarm_object_get_out3d5_button_state);
	cyg_alarm_initialize(alarm_handle_get_out3d5_button_state,cyg_current_time()+5,5);
	cyg_alarm_enable(alarm_handle_get_out3d5_button_state);
}
void StartDelayD2DTimer()
{
	cyg_alarm_initialize(alarm_handle_delay_d2d,cyg_current_time()+200,200);
	cyg_alarm_enable(alarm_handle_delay_d2d);
	whether_could_be_finished_d2d = 0;
}
void StopDelayD2DTimer()
{
	cyg_alarm_disable(alarm_handle_delay_d2d);
	whether_could_be_finished_d2d = 0;
}
extern int switchFlag;

void ANAHandle()
{
	if(!hal_gpio_get_pin(CYGHWR_HAL_KINETIS_PIN(A, 26, 1, KINETIS_PIN_PULLUP)))
	{
		hal_gpio_pin_set( CYGHWR_HAL_KINETIS_PIN(A, 25, 1, KINETIS_PIN_PULLUP) );
		hal_gpio_pin_set( CYGHWR_HAL_KINETIS_PIN(F, 24, 1, KINETIS_PIN_PULLUP) );
	}
	else
	{
		hal_gpio_pin_clear( CYGHWR_HAL_KINETIS_PIN(A, 25, 1, KINETIS_PIN_PULLUP) );
		hal_gpio_pin_clear( CYGHWR_HAL_KINETIS_PIN(F, 24, 1, KINETIS_PIN_PULLUP) );
	}
}

void MRHandle()
{
	if(GetMRExternButtonState() == 1)
	{
		if(bcu_state.mr_button_state == 0)
		{
			bcu_state.mr_button_state = 1;
			OpenMonitorSpeaker();
			//BCU_LED_BUTTON4_ON;
		}
		else
		{
			bcu_state.mr_button_state = 0;
			CloseMonitorSpeaker();
			//BCU_LED_BUTTON4_DIS;
		}	
	}
}


void PCHandle()
{
	network_send_package_t a;

	int dst_no = 0;
	if(bcu_state.pcu_request_info.request_number == 0)
	{
		return;
	}
	if(bcu_state.pcu_request_info.request_number != 0)
	{
		int i  = 0;
		for(i = 0;i < 12;i++)
		{
			if(bcu_state.pcu_request_info_without_ts[i] == 1)
			//if(bcu_state.pcu_request_info_without_ts[i] > 0 )
			{
				dst_no = i + 1;
				break;
			}
		}
	}
	if(dst_no <= 0 || dst_no > 12)
	{
		//diag_printf("Exit:dst_no = %d\n",dst_no);
		return;
	}
	if(GetD2PExternButtonState() == 1)
	{
		if(bcu_state.d2p_button_state == 0)///<enter d2p
		{
			if(bcu_state.bcu_active_ann_state->state_id == 2 &&
				bcu_state.bcu_active_intercom_state->state_id == 255)
			{
				ExitFromLiveWithoutTS();
			}
			else if(bcu_state.bcu_active_ann_state->state_id == 254 &&
			   bcu_state.bcu_active_intercom_state->state_id == 255)
			{

			}
			else
			{
				diag_printf("Exit:bcu_state.bcu_active_ann_state->state_id = %d\n",bcu_state.bcu_active_ann_state->state_id);
				diag_printf("Exit:bcu_state.bcu_active_intercom_state->state_id = %d\n",bcu_state.bcu_active_intercom_state->state_id);
				return;
			}
			a.send_information.event_type_ann = 254;
			a.send_information.event_type_intercom = D2P_INTERCOMM_EVENT;
			strcpy(a.send_information.src_devices_name,"BCU");
			a.send_information.src_devices_no=bcu_state.bcu_info.devices_no;
			a.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_active = 1;
			a.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_bcu_device_no = bcu_state.bcu_info.devices_no;
			a.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no = dst_no;
			a.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_request_or_over = 0;
			bcu_state.pcu_request_info.recept_pcu_no = dst_no;
			bcu_state.bcu_active_intercom_state->exit();
			bcu_state.bcu_active_intercom_state = &d2p_intercomm_state;
			bcu_state.bcu_active_intercom_state->enter(&(a.send_information));
			bcu_state.d2p_button_state =1 ;
		}
		else if(bcu_state.d2p_button_state == 1)///<exit d2p
		{
			diag_printf("Eeeeeeeeeeeeeeeeeee-%d\n",dst_no);
			a.send_information.event_type_ann = 254;
			a.send_information.event_type_intercom = D2P_INTERCOMM_EVENT;
			strcpy(a.send_information.src_devices_name,"BCU");
			a.send_information.src_devices_no=bcu_state.bcu_info.devices_no;
			a.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_active = 0;
			a.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_bcu_device_no = bcu_state.bcu_info.devices_no;
			a.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no = dst_no;
			a.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_request_or_over = 1;
			bcu_state.pcu_request_info.recept_pcu_no = dst_no;
			bcu_state.pcu_request_info.refuse_pcu_no = dst_no;
			bcu_state.bcu_active_intercom_state->exit();
			bcu_state.bcu_active_intercom_state = &idle_intercom_state;
			bcu_state.bcu_active_intercom_state->enter(&(a.send_information));
			bcu_state.d2p_button_state = 0;
		}
	}
}
void ExchangeToLiveWithoutTS()
{
	network_send_package_t a;
	a.send_information.event_type_ann = 2;
	a.send_information.event_type_intercom = 255;
	strcpy(a.send_information.src_devices_name,"BCU");
	a.send_information.src_devices_no=bcu_state.bcu_info.devices_no;
	a.send_information.event_info_ann.live_announce.live_active = 1;
	a.send_information.event_info_ann.live_announce.live_begin_or_over =1;
	a.send_information.event_info_ann.live_announce.live_response =0;

	bcu_state.bcu_active_ann_state->exit();
	bcu_state.bcu_active_ann_state = &live_annoucement_state;
	bcu_state.bcu_active_ann_state->enter(&(a.send_information));
}
void ExitFromLiveWithoutTS()
{
	network_send_package_t a;
	a.send_information.event_type_ann = 2;
	a.send_information.event_type_intercom = 255;
	strcpy(a.send_information.src_devices_name,"BCU");
	a.send_information.src_devices_no=bcu_state.bcu_info.devices_no;
	a.send_information.event_info_ann.live_announce.live_active = 0;
	a.send_information.event_info_ann.live_announce.live_begin_or_over =0;
	a.send_information.event_info_ann.live_announce.live_response =0;
	bcu_state.bcu_active_ann_state->exit();
	bcu_state.bcu_active_ann_state = &idle_ann_state;
	bcu_state.bcu_active_ann_state->enter(&(a.send_information));
}
void PAHandle()
{///<口播处理
	if(bcu_state.bcu_active_intercom_state->state_id != 255)
	{
		return;
	}
	int current_pa_state = GetLiveExternButtonState();
	
	if(current_pa_state == 1)
	{
		hal_set_pin_function( CYGHWR_HAL_KINETIS_PIN(F, 25, 1, KINETIS_PIN_PULLUP) );
		hal_gpio_pin_ddr_out( CYGHWR_HAL_KINETIS_PIN(F, 25, 1, KINETIS_PIN_PULLUP) );
		hal_gpio_pin_clear( CYGHWR_HAL_KINETIS_PIN(F, 25, 1, KINETIS_PIN_PULLUP) );
		bcu_state.tcms_live_ann_state = 0;
		if(bcu_state.live_button_state == 0)///<enter live
		{
			ExchangeToLiveWithoutTS();
			
		}
	}
	if(current_pa_state==0)
	{
		hal_gpio_pin_set( CYGHWR_HAL_KINETIS_PIN(F, 25, 1, KINETIS_PIN_PULLUP) );
		bcu_state.tcms_live_ann_state = 0;
		if(bcu_state.live_button_state == 1)///<exit live
		{
			ExitFromLiveWithoutTS();
		}
	}
}

void ClosePA()
{///<口播处理
	if(bcu_state.bcu_active_ann_state->state_id != 2)
	{
		return;
	}
	ExitFromLiveWithoutTS();
}
void JudegD2PButtonTCMS()
{}
void GetOuterButtonState()
{///<获取外部口播按钮状态
//	if(bcu_state.this_bcu_is_active != 0)
//	{
//		if(GetLiveExternButtonState() == 1)
//		{
			diag_printf("$$$$ func %s\n",__func__);
			if(bcu_state.live_button_state == 0 &&
				BcuGetAnnStatePriority(LIVE_ANN_EVENT) < BcuGetAnnStatePriority(bcu_state.bcu_active_ann_state->state_id))///<enter live
			{
				debug_print(("enter live\n"));
//				ExchangeToLive();
			}
			else if(bcu_state.live_button_state == 1)///<exit live
			{
				debug_print(("exit live\n"));
//				ExitLive();
			}
}
void JudegD2PButton()
{///<判定D2P外部按钮
//	if(GetD2PExternButtonState() == 1)
	{
		debug_print(("bcu_state.d2p_button_state = %d\n",bcu_state.d2p_button_state));
		if(bcu_state.d2p_button_state == 0 && bcu_state.pcu_request_info.request_number>0)///< show d2p page
		{
			intercomm_debug_print(("111Show D2P page\n"));
			intercomm_debug_print(("Show D2P page\n"));
			ResetSoundTimer();
//			ShowD2Ppage();
//			wd_touch_screen->show();
		}
		else if(bcu_state.d2p_button_state == 1 && bcu_state.other_bcu_intercomm_state == INTERCOM_IDLE)///<receive d2p
		{
			diag_printf("Recv D2P page\n");
//			ResetSoundTimer();
//			RecvD2PRequest();
//			wd_touch_screen->show();
		}
		else if(bcu_state.d2p_button_state == 2)///<refuse d2p
		{
			diag_printf("Refuse D2P page\n");
			if(bcu_state.bcu_active_intercom_state->state_id == D2P_INTERCOMM_EVENT &&1 == 1)
			{
				CloseAudioSampleTimer();
			}
//			RefuseD2PRequest();
			diag_printf("refuse d2p -02\n");
//			wd_touch_screen->show();
			diag_printf("refuse d2p -03\n");
		}
	}
}

void SendCMDToEAMP(int param_current_event_state_id)
{///<发送控制命令给EAMP
	network_send_package_t network_send_package;
	strcpy(network_send_package.dst_devices_name,"EAMP");
	network_send_package.dst_devices_no = MUL_DST_NO;
	network_send_package.send_information.event_type_ann = param_current_event_state_id;
	network_send_package.send_information.event_type_intercom = INTERCOM_IDLE;
	switch(param_current_event_state_id)
	{
		case LIVE_ANN_EVENT:
			network_send_package.send_information.event_info_ann.live_announce.live_active = 0;
			break;
		case EMERG_ANN_EVENT:
			network_send_package.send_information.event_info_ann.emerg_announce.emerg_active = 0;
			break;
		case MANUAL_ANN_EVENT:
			network_send_package.send_information.event_info_ann.manual_annnounce.manual_active = 0;
			break;
		case MIC_3D5_OUTER_EVENT:
			network_send_package.send_information.event_info_ann.outer_3d5_ann.outer_3d5_active = 0;
			break;
	}
	BlockBufferWrite(bcu_state.cmd_send_buffer_id,&network_send_package,sizeof(network_send_package));
}

void ForceToIdle(int param_current_event_state_id)
{///<强制切换到空闲状态
	switch(param_current_event_state_id)
	{
		case LIVE_ANN_EVENT:
			LiveAnnExit();
			break;
		case EMERG_ANN_EVENT:
			EmergAnnExit();
			break;
		case MANUAL_ANN_EVENT:
			ManualAnnExit();
			break;
		case MIC_3D5_OUTER_EVENT:
			Outer3D5AnnExit();
			break;
	}
}

void RefuseAllPCUReqest()
{///<拒绝所有PCU请求
	network_send_package_t network_send_package;
	strcpy(network_send_package.dst_devices_name,"PCU");
	network_send_package.dst_devices_no = MUL_DST_NO;
	network_send_package.send_information.event_type_ann = ANN_IDLE;
	network_send_package.send_information.event_type_intercom = D2P_INTERCOMM_EVENT;
	network_send_package.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_active = 0;
	network_send_package.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_bcu_device_no = bcu_state.bcu_info.devices_no;
	network_send_package.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no = MUL_DST_NO;

	BlockBufferWrite(bcu_state.cmd_send_buffer_id,&network_send_package,sizeof(network_send_package));
}

void ClearAllPCURequestInfo()
{///<清除所有PCU请求信息
	int i = 0;
	for(i = 0;i < 30;i++)
	{
//		D2P_Table[i].sequence = 0;
//		D2P_Table[i].state = 0;
		bcu_state.pcu_request_info.pcu_request_table[i] = 0;
	}
	ClearGlobalDeviceInfo();
	bcu_state.pcu_request_info.recept_pcu_no = 0;
	bcu_state.pcu_request_info.refuse_pcu_no = 0;
	bcu_state.pcu_request_info.request_number = 0;
}

void RequestCurrentKeyinfo()
{///<请求查询当前钥匙信息
	network_send_package_t network_send_package;
	strcpy(network_send_package.dst_devices_name,"CCU");
	network_send_package.dst_devices_no = bcu_state.bcu_info.devices_no;
	strcpy(network_send_package.send_information.src_devices_name,"BCU");
	network_send_package.send_information.src_devices_no = bcu_state.bcu_info.devices_no;
	network_send_package.send_information.event_type_ann = PA_KEY_INFORMATION;
	network_send_package.send_information.event_type_intercom = INTERCOM_IDLE;
	network_send_package.send_information.event_info_ann.pa_key_info.get_or_set_key_info = 1;
	network_send_package.send_information.event_info_ann.pa_key_info.bcu_status[bcu_state.bcu_info.devices_no-1] = bcu_state.this_bcu_is_active;
	BlockBufferWrite(bcu_state.cmd_send_buffer_id,&network_send_package,sizeof(network_send_package));
}

void SendToOverOtherBCUD2D()
{///<发送哦命令，是对方BCU 结束D2D状态
	network_send_package_t network_send_package;
	strcpy(network_send_package.dst_devices_name,"BCU");
	network_send_package.dst_devices_no = bcu_state.opposite_bcu_no;
	network_send_package.send_information.event_type_ann = ANN_IDLE;
	network_send_package.send_information.event_type_intercom = D2D_INTERCOMM_EVENT;
	network_send_package.send_information.event_info_intercom.d2d_intercomm.d2d_intercomm_active = 0;
	BlockBufferWrite(bcu_state.cmd_send_buffer_id,&network_send_package,sizeof(network_send_package));
}

void ForceBreakD2DToD2P()
{///<强制打断D2D，返回D2P
	if(bcu_state.bcu_active_intercom_state->state_id == D2D_HANGUP_D2P_EVENT)
	{///<当前状态是司机对讲打断乘客对讲
		send_infomation_t d2d_over_package;

		d2d_over_package.src_devices_no = bcu_state.bcu_info.devices_no;
		strcpy(d2d_over_package.src_devices_name,bcu_state.bcu_info.devices_name);

		d2d_over_package.event_type_ann = ANN_IDLE;
		d2d_over_package.event_type_intercom = D2D_INTERCOMM_EVENT;

		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_active = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_ppt_state = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_bcu_device_no = bcu_state.opposite_bcu_no;
		///<结束司机对讲
		UpdataGlobalDeviceInfo(d2d_over_package);/*Update global device information*/

		if(bcu_state.bcu_active_intercom_state->state_id == D2D_HANGUP_D2P_EVENT)//pending
		{///<切换回司机乘客对讲
			send_infomation_t rt_d2d_bk_d2p,*p_d2d_bk_d2p;
			p_d2d_bk_d2p = &rt_d2d_bk_d2p;
			memset((void *)p_d2d_bk_d2p,0x00,sizeof(rt_d2d_bk_d2p));
			p_d2d_bk_d2p->event_type_ann = ANN_IDLE;
			p_d2d_bk_d2p->event_type_intercom = D2P_INTERCOMM_EVENT;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_active = 1;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_pending = 0;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_bcu_device_no = bcu_state.bcu_info.devices_no;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no = bcu_state.pcu_request_info.recept_pcu_no;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_request_or_over = 1;
			p_d2d_bk_d2p->event_info_intercom.d2p_intercomm.d2p_intercomm_response = 0;
			p_d2d_bk_d2p->src_devices_no = bcu_state.bcu_info.devices_no;
			strcpy((char *)p_d2d_bk_d2p->src_devices_name,(char *)bcu_state.bcu_info.devices_name);
			StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_CONFIRM_TRANSFER_TO_D2P,&rt_d2d_bk_d2p);

			current_is_being_d2d = 0;
			diag_printf("4444\n");
//			AlarmTSToChangeScreen(31);
//			ChangeIntercommLabelState(current_is_being_d2d);
			diag_printf("555\n");
		}
	}
	else if(bcu_state.bcu_active_intercom_state->state_id == D2D_INTERCOMM_EVENT)
	{///<当前是司机对讲
//		whether_change_d2d_to_idle = 1;
		send_infomation_t d2d_over_package;

		d2d_over_package.src_devices_no = bcu_state.bcu_info.devices_no;
		strcpy(d2d_over_package.src_devices_name,bcu_state.bcu_info.devices_name);

		d2d_over_package.event_type_ann = ANN_IDLE;
		d2d_over_package.event_type_intercom = D2D_INTERCOMM_EVENT;

		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_active = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_ppt_state = 0;
		d2d_over_package.event_info_intercom.d2d_intercomm.d2d_intercomm_bcu_device_no = bcu_state.opposite_bcu_no;
		///<结束司机对讲
		StateMachineExchange(&bcu_state.bcu_active_intercom_state,EVENT_PTT_RELEASE_AND_DRIVER_RELEASE,&d2d_over_package);
	}
}

void UpdateDeviceState(send_infomation_t temp_send_infomation)
{///<更新当前设备状态
	if(strcmp(temp_send_infomation.src_devices_name,"EAMP")==0)
	{
		if(temp_send_infomation.event_info_ann.device_state.device_state == 0)
		{
			bcu_state.device_failure_statics.eamp_device_state[temp_send_infomation.src_devices_no] = 1;
		}
		else
		{
			bcu_state.device_failure_statics.eamp_device_state[temp_send_infomation.src_devices_no] = 0;
		}
	}
	AlarmTSToChangeScreen(15);
}

void InitBCULogInfo(common_big_package_t *bcu_send_log_info_to_ccu)
{///<初始化BCU log信息
	bcu_send_log_info_to_ccu->pkg_type = COMMON_PACKAGE_TYPE_BCU_LOG_INFO;
	strcpy(bcu_send_log_info_to_ccu->dst_dev_name,"CCU");
	bcu_send_log_info_to_ccu->dst_dev_number = MUL_SYNC_DST_NO;

	strcpy(bcu_send_log_info_to_ccu->src_dev_name,bcu_state.bcu_info.devices_name);
	bcu_send_log_info_to_ccu->src_dev_number = bcu_state.bcu_info.devices_no;
}

void SendBCULogInfoToCCU(char p_str[])
{///<将BCU 信息发送给CCU
	strcpy((char *)bcu_send_log_info_to_ccu.common_big_data_u.bcu_log_info.bcu_log_info,(char *)p_str);
	BlockBufferWrite(bcu_state.comm_cmd_send_big_buffer_id, &bcu_send_log_info_to_ccu, sizeof(bcu_send_log_info_to_ccu));
}

int BCUCMUCommunication(send_infomation_t bcu_cmu_comm)
{///<BCU 与 CMU通信
	int ret = 0;
#if 1
//	if(strncmp(bcu_cmu_comm.src_devices_name,"CMU",3) == 0)
	{
		diag_printf("pkg_type = %d\n",bcu_cmu_comm.line_status.pkg_type);
		diag_printf("cmd_type = %d\n",bcu_cmu_comm.line_status.cmd_type);
		diag_printf("bcu_mcu_state = %d\n",bcu_state.bcu_mcu_connect_state);
		diag_printf("state_id = %d,%d\n",bcu_state.bcu_active_ann_state->state_id ,LIVE_ANN_EVENT);
		if(bcu_state.this_bcu_is_active != 0 || bcu_state.bcu_current_tcms_cmu_state == 1)
		{
			if(bcu_cmu_comm.line_status.pkg_type == 1 && bcu_cmu_comm.line_status.cmd_type == 1)
			{
				network_send_package_t network_send_package_to_cmu;
				strcpy(network_send_package_to_cmu.dst_devices_name,bcu_cmu_comm.src_devices_name);
				network_send_package_to_cmu.dst_devices_no = bcu_cmu_comm.src_devices_no;
				memcpy((void *)&network_send_package_to_cmu.send_information,(void *)&bcu_cmu_comm,sizeof(bcu_cmu_comm));
				strcpy(network_send_package_to_cmu.send_information.src_devices_name,"BCU");
				network_send_package_to_cmu.send_information.src_devices_no = bcu_state.bcu_info.devices_no;
				network_send_package_to_cmu.send_information.line_status.pkg_type = 2;
//				if(BcuGetAnnStatePriority(bcu_state.bcu_active_ann_state->state_id) < BcuGetAnnStatePriority(LIVE_ANN_EVENT))
				if(bcu_state.bcu_active_intercom_state->state_id != 255)
				{
					network_send_package_to_cmu.send_information.line_status.cmd_type = 7;
					bcu_state.bcu_mcu_connect_state = 0;
				}
				else
				{
					bcu_state.bcu_mcu_connect_state = 1;
					bcu_state.bcu_have_the_initiative_of_cmu = 0;
					SetAudioDataDestination("CMU",232);
					if(1 || bcu_state.tcmns_effective_flag == 0)
					{
						ExchangeToLiveWithoutTS();
					}

				}
				BlockBufferWrite(bcu_state.cmd_send_buffer_id, &network_send_package_to_cmu, sizeof(network_send_package_to_cmu));
			}
			else if(bcu_cmu_comm.line_status.pkg_type == 0 && bcu_cmu_comm.line_status.cmd_type == 4)
			{
				if(bcu_state.bcu_mcu_connect_state == 1 && bcu_state.bcu_active_ann_state->state_id == LIVE_ANN_EVENT)
				{
					bcu_state.bcu_mcu_connect_state = 0;
					bcu_state.bcu_have_the_initiative_of_cmu = 0;
					if(1 || bcu_state.tcmns_effective_flag == 0)
					{
						ExitFromLiveWithoutTS();
					}
				}
			}
		}
		if(bcu_cmu_comm.line_status.pkg_type == 1)
		{
			if(bcu_cmu_comm.line_status.cmd_type == 3)///<cc enter
			{
				if(bcu_state.bcu_mcu_connect_state == 1 && bcu_state.bcu_active_ann_state->state_id == LIVE_ANN_EVENT)
				{
					//bcu_state.bcu_mcu_state = 3;
					if(1 || bcu_state.tcmns_effective_flag == 0)
					{
//						AlarmTSToChangeScreen(29);enter to live
						ExitFromLiveWithoutTS();
					}
				}
				network_send_package_t network_send_package_to_cmu;
				strcpy(network_send_package_to_cmu.dst_devices_name,bcu_cmu_comm.src_devices_name);
				network_send_package_to_cmu.dst_devices_no = bcu_cmu_comm.src_devices_no;
				memcpy((void *)&network_send_package_to_cmu.send_information,(void *)&bcu_cmu_comm,sizeof(bcu_cmu_comm));
				strcpy(network_send_package_to_cmu.send_information.src_devices_name,"BCU");
				network_send_package_to_cmu.send_information.src_devices_no = bcu_state.bcu_info.devices_no;
				network_send_package_to_cmu.send_information.line_status.pkg_type = 2;
				BlockBufferWrite(bcu_state.cmd_send_buffer_id, &network_send_package_to_cmu, sizeof(network_send_package_to_cmu));

				SendD2DAccordingToCMU();
				bcu_state.bcu_mcu_connect_state = 1;
				bcu_state.bcu_have_the_initiative_of_cmu = 0;
				SetAudioDataDestination("CMU",232);
			}
		}
		if(bcu_cmu_comm.line_status.pkg_type == 0 && bcu_cmu_comm.line_status.cmd_type == 6)///<cc exit
		{
			ExitD2DAccordingToCMU();
			bcu_state.bcu_mcu_connect_state = 0;
			bcu_state.bcu_have_the_initiative_of_cmu = 0;
		}
	}
#endif
	return ret;
}


void AdjustStationNo(void)
{
	unsigned char cur_station_no = bcu_state.region_station_info.os_current_station_no;
	unsigned char next_station_no = bcu_state.region_station_info.os_next_station_no;
	unsigned char up_or_down = bcu_state.region_station_info.os_running_up_or_down;
	unsigned char begin_station_no = bcu_state.region_station_info.os_begin_station_no;
	unsigned char ter_station_no = bcu_state.region_station_info.os_terminal_station_no;

	if(bcu_state.region_station_info.stop_or_leave == 0)
	{
		next_station_no = cur_station_no;

		if((cur_station_no == begin_station_no) 
					&& bcu_state.region_station_info.os_cycle_line == 0)
		{
			//if((up_or_down == 1 && ter_station_no > begin_station_no) 
			//	|| (up_or_down == 2 && ter_station_no < begin_station_no))
					cur_station_no = 0;
		}
		else
		{
			do
			{	
				if(up_or_down == 1)
				{
					cur_station_no --;
					if(cur_station_no < 1)
					{
						cur_station_no = MAX_STATION_NUMBER-1;
					}
				}
				else if(up_or_down == 2)
				{
					cur_station_no ++;
					if(cur_station_no > MAX_STATION_NUMBER-1)
					{
						cur_station_no  = 1;
					}
				}
			}while(bcu_state.pp_skip_station_info[1][cur_station_no].effective == 0);
		}
	
	}
	else if(bcu_state.region_station_info.stop_or_leave == 1)
	{
		if((cur_station_no == ter_station_no) 
					&& bcu_state.region_station_info.os_cycle_line == 0)
		{
			//if((ter_station_no < begin_station_no && up_or_down == 1) ||
			//		(ter_station_no > begin_station_no && up_or_down == 1))
			{
				next_station_no = 0;
			}
		}
	}

	bcu_state.region_station_info.os_current_station_no = cur_station_no;
	bcu_state.region_station_info.os_next_station_no = next_station_no;
}


unsigned char GetNextStationNo(unsigned char line_no,unsigned char cur_station_no,unsigned char up_or_down,unsigned char ter_station_no)
{
	unsigned char ret;
	ret = cur_station_no;
	if(line_no == 3)
	{
		if(up_or_down == 1)
		{
			ret ++;
			if(ret >= ter_station_no)
			{
				ret = ter_station_no;
			}
			while(bcu_state.pp_skip_station_info[1][ret].effective == 0)
			{
				ret ++;
				if(ret >= ter_station_no)
				{
					ret  = ter_station_no;
				}
			}
		}
		else
		{
			ret --;
			if(ret <= ter_station_no)
			{
				ret  = ter_station_no;
			}
			while(bcu_state.pp_skip_station_info[1][ret].effective == 0)
			{
				ret --;
				if(ret <= ter_station_no)
				{
					ret  = ter_station_no;
				}
			}
		}
	}
	else if(line_no == 4)
	{
		if(up_or_down == 1)
		{
			ret ++;
			if(ret > MAX_STATION_NUMBER)
			{
				ret  = 1;
			}
			while(bcu_state.pp_skip_station_info[1][ret].effective == 0)
			{
				ret ++;
				if(ret > MAX_STATION_NUMBER)
				{
					ret  = 1;
				}
			}
		}
		else
		{
			ret --;
			if(ret < 1)
			{
				ret = MAX_STATION_NUMBER;
			}
			while(bcu_state.pp_skip_station_info[1][ret].effective == 0)
			{
				ret --;
				if(ret < 1)
				{
					ret = MAX_STATION_NUMBER;
				}
			}
		}
	}
	return ret;
}

int BCUUpdateTCMSLineInfo(send_infomation_t bcu_recv_tcms_line_info_cmd)
{///<BCU 更新TCMS线路号信息
	if(bcu_recv_tcms_line_info_cmd.event_type_ann == LINE_NUMBER_CHANGE_EVENT)
	{
		debug_print(("now:line_no = %d\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.line_no));
		debug_print(("now:region_no = %d\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.region_no));
		debug_print(("now:start_station_no = %d\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.start_station_no));
		debug_print(("now:terminal_station_no = %d\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.over_station_no));
		debug_print(("now:up_or_down = %d\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.running_direction));
		debug_print(("now:whether_the_cycle = %d!\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.whether_the_cycle_bus));
	}

	int whe_refresh_ts = 0;
	if(bcu_recv_tcms_line_info_cmd.event_type_ann == LINE_NUMBER_CHANGE_EVENT &&
		bcu_recv_tcms_line_info_cmd.event_type_intercom == INTERCOM_IDLE)// &&
//		strcmp(bcu_recv_tcms_line_info_cmd.src_devices_name,"CCU") == 0)
	{
		
		if(bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.get_or_set == 0)///收到线路应答包(设置线路)
//		   bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_update_type == 1)///<set the tcms line info
		{
			/*if(bcu_state.this_line_no != bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.line_no)
			{
				bcu_state.this_line_no = bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.line_no;
				bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.line_no 
					= bcu_state.this_line_no ;
				bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_no 
					=bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no;
				bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.region_no
					=bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no;

				if(bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no == 0 &&
						bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_update_type == 1)
				{///<根据接收到的区间信息来设置本地区间信息
					diag_printf("=============wds 20161007 for chengdu7 update region info!!!\n");
					bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.over_station_no =
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.terminal_station_no;

					bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.running_direction =
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.up_or_down;

					bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.start_station_no =
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.start_station_no;

					bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.whether_the_cycle_bus =
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.whether_the_cycle;
					//UpdateTCMSRegionStationInfo(bcu_state.this_line_no);
					whe_refresh_ts = 1;
				}
				else if(bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_update_type == 0)
				{///<根据接收到的区间号，从本地获取区间信息，暂时没必要
					//GetCurrentregionPosition(bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_no);
					//UpdateRegionStationInfo(bcu_state.this_line_no,bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_position);
					ShowRegionInfo(bcu_state.this_line_no);
					whe_refresh_ts = 2;
				}
			}
			else
			*/
			{
				if(bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no == 0 && bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_update_type == 1)
				{
					
					if(bcu_state.region_station_info.os_terminal_station_no != bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.terminal_station_no ||
					   bcu_state.region_station_info.os_running_up_or_down != bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.up_or_down||
 					   bcu_state.region_station_info.os_begin_station_no != bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.start_station_no)
					{
						diag_printf("222============wds 20161007 for chengdu7 update region info!!!\n");
						bcu_state.region_station_info.region_no = 0;
						
						bcu_state.region_station_info.os_begin_station_no =
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.start_station_no;

						bcu_state.region_station_info.os_terminal_station_no=
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.terminal_station_no;

						bcu_state.region_station_info.os_current_station_no = 
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.current_station_no;

						bcu_state.region_station_info.os_running_up_or_down =
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.up_or_down;

						bcu_state.region_station_info.os_cycle_line =
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.whether_the_cycle;

						bcu_state.region_station_info.os_next_station_no
						= GetNextStationNo(DEFAULT_LINE_NO,
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.current_station_no,
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.up_or_down,
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.terminal_station_no);

						whe_refresh_ts = 3;
					}

				}
				else if(bcu_state.bcu_region_info[bcu_state.this_line_no-3].current_region_info.region_no != bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no &&
						bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_update_type == 0)
				{///diag_printf("000============wds 20161007 for chengdu7 update region info!!!\n");
					bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_no = bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no;
//					GetCurrentregionPosition(bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_no);
//					UpdateRegionStationInfo(bcu_state.this_line_no,bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_position);
					ShowRegionInfo(bcu_state.this_line_no);
					whe_refresh_ts = 4;
				}
			}
		}
	}
	debug_print(("whe_refresh_ts = %d\n",whe_refresh_ts));
//	diag_printf("whe_refresh_ts = %d\n",whe_refresh_ts);
	//ShowRegionInfo(bcu_state.this_line_no);
	return whe_refresh_ts;
}


/*
int BCUUpdateTCMSLineInfo(send_infomation_t bcu_recv_tcms_line_info_cmd)
{///<BCU 更新TCMS线路号信息
	if(bcu_recv_tcms_line_info_cmd.event_type_ann == LINE_NUMBER_CHANGE_EVENT)
	{
		debug_print(("now:line_no = %d\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.line_no));
		debug_print(("now:region_no = %d\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.region_no));
		debug_print(("now:start_station_no = %d\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.start_station_no));
		debug_print(("now:terminal_station_no = %d\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.over_station_no));
		debug_print(("now:up_or_down = %d\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.running_direction));
		debug_print(("now:whether_the_cycle = %d!\n",bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.whether_the_cycle_bus));
	}

	int whe_refresh_ts = 0;
	if(bcu_recv_tcms_line_info_cmd.event_type_ann == LINE_NUMBER_CHANGE_EVENT &&
		bcu_recv_tcms_line_info_cmd.event_type_intercom == INTERCOM_IDLE)// &&
//		strcmp(bcu_recv_tcms_line_info_cmd.src_devices_name,"CCU") == 0)
	{
		diag_printf("!!!!!!!!!\n");
		if(bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.get_or_set == 0)// &&
//		   bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_update_type == 1)///<set the tcms line info
		{
			if(bcu_state.this_line_no != bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.line_no)
			{
				bcu_state.this_line_no = bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.line_no;
				bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.line_no = bcu_state.this_line_no ;
				bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_no =
															bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no;
				bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.region_no =
																			bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no;

				if(bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no == 0 &&
						bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_update_type == 1)
				{
					bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.over_station_no =
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.terminal_station_no;

					bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.running_direction =
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.up_or_down;

					bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.start_station_no =
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.start_station_no;

					bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.whether_the_cycle_bus =
							bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.whether_the_cycle;
//					UpdateTCMSRegionStationInfo(bcu_state.this_line_no);
					whe_refresh_ts = 1;
				}
				else if(bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_update_type == 0)
				{
//					GetCurrentregionPosition(bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_no);
//					UpdateRegionStationInfo(bcu_state.this_line_no,bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_position);
					ShowRegionInfo(bcu_state.this_line_no);
					whe_refresh_ts = 2;
				}
			}
			else
			{
				if(bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no == 0 && bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_update_type == 1)
				{
					if(bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.over_station_no != bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.terminal_station_no ||
					   bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.running_direction !=  bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.up_or_down)
					{

						bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_no = 0;

						bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.region_no = 0;

						bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.over_station_no =
											bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.terminal_station_no;

						bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.running_direction =
								bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.up_or_down;

						bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.start_station_no =
								bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.start_station_no;

						bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_info.whether_the_cycle_bus =
								bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.whether_the_cycle;

//						UpdateTCMSRegionStationInfo(bcu_state.this_line_no);

						whe_refresh_ts = 3;
					}

				}
				else if(bcu_state.bcu_region_info[bcu_state.this_line_no-3].current_region_info.region_no != bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no &&
						bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_update_type == 0)
				{
					bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_no = bcu_recv_tcms_line_info_cmd.event_info_ann.line_info.region_no;
//					GetCurrentregionPosition(bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_no);
//					UpdateRegionStationInfo(bcu_state.this_line_no,bcu_state.bcu_region_info[bcu_state.this_line_no - 3].current_region_position);
					ShowRegionInfo(bcu_state.this_line_no);
					whe_refresh_ts = 4;
				}
			}
		}
	}
	debug_print(("whe_refresh_ts = %d\n",whe_refresh_ts));
//	diag_printf("whe_refresh_ts = %d\n",whe_refresh_ts);
	//ShowRegionInfo(bcu_state.this_line_no);
	return whe_refresh_ts;
}
*/

int OpenOrCloseBCUTS(send_infomation_t bcu_recv_tcms_effecient_info_cmd)
{///<打开或者关闭触摸屏
	int ret = -1;
	if(strcmp(bcu_recv_tcms_effecient_info_cmd.src_devices_name,"CCU") == 0 &&
		bcu_recv_tcms_effecient_info_cmd.event_type_ann == ANN_IDLE	&&
		bcu_recv_tcms_effecient_info_cmd.event_type_intercom == INTERCOM_IDLE &&
		bcu_recv_tcms_effecient_info_cmd.update_tcms_effective_flag == 1 &&
		bcu_recv_tcms_effecient_info_cmd.tcms_effective_flag_info.get_or_set_tcms_effective_flag == 0)
	{
		diag_printf("!!!AAAAAA!!!!!!!!%d,%d\n",bcu_state.tcmns_effective_flag,
				bcu_recv_tcms_effecient_info_cmd.tcms_effective_flag_info.tms_effective_flag);
		if(bcu_state.tcmns_effective_flag != bcu_recv_tcms_effecient_info_cmd.tcms_effective_flag_info.tms_effective_flag)
		{
//			bcu_state.tcmns_effective_flag = bcu_recv_tcms_effecient_info_cmd.tms_effective_flag;
			ret = bcu_recv_tcms_effecient_info_cmd.tcms_effective_flag_info.tms_effective_flag;
			diag_printf("!!!!!!!!!!!|n");
		}
	}
	diag_printf("@@@@@@@@@@@@@@@@@@@   !%d!\n",ret);
	return ret;
}

void AskLineInfoToCCU()
{///<查询当前线路信息
	network_send_package_t bcu_line_info_ask_cmd;
	strcpy(bcu_line_info_ask_cmd.dst_devices_name,"CCU");
	bcu_line_info_ask_cmd.dst_devices_no = MUL_DST_NO;;
	bcu_line_info_ask_cmd.send_information.event_type_ann = LINE_NUMBER_CHANGE_EVENT;
	bcu_line_info_ask_cmd.send_information.event_type_intercom = INTERCOM_IDLE;
	bcu_line_info_ask_cmd.send_information.event_info_ann.line_info.get_or_set = 1;
	strcpy(bcu_line_info_ask_cmd.send_information.src_devices_name,"BCU");
	bcu_line_info_ask_cmd.send_information.src_devices_no = bcu_state.bcu_info.devices_no;
	BlockBufferWrite(bcu_state.cmd_send_buffer_id, &bcu_line_info_ask_cmd, sizeof(bcu_line_info_ask_cmd) );
}

void AskTCMSEffectiveFlagInfoToCCU()
{///<查询TCMS当前是否有效
	int ret = 0;
	network_send_package_t bcu_tcms_effective_ask_cmd;
	strcpy(bcu_tcms_effective_ask_cmd.dst_devices_name,"CCU");
	bcu_tcms_effective_ask_cmd.dst_devices_no = MUL_DST_NO;;
	bcu_tcms_effective_ask_cmd.send_information.event_type_ann = ANN_IDLE;
	bcu_tcms_effective_ask_cmd.send_information.event_type_intercom = INTERCOM_IDLE;
	bcu_tcms_effective_ask_cmd.send_information.update_tcms_effective_flag = 1;
	bcu_tcms_effective_ask_cmd.send_information.tcms_effective_flag_info.get_or_set_tcms_effective_flag = 1;
	strcpy(bcu_tcms_effective_ask_cmd.send_information.src_devices_name,"BCU");
	bcu_tcms_effective_ask_cmd.send_information.src_devices_no = bcu_state.bcu_info.devices_no;
	do
	{
		ret = BlockBufferWrite(bcu_state.cmd_send_buffer_id, &bcu_tcms_effective_ask_cmd, sizeof(bcu_tcms_effective_ask_cmd) );
		cyg_thread_delay(10);
	}while(ret <= 0);
		debug_print(("AskTCMSEffectiveFlagInfoToCCU:%d\n",ret));
}
void AskCMuState()
{///<查询CMU状态
	int ret = 0;
	network_send_package_t bcu_cmu_state_ask_cmd;
	strcpy(bcu_cmu_state_ask_cmd.dst_devices_name,"CMU");
	bcu_cmu_state_ask_cmd.dst_devices_no = MUL_DST_NO;;
	bcu_cmu_state_ask_cmd.send_information.event_type_ann = ANN_IDLE;
	bcu_cmu_state_ask_cmd.send_information.event_type_intercom = INTERCOM_IDLE;
	bcu_cmu_state_ask_cmd.send_information.line_status.pkg_type = 0;
	bcu_cmu_state_ask_cmd.send_information.line_status.cmd_type = 8;
	strcpy(bcu_cmu_state_ask_cmd.send_information.src_devices_name,"BCU");
	bcu_cmu_state_ask_cmd.send_information.src_devices_no = bcu_state.bcu_info.devices_no;
	do
	{
		ret = BlockBufferWrite(bcu_state.cmd_send_buffer_id, &bcu_cmu_state_ask_cmd, sizeof(bcu_cmu_state_ask_cmd) );
		cyg_thread_delay(10);
	}while(ret <= 0);
	debug_print(("AskCMuState:%d\n",ret));
}


void SendCommonCMDToCCUTORecordingAudioData(unsigned char intercomm_type,unsigned char src_no,unsigned char dst_no,unsigned char begin_or_over)
{///<发送通用命令给CCU，开始记录录音音频文件
	int ret = 0;
	common_package_t common_package_to_ccu;
	strcpy(common_package_to_ccu.dst_dev_name,"CCU");
	common_package_to_ccu.dst_dev_number = MUL_DST_NO;

	strcpy(common_package_to_ccu.src_dev_name,"BCU");
	common_package_to_ccu.src_dev_number = bcu_state.bcu_info.devices_no;

	common_package_to_ccu.pkg_type = COMMON_PACKAGE_TYPE_INTERCOMM_LOG;

	common_package_to_ccu.common_data_u.intercomm_audio_data_log.intercomm_type = intercomm_type;
	common_package_to_ccu.common_data_u.intercomm_audio_data_log.src_no = src_no;
	common_package_to_ccu.common_data_u.intercomm_audio_data_log.dst_no = dst_no;
	common_package_to_ccu.common_data_u.intercomm_audio_data_log.begin_or_over = begin_or_over;
	int write_counts = 0;
	ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &common_package_to_ccu, sizeof(common_package_to_ccu));
	write_counts ++;
	while(ret <= 0 && write_counts < 2)
	{
		cyg_thread_delay(10);
		ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &common_package_to_ccu, sizeof(common_package_to_ccu));
		write_counts ++;
	}
}
void SendThisBCUAnnStateToEAMP()
{///<发送当前BCU的广播状态给EAMP
	network_send_package_t bcu_ann_state_info_network_package;
//	if(bcu_state.bcu_active_ann_state->state_id == ANN_IDLE)
//	{
//		return ;
//	}
	strcpy(bcu_ann_state_info_network_package.dst_devices_name,"EAMP");
	bcu_ann_state_info_network_package.dst_devices_no = MUL_DST_NO;
	strcpy(bcu_ann_state_info_network_package.send_information.src_devices_name,"BCU");
	bcu_ann_state_info_network_package.send_information.src_devices_no = bcu_state.bcu_info.devices_no;
	bcu_ann_state_info_network_package.send_information.event_type_ann = bcu_state.bcu_active_ann_state->state_id;
	bcu_ann_state_info_network_package.send_information.event_type_intercom = INTERCOM_IDLE;
	switch(bcu_state.bcu_active_ann_state->state_id)
	{
		case OCC_EVENT:
			bcu_ann_state_info_network_package.send_information.event_info_ann.occ_announce.occ_active = 1;
			bcu_ann_state_info_network_package.send_information.event_info_ann.occ_announce.occ_begin_or_over = 0;
			break;
		case LIVE_ANN_EVENT:
			bcu_ann_state_info_network_package.send_information.event_info_ann.live_announce.live_active = 1;
			bcu_ann_state_info_network_package.send_information.event_info_ann.live_announce.live_begin_or_over = 0;
			break;
		case EMERG_ANN_EVENT:
			bcu_ann_state_info_network_package.send_information.event_info_ann.emerg_announce.emerg_active = 1;
			bcu_ann_state_info_network_package.send_information.event_info_ann.emerg_announce.emerg_begin_or_over = 0;
			break;
		case MANUAL_ANN_EVENT:
			bcu_ann_state_info_network_package.send_information.event_info_ann.manual_annnounce.manual_active = 1;
			bcu_ann_state_info_network_package.send_information.event_info_ann.manual_annnounce.manual_begin_or_over = 0;
			break;
		case TMS_MANUAL_ANN_EVENT:
			bcu_ann_state_info_network_package.send_information.event_info_ann.tms_manual_announce.tms_manual_active = 1;
			bcu_ann_state_info_network_package.send_information.event_info_ann.tms_manual_announce.tms_manual_begin_or_over = 0;
			break;
		case TMS_AUTO_ANN_EVENT:
			bcu_ann_state_info_network_package.send_information.event_info_ann.tms_auto_announce.tms_ato_effective = 1;
			bcu_ann_state_info_network_package.send_information.event_info_ann.tms_auto_announce.tms_auto_begin_or_over = 0;
			break;
		case MIC_3D5_OUTER_EVENT:
			bcu_ann_state_info_network_package.send_information.event_info_ann.outer_3d5_ann.outer_3d5_active = 1;
			bcu_ann_state_info_network_package.send_information.event_info_ann.outer_3d5_ann.outer_3d5_begin_or_over = 0;
			break;
		case ANN_IDLE:
			bcu_ann_state_info_network_package.send_information.event_type_ann = BCU_ANN_IDLE_TO_EAMP;
			break;

		default:break;
	}
	BlockBufferWrite(bcu_state.cmd_send_buffer_id, &bcu_ann_state_info_network_package, sizeof(bcu_ann_state_info_network_package));
}

void GetPerDevFailureInfo(unsigned char failure_info_buffer[],int buffer_size)
{
}

int WhetherHaveDevFailureInfo()
{///<判断当前是否有设备故障信息
	int ret = 0;
	int i = 0;
	for(i = 0;i < 2;i++)
	{
		if(bcu_state.dev_falut_info.bcu_info[i].online != 1)
		{
			ret = 1;
			return ret;
		}
	}
	for(i = 0;i < 2;i++)
	{
		if(bcu_state.dev_falut_info.ccu_info[i].online != 1 ||
				bcu_state.dev_falut_info.ccu_info[i].ccu_per_dev_failure_info.fdu_failure_info != 0)
		{
			ret = 1;
			return ret;
		}
	}

	for(i = 0;i < 6;i++)
	{
		if(bcu_state.dev_falut_info.eamp_info[i].online != 1 ||
				bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.dm_failure_info != 0 ||
				bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led1_failure_info != 0 ||
				bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led2_failure_info != 0 ||
				bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.nd1_failure_info != 0 ||
				bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.nd2_failure_info != 0 ||
				bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led1_failure_info != 0 ||
				bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led2_failure_info != 0 )
		{
			ret = 1;
			return ret;
		}
	}
	for(i = 0;i < 12;i++)
	{
		if(bcu_state.dev_falut_info.pcu_info[i].online != 1)
		{
			ret = 1;
			return ret;
		}
	}

	return ret;
}

void alarm_func_handle_key_info(cyg_handle_t counter_handle, cyg_addrword_t data)
{
	whether_ask_key_info = 1;
	whether_send_to_pcu = whether_send_to_pcu + 1;
	key_timer_flag ++;
}

void CreateAskKeyInfoTimer()
{
	cyg_clock_to_counter(cyg_real_time_clock(),&counter_handle_key_info);
	cyg_alarm_create(counter_handle_key_info,alarm_func_handle_key_info,0,&alarm_handle_key_info,&alarm_object_key_info);
	cyg_alarm_initialize(alarm_handle_key_info,cyg_current_time()+1000,1000);
	cyg_alarm_enable(alarm_handle_key_info);
}
void EnableAskKeyInfoTimer()
{
	cyg_alarm_initialize(alarm_handle_key_info,cyg_current_time()+1000,1000);
	cyg_alarm_enable(alarm_handle_key_info);
}

int ReadNumWilson()
{///<获取设备号
	return bcu_state.bcu_info.devices_no;
}

void SendNoPCUToCCU()
{///<发送当前没有PCU请求信息给CCU
	network_send_package_t bcu_no_pcu_to_ccu;
	int ret = 0,write_counts = 0;
	strcpy(bcu_no_pcu_to_ccu.dst_devices_name,"CCU");
	bcu_no_pcu_to_ccu.dst_devices_no = MUL_DST_NO;
	strcpy(bcu_no_pcu_to_ccu.send_information.src_devices_name,"BCU");
	bcu_no_pcu_to_ccu.send_information.src_devices_no = bcu_state.bcu_info.devices_no;
	bcu_no_pcu_to_ccu.send_information.event_type_ann = ANN_IDLE;
	bcu_no_pcu_to_ccu.send_information.event_type_intercom = EVENT_NO_PCU_REQUEST;
	do
	{
		ret = BlockBufferWrite(bcu_state.cmd_send_buffer_id, &bcu_no_pcu_to_ccu, sizeof(bcu_no_pcu_to_ccu));
		write_counts ++;
		if(ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(ret <= 0 && write_counts < 2);
}


void BCUSendDevVolumeInfoToD2DOtherDev()
{///<确保将BCU的的音量信息发送给相关设备，CCU和BCU
	unsigned char write_cmd_buffer_ret = 0;
	do
	{
		write_cmd_buffer_ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &dev_vol_to_ccu, sizeof(dev_vol_to_ccu));
		if(write_cmd_buffer_ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(write_cmd_buffer_ret <= 0);
	do
	{
		write_cmd_buffer_ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &dev_vol_to_bcu, sizeof(dev_vol_to_bcu));
		if(write_cmd_buffer_ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(write_cmd_buffer_ret <= 0);

}
void BCUSendDevVolumeInfoToD2POtherDev()
{///<确保将PCU的的音量信息发送给相关设备，PCU、CCU和BCU
	unsigned char write_cmd_buffer_ret = 0;
	do
	{
		write_cmd_buffer_ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &dev_vol_to_pcu, sizeof(dev_vol_to_pcu));
		if(write_cmd_buffer_ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(write_cmd_buffer_ret <= 0);
	do
	{
		write_cmd_buffer_ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &dev_vol_to_ccu, sizeof(dev_vol_to_ccu));
		if(write_cmd_buffer_ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(write_cmd_buffer_ret <= 0);
	do
	{
		write_cmd_buffer_ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &dev_vol_to_bcu, sizeof(dev_vol_to_bcu));
		if(write_cmd_buffer_ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(write_cmd_buffer_ret <= 0);
}
void BCUSendDevVolumeInfoToPAOtherDev()
{///<确保将EAMP的的音量信息发送给相关设备，EAMP、CCU和BCU
	unsigned char write_cmd_buffer_ret = 0,count_times = 0;
	do
	{
		last_control_flag = control_flag;control_flag = 236;
		write_cmd_buffer_ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &dev_vol_to_eamp, sizeof(dev_vol_to_eamp));
		count_times ++;
		if(write_cmd_buffer_ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(write_cmd_buffer_ret <= 0 && count_times <= 2);
	last_control_flag = control_flag;control_flag = 237;
	count_times = 0;
	do
	{
		write_cmd_buffer_ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &dev_vol_to_ccu, sizeof(dev_vol_to_ccu));
		count_times ++;
		if(write_cmd_buffer_ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(write_cmd_buffer_ret <= 0 && count_times <= 2);
	last_control_flag = control_flag;control_flag = 238;
	count_times = 0;
	do
	{
		write_cmd_buffer_ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &dev_vol_to_bcu, sizeof(dev_vol_to_bcu));
		count_times ++;
		if(write_cmd_buffer_ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(write_cmd_buffer_ret <= 0 && count_times <= 2);
	last_control_flag = control_flag;control_flag = 239;
}

void UpdateSecondDeviceState()
{///<更新二级总线上的设备状态
	int i = 0;
	for(i = 0;i < 2;i++)
	{
		if(bcu_state.dev_falut_info.ccu_info[i].online != 1)
		{
			bcu_state.dev_falut_info.ccu_info[i].ccu_per_dev_failure_info.fdu_failure_info = 1;
		}
	}
	for(i = 0;i < 6;i++)
	{
		if(bcu_state.dev_falut_info.eamp_info[i].online != 1)
		{
			bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.dm_failure_info = 1;

			bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led1_failure_info = 1;
			bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led2_failure_info = 1;

			bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.nd1_failure_info = 1;
			bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.nd2_failure_info = 1;

			bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led1_failure_info = 1;
			bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led2_failure_info = 1;
		}
	}
}

void InitDBVersionInfo()
{///<初始化数据库版本信息
#if 0
	int i = 0;
	for(i = 0;i < 2;i++)
	{
		strcpy(bcu_state.device_db_version.bcu_db_version[i][0],"0.00.000");
		strcpy(bcu_state.device_db_version.bcu_db_version[i][1],"0.00.000");
	}
	for(i = 0;i < 2;i++)
	{
		strcpy(bcu_state.device_db_version.ccu_db_version[i][0],"0.00.000");
		strcpy(bcu_state.device_db_version.ccu_db_version[i][1],"0.00.000");
	}
	for(i = 0;i < 6;i++)
	{
		strcpy(bcu_state.device_db_version.eamp_db_version[i][0],"0.00.000");
		strcpy(bcu_state.device_db_version.eamp_db_version[i][1],"0.00.000");
	}
	for(i = 0;i < 12;i++)
	{
		strcpy(bcu_state.device_db_version.pcu_db_version[i][0],"0.00.000");
		strcpy(bcu_state.device_db_version.pcu_db_version[i][1],"0.00.000");
	}
#endif
}

void RefreshDBVersionPage()
{}
unsigned char CheckCertificateDeviceFailureInfo(unsigned char flag)
{
	unsigned char ret = 0;
	int i = 0;
	switch(flag)
	{
	case 1://BCU
		for(i = 0;i < 2;i++)
		{
			if(bcu_state.dev_falut_info.bcu_info[i].online != bcu_state.dev_falut_last_info.bcu_info[i].online)
			{
				ret = 1;
				break;
			}
		}
		break;
	case 2://CCU
		for(i = 0;i < 2;i++)
		{
			if(bcu_state.dev_falut_info.ccu_info[i].online != bcu_state.dev_falut_last_info.ccu_info[i].online)
			{
				ret = 1;
				break;
			}
		}
		break;
	case 3://EAMP
		for(i = 0;i < 6;i++)
		{
			if(bcu_state.dev_falut_info.eamp_info[i].online != bcu_state.dev_falut_last_info.eamp_info[i].online)
			{
				ret = 1;
				break;
			}
		}
		break;
	case 4://PCU
		for(i = 0;i < 12;i++)
		{
			if(bcu_state.dev_falut_info.pcu_info[i].online != bcu_state.dev_falut_last_info.pcu_info[i].online)
			{
				ret = 1;
				break;
			}
		}
		break;
	case 5://ND
		for(i = 0;i < 6;i++)
		{
			if(bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.nd1_failure_info != bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.nd1_failure_info ||
					bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.nd2_failure_info != bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.nd2_failure_info	)
			{
				ret = 1;
				break;
			}
		}
		break;
	case 6://DM
		for(i = 0;i < 6;i++)
		{
			if(bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.dm_failure_info != bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.dm_failure_info)
			{
				ret = 1;
				break;
			}
		}
		break;
	case 7://FDU
		for(i = 0;i < 2;i++)
		{
			diag_printf("FDU3-%d,%d,%d\n",i,bcu_state.dev_falut_info.ccu_info[i].ccu_per_dev_failure_info.fdu_failure_info ,
					bcu_state.dev_falut_last_info.ccu_info[i].ccu_per_dev_failure_info.fdu_failure_info);
			if(bcu_state.dev_falut_info.ccu_info[i].ccu_per_dev_failure_info.fdu_failure_info !=
					bcu_state.dev_falut_last_info.ccu_info[i].ccu_per_dev_failure_info.fdu_failure_info)
			{
				ret = 1;
				break;
			}
		}
		break;
	case 8://IDU
		for(i = 0;i < 6;i++)
		{
			if(bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led1_failure_info != bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led2_failure_info ||
					bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led2_failure_info != bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led2_failure_info	)
			{
				ret = 1;
				break;
			}
		}
		break;
	case 9://SDU
		for(i = 0;i < 6;i++)
		{
			if(bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led1_failure_info != bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led1_failure_info ||
					bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led2_failure_info != bcu_state.dev_falut_last_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led2_failure_info	)
			{
				ret = 1;
				break;
			}
		}
		break;
	default:
		ret = 0;
		break;
	}

	return ret;
}

unsigned char CheckDeviceFailureStatusPerTypeDeviceFFromNet()
{///<检查每一种类型从网络上接受过来的设备状态
	int i = 0;
	int ret = 0,ret_return = 0;
	for(i = 0;i < 2;i++)
	{
		if(bcu_state.dev_falut_info.bcu_info[i].online != 1)
		{
			ret = 1;
			break;
		}
	}
	device_recv_failure[0] = ret;
	ret = 0;
	for(i = 0;i < 2;i++)
	{
		if(bcu_state.dev_falut_info.ccu_info[i].online != 1)
		{
			ret = 1;
			break;
		}
	}
	device_recv_failure[1] = ret;

	ret = 0;
	for(i = 0;i < 6;i++)
	{
		if(bcu_state.dev_falut_info.eamp_info[i].online != 1)
		{
			ret = 1;
			break;
		}
	}
	device_recv_failure[2] = ret;

	ret = 0;
	for(i = 0;i < 12;i++)
	{
		if(bcu_state.dev_falut_info.pcu_info[i].online != 1)
		{
			ret = 1;
			break;
		}
	}
	device_recv_failure[3] = ret;

	ret = 0;
	for(i = 0;i < 2;i++)
	{
		if(bcu_state.dev_falut_info.ccu_info[i].ccu_per_dev_failure_info.fdu_failure_info != 0)
		{
			ret = 1;
			break;
		}
	}
	device_recv_failure[4] = ret;

	ret = 0;
	for(i = 0;i < 6;i++)
	{
		if(bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led1_failure_info != 0 ||
				bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.in_sider_led2_failure_info != 0)
		{
			ret = 1;
			break;
		}
	}
	device_recv_failure[6] = ret;

	ret = 0;
	for(i = 0;i < 6;i++)
	{
		if(bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led1_failure_info != 0 ||
				bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.outer_sider_led2_failure_info != 0)
		{
			ret = 1;
			break;
		}
	}
	device_recv_failure[5] = ret;

	ret = 0;
	for(i = 0;i < 6;i++)
	{
		if(bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.nd1_failure_info != 0 ||
				bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.nd2_failure_info != 0)
		{
			ret = 1;
			break;
		}
	}
	device_recv_failure[8] = ret;

	ret = 0;
	for(i = 0;i < 6;i++)
	{
		if(bcu_state.dev_falut_info.eamp_info[i].eamp_per_dev_failure_info.dm_failure_info != 0)
		{
			ret = 1;
			break;
		}
	}
	device_recv_failure[7] = ret;
	for(i = 0;i < 9;i++)
	{
		if(device_recv_failure[i] != device_failure[i])
		{
			ret_return = 1;
			break;
		}
	}
	return ret_return;
}

void GetBCUDeviceVersionInfo()
{
	int ret_of_read_cmd_buffer = 0;
	common_big_package_t common_big_package_temp;
	if((ret_of_read_cmd_buffer = BlockBufferRead(bcu_state.software_version_buffer_id,&common_big_package_temp,sizeof(common_big_package_temp))) > 0)
	{
		if(strcmp(common_big_package_temp.src_dev_name,"PTU") == 0 &&
				common_big_package_temp.pkg_type == COMMON_PACKAGE_TYPE_SOFTWARE_VERSION &&
				common_big_package_temp.common_big_data_u.pa_software_version.get_or_set_version_info == 1)
		{
			strcpy(common_big_package_temp.dst_dev_name,"PTU");
			common_big_package_temp.dst_dev_number = 1;
			strcpy(common_big_package_temp.src_dev_name,bcu_state.bcu_info.devices_name);
			common_big_package_temp.src_dev_number = bcu_state.bcu_info.devices_no;
			common_big_package_temp.pkg_type = COMMON_PACKAGE_TYPE_SOFTWARE_VERSION;
			common_big_package_temp.common_big_data_u.pa_software_version.get_or_set_version_info = 0;
			strncpy(common_big_package_temp.common_big_data_u.pa_software_version.db_version_audio,bcu_state.pa_software_version.db_version_audio,30);
			strncpy(common_big_package_temp.common_big_data_u.pa_software_version.db_version_config,bcu_state.pa_software_version.db_version_config,30);
			strncpy(common_big_package_temp.common_big_data_u.pa_software_version.software_date,bcu_state.pa_software_version.software_date,30);
			strncpy(common_big_package_temp.common_big_data_u.pa_software_version.software_version,bcu_state.pa_software_version.software_version,30);
			BlockBufferWrite(bcu_state.comm_cmd_send_big_buffer_id,&common_big_package_temp,sizeof(common_big_package_temp));
		}
	}
}

void UpdateTSInfoAccordingToDB()
{
	if(BCUGetDataFromCCUState() == 1 && whether_updata_db == 0)
	{///<当前已经完成数据库信息的获取，接下来需要更新触摸屏界面相关信息
		last_control_flag = control_flag;control_flag = 22;
//			TSInitAfterGetDataFromCCU();///<触摸屏界面更新
			AlarmTSToChangeScreen(28);
//			SyncSkipStationInfo(bcu_state.this_line_no);///<将获取的站台信息告知触摸屏线程使用的变量
			AlarmTSToChangeScreen(25);
			whether_updata_db = 1;
			bcu_state.this_bcu_is_active = 0;
			AlarmTSToChangeScreen(9);
			cyg_thread_delay(2);
			BCUSetDataFromCCUState(3);///<设置目前已经获取数据库完毕，不再获取
			while(BCUGetDataFromCCUState() != 3)
			{
				diag_printf("BCUGetDataFromCCUState is error\n",BCUGetDataFromCCUState());
				cyg_thread_delay(10);
			}
			cyg_thread_delay(10);
			AskLineInfoToCCU();///<获取当前系统线路号
			cyg_thread_delay(10);
			AskCMuState();///<查询联挂模块状态
			EnableAskKeyInfoTimer();///<使能钥匙信号获取定时器
			whether_send_to_pcu = 7;///<该标志是定期给PCU发送命令包，确保PCU网络正常
			cyg_thread_delay(10);
			AskCCUTcmsCMUState();
			cyg_thread_delay(10);
			AskTCMSEffectiveFlagInfoToCCU();
			ChangeTSAccordingCurrentTCMSFlag();
			last_control_flag = control_flag;control_flag = 23;
	}
	last_control_flag = control_flag;control_flag = 24;
}

void RequestBCUKeyInfo()
{
	if(whether_ask_key_info == 1 && BCUGetDataFromCCUState() == 3)
	{///<请求钥匙信号，前提是当前已经从ccu获取数据库完毕
		whether_ask_key_info = 0;
		diag_printf("ask key info\n");
		RequestCurrentKeyinfo();
	}
}

void UpdateTSGetDBInfoState()
{//更新TS当前获取CCU 数据库信息状态
	if(whether_bcu_have_get_part_info_from_ccu == 1)
	{
		whether_bcu_have_get_part_info_from_ccu = 0;
	}
}

void AskCCUTcmsCMUState()
{
	common_package_t bcu_request_package;
	int ret = 0;
	bcu_request_package.common_data_u.tcms_cmu_state.get_or_set = 1;
	bcu_request_package.pkg_type = COMMON_PACKAGE_TYPE_TCMS_CMU_STATE;
	strcpy(bcu_request_package.dst_dev_name,"CCU");
	bcu_request_package.dst_dev_number = MUL_SYNC_DST_NO;
	strcpy(bcu_request_package.src_dev_name,bcu_state.bcu_info.devices_name);
	bcu_request_package.src_dev_number = bcu_state.bcu_info.devices_no;
	do
	{
		ret = BlockBufferWrite(bcu_state.comm_cmd_send_buffer_id, &bcu_request_package, sizeof(bcu_request_package));
		if(ret <= 0)
		{
			cyg_thread_delay(10);
		}
	}while(ret <= 0);
}
void ChangeTSAccordingCurrentCMUFlag()
{
	if(bcu_state.tcmns_effective_flag == 0)
	{
		diag_printf("Current tcms flag is 0\n");
		return ;
	}
	if(bcu_state.bcu_current_tcms_cmu_state == 1)
	{
		bcu_state.ts_current_state = 0;
		AlarmTSToChangeScreen(13);
	}
	else
	{
		bcu_state.ts_current_state = 1;
		AlarmTSToChangeScreen(14);
	}
}

void ChangeTSAccordingCurrentTCMSFlag()
{
	if(bcu_state.tcmns_effective_flag == 1 && bcu_state.bcu_current_tcms_cmu_state == 0)
	{
		AlarmTSToChangeScreen(14);
	}
	else if(bcu_state.tcmns_effective_flag == 0)
	{
		AlarmTSToChangeScreen(13);
	}
}


