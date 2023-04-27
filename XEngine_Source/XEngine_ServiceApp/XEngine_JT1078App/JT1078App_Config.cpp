#include "JT1078App_Hdr.h"
/********************************************************************
//    Created:     2022/04/24  14:03:51
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App\JT1078App_Config.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App
//    File Base:   JT1078App_Config
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置文件
//    History:
*********************************************************************/
bool JT1078App_Service_Parament(int argc, char** argv)
{
	LPCXSTR lpszServerCfg = _T("./XEngine_Config/XEngine_Config.json");
	LPCXSTR lpszJT1078Cfg = _T("./XEngine_Config/XEngine_JT1078Config.json");

	if (!ModuleConfigure_Json_File(lpszServerCfg, &st_ServiceCfg))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_File:%lX\n", ModuleConfigure_GetLastError());
		return false;
	}
	if (!ModuleConfigure_Json_JT1078(lpszJT1078Cfg, &st_JT1078Cfg))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_JT1078:%lX\n", ModuleConfigure_GetLastError());
		return false;
	}

	for (int i = 0; i < argc; i++)
	{
		if ((0 == _tcscmp("-h", argv[i])) || (0 == _tcscmp("-H", argv[i])))
		{
			JT1078App_Service_ParamentHelp();
			return false;
		}
		else if (0 == _tcscmp("-l", argv[i]))
		{
			st_ServiceCfg.st_XLog.nLogLeave = _ttoi(argv[i + 1]);
		}
		else if (0 == _tcscmp("-d", argv[i]))
		{
			st_ServiceCfg.bDeamon = _ttoi(argv[i + 1]);
		}
	}

	return true;
}

void JT1078App_Service_ParamentHelp()
{
	printf(_T("--------------------------启动参数帮助开始--------------------------\n"));
	printf(_T("流媒体服务启动参数：程序 参数 参数值，参数是区分大小写的。如果不指定将会加载默认的./XEngine_Config/XEngine_JT1078Config.json配置文件里面的参数\n"));
	printf(_T("-h or -H：启动参数帮助提示信息\n"));
	printf(_T("-v or -V：输出版本号\n"));
	printf(_T("-l：设置日志输出级别\n"));
	printf(_T("-d：1 启用守护进程，2不启用\n"));
	printf(_T("--------------------------启动参数帮助结束--------------------------\n"));
}
