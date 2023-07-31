#pragma once
/********************************************************************
//    Created:     2022/04/24  16:25:53
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Error.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp
//    File Base:   ModuleHelp_Error
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     帮助模块导出错误
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的JT1078错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_MODULE_HELP_JT1078_PARAMENT 0xD0001                         //参数错误
//////////////////////////////////////////////////////////////////////////
//                        导出的SRT错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_MODULE_HELP_SRT_PARAMENT 0xD1001                            //参数错误
#define ERROR_MODULE_HELP_SRT_CRAETE 0xD1002                              //创建SRT失败
#define ERROR_MODULE_HELP_SRT_BIND 0xD1003                                //绑定失败
#define ERROR_MODULE_HELP_SRT_LISTEN 0xD1004                              //监听失败
#define ERROR_MODULE_HELP_SRT_EPOLL 0xD1005                               //EPOLL创建失败
#define ERROR_MODULE_HELP_SRT_ADD 0xD1006                                 //添加事件失败
#define ERROR_MODULE_HELP_SRT_THREAD 0xD1007                              //创建线程失败
#define ERROR_MODULE_HELP_SRT_ACCEPT 0xD1008                              //接受失败
#define ERROR_MODULE_HELP_SRT_GETID 0xD1009                               //获取STREAMID
#define ERROR_MODULE_HELP_SRT_NOTFOUND 0xD100A                            //没有找到