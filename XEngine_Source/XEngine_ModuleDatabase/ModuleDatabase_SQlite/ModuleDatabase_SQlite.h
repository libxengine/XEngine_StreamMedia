#pragma once
/********************************************************************
//    Created:     2022/03/01  09:20:01
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleDatabase\ModuleDatabase_SQlite\ModuleDatabase_SQlite.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleDatabase\ModuleDatabase_SQlite
//    File Base:   ModuleDatabase_SQlite
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SQLITE操作类
//    History:
*********************************************************************/


class CModuleDatabase_SQlite
{
public:
	CModuleDatabase_SQlite();
	~CModuleDatabase_SQlite();
public:
	BOOL ModuleDatabase_SQlite_Init(LPCTSTR lpszSQLFile);
	BOOL ModuleDatabase_SQlite_Destory();
private:
	XHDATA xhSQL;
};