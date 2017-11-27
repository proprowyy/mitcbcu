/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称：sound_driver.c
 * 文件标识：
 * 摘    要：声卡驱动源文件
 *
 * 当前版本：V1.0.0
 * 作    者：benty
 * 完成日期：2012-11-**
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 * */

#include "sound_driver.h"
int current_sounder_mode = 0;
unsigned char current_dia = 0;
const unsigned char RIFFHeader0[52] =
{
       'R' , 'I' , 'F' , 'F' , /* Chunk ID (RIFF) */
       0x70, 0x70, 0x70, 0x70, /* Chunk payload size (calculate after rec!) */
       'W' , 'A' , 'V' , 'E' , /* RIFF resource format type */
       'f' , 'm' , 't' , ' ' , /* Chunk ID (fmt ) */
       0x14, 0x00, 0x00, 0x00, /* Chunk payload size (0x14 = 20 bytes) */ // 16-19
       0x11, 0x00,             /* Format Tag (IMA ADPCM) */                           // 20-21
       0x01, 0x00,             /* Channels (1) */                                                   // 22-23
       0x40, 0x1f, 0x00, 0x00, /* Sample Rate, 0x1f40 = 8.0kHz */           // 24-27
       0xd7, 0x0f, 0x00, 0x00, /* Average Bytes Per Second */                 // 28-31
       0x00, 0x01,             /* Data Block Size (256 bytes) */                          // 32-33
       0x04, 0x00,             /* ADPCM encoded bits per sample (4 bits) */     // 34-35

       0x02, 0x00,             /* Extra data (2 bytes) */                                      // 36-37
       0xf9, 0x01,             /* Samples per Block (505 samples) */                // 38-39

       'f' , 'a' , 'c' , 't' , /* Chunk ID (fact) */                                                                      // 70-73
       0xc8, 0x01, 0x00, 0x00, /* Chunk payload size (456 bytes (zeropad!)) */   // 74-81
       0xff, 0xff, 0xff, 0xff  /* Number of Samples (calculate after rec!) */
};

/*Add zero to sound*/
int MicZeroAfterWaveHeader(int buffer_id)
{
	unsigned char zero_fill[2048] = {0x00};

	//write into buffer_id
	CharBufferWrite(buffer_id, zero_fill, 2048);
	return 0;
}

/* add 452 zeros after */
const uint8_t RIFFHeader504[] =
{
		'd' , 'a' , 't' , 'a' , /* Chunk ID (data) */
		0x70, 0x70, 0x70, 0x70  /* Chunk payload size (calculate after rec!) */
};
const unsigned char RIFFHeader711[] =
{
       'R' , 'I' , 'F' , 'F' , /* Chunk ID (RIFF) */
       0xff, 0xff, 0xff, 0xff, /* Chunk payload size (calculate after rec!) */
       'W' , 'A' , 'V' , 'E' , /* RIFF resource format type */
       'f' , 'm' , 't' , ' ' , /* Chunk ID (fmt ) */
       0x12, 0x00, 0x00, 0x00, /* Chunk payload size (0x12 = 20 bytes) */ // 16-19
       0x07, 0x00,             /* Format Tag (IMA ADPCM) */                           // 20-21
       0x01, 0x00,             /* Channels (1) */                                                   // 22-23
       0x40, 0x1f, 0x00, 0x00, /* Sample Rate, 0x1f40 = 8.0kHz */           // 24-27
       0xd7, 0x0f, 0x00, 0x00, /* Average Bytes Per Second */                 // 28-31
       0x01, 0x00,             /* Data Block Size (256 bytes) */                          // 32-33
       0x08, 0x00, 0x00, 0x00,             /* ADPCM encoded bits per sample (4 bits) */     // 34-35
	   'f' , 'a' , 'c' , 't' , /* Chunk ID (fact) */
       0x04, 0x00,0x00,0x00, 0x00,0x53,0x07,0x00,                                    // 36-37
	   'd','a','t','a',
       0xff, 0xff, 0xff, 0xff  /* Number of Samples (calculate after rec!) */
};


/*Add sound header*/
int MicAdPcmWavheader(int buffer_id)
{

	        int ret = 0;
		unsigned char wavheader[128] = {0};


	#if 0 ///< G.722 for 5034
		diag_printf("$$$$ func %s line %d\n",__FUNCTION__,__LINE__);
		memcpy((void *)wavheader, (void *)RIFFHeader0, 16);
	#else ///< G.711 for 551
		memcpy((void *)wavheader, (void *)RIFFHeader711, 58);
		ret = CharBufferWrite(buffer_id, wavheader, 58);
		return 0;
	#endif
	wavheader[4] = 0xff; // file size
	wavheader[5] = 0xff;
	wavheader[6] = 0xff;
	wavheader[7] = 0xff;

	wavheader[16] = 20; //  20=0x14
	wavheader[17] = 0;
	wavheader[18] = 0;
	wavheader[19] = 0;

	wavheader[20] = 143; //  143=0x8F
	wavheader[21] = 2;

	wavheader[22] = 1; // channels
	wavheader[23] = 0;

	wavheader[24] = 64; // sampling rate, 64=0x40
	wavheader[25] = 31; // 31=0x1F
	wavheader[26] = 0;
	wavheader[27] = 0;

	wavheader[28] = 160; //bytes per second when playing, 160=0xA0
	wavheader[29] = 15;	// 15=0x0F
	wavheader[30] = 0;
	wavheader[31] = 0;

	wavheader[32] = 1;
	wavheader[33] = 0;
	wavheader[34] = 4;
	wavheader[35] = 0;
	wavheader[36] = 2;
	wavheader[37] = 0;
	wavheader[38] = 2;
	wavheader[39] = 0;

	wavheader[40] = 100; // 100=0x64
	wavheader[41] = 97;   // 97=0x61
	wavheader[42] = 116; // 116=0x74
	wavheader[43] = 97;    // 97=0x61

	wavheader[44] = 255;
	wavheader[45] = 255;
	wavheader[46] = 255;
	wavheader[47] = 255;

	//write into buffer_id
	ret = CharBufferWrite(buffer_id, wavheader, 48);
	return ret;
}

/*Look up the sound card*/
int LookupSndCard(cyg_io_handle_t *param_audio_handle)
{
	cyg_int16 state;

	if((state = cyg_io_lookup(CYGDAT_IO_SND_VS1063A_NAME, param_audio_handle)) == -ENOENT)
	{
		debug_print(("LookupSndCard:  lookup error\n"));
	}
	else
	{
		debug_print(("LookupSndCard:  card lookup ok\n"));
	}

	return 	state;
}

/*This function is used to open the sound card*/
int OpenSndCard(cyg_io_handle_t *param_audio_handle,char *snd_mode)
{
	int ret = 0;
	cyg_uint32 len_of_open_snd_card = 100;
	cyg_int16 state_of_open_snd_card;
	cyg_snd_info snd_info_of_open_snd_card;
	cyg_uint32 snd_set_of_open_snd_card;

	debug_print(("OpenSndCard\n"));

	if(strcmp(snd_mode,"playback") == 0)
	{
		snd_info_of_open_snd_card.mode = CYG_SND_PLAYBACK_MODE;
		current_sounder_mode = BCU_SND_MODE_PLAYBACK;
	}
	else if(strcmp(snd_mode,"codec") == 0)
	{
		snd_info_of_open_snd_card.mode = CYG_SND_CODEC_MODE;
		current_sounder_mode = BCU_SND_MODE_CODEC;
	}
	else
	{
		debug_print(("OpenSndCard:  snd_mode error\n"));
		return -1;
	}

	snd_set_of_open_snd_card = (1<<8) |CYG_SND_OPEN ;
	state_of_open_snd_card = cyg_io_bwrite(*param_audio_handle,(void *)&snd_info_of_open_snd_card, &len_of_open_snd_card, snd_set_of_open_snd_card) ;
	if( state_of_open_snd_card == 0 )
	{
		debug_print(("OpenSndCard:  Success\n"));
	}
	else
	{
		debug_print(("OpenSndCard:  Faile\n"));
		ret = -1;
	}
	len_of_open_snd_card = 0;
	return ret;
}

/*This function is used to close the sound card*/
int CloseSndCard(cyg_io_handle_t *param_audio_handle,char *snd_mode, char *close_type)
{
	cyg_uint32 len_of_close_snd_card = 100;
	cyg_int16 state_of_close_snd_card;
	cyg_snd_info snd_info_of_close_snd_card;
	cyg_uint32 snd_set_of_close_snd_card;

	if(strcmp(snd_mode,"playback") == 0)
	{
		snd_info_of_close_snd_card.mode = CYG_SND_PLAYBACK_MODE;
	}
	else if(strcmp(snd_mode,"codec") == 0)
	{
		snd_info_of_close_snd_card.mode = CYG_SND_CODEC_MODE;
	}
	else
	{
		debug_print(("CloseSndCard:  snd_mode error\n"));
		return -1;
	}

	if(strcmp(close_type,"finish") == 0)
	{
		snd_set_of_close_snd_card = (1<<8) |CYG_SND_OPERATION_FINISH;
	}
	else if(strcmp(close_type,"cancel") == 0)
	{
		snd_set_of_close_snd_card = (1<<8) |CYG_SND_OPERATION_CANCEL;
	}	else
	{
		debug_print(("CloseSndCard:  close_type error\n"));
		return -1;
	}

	state_of_close_snd_card = cyg_io_bwrite(*param_audio_handle, (void *)&snd_info_of_close_snd_card, &len_of_close_snd_card, snd_set_of_close_snd_card) ;
	if( state_of_close_snd_card == 0 )
	{
		debug_print(("CloseSndCard:  Success\n"));
	}
	else
	{
		debug_print(("CloseSndCard:  Faile\n"));
	}
	return state_of_close_snd_card;
}

int CloseSndCard_BCU(cyg_io_handle_t *param_audio_handle,char *snd_mode, char *close_type)
{///<测试函数，目前没有使用
	cyg_uint32 len_of_close_snd_card = 100;
	cyg_int16 state_of_close_snd_card;
	cyg_snd_info snd_info_of_close_snd_card;
	cyg_uint32 snd_set_of_close_snd_card;

	if(strcmp(snd_mode,"playback") == 0)
	{
		snd_info_of_close_snd_card.mode = CYG_SND_PLAYBACK_MODE;
	}
	else if(strcmp(snd_mode,"codec") == 0)
	{
		snd_info_of_close_snd_card.mode = CYG_SND_CODEC_MODE;
	}
	else
	{
		debug_print(("CloseSndCard:  snd_mode error\n"));
		return -1;
	}

	if(strcmp(close_type,"finish") == 0)
	{
		snd_set_of_close_snd_card = (1<<8) |CYG_SND_OPERATION_FINISH;
	}
	else if(strcmp(close_type,"cancel") == 0)
	{
		snd_set_of_close_snd_card = (1<<8) |CYG_SND_OPERATION_CANCEL;
	}	else
	{
		debug_print(("CloseSndCard:  close_type error\n"));
		return -1;
	}
	len_of_close_snd_card = sizeof(snd_info_of_close_snd_card);
	state_of_close_snd_card = cyg_io_bwrite(bcu_audio_handle, (void *)&snd_info_of_close_snd_card, &len_of_close_snd_card, snd_set_of_close_snd_card) ;
	if( state_of_close_snd_card == 0 )
	{
		debug_print(("CloseSndCard:  Success\n"));
	}
	else
	{
		debug_print(("CloseSndCard:  Faile\n"));
	}
	return state_of_close_snd_card;
}

int GetSndCurrentMode(void) ///< <ret=1>: playback;  <2>: codec
{///<获取当前声卡模式
      int ret = -1;
	  
      if( current_sounder_mode == BCU_SND_MODE_PLAYBACK)
      {
            ret = 1;
      }
      else if( current_sounder_mode == BCU_SND_MODE_CODEC)
      {
            ret = 2;
      }

       return ret;
}

void AdjustBCUSNDVolume(unsigned char current_volume)
{///<调节声卡音量
	cyg_snd_info snd_info;
	cyg_uint32 snd_set;
	cyg_int16 stat;
	cyg_uint32 len = 100;
	snd_info.mode = CYG_SND_PLAYBACK_MODE ;
	snd_info.playback_volume = ((current_volume << 8)|current_volume);
	snd_set = (1<<8) | CYG_SND_PLAYBACK_VALUME;
	debug_print(("11111-%02X\n",snd_info.playback_volume));
	len = sizeof(snd_info);
	last_control_flag = control_flag;control_flag = 233;
	stat = cyg_io_bwrite(bcu_audio_handle,&snd_info,&len,snd_set) ;
	last_control_flag = control_flag;control_flag = 234;
	debug_print(("22222222\n"));
}


void bcu_6d5w_ctrl_wilson(unsigned char dia)
{///<设置6d5w扬声器控制器
	bcu_6d5w_ctrl(dia);
	diag_printf("$$$$$$$$$$$$$$$$$$$-%d\n",dia);
	current_dia = dia;
}
