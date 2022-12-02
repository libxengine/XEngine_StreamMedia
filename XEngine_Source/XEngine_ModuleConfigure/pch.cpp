#include "pch.h"
#include "ModuleConfigure_Json/ModuleConfigure_Json.h"
/********************************************************************
//    Created:     2021/12/02  16:20:26
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure\pch.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
BOOL Config_IsErrorOccur = FALSE;
DWORD Config_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////
CModuleConfigure_Json m_ConfigJson;
//////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleConfigure_GetLastError(int* pInt_ErrorCode)
{
	if (pInt_ErrorCode != NULL)
	{
		*pInt_ErrorCode = errno;
	}
	return Config_dwErrorCode;
}
/************************************************************************/
/*                        配置文件读取                                  */
/************************************************************************/
extern "C" BOOL ModuleConfigure_Json_File(LPCTSTR lpszConfigFile, XENGINE_SERVICECONFIG * pSt_ServerConfig)
{
	return m_ConfigJson.ModuleConfigure_Json_File(lpszConfigFile, pSt_ServerConfig);
}
extern "C" BOOL ModuleConfigure_Json_JT1078(LPCTSTR lpszConfigFile, XENGINE_JT1078CONFIG * pSt_ServerConfig)
{
	return m_ConfigJson.ModuleConfigure_Json_JT1078(lpszConfigFile, pSt_ServerConfig);
}
extern "C" BOOL ModuleConfigure_Json_Forward(LPCTSTR lpszConfigFile, XENGINE_FORWARDCONFIG * pSt_ServerConfig)
{
	return m_ConfigJson.ModuleConfigure_Json_Forward(lpszConfigFile, pSt_ServerConfig);
}