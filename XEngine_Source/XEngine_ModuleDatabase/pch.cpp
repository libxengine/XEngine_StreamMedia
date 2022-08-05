#include "pch.h"
#include "ModuleDB_AVInfo/ModuleDB_AVInfo.h"
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
CModuleDB_AVInfo m_AVInfo;
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
extern "C" BOOL ModuleDB_AVInfo_Init(DATABASE_MYSQL_CONNECTINFO * pSt_MySQLConnector)
{
	return m_AVInfo.ModuleDB_AVInfo_Init(pSt_MySQLConnector);
}
extern "C" BOOL ModuleDB_AVInfo_Destory()
{
	return m_AVInfo.ModuleDB_AVInfo_Destory();
}
extern "C" BOOL ModuleDB_AVInfo_InfoInsert(XENGINE_PROTOCOLSTREAM * pSt_ProtocolStream)
{
	return m_AVInfo.ModuleDB_AVInfo_InfoInsert(pSt_ProtocolStream);
}
extern "C" BOOL ModuleDB_AVInfo_InfoQuery(XENGINE_PROTOCOLSTREAM * pSt_ProtocolStream)
{
	return m_AVInfo.ModuleDB_AVInfo_InfoQuery(pSt_ProtocolStream);
}
extern "C" BOOL ModuleDB_AVInfo_InfoUPDate(XENGINE_PROTOCOLSTREAM * pSt_ProtocolStream)
{
	return m_AVInfo.ModuleDB_AVInfo_InfoUPDate(pSt_ProtocolStream);
}