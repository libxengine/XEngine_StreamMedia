#pragma once
/********************************************************************
//    Created:     2023/09/19  11:09:46
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleQueue\ModuleQueue_JT1078\ModuleQueue_JT1078.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleQueue\ModuleQueue_JT1078
//    File Base:   ModuleQueue_JT1078
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JT1078队列服务
//    History:
*********************************************************************/
typedef struct
{
	int nMsgLen;
	XCHAR* ptszMsgBuffer;
}MODULEQUEUE_PACKET;

class CModuleQueue_JT1078
{
public:
	CModuleQueue_JT1078();
	~CModuleQueue_JT1078();
public:
	bool ModuleQueue_JT1078_Create(LPCXSTR lpszClientAddr);
	bool ModuleQueue_JT1078_Destroy(LPCXSTR lpszClientAddr);
	bool ModuleQueue_JT1078_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);
	bool ModuleQueue_JT1078_Get(LPCXSTR lpszClientAddr, XCHAR** pptszMsgBuffer, int* pInt_MsgLen);
private:
	shared_mutex st_Locker;
private:
	unordered_map<xstring, MODULEQUEUE_PACKET> stl_MapQueue;
};
