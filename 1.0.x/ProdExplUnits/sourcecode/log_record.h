/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： log_record.h
 * 文件标识：
 * 摘    要：The head file of log record
 *
 * 当前版本： V1.1.0
 * 作    者： wilson
 * 完成日期：2013-01-11
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */

#ifndef LOG_RECORD_H_
#define LOG_RECORD_H_

#include <stdio.h>
#include "data_struct.h"
#include <sys/time.h>



typedef struct
{
	int tv_sec_begin;
	int tv_usec_begin;
	int tv_sec_over;
	int tv_usec_over;
	int len_of_read_audio_data;
}log_time_t;//时间

extern struct timeval tp_log;

/** pexit function description
 *	It belongs to BCU
 *	Function:Initial the log,which is used to Print error message
 *
 *  @param   the string of error message
 *  @return  null
 */
//void pexit(char *s);

/** DisplaySendCmd function description
 *	  Function:show the detail of cmd information
 *
 *  @param   cmd package
 *  @return  if show successfully return 0,else return -1
 */
int DisplaySendCmd(send_infomation_t  param_cmd_info);

/** DisplayNetworkCmd function description
 *	  Function:show the detail of cmd information
 *
 *  @param   cmd package
 *  @return  if show successfully return 0,else return -1
 */
int DisplayNetworkCmd(network_send_package_t  param_cmd_info);

/** WriteNetworkErrorToLog function description
 *	 写网络错误log日志
 *
 *  @param   错误信息
 *  @return  null
 */
void WriteNetworkErrorToLog(char *info);

#endif /* LOG_RECORD_H_ */
