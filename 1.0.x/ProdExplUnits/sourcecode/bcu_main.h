/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： bcu_main.h
 * 文件标识：
 * 摘    要： 广播控制盒主函数
 *
 * 当前版本： V1.0.0
 * 作    者： wilson
 * 完成日期：2012-09-14
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */

#ifndef BCU_MAIN_H_
#define BCU_MAIN_H_

#ifndef CMDTOCMU
#define CMDTOCMU
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <cyg/io/io.h>
#include <cyg/io/serialio.h>
#include <cyg/infra/diag.h>
#include <cyg/infra/testcase.h>

#include <cyg/kernel/kapi.h>

#include <cyg/hal/hal_io.h>
#include <cyg/hal/plf_io.h>


#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/drv_api.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/cyg_ass.h>
#include <cyg/io/spi.h>

#include "thread_config.h"
#include "data_struct.h"
#include "bcu_function.h"
#include "buffer_read_write_fun.h"
//
#include "state_machine.h"
#include "sound_driver.h"
#include "bcu_config.h"
#include "audio_sample.h"
#include "alarm_audio_data.h"
#include "bcu_gd_sync.h"
//#include "touch_screen/touch_screen_function.h"
#ifdef CMDTOCMU
#include "./network/zhw_bcu_to_cmu.h"
#include "bcu_cmu_handle.h"
#include "bcu_failure_handle.h"
#endif
#include "./network/zhw_shell_function.h"

/*---------------------------begin of bcu beta 1.1------------------------------*/

/*The declaration of thread stack*/
//char thread_stack_of_screen[PTHREAD_STACK_MIN*128];
//char thread_stack_of_control[PTHREAD_STACK_MIN*16];
char *thread_stack_of_control = NULL;
char *thread_stack_of_screen = NULL;
char *thread_stack_of_network = NULL;
char *thread_stack_of_sample_amd_play = NULL;
char *thread_stack_of_failure_statics = NULL;
char *thread_stack_of_gd_sync = NULL;
char *thread_stack_of_dameo = NULL;
char *thread_stack_of_tftp_queue = NULL;
//char thread_stack_of_sample_amd_play[PTHREAD_STACK_MIN*16];
//char thread_stack_of_failure_statics[PTHREAD_STACK_MIN*4];
//char thread_stack_of_gd_sync[PTHREAD_STACK_MIN*4];
//char thread_stack_of_screen[PTHREAD_STACK_MIN*16];
//char thread_stack_of_screen[PTHREAD_STACK_MIN * 16];
//char thread_stack_of_network[PTHREAD_STACK_MIN];
////char thread_stack_of_control[PTHREAD_STACK_MIN];
//char thread_stack_of_sample_amd_play[PTHREAD_STACK_MIN];
//char thread_stack_of_failure_statics[PTHREAD_STACK_MIN];
//char thread_stack_of_gd_sync[PTHREAD_STACK_MIN];


/*the declaration of thread handle*/
pthread_t thread_of_screen;/*the handle of touch sreen thread*/
pthread_t thread_of_network;/*the handle of network thread*/
pthread_t thread_of_control;/*the handle of control thread*/
pthread_t thread_of_sample_and_play;/*the handle of mic sample and audio play thread*/
pthread_t thread_of_failure_statics;/*the handle of failure statics thread*/
pthread_t thread_of_gd_sync;/*the handle of global device sync thread*/
pthread_t thread_of_demao;
pthread_t thread_of_tftp_queue;/*the handle of tftp thread*/


#ifdef CMDTOCMU
char *thread_stack_of_bcu_to_cmu = NULL;
pthread_t thread_of_bcu_to_cmu; //add by zhouwei 2014-4-17

/** SendCmdToCmuEntry function description
 *  CMU通信线程
 *
 *  @param   void  *arg
 *  @return  null
 */
void *SendCmdToCmuEntry(void *arg);
#endif


/** InitAllDeviceInfo function description
 *  This function belongs to bcu,which is used to initial all device information
 *
 *  @param   null
 *  @return  null
 */
void InitAllDeviceInfo();

/** TouchScreenEntry  function description
 *  The entry of touch screen thread
 *
 *  @param   void *arg
 *  @return  null
 */
void *TouchScreenEntry(void *arg);
void *DemaoEntry(void *arg);

/** NetworkHandleEntry  function description
 *  The entry of network thread
 *
 *  @param   void *arg
 *  @return  null
 */
void *NetworkHandleEntry(void *arg);

/** SystemControl  function description
 *  The entry of control thread
 *
 *  @param   void *arg
 *  @return  null
 */
void *SystemControl(void *arg);

/** BcuMicSampleAndPlayEntry function description
 *  The entry of sample and play thread
 *
 *  @param   void *arg
 *  @return  null
 */
void *BcuMicSampleAndPlayEntry(void *arg);

/** BcuMFailureStaticsEntry function description
 *  The entry of failure  statics thread
 *
 *  @param   void *arg
 *  @return  null
 */
void *BcuFailureStaticsEntry(void *arg);

void *BCUTftpThreadEntry(void *arg);

/*temp function*/

#ifdef CONFIG_TEST_SND_IN_MULTI_THREAD
void CcuAlarmTestSndPuTrigger(void);
#endif

extern int MicAdPcmWavheader_pcm(int buffer_id);
extern void AlarmAudioDataIntoBuffer(int buffer_id);
extern int BcuReadDeviceVersionTableItemInit(void); ///< add, 1010

/*---------------------------end of bcu beta 1.1-------------------------------*/
#endif /* BCU_MAIN_H_ */
