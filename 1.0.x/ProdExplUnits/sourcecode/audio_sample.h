/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： audio_sample.h
 * 文件标识：
 * 摘    要：The head file of sample audio
 *
 * 当前版本： V1.1.0
 * 作    者： wilson
 * 完成日期：2013-01-11
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */

#ifndef AUDIO_SAMPLE_H_
#define AUDIO_SAMPLE_H_

#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/plf_io.h>
#include "thread_comm.h"
#include "bcu_config.h"
#include "bcu_function.h"
#include "sound_driver.h"

/*This timer is used to sample the audio data*/
extern cyg_handle_t  counter_handle_sample_audio;
extern cyg_handle_t alarm_handle_sample_audio;
extern cyg_alarm alarm_object_sample_audio;
extern cyg_io_handle_t bcu_audio_handle;;
extern unsigned char whether_open_sample_timer;
extern int play_audio;
extern unsigned char whether_have_finished_broadcast_audio_data;
extern int begin_to_broadcast_audio_data;
extern unsigned char whe_d2d_souder_have_been_worked;
extern unsigned char whe_d2d_souder_stable;

/** CreateAudioSampleTimer function description
 *  It belongs to BCU
 *	Function:Create the audio sample timer
 *
 *  @param   null
 *  @return  if create successfully return 0
 */
int CreateAudioSampleTimer();

/** DeleteAudioSampleTimer function description
 *  It belongs to BCU
 *	  Function:Delete the audio sample timer
 *
 *  @param   null
 *  @return  if delete successfully return 0
 */
int DeleteAudioSampleTimer();

/** alarm_func_handle_audio_sample function description
 *  It belongs to BCU
 *	  Function:audio sample timer function
 *
 *  @param   timer handle
 *  @param   private data
 *  @return  null
 */
void alarm_func_handle_audio_sample(cyg_handle_t counter_handle, cyg_addrword_t data);

/** SetAudioSampleTimeInterval function description
 *  It belongs to BCU
 *	  Function:Set the audio sample timer time interval
 *
 *  @param   time interval
 *  @return  if set successfully return 0
 */
int SetAudioSampleTimeInterval(int time_interval);

/** OpenAudioSampleTimer function description
 *  It belongs to BCU
 *	  Function:Open the audio sample timer
 *
 *  @param   null
 *  @return  if open successfully return 0
 */
int OpenAudioSampleTimer();

/** CloseAudioSampleTimer function description
 *  It belongs to BCU
 *	  Function:Close the audio sample timer
 *
 *  @param   null
 *  @return  if close successfully return 0
 */
int CloseAudioSampleTimer();

/** StartAudioSampleTimer function description
 *  It belongs to BCU
 *	  Function:Start the audio sample timer
 *
 *  @param   null
 *  @return  if close successfully return 0
 */
void StartAudioSampleTimer();

/** ResetSoundTimer function description
 *	 音频播放与采集线程定时器时间重置
 *
 *  @param   null
 *  @return  null
 */
void ResetSoundTimer();


/** PutMicSampleDataintoBuffer_EAMP function description
 *  It belongs to BCU
 *	  Function:play audio data
 *
 *  @param   handle of sound card
 *  @param   audio data buffer id
 *  @return  null
 *  @note  专门为来硬件测试编写
 */
int BcuPlayAudio(cyg_io_handle_t audio_handle, int audio_source_buffer_id);

/** BcuPlayAudio_analog function description
 *  It belongs to BCU
 *	  Function:play audio data
 *
 *  @param   handle of sound card
 *  @param   audio data buffer id
 *  @return  null
 *  @note  播放模拟音频，口播写入到模拟总线上
 */
int BcuPlayAudio_analog(cyg_io_handle_t audio_handle, int audio_source_buffer_id);

/** PlayAudioTwice function description
 *  It belongs to BCU
 *	  Function:play audio data
 *
 *  @param   handle of sound card
 *  @param   audio data buffer id
 *  @return  null
 *  @note  双缓存播放，播放PCU请求提示音
 */
int PlayAudioTwice(cyg_io_handle_t audio_handle, int audio_source_buffer_id, int pending_audio_buffer_id);

/** PlayAudioTwice_D2P& PlayAudioTwice_D2D function description
 *  It belongs to BCU
 *	  Function:play audio data
 *
 *  @param   handle of sound card
 *  @param   audio data buffer id
 *  @return  null
 *  @note  双缓存播放，播放对讲内容，音频数据不足时，用空白数据抵充
 */
int PlayAudioTwice_D2P(cyg_io_handle_t audio_handle, int audio_source_buffer_id, int pending_audio_buffer_id);
int PlayAudioTwice_D2D(cyg_io_handle_t audio_handle, int audio_source_buffer_id, int pending_audio_buffer_id);
int PlayAudioTwice_analog(cyg_io_handle_t audio_handle, int audio_source_buffer_id, int pending_audio_buffer_id);
/** PutMicSampleDataintoBuffer_EAMP function description
 *  It belongs to BCU
 *	  Function:Audio data sample_EAMP
 *
 *  @param   handle of sound card
 *  @param   audio data buffer id
 *  @return  null
 *  @note  PTT按下之后才采集音频文件
 */
void PutMicSampleDataintoBuffer_EAMP(cyg_io_handle_t audio_handle, int buffer_id);

/** PutMicSampleDataintoBuffer_EAMP_Outer function description
 *  It belongs to BCU
 *	  Function:Audio data sample_EAMP
 *
 *  @param   handle of sound card
 *  @param   audio data buffer id
 *  @return  null
 *  @note  一直采集外接3D5音频文件
 */
void PutMicSampleDataintoBuffer_EAMP_Outer(cyg_io_handle_t audio_handle, int buffer_id);

/** PutMicSampleDataintoBuffer_D2P function description
 *  It belongs to BCU
 *	  Function:Audio data sample_PCU
 *
 *  @param   handle of sound card
 *  @param   audio data buffer id
 *  @return  null
 */
void PutMicSampleDataintoBuffer_D2P(cyg_io_handle_t audio_handle, int buffer_id);


/** PutMicSampleDataintoBuffer_BCU_blank function description
 *  It belongs to BCU
 *	  Function:Audio data sample_BCU
 *
 *  @param   handle of sound card
 *  @param   audio data buffer id
 *  @return  null
 *  @note  目前与PutMicSampleDataintoBuffer_BCU_blank一样，只是在D2D状态下使用
 */
void PutMicSampleDataintoBuffer_BCU(cyg_io_handle_t audio_handle, int buffer_id);

/** PutMicSampleDataintoBuffer_BCU_test_for_hardware function description
 *  It belongs to BCU
 *	  Function:Audio data sample_BCU
 *
 *  @param   handle of sound card
 *  @param   audio data buffer id
 *  @return  null
 *  @note  硬件测试使用函数
 */
void PutMicSampleDataintoBuffer_BCU_test_for_hardware(cyg_io_handle_t audio_handle, int buffer_id);


/** PutMicSampleDataintoBuffer_BCU_blank function description
 *  It belongs to BCU
 *	  Function:Audio data sample_PCU
 *
 *  @param   handle of sound card
 *  @param   audio data buffer id
 *  @return  null
 *  @note  目前与PutMicSampleDataintoBuffer_BCU一样，只是在D2P状态下使用
 */
void PutMicSampleDataintoBuffer_BCU_blank(cyg_io_handle_t audio_handle, int buffer_id);

#ifdef CONFIG_TEST_SND_IN_MULTI_THREAD
void PutMicSampleDataintoBuffer_SndTest(cyg_io_handle_t audio_handle, int buffer_id, int cp_buffer_id);
#endif


/** alarm_func_handle_snd_enable etc function description
 *  声卡死机定时器操作
 *
 *  @param  ect
 *  @return  o
 */
void alarm_func_handle_snd_enable(cyg_handle_t counter_handle, cyg_addrword_t data);
int CreateSndEnableTimer();
int EnableSndEnableTimer();
int DisableSndEnableTimer();

///////////////////////
int CreateLiveMonitorTimer();
int EnableLiveMonitorTimer();
int DisableLiveMonitorTimer();
void ResetLiveMonitorCounts();
unsigned int GetLiveMonitorCounts();
#endif /* AUDIO_SAMPLE_H_ */



