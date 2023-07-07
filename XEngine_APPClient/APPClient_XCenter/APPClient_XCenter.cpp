#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Client/XClient_Socket")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVCollect")
#pragma comment(lib,"XEngine_AVCodec/XEngine_VideoCodec")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp")
#pragma comment(lib,"Ws2_32")
#endif
#include <list>
#include <thread>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Error.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include "../../XEngine_Source/XEngine_UserProtocol.h"
using namespace std;

//需要优先配置XEngine
//WINDOWS使用VS2022 x86 或者 x64 debug 编译
//linux::g++ -std=c++17 -Wall -g APPClient_XCenter.cpp -o APPClient_XCenter.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lNetHelp_APIClient -lXEngine_SystemApi -ljsoncpp
//macos::g++ -std=c++17 -Wall -g APPClient_XCenter.cpp -o APPClient_XCenter.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lNetHelp_APIClient -lXEngine_SystemApi -ljsoncpp
XSOCKET hSocket = 0;
XNETHANDLE xhVideo = 0;
__int64u nTimeVideo = 0;

void CALLBACK XEngine_AVCollect_CBVideo(uint8_t* punStringY, int nYLen, uint8_t* punStringU, int nULen, uint8_t* punStringV, int nVLen, XPVOID lParam)
{
	XCHAR* ptszMsgBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	XENGINE_PROTOCOLDATA st_ProtocolData;

	memset(ptszMsgBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	memset(&st_ProtocolData, '\0', sizeof(XENGINE_PROTOCOLDATA));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQPUSH;
	st_ProtocolHdr.byVersion = 1;
	st_ProtocolHdr.byIsReply = false;   
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	int nListCount = 0;
	AVCODEC_VIDEO_MSGBUFFER** ppSt_MSGBuffer;
	VideoCodec_Stream_EnCodec(xhVideo, punStringY, punStringU, punStringV, nYLen, nULen, nVLen, &ppSt_MSGBuffer, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		nTimeVideo += (1000 / 24);
		st_ProtocolData.byAVType = 0;
		st_ProtocolData.nTimeStamp = nTimeVideo;
		st_ProtocolData.byFrameType = ppSt_MSGBuffer[i]->st_VideoInfo.nFrameType;
		st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOLDATA) + ppSt_MSGBuffer[i]->nYLen;

		memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_ProtocolData, sizeof(XENGINE_PROTOCOLDATA));
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOLDATA), ppSt_MSGBuffer[i]->ptszYBuffer, ppSt_MSGBuffer[i]->nYLen);
		XClient_TCPSelect_SendMsg(hSocket, ptszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOLDATA) + ppSt_MSGBuffer[i]->nYLen);
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_MSGBuffer[i]->ptszYBuffer);

		printf("Time:%llu Size:%d Type:%d\n", nTimeVideo, ppSt_MSGBuffer[i]->nYLen, ppSt_MSGBuffer[i]->st_VideoInfo.nFrameType);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_MSGBuffer, nListCount);
	free(ptszMsgBuffer);
	ptszMsgBuffer = NULL;
}
void CALLBACK XEngine_AVCollect_CBAudio(uint8_t* punStringAudio, int nVLen, XPVOID lParam)
{

}
int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	LPCXSTR lpszServiceAddr = _X("127.0.0.1");
 	if (!XClient_TCPSelect_Create(&hSocket, lpszServiceAddr, 5601))
	{
		_xtprintf("连接失败！%ld\n", XClient_GetLastError());
		return 0;
	}
	_xtprintf("连接成功！\n");
	int nLen = 0;
	XCHAR tszMsgBuffer[2048];
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	XENGINE_PROTOCOLSTREAM st_ProtocolStream;
	
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	memset(&st_ProtocolStream, '\0', sizeof(XENGINE_PROTOCOLSTREAM));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQCREATE;
	st_ProtocolHdr.byVersion = 1;
	st_ProtocolHdr.byIsReply = true;           //获得处理返回结果
	st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOLSTREAM);
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	AVCOLLECT_SCREENINFO st_ScreenInfo;
	memset(&st_ScreenInfo, '\0', sizeof(AVCOLLECT_SCREENINFO));

	st_ScreenInfo.nFrameRate = 24;
	_tcsxcpy(st_ScreenInfo.tszVideoSize, "1920x1080");

	XHANDLE xhScreen = AVCollect_Video_Init("gdigrab", "desktop", &st_ScreenInfo, XEngine_AVCollect_CBVideo);
	AVCollect_Video_GetInfo(xhScreen, &st_ProtocolStream.st_AVInfo);
	printf("AVCollect_Screen_GetInfo:%d %d %lld\n", st_ProtocolStream.st_AVInfo.st_VideoInfo.nWidth, st_ProtocolStream.st_AVInfo.st_VideoInfo.nHeight, st_ProtocolStream.st_AVInfo.st_VideoInfo.nBitRate);

	st_ProtocolStream.st_AVInfo.st_VideoInfo.enAVCodec = ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264;
	if (!VideoCodec_Stream_EnInit(&xhVideo, &st_ProtocolStream.st_AVInfo.st_VideoInfo))
	{
		printf(_X("初始化编码器失败"));
		return -1;
	}

	int nYLen = 0;
	int nULen = 0;
	int nVLen = 0;
	int nFLen = 0;
	XBYTE* ptszYBuffer = (XBYTE*)malloc(XENGINE_MEMORY_SIZE_MAX);
	XBYTE* ptszUBuffer = (XBYTE*)malloc(XENGINE_MEMORY_SIZE_MAX);
	XBYTE* ptszVBuffer = (XBYTE*)malloc(XENGINE_MEMORY_SIZE_MAX);

	memset(ptszYBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
	memset(ptszUBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
	memset(ptszVBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);

	AVCollect_Video_Read(xhScreen, ptszYBuffer, &nYLen, ptszUBuffer, &nULen, ptszVBuffer, &nVLen);

	int nListCount = 0;
	AVCODEC_VIDEO_MSGBUFFER** ppSt_MSGBuffer;
	VideoCodec_Stream_EnCodec(xhVideo, ptszYBuffer, ptszUBuffer, ptszVBuffer, nYLen, nULen, nVLen, &ppSt_MSGBuffer, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		AVHelp_Parse_264Hdr((LPCXSTR)ppSt_MSGBuffer[i]->ptszYBuffer, ppSt_MSGBuffer[i]->nYLen, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &st_ProtocolStream.st_AVInfo.st_VideoInfo.nVLen);
		memcpy(st_ProtocolStream.st_AVInfo.st_VideoInfo.tszVInfo, ppSt_MSGBuffer[i]->ptszYBuffer, st_ProtocolStream.st_AVInfo.st_VideoInfo.nVLen);
		//fwrite(ppSt_MSGBuffer[i]->ptszYBuffer, 1, ppSt_MSGBuffer[i]->nYLen, pSt_File);

		st_ProtocolStream.bLive = true;
		_tcsxcpy(st_ProtocolStream.tszSMSAddr, _X("live/qyt"));

		nLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
		memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_ProtocolStream, sizeof(XENGINE_PROTOCOLSTREAM));

		if (!XClient_TCPSelect_SendMsg(hSocket, tszMsgBuffer, nLen))
		{
			_xtprintf("发送投递失败！\n");
			return 0;
		}
		nLen = 0;
		XCHAR* ptszMsgBuffer;
		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
		if (!XClient_TCPSelect_RecvPkt(hSocket, &ptszMsgBuffer, &nLen, &st_ProtocolHdr))
		{
			_xtprintf("接受数据失败！\n");
			return 0;
		}
		_xtprintf("%d\n", st_ProtocolHdr.wReserve);
		if (nLen > 0)
		{
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_MSGBuffer[i]->ptszYBuffer);
		break;
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_MSGBuffer, nListCount);

	AVCollect_Video_Start(xhScreen);

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}