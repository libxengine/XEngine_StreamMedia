#include "pch.h"
#include "ModuleHelp_Rtsp.h"
/********************************************************************
//    Created:     2023/11/22  16:33:31
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Rtsp\ModuleHelp_Rtsp.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Rtsp
//    File Base:   ModuleHelp_Rtsp
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     RTSP协议处理帮助函数
//    History:
*********************************************************************/
CModuleHelp_Rtsp::CModuleHelp_Rtsp()
{
}
CModuleHelp_Rtsp::~CModuleHelp_Rtsp()
{
}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleHelp_Rtsp_GetSMSAddr
函数功能：获得流媒体地址
 参数.一：lpszURLStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的URL
 参数.二：ptszSMSAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获得的流媒体地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_GetSMSAddr(LPCXSTR lpszURLStr, XCHAR* ptszSMSAddr)
{
	ModuleHelp_IsErrorOccur = false;

	LPCXSTR lpszRTSPAddr = _tcsxstr(lpszURLStr, _X("554"));
	_tcsxcpy(ptszSMSAddr, lpszRTSPAddr + 4);
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtsp_CreateClient
函数功能：创建一个RTSP客户端管理器
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
 参数.二：nVControlID
  In/Out：In
  类型：整数型
  可空：N
  意思：视频ID
 参数.三：nAControlID
  In/Out：In
  类型：整数型
  可空：N
  意思：音频控制ID
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_CreateClient(LPCXSTR lpszClientID, int nVControlID, int nAControlID)
{
	ModuleHelp_IsErrorOccur = false;

	RTSPPROTOCOL_CLIENTINFO st_RTSPClient = {};

	st_RTSPClient.nATrackID = nAControlID;
	st_RTSPClient.nVTrackID = nVControlID;
	
	stl_MapRTSPClient.insert(make_pair(lpszClientID, st_RTSPClient));
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtsp_DeleteClient
函数功能：删除一个RTSP客户端
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_DeleteClient(LPCXSTR lpszClientID)
{
	ModuleHelp_IsErrorOccur = false;

	unordered_map<string, RTSPPROTOCOL_CLIENTINFO>::iterator stl_MapIterator = stl_MapRTSPClient.find(lpszClientID);
	if (stl_MapIterator != stl_MapRTSPClient.end())
	{
		stl_MapRTSPClient.erase(stl_MapIterator);
	}
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtsp_SetClient
函数功能：设置一个RTSP客户端管理器
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
 参数.二：nRTPPort
  In/Out：In
  类型：整数型
  可空：N
  意思：RTP端口
 参数.三：nRTCPPort
  In/Out：In
  类型：整数型
  可空：N
  意思：RTCP端口
 参数.四：nTrackID
  In/Out：In
  类型：整数型
  可空：N
  意思：所属端口音视频类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_SetClient(LPCXSTR lpszClientID, int nRTPPort, int nRTCPPort, int nTrackID)
{
	ModuleHelp_IsErrorOccur = false;

	unordered_map<string, RTSPPROTOCOL_CLIENTINFO>::iterator stl_MapIterator = stl_MapRTSPClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapRTSPClient.end())
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_RTSP_NOTFOUND;
		return false;
	}

	if (nTrackID == stl_MapIterator->second.nVTrackID)
	{
		stl_MapIterator->second.nVideoRTPPort = nRTPPort;
		stl_MapIterator->second.nVideoRTCPPort = nRTCPPort;
	}
	else if (nTrackID == stl_MapIterator->second.nATrackID)
	{
		stl_MapIterator->second.nAudioRTPPort = nRTPPort;
		stl_MapIterator->second.nAudioRTCPPort = nRTCPPort;
	}
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtsp_GetClient
函数功能：获取一个RTSP客户端管理器
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
 参数.二：pInt_RTPPort
  In/Out：In
  类型：整数型指针
  可空：N
  意思：RTP端口
 参数.三：pInt_RTCPPort
  In/Out：In
  类型：整数型指针
  可空：N
  意思：RTCP端口
 参数.四：nTrackID
  In/Out：In
  类型：整数型
  可空：N
  意思：所属端口音视频类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_GetClient(LPCXSTR lpszClientID, int* pInt_RTPPort /* = NULL */, int* pInt_RTCPPort /* = NULL */, bool bVideo /* = true */)
{
	ModuleHelp_IsErrorOccur = false;

	unordered_map<string, RTSPPROTOCOL_CLIENTINFO>::iterator stl_MapIterator = stl_MapRTSPClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapRTSPClient.end())
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_RTSP_NOTFOUND;
		return false;
	}

	if (bVideo)
	{
		if (NULL != pInt_RTPPort)
		{
			*pInt_RTPPort = stl_MapIterator->second.nVideoRTPPort;
		}
		if (NULL != pInt_RTCPPort)
		{
			*pInt_RTCPPort = stl_MapIterator->second.nVideoRTCPPort;
		}
	}
	else
	{
		if (NULL != pInt_RTPPort)
		{
			*pInt_RTPPort = stl_MapIterator->second.nAudioRTPPort;
		}
		if (NULL != pInt_RTCPPort)
		{
			*pInt_RTCPPort = stl_MapIterator->second.nAudioRTCPPort;
		}
	}
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtsp_SetSession
函数功能：设置一个会话ID
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
 参数.二：lpszSessionStr
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输入要设置的会话ID
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_SetSession(LPCXSTR lpszClientID, LPCXSTR lpszSessionStr)
{
	ModuleHelp_IsErrorOccur = false;

	unordered_map<string, RTSPPROTOCOL_CLIENTINFO>::iterator stl_MapIterator = stl_MapRTSPClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapRTSPClient.end())
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_RTSP_NOTFOUND;
		return false;
	}

	_tcsxcpy(stl_MapIterator->second.tszSession, lpszSessionStr);
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtsp_GetSession
函数功能：获取一个会话ID
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
 参数.二：ptszSessionStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取到的会话ID
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_GetSession(LPCXSTR lpszClientID, XCHAR* ptszSessionStr)
{
	ModuleHelp_IsErrorOccur = false;

	unordered_map<string, RTSPPROTOCOL_CLIENTINFO>::iterator stl_MapIterator = stl_MapRTSPClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapRTSPClient.end())
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_RTSP_NOTFOUND;
		return false;
	}
	_tcsxcpy(ptszSessionStr, stl_MapIterator->second.tszSession);
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtsp_SetSsrc
函数功能：设置一个RTSP关联的SSRC
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
 参数.二：lpszSsrcStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要关联的SSRC
 参数.三：bVideo
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：输入关联的类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_SetSsrc(LPCXSTR lpszClientID, LPCXSTR lpszSsrcStr, bool bVideo /* = true */)
{
	ModuleHelp_IsErrorOccur = false;

	unordered_map<string, RTSPPROTOCOL_CLIENTINFO>::iterator stl_MapIterator = stl_MapRTSPClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapRTSPClient.end())
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_RTSP_NOTFOUND;
		return false;
	}

	if (bVideo)
	{
		_tcsxcpy(stl_MapIterator->second.tszVSsrc, lpszSsrcStr);
	}
	else
	{
		_tcsxcpy(stl_MapIterator->second.tszASsrc, lpszSsrcStr);
	}
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtsp_GetSsrc
函数功能：获取一个RTSP关联的SSRC
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
 参数.二：ptszSsrcStr
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出关联的SSRC
 参数.三：bVideo
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：输入关联的类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_GetSsrc(LPCXSTR lpszClientID, XCHAR* ptszSsrcStr, bool bVideo /* = true */)
{
	ModuleHelp_IsErrorOccur = false;

	unordered_map<string, RTSPPROTOCOL_CLIENTINFO>::iterator stl_MapIterator = stl_MapRTSPClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapRTSPClient.end())
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_RTSP_NOTFOUND;
		return false;
	}

	if (bVideo)
	{
		_tcsxcpy(ptszSsrcStr,stl_MapIterator->second.tszVSsrc);
	}
	else
	{
		_tcsxcpy(ptszSsrcStr,stl_MapIterator->second.tszASsrc);
	}
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtsp_GetTrack
函数功能：获取RTSP的TRACKID类型
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
 参数.二：nTrackID
  In/Out：In
  类型：整数型
  可空：N
  意思：输入TRACKID
 参数.三：pbVideo
  In/Out：Out
  类型：逻辑型指针
  可空：Y
  意思：输出是视频还是音频
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_GetTrack(LPCXSTR lpszClientID, int nTrackID, bool* pbVideo)
{
	ModuleHelp_IsErrorOccur = false;

	unordered_map<string, RTSPPROTOCOL_CLIENTINFO>::iterator stl_MapIterator = stl_MapRTSPClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapRTSPClient.end())
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_RTSP_NOTFOUND;
		return false;
	}

	if (nTrackID == stl_MapIterator->second.nVTrackID)
	{
		*pbVideo = true;
	}
	else
	{
		*pbVideo = false;
	}
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtsp_GetRTPAddr
函数功能：获取RTP发送地址端口
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
 参数.二：ptszADDRStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出地址字符串
 参数.三：bVideo
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是音频还是视频
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_GetRTPAddr(LPCXSTR lpszClientID, XCHAR* ptszADDRStr, bool bVideo)
{
	ModuleHelp_IsErrorOccur = false;

	unordered_map<string, RTSPPROTOCOL_CLIENTINFO>::iterator stl_MapIterator = stl_MapRTSPClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapRTSPClient.end())
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_RTSP_NOTFOUND;
		return false;
	}
	int nPort = 0;
	XCHAR tszIPAddr[128] = {};
	_tcsxcpy(tszIPAddr, lpszClientID);

	BaseLib_OperatorIPAddr_SegAddr(tszIPAddr, &nPort);
	if (bVideo)
	{
		_xstprintf(ptszADDRStr, _X("%s:%d"), tszIPAddr, stl_MapIterator->second.nVideoRTPPort);
	}
	else
	{
		_xstprintf(ptszADDRStr, _X("%s:%d"), tszIPAddr, stl_MapIterator->second.nAudioRTPPort);
	}
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtsp_GetRTPAddr
函数功能：获取RTCP发送地址端口
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
 参数.二：ptszADDRStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出地址字符串
 参数.三：bVideo
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是音频还是视频
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtsp::ModuleHelp_Rtsp_GetRTCPAddr(LPCXSTR lpszClientID, XCHAR* ptszADDRStr, bool bVideo)
{
	ModuleHelp_IsErrorOccur = false;

	unordered_map<string, RTSPPROTOCOL_CLIENTINFO>::iterator stl_MapIterator = stl_MapRTSPClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapRTSPClient.end())
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_RTSP_NOTFOUND;
		return false;
	}
	int nPort = 0;
	XCHAR tszIPAddr[128] = {};
	BaseLib_OperatorIPAddr_SegAddr(tszIPAddr, &nPort);
	if (bVideo)
	{
		_xstprintf(ptszADDRStr, _X("%s:%d"), tszIPAddr, stl_MapIterator->second.nVideoRTCPPort);
	}
	else
	{
		_xstprintf(ptszADDRStr, _X("%s:%d"), tszIPAddr, stl_MapIterator->second.nAudioRTCPPort);
	}
	return true;
}