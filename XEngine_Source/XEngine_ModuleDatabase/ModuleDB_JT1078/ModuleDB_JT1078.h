#pragma once
/********************************************************************
//    Created:     2022/04/24  14:44:41
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleDatabase\ModuleDB_JT1078\ModuleDB_JT1078.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleDatabase\ModuleDB_JT1078
//    File Base:   ModuleDB_JT1078
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JT1078数据库表操作
//    History:
*********************************************************************/

class CModuleDB_JT1078
{
public:
	CModuleDB_JT1078();
	~CModuleDB_JT1078();
public:
	BOOL ModuleDB_JT1078_Init(DATABASE_MYSQL_CONNECTINFO* pSt_MySQLConnector);
	BOOL ModuleDB_JT1078_Destory();
public:
	BOOL ModuleDB_JT1078_DeviceInsert(LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, XNETHANDLE xhClient, BOOL bLive, LPCTSTR lpszDeviceVer);
	BOOL ModuleDB_JT1078_DeviceDelete(LPCTSTR lpszDeviceAddr);
	BOOL ModuleDB_JT1078_DeviceClear();
public:
	BOOL ModuleDB_JT1078_InfoInsert(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);
	BOOL ModuleDB_JT1078_InfoQuery(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream = NULL);
	BOOL ModuleDB_JT1078_InfoUPDate(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);
private:
	XHDATA xhSQL;
};