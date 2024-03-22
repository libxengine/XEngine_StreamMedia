#pragma once
/********************************************************************
//    Created:     2024/03/15  14:42:30
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_SRtp\ModuleHelp_SRTPCore.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_SRtp
//    File Base:   ModuleHelp_SRTPCore
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SRTP协议帮助处理模块
//    History:
*********************************************************************/

typedef struct  
{
	srtp_t pSt_SRTPSendCtx;
	srtp_t pSt_SRTPRecvCtx;
}SRTPCORE_CLIENTINFO;

class CModuleHelp_SRTPCore
{
public:
	CModuleHelp_SRTPCore();
	~CModuleHelp_SRTPCore();
public:
	bool ModuleHelp_SRTPCore_Init();
	bool ModuleHelp_SRTPCore_Destory();
	bool ModuleHelp_SRTPCore_Create(LPCXSTR lpszSendKey, LPCXSTR lpszRecvKey);
protected:
private:
};