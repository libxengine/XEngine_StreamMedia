#include "pch.h"
#include "ModuleHelp_Rtmp.h"
/********************************************************************
//    Created:     2023/12/14  14:29:19
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Rtmp\ModuleHelp_Rtmp.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Rtmp
//    File Base:   ModuleHelp_Rtmp
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     RTMP协议处理帮助函数
//    History:
*********************************************************************/
CModuleHelp_Rtmp::CModuleHelp_Rtmp()
{
}
CModuleHelp_Rtmp::~CModuleHelp_Rtmp()
{
}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleHelp_Rtmp_CreateSession
函数功能：创建一个rtmp会话
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入创建的地址
 参数.二：lpszURLStr
  In/Out：Out
  类型：常量字符指针
  可空：N
  意思：输入要解析的URL
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtmp::ModuleHelp_Rtmp_CreateSession(LPCXSTR lpszURLAddr, LPCXSTR lpszURLStr)
{
	ModuleHelp_IsErrorOccur = false;

	RTMPPROTOCOL_CLIENTINFO st_RTMPInfo = {};
	_tcsxcpy(st_RTMPInfo.tszURLStr, lpszURLStr);

	size_t nLen = _tcsxlen(lpszURLStr);
	for (size_t i = nLen; i != 0; i--)
	{
		if (lpszURLStr[i] == '/')
		{
			_tcsxcpy(st_RTMPInfo.tszLiveName, lpszURLStr + i + 1);
			break;
		}
	}
	st_Locker.lock();
	stl_MapRTMPClient.insert(make_pair(lpszURLAddr, st_RTMPInfo));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtmp_GetSession
函数功能：获取会话名称
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
 参数.二：ptszLiveName
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出RTMP的LIVE名称
 返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtmp::ModuleHelp_Rtmp_GetSession(LPCXSTR lpszURLAddr, XCHAR* ptszLiveName)
{
	ModuleHelp_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapRTMPClient.find(lpszURLAddr);
	if (stl_MapIterator == stl_MapRTMPClient.end())
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_RTMP_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	_tcsxcpy(ptszLiveName, stl_MapIterator->second.tszLiveName);
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleHelp_Rtmp_DeleteSession
函数功能：删除一个RTMP客户端
 参数.一：lpszURLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端ID
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_Rtmp::ModuleHelp_Rtmp_DeleteSession(LPCXSTR lpszURLAddr)
{
	ModuleHelp_IsErrorOccur = false;

	st_Locker.lock();
	auto stl_MapIterator = stl_MapRTMPClient.find(lpszURLAddr);
	if (stl_MapIterator != stl_MapRTMPClient.end())
	{
		stl_MapRTMPClient.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}