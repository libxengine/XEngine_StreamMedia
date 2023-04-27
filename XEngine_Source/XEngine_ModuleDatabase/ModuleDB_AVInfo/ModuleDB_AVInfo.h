#pragma once
/********************************************************************
//    Created:     2022/07/27  10:29:36
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleDatabase\ModuleDB_AVInfo\ModuleDB_AVInfo.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleDatabase\ModuleDB_AVInfo
//    File Base:   ModuleDB_AVInfo
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     音视频信息数据库表操作
//    History:
*********************************************************************/

class CModuleDB_AVInfo
{
public:
	CModuleDB_AVInfo();
	~CModuleDB_AVInfo();
public:
	bool ModuleDB_AVInfo_Init(DATABASE_MYSQL_CONNECTINFO* pSt_MySQLConnector);
	bool ModuleDB_AVInfo_Destory();
public:
	bool ModuleDB_AVInfo_InfoInsert(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);
	bool ModuleDB_AVInfo_InfoQuery(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream = NULL);
	bool ModuleDB_AVInfo_InfoUPDate(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);
private:
	XNETHANDLE xhSQL;
};