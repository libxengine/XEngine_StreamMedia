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
//                        导出的错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT 0xE0001                 //参数错误
#define ERROR_STREAMMEDIA_MODULE_SESSION_MALLOC 0xE0002                   //申请内存失败
#define ERROR_STREAMMEDIA_MODULE_SESSION_ADDR 0xE0003                     //地址匹配失败,需要重建
#define ERROR_STREAMMEDIA_MODULE_SESSION_NOTCLIENT 0xE0004                //没有客户端
#define ERROR_STREAMMEDIA_MODULE_SESSION_EXIST 0xE0005                    //存在
#define ERROR_STREAMMEDIA_MODULE_SESSION_NOTDEVICE 0xE0006                //没有设备
#define ERROR_STREAMMEDIA_MODULE_SESSION_NOTCHANNEL 0xE0007               //没有通道
#define ERROR_STREAMMEDIA_MODULE_SESSION_NOTLIVE 0xE0008                  //没有找到属性
#define ERROR_STREAMMEDIA_MODULE_SESSION_EMPTY 0xE0009                    //空的列表
#define ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND 0xE000A                 //没有找到
#define ERROR_STREAMMEDIA_MODULE_SESSION_NOTSUPPORT 0xE000B               //不支持的类型