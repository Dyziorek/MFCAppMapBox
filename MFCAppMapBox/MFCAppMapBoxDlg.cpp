
// MFCAppMapBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCAppMapBox.h"
#include "MFCAppMapBoxDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include <memory>
#include <mbgl\platform\default\glfw_view.hpp>
#include <mbgl\storage\default_file_source.hpp>
#include <mbgl\storage\sqlite_cache.hpp>
#include <mbgl\platform\default_styles.hpp>
//#include <mbgl\platform\event.hpp>


std::unique_ptr<GLFWView> viewer;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCAppMapBoxDlg dialog



CMFCAppMapBoxDlg::CMFCAppMapBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCAppMapBoxDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAppMapBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCAppMapBoxDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMFCAppMapBoxDlg message handlers

BOOL CMFCAppMapBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	
	viewer = std::make_unique<GLFWView>();
	
	mbgl::DefaultFileSource fsrc(new mbgl::SQLiteCache("cache-db"));

		mbgl::Map mapWork(*viewer, fsrc);

		viewer->setChangeStyleCallback([&mapWork]() {
			static uint8_t currentStyleIndex;

			if (++currentStyleIndex == mbgl::util::defaultStyles.size()) {
				currentStyleIndex = 0;
			}

			const auto& newStyle = mbgl::util::defaultStyles[currentStyleIndex];
			mapWork.setStyleURL(newStyle.first);
			viewer->setWindowTitle(newStyle.second);

			//mbgl::Log::Info(mbgl::Event::Setup, std::string("Changed style to: ") + newStyle.first);
		});

		mapWork.setAccessToken("pk.eyJ1IjoiZHl6aW9yZWsiLCJhIjoiNmUxMjE0NzIwNjU4N2Y4NDEyNWI2M2FjYmYyMjBkNDEifQ.AnYZmHHGVWdW1u-dN7APrA");
		mapWork.setStyleURL("asset://styles\\mapbox-streets-v7.json");
		mapWork.setDebug(true);
		mapWork.setLatLngZoom(mbgl::LatLng(), 0);
		mapWork.setBearing(0);
		HWND windows = viewer->getWindowGLHandle();

		::SetParent(GetSafeHwnd(), windows);


		viewer->run();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCAppMapBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCAppMapBoxDlg::OnPaint()
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
HCURSOR CMFCAppMapBoxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

