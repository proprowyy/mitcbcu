/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： bcu_config.h
 * 文件标识：
 * 摘    要：The head file of current bcu configure
 *
 * 当前版本： V1.1.0
 * 作    者： wilson
 * 完成日期：2013-01-09
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */

#ifndef BCU_CONFIG_H_
#define BCU_CONFIG_H_

#include <stdio.h>
#include <string.h>
#include "bcu_function.h"
#include "data_struct.h"
#include "state_machine.h"

#define ALARM_INTERVAL_D2P (6000*20)  //60s * 20 = 20min
#define HEART_BETA 100
#define SAMPLE_AUDIO_FREQUENCE 20//15//21   ///<2014-01-13

#define DEFAULT_LINE_NO 3

#define BCU_PRIORIT 10 ///<BCU thread priority
#define BCU_HIGH_MIC_SAMPLE_PRIORITY 14
#define BCU_HIGH_NETWORK_PRIORITY 15

#define LOCAL_AUDIO_BUF_SIZE 1024 ///<local audio data buffer size

#define SCIF_BAUDRATE CYGNUM_SERAL_BAUD_115200 ///<the baud of the serial


/** GetCurrentDeviceInfomation Function
 * This function is used to get current device name id and car number
 *
 * @param The pointer of device id info
 * @return If success,return 0
*/
int GetCurrentDeviceInfomation(device_id_info_t *this_device_id_info);

/** GetOtherDeviceNo Function
 * This function is used to get other device number
 *
 * @param  device id info
 * @return If success,return 0
*/
void GetOtherDeviceNo(device_id_info_t this_device_id_info);

/** UpdateCurrentDeviceInformation Function
 * Update current device information
 *
 * @param  the pointer of bcu device information
 * @param  the id and name of the bcu
 * @return null
*/
void UpdateCurrentDeviceInformation(device_bcu_state_t *param_bcu_device_info,device_id_info_t param_bcu_device);

#endif /* BCU_CONFIG_H_ */
