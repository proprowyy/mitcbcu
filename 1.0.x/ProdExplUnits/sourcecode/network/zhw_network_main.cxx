/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： zhw_network_main.c
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
#include "zhw_network_main.h"

extern int db_err_status;
//wilson add
struct global_info dev_info_local;
buffer_pointer_t *p_buffer_pointer;
int net_recv_buffer_id_wilson = 0;
extern pthread_t thread_of_network;
cyg_flag_t cmd_flag_type ; //flag 4-1 add by zhw
#ifdef EAMP_DEV
extern pthread_t thread_of_control_processing;
extern pthread_t thread_of_network_processing;
extern unsigned char control_thread_flag;
#endif
unsigned int net_audio_send_total = 0; ///< debug
#if 1
unsigned int pack_number = 0;
unsigned char gd_thread_enter_ccu_db = 0;
void NetSimulationRecvPackage(int buffer_id)
{
        common_big_package_t common_package_buffer = {"\0", 0};
//	int i = 0;

        strcpy(common_package_buffer.dst_dev_name, "CCU");
        common_package_buffer.dst_dev_number = 2;
        strcpy(common_package_buffer.src_dev_name, "CCU");
        common_package_buffer.src_dev_number = 1;		
        common_package_buffer.pkg_type = 2;
        common_package_buffer.common_big_data_u.pa_common_cmd_info.sequence = pack_number++;
        BlockBufferWrite(buffer_id, &common_package_buffer, sizeof(common_package_buffer));
		
        return ;
}
#endif

int gd_udp_common_socket_send_buffer = 0; ///< debug
shell_package_t shellcmd[SHELL_CMD_NUMBER] = {
				{ProtocolTestStart,ZhwProTS},
				{CommQualityTest,ZhwCqcModule},
				{SystemReboot,NetShellReboot},
				{DevFirmwareUpdate,DFUProc},
				{OpenAtuo,OpenAtuoProc},
				{CloseAtuo,CloseAtuoProc},
				{ZhwFunc6,ZhwBakFunc6},
				{ZhwFunc7,ZhwBakFunc7},
				{ZhwFunc8,ZhwBakFunc8},
				{ZhwFunc9,ZhwBakFunc9}
};


void NetWorkMain(network_buffer_t network_buffer)
{
	net_recv_buffer_id_wilson = network_buffer.udp_data_socket_buffer.udp_data_socket_recv_buffer;
//	net_debug_printf(("I am success entry network handle thread!\n"));
	int read_data_long = 0;

	int network_thread_loop_number = 0 ;

	int udp_cmd_socket;
	int udp_data_socket;
	int udp_common_socket;
	int udp_shell_socket;
	int udp_common_big_socket;
	int udp_heart_socket;
//	int tcp_server_socket;
	int max_socket = 0;

	socklen_t len;

	int select_ret = 0;

	int udp_data_flag = 0;//1:表示传送命令，2：表示传送数据

	struct sockaddr_in udp_cmd_socket_addr;//用于提供创建udp命令sockfd的ip和端口
	struct sockaddr_in udp_data_socket_addr;//用于提供创建udp数据sockfd的ip和端口
	struct sockaddr_in udp_common_socket_addr;//用于提供创建udp普通命令sockfd的ip和端口
	struct sockaddr_in udp_shell_socket_addr;//用于提供创建udp shell命令sockfd的ip和端口
	struct sockaddr_in udp_common_big_socket_addr;
	struct sockaddr_in udp_heart_socket_addr;
//	struct sockaddr_in tcp_server_socket_addr;//用于提供创建tcp服务器sockfd的ip和端口

	struct sockaddr_in udp_shell_client_addr; //保存shell通讯的客户端地址
	len = sizeof(udp_shell_client_addr);

	char data_buffer_send[DATA_SIZE];

	//传给银华的参数，用于获取全局变量，在获取中银华进行加锁。
	char device_name_send_data[30];
	int device_number_send_data = 0;


	network_send_package_t network_send_cmd_buf;	//用于网络发送控制信息缓存
	common_package_t common_package_buffer;		//用于存放通用套接字的数据缓存
	common_big_package_t common_big_package_buffer;
	network_heart_package_t heart_package_buffer;

	char shell_recv_buffer[128];
	char shell_send_buffer[128];

	struct dev_type_and_no local_device_io;
	struct global_info dev_info_local;
	struct global_info tar_dev_info;
	char *set_ip;
	char local_mac[6];
	int mac_loop;

	//init the global array---zhw_5.7
//	InitGlobalArray(); //全局设备信息数组初始化，已经移至初始化线程中使用。

	//read message from I/O
	local_device_io = ReadFromIO();
//	net_debug_printf(("the dev type is :%d,the dev number is :%d\n",local_device_io.dev_type,local_device_io.dev_number));

//	diag_printf("db_err_status = %d\n",db_err_status);
	if( 1 == db_err_status )
	{
		dev_info_local = ReadGlobalArray(local_device_io.dev_type,NULL,200,1);
		dev_info_local.dev_mac[0] = 0;
		dev_info_local.dev_mac[1] = 90;
		dev_info_local.dev_mac[2] = 27;
		dev_info_local.dev_mac[3] = 200;
		dev_info_local.dev_mac[4] = local_device_io.dev_type;
		dev_info_local.dev_mac[5] = local_device_io.dev_number;
	}
	else
	{
		dev_info_local = ReadGlobalArray(local_device_io.dev_type,NULL,local_device_io.dev_number,1);
	}

	set_ip = dev_info_local.dev_ip;
	for(mac_loop = 0 ;mac_loop < 6 ;mac_loop++)
	{
		local_mac[mac_loop] = dev_info_local.dev_mac[mac_loop];
	}

#ifdef CYGHWR_NET_DRIVER_ETH0_MANUAL
	net_debug_printf(("system already define CYGHWR_NET_DRIVER_ETH0_MANUAL!!!\n"));
	net_info_t net_info;
	net_info.inf_name = LOCAL_DEV_NAME;
	net_info.addrs_ip = set_ip;
	net_info.addrs_netmask = NETMASK_ADDRS;
	net_info.addrs_broadcast = BROADCAST_ADDRS;
	net_info.addrs_gateway = GATEWAY_ADDRS;
	net_info.addrs_server = SERVER_ADDRS;

	if(ZhwSetAddToSys(net_info,local_mac)<0)
	{
		net_debug_printf(("init network is failed for %s !!!\n",net_info.inf_name));
		exit(0);
	}
#else
	net_debug_printf(("system not define CYGHWR_NET_DRIVER_ETH0_MANUAL!!!\n"));
	init_all_network_interfaces();
#endif

	memset(&udp_cmd_socket_addr, 0, sizeof(udp_cmd_socket_addr));
	udp_cmd_socket_addr = IpAndPortInit(set_ip,UDP_CMD_PORT);

	memset(&udp_data_socket_addr, 0, sizeof(udp_data_socket_addr));
	udp_data_socket_addr = IpAndPortInit(set_ip,UDP_DATA_PORT);

	memset(&udp_common_socket_addr, 0, sizeof(udp_common_socket_addr));
	udp_common_socket_addr = IpAndPortInit(set_ip,UDP_COMMON_PORT);

	memset(&udp_shell_socket_addr, 0, sizeof(udp_shell_socket_addr));
	udp_shell_socket_addr = IpAndPortInit(set_ip,UDP_SHELL_PORT);

	memset(&udp_common_big_socket_addr,0,sizeof(udp_common_big_socket_addr));
	udp_common_big_socket_addr = IpAndPortInit(set_ip,UDP_COMMON_BIG_PORT);

	memset(&udp_heart_socket_addr,0,sizeof(udp_heart_socket_addr));
	udp_heart_socket_addr = IpAndPortInit(set_ip,UDP_HEART_PORT);

//	memset(&tcp_server_socket_addr, 0, sizeof(tcp_server_socket_addr));
//	tcp_server_socket_addr = IpAndPortInit(set_ip,TCP_SERVER_PORT);

	udp_cmd_socket = SockfdServerInit(udp_cmd_socket_addr,"SOCK_DGRAM");//创建与eamp的套接字
	if(udp_cmd_socket<0)
	{
		net_debug_printf(("the udp socket of cmd init fail!!!\n"));
	}
	else
	{
		max_socket = MaxSocket(max_socket,udp_cmd_socket);
	}

	udp_data_socket = SockfdServerInit(udp_data_socket_addr,"SOCK_DGRAM");//创建与eamp的套接字
	if(udp_data_socket<0)
	{
		net_debug_printf(("the udp socket of data init fail!!!\n"));
	}
	else
	{
		max_socket = MaxSocket(max_socket,udp_data_socket);
	}

	udp_common_socket = SockfdServerInit(udp_common_socket_addr,"SOCK_DGRAM");//创建与eamp的套接字
	if(udp_common_socket<0)
	{
		net_debug_printf(("the udp socket of common init fail!!!\n"));
	}
	else
	{
		max_socket = MaxSocket(max_socket,udp_common_socket);
	}

	udp_common_big_socket = SockfdServerInit(udp_common_big_socket_addr,"SOCK_DGRAM");
	if(udp_common_big_socket < 0)
	{
		net_debug_printf(("the udp socket of udp_common_big_socket init fail!!!\n"));
	}
	else
	{
		max_socket = MaxSocket(max_socket,udp_common_big_socket);
	}

	udp_heart_socket = SockfdServerInit(udp_heart_socket_addr,"SOCK_DGRAM");
	if(udp_heart_socket < 0)
	{
		net_debug_printf(("the udp socket of udp_heart_socket init fail!!!\n"));
	}
	else
	{
		max_socket = MaxSocket(max_socket,udp_heart_socket);
	}

	//test ????????????????????????? zhw-6.6
	udp_shell_socket = SockfdServerInit(udp_shell_socket_addr,"SOCK_DGRAM");//创建与eamp的套接字
	if(udp_shell_socket<0)
	{
		net_debug_printf(("the udp shell sockfd init fail!!!\n"));
	}
	else
	{
		net_debug_printf(("the  udp shell socket is created !!!\n"));
		max_socket = MaxSocket(max_socket,udp_shell_socket);
	}

	if(strcmp(dev_info_local.dev_name,"BCU")==0)
	{
		if(ZhwSetMulitIp(udp_cmd_socket,set_ip,MUL_IP_BCU_CMD,UDP_CMD_PORT) < 0)
		{
			net_debug_printf(("udp_cmd_socket %d set mul_ip:%s,port :%d err!!!\n",udp_cmd_socket,MUL_IP_BCU_CMD,UDP_CMD_PORT));
		}
		if(ZhwSetMulitIp(udp_heart_socket,set_ip,MUL_IP_COMMON,UDP_HEART_PORT) < 0)
		{
			net_debug_printf(("udp_heart_socket %d set mul_ip:%s,port :%d err!!!\n",udp_heart_socket,MUL_IP_COMMON,UDP_HEART_PORT));
		}
		if(ZhwSetMulitIp(udp_common_big_socket, set_ip, MUL_IP_SYNC_COMMON_BCU, UDP_SYNC_COMMON_BCU_PORT) < 0) ///< add, 2013
		{
			net_debug_printf(("BCU, sync, udp_common_big_socket=%d, mul_ip=%s,port=%d, error\n", udp_common_big_socket, MUL_IP_SYNC_COMMON_BCU, UDP_SYNC_COMMON_BCU_PORT));
		}
		if(ZhwSetMulitIp(udp_data_socket, set_ip, MUL_IP_CC_DATA, UDP_DATA_PORT) < 0) ///< add, 0819, now, ccu is eamp_x
		{//"230.10.10.56"
			net_debug_printf(("udp_data_socket of BCU=%d, mul_ip=%s_port=%d, error!\n", udp_data_socket, MUL_IP_CC_DATA, UDP_DATA_PORT));
		}
		if(ZhwSetMulitIp(udp_common_socket, set_ip, "230.10.10.86", UDP_COMMON_PORT) < 0) ///< add, 2013
		{
			net_debug_printf(("BCU, sync, udp_common_socket=%d, mul_ip=%s,port=%d, error\n", udp_common_socket, "230.10.10.86", UDP_COMMON_PORT));
		}
		net_debug_printf(("BCU Multi is set ok!!!\n"));
	}
	else if(strcmp(dev_info_local.dev_name,"EAMP")==0)
	{
		if(ZhwSetMulitIp(udp_cmd_socket,set_ip,MUL_IP_EAMP_CMD,UDP_CMD_PORT) < 0)
		{
			net_debug_printf(("udp_cmd_socket %d set mul_ip:%s,port :%d err!!!\n",udp_cmd_socket,MUL_IP_EAMP_CMD,UDP_CMD_PORT));
		}
		if(ZhwSetMulitIp(udp_data_socket,set_ip,MUL_IP_EAMP_DATA,UDP_DATA_PORT) < 0)
		{
			net_debug_printf(("udp_data_socket %d set mul_ip:%s,port :%d err!!!\n",udp_data_socket,MUL_IP_EAMP_DATA,UDP_DATA_PORT));
		}
		if(ZhwSetMulitIp(udp_heart_socket,set_ip,MUL_IP_COMMON,UDP_HEART_PORT) < 0)
		{
			net_debug_printf(("udp_heart_socket %d set mul_ip:%s,port :%d err!!!\n",udp_heart_socket,MUL_IP_COMMON,UDP_HEART_PORT));
		}
		if(ZhwSetMulitIp(udp_common_big_socket, set_ip, MUL_IP_SYNC_COMMON_EAMP, UDP_SYNC_COMMON_EAMP_PORT) < 0) ///< add, 2013
		{
			net_debug_printf(("EAMP, sync, udp_common_big_socket=%d, mul_ip=%s, port=%d, error\n", udp_common_big_socket, MUL_IP_SYNC_COMMON_EAMP, UDP_SYNC_COMMON_EAMP_PORT));
		}		
		net_debug_printf(("EAMP Multi is set ok!!!\n"));
	}
	else if(strcmp(dev_info_local.dev_name,"PCU")==0)
	{
		if(ZhwSetMulitIp(udp_heart_socket,set_ip,MUL_IP_COMMON_PCU,UDP_HEART_PORT) < 0)
		{
			net_debug_printf(("udp_heart_socket %d set mul_ip:%s,port :%d err!!!\n",udp_heart_socket,MUL_IP_COMMON,UDP_HEART_PORT));
		}
		if(ZhwSetMulitIp(udp_shell_socket,set_ip,MUL_IP_SHELL,UDP_SHELL_PORT) < 0)
		{
			net_debug_printf(("udp_common_socket %d set mul_ip:%s,port :%d err!!!\n",udp_shell_socket,MUL_IP_SHELL,UDP_SHELL_PORT));
		}
		if(ZhwSetMulitIp(udp_common_big_socket, set_ip, MUL_IP_SYNC_COMMON_PCU, UDP_SYNC_COMMON_PCU_PORT) < 0) ///< add, 2013
		{
			net_debug_printf(("PCU, sync, comm_socket=%d, mul_ip=%s, port=%d, error\n", udp_common_big_socket, MUL_IP_SYNC_COMMON_PCU, UDP_SYNC_COMMON_PCU_PORT));
		}
		if(ZhwSetMulitIp(udp_cmd_socket, set_ip, MUL_IP_PCU_CMD, UDP_CMD_PORT) < 0) ///< wilson 2014-01-20
		{
			net_debug_printf(("PCU, sync, comm_socket=%d, mul_ip=%s, port=%d, error\n", udp_cmd_socket, MUL_IP_SYNC_COMMON_PCU, UDP_SYNC_COMMON_PCU_PORT));
		}
		net_debug_printf(("PCU Multi is set ok!!!\n"));
	}
	else if(strcmp(dev_info_local.dev_name,"CCU")==0)
	{
		if(ZhwSetMulitIp(udp_cmd_socket, set_ip, MUL_IP_EAMP_CMD, UDP_CMD_PORT) < 0) ///< add, 0819, now, ccu is eamp_x
		{
			net_debug_printf(("udp_cmd_socket of CCU=%d, mul_ip=%s_port=%d, error!\n", udp_cmd_socket, MUL_IP_EAMP_CMD, UDP_CMD_PORT));
		}
		if(ZhwSetMulitIp(udp_cmd_socket, set_ip, MUL_IP_BCU_CMD, UDP_CMD_PORT) < 0) ///< add, 0803, now, ccu is bcu_x
		{
			net_debug_printf(("udp_cmd_socket of CCU=%d, mul_ip=%s_port=%d, error!\n", udp_cmd_socket, MUL_IP_BCU_CMD, UDP_CMD_PORT));
		}	
		if(ZhwSetMulitIp(udp_common_big_socket, set_ip, MUL_IP_SYNC_COMMON_CCU, UDP_SYNC_COMMON_CCU_PORT) < 0) ///< add, 20131231
		{
			net_debug_printf(("udp_common_socket %d, CCU SYNC, set mul_ip:%s,port :%d err!!!\n", udp_common_socket, MUL_IP_SYNC_COMMON_CCU, UDP_SYNC_COMMON_CCU_PORT));
		}
		if(ZhwSetMulitIp(udp_heart_socket,set_ip,MUL_IP_COMMON,UDP_HEART_PORT) < 0)
		{
			net_debug_printf(("udp_heart_socket %d set mul_ip:%s,port :%d err!!!\n",udp_heart_socket,MUL_IP_COMMON,UDP_HEART_PORT));
		}
		if(ZhwSetMulitIp(udp_heart_socket,set_ip,MUL_IP_COMMON_PCU,UDP_HEART_PORT) < 0)
		{
			net_debug_printf(("udp_heart_socket %d set mul_ip:%s,port :%d err!!!\n",udp_heart_socket,MUL_IP_COMMON_PCU,UDP_HEART_PORT));
		}
		if(ZhwSetMulitIp(udp_cmd_socket,set_ip,"230.10.10.44",UDP_COMMON_PORT) < 0)///<just for test
		{
			net_debug_printf(("bcu send  %d set mul_ip:%s,port :%d err!!!\n",udp_common_socket,MUL_IP_COMMON,UDP_COMMON_PORT));
		}
		if(ZhwSetMulitIp(udp_data_socket, set_ip, MUL_IP_CC_DATA, UDP_DATA_PORT) < 0) ///< add, 0819, now, ccu is eamp_x
		{
			net_debug_printf(("udp_data_socket of CCU=%d, mul_ip=%s_port=%d, error!\n", udp_data_socket, MUL_IP_CC_DATA, UDP_DATA_PORT));
		}
//		net_debug_printf(("CCU Multi is set ok!!!\n"));
	}
	else if(strcmp(dev_info_local.dev_name,"CMU")==0)
	{
		if(ZhwSetMulitIp(udp_cmd_socket,set_ip,MUL_IP_CMU_CMD,UDP_CMD_PORT) < 0)
		{
			net_debug_printf(("mcu cmd socket  %d set mul_ip:%s,port :%d err!!!\n",udp_cmd_socket,MUL_IP_CMU_CMD,UDP_CMD_PORT));
		}
		if(ZhwSetMulitIp(udp_data_socket,set_ip,MUL_IP_EAMP_DATA,UDP_DATA_PORT) < 0)
		{
			net_debug_printf(("mcu data socket  %d set mul_ip:%s,port :%d err!!!\n",udp_data_socket,MUL_IP_EAMP_DATA,UDP_DATA_PORT));
		}
		if(ZhwSetMulitIp(udp_data_socket,set_ip,MUL_IP_CC_DATA,UDP_DATA_PORT) < 0)
		{
			net_debug_printf(("mcu data socket  %d set mul_ip:%s,port :%d err!!!\n",udp_data_socket,MUL_IP_CC_DATA,UDP_DATA_PORT));
		}
	}

	fd_set frd;
	fd_set fwd;
	fd_set fad;
	FD_ZERO(&fad);

	FD_SET(udp_cmd_socket,&fad);
	FD_SET(udp_data_socket,&fad);
	FD_SET(udp_common_socket,&fad);
	FD_SET(udp_shell_socket,&fad);
	FD_SET(udp_common_big_socket,&fad);
	FD_SET(udp_heart_socket,&fad);
//	FD_SET(tcp_server_socket,&fad);

	memset(&data_buffer_send,'\0',sizeof(data_buffer_send));//初始化发送音频数据的buffer；从别处移来，防止清除发送数据——zhw
	memset(&shell_recv_buffer,'\0',sizeof(shell_recv_buffer));
	memset(&shell_send_buffer,'\0',sizeof(shell_send_buffer));

	buffer_pointer_t	CachePoint = CacheCreate();
	p_buffer_pointer = &CachePoint;

#ifdef TCP_IP_TEST
	ZhwTcpIpTestAlarmInit();
#endif

//	ZhwShellInit(); //add by zhw_8-21 ;初始化shell命令

	gd_udp_common_socket_send_buffer = network_buffer.udp_common_socket_buffer.udp_common_socket_send_buffer;
#ifdef	EAMP_DEV
	CreateNetworkTimer();
	PthreadPriorityChangeForSchedRr(thread_of_network_processing, 10+9);
#endif
	last_network_flag = network_flag;network_flag = 0;
	network_count = 0;
	char ptu_ip_buffer[20] = "192.168.7.2";
	for(;;)
	{
#ifdef EAMP_DEV
		sem_wait(&sem_wakeup_eamp_network);
#endif
		if( network_thread_loop_number == 5000 )
		{
			network_thread_loop_number = 0 ;
			net_debug_printf(("the network module is run ok!!!\n"));
		}
		else
		{
			network_thread_loop_number = network_thread_loop_number + 1 ;
		}
		FD_ZERO(&frd);
		FD_ZERO(&fwd);
		frd=fad;//将可读标志位赋值
		fwd=fad;
		last_network_flag = network_flag;network_flag = 1;
		select_ret = select(max_socket+1,&frd,&fwd,NULL,0);//监听所有套接口套接口，看是否有数据
		last_network_flag = network_flag;network_flag = 2;
		if(select_ret==-1)
		{
			last_network_flag = network_flag;network_flag = 3;
			net_debug_printf(("the select is error!\n"));
			exit(1);
		}
		else
		{
			last_network_flag = network_flag;network_flag = 4;
			if(FD_ISSET(udp_cmd_socket,&frd))//检测udp的cmd套接字是否有数据可以接受
			{
				last_network_flag = network_flag;network_flag = 5;
				net_debug_printf(("There have cmd to receive\n"));
				diag_printf("There have cmd to receive\n");
				udp_data_flag=1;
				last_network_flag = network_flag;network_flag = 6;
				UdpRecvFun(udp_cmd_socket,udp_data_flag,&CachePoint,network_buffer.udp_cmd_socket_buffer.udp_cmd_socket_recv_buffer);
		#ifdef	EAMP_DEV
				ShowCurrentTime(0);
				control_thread_flag = 0;
				PthreadPriorityChangeForSchedRr(thread_of_control_processing, 18);
			#endif
				if(strcmp(dev_info_local.dev_name,"CMU")==0)
				{
					cyg_flag_setbits(&cmd_flag_type,0x40);
				}
				last_network_flag = network_flag;network_flag = 7;
			} //end of FD_ISSET
			last_network_flag = network_flag;network_flag = 8;
			if(FD_ISSET(udp_data_socket,&frd))//检测udp的data套接字是否有数据接收
			{
				last_network_flag = network_flag;network_flag = 9;
//				net_debug_printf(("udp_data_flag = 2\n"));
				net_debug_printf(("There have audio data to receive!!!\n"));
				udp_data_flag = 2;
				last_network_flag = network_flag;network_flag = 10;
				UdpRecvFun(udp_data_socket,udp_data_flag,&CachePoint,network_buffer.udp_data_socket_buffer.udp_data_socket_recv_buffer);
//				net_debug_printf(("out of udp_data_flag = 2\n"));
				last_network_flag = network_flag;network_flag = 11;
			}
			last_network_flag = network_flag;network_flag = 12;
			if(FD_ISSET(udp_common_socket,&frd))//检测udp的common套接字是否有数据接收
			{
				last_network_flag = network_flag;network_flag = 13;
#ifdef TCP_IP_TEST
				tcp_ip_test_count = 10 ;
#endif
				net_debug_printf(("There have common cmd to receive\n"));
				udp_data_flag = 1;

				int ret = 0;
				struct sockaddr_in recv_add = {0};
				int len_num = sizeof(common_package_buffer);
				int *pToPackLen = &len_num;
				last_network_flag = network_flag;network_flag = 14;
				ret = recvfrom(udp_common_socket, (void *)&common_package_buffer, len_num, 0, (struct sockaddr *)&recv_add, (socklen_t*)pToPackLen);
				last_network_flag = network_flag;network_flag = 15;
				if ( ret > 0)
				{
            	  net_debug_printf(("Net : receive common cmd ok \n"));
//            	  diag_printf("commo recvfrom :%d\n",common_package_buffer.pkg_type);
            	  last_network_flag = network_flag;network_flag = 16;
            	  ret = BlockBufferWrite(network_buffer.udp_common_socket_buffer.udp_common_socket_recv_buffer, (void *)&common_package_buffer, sizeof(common_package_buffer));
            	  last_network_flag = network_flag;network_flag = 17;
            	  if ( ret < 0 )
            	  {
            		  net_debug_printf(("Net : receive common cmd ok, but BlockBufferWrite()<0 \n"));
            	  }
            	  last_network_flag = network_flag;network_flag = 18;
              }
              else
              {
            	  last_network_flag = network_flag;network_flag = 19;
            	  net_debug_printf(("Net, comm_socket:  recvfrom() < 0 \n"));
              }
				last_network_flag = network_flag;network_flag = 20;
				//????添加分类接受函数
				if( ZhwNetworkRecvForward(network_buffer.udp_common_socket_buffer.udp_common_socket_recv_buffer,1) < 0)
				{
					last_network_flag = network_flag;network_flag = 21;
					net_debug_printf(("send package to apply is error!!!\n"));
				}
				last_network_flag = network_flag;network_flag = 22;
			}
			last_network_flag = network_flag;network_flag = 23;
			if(FD_ISSET(udp_common_big_socket,&frd))//检测udp的big common套接字是否有数据接收
			{
				last_network_flag = network_flag;network_flag = 24;
				net_debug_printf(("There have big common cmd to receive\n"));
				udp_data_flag = 1;

				int ret = 0;
				struct sockaddr_in recv_add = {0};
				int len_num = sizeof(common_big_package_buffer);
				int *pToPackLen = &len_num;
				last_network_flag = network_flag;network_flag = 25;
				ret = recvfrom(udp_common_big_socket, (void *)&common_big_package_buffer, len_num, 0, (struct sockaddr *)&recv_add, (socklen_t*)pToPackLen);
				last_network_flag = network_flag;network_flag = 26;
				if ( ret > 0)
                {
            	  net_debug_printf(("Net : receive big common cmd ok-%d,%s \n",common_big_package_buffer.pkg_type,
            			  common_big_package_buffer.src_dev_name));
            	  last_network_flag = network_flag;network_flag = 27;
            	  ret = BlockBufferWrite(network_buffer.udp_common_socket_big_buffer.udp_common_socket_recv_big_buffer, (void *)&common_big_package_buffer, sizeof(common_big_package_buffer));
            	  last_network_flag = network_flag;network_flag = 28;
            	  if ( ret < 0 )
            	  {
            		  last_network_flag = network_flag;network_flag = 29;
            		  net_debug_printf(("Net : receive common cmd ok, but BlockBufferWrite()<0 \n"));
            	  }
            	  last_network_flag = network_flag;network_flag = 30;
              }
              else
              {
            	  last_network_flag = network_flag;network_flag = 31;
            	  net_debug_printf(("Net, comm_socket:  recvfrom() < 0 \n"));
              }
				last_network_flag = network_flag;network_flag = 32;
				//????添加分类接受函数
				if( ZhwNetworkRecvForward(network_buffer.udp_common_socket_big_buffer.udp_common_socket_recv_big_buffer,2) < 0)
				{
					last_network_flag = network_flag;network_flag = 33;
					net_debug_printf(("send package to apply is error!!!\n"));
//					diag_printf("send package to apply is error!!!\n");
				}
				last_network_flag = network_flag;network_flag = 34;
			}
			last_network_flag = network_flag;network_flag = 35;

			if(FD_ISSET(udp_heart_socket,&frd))//检测heart套接字是否有数据接收
			{
				last_network_flag = network_flag;network_flag = 36;
				net_debug_printf(("There have heart package to receive\n"));
				udp_data_flag = 1;

				int ret = 0;
				struct sockaddr_in recv_add = {0};
				int len_num = sizeof(heart_package_buffer);
				int *pToPackLen = &len_num;
				last_network_flag = network_flag;network_flag = 37;
				ret = recvfrom(udp_heart_socket, (void *)&heart_package_buffer, len_num, 0, (struct sockaddr *)&recv_add, (socklen_t*)pToPackLen);
              if ( ret > 0)
              {
            	  last_network_flag = network_flag;network_flag = 38;
            	  net_debug_printf(("Net : receive heart package ok--%s-%d-%d \n",heart_package_buffer.src_dev_name,heart_package_buffer.src_dev_number,heart_package_buffer.heart_package.package_type));
            	  ret = BlockBufferWrite(network_buffer.udp_heart_socket_buffer.udp_heart_socket_recv_buffer, (void *)&heart_package_buffer, sizeof(heart_package_buffer));
            	  last_network_flag = network_flag;network_flag = 39;
            	  if ( ret < 0 )
            	  {
            		  last_network_flag = network_flag;network_flag = 40;
            		  net_debug_printf(("Net : receive heart package ok, but BlockBufferWrite()<0 \n"));
            	  }
            	  last_network_flag = network_flag;network_flag = 41;
              }
              else
              {
            	  last_network_flag = network_flag;network_flag = 42;
            	  net_debug_printf(("Net, heart_socket:  recvfrom() < 0 \n"));
              }
              last_network_flag = network_flag;network_flag = 43;
			}

			if(FD_ISSET(udp_shell_socket,&frd))//检测udp的shell套接字是否有数据接收？？？？？？？
			{
				memset(shell_recv_buffer,0,sizeof(shell_recv_buffer));
				last_network_flag = network_flag;network_flag = 44;
				net_debug_printf(("There shell socket have data to receive\n"));
				last_network_flag = network_flag;network_flag = 45;
				if(recvfrom(udp_shell_socket, (void *)shell_recv_buffer,sizeof(shell_recv_buffer),0,(struct sockaddr *)&udp_shell_client_addr,&len) < 0)
				{
					last_network_flag = network_flag;network_flag = 46;
					net_debug_printf(("udp_shell_socket receive data is err!!!\n"));
				}
				else
				{
					last_network_flag = network_flag;network_flag = 47;
					if(strlen(shell_recv_buffer) > 20 )
					{
						last_network_flag = network_flag;network_flag = 48;
						net_debug_printf(("the mast shell common long is 20 ,now the common of into is :%d!!!\n",strlen(shell_recv_buffer)));
					}
					else
					{
						net_debug_printf2(("shell_recv_buffer = %s\n",shell_recv_buffer));
						network_shell_package_t recv_shell_from_network;
						memcpy(&recv_shell_from_network,shell_recv_buffer,sizeof(network_shell_package_t));
						net_debug_printf2((">>>>>cmd = %d\n",recv_shell_from_network.cmd));
						
						int ret = 0;
						ret = BlockBufferWrite(network_buffer.udp_shell_socket_buffer.udp_shell_socket_recv_buffer, (void *)&shell_recv_buffer, sizeof(shell_recv_buffer));
		            	if ( ret < 0 )
		            	{                      
		            		 net_debug_printf2(("Net : receive shell cmd ok, but BlockBufferWrite()<0 \n"));
		            	}
						
						last_network_flag = network_flag;network_flag = 53;
					}
					last_network_flag = network_flag;network_flag = 54;
				}
				last_network_flag = network_flag;network_flag = 55;
			}
			last_network_flag = network_flag;network_flag = 56;

			if(FD_ISSET(udp_shell_socket,&fwd))
			{
				memset(shell_send_buffer,0,sizeof(shell_send_buffer));
				if(BlockBufferRead(network_buffer.udp_shell_socket_buffer.udp_shell_socket_send_buffer,(void *)&shell_send_buffer,sizeof(shell_send_buffer))>0)
				{
					net_debug_printf2(("There shell socket have data to send-2016\n"));
					int ret = sendto(udp_shell_socket, (void *)shell_send_buffer,
													sizeof(network_shell_package_t),0,
													(struct sockaddr *)&udp_shell_client_addr, len);
					if(ret < 0)
					{
						net_debug_printf(("udp_shell_socket send data is err!!!\n"));
					}
				}
			}

			if(FD_ISSET(udp_cmd_socket,&fwd))
			{
				last_network_flag = network_flag;network_flag = 57;
				//发送控制信息
				if(BlockBufferRead(network_buffer.udp_cmd_socket_buffer.udp_cmd_socket_send_buffer,(void *)&network_send_cmd_buf,sizeof(network_send_cmd_buf))>0)
				{
					last_network_flag = network_flag;network_flag = 58;
//					net_debug_printf(("!!!!!!!!!!!!!!-%s\n",network_send_cmd_buf.dst_devices_name));
//					diag_printf("!!!!!!!!!!!!!!-%s:%d-%d-%d-%d\n",network_send_cmd_buf.dst_devices_name,network_send_cmd_buf.dst_devices_no,
//							network_send_cmd_buf.send_information.line_status.cmd_type,
//							network_send_cmd_buf.send_information.line_status.pkg_number,
//							network_send_cmd_buf.send_information.line_status.pkg_type);
					tar_dev_info = ReadGlobalArray(0,network_send_cmd_buf.dst_devices_name,network_send_cmd_buf.dst_devices_no,2);
					last_network_flag = network_flag;network_flag = 59;
					net_debug_printf(("BCU,net cmd:  tar_ip is : %s\n",tar_dev_info.dev_ip));
//					diag_printf("BCU,net cmd:  tar_ip is : %s\n",tar_dev_info.dev_ip);
					last_network_flag = network_flag;network_flag = 60;

					if(network_send_cmd_buf.send_information.event_type_intercom==7)
					{
						UdpSendFunCMD(udp_cmd_socket,&network_send_cmd_buf,sizeof(network_send_package_t),"192.168.10.200",UDP_CMD_PORT);//发送控制数据
					}
					else
					{
						UdpSendFunCMD(udp_cmd_socket,&network_send_cmd_buf,sizeof(network_send_package_t),tar_dev_info.dev_ip,UDP_CMD_PORT);//发送控制数据
					}

					last_network_flag = network_flag;network_flag = 61;
				}
				last_network_flag = network_flag;network_flag = 62;
			}
			last_network_flag = network_flag;network_flag = 63;
			if(FD_ISSET(udp_data_socket,&fwd))
			{
				last_network_flag = network_flag;network_flag = 64;
				//发送音频信息
				read_data_long = CharBufferRead(network_buffer.udp_data_socket_buffer.udp_data_socket_send_buffer,(void *)data_buffer_send,sizeof(data_buffer_send));
				last_network_flag = network_flag;network_flag = 65;
				if( read_data_long > 0)
				{
					last_network_flag = network_flag;network_flag = 66;
					net_debug_printf(("BCU,net:  send audio, bytes = %d\n",read_data_long));
//					diag_printf("BCU,net:  send audio, bytes = %d\n",read_data_long);
					DSTDeviceNameAndNumber(device_name_send_data,&device_number_send_data);
					last_network_flag = network_flag;network_flag = 67;
					tar_dev_info = ReadGlobalArray(0,device_name_send_data,device_number_send_data,2);
					last_network_flag = network_flag;network_flag = 68;
					net_debug_printf(("BCU,net:  send audio, dst-ip = %s\n", tar_dev_info.dev_ip));
//					diag_printf("BCU,net:  send audio, dst-ip = %s,%d\n", tar_dev_info.dev_ip,device_number_send_data);
					UdpSendFunDATA(udp_data_socket,data_buffer_send,read_data_long,tar_dev_info.dev_ip,UDP_DATA_PORT);//发送音频数据给EAMP
					last_network_flag = network_flag;network_flag = 69;
					if(strcmp("PCU",device_name_send_data) == 0)
					{
						last_network_flag = network_flag;network_flag = 70;
						tar_dev_info = ReadGlobalArray(0,"CC",230,2);
						last_network_flag = network_flag;network_flag = 71;
						UdpSendFunDATA(udp_data_socket,data_buffer_send,read_data_long,tar_dev_info.dev_ip,UDP_DATA_PORT);//发送音频数据给CC
						last_network_flag = network_flag;network_flag = 72;
					}
					last_network_flag = network_flag;network_flag = 73;
				}
				last_network_flag = network_flag;network_flag = 74;
			}
			last_network_flag = network_flag;network_flag = 75;

			if(FD_ISSET(udp_common_socket, &fwd))
			{
				last_network_flag = network_flag;network_flag = 76;
				//发送common 数据
				if(BlockBufferRead(network_buffer.udp_common_socket_buffer.udp_common_socket_send_buffer,(void *)&common_package_buffer,sizeof(common_package_buffer))>0)
				{
					last_network_flag = network_flag;network_flag = 77;
					net_debug_printf(("Comm: !!!!!!!!!!!!!!-%s_%d\n",common_package_buffer.dst_dev_name, common_package_buffer.dst_dev_number));
					net_debug_printf(("Comm: !!!!!!!!!!!!!!-pkg_type=%d\n",common_package_buffer.pkg_type));

					tar_dev_info = ReadGlobalArray(0,common_package_buffer.dst_dev_name,common_package_buffer.dst_dev_number,2);
					last_network_flag = network_flag;network_flag = 78;
					net_debug_printf(("Comm:  tar_ip is : %s\n",tar_dev_info.dev_ip));
					if(common_package_buffer.pkg_type == 13)
					{
						diag_printf("Big Comm:  tar_ip is : %s\n",tar_dev_info.dev_ip);
					}
					UdpSendFunCMD(udp_common_socket,&common_package_buffer,sizeof(common_package_t),tar_dev_info.dev_ip,UDP_COMMON_PORT);//发送控制数据
					last_network_flag = network_flag;network_flag = 79;
				}
				last_network_flag = network_flag;network_flag = 80;
			}
			last_network_flag = network_flag;network_flag = 81;
			if(FD_ISSET(udp_common_big_socket, &fwd))
			{
				last_network_flag = network_flag;network_flag = 82;
				//发送big common 数据
				if(BlockBufferRead(network_buffer.udp_common_socket_big_buffer.udp_common_socket_send_big_buffer,(void *)&common_big_package_buffer,sizeof(common_big_package_buffer))>0)
				{
					last_network_flag = network_flag;network_flag = 83;
					net_debug_printf(("Big Comm: !!!!!!!!!!!!!!-%s_%d\n",common_big_package_buffer.dst_dev_name, common_big_package_buffer.dst_dev_number));
					net_debug_printf(("Big Comm: !!!!!!!!!!!!!!-pkg_type=%d\n",common_big_package_buffer.pkg_type));

					tar_dev_info = ReadGlobalArray(0,common_big_package_buffer.dst_dev_name,common_big_package_buffer.dst_dev_number,2);
					last_network_flag = network_flag;network_flag = 84;
					net_debug_printf(("Big Comm:  tar_ip is : %s\n",tar_dev_info.dev_ip));
					//diag_printf("Big Comm:  tar_ip is : %s\n",tar_dev_info.dev_ip);
					if(common_big_package_buffer.pkg_type == 12)
					{
						tar_dev_info.dev_ip = ptu_ip_buffer;
					}
					UdpSendFunCMD(udp_common_big_socket,&common_big_package_buffer,sizeof(common_big_package_t),tar_dev_info.dev_ip,UDP_COMMON_BIG_PORT);//发送控制数据
					last_network_flag = network_flag;network_flag = 85;
				}
				last_network_flag = network_flag;network_flag = 86;
			}
			last_network_flag = network_flag;network_flag = 87;
			if(FD_ISSET(udp_heart_socket, &fwd))
			{
				last_network_flag = network_flag;network_flag = 88;
				//发送heart 数据
				if(BlockBufferRead(network_buffer.udp_heart_socket_buffer.udp_heart_socket_send_buffer,(void *)&heart_package_buffer,sizeof(heart_package_buffer))>0)
				{
					last_network_flag = network_flag;network_flag = 89;
					net_debug_printf(("Heart send: !!!!!!!!!!!!!!-%s_%d\n",heart_package_buffer.dst_dev_name, heart_package_buffer.dst_dev_number));

					tar_dev_info = ReadGlobalArray(0,heart_package_buffer.dst_dev_name,heart_package_buffer.dst_dev_number,2);
					last_network_flag = network_flag;network_flag = 90;
					net_debug_printf(("Heart:  tar_ip is : %s\n",tar_dev_info.dev_ip));
					UdpSendFunCMD(udp_heart_socket,&heart_package_buffer,sizeof(network_heart_package_t),tar_dev_info.dev_ip,UDP_HEART_PORT);
					last_network_flag = network_flag;network_flag = 91;
				}
				last_network_flag = network_flag;network_flag = 92;
			}
			last_network_flag = network_flag;network_flag = 93;
		}
		last_network_flag = network_flag;network_flag = 94;
#ifdef BCU_DEV
        PthreadPriorityChangeForSchedRr(thread_of_network, 10);
        last_network_flag = network_flag;network_flag = 95;
        network_count ++;
#endif
#ifdef PCU_DEV
        PthreadPriorityChangeForSchedRr(thread_of_network, 10);
#endif
        sched_yield();//释放此次线程控制权		
	}
	
	CacheFree(CachePoint);
}

char *zhw_network_get_local_ip(void)
{
	struct dev_type_and_no local_device_io;
	struct global_info local_dev_info_local;
	
	local_device_io = ReadFromIO();
	local_dev_info_local = ReadGlobalArray(local_device_io.dev_type,NULL,local_device_io.dev_number,1);
	debug_print(("zhw_networ_get_local_ip: %d-%d-%s\n", local_device_io.dev_type, 
		               local_device_io.dev_number, local_dev_info_local.dev_ip));

	return  local_dev_info_local.dev_ip;
}
int zhw_dev_weather_server(int a)
{
	struct dev_type_and_no local_device_io;
	struct global_info local_dev_info_local;
	
	local_device_io = ReadFromIO();
	local_dev_info_local = ReadGlobalArray(local_device_io.dev_type,NULL,local_device_io.dev_number,1);
	debug_print(("zhw_dev_weather_server: %d-%d-%s\n", local_device_io.dev_type, local_device_io.dev_number, local_dev_info_local.dev_ip));

	return  local_dev_info_local.is_server;

}
char *zhw_server_dev_info(int dev_type, int dev_number)
{
	struct global_info local_dev_info_local;
	
	local_dev_info_local = ReadGlobalArray(dev_type, NULL, dev_number, 1);
	debug_print(("zhw_server_dev_info: %d_%d_%s \n", dev_type, dev_number, local_dev_info_local.dev_ip));

	return  local_dev_info_local.dev_ip;

}

void ForceBUfferData_wilson()
{
//	BufferPoint->ArrayToHandle,&(BufferPoint->pToData
	ForceClearDataCache(p_buffer_pointer->ArrayToHandle,&(p_buffer_pointer->pToData));
}
void ForceGetBufferData_wilson()
{
	ForcePushDataCacheToBuffer(p_buffer_pointer->ArrayToHandle,&(p_buffer_pointer->pToData),net_recv_buffer_id_wilson);
}

struct global_info zhw_server_info(int a)
{
	struct global_info local_dev_info_local = {0};
	if( (a==1) || (a==2) )
	{
		local_dev_info_local = ResolveUseServerNumber(a);
	}
	 
	return local_dev_info_local;
}

