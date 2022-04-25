#pragma once
/********************************************************************
//    Created:     2021/12/02  16:09:03
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure\ModuleConfig_Define.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure
//    File Base:   ModuleConfig_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置模块导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct tag_XEngine_ServiceConfig
{
	TCHAR tszIPAddr[128];                     //本机IP地址,根据需要配置
	BOOL bDeamon;                             //是否以守护进程启动,LINUX有效
	int nCenterPort;                          //业务端口
	int nHttpPort;                            //HTTP服务端口
	int nHttp2Port;                           //HTTP2服务端口
	int nWSPort;                              //WEBSOCKET端口
	struct
	{
		int nMaxClient;                       //最大客户端个数
		int nMaxQueue;                        //最大队列个数
		int nIOThread;                        //网络IO线程数
		int nCenterThread;                    //业务任务处理线程数
		int nHTTPThread;                      //HTTP任务处理线程数
		int nHTTP2Thread;                     //HTTP2任务处理线程数
		int nWSThread;                        //WEBSOCKET处理线程数
	}st_XMax;
	struct
	{
		int nTimeCheck;                       //检测次数
		int nCenterTimeOut;                   //业务超时时间
		int nHTTPTimeOut;                     //HTTP超时时间
		int nHTTP2TimeOut;                    //HTTP2超时时间
		int nWSTimeOut;                       //WEBSOCKET超时时间
	}st_XTime;                                //次数*时间=超时
	struct
	{
		TCHAR tszSQLAddr[128];                //数据库地址
		TCHAR tszSQLUser[128];                //数据库用户名
		TCHAR tszSQLPass[128];                //数据库密码
		TCHAR tszSQLName[128];                //数据库表名
		int nSQLPort;                         //端口
	}st_XSql;
	struct
	{
		int nMaxSize;                         //最大日志大小
		int nMaxCount;                        //最大日志个数
		int nLogLeave;                        //日志等级
	}st_XLog;
	struct  
	{
		list<string> *pStl_ListVer;           //版本列表
	}st_XVer;
}XENGINE_SERVICECONFIG;
//JT1078服务配置文件
typedef struct 
{
	TCHAR tszIPAddr[64];                      //本机IP地址
	BOOL bDeamon;                             //是否使用守护进程
	BOOL bAudio;                              //是否使用音频
	int nStreamPort;                          //直播流端口
	int nRecordPort;                          //录像流端口
	struct
	{
		int nMaxClient;                       //最大客户端个数
		int nMaxQueue;                        //队列最大个数
		int nIOThread;                        //网络线程池个数
		int nStreamThread;                    //直播流线程个数
		int nRecordThread;                    //录像流线程个数
	}st_XMax;
	struct
	{
		int nTimeCheck;                       //检测次数
		int nStreamTimeout;                   //直播超时时间
		int nRecordTimeout;                   //录像超时时间
	}st_XTime;
	struct
	{
		TCHAR tszIPAddr[128];                 //服务器地址
		int nPort;                            //服务器端口
		int nMaxConnect;                      //最大连接个数
	}st_XClient;
	struct
	{
		list<string>* pStl_ListVer;           //版本列表
	}st_XVer;
}XENGINE_JT1078CONFIG;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleConfigure_GetLastError(int* pInt_ErrorCode = NULL);
/************************************************************************/
/*                        文件配置读取                                  */
/************************************************************************/
/********************************************************************
函数名称：ModuleConfigure_Json_File
函数功能：读取JSON配置文件
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要读取的配置文件
 参数.二：pSt_ServerConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出服务配置信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleConfigure_Json_File(LPCTSTR lpszConfigFile, XENGINE_SERVICECONFIG* pSt_ServerConfig);
/********************************************************************
函数名称：ModuleConfigure_Json_JT1078
函数功能：读取1078配置文件
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要读取的配置文件
 参数.二：pSt_ServerConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出服务配置信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleConfigure_Json_JT1078(LPCTSTR lpszConfigFile, XENGINE_JT1078CONFIG* pSt_ServerConfig);