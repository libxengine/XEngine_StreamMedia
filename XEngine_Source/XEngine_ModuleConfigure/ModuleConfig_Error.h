#pragma once
/********************************************************************
//    Created:     2021/12/02  16:15:47
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure\ModuleConfig_Error.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure
//    File Base:   ModuleConfig_Error
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置模块导出错误
//    History:
*********************************************************************/
#define ERROR_MODULE_CONFIGURE_JSON_PARAMENT 0xA0001                 //参数错误
#define ERROR_MODULE_CONFIGURE_JSON_OPENFILE 0xA0002                 //打开文件失败
#define ERROR_MODULE_CONFIGURE_JSON_PARSE 0xA0003                    //解析JSON失败
#define ERROR_MODULE_CONFIGURE_JSON_XMAX 0xA0004                     //读取XMAX配置失败 
#define ERROR_MODULE_CONFIGURE_JSON_XTIME 0xA0005                    //读取XTIME配置失败
#define ERROR_MODULE_CONFIGURE_JSON_XLOG 0xA0006                     //读取XLOG配置失败
#define ERROR_MODULE_CONFIGURE_JSON_XVER 0xA0007                     //读取版本列表失败
#define ERROR_MODULE_CONFIGURE_JSON_MALLOC 0xA0008                   //申请内存失败
#define ERROR_MODULE_CONFIGURE_JSON_XSQL 0xA0009                     //数据库配置错误