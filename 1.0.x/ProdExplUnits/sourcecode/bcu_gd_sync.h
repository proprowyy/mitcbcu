/*
 * bcu_gd_sync.h
 *
 *  Created on: 2013-6-27
 *      Author: benty
 */

#ifndef BCU_GD_SYNC_H_
#define BCU_GD_SYNC_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <cyg/kernel/kapi.h> //for alarm

#include "data_struct.h"
#include "buffer_read_write_fun.h"
#include "bcu_state_machine.h"
#include "alarm_list_fun.h"
//#include "sqlite_api_fun.h"

#include "struct_type.h"
#include "bcu_function.h"

#ifdef CONFIG_BCU_SYNC_FROM_CCU_NET
#include "bcu_update_table_from_ccu.h"
#include "bcu_get_data_from_ccu_type.h"
#endif

#define CONFIG_BCU_TOTAL  2 
#define CONFIG_CCU_TOTAL  2 
#define CONFIG_EAMP_TOTAL  8 
#define CONFIG_PCU_TOTAL  16

#define CCU_STEP_SYNC_WITH_SM

//mask for field updating,目前暂时未用
#define GD_SYNC_FIELD_DEV_NAME                                   0x01
#define GD_SYNC_FIELD_DEV_TYPE                                      0x02
#define GD_SYNC_FIELD_DEV_NO                                          0x04
#define GD_SYNC_FIELD_DEV_ANN_STATE                         0x08
#define GD_SYNC_FIELD_DEV_INTERCOMM_STATE        0x10
#define GD_SYNC_FIELD_CONFIG_DB_VERSION                0x20
#define GD_SYNC_FIELD_ANN_DB_VERSION                       0x40
#define GD_SYNC_FIELD_DEV_CURRENT_STATE                0x80
#define GD_SYNC_FIELD_SERVER_FLAG                                0x100
#define GD_SYNC_FIELD_DKEY_FLAG                                  0x200

/****************  struct for database, begin, 2013 ****************************/
/********************************************************************/
#define STATION_NAME_MAX_BYTES  32

//#1:  when power up, we can read all items from station table,2013
//For example, there are 32 stations, so there are  32 * 45 bytes = 1440
typedef struct StationTableItemType
{
       unsigned int station_id;
       unsigned int line_no;	   
       unsigned int station_sequeue; // 1,2,3,
       unsigned char skip_this_station; // [1]: skip it forever
       unsigned char station_name[STATION_NAME_MAX_BYTES];
}station_table_item_t;

//#2:  section talbe
typedef struct SectionTableItemType
{
       unsigned int section_id;
       unsigned int line_no;
       unsigned char line_direction; // [1]: up;  [2]: down
       unsigned char setion_no;
       unsigned int start_station_id;
       unsigned int end_station_id;	   
}section_table_item_t;

//#3:  announce talbe
typedef struct AnnServiceTableItemType
{
       unsigned int section_id;
       unsigned int current_station_id;
       unsigned int service_type;	
       unsigned int next_station_id;
       unsigned int audio_file_id;	
       unsigned int led_file_id;
       unsigned int repeat_times;
       unsigned int distance_delta;
       unsigned int distance_arrive_trigger;
       unsigned int distance_to_next_station;
       unsigned int distance_forecast_trigger;
}ann_service_table_item_t;

//#4:  emergency talbe
typedef struct EmergServiceTableItemType
{
       unsigned int emerg_ann_id;
       unsigned int line_no;
       unsigned int repeat_times;
       unsigned int audio_file_id;	
       unsigned int led_file_id;	   
}emerg_service_table_item_t;

//#5: audio file path talbe
#define AUDIO_FILE_PATH_MAX_LEN  32
typedef struct AudioFilePathTableItemType
{
       unsigned int audio_file_id;	
       char audio_file_path[AUDIO_FILE_PATH_MAX_LEN];
}audio_file_path_table_item_t;

//#6:  display chars talbe for LED/LCD
#define LED_DISPLAY_MAX_BYTES  256   // 128 chinese if GB2312
typedef struct LedCharTableItemType
{
       unsigned int led_char_id;
       char led_char[LED_DISPLAY_MAX_BYTES];
}led_char_table_item_t;

//following struct is for EAMP
typedef struct AnnServiceTableMemoryType
{
       unsigned int current_station_id;
       unsigned int service_type;	
       unsigned int next_station_id;
       char audio_file_path[AUDIO_FILE_PATH_MAX_LEN];
       char led_char[LED_DISPLAY_MAX_BYTES];
       unsigned int repeat_times;
       unsigned int distance_delta;
       unsigned int distance_arrive_trigger;
       unsigned int distance_to_next_station;
       unsigned int distance_forecast_trigger;
}ann_service_table_memory_t;
#define  TOTAL_OF_ALL_STATIONS  32
//ann_service_table_memory_t  eamp_current_ann_table[TOTAL_OF_ALL_STATIONS]; //[320]x32 = 10KiB for one section line

/********************************************************************/
/****************  struct for database, end, 2013 ****************************/
typedef struct CcuServerTableItemType
{
      unsigned char flag;
      unsigned int  connect_count;
}ccu_server_table_t;
typedef struct DevConfigTableItemType
{
       unsigned int dev_type;
       unsigned int dev_no;
       unsigned int db_server_dev_no;
       unsigned int config_db_version;
       unsigned int ann_db_version;
       char dev_name[DEVICE_NAME_LENGTH];
       char db_server_dev_name[DEVICE_NAME_LENGTH];	   
       char db_server_ip[IP_NAME_LENGTH];
       char dev_ip[IP_NAME_LENGTH]; //local ip for tftp
       unsigned char is_db_server;
       unsigned char driver_key;
       ccu_server_table_t  db_ccu_status[4]; ///< <1>: CCU-1;  <2>: CCU-2	   
} dev_config_table_item_t;

typedef struct DevAnnTableItemType
{
       unsigned int ann_db_version;	   
       unsigned int line_no;
       unsigned int start_station;	
       unsigned int end_station;
       //ann_station_info_t *plist_head;
} dev_ann_table_item_t;

typedef struct DevInfoTableUpdateType
{
       unsigned int dev_type;
       unsigned int dev_no;
       unsigned int config_db_version;
       unsigned int ann_db_version;	   
       unsigned int gd_mask;	   
       char dev_name[DEVICE_NAME_LENGTH];	   
       unsigned char is_db_server;
       unsigned char driver_key;	   
       unsigned char dev_stat;
       unsigned char stat_context[GD_DEV_INFO_MAX_LEN];
} dev_info_table_update_t;

typedef struct DevVersionTableType
{
       unsigned char used;  
       unsigned char dev_no;  
       char config_db_version[CONFIG_CCU_VERSION_STRING_LEN];
       char ann_db_version[CONFIG_CCU_VERSION_STRING_LEN];
} dev_version_table_t;
typedef struct LcdDevVersionTableType
{  
       char *config_db_version;
       char *ann_db_version;
} lcd_dev_version_table_t;

//global data sync struct, begin
#define GD_SYNC_STATE_IDLE  0x100
#define GD_SYNC_STATE_CCU_DB_SYNC  0x101
#define GD_SYNC_STATE_BEP_DB_SYNC  0x102
#define GD_SYNC_STATE_CCU_DEV_SYNC  0x103 //dev from bcu ---> ccu

#define GD_SYNC_STATE_TRIGGER_IDLE  0x200
#define GD_SYNC_STATE_TRIGGER_PU_READY 0x201
#define GD_SYNC_STATE_TRIGGER_CCU_DB_SYNC  0x202
#define GD_SYNC_STATE_TRIGGER_BEP_DB_SYNC  0x202
#define GD_SYNC_STATE_TRIGGER_CCU_DEV_SYNC  0x203

///< 目前,BCU GD代码未使用状态切换,保留
typedef struct
{
	void (*enter)(void *);
	void (*exit)(void *);
	void (*process)(void *);
	int state_id;
}gd_state_process_t;

typedef struct
{
	gd_state_process_t *psrc_gd_state;
	int trigger_event;	
	gd_state_process_t *pdest_gd_state;
}gd_state_rule_table_t;

typedef struct
{
     dev_config_table_item_t   *plocal_dev_config_info;
     gd_state_process_t  *plocal_curr_gd_state;	 
     network_send_package_t  *precv_net_cmd;
     unsigned int send_net_cmd_buffer_id;
     unsigned int recv_net_cmd_buffer_id;
     unsigned int soft_count;	 
}gd_state_param_t;

typedef struct
{
      unsigned char bcu_state_count[CONFIG_BCU_NUM];
      unsigned char ccu_state_count[CONFIG_CCU_NUM];
      unsigned char eamp_state_count[CONFIG_EAMP_NUM];
      unsigned char pcu_state_count[CONFIG_PECU_NUM];	  
}gd_dev_state_count_t;

typedef struct
{
     unsigned int server_dev_no;
     char server_dev_name[DEVICE_NAME_LENGTH];
     char server_dev_ip[IP_NAME_LENGTH];	  
}db_server_update_t;
//global data sync, end

void BcuAlarmSyncWaiting(cyg_handle_t bcu_alarm_handle_sync_waiting, cyg_addrword_t data);
int BcuCheckDriverKey(int device_type_no, int io_no);
int BcuGdGetLocalDevIO(int *device_type_no, int *io_no);
extern void BCUSetDataFromCCUState(int value);
extern void CreateBCUEmergSpace(int param_emerg_total_items);
#ifndef  CONFIG_BCU_SYNC_FROM_CCU_NET

int BcuGetAllFromStationTable(int line_no);
int BcuGdSendLocalDbVersion(int net_send_cmd_buffer_id, network_send_package_t  *pnet_recv_cmd, unsigned int local_db_version);
int BcuGdSetLocalGdTable(global_device_info_state_t  *gd_dev_table);
global_device_info_state_t  *BcuGdGetLocalGdTable(void);
int BcuSqlite3GetDevConfigInfo(dev_config_table_item_t  *pdev_config_info);
int BcuGdGetDevConfigFromDB(dev_config_table_item_t  *pdev_config_info);
int BcuGdUpdateLocalDevInfo(dev_info_table_update_t *dev_info_update);
int BcuGdLocalDevConfigInit(dev_config_table_item_t  *pdev_config_info);
int BcuGdSyncDbBcu(int gd_cmd_rx_buffer_id);
int BcuGdSendDbSyncRequest(char *dest_dev_name, int dest_dev_no, int net_cmd_buffer_id, int src_dev_no);
int BcuGdDetectRemoteBcu(int net_cmd_buffer_id, int src_dev_no);
int BcuCheckGlobalDeviceInfoChanged(gd_dev_state_count_t *last_dev_state_count_array);
int BcuCopyNetCmdForGdThread(network_send_package_t  *pnet_new_cmd_rx);
int BcuCopyTouchCmdForGdThread(send_infomation_t *ptouch_new_cmd);
int BcuGdDbVersionTftpRequest(char *server_ip, char *target_file, char *client_file);
int BcuSendDbSyncReq(int cmd_tx_buffer_id, dev_config_table_item_t  *pdev_config_info);
void *BcuGdThreadEntry(void *arg);
#else // we define CONFIG_BCU_SYNC_FROM_CCU_NET,目前使用这里
int BcuCopyNetCmdForGdThread(network_send_package_t  *pnet_new_cmd_rx);
int BcuCopyTouchCmdForGdThread(send_infomation_t *ptouch_new_cmd);
int BcuSendDbSyncReq(int cmd_tx_buffer_id, dev_config_table_item_t  *pdev_config_info);
int BcuGdGetDbFromServerCcu( common_big_package_t *pnet_recv_cmd );
void *BcuGdThreadEntry(void *arg);
#endif

/*********************************************************
 ** @brief      LCD thread will get bcu/ccu/eamp/pcu version
 ** @param   type:         private type, <1>: bcu; <2>: ccu; <3>: eamp; <4>: pcu
 ** @param   dev_no:    devie number
 ** @param   lcd_dev_version:  save version string pointer for LCD thread
 ** @return   0: LCD thread will get version string successfully
 ** @return  -1: null pointer or blank string
 ** @see
 ** @note
 ********************************************************/
int BcuReadDeviceVersionTableItem(int type, int dev_no, lcd_dev_version_table_t *lcd_dev_version);

extern unsigned char db_version_last_type;
#endif /* BCU_GD_SYNC_H_ */
