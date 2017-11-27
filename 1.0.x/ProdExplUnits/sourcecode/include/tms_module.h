/*
 * tms_module.h
 *
 *  Created on: 2013-6-7
 *      Author: benty
 */

#ifndef TMS_MODULE_H_
#define TMS_MODULE_H_


#define TMS_SIG_PACK_HEAD  (0x7E)
#define TMS_SIG_PACK_TYPE_PING  (1)
#define TMS_SIG_PACK_TYPE_SETTING (2)
#define TMS_SIG_PACK_TYPE_DATA  (3)

typedef struct
{
	unsigned char frame_begin_flag;//桢起始符,0x7E作为起始标志
	unsigned char frame_type;//桢类型	  0:正常通讯,1参数设置,2数据透传,
	unsigned char begin_station_no;//起始站代码	 0x00无效0x01~0xC8为站代码0xC9~0xFE特殊信息
	unsigned char terminal_station_no;//终点站代码	 0x00无效0x01~0xC8为站代码0xC9~0xFE特殊信息
	unsigned char next_station_no;//下一站代码	0x00无效0x01~0xC8为站代码0xC9~0xFE特殊信息
	unsigned char skip_station[4];//越站代码	0x00无效0x01~0xC8为站代码0xC9~0xFE特殊信息
	unsigned char running_distance[2];//运行距离，列车到站后清零，0~65535m/0x0000~0xffff.单位为m
	unsigned char emerg_ann_segment_no;//紧急广播段码  0x00无效
	unsigned char door_state_and_time_setting; //位0 关门状态位  1:门正在关闭       0:门处于常态   用于播放关门提示音
													//位1 门状态指示  1:所有门已关闭     0:至少有一个门未关
													//位2 左侧门状态  1:左侧门开         0:左侧门关
													//位3 右侧门状态  1:右侧门开         0:右侧门关
													//位4 到站状态位  1:列车已到站       0:列车未到站
													//位5 广播测试    1:进行广播测试     0:正常状态
													//位6 时间有效    1:系统时间有效     0:系统时间无效
													//位7  保留
	unsigned char door_unlock_flag[10][2];//[x][0]:CARx的1~8门bypass状态显示         每一位指示车厢内一扇门的状态
											  //[x][1]:CARx的9~16门bypass状态显示        1:门被bypass   0:正常
	unsigned char pcu_connect_state[10];//[x]:CARx车厢报警器连接状态   每一位表示车厢内一个报警器的状态  1:报警器连接  0:报警器未连接
	unsigned char total_car_number;//车厢总数   标示一列车有多少节车厢，具体表示根据车辆有所不同
	unsigned char system_time[6];//系统时间（BCD格式）  当系统时间有效位指示为有效时，用于校正系统时间
									//[0]   年 位0-3：年低位（0-9）   位4-7：年高位（0-9）
	                                //[1]   月 位0-3：月低位（0-9）   位4-7：月高位（0-1）
	                                //[2]   日 位0-3：日低位（0-9）   位4-7：日高位（0-3）
									//[3] 小时 位0-3：小时低位（0-9） 位4-7：小时高位（0-2）
									//[4] 分钟 位0-3：分钟低位（0-9） 位4-7：分钟高位（0-5）
									//[5]   秒 位0-3：秒低位（0-9）   位4-7：秒高位（0-5）
	unsigned char manual_setting;//手动设置字节
	unsigned char emerg_alarm_handle;//紧急报警处理
	unsigned char emerg_ann_begin_or_over;//紧急广播开始和停止
											  //字节0：开始紧急广播
											  //字节1：停止紧急广播
	                                          //字节2-7：保留
	unsigned char active_cab;
	unsigned char line_no;
	unsigned char whether_arrive_station;//列车是否到站
	unsigned char up_or_down;
	unsigned char door_side;
	unsigned char reserve[10];//保留
	unsigned char check_sum;//校验和
	unsigned char frame_over_flag;//桢结束符,0x7E作为结束标志
}tms_to_ccu_t;

typedef struct
{
	unsigned char frame_begin_flag;//桢起始符,0x7E作为起始标志
	unsigned char communication_state;//通讯状态	  0:正常通讯,1参数设置,2数据透传,
	unsigned char ccu_state_and_address;//中央控制器状态和地址
											//中央控制器状态:  0~3	 0:错误  1:主  2:备   =3
											//中央控制器地址   4-5    1:本车A车    0:本车F车
											//保留  6-7
	unsigned char DVA_state;//DVA状态  0错误   1空闲   2忙，
							  //0~1	A车DVA状态
							  //2~3	F车DVA状态
							  //4-7 保留
	unsigned char media_host_state;//媒体主机状态  0：错误   1：正常
									  //0~1 	A车媒体主机状态
	                               //2~3	F车媒体主机状态
	                                  //4~7     保留
	unsigned char terminal_station_screen_state;//终点站屏状态   0:错误   1:正常
													 //0~1	A车终点站屏状态
													 //2~3	F车终点站屏状态
													 //4~7     保留
	unsigned char power_state[3];//电源状态 0:错误   1:正常
									//[0] 0-1 A车司机室电源状态   2~3	F车司机室电源状态     4~5	CAR1电源状态   6~7	CAR2电源状态
									//[1] 0~1	CAR3电源状态      2~3	CAR4电源状态         4~5	CAR5电源状态   6~7	CAR6电源状态
									//[2] 0~1	CAR7电源状态      2~3	CAR8电源状态         4~5	CAR9电源状态   6~7	CAR10电源状态
	unsigned char car_led_state[10];//客室LED屏状态 0:错误  1:正常
									   //[x]:0~1	CARx_1客室LED屏状态    2~3	CARx_2客室LED屏状态   4~7	保留
	unsigned char car_pcu_state[10];//客室报警器状态 0:错误   1:报警等待响应  2:对讲中  3:正常(无报警)
	                               //[x]:0~1	CARx_1客室报警器状态   2~3	CARx_2客室报警器状态  4~7	保留
	unsigned char car_controler_state[3];//客室控制器状态   0:错误  1:正常
	                                     //[0] 0~1	CAR1客室控制器状态 2~3	CAR2客室控制器状态 4~5	CAR3客室控制器状态 6~7	CAR4客室控制器状态
											 //[1] 0~1	CAR5客室控制器状态 2~3	CAR6客室控制器状态 4~5	CAR7客室控制器状态 6~7	CAR8客室控制器状态
											 //[2] 0~1	CAR9客室控制器状态 2~3	CAR10客室控制器状态 4~7 保留
	unsigned char OPA_state[3];//客室运放状态   0:错误  1:正常 2:空闲
								  //[0] 0~1	CAR1客室运放状态  2~3	CAR2客室运放状态   4~5	CAR3客室运放状态 	6~7	CAR4客室运放状态
		                       //[1] 0~1	   CAR5客室运放状态  2~3	CAR6客室运放状态   4~5	CAR7客室运放状态 	6~7	CAR8客室运放状态
		 	 	 	 	 	     //[2] 0~1	   CAR9客室运放状态  2~3	CAR10客室运放状态  4~7 保留
	unsigned char DCP_state;//DCP状态	0~3	A车司机室DCP状态   4~7	F车司机室DCP状态
	unsigned char line_no;//线路号
	unsigned char reserve[31];//保留
	unsigned char check_sum;//校验和
	unsigned char frame_over_flag;//桢结束符,0x7E作为结束标志
}ccu_to_tms_t;

typedef struct
{
	unsigned char begin_station_no;//起始站站代码
	unsigned char terminal_station_no;//终点站站代码
	unsigned char current_station_no;//当前站站代码
	unsigned char next_station_no;//下一站站代码
	unsigned char line_no;//线路号
	unsigned char up_or_down;//上下行
}auto_ann_station_info_t;



typedef struct
{
	auto_ann_station_info_t auto_ann_station_info;//自动报站控制命令包
}tms_info_t;

#endif /* TMS_MODULE_H_ */
