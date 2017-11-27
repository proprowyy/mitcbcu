/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： bcu_ann.h
 * 文件标识：
 * 摘    要：The head file of bcu announcement
 *
 * 当前版本： V1.1.0
 * 作    者： wilson
 * 完成日期：2013-01-11
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */
#ifndef BCU_ANN_H_
#define BCU_ANN_H_

#include "data_struct.h"
#include "state_machine.h"
#include "bcu_function.h"
#include "bcu_config.h"
#include "audio_sample.h"

extern unsigned char wherther_send_sync_signal;

extern send_infomation_t recv_cmd_info_of_ann,recv_cmd_info_of_intercom;

extern state_process_t idle_ann_state;
extern state_process_t auto_annoucement_state;
extern state_process_t live_annoucement_state;
extern state_process_t occ_state;
extern state_process_t emergency_annoucement_state;
extern state_process_t manual_annoucement_state;
extern state_process_t mic_outer_3d5_ann_state;

/** IdleAnnEnter function description
 *  This function belongs to BCU，it's the entre of idle announcement state
 *  @param   the point of control information
 *  @return  null
 */
void IdleAnnEnter(send_infomation_t *send_information_idle);

/** IdleAnnExit function description

 *  This function belongs to BCU，it's the exit of idle announcement state
 *  @param   null
 *  @return  null
 */
void IdleAnnExit(void);

/** IdleAnnProcess function description

 *  This function belongs to BCU，it's the process of idle announcement state
 *  @param   the point of control information
 *  @return  null
 */
void IdleAnnProcess(send_infomation_t *send_information_idle);

/** AutoAnnEnter function description
  *  This function belongs to BCU，it's the entre of tms auto announcement state
  *  @param   the point of control information
  *  @return  null
  */
void AutoAnnEnter(send_infomation_t *send_information_tms_auto);

/** AutoAnnExit function description

 *  This function belongs to BCU，it's the exit of tms auto announcement state
 *  @param   null
 *  @return  null
 */
void AutoAnnExit(void);

/** AutoAnnProcess function description

 *  This function belongs to BCU，it's the process of tms auto announcement state
 *  @param   the point of control information
 *  @return  null
 */
void AutoAnnProcess(send_infomation_t *send_information_tms_auto);

/** LiveAnnEnter function description
 *  This function belongs to BCU，it's the entre of live announcement state
 *  @param   the point of control information
 *  @return  null
 */
void LiveAnnEnter(send_infomation_t *send_information_live);

/** LiveAnnExit function description
 *  This function belongs to BCU，it's the exit of live announcement state
 *  @param   null
 *  @return  null
 */
void LiveAnnExit(void);

/** LiveAnnProcess function description
 *  This function belongs to BCU，it's the process of live announcement state
 *  @param   the point of control information
 *  @return  null
 */
void LiveAnnProcess(send_infomation_t *send_information_live);

/** OCCEnter function description
 *  This function belongs to BCU，it's the entre of occ announcement state
 *  @param   the point of control information
 *  @return  null
 */
void OCCEnter(send_infomation_t *send_information_occ);

/** OCCExit function description
 *  This function belongs to BCU，it's the exit of occ announcement state
 *  @param   null
 *  @return  null
 */
void OCCExit(void);

/** OCCProcess function description
 *  This function belongs to BCU，it's the process of occ announcement state
 *  @param   the point of control information
 *  @return  null
 */
void OCCProcess(send_infomation_t *send_information_occ);

/** EmergAnnEnter function description
 *  This function belongs to BCU，it's the entre of emerg announcement state
 *  @param   the point of control information
 *  @return  null
 */
void EmergAnnEnter(send_infomation_t *);

/** EmergAnnExit function description
 *  This function belongs to BCU，it's the exit of emerg announcement state
 *  @param   null
 *  @return  null
 */
void EmergAnnExit();

/** EmergAnnProcess function description
 *  This function belongs to BCU，it's the process of emerg announcement state
 *  @param   the point of control information
 *  @return  null
 */
void EmergAnnProcess(send_infomation_t *);

/** ManualAnnEnter function description
 *  This function belongs to BCU，it's the entre of manual announcement state
 *  @param   the point of control information
 *  @return  null
 */
void ManualAnnEnter(send_infomation_t *send_information_manual);

/** ManualAnnExit function description
 *  This function belongs to BCU，it's the exit of manual announcement state
 *  @param   null
 *  @return  null
 */
void ManualAnnExit(void);

/** ManualAnnProcess function description
 *  This function belongs to BCU，it's the process of manual announcement state
 *  @param   the point of control information
 *  @return  null
 */
void ManualAnnProcess(send_infomation_t *send_information_manual);

/** Outer3D5AnnEnter function description
 *  This function belongs to BCU，it's the entre of outer3D5 announcement state
 *  @param   the point of control information
 *  @return  null
 */
void Outer3D5AnnEnter(send_infomation_t *send_information_manual);

/** Outer3D5AnnExit function description
 *  This function belongs to BCU，it's the exit of outer3D5 announcement state
 *  @param   null
 *  @return  null
 */
void Outer3D5AnnExit(void);

/** Outer3D5AnnProcess function description
 *  This function belongs to BCU，it's the process of outer3D5 announcement state
 *  @param   the point of control information
 *  @return  null
 */
void Outer3D5AnnProcess(send_infomation_t *send_information_manual);

/** CreateSYNCTimer function description
 *  创建设备同步信号定时器
 *
 *  @param   null
 *  @return  null
 */
void CreateSYNCTimer();

/** RestartSYNCTimer function description
 *  重启设备同步信号定时器
 *
 *  @param   null
 *  @return  null
 */
void RestartSYNCTimer();

/** CloseSYNCTimer function description
 *  关闭设备同步信号定时器
 *
 *  @param   null
 *  @return  null
 */
void CloseSYNCTimer();

/** SendSYNCPackageToEamp function description
 *  发送设备同步信号给EAMP
 *
 *  @param   null
 *  @return  null
 */
void SendSYNCPackageToEamp();

/** SendEmergExitToCCU function description
 *  发送紧急广播退出信号给CCU，确保媒体联动可以退出
 *
 *  @param   null
 *  @return  null
 */
void SendEmergExitToCCU();

/** DevSyncHandle function description
 *  发送PA设备同步信号
 *
 *  @param   null
 *  @return  null
 */
void DevSyncHandle();

/** Outer3D5Handle function description
 *  外接3D5触发与结束
 *
 *  @param   null
 *  @return  null
 */
void Outer3D5Handle();

#endif /* BCU_ANN_H_ */
