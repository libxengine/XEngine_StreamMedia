#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIClient")
#pragma comment(lib,"Ws2_32")
#endif
#include <list>
#include <thread>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Error.h>
#include "../../XEngine_Source/XEngine_UserProtocol.h"
using namespace std;

//需要优先配置XEngine
//WINDOWS使用VS2022 x86 或者 x64 debug 编译
//linux::g++ -std=c++17 -Wall -g APPClient_FLVPull.cpp -o APPClient_FLVPull.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lNetHelp_APIClient -lXEngine_SystemApi -ljsoncpp
//macos::g++ -std=c++17 -Wall -g APPClient_FLVPull.cpp -o APPClient_FLVPull.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lNetHelp_APIClient -lXEngine_SystemApi -ljsoncpp
FILE* pSt_File;

static void CALLBACK APPClient_FLVPull_Callback(XNETHANDLE xhToken, XPVOID lpszMsgBuffer, int nMsgLen, XPVOID lParam)
{
	printf("%d\n", nMsgLen);
	fwrite(lpszMsgBuffer, 1, nMsgLen, pSt_File);
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	XNETHANDLE xhToken = 0;
	LPCXSTR lpszFLVUrl = _X("http://127.0.0.1:5600/api?stream=play&sms=live/013699435573_1&type=flv");

	pSt_File = _xtfopen("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\1.flv", "wb");

	APIClient_Http_Create(&xhToken, APPClient_FLVPull_Callback);
	APIClient_Http_SetUrl(xhToken, lpszFLVUrl, _X("GET"));
	APIClient_Http_Excute(xhToken);
	
	std::this_thread::sleep_for(std::chrono::seconds(20));

	APIClient_Http_Close(xhToken);
	fclose(pSt_File);
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}