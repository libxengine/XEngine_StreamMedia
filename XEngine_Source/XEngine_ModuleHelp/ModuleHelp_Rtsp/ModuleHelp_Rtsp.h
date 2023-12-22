#pragma once
/********************************************************************
//    Created:     2023/11/22  16:32:33
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Rtsp\ModuleHelp_Rtsp.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Rtsp
//    File Base:   ModuleHelp_Rtsp
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     RTSP协议处理帮助函数
//    History:
*********************************************************************/
typedef struct  
{
	XCHAR tszSession[64];
	XCHAR tszVSsrc[64];
	XCHAR tszASsrc[64];
	int nVTrackID;
	int nATrackID;
	int nVideoRTPPort;
	int nVideoRTCPPort;
	int nAudioRTPPort;
	int nAudioRTCPPort;
}RTSPPROTOCOL_CLIENTINFO;

class CModuleHelp_Rtsp
{
public:
	CModuleHelp_Rtsp();
	~CModuleHelp_Rtsp();
public:
	bool ModuleHelp_Rtsp_GetSMSAddr(LPCXSTR lpszURLStr, XCHAR* ptszSMSAddr);
public:
	bool ModuleHelp_Rtsp_CreateClient(LPCXSTR lpszClientID, int nVControlID, int nAControlID);
	bool ModuleHelp_Rtsp_DeleteClient(LPCXSTR lpszClientID);
	bool ModuleHelp_Rtsp_SetClient(LPCXSTR lpszClientID, int nRTPPort, int nRTCPPort, int nTrackID);
	bool ModuleHelp_Rtsp_GetClient(LPCXSTR lpszClientID, int* pInt_RTPPort = NULL, int* pInt_RTCPPort = NULL, bool bVideo = true);
	bool ModuleHelp_Rtsp_SetSession(LPCXSTR lpszClientID, LPCXSTR lpszSessionStr);
	bool ModuleHelp_Rtsp_GetSession(LPCXSTR lpszClientID, XCHAR *ptszSessionStr);
	bool ModuleHelp_Rtsp_SetSsrc(LPCXSTR lpszClientID, LPCXSTR lpszSsrcStr, bool bVideo = true);
	bool ModuleHelp_Rtsp_GetSsrc(LPCXSTR lpszClientID, XCHAR *ptszSsrcStr, bool bVideo = true);
	bool ModuleHelp_Rtsp_GetTrack(LPCXSTR lpszClientID, int nTrackID, bool* pbVideo);
	bool ModuleHelp_Rtsp_GetRTPAddr(LPCXSTR lpszClientID, XCHAR* ptszADDRStr, bool bVideo);
	bool ModuleHelp_Rtsp_GetRTCPAddr(LPCXSTR lpszClientID, XCHAR* ptszADDRStr, bool bVideo);
protected:
private:
	unordered_map<string, RTSPPROTOCOL_CLIENTINFO> stl_MapRTSPClient;
};