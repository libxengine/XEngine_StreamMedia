#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl")
#pragma comment(lib,"XEngine_Client/XClient_Socket")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_SDPProtocol")
#pragma comment(lib,"Ws2_32")
#endif
#include <list>
#include <thread>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include <XEngine_Include/XEngine_Client/SslClient_Define.h>
#include <XEngine_Include/XEngine_Client/SslClient_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/SDPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/SDPProtocol_Error.h>
#include "../../XEngine_Source/XEngine_UserProtocol.h"
using namespace std;

int main()
{
	int nMSGLen = 0;
	XNETHANDLE xhToken = 0;
	XCHAR tszMSGBuffer[2048] = {};
	//LPCXSTR lpszAPIUrl = _X("http://10.0.1.88:5600/rtc/v1/whep/?app=live&stream=livestream.flv");
	LPCXSTR lpszAPIUrl = _X("http://10.0.1.88:1985/rtc/v1/whep/?app=live&stream=livestream");
	LPCXSTR lpszFileCert = _X("");

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
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("fingerprint"), _X("sha-256 7E:A0:FF:BA:8F:C4:2C:E8:02:4C:82:22:96:A7:A2:B2:BF:53:79:AA:B0:0D:61:72:62:33:F9:0D:CC:A3:13:C8"));
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
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("fingerprint"), _X("sha-256 7E:A0:FF:BA:8F:C4:2C:E8:02:4C:82:22:96:A7:A2:B2:BF:53:79:AA:B0:0D:61:72:62:33:F9:0D:CC:A3:13:C8"));
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

	SDPProtocol_Packet_GetPacket(xhToken, tszMSGBuffer, &nMSGLen);
	SDPProtocol_Packet_Destory(xhToken);
	printf("%s\n", tszMSGBuffer);

	int nHTTPCode = 0;
	XCHAR* ptszMSGBuffer = NULL;
	if (!APIClient_Http_Request(_X("POST"), lpszAPIUrl, tszMSGBuffer, &nHTTPCode, &ptszMSGBuffer, &nMSGLen))
	{
		printf("APIClient_Http_Request:%lX\n", APIClient_GetLastError());
		return -1;
	}


	return 0;
}