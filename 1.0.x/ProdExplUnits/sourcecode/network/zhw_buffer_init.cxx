/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： zhw_buffer_init.c
 * 文件标识：
 * 摘    要： 网络主函数
 *
 * 当前版本： V1.0.1
 * 作    者： zhouwei
 * 完成日期：
 *
 * 取代版本：v1.0.0
 * 原作者  ：zhouwei
 * 完成日期：2012-10-08
 *
 */

#include "zhw_buffer_init.h"

static unsigned char local_mutex_init_finish = 0; ///< add, 2013
extern pthread_mutex_t zhw_buffer_register_lock;

buffer_info_t buffer_info_head =
{
		0,
		0,
		0,
		0,
		NULL,
};

buffer_info_t *buffer_info_point ;
buffer_info_t *buffer_info_prt = &buffer_info_head;

//buffer_info_prt = &buffer_info_head;


/*
 *   	@brief	本函数的功能是:将应用层创建的buffer，进行包类型和buffer号绑定，用于网络层向上传递数据时查询；
 *  	@param pkg_type 应用层的包类型
 *     @param buffer_type 创建的buffer的类型（1表示接收buffer，2表示发送buffer）
 *     @param buffer_id 应用层创建的buffer号
 *     @return 1:表示绑定成功，-1：表示buffer_id已经存在，-2：表示绑定失败。
 */
int ZhwBufferRegister_zhw(int common_type,unsigned int pkg_type,unsigned int buffer_type, int buffer_id)
{
	buffer_info_point = &buffer_info_head;

	while(buffer_info_point != NULL)
	{
		if(buffer_info_point->buffer_id == buffer_id)
		{
//			net_debug_printf(("run out ZhwBufferInit() function the new id already in the list\n"));
			return -1; //buffer_id 已经存在
		}
		buffer_info_point = buffer_info_point->next;
	}

	buffer_info_prt->next = (buffer_info_t *)malloc(sizeof(buffer_info_t));
	buffer_info_prt = buffer_info_prt->next ;
	if(buffer_info_prt != NULL)
	{
		buffer_info_prt->common_type = common_type;
		buffer_info_prt->pkg_type = pkg_type;
		buffer_info_prt->buffer_type = buffer_type;
		buffer_info_prt->buffer_id = buffer_id;
		buffer_info_prt->next = NULL;
		return 1;
	}

//	net_debug_printf(("run out ZhwBufferInit() function the new id add fail\n"));
   return -2;

}

int ZhwBufferRegister(int common_type,unsigned int pkg_type,unsigned int buffer_type, int buffer_id)
{
       buffer_info_t *tmp_buffer_info_point = &buffer_info_head;	   
       buffer_info_t *new_buffer_info_ptr = NULL;

	while(tmp_buffer_info_point->next != NULL)
	{
		if(tmp_buffer_info_point->buffer_id == buffer_id)
		{
//			net_debug_printf(("run out ZhwBufferInit() function the new id already in the list\n"));
			return -1;
		}
		tmp_buffer_info_point = tmp_buffer_info_point->next;
	}
	new_buffer_info_ptr = (buffer_info_t *)malloc(sizeof(buffer_info_t));
        if(NULL == new_buffer_info_ptr)
        {
              return -1;
        }
        new_buffer_info_ptr->common_type = common_type;
        new_buffer_info_ptr->pkg_type = pkg_type;
        new_buffer_info_ptr->buffer_type = buffer_type;
        new_buffer_info_ptr->buffer_id = buffer_id;
        new_buffer_info_ptr->next = NULL;
		
	pthread_mutex_lock(&zhw_buffer_register_lock);
	tmp_buffer_info_point->next = new_buffer_info_ptr;
	pthread_mutex_unlock(&zhw_buffer_register_lock);		

       return 0;
}

/*
 *   	@brief	本函数的功能是根据所给出的应用层包类型和buffer类型查找出应用层所使用的buffer id；
 *  	@param pkg_type 应用层的包类型；
 *     @param buffer_type 所要查询的buffer类型：1表示接受buffer，2表示发送buffer
 *
 *     @return   int : 小于0表示查询失败，否则返回所查到的buffer_id
 */
int ZhwBufferInq(int common_type,int pkg_type,int buffer_type)
{
	buffer_info_point = &buffer_info_head;
	while(buffer_info_point != NULL)
	{
		if(common_type == buffer_info_point->common_type && pkg_type == buffer_info_point->pkg_type && buffer_type == buffer_info_point->buffer_type)
		{
			return buffer_info_point->buffer_id;
		}
		else
		{
			buffer_info_point = buffer_info_point->next;
		}
	}

	return -1;
}

