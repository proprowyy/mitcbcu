/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称：thread_comm.h
 * 文件标识：
 * 摘    要：the header of thread communications
 *
 * 当前版本：V1.0.0
 * 作    者：wilson
 * 完成日期：2012-10-09
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */
#ifndef THREAD_COMM_H_
#define THREAD_COMM_H_

#include "data_struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cyg/kernel/kapi.h>
#include <pthread.h>
#include <semaphore.h>
#include "bcu_function.h"
#include "state_machine.h"

/*The declaration of mutex*/
extern pthread_mutex_t mutex_of_control_to_network_comm;/*the mutex between control thread and netwoek thread*/
extern pthread_mutex_t mutex_of_global_device_info;/*the mutex of global info struct*/

/*The declaration of semaphore*/
extern sem_t  sem_wakeup_bcu_mic_sample;
extern sem_t sem_demao;

#ifdef CONFIG_TEST_SND_IN_MULTI_THREAD
extern sem_t  sem_snd_card_self_test;
#endif

extern volatile global_device_info_state_t global_device_info_state;

/** InitMutex function description
 *  This function belongs to bcu,which is used to initialize the mutex
 *
 *  @param   null
 *  @return  null
 */
void InitMutex();

/** InitSemaphore function description
 *  This function belongs to bcu，it's used to initialize the semaphore
 *  @param   the pointer of semaphore
 *  @return  null
 */
void InitSemaphore(sem_t *param_sem);

/** InitAudioDataDestination function description
 *  This function belongs to bcu,which is used to initialize the destination of audio data
 *
 *  @param   null
 *  @return  null
 */
void InitAudioDataDestination(void);

/** ClearAudioDataDestination function description
 *  This function belongs to bcu,which is used to clear the destination of audio data
 *
 *  @param   null
 *  @return  null
 */
void ClearAudioDataDestination(void);

/** SetAudioDataDestination function description
 *  This function belongs to bcu,which is used to set the destination of audio data
 *
 *  @param   destination device name
 *  @param   destination device no
 *  @return  null
 */
void SetAudioDataDestination(char *device_name, unsigned char device_no);


/** InitAllDeviceInfo function description
 *  This function belongs to bcu,which is used to Initial all device information
 *
 *  @param   null
 *  @return  null
 */
void InitAllDeviceInfo();


/** UpdataDeviceInformation function description
 *  This function belongs to bcu,which is used to Update  device information
 *
 *  @param   received cmd information
 *  @return  null
 */
void UpdataDeviceInformation(send_infomation_t recv_package);


/** InitGlobalInfo function description
 *  This function belongs to bcu,which is used to Initial global device information struct
 *
 *  @param   null
 *  @return  null
 */
void InitGlobalInfo();

/** UpdateGlobalInfo function description
 *  This function belongs to bcu,which is used to Update  global device information struct
 *
 *  @param   received cmd information
 *  @return  null
 */
void UpdateGlobalInfo(send_infomation_t receive_package);

/** AdjustPCURequestSequence function description
 *  This function belongs to bcu,which is used to adjust PCU request sequence
 *
 *  @param   the PCU request sequence number,which we want to close
 *  @return  null
 */
void AdjustPCURequestSequence(int param_request_sequence);

/** SetDSTDeviceNameAndNumber function description
 *  This function belongs to bcu,which is used to Set the destination device.which is offer to network thread
 *
 *  @param   device's name's pointer
 *  @param   device's number
 *  @return  null
 */
void SetDSTDeviceNameAndNumber(char *device_name,int device_number);

/** DSTDeviceNameAndNumber function description
 *  This function belongs to bcu,which is used to Get the destination device's name and number
 *
 *  @param   device's name's pointer
 *  @param   device's number
 *  @return  null
 */
void DSTDeviceNameAndNumber(char *dst_device_name,int *dst_device_number);

/** UpdataGlobalDeviceInfo function description
 *  Update global device information
 *
 *  @param   cmd informationl
 *  @return  null
 */
void UpdataGlobalDeviceInfo(send_infomation_t );

/** ClearGlobalDeviceInfo function description
 *  Clear global device information
 *
 *  @param   null
 *  @return  null
 */
void ClearGlobalDeviceInfo();

/** ShowPCURequest function description
 *  Show pcu request
 *
 *  @param   null
 *  @return  null
 */
void ShowPCURequest();

/** ScreenCopyGlobalDeviceInfo function description
 *  Copy global struct
 *
 *  @param   null
 *  @return  null
 */
void ScreenCopyGlobalDeviceInfo(global_device_info_state_t *);

/** TryScreenCopyGlobalDeviceInfo function description
 *  Try Copy global struct
 *
 *  @param   null
 *  @return  null
 */
int TryScreenCopyGlobalDeviceInfo(global_device_info_state_t *);

/** BCUUpdataPCUDeviceInfo function description
 *  BCU update PCU state
 *
 *  @param   md informationl
 *  @return  null
 */
void BCUUpdataPCUDeviceInfo(send_infomation_t );



#endif /* THREAD_COMM_H_ */
