/*
 * Copyright (c) 2013, MIT, All right reserved.
 *
 * File Name:	bcu_update_table_from_ccu.h
 * File Identification: 
 * Abstract:  
 *
 * Current Version:	V2013_0415_0.0.1
 * Author:	 H.Product Department
 * Completion Date:	2013-07-12
 *
 * Last Version:
 * Last Author:
 * Finishing Date:
 */
 #ifndef  _BCU_UPDATE_TABLE_FROM_CCU_H_
#define _BCU_UPDATE_TABLE_FROM_CCU_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bcu_function.h"
#include "bcu_get_data_from_ccu_type.h"

#ifdef CONFIG_BCU_SYNC_FROM_CCU_NET
extern bcu_state_t  bcu_state;
extern unsigned char whether_bcu_have_get_part_info_from_ccu ;

unsigned char BCUGetSyncState(void);
unsigned char BCUGetDataFromCCUState(void);
void BCUSetDataFromCCUState(int value);

int BcuUpdateLogPathArray( log_path_table_row_t *plog_path_table_row );

int BcuUpdateDoorSideArray( line_station_door_side_t *pline_station_door_side );

int BcuUpdateRecordPathArray( record_path_table_row_t *record_path_table_row_t );

int BcuUpdateDevNumberArray( device_number_table_row_t *pdev_number_table_row );

int BcuUpdateConfigVersionArray( config_db_version_table_row_t *pconfig_db_version_table_row );

int BcuUpdateAnnVersionArray( ann_db_version_table_row_t *pann_db_version_table_row );

int BcuUpdateNetIpArray( ip_mac_table_row_t *pip_mac_table_row );

int BcuUpdateLcdEmergAnnArray( emerg_ann_table_row_t *pemerg_ann_table_row );

int BcuUpdateLcdDefaultLineArray( default_line_table_row_t  *pdefault_line_table_row );

int BcuUpdateLcdLineStationArray( line_station_code_row_t  *pcurrent_station_row );

int BcuUpdateLcdLineRegionArray(line_section_row_t *pcurrent_section_row);

int BcuUpdatePriorArray( priority_table_row_t *pprior_table_row );
int BcuUpdateBcuVolArray( pu_volume_table_row_t *pbcu_vol_table_row );
#endif

#endif /* _BCU_UPDATE_TABLE_FROM_CCU_H_ */
