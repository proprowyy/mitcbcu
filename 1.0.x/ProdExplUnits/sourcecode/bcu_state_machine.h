/*
 * Copyright (c) 2013, MIT, All right reserved.
 *
 * File Name:	bcu_state_machine.h
 * File Identification: 
 * Abstract:  
 *
 * Current Version:	V2013_0415_0.0.1
 * Author:	AlvinChang
 * Completion Date:	2013-04-16
 *
 * Last Version:
 * Last Author:
 * Finishing Date:
 */
#ifndef BCU_STATE_MACHINE_H_
#define BCU_STATE_MACHINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cyg/kernel/kapi.h>
#include "data_struct.h"
#include "state_machine.h"

#define EVENT_NAME_LEN  32

//add begin, ach, 2013
typedef struct event_option_table_s
{
     char name[EVENT_NAME_LEN];
     unsigned int unique_id;
} event_option_table_t;

struct state_priority_list_s
{
     unsigned int unique_id;
     unsigned int priority;
     state_process_t *this_state_process;
     struct state_priority_list_s *next;
};
typedef struct state_priority_list_s state_priority_list_t;

typedef struct event_priority_table_s
{
     char event_name[EVENT_NAME_LEN];
     unsigned int event_priority;
} event_priority_table_t;



int BcuTestSelectNextEvent(void);
int BcuDynamicStateSort(event_priority_table_t  *priority_table_from_database, unsigned int table_items);
//state_priority_list_t *BcuAnnStateAddNewNode(event_priority_table_t *new_priority_item );
int BcuAnnStateCreatePriorityNode(event_priority_table_t *db_priority_table );
int BcuAnnStateCreatePriorityList(event_priority_table_t *db_priority_table, unsigned int table_items);
int BcuAnnStateListUpdate(event_priority_table_t *db_priority_table, unsigned int table_items);
int BcuAnnStateNodeUpdate(event_priority_table_t *db_priority_table);
int BcuStateEventIdJudge(int event_id );
int BcuStateParseAnnNewCommand(send_infomation_t *pnew_command);
int BcuStateParseIntercomNewCommand(send_infomation_t *pnew_command);
state_priority_list_t  *BcuAnnStatePriorityListSearch(unsigned int event_id);
extern unsigned int BcuGetAnnStatePriority(unsigned int event_id);
int BcuDynamicStateExchange(state_process_t **ppcurrent_state, send_infomation_t *pnew_command);
int BcuSimulateStateInit(void);


void ShowPriorityListInfo();


//add end, ach, 2013
#endif /* BCU_STATE_MACHINE_H_ */
