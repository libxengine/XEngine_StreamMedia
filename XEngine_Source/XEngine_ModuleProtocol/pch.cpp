#include "pch.h"
#include "ModuleProtocol_Packet/ModuleProtocol_Packet.h"
#include "ModuleProtocol_Parse/ModuleProtocol_Parse.h"
/********************************************************************
//    Created:     2022/03/01  09:27:08
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleProtocol\pch.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleProtocol
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出协议模块实现
//    History:
*********************************************************************/
BOOL ModuleProtocol_IsErrorOccur = FALSE;
DWORD ModuleProtocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleProtocol_Packet m_ProtocolPacket;
CModuleProtocol_Parse m_ProtocolParse;
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleProtocol_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return ModuleProtocol_dwErrorCode;
}
/************************************************************************/
/*                         导出的协议打包函数                           */
/************************************************************************/

/************************************************************************/
/*                         导出的协议解析函数                           */
/************************************************************************/