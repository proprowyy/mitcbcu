/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： alarm_audio_data.h
 * 文件标识：
 * 摘    要： 紧急对讲提示音
 *
 * 当前版本： V1.0.0
 * 作    者： wilson
 * 完成日期：2013-11-15
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */
#ifndef ALARM_AUDIO_DATA_H_
#define ALARM_AUDIO_DATA_H_

#include <stdio.h>
#include <string.h>
#include "buffer_read_write_fun.h"

extern unsigned char mic_blank_audio_data[3 * 1024];

/** CalculateLength function description
 *  计算PCU请求时提示音长度
 *
 *  @param   null
 *  @return  PCU请求提示音长度
 */
int CalculateLength();

/** ReadAlarmAudioDataToBuffer function description
 *  将PCU请求提示音读取到目标缓存区中
 *
 *  @param   目标缓存区ID
 *  @return  null
 */
void ReadAlarmAudioDataToBuffer(int buffer_id);


/** MicAdPcmWavheader_pcm function description
 *  将PCM头写入到缓存区中测试声卡使用，正式程序没有使用这个函数
 *
 *  @param   目标缓存区ID
 *  @return  写入缓存区中的长度
 */
int MicAdPcmWavheader_pcm(int buffer_id);

#endif /* ALARM_AUDIO_DATA_H_ */
