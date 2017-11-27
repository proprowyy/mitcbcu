/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： function_for_network.cxx
 * 文件标识：
 * 摘    要： 与网络线程共用函数
 *
 * 当前版本： V1.0.0
 * 作    者： wilson
 * 完成日期：2014-5-21
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */
#include "function_for_network.h"
unsigned char second = 0;
unsigned char network_priority = 10;
unsigned char ts_priority = 10;
unsigned char control_priority = 10;
current_time_t GetCurrentTCMSTime()
{///<获取当前时间，目前没有使用
	current_time_t a;
	return a;
}

void SetFailureTimeFlag()
{///<设置故障时间标志，预留接口

}
void SendHeartBetaToPeripheral()
{///<发送外围设备接口心跳包，预留接口

}
