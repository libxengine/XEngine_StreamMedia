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

	LPCXSTR lpszRTSPAddr = _tcsstr(lpszURLStr, _X("554"));
	_tcsxcpy(ptszSMSAddr, lpszRTSPAddr + 4);
	return true;
}