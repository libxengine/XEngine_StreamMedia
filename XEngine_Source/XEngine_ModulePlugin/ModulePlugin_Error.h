#pragma once
/********************************************************************
//    Created:     2022/06/15  13:48:36
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModulePlugin\ModulePlugin_Error.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModulePlugin
//    File Base:   ModulePlugin_Error
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     插件核心错误导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                       错误定义
//////////////////////////////////////////////////////////////////////////
#define ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_PARAMENT 0xE1001          //参数错误
#define ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_OPENDL 0xE1002            //打开模块插件失败
#define ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPINIT 0xE1003            //查找初始化函数失败
#define ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPUNINIT 0xE1004          //查找卸载函数失败
#define ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPPLAY 0xE1005            //查找播放函数失败
#define ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPSTOP 0xE1006            //查找停止函数失败
#define ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPCALL 0xE1007            //查找调用执行功能函数失败
#define ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPERROR 0xE1008           //获取错误函数失败
#define ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_NOTFOUND 0xE100A          //没有找到句柄
#define ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_EXECTION 0xE100B          //插件执行失败