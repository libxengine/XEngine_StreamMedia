#include "pch.h"
#include "ModuleHelp_SRTPCore.h"
/********************************************************************
//    Created:     2024/03/15  14:42:49
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_SRtp\ModuleHelp_SRTPCore.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_SRtp
//    File Base:   ModuleHelp_SRTPCore
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SRTP协议帮助处理模块
//    History:
*********************************************************************/
CModuleHelp_SRTPCore::CModuleHelp_SRTPCore()
{
}
CModuleHelp_SRTPCore::~CModuleHelp_SRTPCore()
{
}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleHelp_SRTPCore_Init
函数功能：启动SRT
 参数.一：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要绑定的端口
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SRTPCore::ModuleHelp_SRTPCore_Init()
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_RTC
	srtp_init();
#endif
	return true;
}
bool CModuleHelp_SRTPCore::ModuleHelp_SRTPCore_Destory()
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_RTC
	srtp_shutdown();
#endif
	return true;
}
bool CModuleHelp_SRTPCore::ModuleHelp_SRTPCore_Create(LPCXSTR lpszSendKey, LPCXSTR lpszRecvKey)
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_RTC
	SRTPCORE_CLIENTINFO st_SRTPCore = {};
	srtp_policy_t st_SRTPPolicy = {};

	srtp_crypto_policy_set_aes_cm_128_hmac_sha1_80(&st_SRTPPolicy.rtp);
	srtp_crypto_policy_set_aes_cm_128_hmac_sha1_80(&st_SRTPPolicy.rtcp);

	st_SRTPPolicy.ssrc.value = 0;
	st_SRTPPolicy.window_size = 8192;
	st_SRTPPolicy.allow_repeat_tx = 1;
	st_SRTPPolicy.next = NULL;

	//初始化接受上下文
	st_SRTPPolicy.ssrc.type = ssrc_any_inbound;
	st_SRTPPolicy.key = (unsigned char*)lpszRecvKey;

	srtp_err_status_t r0 = srtp_err_status_ok;
	if ((r0 = srtp_create(&st_SRTPCore.pSt_SRTPRecvCtx, &st_SRTPPolicy)) != srtp_err_status_ok)
	{

	}

	st_SRTPPolicy.ssrc.type = ssrc_any_outbound;
	st_SRTPPolicy.key = (unsigned char*)lpszSendKey;

	if ((r0 = srtp_create(&st_SRTPCore.pSt_SRTPSendCtx, &st_SRTPPolicy)) != srtp_err_status_ok)
	{

	}
#endif
	return true;
}