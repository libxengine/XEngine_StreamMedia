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
extern "C" DWORD ModuleDB_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         导出的JT1078操作函数                         */
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
extern "C" BOOL ModuleDB_JT1078_Init(DATABASE_MYSQL_CONNECTINFO* pSt_MySQLConnector);
/********************************************************************
函数名称：ModuleDatabase_MySql_Destory
函数功能：销毁MYSQL数据库资源链接
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleDB_JT1078_Destory();
/********************************************************************
函数名称：ModuleDB_JT1078_DeviceInsert
函数功能：设备插入
 参数.一：lpszDeviceAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备IP地址
 参数.二：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.三：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入通道号
 参数.四：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入绑定的客户端
 参数.五：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：直播还是录像
 参数.六：lpszDeviceVer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：设备版本
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleDB_JT1078_DeviceInsert(LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, XNETHANDLE xhClient, BOOL bLive, LPCTSTR lpszDeviceVer);
/********************************************************************
函数名称：ModuleDB_JT1078_DeviceDelete
函数功能：设备删除
 参数.一：lpszDeviceAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleDB_JT1078_DeviceDelete(LPCTSTR lpszDeviceAddr);
/********************************************************************
函数名称：ModuleDB_JT1078_DeviceClear
函数功能：清空设备表
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleDB_JT1078_DeviceClear();
/********************************************************************
函数名称：ModuleDB_JT1078_InfoInsert
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
extern "C" BOOL ModuleDB_JT1078_InfoInsert(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);
/********************************************************************
函数名称：ModuleDB_JT1078_InfoQuery
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
extern "C" BOOL ModuleDB_JT1078_InfoQuery(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream = NULL);
/********************************************************************
函数名称：ModuleDB_JT1078_InfoUPDate
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
extern "C" BOOL ModuleDB_JT1078_InfoUPDate(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);