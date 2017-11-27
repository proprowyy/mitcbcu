/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： bcu_extern_button.h
 * 文件标识：
 * 摘    要： BCU extern button control function
 *
 * 当前版本： V1.0.0
 * 作    者： wilson
 * 完成日期：2013-12-30
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */


#ifndef BCU_EXTERN_BUTTON_H_
#define BCU_EXTERN_BUTTON_H_

#include <pkgconf/hal.h>
#include <cyg/io/io.h>
#include <cyg/infra/diag.h>
#include <cyg/infra/testcase.h>
#include <cyg/hal/hal_io.h>
#include <cyg/kernel/kapi.h>
#include <cyg/hal/plf_io.h>

extern unsigned char ppt_state_flag;
extern unsigned char pa_state_flag;
extern unsigned char pc_state_flag;
extern unsigned char cc_state_flag;
extern unsigned char out_3d5_state_flag;

/** GetLiveExternButtonState function description
 *  Get the live extern button state
 *
 *  @param   null
 *  @return  if this button is pressed return 1,otherwise return 0
 */
int GetLiveExternButtonState();

/** GetD2PExternButtonState function description
 *  Get the live extern button state
 *
 *  @param   null
 *  @return  if this button is pressed return 1,otherwise return 0
 */
int GetD2PExternButtonState();

/** GetD2DExternButtonState function description
 *  Get the live extern button state
 *
 *  @param   null
 *  @return  if this button is pressed return 1,otherwise return 0
 */
int GetD2DExternButtonState();

int GetMRExternButtonState();


/** GetPAAllOuterButtonState function description
 *   获取广播五个外部按钮状态值，供消抖使用
 *
 *  @param   null
 *  @return  null
 */
int GetPTTState();
void GetPAAllOuterButtonState();
void GetPAAllOuterButtonState2();
extern int read_cc_key();
#endif /* BCU_EXTERN_BUTTON_H_ */
