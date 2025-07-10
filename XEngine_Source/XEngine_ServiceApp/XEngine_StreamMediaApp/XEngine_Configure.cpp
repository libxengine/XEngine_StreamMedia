#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/06/04  10:53:58
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\XEngine_Configure.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp
//    File Base:   XEngine_Configure
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置文件读写代码
//    History:
*********************************************************************/
bool XEngine_Configure_Parament(int argc, char** argv)
{
	LPCXSTR lpszServiceCfg = _X("./XEngine_Config/XEngine_Config.json");
	LPCXSTR lpszServiceVersion = _X("./XEngine_Config/XEngine_Version.json");
	if (!ModuleConfigure_Json_File(lpszServiceCfg, &st_ServiceConfig))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_File：%lX", ModuleConfigure_GetLastError());
		return false;
	}
	if (!ModuleConfigure_Json_Versions(lpszServiceVersion, &st_ServiceConfig))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_Versions：%lX", ModuleConfigure_GetLastError());
		return false;
	}
	st_ServiceConfig.bDebug = true;

	for (int i = 0; i < argc; i++)
	{
		if ((0 == _tcsxicmp("-h", argv[i])) || (0 == _tcsxicmp("-H", argv[i])))
		{
			XEngine_Configure_Help();
			return false;
		}
		else if (0 == _tcsxicmp("-d", argv[i]))
		{
			st_ServiceConfig.bDeamon = _ttxoi(argv[i + 1]);
		}
		else if (0 == _tcsxicmp("-b", argv[i]))
		{
			st_ServiceConfig.bDebug = true;
		}
		else if (0 == _tcsxicmp("-t", argv[i]))
		{
			bIsTest = true;
		}
		else if (0 == _tcsxicmp("-l", argv[i]))
		{
			LPCXSTR lpszLogLevel = argv[++i];
			if (0 == _tcsxicmp("debug", lpszLogLevel))
			{
				st_ServiceConfig.st_XLog.nLogLeave = XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DETAIL;
			}
			else if (0 == _tcsxicmp("detail", lpszLogLevel))
			{
				st_ServiceConfig.st_XLog.nLogLeave = XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DETAIL;
			}
			else if (0 == _tcsxicmp("info", lpszLogLevel))
			{
				st_ServiceConfig.st_XLog.nLogLeave = XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO;
			}
		}
	}

	return true;
}

void XEngine_Configure_Help()
{
	printf(_X("--------------------------启动参数帮助开始--------------------------\n"));
	printf(_X("网络服务启动参数：程序 参数 参数值，参数是区分大小写的。如果不指定将会加载默认的配置文件里面的参数\n"));
	printf(_X("-h or -H：启动参数帮助提示信息\n"));
	printf(_X("-d：1 启用守护进程，2不启用\n"));
	printf(_X("--------------------------启动参数帮助结束--------------------------\n"));
}
