
// CorrelatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Correlator.h"
#include "CorrelatorDlg.h"
#include "afxdialogex.h"
#include <string>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <thread>
#include "corrRun.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HINSTANCE hDLL;               // Handle to DLL
string strStatus = "DLL not loaded";
CONST string RUNS = "1";

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCorrelatorDlg dialog



CCorrelatorDlg::CCorrelatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CORRELATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCorrelatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCorrelatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCorrelatorDlg::OnBnClickedOk)
	ON_BN_CLICKED(btnPlot, &CCorrelatorDlg::OnBnClickedPlot)
	ON_BN_CLICKED(IDCANCEL, &CCorrelatorDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCorrelatorDlg message handlers

BOOL CCorrelatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CString defaultDLLStatus(strStatus.c_str());
	SetDlgItemText(txtDLLStatus, defaultDLLStatus);

	
	string strFilenname = "a.dll";
	CString Filename(strFilenname.c_str());
	hDLL = LoadLibrary(Filename);
	CWnd *runButton = GetDlgItem(IDOK);
	if (hDLL != NULL)
	{
		strStatus = "DLL loaded";
		CString cstrStatus(strStatus.c_str());
		SetDlgItemText(txtDLLStatus, cstrStatus);
		runButton->EnableWindow(TRUE);
		FreeLibrary(hDLL);
	}
	else
	{
		runButton->EnableWindow(FALSE);
	}

	CString cstrRuns(RUNS.c_str());
	SetDlgItemText(txtNoOfRuns, cstrRuns);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCorrelatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCorrelatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCorrelatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#define TIME_FLEX99O12B  0.1179648 // in seconds
#define FIRSTDELAY   1.25E-8	// Sample time in seconds
typedef BYTE(__cdecl* USBINITIALIZE)();       //Initialize the correlator, no argument.
											  // return true if successful.
typedef void(__cdecl* USBSTART)(BYTE automode);     //Start correlator,
													// automode: 1 auto correlate, 0 cross correlate
typedef BYTE(__cdecl* USBUPDATE)(float *ElapsedTime, unsigned short int *nTrace,
	float *corr, float *traceA, float *traceB);

typedef void(__cdecl* USBSTOP)(void);  // Stop the correlator.
typedef void(__cdecl* USBFREE)(void);  // Call this function to clean up when finished.


void function_1()
{
	#define TIME_FLEX99O12B  0.1179648 // in seconds
	#define FIRSTDELAY   1.25E-8	// Sample time in seconds
		typedef BYTE(__cdecl* USBINITIALIZE)();       //Initialize the correlator, no argument.
													  // return true if successful.
		typedef void(__cdecl* USBSTART)(BYTE automode);     //Start correlator,
															// automode: 1 auto correlate, 0 cross correlate
		typedef BYTE(__cdecl* USBUPDATE)(float *ElapsedTime, unsigned short int *nTrace,
			float *corr, float *traceA, float *traceB);

		typedef void(__cdecl* USBSTOP)(void);  // Stop the correlator.
		typedef void(__cdecl* USBFREE)(void);  // Call this function to clean up when finished.

		int					i, j, k;
		float				ElapsedTime;
		unsigned short int	tracecnt;
		float				corr[288];
		float				DelayTime[288];
		float				traceA[512];
		float				traceB[512];
		double				IntensityA, IntensityB;
		int                             mnint;
		FILE				*stream;
		char				filename[256], filenumber[5];
		HINSTANCE hDLL;               // Handle to DLL
		USBSTART Start;    // Function pointer
		USBSTOP Stop;    // Function pointer
		USBINITIALIZE Initialize;    // Function pointer
		USBUPDATE Update;    // Function pointer
		USBFREE Usbfree;    // Function pointer

		BYTE mode;
		int runNumber;

		int	DurationTime = 0;

		CString textInput;

		//GetDlgItemText(etxt_duration, textInput);
		//DurationTime = _ttoi(textInput);

		DurationTime = 10;

		if (DurationTime == 0)
		{
			strStatus = "Invalid duration time";
			CString cstrStatus(strStatus.c_str());
			return;
		}

		runNumber = 1;

		// Auto mode
		mode = 0;

		// Calculate the delay times
		for (i = 0; i<16; i++)
			DelayTime[i] = (i + 1)*FIRSTDELAY;
		for (j = 1; j<35; j++)
			for (i = 0; i<8; i++)
				DelayTime[i + (j - 1) * 8 + 16] = DelayTime[(j - 1) * 8 + 16 + i - 1] + FIRSTDELAY * (float)pow(2, j);

		string strFilenname = "a.dll";
		CString Filename(strFilenname.c_str());
		hDLL = LoadLibrary(Filename);
		Initialize = (USBINITIALIZE)GetProcAddress(hDLL, "_USBInitialize");

		strStatus = "Wait...";
		CString cstrStatus(strStatus.c_str());

		// Test the presence of the correlator
		if (Initialize())
		{
			strStatus = "The card is present";
			CString cstrStatus(strStatus.c_str());
		}
		else
		{
			strStatus = "The card is not present";
			CString cstrStatus(strStatus.c_str());
			FreeLibrary(hDLL);
			return;
		}

		if (!Initialize)
		{
			// handle the error
			strStatus = "Initialization failed";
			CString cstrStatus(strStatus.c_str());
			FreeLibrary(hDLL);
			return;
		}

		Start = (USBSTART)GetProcAddress(hDLL, "_USBStart");
		if (!Start)
		{
			// handle the error
			strStatus = "Start command error";
			CString cstrStatus(strStatus.c_str());
			FreeLibrary(hDLL);
			return;
		}

		Stop = (USBSTOP)GetProcAddress(hDLL, "_USBStop");
		if (!Stop)
		{
			// handle the error
			strStatus = "Stop command error";
			CString cstrStatus(strStatus.c_str());
			FreeLibrary(hDLL);
			return;
		}

		Update = (USBUPDATE)GetProcAddress(hDLL, "_USBUpdate");
		if (!Update)
		{
			// handle the error
			strStatus = "Update command error";
			CString cstrStatus(strStatus.c_str());
			FreeLibrary(hDLL);
			return;
		}

		Usbfree = (USBFREE)GetProcAddress(hDLL, "_USBFree");
		if (!Usbfree)
		{
			// handle the error
			strStatus = "Usbfree command error";
			CString cstrStatus(strStatus.c_str());
			FreeLibrary(hDLL);
			return;
		}
		else
		{
			strStatus = "Usbfree successful";
			CString cstrStatus(strStatus.c_str());
		}

		mnint = 0;
		strcpy(filename, "intensity");
		itoa(0, filenumber, 10);
		strcat(filename, filenumber);
		strcat(filename, ".dat");
		stream = fopen(filename, "wt");
		Start(mode);
		ElapsedTime = 0;

		while (ElapsedTime < DurationTime)
		{
			// Sleep for a second
			Sleep(1000);

			// Hello correlator
			if (Update(&ElapsedTime, &tracecnt, corr, traceA, traceB) == 0)
			{
				strStatus = "Fatal application error";
				CString cstrStatus(strStatus.c_str());
				Usbfree();
				FreeLibrary(hDLL);
				return;
			}

			// Calculate the average intensity for channel A
			IntensityA = 0;
			IntensityB = 0;

			for (i = 0; i<tracecnt; i++)
			{
				IntensityA += traceA[i];
				IntensityB += traceB[i];
				fprintf(stream, "%e,%e,%e\n", mnint*TIME_FLEX99O12B, traceA[i] / TIME_FLEX99O12B, traceB[i] / TIME_FLEX99O12B);
				mnint++;
			}

			IntensityA /= tracecnt * TIME_FLEX99O12B;
			IntensityB /= tracecnt * TIME_FLEX99O12B;

			// Display it
			strStatus = "Intensities: " + to_string(IntensityA) + "," + to_string(IntensityB);
			strStatus = "Run successful";
			CString cstrStatus(strStatus.c_str());
		}
		fclose(stream);

		//stops the correlator
		Stop();

		// Sleep for sometimes
		Sleep(30);

		// Write corr file
		strcpy(filename, "corr");
		itoa(0, filenumber, 10);
		strcat(filename, filenumber);
		strcat(filename, ".dat");
		stream = fopen(filename, "wt");
		for (i = 0; i<288; i++)
			fprintf(stream, "%e,%e\n", DelayTime[i], corr[i]);
		fclose(stream);
		return;
}


void CCorrelatorDlg::OnBnClickedOk()
{
	std::thread t1(function_1);
	t1.detach();

	// THIS IS A NEW LINE TO TEST SYNC
	// THIS IS SECOND LINE TO TEST SYNC
	return;
}


void CCorrelatorDlg::OnBnClickedPlot()
{
	string strOpen = "open";
	CString open(strOpen.c_str());
	string strPath = "E:\\corr_eng\\new.exe";
	CString path(strPath.c_str());
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, open, path, NULL, NULL, SW_SHOWDEFAULT);
	strStatus = "Success";
	CString cstrStatus(strStatus.c_str());
	SetDlgItemText(txtDLLStatus, cstrStatus);
}

void CCorrelatorDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if (hDLL != NULL)
	{
		FreeLibrary(hDLL);
	}
	CDialogEx::OnCancel();
}
