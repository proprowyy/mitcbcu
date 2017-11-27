/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称：dynamic_map.h
 * 文件标识：
 * 摘    要：动态地图结构体
 *
 * 当前版本：V1.1.0
 * 作    者：wilson
 * 完成日期：2013-6-24
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */

#ifndef DYNAMIC_MAP_H_
#define DYNAMIC_MAP_H_

#ifdef OLD_DYNAMIC_MAP
typedef struct
{
	unsigned char begin_flag;
	unsigned char function_code;
	unsigned char line_no;
	unsigned char running_direction;
	unsigned char begin_station_no;
	unsigned char current_station_no;
	unsigned char next_station_no;
	unsigned char terminal_station_no;
	unsigned char open_door_sider;
	unsigned char over_flag;
	unsigned char check_sum;
}dynamic_map_info_t;
#else
typedef struct
{
	unsigned char begin_flag;
	unsigned char function_code;
	unsigned char line_no;
	unsigned char running_direction;
	unsigned char begin_station_no;
	unsigned char current_station_no;
	unsigned char next_station_no;
	unsigned char terminal_station_no;
	unsigned char open_door_sider;
	unsigned char over_flag;
	unsigned char check_sum;
}dynamic_map_info_t;//动态地图通信结构体，目前没有使用
#endif


#endif /* DYNAMIC_MAP_H_ */
