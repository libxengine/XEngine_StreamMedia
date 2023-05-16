#pragma once
/********************************************************************
//    Created:     2022/03/01  09:12:45
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleDatabase\ModuleDB_Define.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleDatabase
//    File Base:   ModuleDB_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     数据库操作导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleDB_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         导出的操作函数                               */
/************************************************************************/
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
extern "C" bool ModuleDB_AVInfo_Init(DATABASE_MYSQL_CONNECTINFO* pSt_MySQLConnector);
/********************************************************************
函数名称：ModuleDatabase_MySql_Destory
函数功能：销毁MYSQL数据库资源链接
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleDB_AVInfo_Destory();
/********************************************************************
函数名称：ModuleDB_AVInfo_InfoInsert
函数功能：音视频参数插入
 参数.一：pSt_ProtocolStream
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要插入的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleDB_AVInfo_InfoInsert(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);
/********************************************************************
函数名称：ModuleDB_AVInfo_InfoQuery
函数功能：音视频信息参数查询
 参数.一：pSt_ProtocolStream
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出查询到的内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleDB_AVInfo_InfoQuery(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream = NULL);
/********************************************************************
函数名称：ModuleDB_AVInfo_InfoUPDate
函数功能：更新音视频参数信息
 参数.一：pSt_ProtocolStream
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要更新的内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleDB_AVInfo_InfoUPDate(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);