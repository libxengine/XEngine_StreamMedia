#pragma once
/********************************************************************
//    Created:     2022/02/28  17:24:57
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleDatabase\ModuleDatabase_MySql\ModuleDatabase_MySql.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleDatabase\ModuleDatabase_MySql
//    File Base:   ModuleDatabase_MySql
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     MYSQL操作类
//    History:
*********************************************************************/


class CModuleDatabase_MySql
{
public:
	CModuleDatabase_MySql();
	~CModuleDatabase_MySql();
public:
	BOOL ModuleDatabase_MySql_Init(DATABASE_MYSQL_CONNECTINFO* pSt_MySQLConnector);
	BOOL ModuleDatabase_MySql_Destory();
private:
	XHDATA xhSQL;
};