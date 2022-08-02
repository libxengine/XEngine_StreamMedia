
// XEngine_APPJT1078Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "XEngine_APPJT1078.h"
#include "XEngine_APPJT1078Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXEngineAPPJT1078Dlg 对话框



CXEngineAPPJT1078Dlg::CXEngineAPPJT1078Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XENGINE_APPJT1078_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXEngineAPPJT1078Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_EditIPAddr);
	DDX_Control(pDX, IDC_EDIT1, m_EditPort);
	DDX_Control(pDX, IDC_EDIT3, m_EditCount);
	DDX_Control(pDX, IDC_EDIT2, m_EditSim);
	DDX_Control(pDX, IDC_EDIT4, m_EditVideo);
	DDX_Control(pDX, IDC_EDIT5, m_EditAudio);
	DDX_Control(pDX, IDC_RADIO1, m_Radio2016);
	DDX_Control(pDX, IDC_RADIO2, m_Radio2014);
	DDX_Control(pDX, IDC_BUTTON3, m_BtnConnect);
	DDX_Control(pDX, IDC_BUTTON4, m_BtnSend);
	DDX_Control(pDX, IDC_BUTTON5, m_BtnClose);
}

BEGIN_MESSAGE_MAP(CXEngineAPPJT1078Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CXEngineAPPJT1078Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CXEngineAPPJT1078Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CXEngineAPPJT1078Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CXEngineAPPJT1078Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CXEngineAPPJT1078Dlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CXEngineAPPJT1078Dlg 消息处理程序

BOOL CXEngineAPPJT1078Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);

	InitializeSRWLock(&st_Locker);

	m_EditVideo.SetWindowText(_T("D:\\h264 file\\480p.264"));
	m_EditSim.SetWindowText(_T("013699435573"));
	m_EditIPAddr.SetWindowText(_T("127.0.0.1"));
	m_EditPort.SetWindowText("10780");
	m_EditCount.SetWindowText("1");
	m_Radio2016.SetCheck(1);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXEngineAPPJT1078Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CXEngineAPPJT1078Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CXEngineAPPJT1078Dlg::XEngine_Device_StrtoBCD(LPCTSTR lpszPhoneCode, BYTE* ptszBCD)
{
	for (int i = 0, nPos = 0; i < 6; i++)
	{
		OPenSsl_Codec_2BytesToBCD(&lpszPhoneCode[nPos], ptszBCD[i]);
		nPos += 2;
	}
}
DWORD WINAPI CXEngineAPPJT1078Dlg::XEngine_Device_Thread2014(LPVOID lParam)
{
	XENGINE_DEVICEINFO* pSt_DeviceInfo = (XENGINE_DEVICEINFO*)lParam;
	CXEngineAPPJT1078Dlg* pClass_This = (CXEngineAPPJT1078Dlg*)pSt_DeviceInfo->pClass;
	CString m_StrSim;
	pClass_This->m_EditSim.GetWindowText(m_StrSim);

	int nSeq = 0;
	int nMsgLen = 0;
	FILE* pSt_File = _tfopen("D:\\XEngine_JT1078\\XEngine_APPClient\\Debug\\1.rtp", _T("wb"));
	TCHAR* ptszMsgBuffer = new TCHAR[1024 * 1024 * 10];
	TCHAR* ptszTmpBuffer = new TCHAR[1024 * 1024 * 10];

	while (pSt_DeviceInfo->bRun)
	{
		TCHAR tszRVBuffer[2048];
		memset(ptszTmpBuffer, '\0', 1024 * 1024 * 10);
		memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

		int nRet = fread(tszRVBuffer, 1, sizeof(tszRVBuffer), pSt_DeviceInfo->pSt_File);
		if (nRet <= 0)
		{
			fclose(pSt_DeviceInfo->pSt_File);
			CString m_StrAVFile;
			pClass_This->m_EditVideo.GetWindowText(m_StrAVFile);
			if (0 != _tfopen_s(&pSt_DeviceInfo->pSt_File, m_StrAVFile.GetBuffer(), _T("rb")))
			{
				break;
			}
		}
		if (nMsgLen > 0)
		{
			memcpy(ptszTmpBuffer, ptszMsgBuffer, nMsgLen);
			memcpy(ptszTmpBuffer + nMsgLen, tszRVBuffer, nRet);
		}
		else
		{
			memcpy(ptszTmpBuffer, tszRVBuffer, nRet);
		}
		nMsgLen += nRet;

		int nPos = 0;
		while (TRUE)
		{
			int nFIXLen = 0;
			int nNALLen = 0;
			if (!AVHelp_Parse_NaluHdr(ptszTmpBuffer + nPos, nMsgLen, &nNALLen, &nFIXLen))
			{
				break;
			}
			BOOL bFirst = TRUE;
			int nMsgCount = nNALLen;
			int nCpyCount = 0;
			int nUseCount = nPos;
			XENGINE_AVCODER_VIDEOFRAMETYPE enFrameType;

			AVHelp_Parse_H264NaluType(ptszTmpBuffer + nPos, nFIXLen, &enFrameType);
			while (nMsgCount > 0)
			{
				XENGINE_RTPPACKETHDR2014 st_RTPPacket;
				memset(&st_RTPPacket, '\0', sizeof(XENGINE_RTPPACKETHDR2014));

				st_RTPPacket.byV = 2;
				st_RTPPacket.byX = 0;
				st_RTPPacket.byCC = 1;
				st_RTPPacket.byM = 1;
				st_RTPPacket.byPT = 98;

				st_RTPPacket.byChannel = pSt_DeviceInfo->nChannel;
				st_RTPPacket.byType = 0;
				st_RTPPacket.byPacket = 0;

				if (nMsgCount > 950)
				{
					if (bFirst)
					{
						st_RTPPacket.byM = 0;
						st_RTPPacket.byPacket = 1;
						bFirst = FALSE;
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

				if (ENUM_NETENGINE_AVCODER_VIDEO_FRAMETYPE_P == enFrameType)
				{
					st_RTPPacket.byType = 1;
				}
				else
				{
					st_RTPPacket.byType = 0;
				}
				st_RTPPacket.wSerial = htons(nSeq++);
				st_RTPPacket.wLen = htons(nCpyCount);

				pClass_This->XEngine_Device_StrtoBCD(m_StrSim.GetBuffer(), st_RTPPacket.bySIMNumber);
				BaseLib_OperatorTime_SetXTPTime(&st_RTPPacket.ullTimestamp);

				fwrite(&st_RTPPacket, 1, sizeof(XENGINE_RTPPACKETHDR2014), pSt_File);
				fwrite(ptszTmpBuffer + nUseCount, 1, nCpyCount, pSt_File);
				//XClient_TCPSelect_SendMsg(pSt_DeviceInfo->hSocket, (const char*)tszSDBuffer, nPos);

				//Sleep(10);
				nUseCount += nCpyCount;
				nMsgCount -= nCpyCount;
			}
			nPos += nNALLen;
			nMsgLen -= nNALLen;
		}
		if (nMsgLen > 0)
		{
			memset(ptszMsgBuffer, '\0', 1024 * 1024 * 10);
			memcpy(ptszMsgBuffer, ptszTmpBuffer + nPos, nMsgLen);
		}
	}
	delete[] ptszMsgBuffer;
	delete[] ptszTmpBuffer;
	ptszMsgBuffer = NULL;
	ptszTmpBuffer = NULL;
	fclose(pSt_File);
	return 0;
}
DWORD WINAPI CXEngineAPPJT1078Dlg::XEngine_Device_Thread2016(LPVOID lParam)
{
	XENGINE_DEVICEINFO* pSt_DeviceInfo = (XENGINE_DEVICEINFO*)lParam;
	CXEngineAPPJT1078Dlg* pClass_This = (CXEngineAPPJT1078Dlg*)pSt_DeviceInfo->pClass;
	CString m_StrSim;
	pClass_This->m_EditSim.GetWindowText(m_StrSim);

	int nSeq = 0;
	int nMsgLen = 0;
	TCHAR* ptszMsgBuffer = new TCHAR[1024 * 1024 * 10];
	TCHAR* ptszTmpBuffer = new TCHAR[1024 * 1024 * 10];

	while (pSt_DeviceInfo->bRun)
	{
		TCHAR tszRVBuffer[2048];
		memset(ptszTmpBuffer, '\0', 1024 * 1024 * 10);
		memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

		int nRet = fread(tszRVBuffer, 1, sizeof(tszRVBuffer), pSt_DeviceInfo->pSt_File);
		if (nRet <= 0)
		{
			fclose(pSt_DeviceInfo->pSt_File);
			CString m_StrAVFile;
			pClass_This->m_EditVideo.GetWindowText(m_StrAVFile);
			if (0 != _tfopen_s(&pSt_DeviceInfo->pSt_File, m_StrAVFile.GetBuffer(), _T("rb")))
			{
				break;
			}
		}
		if (nMsgLen > 0)
		{
			memcpy(ptszTmpBuffer, ptszMsgBuffer, nMsgLen);
			memcpy(ptszTmpBuffer + nMsgLen, tszRVBuffer, nRet);
		}
		else
		{
			memcpy(ptszTmpBuffer, tszRVBuffer, nRet);
		}
		nMsgLen += nRet;

		int nPos = 0;
		while (TRUE)
		{
			int nFIXLen = 0;
			int nNALLen = 0;
			if (!AVHelp_Parse_NaluHdr(ptszTmpBuffer + nPos, nMsgLen, &nNALLen, &nFIXLen))
			{
				break;
			}
			BOOL bFirst = TRUE;
			int nMsgCount = nNALLen;
			int nCpyCount = 0;
			int nUseCount = nPos;
			XENGINE_AVCODER_VIDEOFRAMETYPE enFrameType;

			AVHelp_Parse_H264NaluType(ptszTmpBuffer + nPos, nFIXLen, &enFrameType);
			while (nMsgCount > 0)
			{
				XENGINE_RTPPACKETHDR2016 st_RTPPacket;
				XENGINE_RTPPACKETTAIL st_PacketTail;

				memset(&st_RTPPacket, '\0', sizeof(XENGINE_RTPPACKETHDR2016));
				memset(&st_PacketTail, '\0', sizeof(XENGINE_RTPPACKETTAIL));

				st_RTPPacket.byFlags[0] = 0x30;
				st_RTPPacket.byFlags[1] = 0x31;
				st_RTPPacket.byFlags[2] = 0x63;
				st_RTPPacket.byFlags[3] = 0x64;

				st_RTPPacket.byV = 2;
				st_RTPPacket.byX = 0;
				st_RTPPacket.byCC = 1;
				st_RTPPacket.byM = 1;
				st_RTPPacket.byPT = 98;

				st_RTPPacket.byChannel = pSt_DeviceInfo->nChannel;
				st_RTPPacket.byType = 0;
				st_RTPPacket.byPacket = 0;

				if (nMsgCount > 950)
				{
					if (bFirst)
					{
						st_RTPPacket.byM = 0;
						st_RTPPacket.byPacket = 1;
						bFirst = FALSE;
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

				if (ENUM_NETENGINE_AVCODER_VIDEO_FRAMETYPE_P == enFrameType)
				{
					st_RTPPacket.byType = 1;
				}
				else
				{
					st_RTPPacket.byType = 0;
				}

				st_RTPPacket.wSerial = htons(nSeq++);
				pClass_This->XEngine_Device_StrtoBCD(m_StrSim.GetBuffer(), st_RTPPacket.bySIMNumber);
				BaseLib_OperatorTime_SetXTPTime(&st_RTPPacket.ullTimestamp);

				int nPos = 0;
				TCHAR tszMsgBuffer[2048];
				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				memcpy(tszMsgBuffer, &st_RTPPacket, sizeof(XENGINE_RTPPACKETHDR2016));
				nPos += sizeof(XENGINE_RTPPACKETHDR2016);
				
				memcpy(tszMsgBuffer + nPos, &st_PacketTail, sizeof(XENGINE_RTPPACKETTAIL));
				nPos += sizeof(XENGINE_RTPPACKETTAIL);

				WORD wLen = htons(nCpyCount);
				memcpy(tszMsgBuffer + nPos, &wLen, sizeof(WORD));
				nPos += sizeof(WORD);

				XClient_TCPSelect_SendMsg(pSt_DeviceInfo->hSocket, tszMsgBuffer, nPos);
				XClient_TCPSelect_SendMsg(pSt_DeviceInfo->hSocket, ptszTmpBuffer + nUseCount, nCpyCount);
				Sleep(1);
				nUseCount += nCpyCount;
				nMsgCount -= nCpyCount;
			}
			nPos += nNALLen;
			nMsgLen -= nNALLen;
		}
		if (nMsgLen > 0)
		{
			memset(ptszMsgBuffer, '\0', 1024 * 1024 * 10);
			memcpy(ptszMsgBuffer, ptszTmpBuffer + nPos, nMsgLen);
		}
	}
	delete[] ptszMsgBuffer;
	delete[] ptszTmpBuffer;
	ptszMsgBuffer = NULL;
	ptszTmpBuffer = NULL;
	return 0;
}

void CXEngineAPPJT1078Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString gReadFilePathName;
	CFileDialog m_FileDlg(TRUE, _T("h264"), _T("*.h264"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("264 Files (*.264)|*.264|h264 File(*.h264)|*.h264|All File (*.*)|*.*||"), NULL);
	if (m_FileDlg.DoModal() == IDOK)
	{
		m_EditVideo.SetWindowText(m_FileDlg.GetPathName());
	}
}


void CXEngineAPPJT1078Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString gReadFilePathName;
	CFileDialog m_FileDlg(TRUE, _T("audio"), _T("*.*"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("g711a Files (*.g711a)|*.g711a|aac File(*.aac)|*.aac|All File (*.*)|*.*||"), NULL);
	if (m_FileDlg.DoModal() == IDOK)
	{
		m_EditAudio.SetWindowText(m_FileDlg.GetPathName());
	}
}



void CXEngineAPPJT1078Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_StrIPAddr;
	CString m_StrPort;
	CString m_StrCount;
	CString m_StrAVFile;

	m_EditVideo.GetWindowText(m_StrAVFile);
	m_EditIPAddr.GetWindowText(m_StrIPAddr);
	m_EditPort.GetWindowText(m_StrPort);
	m_EditCount.GetWindowText(m_StrCount);

	for (int i = 0; i < _ttoi(m_StrCount.GetBuffer()); i++)
	{
		XENGINE_DEVICEINFO* pSt_DeviceInfo = new XENGINE_DEVICEINFO;
		memset(pSt_DeviceInfo, '\0', sizeof(XENGINE_DEVICEINFO));

		pSt_DeviceInfo->nChannel = (i + 1);
		if (!XClient_TCPSelect_Create(&pSt_DeviceInfo->hSocket, m_StrIPAddr.GetBuffer(), _ttoi(m_StrPort.GetBuffer())))
		{
			AfxMessageBox(_T("连接失败"));
			return;
		}
		if (0 != _tfopen_s(&pSt_DeviceInfo->pSt_File, m_StrAVFile.GetBuffer(), _T("rb")))
		{
			AfxMessageBox(_T("文件打开失败!"));
			return;
		}
		AcquireSRWLockExclusive(&st_Locker);
		stl_MapDevice.insert(make_pair(pSt_DeviceInfo->hSocket, pSt_DeviceInfo));
		ReleaseSRWLockExclusive(&st_Locker);
	}
	m_BtnConnect.EnableWindow(FALSE);
	m_BtnClose.EnableWindow(TRUE);
	m_BtnSend.EnableWindow(TRUE);
}


void CXEngineAPPJT1078Dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BtnConnect.EnableWindow(FALSE);
	m_BtnClose.EnableWindow(TRUE);
	m_BtnSend.EnableWindow(FALSE);

	AcquireSRWLockShared(&st_Locker);
	unordered_map<SOCKET, XENGINE_DEVICEINFO*>::const_iterator stl_MapIterator = stl_MapDevice.begin();
	for (; stl_MapIterator != stl_MapDevice.end(); stl_MapIterator++)
	{
		stl_MapIterator->second->pClass = this;
		stl_MapIterator->second->bRun = TRUE;
		if (m_Radio2016.GetCheck())
		{
			stl_MapIterator->second->hThread = CreateThread(NULL, 0, XEngine_Device_Thread2016, stl_MapIterator->second, 0, NULL);
		}
		else
		{
			stl_MapIterator->second->hThread = CreateThread(NULL, 0, XEngine_Device_Thread2014, stl_MapIterator->second, 0, NULL);
		}
	}
	ReleaseSRWLockShared(&st_Locker);
}


void CXEngineAPPJT1078Dlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	AcquireSRWLockExclusive(&st_Locker);
	unordered_map<SOCKET, XENGINE_DEVICEINFO*>::iterator stl_MapIterator = stl_MapDevice.begin();
	for (; stl_MapIterator != stl_MapDevice.end(); stl_MapIterator++)
	{
		if (stl_MapIterator->second->bRun)
		{
			stl_MapIterator->second->bRun = FALSE;
			DWORD dwRet = WaitForSingleObject(stl_MapIterator->second->hThread, 1000);
			if (WAIT_OBJECT_0 != dwRet)
			{
				TerminateThread(stl_MapIterator->second->hThread, -1);
				CloseHandle(stl_MapIterator->second->hThread);
			}
		}
		if (NULL != stl_MapIterator->second->pSt_File)
		{
			fclose(stl_MapIterator->second->pSt_File);
			stl_MapIterator->second->pSt_File = NULL;
		}
		XClient_TCPSelect_Close(stl_MapIterator->second->hSocket);

		delete stl_MapIterator->second;
		stl_MapIterator->second = NULL;
	}
	stl_MapDevice.clear();
	ReleaseSRWLockExclusive(&st_Locker);

	m_BtnConnect.EnableWindow(TRUE);
	m_BtnClose.EnableWindow(FALSE);
	m_BtnSend.EnableWindow(FALSE);
}
