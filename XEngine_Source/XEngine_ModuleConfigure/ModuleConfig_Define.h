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
	XCHAR tszSMSUrl[MAX_PATH];                //流媒体服务器地址
	XCHAR tszIPAddr[128];                     //本机IP地址,根据需要配置
	bool bDeamon;                             //是否以守护进程启动,LINUX有效
	bool bDebug;                              //是否使用DEBUG
	int nHttpPort;                            //API接口服务
	int nStreamPort;                          //RTMP标准推流服务
	int nCenterPort;                          //X协议推流服务
	int nJT1078Port;                          //JT1078协议流支持
	struct
	{
		int nMaxClient;                       //最大客户端个数
		int nMaxQueue;                        //最大队列个数
		int nIOThread;                        //网络IO线程数
		int nHTTPThread;                      //API接口任务处理线程数
		int nCenterThread;                    //业务任务处理线程数
		int nJT1078Thread;                    //JT1078任务线程
	}st_XMax;
	struct
	{
		int nTimeCheck;                       //检测次数
		int nHTTPTimeout;                     //HTTP超时时间
		int nCenterTimeout;                   //业务超时时间
		int nJT1078Timeout;                   //1078超时时间
	}st_XTime;                                //次数*时间=超时
	struct
	{
		XCHAR tszSQLAddr[128];                //数据库地址
		XCHAR tszSQLUser[128];                //数据库用户名
		XCHAR tszSQLPass[128];                //数据库密码
		XCHAR tszSQLName[128];                //数据库表名
		int nSQLPort;                         //端口
		bool bEnable;                         //是否启用数据库
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
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleConfigure_GetLastError(int* pInt_ErrorCode = NULL);
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
extern "C" bool ModuleConfigure_Json_File(LPCXSTR lpszConfigFile, XENGINE_SERVICECONFIG* pSt_ServerConfig);