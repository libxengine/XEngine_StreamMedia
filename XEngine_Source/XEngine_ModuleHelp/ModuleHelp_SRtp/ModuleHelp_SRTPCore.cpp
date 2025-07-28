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
函数功能：SRTP初始化函数
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
/********************************************************************
函数名称：ModuleHelp_SRTPCore_Destory
函数功能：SRTP销毁函数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SRTPCore::ModuleHelp_SRTPCore_Destory()
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_RTC
	srtp_shutdown();
#endif
	return true;
}
/********************************************************************
函数名称：ModuleHelp_SRTPCore_Create
函数功能：创建SRTP发送和接受处理程序
 参数.一：lpszKEYBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入SSL的KEY
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SRTPCore::ModuleHelp_SRTPCore_Create(LPCXBTR lpszKEYBuffer)
{
	ModuleHelp_IsErrorOccur = false;

	int nPos = 0;
	const int SRTP_MASTER_KEY_KEY_LEN = 16;
	const int SRTP_MASTER_KEY_SALT_LEN = 14;

	std::string m_StrClientKey(reinterpret_cast<LPCXSTR>(lpszKEYBuffer), SRTP_MASTER_KEY_KEY_LEN);
	nPos += SRTP_MASTER_KEY_KEY_LEN;
	std::string m_StrServerKey(reinterpret_cast<LPCXSTR>(lpszKEYBuffer + nPos), SRTP_MASTER_KEY_KEY_LEN);
	nPos += SRTP_MASTER_KEY_KEY_LEN;
	std::string m_StrClientSalt(reinterpret_cast<LPCXSTR>(lpszKEYBuffer + nPos), SRTP_MASTER_KEY_SALT_LEN);
	nPos += SRTP_MASTER_KEY_SALT_LEN;
	std::string m_StrServerSalt(reinterpret_cast<LPCXSTR>(lpszKEYBuffer + nPos), SRTP_MASTER_KEY_SALT_LEN);

	std::string m_ClientKey = m_StrClientKey + m_StrClientSalt;
	std::string m_ServerKey = m_StrServerKey + m_StrServerSalt;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_RTC
	srtp_crypto_policy_set_aes_cm_128_hmac_sha1_80(&st_SRTPPolicy.rtp);
	srtp_crypto_policy_set_aes_cm_128_hmac_sha1_80(&st_SRTPPolicy.rtcp);

	st_SRTPPolicy.ssrc.value = 0;
	st_SRTPPolicy.window_size = 8192;
	st_SRTPPolicy.allow_repeat_tx = 1;
	st_SRTPPolicy.next = NULL;

	//初始化接受上下文
	st_SRTPPolicy.ssrc.type = ssrc_any_inbound;
	st_SRTPPolicy.key = (unsigned char*)m_ClientKey.c_str();

	srtp_err_status_t nRet = srtp_err_status_ok;
	if (srtp_err_status_ok != (nRet = srtp_create(&st_SRTPCore.pSt_SRTPRecvCtx, &st_SRTPPolicy)))
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRTP_CREATE;
		return false;
	}
	st_SRTPPolicy.ssrc.type = ssrc_any_outbound;
	st_SRTPPolicy.key = (unsigned char*)m_ServerKey.c_str();

	if (srtp_err_status_ok != (nRet = srtp_create(&st_SRTPCore.pSt_SRTPSendCtx, &st_SRTPPolicy)))
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRTP_CREATE;
		return false;
	}
#endif
	return true;
}
/********************************************************************
函数名称：ModuleHelp_SRTPCore_RTPINProtect
函数功能：RTP协议加密函数
 参数.一：ptszMSGBuffer
  In/Out：In/Out
  类型：字符指针
  可空：N
  意思：输入要加密的数据,输出加密后的数据
 参数.二：pInt_MSGLen
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入要加密的大小,输出加密后的大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SRTPCore::ModuleHelp_SRTPCore_RTPINProtect(XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_RTC

	int nRet = srtp_protect(st_SRTPCore.pSt_SRTPSendCtx, ptszMSGBuffer, pInt_MSGLen);
	if (srtp_err_status_ok != nRet)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRTP_INPROTECT;
		return false;
	}

#endif
	return true;
}
/********************************************************************
函数名称：ModuleHelp_SRTPCore_RTPUNProtect
函数功能：RTP协议解密函数
 参数.一：ptszMSGBuffer
  In/Out：In/Out
  类型：字符指针
  可空：N
  意思：输入要解密的数据,输出解密后的数据
 参数.二：pInt_MSGLen
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入要解密的大小,输出解密后的大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SRTPCore::ModuleHelp_SRTPCore_RTPUNProtect(XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_RTC

	int nRet = srtp_unprotect(st_SRTPCore.pSt_SRTPRecvCtx, ptszMSGBuffer, pInt_MSGLen);
	if (srtp_err_status_ok != nRet)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRTP_UNPROTECT;
		return false;
	}

#endif
	return true;
}
/********************************************************************
函数名称：ModuleHelp_SRTPCore_RTCPINProtect
函数功能：RTCP协议加密函数
 参数.一：ptszMSGBuffer
  In/Out：In/Out
  类型：字符指针
  可空：N
  意思：输入要加密的数据,输出加密后的数据
 参数.二：pInt_MSGLen
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入要加密的大小,输出加密后的大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SRTPCore::ModuleHelp_SRTPCore_RTCPINProtect(XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_RTC

	int nRet = srtp_protect_rtcp(st_SRTPCore.pSt_SRTPSendCtx, ptszMSGBuffer, pInt_MSGLen);
	if (srtp_err_status_ok != nRet)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRTP_INPROTECT;
		return false;
	}

#endif
	return true;
}
/********************************************************************
函数名称：ModuleHelp_SRTPCore_RTCPUNProtect
函数功能：RTCP协议解密函数
 参数.一：ptszMSGBuffer
  In/Out：In/Out
  类型：字符指针
  可空：N
  意思：输入要解密的数据,输出解密后的数据
 参数.二：pInt_MSGLen
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入要解密的大小,输出解密后的大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SRTPCore::ModuleHelp_SRTPCore_RTCPUNProtect(XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_RTC

	int nRet = srtp_unprotect_rtcp(st_SRTPCore.pSt_SRTPRecvCtx, ptszMSGBuffer, pInt_MSGLen);
	if (srtp_err_status_ok != nRet)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRTP_UNPROTECT;
		return false;
	}

#endif
	return true;
}