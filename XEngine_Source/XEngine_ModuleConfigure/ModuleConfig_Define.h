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
	int nRTMPPort;                            //RTMP流服务
	int nXStreamPort;                         //X流协议服务
	int nJT1078Port;                          //JT1078协议流支持
	int nSrtPort;                             //srt协议端口
	struct
	{
		int nMaxClient;                       //最大客户端个数
		int nMaxQueue;                        //最大队列个数
		int nIOThread;                        //网络IO线程数
		int nHTTPThread;                      //API接口任务处理线程数
		int nXStreamThread;                   //业务任务处理线程数
		int nRTMPThread;                      //RTMP任务处理线程数
		int nJT1078Thread;                    //JT1078任务线程
		int nSRTThread;
	}st_XMax;
	struct
	{
		int nTimeCheck;                       //检测次数
		int nHTTPTimeout;                     //HTTP超时时间
		int nXStreamTimeout;                   //业务超时时间
		int nRTMPTimeout;                     //RTMP超时时间
		int nJT1078Timeout;                   //1078超时时间
	}st_XTime;                                //次数*时间=超时
	struct
	{
		int nMaxSize;                         //最大日志大小
		int nMaxCount;                        //最大日志个数
		int nLogLeave;                        //日志等级
	}st_XLog;
	//拉流服务配置,启动越多,服务器性能越差
	struct  
	{
		struct  
		{
			bool bEnable;
		}st_PullXStream;
		struct  
		{
			bool bEnable;
		}st_PullRtmp;
		struct
		{
			bool bEnable;
		}st_PullFlv;
		struct
		{
			XCHAR tszHLSPath[MAX_PATH];
			int nTime;
			bool bEnable;
			bool bClear;
		}st_PullHls;
		struct
		{
			bool bEnable;
		}st_PullSrt;
		struct  
		{
			bool bEnable;
			int nVRTPPort;
			int nVRTCPPort;
			int nARTPPort;
			int nARTCPPort;
		}st_PullRtsp;
		struct  
		{
			bool bEnable;
			int nSTUNPort;
		}st_PullWebRtc;
	}st_XPull;
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