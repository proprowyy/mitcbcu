/*
 * bcu_cmu_handle.h
 *
 *  Created on: 2015-2-11
 *      Author: benty
 */

#ifndef BCU_CMU_HANDLE_H_
#define BCU_CMU_HANDLE_H_

#include "bcu_function.h"

/** BCUCMUCarTypeHandle  function description
 *  连挂车对象
 *
 *  @param   网络接受命令包
 *  @return  null
 */
void BCUCMUCarTypeHandle(common_package_t common_package_info);

/** BCUCMUACKStateHandle  function description
 *  连挂车ACK信号
 *
 *  @param   网络接受命令包
 *  @return  null
 */
void BCUCMUACKStateHandle(common_package_t common_package_info);

/** BCUCMUConnectionCatTypeAndAckState  function description
 *  连挂车类型和ACK信号
 *
 *  @param   null
 *  @return  null
 */
void BCUCMUConnectionCatTypeAndAckState();

#endif /* BCU_CMU_HANDLE_H_ */
