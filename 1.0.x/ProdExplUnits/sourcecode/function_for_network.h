/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： function_for_network.h
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

#ifndef FUNCTION_FOR_NETWORK_H_
#define FUNCTION_FOR_NETWORK_H_

#include "bcu_function.h"

#define BCU_DEV

extern unsigned char network_priority ;
extern unsigned char ts_priority;
extern unsigned char control_priority;
extern pthread_t thread_of_network;

/** GetCurrentTCMSTime function description
*  获取当前TCMS时间
*
*  @param   null
*  @return  当前tcms时间
*/
current_time_t GetCurrentTCMSTime();

/** SetFailureTimeFlag function description
*  设置发送外围设备心跳包标志
*
*  @param   null
*  @return  null
*/
void SetFailureTimeFlag();

/** SendHeartBetaToPeripheral function description
 *  send heart beta to peripheral
 *
 *  @param   null
 *  @return  null
 */
void SendHeartBetaToPeripheral();
#endif /* FUNCTION_FOR_NETWORK_H_ */
