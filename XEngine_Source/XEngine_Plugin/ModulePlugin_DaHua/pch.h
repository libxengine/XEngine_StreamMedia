// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

#ifdef _MSC_BUILD
// 添加要在此处预编译的标头
#include "framework.h"
#include <tchar.h>
#endif
#endif //PCH_H
#include <string>
#include <list>
#include <shared_mutex>
#include <unordered_map>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include "Plugin_SDKHdr/dhnetsdk.h"
#include "../../XEngine_ModulePlugin/ModulePlugin_Define.h"
#include "PluginDahua_Define.h"
#include "PluginDahua_Error.h"
using namespace std;
/********************************************************************
//    Created:     2022/06/14  13:51:05
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_Plugin\ModulePlugin_DaHua\pch.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_Plugin\ModulePlugin_DaHua
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公用头文件
//    History:
*********************************************************************/
extern BOOL SDKPlugin_IsErrorOccur;
extern DWORD SDKPlugin_dwErrorCode;

#ifdef _MSC_BUILD
#pragma comment(lib,"Plugin_SDKLib/dhnetsdk_x86")
#pragma comment(lib,"Plugin_SDKLib/dhconfigsdk_x86")
#endif