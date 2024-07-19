﻿#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Client/XClient_Socket")
#pragma comment(lib,"XEngine_Client/XClient_OPenSsl")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_SDPProtocol")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_NatProtocol")
#pragma comment(lib,"Ws2_32")
#endif
#include <list>
#include <thread>
#include <memory>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include <XEngine_Include/XEngine_Client/SslClient_Define.h>
#include <XEngine_Include/XEngine_Client/SslClient_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/SDPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/SDPProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/NatProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/NatProtocol_Error.h>
#include "../../XEngine_Source/XEngine_UserProtocol.h"
using namespace std;

bool APPClient_WEBRTC_SDPPacket(LPCXSTR lpszAPIUrl, LPCXSTR lpszFileCert, XCHAR* ptszSDPPacket, int* pInt_SDPLen)
{
	XNETHANDLE xhToken = 0;

	SDPProtocol_Packet_Create(&xhToken);
	SDPProtocol_Packet_Owner(xhToken, _X("-"), 123456789, _X("127.0.0.1"));
	SDPProtocol_Packet_Session(xhToken, _X("live/stream"));
	SDPProtocol_Packet_KeepTime(xhToken);
	SDPProtocol_Packet_Bundle(xhToken);

	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("extmap-allow-mixed"));
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("msid-semantic"), _X(" WMS"));
	//AUDIO
	int nAVCount = 1;
	XCHAR** pptszAVList;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&pptszAVList, nAVCount, 64);
	_xstprintf(pptszAVList[0], _X("111"));

	SDPProtocol_Packet_AddMedia(xhToken, _X("audio"), _X("UDP/TLS/RTP/SAVPF"), &pptszAVList, nAVCount, 0, 9);
	SDPProtocol_Packet_ClientInet(xhToken, _X("0.0.0.0"));
	SDPProtocol_Packet_RtcpInet(xhToken, 9);
	SDPProtocol_Packet_ICEUser(xhToken, _X("nzWE"), _X("xk/FvO+TXrJy6739etI/y0Kc"), true);
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("fingerprint"), _X("sha-256 E6:29:09:F9:C5:C7:A0:EA:0B:71:AC:F9:4C:D7:78:79:CC:35:DE:C3:90:45:25:EB:79:8F:14:0A:95:BC:30:B5"));
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("setup"), _X("actpass"));
	SDPProtocol_Packet_OnlyRWFlag(xhToken);
	SDPProtocol_Packet_RtcpComm(xhToken, true, false);

	STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMedia = {};
	st_SDPMedia.st_RTPMap.nChannel = 2;
	st_SDPMedia.st_RTPMap.nSampleRate = 48000;
	st_SDPMedia.st_FmtpAudio.nMinPTime = 10;
	st_SDPMedia.st_FmtpAudio.nUseINBandfec = 1;
	_xstprintf(st_SDPMedia.st_RTPMap.tszCodecName, _X("opus"));
	st_SDPMedia.bTransportCC = true;
	SDPProtocol_Packet_AudioFmt(xhToken, 111, &st_SDPMedia, true);
	//VIDEO
	_xstprintf(pptszAVList[0], _X("106"));
	SDPProtocol_Packet_AddMedia(xhToken, _X("video"), _X("UDP/TLS/RTP/SAVPF"), &pptszAVList, nAVCount, 1, 9);
	SDPProtocol_Packet_ClientInet(xhToken, _X("0.0.0.0"));
	SDPProtocol_Packet_RtcpInet(xhToken, 9);
	SDPProtocol_Packet_ICEUser(xhToken, _X("nzWE"), _X("xk/FvO+TXrJy6739etI/y0Kc"), true);
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("fingerprint"), _X("sha-256 E6:29:09:F9:C5:C7:A0:EA:0B:71:AC:F9:4C:D7:78:79:CC:35:DE:C3:90:45:25:EB:79:8F:14:0A:95:BC:30:B5"));
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("setup"), _X("actpass"));
	SDPProtocol_Packet_OnlyRWFlag(xhToken);
	SDPProtocol_Packet_RtcpComm(xhToken, true, true);

	memset(&st_SDPMedia, '\0', sizeof(STREAMMEDIA_SDPPROTOCOL_MEDIAINFO));
	st_SDPMedia.st_RTPMap.nSampleRate = 90000;
	_xstprintf(st_SDPMedia.st_RTPMap.tszCodecName, _X("H264"));
	st_SDPMedia.bGoogRemb = true;
	st_SDPMedia.bTransportCC = true;
	st_SDPMedia.bCcmFir = true;
	st_SDPMedia.bNAck = true;
	st_SDPMedia.bNAckPli = true;
	st_SDPMedia.st_FmtpVideo.bLevelAllow = true;
	st_SDPMedia.st_FmtpVideo.nPacketMode = 1;
	st_SDPMedia.st_FmtpVideo.tszLeaveId[0] = 0x42;
	st_SDPMedia.st_FmtpVideo.tszLeaveId[1] = 0xe0;
	st_SDPMedia.st_FmtpVideo.tszLeaveId[2] = 0x1f;
	SDPProtocol_Packet_VideoFmt(xhToken, 106, &st_SDPMedia, true);

	SDPProtocol_Packet_GetPacket(xhToken, ptszSDPPacket, pInt_SDPLen);
	SDPProtocol_Packet_Destory(xhToken);
	return true;
}
bool APPClient_WEBRTC_SDPParse(LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR* ptszICEUser, XCHAR* ptszICEPass)
{
	XNETHANDLE xhParse = 0;
	
	if (!SDPProtocol_Parse_Create(&xhParse, lpszMSGBuffer, nMSGLen))
	{
		return false;
	}
	int nAttrCount = 0;
	STREAMMEDIA_SDPPROTOCOL_ATTR** ppSt_ListAttr;
	SDPProtocol_Parse_GetAttr(xhParse, &ppSt_ListAttr, &nAttrCount);

	int nListCount = 0;
	STREAMMEDIA_SDPPROTOCOL_AVMEDIA** ppSt_AVMedia;
	SDPProtocol_Parse_GetAVMedia(xhParse, &ppSt_AVMedia, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMeida = {};
		if (0 == _tcsxnicmp(ppSt_AVMedia[i]->tszAVType, "audio", 5))
		{
			SDPProtocol_Parse_RTPMapAudio(&ppSt_ListAttr, nAttrCount, ppSt_AVMedia[i]->nCodecNumber, &st_SDPMeida);
		}
		else
		{
			SDPProtocol_Parse_RTPMapVideo(&ppSt_ListAttr, nAttrCount, ppSt_AVMedia[i]->nCodecNumber, &st_SDPMeida);
		}
		printf("%s:\n", ppSt_AVMedia[i]->tszAVType);
	}
	int nAVIndex1 = 0;
	int nAVIndex2 = 0;
	SDPProtocol_Parse_AttrBundle(&ppSt_ListAttr, nAttrCount, &nAVIndex1, &nAVIndex2);
	SDPProtocol_Parse_AttrICEUser(&ppSt_ListAttr, nAttrCount, ptszICEUser, ptszICEPass);

	XCHAR tszAlgorithmKey[MAX_PATH] = {};
	XCHAR tszAlgorithmVlu[MAX_PATH] = {};
	SDPProtocol_Parse_AttrFinger(&ppSt_ListAttr, nAttrCount, tszAlgorithmKey, tszAlgorithmVlu);

	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_AVMedia, nListCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAttr, nAttrCount);
	SDPProtocol_Parse_Destory(xhParse);
	return true;
}
bool APPClient_WEBRTC_StunSend(XSOCKET hSocket, LPCXSTR lpszICEUser, LPCXSTR lpszICEPass)
{
	int nMSGLen = 0;
	XCHAR tszMSGBuffer[1024] = {};
	XCHAR tszICEUser[128] = {};
	XCHAR tszTmpBuffer[128] = {};
	XCHAR tszTokenStr[64] = {};
	XCHAR tszRandomStr[10] = {};

	_xstprintf(tszICEUser, _X("%s:nzWE"), lpszICEUser);
	BaseLib_OperatorHandle_CreateStr(tszRandomStr, 8, 0, 2);
	NatProtocol_StunNat_BuildAttr(tszTmpBuffer, &nMSGLen, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_USERNAME, tszICEUser, 13);
	NatProtocol_StunNat_BuildAttr(tszTmpBuffer + nMSGLen, &nMSGLen, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_ICE_CONTROLLING, tszRandomStr, 8);
	NatProtocol_StunNat_BuildPriority(tszTmpBuffer + nMSGLen, &nMSGLen);

	BaseLib_OperatorHandle_CreateStr(tszTokenStr, 12, 0, 2);
	NatProtocol_StunNat_Packet(tszMSGBuffer, &nMSGLen, tszTokenStr, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_CLASS_REQUEST, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_MAPPED_ADDRESS, tszTmpBuffer, true, lpszICEPass, true);

	while (true)
	{
		if (!XClient_UDPSelect_SendMsg(hSocket, tszMSGBuffer, nMSGLen))
		{
			return false;
		}
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	
	return true;
}
bool APPClient_WEBRTC_Dlts(XSOCKET hSocket)
{
	LPCXSTR lpszCertFile = _X("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\certificate.crt");
	LPCXSTR lpszPrivateFile = _X("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\private.key");

	XHANDLE xhSsl = XClient_OPenSsl_InitEx(ENUM_XCLIENT_SSL_TYPE_DTL_VERSION, false, lpszCertFile, NULL, lpszPrivateFile);
	if (NULL == xhSsl)
	{
		return false;
	}
	XClient_OPenSsl_ConfigEx(xhSsl);

	XCLIENT_SSLCERT_SRVINFO st_SslInfo;
	XClient_OPenSsl_ConnectEx(xhSsl, hSocket, &st_SslInfo);

	XBYTE byKEYBuffer[128] = {};
	XClient_OPenSsl_GetKeyEx(xhSsl, byKEYBuffer);
	while (true)
	{
		int nMSGLen = 2048;
		XCHAR tszMSGBuffer[2048] = {};
		if (XClient_UDPSelect_RecvMsg(hSocket, tszMSGBuffer, &nMSGLen))
		{
			int nRVLen = 0;
			XCHAR* ptszMSGBuffer = NULL;
			XClient_OPenSsl_RecvMemoryEx(xhSsl, &ptszMSGBuffer, &nRVLen, tszMSGBuffer, nMSGLen);
			printf("%d\n", nRVLen);
		}
	}
	return true;
}

int main()
{
	XSOCKET hSocket;
	//LPCXSTR lpszAPIUrl = _X("http://10.0.1.88:5600/rtc/v1/whep/?app=live&stream=livestream.flv");
	LPCXSTR lpszAPIUrl = _X("http://app.xyry.org:1985/rtc/v1/whep/?app=live&stream=livestream");
	LPCXSTR lpszFileCert = _X("");
	int nMSGLen = 0;
	XCHAR tszMSGBuffer[2048] = {};

	APPClient_WEBRTC_SDPPacket(lpszAPIUrl, lpszFileCert, tszMSGBuffer, &nMSGLen);

	int nHTTPCode = 0;
	XCHAR* ptszMSGBuffer = NULL;
	if (!APIClient_Http_Request(_X("POST"), lpszAPIUrl, tszMSGBuffer, &nHTTPCode, &ptszMSGBuffer, &nMSGLen))
	{
		printf("APIClient_Http_Request:%lX\n", APIClient_GetLastError());
		return -1;
	}
	XCHAR tszICEUser[128] = {};
	XCHAR tszICEPass[128] = {};
	APPClient_WEBRTC_SDPParse(ptszMSGBuffer, nMSGLen, tszICEUser, tszICEPass);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMSGBuffer);

	XClient_UDPSelect_Create(&hSocket);
	XClient_UDPSelect_Connect(hSocket, "43.139.170.67", 8000);

	std::thread pSTDThread_Stun(APPClient_WEBRTC_StunSend, hSocket, tszICEUser, tszICEPass);
	pSTDThread_Stun.detach();

	std::thread pSTDThread_Dtls(APPClient_WEBRTC_Dlts, hSocket);
	pSTDThread_Dtls.detach();

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	XClient_UDPSelect_Close(hSocket);
	return 0;
}