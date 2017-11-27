/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： bcu_intercom.h
 * 文件标识：
 * 摘    要：The head file of bcu intercom
 *
 * 当前版本： V1.1.0
 * 作    者： wilson
 * 完成日期：2013-01-11
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */
#ifndef BCU_INTERCOM_H_
#define BCU_INTERCOM_H_

#include "data_struct.h"
#include "state_machine.h"
#include "bcu_function.h"
#include "bcu_config.h"
#include "alarm_audio_data.h"

extern int whether_d2d_hangup_d2p_timeout;
extern int begin_to_broadcast_d2d;
extern unsigned  char  current_is_broadcastting_pcu_request_alarm_audio_data;

extern state_process_t idle_intercom_state;
extern state_process_t  d2d_intercomm_state;
extern state_process_t  d2p_intercomm_state;
extern state_process_t  intercomm_hang_up_state;

/*This timer is used to close the pcu,when driver forget to close the pcu request*/
extern cyg_handle_t  counter_handle_d2d_hangup_d2p_timeout;
extern cyg_handle_t alarm_handle_d2d_hangup_d2p_timeout;
extern cyg_alarm alarm_object_d2d_hangup_d2p_timeout;

/** CreateD2DHangUpD2PTimeOutTimer function description
 *  It belongs to BCU
 *	  Function:Create the d2d hangup d2p timeout timer
 *
 *  @param   null
 *  @return  if create successfully return 0
 */
int CreateD2DHangUpD2PTimeOutTimer();

/** DeleteHeartBetaTimer function description
 *  It belongs to BCU
 *	  Function:Delete the d2d hangup d2p timeout timer
 *
 *  @param   null
 *  @return  if delete successfully return 0
 */
int DeleteD2DHangUpD2PTimeOutTimer();

/** alarm_func_handle_d2d_hangup_d2p_timeout function description
 *  It belongs to BCU
 *	  Function:d2d hangup d2p timeout timer function
 *
 *  @param   timer handle
 *  @param   private data
 *  @return  null
 */
void alarm_func_handle_d2d_hangup_d2p_timeout(cyg_handle_t counter_handle_heart_beat, cyg_addrword_t data);

/** SetD2DHangUpD2PTimeOutTimeInterval function description
 *  It belongs to BCU
 *	  Function:Set the d2d hangup d2p timeout timer time interval
 *
 *  @param   time interval
 *  @return  if set successfully return 0
 */
int SetD2DHangUpD2PTimeOutTimeInterval(int time_interval);

/** OpenD2DHangUpD2PTimeOutTimer function description
 *  It belongs to BCU
 *	  Function:Open the d2d hangup d2p timeout timer
 *
 *  @param   null
 *  @return  if open successfully return 0
 */
int OpenD2DHangUpD2PTimeOutTimer();

/** CloseD2DHangUpD2PTimeOutTimer function description
 *  It belongs to BCU
 *	Function:Close the d2d hangup d2p timeout timer
 *
 *  @param   null
 *  @return  if close successfully return 0
 */
int CloseD2DHangUpD2PTimeOutTimer();

/** StartD2DHangUpD2PTimeOutTimer function description
 *  It belongs to BCU
 *	Function:Start the d2d hangup d2p timeout timer
 *
 *  @param   null
 *  @return  null
 */
void StartD2DHangUpD2PTimeOutTimer();

/** IdleIntercomEnter function description
 *  It belongs to BCU
 *	Function:The enter of intercom idle
 *
 *  @param   pointer of send cmd infotmation
 *  @return  null
 */
void IdleIntercomEnter(send_infomation_t *send_information_intercomm_idle);

/** IdleIntercomExit function description
 *  It belongs to BCU
 *	Function:The exit of intercom idle
 *
 *  @param   null
 *  @return  null
 */
void IdleIntercomExit();

/** IdleIntercomProcess function description
 *  It belongs to BCU
 *	Function:The process of intercom idle
 *
 *  @param   pointer of send cmd infotmation
 *  @return  null
 */
void IdleIntercomProcess(send_infomation_t *send_information_intercomm_idle);

/** D2DIntercomEnter function description
 *  It belongs to BCU
 *	Function:The enter of D2D intercom
 *
 *  @param   pointer of send cmd infotmation
 *  @return  null
 */
void D2DIntercomEnter(send_infomation_t *local_send_information);

/** D2DIntercomExit function description
 *  It belongs to BCU
 *	Function:The exit of D2D intercom
 *
 *  @param   null
 *  @return  null
 */
void D2DIntercomExit();

/** D2DIntercomProcess function description
 *  It belongs to BCU
 *	Function:The process of D2D intercom
 *
 *  @param   pointer of send cmd infotmation
 *  @return  null
 */
void D2DIntercomProcess(send_infomation_t *send_information);

/** D2PIntercomEnter function description
 *  It belongs to BCU
 *	Function:The enter of D2P intercom
 *
 *  @param   pointer of send cmd infotmation
 *  @return  null
 */
void D2PIntercomEnter(send_infomation_t *send_information_intercomm_d2p);

/** D2PIntercomExit function description
 *  It belongs to BCU
 *	Function:The exit of D2P intercom
 *
 *  @param   null
 *  @return  null
 */
void D2PIntercomExit();

/** D2PIntercomProcess function description
 *  It belongs to BCU
 *	Function:The process of D2P intercom
 *
 *  @param   pointer of send cmd infotmation
 *  @return  null
 */
void D2PIntercomProcess(send_infomation_t *send_information_intercomm_d2p);

/** IntercomHangUpEnter function description
 *  It belongs to BCU
 *	Function:The enter of D2D hangup D2P intercom
 *
 *  @param   pointer of send cmd infotmation
 *  @return  null
 */
void IntercomHangUpEnter(send_infomation_t *send_information_intercomm_hangup);

/** IntercomHangUpExit function description
 *  It belongs to BCU
 *	Function:The exit of D2D hangup D2P intercom
 *
 *  @param   nulln
 *  @return  null
 */
void IntercomHangUpExit();

/** IntercomHangUpProcess function description
 *  It belongs to BCU
 *	Function:The process of D2D hangup D2P intercom
 *
 *  @param   pointer of send cmd infotmation
 *  @return  null
 */
void IntercomHangUpProcess(send_infomation_t *send_information_intercomm_hangup);

/** BcuResetPlayAlarmAudioWhenD2pReq function description
 *	BCU重置PCU请求提示音播放
 *
 *  @param   null
 *  @return  if ok return 0
 */
int BcuResetPlayAlarmAudioWhenD2pReq(void); ///< add, 0107

/** BcuInitPlayAlarmAudioWhenD2pReq function description
 *	BCU初始化PCU请求提示音播放
 *
 *  @param   null
 *  @return  if ok return 0
 */
int BcuInitPlayAlarmAudioWhenD2pReq(void);

/** StartOrBrokeBroadcastPcuRequestAlarmAudioData function description
 *	开始播放PCU请求提示音
 *
 *  @param   null
 *  @return  null
 */
void  StartOrBrokeBroadcastPcuRequestAlarmAudioData();

/** AdjustVolumeAfterCODEC function description
 *	调节BCU音量，声卡音量，codec模式下
 *
 *  @param   null
 *  @return  null
 */
void AdjustVolumeAfterCODEC();

/** D2DReqAndResponseHandle function description
 *	司机对讲请求与挂断处理
 *
 *  @param   null
 *  @return  null
 */
void D2DReqAndResponseHandle();

/** KeepComminicationWithPCU function description
 *	司机对讲请求与挂断处理
 *
 *  @param   null
 *  @return  null
 */
void KeepComminicationWithPCU();

extern int GetSndCurrentMode(void); ///< add, 0107
extern int CloseAudioSampleTimerForPcuAlarm(void); ///< add, 0107

#endif /* BCU_INTERCOM_H_ */
