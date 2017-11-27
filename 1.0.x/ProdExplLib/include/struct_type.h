/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称：struct_type.h
 * 文件标识：
 * 摘    要：事件结构体类型
 *
 * 当前版本：V1.0.3
 * 作    者：wilson
 * 完成日期：2012-10-08
 *
 * 取代版本：V1.0.2
 * 原作者  ：wilson
 * 完成日期：2012-09-27
 *
 * 取代版本：V1.0.1
 * 原作者  ：wilson
 * 完成日期：2012-08-25
 */

#ifndef STRUCT_TYPE_H_
#define STRUCT_TYPE_H_

#define TRUE 1
#define FALSE 0

#define Print
#ifndef Print
   #define debug_print(s)
	#define ts_debug_print(s) //diag_printf s
	#define tms_debug_print(s) //diag_printf
#else
    #define debug_print(s)     //diag_printf s
    #define debug_print2(s)     diag_printf s
    #define debug_simulat_print(s)     //diag_printf s
 	#define debug_print_z(s)     //diag_printf s
	#define ts_debug_print(s)  //diag_printf s
	#define tms_debug_print(s) //diag_printf s
	#define sd_debug_print(s)  diag_printf s
    #define fs_debug_print(s)  //diag_printf s
    #define intercomm_debug_print(s) diag_printf s
    #define thread_debug_print(s) //diag_printf s
    #define dfu_debug_print(s)   diag_printf s
#endif

/**
 * 系统设备个数
 */
#define CONFIG_BCU_NUM   2				///<广播控制器个数
#define CONFIG_CCU_NUM   2				///<中央控制器个数
#define CONFIG_PECU_NUM  12				///<乘客报警器个数
#define CONFIG_EAMP_NUM  6				///<功放个数

/**
 * 系统设备类型
 */
#define TYPE_OF_BCU  0x10    			///<广播控制盒
#define TYPE_OF_CCU  0x20 				///<中央控制器
#define TYPE_OF_EAMP 0x30					///<功放
#define TYPE_OF_PECU 0x40					///<乘客报警器


/**
 * 系统响应类型
 */
#define OPERATE_STATE_REQUEST  (1<<0)	///<请求
#define OPERATE_STATE_RESPONSE (1<<1)	///<响应
#define OPERATE_STATE_OVER     (1<<2)	///<结束
/**
 * 数据类型
 */
#define CMD_STATE_CMD  (1<<0)			///<控制信息
#define CMD_STATE_DATA (1<<1)			///<音频数据

/**
 * 系统事件类型
 */
#define OCC_EVENT 1					///<OCC
#define LIVE_ANN_EVENT 2				///<口播
#define EMERG_ANN_EVENT 3				///<紧急广播
#define MANUAL_ANN_EVENT 4			///<手动广播
#define TMS_MANUAL_ANN_EVENT 5		///<半自动广播
#define D2D_INTERCOMM_EVENT 7		///<司机——司机对讲
#define TMS_AUTO_ANN_EVENT 6			///<自动广播
#define D2P_INTERCOMM_EVENT 8		///<司机——乘客对讲
#define D2D_HANGUP_D2P_EVENT 9		///司机对讲，乘客挂起   //wilson 2012-10-17
#define D2P_INTERCOM_PENDING 10		///乘客呼叫等待  	   //wilson 2012-10-26
#define LINE_NUMBER_CHANGE_EVENT 11
#define MIC_3D5_OUTER_EVENT 12
#define SKIP_STATION 50				///<越站信息
#define PA_KEY_INFORMATION 51 ///<PA-BCU钥匙信号
#define CLEAR_ALL_SKIP_STATION 52///< 清除所有越战信息
#define DEVICE_UPDATE_STATE 53///< 设备更新状态
#define EAMP_GET_BCU_ANN_STATE 54///<功放获取BCU当前的广播状态
#define AUTO_EAMP_SYNC_SIGNAL 55///<功放自动同步信号
#define TWO_CARS_CONNECT_STATE 56///<当前联挂
#define EVENT_NO_PCU_REQUEST 57///<当前没有PCU请求
#define BCU_ANN_IDLE_TO_EAMP 58///< EAMP从模拟之后请求获得BCU当前状态（ANN IDLE）


#define EVENT_TYPE_DB_UPDATE_QUERY  248 ///<add for db sync query, 2013
#define EVENT_TYPE_AUDIO_PLAY_SYNC  249 ///<add for audio play sync, 2013
#define EVENT_TYPE_SYS_IMAGE_UPDATE  250 ///<add for system program update, 2013
#define EVENT_TYPE_SYS_UPDATE_SYNC   251 ///<add for db/dev sync, 2013
#define EVENT_TYPE_SYS_UPDATE_SYNC_END 252
#define EVENT_TYPE_DEV_DEAD   253  ///< device is not online, probably it doesn't work
#define ANN_IDLE 254					///<系统空闲状态       //wilson 2012-10-23
#define INTERCOM_IDLE 255				///<系统空闲状态       //wilson 2012-10-23

#define BCU_6W5_DB12    1
#define BCU_6W5_DB16    2
#define BCU_6W5_DB23    3
#define BCU_6W5_DB36    4

#define SET_MIC_AUDIO    0 //1///<手持MIC

#define SET_HD_AUDIO     1 //2///<耳机

#define SET_OCC_AUDIO     3///<OCC

#define SET_MEDIA_AUDIO    2

//event type for common, ach add begin, 2013
#define SUBEVENT_TYPE_DB_VERSION_SYNC_CCU (1)
#define SUBEVENT_TYPE_DB_VERSION_SYNC_BEP  (2) 
#define SUBEVENT_TYPE_DB_SERVER_CHANGED   (3)
#define SUBEVENT_TYPE_DEV_INFO_SYNC_CCU      (4)
#define SUBEVENT_TYPE_DEV_INFO_SYNC_BEP      (5)
#define SUBEVENT_TYPE_LINE_STATION_TABLE_SYNC     (17)
#define SUBEVENT_TYPE_LINE_SECTION_TABLE_SYNC     (19)
#define SUBEVENT_TYPE_DEFAULT_LINE_TABLE_SYNC     (21)
#define SUBEVENT_TYPE_EMERG_TABLE_SYNC     (22)
#define SUBEVENT_TYPE_IP_TABLE_SYNC     (23)
#define SUBEVENT_TYPE_ANN_VERSION_TABLE_SYNC     (24)
#define SUBEVENT_TYPE_CONFIG_VERSION_TABLE_SYNC     (25)
#define SUBEVENT_TYPE_PRIORITY_TABLE_SYNC     (26)
#define SUBEVENT_TYPE_DEV_NUMBER_TABLE_SYNC     (27)
#define SUBEVENT_TYPE_RECORD_PATH_TABLE_SYNC     (28)
#define SUBEVENT_TYPE_LOG_PATH_TABLE_SYNC     (29)
#define SUBEVENT_TYPE_STATION_DOOR_SIDE_SYNC     (30)
#define SUBEVENT_TYPE_STATION_BCU_VOLUME_SYNC     (31)
#define SUBEVENT_TYPE_STATION_BCU_TS_PASSWORD (32)

#define SUBEVENT_TYPE_ALL_DEV_VERSION  (128) ///< add, 1010
#define SUBEVENT_TYPE_PCU_UPDATED                         (250)
#define SUBEVENT_TYPE_PCU_UPDATING                         (251)
#define SUBEVENT_TYPE_EAMP_UPDATED                         (252)
#define SUBEVENT_TYPE_EAMP_UPDATING                         (253)

#define SUBEVENT_TYPE_READY_SYNC_REQ                       (254)
#define SUBEVENT_TYPE_END_SYNC                                       (255)
//event type for common, ach add end, 2013


#define CONFIG_CCU_VERSION_STRING_LEN 32
#define MAX_STATION_NUMBER 32
#define MAX_EMERG_CONTENT_LENGTH 300

#define EMERG_DEBUG_FORTS_TOTAL_ITEMS (8)
#define LINE3_DEFULT_REGIONS_NUMBER (8)
#define LINE4_DEFULT_REGIONS_NUMBER (8)


#define MAX_LINE_NUMBER 20

//长度大小待定，是否需要固定,待定？？？  9-26
#define CONFIG_DEVICE_NAME_LEN  32		///<设备名称长度
#define DEVICE_NAME_LENGTH 30			///<设备名长度
#define IP_NAME_LENGTH 20					///<IP长度
#define MUL_DST_NO 230				   ///<多播目标设备号
#define MUL_SYNC_DST_NO 251
#define LIVE_DST_NO 231				   ///<口播目标设备号
#define BCU_CMU_CAR_CONNECT_PACKAGE 231   ///<连挂通信包
#define AUDIO_DST_NO 231
#define AUDIO_BUF_SIZE 1024

///#define AUDIO_FILE_NAME_LENGTH 30   ///<the length of audio file name
#define AUDIO_FILE_NAME_LENGTH 64   ///<音频文件路径名长度,2013-06-27 modify

#define FILE_PATH_LENGTH 32   ///<文件目录路径名长度,2013-06-27 add
#define EMERG_CONTENT_LENGTH 400//256///<紧急广播内容长度,2013-06-27 add--2015-4-23 modify
#define EMERG_CONTENT_LENGTH_BETWEEN_BCU_CCU 256
#define EMERG_NAME_LENGTH 32///紧急广播名称长度,2013-06-27 add
#define STATION_NAME_LENGTH 32///<站名长度,2013-06-27 add
#define MAC_ADD_LENGTH 20  ///<MAC地址长度，2013-06-27 add
#define TIME_LENGTH 32    ///<时间长度 ，2013-06-27 add
#define EVENT_TYPE_LENGTH 32 ///<事件类型长度，2013-06-27 add
#define FILE_SHORT_NAME_LENGTH 32
#define LOCAL_AUDIO_DATA_BUFFER_SIZE (1024*8)


#define CONFIG_BCU_SYNC_FROM_CCU_NET  ///<only for bcu
#define CONFIG_CCU_VERSION_STRING_LEN  32


///<通用包类型
#define TMS_PACKAGE_ID 10   ///<tms控制包
#define COMMON_PACKAGE_TYPE_SYNC_GD  (2)  ///<
#define COMMON_PACKAGE_TYPE_PTT_STATE  (3)  ///< ptt status
#define COMMON_PACKAGE_TYPE_DEVICE_INFO (4)  ///< dev volume
#define COMMON_PACKAGE_TYPE_MONITOR_SPEAKER_CONTROL (5)  ///< speaker control
#define COMMON_PACKAGE_TYPE_BCU_LOG_INFO (6)  ///< bcu log info
#define COMMON_PACKAGE_TYPE_PCU_STATE (7)  ///<pcu state
#define COMMON_PACKAGE_TYPE_INTERCOMM_LOG (8)  ///<intercomm auido data recording
#define COMMON_PACKAGE_TYPE_EAMP_NET_STATE (9)///<eamp network state
#define COMMON_PACKAGE_TYPE_ALL_DEV_STATE (8)///<all dsev state
#define COMMON_PACKAGE_TYPE_DDU_SIMULATE (11)	///<ddu simulate
#define COMMON_PACKAGE_TYPE_SOFTWARE_VERSION (12)///software version
#define COMMON_PACKAGE_TYPE_CMU_CAR_TYPE (13)//联挂车对象类型
#define COMMON_PACKAGE_TYPE_ACK_STATE (14)//CMU ACK状态
#define COMMON_PACKAGE_TYPE_TCMS_CMU_STATE (15)//TCMS CMU状态

#define BUFFER_TYPE_RECV_FROM_NET  (1)  ///<
#define BUFFER_TYPE_SEND_TO_NET  (2)  ///<
#define BUFFER_TYPE_COMMON_BIG  (2)  ///<
#define BUFFER_TYPE_COMMON_NORMAL  (1)  ///<

#define WITH_DB ///< 2013-12-19
#define PCU_ALARM

///<设备默认音量
#define DEFAULT_MONITOR_VOLUME (2)
#define DEFAULT_CAR_VOLUME (92)
#define DEFAULT_INTERCOMM_VOLUME (3)

//add by zhw 2014-5-20;
typedef struct
{
	unsigned char year;
	unsigned char month;
	unsigned char date;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
}current_time_t;//时间结构类型

typedef struct
{
    unsigned char outer_sider_led1_failure_info;
    unsigned char outer_sider_led2_failure_info;
    unsigned char in_sider_led1_failure_info;
    unsigned char in_sider_led2_failure_info;
    unsigned char nd1_failure_info;
    unsigned char nd2_failure_info;
    unsigned char dm_failure_info;
}eamp_per_dev_failure_t;//eamp外围设备故障

typedef struct
{
  unsigned char fdu_failure_info;
}ccu_per_dev_failure_t;




////>>>>>>>>>>>>>>>>>>state machine define begin>>>>>>>>>>>>>>>>>>>>>>>>
/** 本结构体功能：标识occ状态
 *	本结构体主要是标识OCC的操作状态信息
 */
typedef struct
{
	unsigned char occ_active;		///<OCC状态请求
	unsigned char occ_begin_or_over;		///<OCC开始或结束
	unsigned char occ_response;	///<OCC响应,响应时带标志位：是请求还是结束
}occ_ann_t;

/** 本结构体功能：口播状态
 *	本结构体主要是标识口播状态信息
 */
typedef struct
{
	unsigned char live_active;	///<live状态请求
	unsigned char live_begin_or_over;		///<口播开始或结束
	unsigned char live_response;	///<口播响应
}live_ann_t;

/** 本结构体功能：3d5状态
 *	本结构体主要是标识3d5状态信息
 */
typedef struct
{
	unsigned char outer_3d5_active;	///<outer_3d5状态请求
	unsigned char outer_3d5_begin_or_over;		///<outer_3d5开始或结束
	unsigned char outer_3d5_response;	///<outer_3d5响应
}outer_3d5_ann_t;

/** 本结构体功能：紧急广播状态
 *	本结构体主要是标识紧急广播状态信息
 */
typedef struct
{
	unsigned char emerg_active;		///<紧急广播状态请求
	unsigned char emerg_begin_or_over;		///<紧急广播开始或结束
	unsigned char emerg_response;	///<紧急广播响应
	unsigned char emerg_times;		///<紧急广播次数:1、3、无限大
	unsigned char emerg_sequence;	///<紧急广播音频文件序列号
}emerg_ann_t;

/** 本结构体功能：手动报站状态
 *	本结构体主要是标识手动报站状态信息
 */
typedef struct
{
	unsigned char manual_active;					///<手动报站状态请求
	unsigned char manual_pre_read;			///<手动报站预读
	unsigned char manual_begin_or_over;					///<手动报站开始或结束
	unsigned char manual_response;				///<手动报站响应
	unsigned char line_number;					///<线路号：3、4号线
	unsigned char up_down;						///<当前上行还是下行 0:上行 1：下行
	unsigned char stop_or_leave;					///<是否到站 0:未到站 1：到站;用宏定义表示
	unsigned short int start_station_code;		///<起始站
	unsigned short int current_station_code;	///<当前站
	unsigned short int next_station_code;		///<下一站
	unsigned short int end_station_code;		///<终点站
	unsigned char door_side;						///<开门侧
	unsigned char key_side;						///<激活端
}manual_ann_t;

/** 本结构体功能：tms自动报站状态
 *	本结构体主要是标识自动报站状态信息
 */
typedef	 struct
{
	unsigned char tms_auto_active;				///<自动报站状态请求
	unsigned char tms_auto_pre_read;			///<自动报站预读
	unsigned char tms_auto_begin_or_over;				///<自动报站开始或结束
	unsigned char tms_auto_response;				///<自动报站响应
	unsigned char line_number;					///<线路号：3、4号线
	unsigned char up_down;						///<当前上行还是下行 0:上行 1：下行
	unsigned char stop_or_leave;						///<是否到站 0:未到站 1：到站
	unsigned short int start_station_code;		///<起始站
	unsigned short int current_station_code;	///<当前站
	unsigned short int next_station_code;		///<下一站
	unsigned short int end_station_code;		///<终点站
	unsigned char door_side;						///<开门侧
	unsigned char door_state;						///<开门状态
	unsigned short int train_speed;				///<列车速度
	unsigned short int train_distance;			///<离站距离
	unsigned char key_side;						///<激活端
	unsigned char tms_emerg_times;		///<紧急广播次数:1、3、无限大
	unsigned char tms_emerg_sequence;	///<紧急广播音频文件序列号
	unsigned char tms_type_flag;///<1:tms auto  2:tms half auto  3:tms manual  4:tms emerg
	unsigned char tms_ato_effective;
}tms_auto_ann_t;

/** 本结构体功能：tms半自动报站状态（即从tms触摸屏上进行手动报站）
 *	本结构体主要是标识自动报站状态信息
 */
typedef	 struct
{
	unsigned char tms_manual_active;				///<自动报站状态请求
	unsigned char tms_manual_begin_or_over;				///<自动报站开始或结束
	unsigned char tms_manual_response;			///<自动报站响应
	unsigned char line_number;					///<线路号：3、4号线
	unsigned char up_down;						///<当前上行还是下行 0:上行 1：下行
	unsigned char ann_type;						///<是否到站 0:未到站 1：到站
	unsigned short int start_station_code;		///<起始站
	unsigned short int current_station_code;	///<当前站
	unsigned short int next_station_code;		///<下一站
	unsigned short int end_station_code;		///<终点站
	unsigned char door_side;						///<开门侧
	unsigned char door_state;						///<开门状态
	unsigned short int train_speed;				///<列车速度
	unsigned short int train_distance;			///<离站距离
	unsigned char key_side;						//激活端
}tms_manual_ann_t;

typedef	 struct
{
	unsigned char live_active;	///<live状态请求
	unsigned char live_begin_or_over;		///<口播开始或结束
	unsigned char live_response;	///<口播响应
}tms_live_ann_t;


/** 本结构体功能：司机与司机对讲状态
 *	本结构体主要是标识对讲状态信息
 */
typedef struct
{
	unsigned char d2d_intercomm_active;				///<对讲状态请求
	unsigned char d2d_intercomm_request_or_over;			///<对讲请求或结束
	unsigned char d2d_intercomm_response;			///<对讲响应
	unsigned char d2d_intercomm_bcu_device_no;		///<对讲目标设备号
	unsigned char d2d_ppt_state;                   ///<ppt的状态 0:松开  1:摁下
}d2d_intercomm_t;

/** 本结构体功能：司机与乘客对讲状态
 *	本结构体主要是标识对讲状态信息
 */
typedef struct
{
	unsigned char d2p_intercomm_active;				///<对讲状态请求
	unsigned char d2p_intercomm_request_or_over;			///<对讲请求或结束
	unsigned char d2p_intercomm_pending;           ///<对讲挂起
	unsigned char d2p_intercomm_response;			///<对讲响应
	unsigned char d2p_intercomm_bcu_device_no;		///<对讲目标设备号
	unsigned char d2p_intercomm_pcu_device_no;		///<对讲目标设备号
	unsigned char d2p_intercomm_whether_is_connecting;///<wilson 2014-02-28
}d2p_intercomm_t;

////<<<<<<<<<<<<<<<<state machine define over<<<<<<<<<<<<<<<<<<<</

////>>>>>>>>>>>>>>>>>>>>>>>>device info define begin>>>>>>>>>>>>>>>>>>>>>>/
/** 本结构体功能：全局配置表
 *	本结构体主要是设定全局配置表
 */
typedef struct
{
	char *name;			///<设备名
	int device_no;			///<设备号
	char *ip_addr;			///<IP地址
}device_info_t;

/** 本结构体功能：本地设备IP表
 *	本结构体主要是记录本地设备IP
 */
typedef struct
{
	char *ip_addr;			///<IP
}local_device_info_t;		//modify

/** 本结构体功能：模拟输入的设备名与对应的接口号
 */
typedef struct
{
	char *name;			///<设备名
	int io_no;				///<IO 接口号
}device_name_t;

/** Struct function：identify device name,id and car number
 */
typedef struct
{
	char device_name[DEVICE_NAME_LENGTH];			///<设备名
	char device_no;				                  ///<设备号
	char device_car_no;								///<车厢号
	int dev_type;
}device_id_info_t;

typedef struct eamp_info
{
	unsigned char online;//EAMP是否在线
	eamp_per_dev_failure_t eamp_per_dev_failure_info;//EAMP的外设是否在线
}eamp_info_t;

typedef struct bcu_info
{
	unsigned char online;//BCU是否在线
}bcu_info_t;

typedef struct ccu_info
{
	unsigned char online;//CCU是否在线
	ccu_per_dev_failure_t ccu_per_dev_failure_info;//CCU的外设是否在线
}ccu_info_t;

typedef struct pcu_info
{
	unsigned char online;//PCU是否在线
}pcu_info_t;

typedef struct cmu_info
{
	unsigned char online;//CMU是否在线
}cmu_info_t;

typedef struct g_dev_info
{
	eamp_info_t eamp_info[6];
	bcu_info_t bcu_info[2];
	ccu_info_t ccu_info[2];
	pcu_info_t pcu_info[12];
	cmu_info_t cmu_info[2];
}g_dev_fault_info_t;//全局设备故障表格
/////<<<<<<<<<<<<<<<<<<<<<<<<<<device info define over<<<<<<<<<<<<<<<<<<<<<<<<<<<</


///////////>>>>>>>>>>>>>>>>>>>>>>>thread communication define beign>>>>>>>>>>>>>>>>>>>>>>>
/** 本结构体功能：告知网络，音频数据该发往何处
 *	本结构体主要标识来目标设备名和目标设备号
 *	主要用来标识音频数据的目的地
 */
typedef struct
{
	char device_name[DEVICE_NAME_LENGTH];	///<目标设备名
	unsigned char device_no;					///<目标设备号
}threads_communication_type_t;
//////<<<<<<<<<<<<<<<<<<<<<<<<<<thread communication define over<<<<<<<<<<<<<<<<<<<<<<<<<<

typedef struct
{
	unsigned char get_or_set_key_info;//设置还是获取钥匙信号
	unsigned char line_no;//线路号
	unsigned char region_no;//区间号
	unsigned char bcu_status[2];//钥匙信号
}pa_key_info_t;//钥匙信号

typedef struct
{
	unsigned char get_or_set_tcms_effective_flag;//查询还是设置TCMS当前状态
	unsigned char tms_effective_flag;//TCMS有效信息
}tcms_effective_flag_info_t;//TCMS有效信息结构体

typedef struct
{
	unsigned char line_no;//线路号
}clear_all_skip_station_t;//越站信息清除结构体

typedef struct
{
	unsigned char device_state;///<0:nomal 1:updating
}device_state_t;//设备状态

typedef struct
{
	unsigned char desire_eamp_voulme;//EAMP音量信息
}eamp_vol_info_t;


////////>>>>>>>>>>>>>>>>>>>>>>>running info define begin>>>>>>>>>>>>>>>>>>>>>>>
typedef struct
{
	unsigned char line_no;//线路号
	unsigned char region_no;//区间号
	unsigned char region_update_type;///<0:according to the region_no 1:otherwise
	unsigned char start_station_no;//起始站
	unsigned char terminal_station_no;//终点站
	unsigned char up_or_down;//上下行
	unsigned char whether_the_cycle;//是否环线
	unsigned char get_or_set;///<0:want to require the line info 1:set the line info
	unsigned char current_station_no;//add 2015-7-14 wilson
}line_info_t;//线路号信息

//struct REGIONSTATION
typedef struct
{
	unsigned char line_no;
	unsigned char region_no;
	unsigned char running_direction;//1：内圈-上行  2：外圈-下行
	unsigned char whether_the_cycle_bus;
	unsigned char start_station_no;
	unsigned char over_station_no;
	unsigned char current_region_flag;
    unsigned char start_time[10];
    unsigned char end_time[10];
}region_station_t;//区间信息

//This struct is used to record the effective of station
typedef struct
{
	unsigned char line_number;//线路号
	unsigned char station_no;//站代码
	unsigned char effective;//是否有效
	char station_chinese_name[STATION_NAME_LENGTH];//站代码中文名
	char station_english_name[STATION_NAME_LENGTH * 2];//站代码英文名
	unsigned char station_audio[100];
	void *p_station_info;//站详细信息链表头指针
	unsigned char open_door_side[2];///<2014-08-15 开门侧信息
}station_information_t;//站信息
///////<<<<<<<<<<<<<<<<<<<<<<<<<<running info define over<<<<<<<<<<<<<<<<<<<<<<<<<<


////////>>>>>>>>>>>>>>>>>>>>>>>failure info define begin>>>>>>>>>>>>>>>>>>>>>>>
typedef struct FailureInfoTableType
{
       int fail_code;
       char *description;
       char *resolution;
}failure_info_table_t;
////<<<<<<<<<<<<<<<<<<<<<<<<<<failure info define over<<<<<<<<<<<<<<<<<<<<<<<<<<

///<add  by wilson,2013-10-15
typedef struct
{
	unsigned char version_one;
	unsigned char version_two;
	unsigned char version_three;
}device_software_version_t;//版本号

typedef struct {
	unsigned char update_state; ///< <0>: not be updated; <1>: updating; <2>: updated
	unsigned char reserve_one;
	unsigned char reserve_two;
	unsigned char reserve_three;	
} db_file_update_state_t;

#define CONFIG_EAMP_PRIOR_SWTICH ///<



#endif /* STRUCT_TYPE_H_ */

