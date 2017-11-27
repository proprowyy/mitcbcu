/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称：sound_driver.h
 * 文件标识：
 * 摘    要：The header of sound drivers
 *
 * 当前版本：V1.0.0
 * 作    者：benty
 * 完成日期：
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 * */


#ifndef SOUND_DRIVER_H_
#define SOUND_DRIVER_H_

#include <stdio.h>
#include <string.h>
#include <pkgconf/hal.h>
#include <pkgconf/io_audio.h>
#include CYGHWR_MEMORY_LAYOUT_H
#include <cyg/io/io.h>
#include <cyg/infra/diag.h>
#include <cyg/infra/testcase.h>
#include <errno.h>
#include <cyg/io/audio/audio.h>
#include <cyg/io/audio/audio_dev.h>
#include <cyg/hal/hal_if.h>
#include <pkgconf/devs_snd_vs1063a.h>
#include "buffer_read_write_fun.h"
#include "log_record.h"
#include "audio_sample.h"

extern int current_sounder_mode;
extern const unsigned char RIFFHeader0[52];
extern unsigned char current_dia;

#define BCU_SND_MODE_PLAYBACK 0
#define BCU_SND_MODE_CODEC 1

/** MicAdPcmWavheader function description
 *  Add sound header
 *
 *  @param   buffer id
 *  @return  if success,return 0
 */
int MicAdPcmWavheader(int buffer_id);

/** MicZeroAfterWaveHeader function description
 *  Add zero to sound
 *
 *  @param   buffer id
 *  @return  if success,return 0
 */
int MicZeroAfterWaveHeader(int buffer_id);

/** LookupSndCard function description
 *  This function is used to lookup the sound card
 *
 *  @param   the point of sound card handle
 *  @return  the state of lookup sound card
 */
int LookupSndCard(cyg_io_handle_t *param_audio_handle) ;

/** OpenSndCard function description
 *  This function is used to open the sound card
 *
 *  @param   the point of sound card handle
 *  @param   the mode of open sound card
 *  @return  the state of open sound card
 */
int OpenSndCard(cyg_io_handle_t *param_audio_handle,char *snd_mode) ;

/** CloseSndCard function description
 *  This function is used to close the sound card
 *
 *  @param   the handle of sound card
 *  @param   the mode of sound card

 *  @param   the type of close sound card
 *  @return  the state of close sound card
 */
int CloseSndCard(cyg_io_handle_t *param_audio_handle,char *snd_mode, char *close_type);
int CloseSndCard_BCU(cyg_io_handle_t *param_audio_handle,char *snd_mode, char *close_type);

/** AdjustBCUSNDVolume function description
 *	调节BCU音量具体实现函数
 *
 *  @param   当前音量
 *  @return  null
 */
void AdjustBCUSNDVolume(unsigned char current_volume);
void bcu_6d5w_ctrl_wilson(unsigned char dia);
#endif /* SOUND_DRIVER_H_ */

