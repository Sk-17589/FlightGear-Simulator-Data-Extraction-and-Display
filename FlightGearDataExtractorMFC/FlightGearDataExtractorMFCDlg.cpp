// FlightGearDataExtractorMFCDlg.cpp : implementation file
//

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "pch.h"
#include "framework.h"
#include "FlightGearDataExtractorMFC.h"
#include "FlightGearDataExtractorMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

// CFlightGearDataExtractorMFCDlg dialog
CFlightGearDataExtractorMFCDlg::CFlightGearDataExtractorMFCDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_FLIGHTGEARDATAEXTRACTORMFC_DIALOG, pParent), m_nTimer(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFlightGearDataExtractorMFCDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PITCH_EDIT, m_PitchEdit);
    DDX_Control(pDX, IDC_YAW_EDIT, m_GroundElevationEdit);
    DDX_Control(pDX, IDC_ROLL_EDIT, m_RollEdit);
    DDX_Control(pDX, IDC_SPEED_EDIT, m_SpeedEdit);
    DDX_Control(pDX, IDC_ALTITUDE_EDIT, m_AltitudeEdit);
    DDX_Control(pDX, IDC_LONGITUDE_EDIT, m_LongitudeEdit);
    DDX_Control(pDX, IDC_LATITUDE_EDIT, m_LatitudeEdit);
    DDX_Control(pDX, IDC_AIRSPEED_EDIT, m_AirspeedEdit);
}

BEGIN_MESSAGE_MAP(CFlightGearDataExtractorMFCDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_REFRESH_BUTTON, &CFlightGearDataExtractorMFCDlg::OnBnClickedRefreshButton)
    ON_EN_CHANGE(IDC_PITCH_EDIT, &CFlightGearDataExtractorMFCDlg::OnEnChangePitchEdit)
    ON_EN_CHANGE(IDC_YAW_EDIT, &CFlightGearDataExtractorMFCDlg::OnEnChangeYawEdit)
    ON_EN_CHANGE(IDC_DATA_EDIT, &CFlightGearDataExtractorMFCDlg::OnEnChangeDataEdit)
    ON_EN_CHANGE(IDC_ROLL_EDIT, &CFlightGearDataExtractorMFCDlg::OnEnChangeRollEdit)
    ON_EN_CHANGE(IDC_SPEED_EDIT, &CFlightGearDataExtractorMFCDlg::OnEnChangeSpeedEdit)
    ON_EN_CHANGE(IDC_AIRSPEED_EDIT, &CFlightGearDataExtractorMFCDlg::OnEnChangeAirspeedEdit)
    ON_EN_CHANGE(IDC_LONGITUDE_EDIT, &CFlightGearDataExtractorMFCDlg::OnEnChangeLongitudeEdit)
END_MESSAGE_MAP()

BOOL CFlightGearDataExtractorMFCDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

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

    m_nTimer = SetTimer(1, 1000, nullptr);

    return TRUE;
}

void CFlightGearDataExtractorMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFlightGearDataExtractorMFCDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CFlightGearDataExtractorMFCDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

size_t CFlightGearDataExtractorMFCDlg::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void CFlightGearDataExtractorMFCDlg::FetchData(const std::string& url, CEdit& editControl)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            AfxMessageBox(CString("curl_easy_perform() failed: ") + curl_easy_strerror(res));
        }
        else {
            try {
                auto json = nlohmann::json::parse(readBuffer);
                CString strData;

                // Check if the value is not null before parsing
                if (!json["value"].is_null()) {
                    strData.Format(_T("%f"), json["value"].get<double>());
                }
                else {
                    strData = _T("N/A");  // Set to "N/A" or some default value
                }

                editControl.SetWindowText(strData);
            }
            catch (nlohmann::json::exception& e) {
                AfxMessageBox(CString("JSON parse error: ") + e.what());
            }
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void CFlightGearDataExtractorMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == m_nTimer) {
        FetchData(pitchURL, m_PitchEdit);
        FetchData(groundelevURL, m_GroundElevationEdit);
        FetchData(rollURL, m_RollEdit);
        FetchData(speedURL, m_SpeedEdit);
        FetchData(altitudeURL, m_AltitudeEdit);
        FetchData(longitudeURL, m_LongitudeEdit);
        FetchData(latitudeURL, m_LatitudeEdit);
        FetchData(airspeedURL, m_AirspeedEdit);
    }

    CDialogEx::OnTimer(nIDEvent);
}

void CFlightGearDataExtractorMFCDlg::OnBnClickedRefreshButton()
{
    FetchData(pitchURL, m_PitchEdit);
    FetchData(groundelevURL, m_GroundElevationEdit);
    FetchData(rollURL, m_RollEdit);
    FetchData(speedURL, m_SpeedEdit);
    FetchData(altitudeURL, m_AltitudeEdit);
    FetchData(longitudeURL, m_LongitudeEdit);
    FetchData(latitudeURL, m_LatitudeEdit);
    FetchData(airspeedURL, m_AirspeedEdit);
}


void CFlightGearDataExtractorMFCDlg::OnEnChangePitchEdit()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}


void CFlightGearDataExtractorMFCDlg::OnEnChangeYawEdit()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}


void CFlightGearDataExtractorMFCDlg::OnEnChangeDataEdit()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}


void CFlightGearDataExtractorMFCDlg::OnEnChangeRollEdit()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}


void CFlightGearDataExtractorMFCDlg::OnEnChangeSpeedEdit()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}


void CFlightGearDataExtractorMFCDlg::OnEnChangeAirspeedEdit()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}


void CFlightGearDataExtractorMFCDlg::OnEnChangeLongitudeEdit()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}
