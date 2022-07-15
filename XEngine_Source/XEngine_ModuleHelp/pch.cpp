#include "pch.h"
#include "ModuleHelp_JT1078/ModuleHelp_JT1078.h"
/********************************************************************
//    Created:     2022/04/24  16:28:09
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\pch.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
BOOL ModuleHelp_IsErrorOccur = FALSE;
DWORD ModuleHelp_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleHelp_JT1078 m_HelpJT1078;
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleHelp_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return ModuleHelp_dwErrorCode;
}
/************************************************************************/
/*                         导出的JT1078帮助函数                         */
/************************************************************************/
extern "C" BOOL ModuleHelp_JT1078_BCDToString(BYTE bySIMNumber[6], TCHAR * ptszMsgBuffer)
{
	return m_HelpJT1078.ModuleHelp_JT1078_BCDToString(bySIMNumber, ptszMsgBuffer);
}
extern "C" BOOL ModuleHelp_JT1078_StringToBCD(LPCTSTR lpszMsgBuffer, BYTE * pbySIMNumber)
{
	return m_HelpJT1078.ModuleHelp_JT1078_StringToBCD(lpszMsgBuffer, pbySIMNumber);
}