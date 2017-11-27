/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称：bcu_get_data_from_ccu_type.h
 * 文件标识：
 * 摘    要：BCU数据库更新头文件
 *
 * 当前版本：V1.0.1
 * 作    者：wilson
 * 完成日期：2013-06-26
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */
#ifndef BCU_GET_DATA_FROM_CCU_TYPE_H_
#define BCU_GET_DATA_FROM_CCU_TYPE_H_

#include "struct_type.h"

#ifdef CONFIG_BCU_SYNC_FROM_CCU_NET

///< add for CCU-NET-BCU-SYNC,2013
typedef struct
{
       unsigned int station_no;//站代码
       unsigned char fix_across_station;//
       unsigned char spec_open_door;//特殊开门侧，目前么有使用
       char station_name[STATION_NAME_LENGTH]; ///< chinse station name
       char station_name_english[STATION_NAME_LENGTH];  ///< english station name
}line_station_code_table_item_t;//站台信息表

typedef struct
{
       unsigned int line_no;//线路号
       unsigned char line_direction;//上下行
       unsigned char spec_cycle;//是否环线
       unsigned char short_name[100];//描述列
       unsigned char start_time[10];//开始事件
       unsigned char end_time[10];//结束事件
       unsigned char default_section;//默认区间
       unsigned int section_no;//区间号
       unsigned int start_station;//起始站
       unsigned int end_station;//终点站
}line_section_table_item_t;//区间信息表
typedef struct
{
	unsigned char line_no;//线路号
	unsigned char default_line;//默认区间
}line_table_item_t;//线路号表
typedef struct
{
       unsigned int emerg_ann_id;//紧急广播id
       unsigned int line_no;//线路号
       unsigned int repeat_times;//重复次数
       char audio_path[AUDIO_FILE_NAME_LENGTH];//音频路径
       char display_message[EMERG_CONTENT_LENGTH];//显示文字
       char emerg_ann_name[EMERG_NAME_LENGTH];//紧急广播名称
}emerg_table_items_t;//紧急广播表

typedef struct
{
       unsigned int emerg_ann_id;//紧急广播id
       unsigned int line_no;//线路号
       unsigned int repeat_times;//重复次数
       char audio_path[AUDIO_FILE_NAME_LENGTH];//音频路径
       char display_message[EMERG_CONTENT_LENGTH_BETWEEN_BCU_CCU];//显示文字
       char emerg_ann_name[EMERG_NAME_LENGTH];//紧急广播名称
}emerg_table_items_between_bcu_ccu_t;//紧急广播表
typedef struct
{
	unsigned char device_name[10]; //EAMP、PECU、BCU、CCU
	unsigned char device_type;//<<,add,2013-12-30. 4:PCU; 5:CCU; 6:BCU; 7:EAMP
	unsigned char device_no;
	unsigned char ip_address[20];//“192.168.111.222”
	unsigned char mac_address[50];
	unsigned char server_whether;
}ip_mac_table_item_t;

typedef struct
{
	unsigned int version_id;
	unsigned char short_name[100];
	unsigned char time[12];
	unsigned char current_version[100];
}db_version_table_item_t;
typedef struct
{
	unsigned char priority_id;
	char event_type[EVENT_TYPE_LENGTH];
}priority_table_item_t;
typedef struct
{
	//unsigned char device_type;
	int device_number_count;
	char device_name[DEVICE_NAME_LENGTH];
}device_number_table_item_t;
typedef struct
{
        char even_type[20];
	char path_name[100];
}record_path_table_item_t;
typedef struct
{
        char even_type[20];
	char path_name[100];
}log_path_table_item_t;
typedef struct
{
        char even_type[20];
	char path_name[100];
}file_path_table_item_t;

typedef struct
{
      int  d2d_volume;
      int  d2p_volume;
      int  d2eamp_volume;
      int  d2listen_volume;	
}pu_volume_table_item_t;

typedef struct
{
	char ts_password[10];
}pu_ts_passeord_table_item_t;

///< ach add-pang,2013
typedef struct
{
   unsigned int line_no;
   unsigned int total_row;    ///< total stations of one line
   unsigned int current_index;  ///< For example, current_index=1 mean that we have received the first station information
   line_station_code_table_item_t  current_row_info; ///< one station information
}  line_station_code_row_t;

typedef struct
{
      unsigned int line_no;
      unsigned int total_row;  ///< total sections of one line
      unsigned int current_index; ///< 
      line_section_table_item_t  current_row_info;
}  line_section_row_t;

typedef struct
{
      unsigned int total_row;
      unsigned int current_index;
      line_table_item_t  current_row_info;
}  default_line_table_row_t;

typedef struct
{
      unsigned int line_no;
      unsigned int total_row;
      unsigned int current_index;
      emerg_table_items_between_bcu_ccu_t  current_row_info;
}  emerg_ann_table_row_t;

typedef struct
{
      unsigned int total_row;
      unsigned int current_index;
      ip_mac_table_item_t   current_row_info;
}  ip_mac_table_row_t;

typedef struct
{
      unsigned int total_row;
      unsigned int current_index;
      db_version_table_item_t  current_row_info;
}  ann_db_version_table_row_t;

typedef struct
{
      unsigned int total_row;
      unsigned int current_index;
      db_version_table_item_t  current_row_info;
}  config_db_version_table_row_t;

typedef struct
{
      unsigned int total_row;
      unsigned int current_index;
      priority_table_item_t  current_row_info;
}  priority_table_row_t;

typedef struct
{
      unsigned int total_row;
      unsigned int current_index;
      device_number_table_item_t  current_row_info;
}  device_number_table_row_t;

typedef struct
{
      unsigned int total_row;
      unsigned int current_index;
      record_path_table_item_t  current_row_info;	  
}  record_path_table_row_t;

typedef struct
{
      unsigned int total_row;
      unsigned int current_index;
      log_path_table_item_t  current_row_info;	
}  log_path_table_row_t;

typedef struct
{
      unsigned int total_row;
      unsigned int current_index;
      file_path_table_item_t  current_row_info;	
}  file_path_table_row_t;

typedef struct
{
      unsigned int total_row;
      unsigned int current_index;
      pu_volume_table_item_t  current_row_info;
}  pu_volume_table_row_t;
///< add for CCU-NET-BCU-SYNC,2013
typedef struct
{
	 unsigned int total_row;
	 unsigned int current_index;
	 pu_ts_passeord_table_item_t current_row_info;
}pu_ts_password_table_row_t;

typedef struct
{
      unsigned int station_no;
      unsigned char door_side[2]; ///< <1>: left;  <2>: right;  [0]: up, [1]: down
}  line_station_door_side_array_t;
typedef struct
{
      unsigned int line_no;
      unsigned int section_no;
      unsigned int total_bytes;
      unsigned int array_items; ///< how much station for this line and this section
      line_station_door_side_array_t  door_side_array[0];
}  line_station_door_side_t;

typedef union
{
	line_station_code_row_t  station_code_table_row;
	line_section_row_t  line_section_table_row;
	default_line_table_row_t    default_line_table_row;
	emerg_ann_table_row_t  emerg_ann_table_row;
	ip_mac_table_row_t  ip_mac_table_row;
	ann_db_version_table_row_t  ann_db_version_table_row;
	config_db_version_table_row_t  config_db_version_table_row;
	priority_table_row_t  priority_table_row;
	device_number_table_row_t   device_number_table_row;
	record_path_table_row_t  record_path_table_row;
	log_path_table_row_t  log_path_table_row;
}bcu_get_db_data_from_ccu_t;

typedef struct
{
	unsigned char bcu_get_data_from_ccu_event_type;
	bcu_get_db_data_from_ccu_t bcu_db_data;
}bcu_get_data_from_ccu_t;

#endif /* CONFIG_BCU_SYNC_FROM_CCU_NET */

#endif /* BCU_GET_DATA_FROM_CCU_TYPE_H_ */

