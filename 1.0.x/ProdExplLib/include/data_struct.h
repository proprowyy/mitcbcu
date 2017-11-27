/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称：data_struct.h
 * 文件标识：
 * 摘    要：控制信息结构体
 *
 * 当前版本：V1.0.3
 * 作    者：wilson
 * 完成日期：2012-10-08
 *
 * 取代版本：V1.0.2
 * 原作者  ：wilson
 * 完成日期：2012-9-27
 *
 *  取代版本：V1.0.1
 * 原作者  ：wilson
 * 完成日期：2012-8-25
 *
 * 取代版本：V1.0.0
 * 原作者  ：周魏
 * 完成日期：2012-08-23
 */

#ifndef DATA_STRUCT_H_
#define DATA_STRUCT_H_
#include "struct_type.h"
#include "dynamic_map.h"
#include "tms_module.h"
#include "ts_type.h"

//#define CONFIG_TEST_SND_IN_MULTI_THREAD   ///< debug for sound_card, 01-10

#ifdef  CONFIG_BCU_SYNC_FROM_CCU_NET
#include "bcu_get_data_from_ccu_type.h"
#endif

/** 本联合体功能：广播事件信息标识
 *  本联合体包含所有广播事件信息，但每时刻只能标识一个事件信息
 */
typedef union
{
	occ_ann_t occ_announce;                 ///<occ状态
	outer_3d5_ann_t outer_3d5_ann;			///<3d5状态
	live_ann_t live_announce;   				///<口播状态
	emerg_ann_t emerg_announce;				///<紧急广播状态
	manual_ann_t manual_annnounce;			///<手动报站状态
	tms_auto_ann_t tms_auto_announce;		///<tms自动报站状态
	tms_manual_ann_t tms_manual_announce;	///<tms半自动报站状态
	tms_live_ann_t tms_live_announce;	    ///<tms口播
	station_information_t station_information; ///<越站信息
	line_info_t line_info;///<线路号-区间号设置
	db_file_update_state_t db_file_state; ///< for changming
	pa_key_info_t pa_key_info;///<司机室激活端
	clear_all_skip_station_t clear_all_skip_station;///<清除所有的越站信息
	device_state_t device_state;
	eamp_vol_info_t eamp_vol_info;
}event_infomation_ann_t;

/** 本联合体功能：对讲事件信息标识
 *  本联合体包含所有对讲事件信息，但每时刻只能标识一个事件信息
 */
typedef union
{
	d2d_intercomm_t d2d_intercomm;			///<司机对讲
	d2p_intercomm_t d2p_intercomm;			///<乘客对讲
}event_infomation_intercomm_t;


/** 本结构体功能：广播控制盒状态
 *	本结构体主要是标识广播控制盒状态信息
 */
typedef struct
{
	  char devices_name[DEVICE_NAME_LENGTH];					///<设备名称
	  int devices_no;												///<设备编号
	  unsigned char current_state_ann;							///<设备广播当前状态
	  event_infomation_ann_t event_infomation_ann;				///<设备广播信息
	  unsigned char current_state_intercom;						///<设备对讲当前状态
	  event_infomation_intercomm_t event_infomation_intercom;	///<设备对讲信息
	  int time;	///<时间
	  unsigned int config_db_version;  ///<add, 2013
	  unsigned int ann_db_version;  ///<add, 2013	
	  unsigned char driver_key_inserted; ///<ach add, 2013
	  unsigned char is_db_server; ///<ach add, 2013
	  unsigned int state_count;  ///<add, 2013	 
}device_bcu_state_t;

/** 本结构体功能：功放状态
 *	本结构体主要是标识功放状态信息
 */
typedef struct
{
	  char devices_name[DEVICE_NAME_LENGTH];				///<设备名称
	  int devices_no;											///<设备编号
	  unsigned char current_state;							///<设备当前状态
	  event_infomation_ann_t event_infomation_ann; 		///<设备广播信息
	  int time;												///<时间
	  unsigned int config_db_version;  ///<add, 2013
	  unsigned int ann_db_version;  ///<add, 2013	
	  unsigned char is_db_server; ///<ach add, 2013
	  unsigned int state_count;  ///<add, 2013
}device_eamp_state_t;

/** 本结构体功能：乘客报警器状态
 *	本结构体主要是标识乘客报警器状态信息
 */
typedef struct
{
	  char devices_name[DEVICE_NAME_LENGTH];						///<设备名称
	  int devices_no;													///<设备编号
	  unsigned char current_state_intercom;							///<设备当前状态,wilson 2012-10-26 current_state->current_state_intercom
	  event_infomation_intercomm_t event_infomation_intercom;		///<设备对讲信息
	  unsigned char  request_sequence;														///<时间
	  unsigned int config_db_version;  ///<add, 2013
	  unsigned int ann_db_version;  ///<add, 2013	
	  unsigned char is_db_server; ///<ach add, 2013
	  unsigned int state_count;  ///<add, 2013
}device_pcu_state_t;

/** 本结构体功能：中央控制器状态
 *	本结构体主要是标识中央控制器状态信息
 */
typedef struct
{
	  char devices_name[DEVICE_NAME_LENGTH];	///<设备名称
	  int devices_no;								///<设备编号
	  unsigned char current_state;				///<设备当前状态
	  int time;									///<时间
	  unsigned int config_db_version;  ///<add, 2013
	  unsigned int ann_db_version;  ///<add, 2013	
	  unsigned char is_db_server; ///<ach add, 2013
	  unsigned int state_count;  ///<add, 2013
}device_ccu_state_t;

//add begin, 2013
#define GD_VALUE_MAX_MACRO(a,b)	((a)>=(b)?(a):(b))
#define TMP_MAX_A (GD_VALUE_MAX_MACRO(sizeof(device_eamp_state_t), sizeof(device_pcu_state_t)))
#define TMP_MAX_B (GD_VALUE_MAX_MACRO(TMP_MAX_A, sizeof(device_bcu_state_t)))
#define TMP_MAX_C	(GD_VALUE_MAX_MACRO(TMP_MAX_B, sizeof(device_ccu_state_t)))

#ifdef CONFIG_BCU_SYNC_FROM_CCU_NET
#define GD_DEV_INFO_MAX_LEN	((GD_VALUE_MAX_MACRO(TMP_MAX_C, sizeof(bcu_get_db_data_from_ccu_t)))+4) ///< last 4 bytes are CRC
#else
#define GD_DEV_INFO_MAX_LEN	(TMP_MAX_C+4) ///< last 4 bytes are CRC
#endif

typedef struct
{
        unsigned int gd_sync_context_len;
        unsigned char gd_sync_req;
        unsigned char gd_sync_res;
        unsigned char gd_sync_mask; //config db sync;  station db sync;  audio file sync; global dev info sync; sys update
        unsigned char gd_sync_context[GD_DEV_INFO_MAX_LEN];
        unsigned char gd_source_ip[IP_NAME_LENGTH];
}gd_sync_cmd_t;

typedef union
{
      gd_sync_cmd_t  gd_sync_cmd_info;
}event_information_common_t;

typedef struct
{
	unsigned char event_type_common;                        ///< 
	event_information_common_t  event_info_common; ///< 
	unsigned int  sequence;                                             ///<
}pa_common_information_t;
//add end, 2013

typedef struct
{
	int pkg_number;
	int pkg_type; //0:结束请求包，不需要对方回复;1:开始请求包，需要对方回复; 2:回复包；
	//1:pa/l_pa start;2:pc/l_pc start;3:cc/l_cc start;4:pa/l_pa end;5:pc/l_pc end;6:cc/l_cc end .add by zhw-2014-5-4
	int cmd_type;
}cmu_cmd_t; //add by zhw-5-4

/** 本结构体功能：包含所有的控制信息
 *	本结构体是所有设备决定接下来操作的依据
 *	主要用来传递不同设备之间的相互请求
 */
typedef struct
{
	void (*init)(void);								//结构体初始化
	void (*setinformation)(unsigned char event_type,
								char *src_devices_name,
								unsigned char src_devices_no,
								unsigned char state_request,
								unsigned char request_or_over,
								unsigned char line_number,
								unsigned char region_number,
								unsigned int broadcast_sequence,
								unsigned char broadcast_times,
								unsigned char arrived_station,
								unsigned char current_station_code,
								unsigned char station_effective,
								unsigned char whether_pre_read,
								unsigned char dst_device_number
								);		///<填充该结构体

	char src_devices_name[DEVICE_NAME_LENGTH];		///<源设备名
	unsigned char src_devices_no;					///<源设备号
	unsigned char event_ptt_state;

	unsigned char event_type_ann;					///<事件类型
	event_infomation_ann_t event_info_ann;	///<事件信息

	unsigned char event_type_intercom;				///<事件类型
	event_infomation_intercomm_t event_info_intercom;///<事件信息

	//1:pa/l_pa start;2:pc/l_pc start;3:cc/l_cc start;4:pa/l_pa end;5:pc/l_pc end;6:cc/l_cc end .add by zhw-2014-4-4
//	unsigned char line_status ; //连挂线状态
	cmu_cmd_t line_status;
	
	unsigned char update_tcms_effective_flag;
	tcms_effective_flag_info_t tcms_effective_flag_info;
	int  sequence;                                 ///<包的序列号;
}send_infomation_t;

/** 本结构体功能：将控制信息进行二次打包，便于网络发送
 *	本结构体是在控制结构体的基础上添加目标设备名和目标设备号
 *	主要用来在网络上传递
 */
typedef struct
{
	char dst_devices_name[DEVICE_NAME_LENGTH];		///<目的设备名
	int  dst_devices_no;								///<目的设备号
	int  time;
	send_infomation_t send_information;				///<需要发送到的控制信息
}network_send_package_t;

/** 本结构体功能：保存列车所有设备的当前信息
 *	本结构体包含来列车上所有设备的当前信息
 *	主要用来想触摸屏提供不同设备的信息
 */
typedef struct
{
	void (*init)(void);
	void (*setglobalinfo)(send_infomation_t);

	device_bcu_state_t  device_bcu[CONFIG_BCU_NUM + 1];    ///<列车广播控制盒状态
	device_ccu_state_t  device_ccu[CONFIG_CCU_NUM + 1];    ///<列车中央控制器状态
	device_eamp_state_t device_eamp[CONFIG_EAMP_NUM + 1];  ///<列车功放状态
	device_pcu_state_t device_pcu[CONFIG_PECU_NUM + 1];   ///<列车乘客报警器状态
}global_device_info_state_t;


// 设备的整体状态信息
typedef struct eamp_dev_ifo
{
	int set_online; // add by zhw-2014-11-24
	int reply;
	int dev_number;
	eamp_info_t eamp_info;
	struct eamp_dev_ifo *next;
}eamp_dev_info_t;

typedef struct bcu_dev_info
{
	int set_online;
	int reply;
	int dev_number;
	bcu_info_t bcu_info;
	struct bcu_dev_info *next;
}bcu_dev_info_t;

typedef struct ccu_dev_info
{
	int set_online;
	int reply;
	int dev_number;
	ccu_info_t ccu_info;
	struct ccu_dev_info *next;
}ccu_dev_info_t;

typedef struct pcu_dev_info
{
	int set_online;
	int reply;
	int dev_number;
	pcu_info_t pcu_info;
	struct pcu_dev_info *next;
}pcu_dev_info_t;

typedef struct cmu_dev_info
{
	int set_online;
	int reply;
	int dev_number;
	cmu_info_t cmu_info;
	struct cmu_dev_info *next;
}cmu_dev_info_t;

typedef struct
{
	eamp_dev_info_t *eamp_dev_info_head;
	bcu_dev_info_t *bcu_dev_info_head;
	ccu_dev_info_t *ccu_dev_info_head;
	pcu_dev_info_t *pcu_dev_info_head;
	cmu_dev_info_t *cmu_dev_info_head;
}dev_status_info_t;

typedef struct
{
	int package_type; //1表示：心跳查询包、2表示：心跳回复包；3表示:;
	int package_number;//
	union dev_info
	{
		eamp_info_t eamp_info;
		bcu_info_t bcu_info;
		ccu_info_t ccu_info;
		pcu_info_t pcu_info;
		cmu_info_t cmu_info;
	}dev_info_u;
	current_time_t current_time; //add by zhw 2014-5-20,time info send to client which service get from TMCS;
}heart_package_t;

typedef struct PttStateType
{
      unsigned char dst_ptt;
      unsigned char src_ptt;
}ptt_state_t;

typedef struct device_volume_info
{
	char device_name[DEVICE_NAME_LENGTH];
	int device_no;
	int device_volume;
	int device_adjust_volume;
	int device_no_recept_timeout;
}ts_dev_volume_info_t;
typedef struct
{
	char monitor_speaker_state;
	char monitor_speaker_volume;
}monitor_speaker_control_t;

typedef struct
{
	unsigned char bcu_log_info[200];
}bcu_log_info_t;
typedef struct
{
	unsigned char intercomm_type;///< 0:d2d 1:d2p 2:d2d break up d2p
	unsigned char src_no;
	unsigned char dst_no;
	unsigned char begin_or_over;
}intercomm_audio_data_log_t;
typedef struct
{
	unsigned char pa_volume;
	unsigned char monitor_volume;
	unsigned char tcms_flag;///<add by wilson,inorder to tell eamp current tcms flag
	current_time_t tcms_time;
}eamp_heart_beta_t;
typedef struct
{
	unsigned char speed;
	unsigned char door_status;	//0:至少有一个门未关；1：所有门关好
	int whether_start_simulateddu_flag;
}simulate_ddu_t;

typedef struct
{
	unsigned char old_car_line_no;
	unsigned char state;
}cmu_car_type_t;
typedef struct
{
	unsigned char ack_state;
}cmu_ack_state_t;

typedef struct
{
	unsigned char get_or_set;
	unsigned char tcms_current_cmu_state;
}tcms_cmu_state_t;

typedef union
{
//	heart_package_t heart_package;
	ptt_state_t   comm_ptt_state; ///< pkg_type = 3
	ts_dev_volume_info_t ts_dev_volume_info;///<pkg_type = 4
	monitor_speaker_control_t monitor_speaker_control;///< 5 speaker control
	intercomm_audio_data_log_t intercomm_audio_data_log;///< 8 intercomm auido data recording
	eamp_heart_beta_t eamp_heart_beta;
	simulate_ddu_t simulate_ddu;	//TCMS模拟ddu距离报站
	cmu_car_type_t cmu_car_type;//COMMON_PACKAGE_TYPE_CMU_CAR_TYPE
	cmu_ack_state_t cmu_ack_state;//COMMON_PACKAGE_TYPE_ACK_STATE
	tcms_cmu_state_t tcms_cmu_state;//COMMON_PACKAGE_TYPE_TCMS_CMU_STATE
}common_data_t;

typedef struct
{
	char dst_dev_name[DEVICE_NAME_LENGTH];
	int dst_dev_number;
	char src_dev_name[DEVICE_NAME_LENGTH];
	int src_dev_number;
	int pkg_type; //1表示:心跳包、2表示: 、3表示: 10(TMS_PACKAGE_ID):TMS模拟;5:speaker control
	common_data_t common_data_u;
}common_package_t;

typedef struct
{
	char dst_dev_name[DEVICE_NAME_LENGTH];
	int dst_dev_number;
	char src_dev_name[DEVICE_NAME_LENGTH];
	int src_dev_number;
//	int pkg_type; //1表示:心跳包、2表示: 、3表示: 10(TMS_PACKAGE_ID):TMS模拟;5:speaker control
	heart_package_t heart_package;
}network_heart_package_t;

typedef struct
{//长度为30 ，与数据库版本号一致
	char get_or_set_version_info;///0:set 1:get
	char software_version[32];
	char software_date[32];
	char db_version_audio[32];
	char db_version_config[32];
}pa_software_version_t;


typedef union
{
	tms_to_ccu_t tms_to_ccu_info;//<pkg_type=1
	pa_common_information_t pa_common_cmd_info; ///< pkg_type=2
	bcu_log_info_t bcu_log_info;///< pkg_type=6
	g_dev_fault_info_t dev_falut_info;//pkg_type =8
	pa_software_version_t pa_software_version; //pkg_type =12
}common_big_data_t;

typedef struct
{
	char dst_dev_name[DEVICE_NAME_LENGTH];
	int dst_dev_number;
	char src_dev_name[DEVICE_NAME_LENGTH];
	int src_dev_number;
	int pkg_type; //2表示:pa_common_information_t
	common_big_data_t common_big_data_u;
}common_big_package_t;

typedef struct {
    int current_event;
    int audio_play_buffer_id;	
    int send_audio_buffer_id;
    int pending_audio_buffer_id;	
    unsigned char enable_play;
    unsigned char enable_sample;	
    unsigned char play_times;	
} audio_play_control_t;

typedef struct {	
    unsigned char dev_no;
    unsigned char play_waiting;
} eamp_audio_sync_t;


typedef enum { BCU,CCU, EAMP, PCU }DevType_t;
typedef enum { ProtocolTestStart,CommQualityTest, SystemReboot, DevFirmwareUpdate,
							OpenAtuo,CloseAtuo,ZhwFunc6,ZhwFunc7,ZhwFunc8,ZhwFunc9 } CmdType_t;
typedef enum { NUL,MOUNT,UPDATE,VERIFY } SubCmdType_t;
typedef enum { INVALID,VALID,FAIL,SUCCESS } DevStatus_t;

#pragma pack(4)
typedef struct
{
	int sub_cmd;	
	char dev_name[8];
	int dev_type;	
	int	dev_index;	
	int dev_status;	
	int file_crc;	
	int file_ver;
}dfu_info_t;

typedef struct
{
	int cmd;
	dfu_info_t dfu_info;
}network_shell_package_t;


/** BCUSendInfoInit函数说明
 *  本函数隶属于BCU，主要操作是清空控制信息结构体
 *
 *  @param   void
 *  @return  null
 */
void BCUSendInfoInit(void);

/** BCUSetInformation函数说明
 *  本函数隶属于BCU，主要操作是填充控制信息结构体
 *
 *  @param 事件类型
 *  @param 源设备名称
 *  @param 源设备号
 *  @param 状态请求(0)、结束(1)
 *  @param 线路号
 *  @param 文件序列号
 *  @param 播报次数
 *  @param 是否到站
 *  @param 站代码
 *  @param 对讲状态下的目标设备号
 *  @return null
 *  @note 本函数入口参数过多，填充是需要格外注意。每一次事件请求其中只有部分信息有用。对该此请求无用的数据项可用0或“”代替
 */
void BCUSetInformation(unsigned char,char *, unsigned char, unsigned char,unsigned char,unsigned char,
					  	unsigned int, unsigned char, unsigned char,unsigned char,int);

/** InitThreadCommunicationInfo函数说明
 *  本函数隶属于BCU，主要操作是对控制-网络线程之间的通信结构体进行初始化
 *
 *  @param   void
 *  @return  null
 */
void InitThreadCommunicationInfo(void);		//modify  function name too long

/** SetThreadCommunicationInfo函数说明
 *  本函数隶属于BCU，主要操作是对控制-网络线程之间的通信结构体进行初始化
 *
 *  @param   操作类型
 *  @param   数据类型
 *  @param   目标设备名
 *  @param   目标设备号
 *  @return  null
 */
void SetThreadCommunicationInfo(char *,unsigned char);		//modify


/** GlobalInfoInit函数说明
 *  本函数隶属于BCU，主要操作是对全局设备信息进行初始化
 *
 *  @param   void
 *  @return  null
 */
//extern void InitGlobalInfo(void);

/** GlobalInfoUpdate函数说明
 *  本函数隶属于BCU，主要操作是对全局设备信息进行更新
 *
 *  @param   接收到的控制信息结构体
 *  @return  null
 */
//void UpdateGlobalInfo(send_infomation_t);

//根据设备名称判断设备类型
/** JudgeDeviceType函数说明
 *  本函数主要操作是根据设备名得出设备类型
 *
 *  @param   设备名
 *  @return  null
 */
//int JudgeDeviceType(char *devices_name);
int GetDeviceType(char *devices_name);			//modify



#endif /* DATA_STRUCT_H_ */
