/*
 * Copyright (c) 2013, MIT, All right reserved.
 *
 * File Name:	bcu_state_machine.c
 * File Identification: 
 * Abstract:  
 *
 * Current Version:	V2013_0306_0.0.1
 * Author:	AlvinChang
 * Completion Date:	2013-03-06
 *
 * Last Version:
 * Last Author:
 * Finishing Date:
 */ 
#include "bcu_state_machine.h"
#include "bcu_function.h"

//extern state_process_t *bcu_all_states[];
//extern unsigned int bcu_all_states_items;

extern state_process_t auto_annoucement_state;
extern state_process_t idle_ann_state;
extern state_process_t live_annoucement_state;
extern state_process_t occ_state;
extern state_process_t mic_outer_3d5_ann_state;
extern state_process_t emergency_annoucement_state;
extern state_process_t manual_annoucement_state;
extern state_process_t idle_intercom_state;
extern state_process_t d2d_intercomm_state;
extern state_process_t d2p_intercomm_state;
extern state_process_t intercomm_hang_up_state;

//add begin, ach, 2013
state_process_t *bcu_all_states[] = 
{
    &auto_annoucement_state,
    &idle_ann_state,
    &live_annoucement_state,
    &occ_state,
    &mic_outer_3d5_ann_state,
    &emergency_annoucement_state,
    &manual_annoucement_state,
    &idle_intercom_state,
    &d2d_intercomm_state,
    &d2p_intercomm_state,
    &intercomm_hang_up_state
};
unsigned int bcu_all_states_items = (sizeof(bcu_all_states))/(sizeof(bcu_all_states[0]));
//add end, ach, 2013

//now, begin to define params
#ifdef WITH_DB
event_option_table_t  bcu_ann_option_table[] = 
{
      	{"OCC", OCC_EVENT},
      	{"Outer3D5",MIC_3D5_OUTER_EVENT},
      	{"LIVE", LIVE_ANN_EVENT},
      	{"EMERG", EMERG_ANN_EVENT},
      	{"MANUAL", MANUAL_ANN_EVENT},
      	{"TMS-MANUAL", TMS_MANUAL_ANN_EVENT},
      	{"TMS-AUTO", TMS_AUTO_ANN_EVENT},
      	{"ANN-IDLE",ANN_IDLE},
      	{"D2D", D2D_INTERCOMM_EVENT},
      	{"D2P", D2P_INTERCOMM_EVENT},
      	{"D2DXD2P", D2D_HANGUP_D2P_EVENT},
      	{"D2PPEND", D2P_INTERCOM_PENDING},
      	{"INTERCOM-IDLE",INTERCOM_IDLE},
      	{"LINENO", LINE_NUMBER_CHANGE_EVENT}
};
#else
event_option_table_t  bcu_ann_option_table[] =
{
      	{"OCC", OCC_EVENT},
      	{"MIC_3D5",MIC_3D5_OUTER_EVENT},
      	{"LIVE", LIVE_ANN_EVENT},
      	{"EMERG", EMERG_ANN_EVENT},
      	{"MANUAL", MANUAL_ANN_EVENT},
      	{"TMS-MANUAL", TMS_MANUAL_ANN_EVENT},
      	{"TMS-AUTO", TMS_AUTO_ANN_EVENT},
      	{"ANN-IDLE",ANN_IDLE},
      	{"D2D", D2D_INTERCOMM_EVENT},
      	{"D2P", D2P_INTERCOMM_EVENT},
      	{"D2DXD2P", D2D_HANGUP_D2P_EVENT},
      	{"D2PPEND", D2P_INTERCOM_PENDING},
      	{"INTERCOM-IDLE",INTERCOM_IDLE},
      	{"LINENO", LINE_NUMBER_CHANGE_EVENT}
};
#endif
#define BCU_ANN_OPTION_ITEMS ((sizeof(bcu_ann_option_table))/(sizeof(bcu_ann_option_table[0])))

static state_priority_list_t  bcu_ann_priority_list_node_idle = 
{
      ANN_IDLE,
       0xFFFFFFFF, //this is always the lowest priority for idle state
       &idle_ann_state,
       NULL  //when power up, there is only idle node in the list
};

event_priority_table_t bcu_ann_priority_default_table[] =
{
      	{"OCC", 1},
      	{"MIC_3D5",2},
      	{"EMERG", 4},
      	{"LIVE", 3},
      	{"TMS-AUTO", 6},
      	{"MANUAL", 5},
      	{"TMS-MANUAL", 7}
};
#define BCU_ANN_PRIORITY_DEFAULT_TABLE_ITEMS  ((sizeof(bcu_ann_priority_default_table))/(sizeof(bcu_ann_priority_default_table[0])))

//simulation for new priority table
event_priority_table_t bcu_ann_priority_new_table[] = 
{
      	{"EMERG", 1},
      	{"TMS-AUTO", 2},
      	{"LIVE", 3},      	
      	{"OCC", 4},
      	{"MANUAL", 5},
      	{"TMS-MANUAL", 6}
};
#define BCU_ANN_PRIORITY_NEW_TABLE_ITEMS  ((sizeof(bcu_ann_priority_new_table))/(sizeof(bcu_ann_priority_new_table[0])))


int BcuTestSelectNextEvent(void)
{
       static unsigned int count_next_event = 0;
       unsigned int ret = 0;

        if( count_next_event >= BCU_ANN_OPTION_ITEMS )
        {
             count_next_event = 0; //reset
//             debug_print(("BcuTestSelectNextEvent: end\n"));
             return -1;
        }
        ret = bcu_ann_option_table[count_next_event++].unique_id;		
		
       return (int)ret;
}

int BcuDynamicStateSort(event_priority_table_t  *priority_table_from_database, unsigned int table_items)
{
      //int ret = -1;
      int i = 0, j = 0;
      event_priority_table_t  tmp_table_item = {"", 0};
	  
      if( NULL==priority_table_from_database )
      {
    	  debug_print(("BcuDynamicStateSort"));
             return -1;
      }

      #if 1 //debug
      {
	      for(j=0; j<table_items; j++)
	      {
	    	  debug_print(("before sort:  %s-%d\n", priority_table_from_database[j].event_name,  priority_table_from_database[j].event_priority));
	      }
      }
      #endif	  
	  
      for(j=0; j<table_items-1; j++)   // loot time is table_items-1
      {
              for(i=0; i<table_items-j-1; i++)
              {
                      if( priority_table_from_database[i].event_priority > priority_table_from_database[i+1].event_priority )
                      { //priority value is larger, the priority is smaller
                            tmp_table_item = priority_table_from_database[i];
                            priority_table_from_database[i] = priority_table_from_database[i+1];
                            priority_table_from_database[i+1] = tmp_table_item;
                      }
             }
      }

      #if 1 //debug
      {
	      for(j=0; j<table_items; j++)
	      {
	    	  debug_print(("after sort:  %s, %d\n", priority_table_from_database[j].event_name,  priority_table_from_database[j].event_priority));
	      }
      }
      #endif

       return 0;
}
static state_priority_list_t *BcuAnnStateAddNewNode(event_priority_table_t *new_priority_item )
{
        state_priority_list_t *tmp_node = NULL;
         int min_len = 0, s1_len = 0, s2_len = 0;
         int j = 0;

         if( NULL == new_priority_item )
        {
            debug_print(("BcuAnnStateAddNewNode:  null ptr\n"));
            return NULL;
        }		 	
		
        tmp_node = (state_priority_list_t *)malloc( sizeof(state_priority_list_t));
        if( NULL==tmp_node )
        {
            debug_print(("BcuAnnStateAddNewNode: malloc fail\n"));
            return NULL;
        }
        memset( (void *)tmp_node, 0x00, sizeof(*tmp_node) );
        
        //No1.			
        s2_len = strlen( new_priority_item->event_name);
        for(j=0; j<BCU_ANN_OPTION_ITEMS; j++)
        {
              min_len = 0;
              s1_len = 0;					
              s1_len = strlen(bcu_ann_option_table[j].name);
              min_len = (s1_len>=s2_len) ? (s2_len) : (s1_len);
              if( 0==strncmp(bcu_ann_option_table[j].name, new_priority_item->event_name,  min_len) )
              { 
                    //name matched
                    tmp_node->unique_id = bcu_ann_option_table[j].unique_id;
                    break;
              }
        }
		
        if( j==BCU_ANN_OPTION_ITEMS ) //not found in option table, so id is 0xFFFF-FFFF
        {
             tmp_node->unique_id = 0xFFFFFFFF;
             debug_print(("BcuAnnStateAddNewNode:  event id not found\n"));
        }
        
        //No2.
        tmp_node->priority = new_priority_item->event_priority;
        
        //No3.
        for(j=0; j<bcu_all_states_items; j++)
        {
              if(  tmp_node->unique_id == bcu_all_states[j]->state_id )
              { 
                  //id matched 
                  tmp_node->this_state_process = bcu_all_states[j];
                  break;
              }                           
        }
		
        if( j==bcu_all_states_items )
        {
             tmp_node->this_state_process = NULL;
             debug_print(("BcuAnnStateAddNewNode:  this_state_process not found\n"));
        }

        return tmp_node;
}
int BcuAnnStateCreatePriorityNode(event_priority_table_t *db_priority_table )
{
		state_priority_list_t *tmp_node = NULL;

		state_priority_list_t  *current_node = (state_priority_list_t  *)&bcu_ann_priority_list_node_idle;

		if( NULL==db_priority_table )
		{
			return -1;
		}

		//find the last node
		while( current_node->next != NULL )
		{
			debug_print(("BcuAnnStateCreatePriorityNode:  %d,%d\n", (int)current_node->unique_id, (int)current_node->priority));
			debug_print(("BcuAnnStateCreatePriorityNode:  %d,%d\n", (int)current_node->this_state_process, (int)current_node->next));
			current_node = current_node->next;
		}

		//create new node as new last node
		tmp_node = BcuAnnStateAddNewNode( db_priority_table );
		if( NULL == tmp_node )
		{
			debug_print(("BcuAnnStateCreatePriorityNode: new node fail\n"));
			return -1;
		}

		current_node->next = tmp_node;
		//No4.
		current_node = tmp_node; //current node updated
		current_node->next = NULL; //new end node

		return 0;
}
int BcuAnnStateCreatePriorityList(event_priority_table_t *db_priority_table, unsigned int table_items)
{
	int i = 0, j = 0;
	state_priority_list_t *tmp_node = NULL;
	int min_len = 0, s1_len = 0, s2_len = 0;
	state_priority_list_t  *current_node = (state_priority_list_t  *)&bcu_ann_priority_list_node_idle;

	if( NULL==db_priority_table )
	{
		   return -1;
	}

	//BcuDynamicStateSort(db_priority_table, table_items);

	for(i=table_items; i>0; i--)
	{
		tmp_node = (state_priority_list_t *)malloc( sizeof(state_priority_list_t) );
		if( NULL==tmp_node )
		{
			debug_print(("BcuAnnStateCreatePriorityList: malloc fail\n"));
			return -1;
		}
		memset( (void *)tmp_node, 0x00, sizeof(*tmp_node) );
  
		//No1.
		s2_len = strlen(db_priority_table[i-1].event_name);
		for(j=0; j<BCU_ANN_OPTION_ITEMS; j++)
		{
			min_len = 0;
			s1_len = 0;
			s1_len = strlen(bcu_ann_option_table[j].name);
			min_len = (s1_len>=s2_len) ? (s2_len) : (s1_len);
			if( 0==strncmp(bcu_ann_option_table[j].name, db_priority_table[i-1].event_name,  min_len) )
			{
				//name matched
				tmp_node->unique_id = bcu_ann_option_table[j].unique_id;
				break;
			}
		}
		if( j==BCU_ANN_OPTION_ITEMS ) //not found in option table, so id is 0xFFFF-FFFF
		{
			tmp_node->unique_id = 0xFFFFFFFF;
			debug_print(("BcuAnnStateCreatePriorityList:  event id not found\n"));
		}
  
	  	    //No2.
		tmp_node->priority = db_priority_table[i-1].event_priority;
	  
	                  //No3.
		for(j=0; j<bcu_all_states_items; j++)
		{
			if(  tmp_node->unique_id == bcu_all_states[j]->state_id )
			{
				//id matched
				tmp_node->this_state_process = bcu_all_states[j];
				break;
			}
		}
		if( j==bcu_all_states_items )
		{
			tmp_node->this_state_process = NULL;
			debug_print(("BcuAnnStateCreatePriorityList:  this_state_process not found\n"));
		}
	           
		current_node->next = tmp_node;
		//No4.
		current_node = tmp_node; //current node updated
		tmp_node = NULL; //for next loop
	}

	  #if 1
		{
			state_priority_list_t  *curr_node = (state_priority_list_t  *)&bcu_ann_priority_list_node_idle;
			while( curr_node != NULL )
			{
				debug_print(("new list: %d,%d, 0x%08x <==> ", curr_node->unique_id, curr_node->priority, (unsigned int)(curr_node->this_state_process)));
				for(j=0; j<bcu_all_states_items; j++)
				{
					if(  curr_node->unique_id == bcu_all_states[j]->state_id )
					{
						//id matched
						debug_print((" bcu state=0x%08x\n", (unsigned int)(bcu_all_states[j])));
						break;
					}
				}
				if( j==bcu_all_states_items )
				{
					debug_print((" bcu state=not\n"));
				}
				curr_node = curr_node->next; //pointer to next which priority is larger
			}
		}
	  #endif

	  return 0;
}
int BcuAnnStateListUpdate(event_priority_table_t *db_priority_table, unsigned int table_items)
{
	int ret = -1;
	  
	if( NULL==db_priority_table )
	{
		debug_print(("BcuAnnStateListUpdate: use default priority,total=%d\n", BCU_ANN_PRIORITY_DEFAULT_TABLE_ITEMS));
		if ( 0 == BcuAnnStateCreatePriorityList(bcu_ann_priority_default_table, BCU_ANN_PRIORITY_DEFAULT_TABLE_ITEMS) )
		{
			debug_print(("BcuAnnStateListUpdate: default list ok\n"));
			ret = 0;
		}
	}
	else
	{
		debug_print(("BcuAnnStateListUpdate: use new priority,total=%d\n", table_items));
		if ( 0 == BcuAnnStateCreatePriorityList(db_priority_table, table_items) )
		{
			debug_print(("BcuAnnStateListUpdate: new list ok\n"));
			ret = 0;
		}
	}
  return ret;
}
int BcuAnnStateNodeUpdate(event_priority_table_t *db_priority_table)
{
	int ret = -1;

	if( NULL==db_priority_table )
	{
		debug_print(("BcuAnnStateNodeUpdate: add new priority node\n"));
		if ( 0 == BcuAnnStateCreatePriorityNode(db_priority_table) )
		{
			debug_print(("BcuAnnStateNodeUpdate: new list ok\n"));
			ret = 0;
		}
	}

	return ret;
}
int BcuStateEventIdJudge(int event_id )
{
    int ret = -1;

	switch( event_id )
	{
		 case OCC_EVENT:
		 case LIVE_ANN_EVENT:
		 case MIC_3D5_OUTER_EVENT:
		 case EMERG_ANN_EVENT:
		 case MANUAL_ANN_EVENT:
		 case TMS_MANUAL_ANN_EVENT:
		 case TMS_AUTO_ANN_EVENT:
		 case ANN_IDLE:
		 	  ret = 0;
			  break;
			  
		 case D2D_INTERCOMM_EVENT:
		 case D2P_INTERCOMM_EVENT:
		 case D2D_HANGUP_D2P_EVENT:
		 case D2P_INTERCOM_PENDING:
		 case INTERCOM_IDLE:
		 	ret = 1;
			break;
			
		 case LINE_NUMBER_CHANGE_EVENT:
		 	ret =2;
		        break;

//		case ANN_IDLE:
//		case INTERCOM_IDLE:
//			ret = -1;
//			break;

		default:
			ret = -1;
			break;               
	}

	return ret;
}
int BcuStateParseAnnNewCommand(send_infomation_t *pnew_command)
{
	if ( NULL== pnew_command )
	{
		debug_print(("BcuStateParseAnnNewCommand:  null pointer\n"));
		return -1;
	}

	//switch( pnew_command->event_type_ann )
	{

	}

	return 0;
}
int BcuStateParseIntercomNewCommand(send_infomation_t *pnew_command)
{
	if ( NULL==pnew_command )
	{
		debug_print(("BcuStateParseIntercomNewCommand:  null pointer\n"));
		return -1;
	}

	//switch()
	{

	}

	return 0;
}
state_priority_list_t  *BcuAnnStatePriorityListSearch(unsigned int event_id)
{
	state_priority_list_t *ret_ptr = NULL;
	state_priority_list_t  *current_node = (state_priority_list_t  *)&bcu_ann_priority_list_node_idle; //now, currrent node is idle

	while( current_node != NULL )
	{
//		debug_print(("BcuAnnStatePriorityListSearch: %d,%d, %d\n", current_node->unique_id, current_node->priority, event_id));
		if( event_id == current_node->unique_id )
		{
			ret_ptr = current_node;
			break;
		}
		current_node = current_node->next; //pointer to next which priority is larger
	}
	return ret_ptr;
}

unsigned int BcuGetAnnStatePriority(unsigned int event_id)///<add by wilson 2013-06-28
{
	unsigned int ret = 0;
	state_priority_list_t *temp_state_priority_list;
	temp_state_priority_list = BcuAnnStatePriorityListSearch(event_id);
	ret = temp_state_priority_list->priority;
	return ret;
}
int WhetherOverPackage(send_infomation_t *pnew_command)
{
	int ret = 0;
	int event_type = (int)pnew_command->event_type_ann;
	if(pnew_command->event_type_intercom == INTERCOM_IDLE)
	{
		switch(event_type)
		{
		case OCC_EVENT:
				ret = (pnew_command->event_info_ann.occ_announce.occ_active == 0)?1:0;
				break;
		case MIC_3D5_OUTER_EVENT:
				ret = (pnew_command->event_info_ann.outer_3d5_ann.outer_3d5_active == 0)?1:0;
				break;
			case LIVE_ANN_EVENT:
				ret = (pnew_command->event_info_ann.live_announce.live_active == 0)?1:0;
				break;
			case EMERG_ANN_EVENT:
				ret = (pnew_command->event_info_ann.emerg_announce.emerg_active == 0)?1:0;
				break;
			case MANUAL_ANN_EVENT:
				ret = (pnew_command->event_info_ann.manual_annnounce.manual_active == 0)?1:0;
				break;
			case TMS_MANUAL_ANN_EVENT:
				ret = (pnew_command->event_info_ann.tms_manual_announce.tms_manual_active == 0)?1:0;
				break;
			case TMS_AUTO_ANN_EVENT:
				ret = (pnew_command->event_info_ann.tms_auto_announce.tms_auto_active == 0)?1:0;
				break;
			default:
				ret = 0;
				break;
		}
	}
	return ret;
}

int BcuDynamicStateExchange(state_process_t **ppcurrent_state, send_infomation_t *pnew_command)
{
	state_priority_list_t *new_state_node = NULL;
	state_priority_list_t *current_state_node = NULL;

	if ( NULL==ppcurrent_state || NULL==(*ppcurrent_state) || NULL==pnew_command )
	{
		debug_print(("BcuDynamicStateExchange:  null pointer\n"));
		return -1;
	}
	
	debug_print(("BcuDynamicStateExchange:  input state_id = %d\n", (*ppcurrent_state)->state_id));
        
	if( pnew_command->event_type_intercom==INTERCOM_IDLE && pnew_command->event_type_ann != ANN_IDLE )
	{
		if( 0==BcuStateEventIdJudge((*ppcurrent_state)->state_id)  ) //current state is in one ann.
		{

			debug_print(("QQQQ:%d\n",pnew_command->event_type_ann));
			new_state_node = BcuAnnStatePriorityListSearch(pnew_command->event_type_ann);
			debug_print(("A:unique_id = %d,priority=%d\n",new_state_node->unique_id,new_state_node->priority));

			current_state_node = BcuAnnStatePriorityListSearch( (*ppcurrent_state)->state_id );

			debug_print(("c:%d  n:%d\n",current_state_node->priority,new_state_node->priority));
			debug_print(("cid:%d  nid:%d\n",current_state_node->this_state_process->state_id,new_state_node->this_state_process->state_id));

			if( NULL != new_state_node && NULL != current_state_node  )
			{
				if( new_state_node->priority <= current_state_node->priority ) //here: priority value little, but the priority large
				{
					if(WhetherOverPackage(pnew_command) ) ///< back to idle only
					{
						if(new_state_node->unique_id == current_state_node->unique_id)
						{
						   new_state_node = (state_priority_list_t  *)&bcu_ann_priority_list_node_idle;
						}
					}
					if( NULL != new_state_node->this_state_process )
					{
						debug_print(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"));
						debug_print(("BcuDynamicStateExchange:  old,state-id=%d\n", (*ppcurrent_state)->state_id));
						(*ppcurrent_state)->exit();
						diag_printf("!!!-   %d\n",new_state_node->this_state_process->state_id);
						diag_printf("!!!-   %d\n",new_state_node->this_state_process);
						*ppcurrent_state = new_state_node->this_state_process;
						(*ppcurrent_state)->enter(pnew_command);
						debug_print(("BcuDynamicStateExchange:  new,state-id=%d\n", (*ppcurrent_state)->state_id));
					}
					else
					{
						debug_print(("BcuDynamicStateExchange:  try to new state but null pointer\n"));
					}
				 }
				else
				{
					debug_print(("BcuDynamicStateExchange:  check priority\n"));
				}
			}
			else
			{
				debug_print(("BcuDynamicStateExchange:  find state but null pointer\n"));
			}
	      }
        }
	else if ( pnew_command->event_type_ann == ANN_IDLE && pnew_command->event_type_intercom != INTERCOM_IDLE )
	{
		debug_print(("BcuDynamicStateExchange:  please call StateMachineExchange() for intercom\n"));
	}
	else
	{
		debug_print(("BcuDynamicStateExchange: unsupported when ann and intercomm\n"));
		return -1;
	}
	
	return 0;
}
int BcuSimulateStateInit(void)
{
       int ret = -1;
	   
       ret = BcuAnnStateListUpdate(NULL, 0);

       return ret;
}

void ShowPriorityListInfo()
{
	state_priority_list_t  *p_current_node = (state_priority_list_t  *)&bcu_ann_priority_list_node_idle;
	int i = 0;
	char temp[32];
	 while(p_current_node != NULL)
	 {
		 i = 0;
		 for(i = 0; i < BCU_ANN_OPTION_ITEMS;i++)
		 {
			 debug_print(("bcu_ann_option_table:%d,%d\n",bcu_ann_option_table[i].unique_id,p_current_node->unique_id));
			 if(bcu_ann_option_table[i].unique_id == p_current_node->unique_id)
			 {
				 strcpy(temp,bcu_ann_option_table[i].name);
				 break;
			 }
		 }
		 debug_print(("Event:%s,prio:%d\n",temp,p_current_node->priority));
		 p_current_node = p_current_node->next;
	 }
}

