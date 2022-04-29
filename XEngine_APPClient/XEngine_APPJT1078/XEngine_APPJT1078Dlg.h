
// XEngine_APPJT1078Dlg.h: 头文件
//

#pragma once

typedef struct
{
	SOCKET hSocket;
	HANDLE hThread;
	int nChannel;
	BOOL bRun;

	FILE* pSt_File;
	LPVOID pClass;
}XENGINE_DEVICEINFO;
// CXEngineAPPJT1078Dlg 对话框
class CXEngineAPPJT1078Dlg : public CDialogEx
{
// 构造
public:
	CXEngineAPPJT1078Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XENGINE_APPJT1078_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	SRWLOCK st_Locker;
	unordered_map<SOCKET, XENGINE_DEVICEINFO*> stl_MapDevice;

	void XEngine_Device_StrtoBCD(LPCTSTR lpszPhoneCode, BYTE* ptszBCD);
	static DWORD WINAPI XEngine_Device_Thread2014(LPVOID lParam);
	static DWORD WINAPI XEngine_Device_Thread2016(LPVOID lParam);
public:
	CIPAddressCtrl m_EditIPAddr;
	CEdit m_EditPort;
	CEdit m_EditCount;
	CEdit m_EditSim;
	CEdit m_EditVideo;
	CEdit m_EditAudio;
	CButton m_Radio2016;
	CButton m_Radio2014;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CButton m_BtnConnect;
	CButton m_BtnSend;
	CButton m_BtnClose;
};
