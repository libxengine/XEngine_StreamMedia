#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl")
#pragma comment(lib,"XEngine_Client/XClient_Socket")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp")
#pragma comment(lib,"Ws2_32")
#endif
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
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include "../../XEngine_Source/XEngine_UserProtocol.h"
using namespace std;

//需要优先配置XEngine
//WINDOWS使用VS2022 x86 或者 x64 debug 编译
//linux::g++ -std=c++17 -Wall -g APPClient_JT1078.cpp -o APPClient_JT1078.exe -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lNetHelp_APIClient -lXEngine_SystemApi -ljsoncpp
//macos::g++ -std=c++17 -Wall -g APPClient_JT1078.cpp -o APPClient_JT1078.exe -lXEngine_BaseLib -lNetHelp_APIClient -lXEngine_SystemApi -ljsoncpp

void XEngine_Device_StrtoBCD(LPCXSTR lpszPhoneCode, XBYTE* ptszBCD)
{
	for (int i = 0, nPos = 0; i < 6; i++)
	{
		OPenSsl_Codec_2BytesToBCD(&lpszPhoneCode[nPos], ptszBCD[i]);
		nPos += 2;
	}
}

int main()
{
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);

	LPCXSTR lpszFile = _X("D:\\h264 file\\1080P.264");
	LPCXSTR lpszPhone = _X("013699435573");
	LPCXSTR lpszAddr = _X("127.0.0.1");
	int nPort = 5602;
	XSOCKET hSocket;

	if (!XClient_TCPSelect_Create(&hSocket, lpszAddr, nPort))
	{
		_xtprintf(_X("连接失败"));
		return -1;
	}

	FILE *pSt_File = _xtfopen(lpszFile, _X("rb"));
	if (NULL == pSt_File)
	{
		_xtprintf(_X("文件打开失败!"));
		return -1;
	}
	
	int nSeq = 0;
	XNETHANDLE xhToken = 0;
	AVHelp_Parse_FrameInit(&xhToken, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);

	while (true)
	{
		XCHAR tszRVBuffer[8192];
		memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

		int nRet = fread(tszRVBuffer, 1, sizeof(tszRVBuffer), pSt_File);
		if (nRet <= 0)
		{
			fclose(pSt_File);
			pSt_File = _xtfopen(lpszFile, _X("rb"));
			if (NULL == pSt_File)
			{
				break;
			}
		}
		int nListCount = 0;
		AVHELP_FRAMEDATA** ppSt_Frame;
		AVHelp_Parse_FrameGet(xhToken, tszRVBuffer, nRet, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			bool bFirst = true;
			int nCpyCount = 0;
			int nUseCount = 0;
			int nMsgCount = ppSt_Frame[i]->nMsgLen;
			XENGINE_AVCODEC_VIDEOFRAMETYPE enFrameType;

			printf("%d\n", nMsgCount);
			AVHelp_Parse_NaluType((LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, &enFrameType);
			while (nMsgCount > 0)
			{
				XENGINE_RTPPACKETHDR st_RTPPacket;
				XENGINE_RTPPACKETTAIL st_PacketTail;

				memset(&st_RTPPacket, '\0', sizeof(XENGINE_RTPPACKETHDR));
				memset(&st_PacketTail, '\0', sizeof(XENGINE_RTPPACKETTAIL));

				st_RTPPacket.byFlags[0] = 0x30;
				st_RTPPacket.byFlags[1] = 0x31;
				st_RTPPacket.byFlags[2] = 0x63;
				st_RTPPacket.byFlags[3] = 0x64;

				st_RTPPacket.byV = 2;
				st_RTPPacket.byX = 0;
				st_RTPPacket.byCC = 1;
				st_RTPPacket.byM = 0;
				st_RTPPacket.byPT = 98;

				st_RTPPacket.byChannel = 1;
				st_RTPPacket.byType = 0;
				st_RTPPacket.byPacket = 0;

				if (nMsgCount > 950)
				{
					if (bFirst)
					{
						st_RTPPacket.byPacket = 1;
						bFirst = false;
					}
					else
					{
						st_RTPPacket.byPacket = 3;
					}
					nCpyCount = 950;
				}
				else
				{
					st_RTPPacket.byM = 1;
					st_RTPPacket.byPacket = 2;
					nCpyCount = nMsgCount;
				}

				if (ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_P == enFrameType)
				{
					st_RTPPacket.byType = 1;
				}
				else
				{
					st_RTPPacket.byType = 0;
				}

				st_RTPPacket.wSerial = htons(nSeq++);
				XEngine_Device_StrtoBCD(lpszPhone, st_RTPPacket.bySIMNumber);
				BaseLib_OperatorTime_SetXTPTime(&st_RTPPacket.ullTimestamp);

				int nPos = 0;
				XCHAR tszMsgBuffer[2048];
				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				memcpy(tszMsgBuffer, &st_RTPPacket, sizeof(XENGINE_RTPPACKETHDR));
				nPos += sizeof(XENGINE_RTPPACKETHDR);

				memcpy(tszMsgBuffer + nPos, &st_PacketTail, sizeof(XENGINE_RTPPACKETTAIL));
				nPos += sizeof(XENGINE_RTPPACKETTAIL);

				XSHOT wLen = htons(nCpyCount);
				memcpy(tszMsgBuffer + nPos, &wLen, sizeof(XSHOT));
				nPos += sizeof(XSHOT);

				XClient_TCPSelect_SendMsg(hSocket, tszMsgBuffer, nPos);
				XClient_TCPSelect_SendMsg(hSocket, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer + nUseCount, nCpyCount);
				nUseCount += nCpyCount;
				nMsgCount -= nCpyCount;
			}
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_Frame[i]->ptszMsgBuffer);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Frame, nListCount);
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	}
	AVHelp_Parse_FrameClose(xhToken);

	WSACleanup();
	return 0;
}
