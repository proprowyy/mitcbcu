/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称：udp_types.h
 * 文件标识：
 * 摘    要： 触发事件类型定义
 *
 * 当前版本： V1.0.0
 * 作    者： roben_ye
 * 完成日期：2012-9-1
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */

#ifndef UDP_TYPE_H_
#define UDP_TYPE_H_

//roben_ye_0001>>>>>>>>>
#include "data_struct.h"
#include <sys/time.h>

//#define _LED

#define SIZE_LEN 1024//每次发送音频数据的实际长度
#define Cache_PACK_NUM 6

//#roben_ye_0014>>>>>>>>>>>
#pragma pack(1)
//#roben_ye_0014<<<<<<<<<<

/** 本结构体功能：udp层音频数据打包
 */
typedef struct
{
	unsigned char end_flags;			   		 ///<1:the last package
	int package_counter;	  		 ///<the number of the railroad box
	int send_len;								 ///<the length of the package data buffer
	struct timeval tv;		   				 ///<package send time just before the sendto() function
	unsigned char data_buffer[SIZE_LEN];	 ///<the data we send
}data_package_node_t;

/** 本结构体功能：为UdpRecvFun创建参数，
 */
typedef struct
{
	network_send_package_t *pToCommand;          ///< point to the buffer which is used to receive command package
	data_package_node_t *pToHead;				  ///< point to the cache head. Cache is used to sort thr audio package.
	data_package_node_t *pToData;				  ///< point to the buffer address which is used to restore audio package
											        ///<		received from the network in the recvfrom()
	data_package_node_t *ArrayToHandle[8];		  ///<the array point to the whole cache(a buffer,I built)
													  ///<ArrayToHandle数组的成员指向整个cache，通过数组来实现cache空间的重复利用
}buffer_pointer_t;

//#roben_ye_0014>>>>>>>>>>>
#pragma pack()
//#roben_ye_0014<<<<<<<<<<
#endif /* UDP_TYPE_H_ */
