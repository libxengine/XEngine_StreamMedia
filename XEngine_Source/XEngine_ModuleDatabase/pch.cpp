#include "pch.h"
#include "ModuleDB_JT1078/ModuleDB_JT1078.h"
/********************************************************************
//    Created:     2022/03/01  09:17:08
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleDatabase\pch.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleDatabase
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
DWORD DBModule_IsErrorOccur = FALSE;
BOOL DBModule_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleDB_JT1078 m_JT1078;
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleDB_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return DBModule_dwErrorCode;
}
/************************************************************************/
/*                         导出的JT1078操作函数                         */
/************************************************************************/
extern "C" BOOL ModuleDB_JT1078_Init(DATABASE_MYSQL_CONNECTINFO * pSt_MySQLConnector)
{
	return m_JT1078.ModuleDB_JT1078_Init(pSt_MySQLConnector);
}
extern "C" BOOL ModuleDB_JT1078_Destory()
{
	return m_JT1078.ModuleDB_JT1078_Destory();
}
extern "C" BOOL ModuleDB_JT1078_DeviceInsert(LPCTSTR lpszDeviceNumber, int nChannel, LPCTSTR lpszDeviceAddr, BOOL bLive, LPCTSTR lpszDeviceVer)
{
	return m_JT1078.ModuleDB_JT1078_DeviceInsert(lpszDeviceNumber, nChannel, lpszDeviceAddr, bLive, lpszDeviceVer);
}
extern "C" BOOL ModuleDB_JT1078_DeviceDelete(LPCTSTR lpszDeviceAddr)
{
	return m_JT1078.ModuleDB_JT1078_DeviceDelete(lpszDeviceAddr);
}
extern "C" BOOL ModuleDB_JT1078_DeviceClear()
{
	return m_JT1078.ModuleDB_JT1078_DeviceClear();
}
extern "C" BOOL ModuleDB_JT1078_InfoInsert(XENGINE_PROTOCOLSTREAM * pSt_ProtocolStream)
{
	return m_JT1078.ModuleDB_JT1078_InfoInsert(pSt_ProtocolStream);
}
extern "C" BOOL ModuleDB_JT1078_InfoQuery(XENGINE_PROTOCOLSTREAM * pSt_ProtocolStream)
{
	return m_JT1078.ModuleDB_JT1078_InfoQuery(pSt_ProtocolStream);
}
extern "C" BOOL ModuleDB_JT1078_InfoUPDate(XENGINE_PROTOCOLSTREAM * pSt_ProtocolStream)
{
	return m_JT1078.ModuleDB_JT1078_InfoUPDate(pSt_ProtocolStream);
}