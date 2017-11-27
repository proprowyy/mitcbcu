/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： ts_type.h
 * 文件标识：
 * 摘    要： 触摸屏类型定义
 *
 * 当前版本： V1.0.0
 * 作    者： wilson
 * 完成日期：2013-06-20
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */

#ifndef TS_TYPE_H_
#define TS_TYPE_H_

#include "bcu_get_data_from_ccu_type.h"



//The size of touch panel
#define TOUCHSXREENWIDTH 800//触摸宽度
#define TOUCHSXREENHEIGTH 480//触摸高度

/**
	The declearation of user's password
*/
#define PASSWORD_ERROR 1
#define FORGET_PASSWORD 2
#define INPUT_PASSWORD 3


/**
 * The size of font
 */
#define CHINESELENGTH 24
#define ENGLISHLENGTH 12

/**
 * The size of buffer,which is used to show tms tip information
 */
//#define TMSSIZE 4096
//#define TMSSTATIC 512

#define TMSSIZE  256 //4096
#define TMSSTATIC 128 //512

typedef struct
{
	unsigned char monitor_volume;//监听音量
	unsigned char car_volume;//客室音量
	unsigned char intercomm_volume;//紧急对讲音量
	unsigned char d2d_volume;//司机对讲音量
	char correct_password[10];//正确密码
}device_volume_t;

typedef struct
{
	int length;
	int show_length;
	char *buffer;
	char *buffer_show;
	int row_number;
	int width;
	int heigth;
}tms_top_buffer_info_t;//tms提示信息（简单），目前没有使用

typedef struct
{
	int length;
	char buffer[TMSSTATIC];
	int row_number;
	int total_row_number;
}tms_tip_buffer_info_t;//tms提示信息（全部），目前没有使用

typedef struct
{
	int length;
	char buffer[TMSSIZE];
	int row_number;
	int total_row_number;
}tms_tip_big_buffer_info_t;

typedef struct
{
	unsigned char os_begin_station_no;//起始站
	unsigned char os_terminal_station_no;//终点站
	unsigned char os_current_station_no;//当前站
	unsigned char os_next_station_no;//下一站
	unsigned char os_cycle_line;//是否环线
	unsigned char stop_or_leave;//离站到站

	///<add by wds 20161008 begin
	unsigned char region_no;//区间号
	unsigned char os_door_side;//开门侧
	///<add by wds 20161008 end

	unsigned char os_running_up_or_down;//上下行
	int next_stataion_is_terminal_station;//下一站是否终点站

	unsigned char current_region_station[30];//当前区间站代码顺序表
}region_station_info_t;//区间信息

typedef struct
{
	unsigned char current_emerg_no;//紧急广播序列号
	unsigned char emerg_broadcast_times;//紧急广播播放次数
	unsigned char emerg_total_iteams;//紧急广播总条目数
}emerg_info_t;//紧急广播

typedef struct
{
//	char car_failure[6];
	char bcu_device_state[2 * 3];
	char ccu_device_state[2 * 3];
	char eamp_device_state[6 * 3];
	char pcu_device_state[12 * 3];
}device_failure_statics_t;//设备故障信息，目前没有使用

typedef struct
{
	unsigned int manual_event_priority;//手动报站优先级表
	unsigned int emerg_event_priority;//紧急广播优先级表
	unsigned int live_event_priority;//口播优先级表
}event_priority_t;//广播优先级表

typedef struct
{
	unsigned char total_pcu_number;//PCU总算
	unsigned char recept_pcu_no;//拒绝PCU的号码
	unsigned char refuse_pcu_no;//接通的PUC号码
	unsigned char request_number;//PCU请求的顺序
	unsigned char hang_up_pcu_no;//挂起的PCU号码
	unsigned char pcu_request_table[30];//PCU请求表
	unsigned char open_snd_playback; ///< add, 0107	
	unsigned char pcu_alarm_playing; ///< add, 0107	
	unsigned char pcu_alarm_playing_again; ///< add, 0107
	unsigned char pcu_connection_status[30];//PCU接通状态表
	unsigned char current_conect_pcu_no;//当前接通PCU号码
}pcu_request_info_t;//PCU请求记录表

typedef struct BcuScreenParam
{
       char *src_devices_name;
       unsigned int broadcast_sequence;
       int dst_device_number;
       unsigned short stop_or_leave; // 1: predict;  2: arrived
       unsigned short current_station_code;
       unsigned char event_type;
       unsigned char src_devices_no;
       unsigned char state_active; // 1: active;  0: inactive
       unsigned char begin_or_over; // 1: over;  0: begin
       unsigned char line_number;
       unsigned char region_number;
       unsigned char broadcast_times;
       unsigned char manual_pre_read;
       unsigned char station_effective;
       unsigned short begin_station_code;
       unsigned short over_station_code;
}bcu_screen_param_t;//触摸屏线程与控制线程使用的结构

typedef struct
{
	region_station_t *p_regin_info;//区间信息链表头
	int default_region_no;//默认区间信息
	unsigned char total_regions_number;//区间总数
	unsigned int current_region_no;//当前区间号
	unsigned char current_region_position;//当前区间位置(链表中的顺序)
	unsigned char total_stations_number;//该线路站台总数
	unsigned char current_audio_file_number;//当前线路区间总共站台数
	region_station_t current_region_info;//当前区间详细信息
}global_region_info_t;

typedef struct
{
	emerg_table_items_t *emerg_table_items;//紧急广播链表头
	int emerg_total_items;//紧急广播总条数
	unsigned char emerg_broadcast_times;//紧急广播次数
	unsigned int current_emerg_no;//紧急广播号
	unsigned char current_emerg_flag;///<经广播标志
}bcu_emerg_info_t;
typedef  bcu_emerg_info_t ccu_emerg_info_t;

typedef struct
{
	char name[20];//PCU名称
	unsigned char state;//PCU请求状态 0:normal 1:request
	unsigned char sequence;//PCU请求序列
}d2p_table_t;
#endif /* TS_TYPE_H_ */
