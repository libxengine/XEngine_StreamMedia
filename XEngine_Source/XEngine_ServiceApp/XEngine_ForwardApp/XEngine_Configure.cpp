#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/10/17  10:59:29
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp\XEngine_Configure.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp
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
	LPCXSTR lpszForwardCfg = _X("./XEngine_Config/XEngine_ForwardConfig.json");

	if (!ModuleConfigure_Json_File(lpszServiceCfg, &st_ServiceConfig))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_File：%lX", ModuleConfigure_GetLastError());
		return false;
	}
	if (!ModuleConfigure_Json_Forward(lpszForwardCfg, &st_ForwardConfig))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_Forward：%lX", ModuleConfigure_GetLastError());
		return false;
	}

	for (int i = 0; i < argc; i++)
	{
		if ((0 == _tcsxcmp("-h", argv[i])) || (0 == _tcsxcmp("-H", argv[i])))
		{
			XEngine_Configure_Help();
			return false;
		}
		else if (0 == _tcsxcmp("-d", argv[i]))
		{
			st_ForwardConfig.bDeamon = _ttxoi(argv[i + 1]);
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
