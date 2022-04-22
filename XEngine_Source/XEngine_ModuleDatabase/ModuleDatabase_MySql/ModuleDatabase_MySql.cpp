#include "pch.h"
#include "ModuleDatabase_MySql.h"
/********************************************************************
//    Created:     2022/02/28  17:25:01
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleDatabase\ModuleDatabase_MySql\ModuleDatabase_MySql.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleDatabase\ModuleDatabase_MySql
//    File Base:   ModuleDatabase_MySql
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     MYSQL操作类
//    History:
*********************************************************************/
CModuleDatabase_MySql::CModuleDatabase_MySql()
{
}
CModuleDatabase_MySql::~CModuleDatabase_MySql()
{

}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleDatabase_MySql_Init
函数功能：初始化
 参数.一：lpszSQLFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的SQL文件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleDatabase_MySql::ModuleDatabase_MySql_Init(DATABASE_MYSQL_CONNECTINFO* pSt_MySQLConnector)
{
	DBModule_IsErrorOccur = FALSE;
	//打开数据库
	if (!DataBase_MySQL_Connect(&xhSQL, pSt_MySQLConnector))
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：ModuleDatabase_MySql_Destory
函数功能：销毁MYSQL数据库资源链接
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleDatabase_MySql::ModuleDatabase_MySql_Destory()
{
	DBModule_IsErrorOccur = FALSE;

	DataBase_MySQL_Close(xhSQL);
	return TRUE;
}