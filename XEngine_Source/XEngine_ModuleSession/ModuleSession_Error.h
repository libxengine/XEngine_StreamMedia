#pragma once
/********************************************************************
//    Created:     2022/04/25  10:13:22
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Error.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession
//    File Base:   ModuleSession_Error
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出的1078会话错误
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的JT1078错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_MODULE_SESSION_JT1078_PARAMENT 0xE0001                      //参数错误
#define ERROR_MODULE_SESSION_JT1078_MALLOC 0xE0002                        //申请内存失败
#define ERROR_MODULE_SESSION_JT1078_ADDR 0xE0003                          //地址匹配失败,需要重建
#define ERROR_MODULE_SESSION_JT1078_NOTCLIENT 0xE0004                     //没有客户端