#pragma once

#include <afxwin.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>

class CFlightGearDataExtractorMFCDlg : public CDialogEx
{
public:
    CFlightGearDataExtractorMFCDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_FLIGHTGEARDATAEXTRACTORMFC_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    HICON m_hIcon;
    CEdit m_PitchEdit;
    CEdit m_GroundElevationEdit;
    CEdit m_RollEdit;
    CEdit m_SpeedEdit;
    CEdit m_AltitudeEdit;
    CEdit m_LongitudeEdit;
    CEdit m_LatitudeEdit;
    CEdit m_AirspeedEdit;

    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedRefreshButton();
    DECLARE_MESSAGE_MAP()

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    void FetchData(const std::string& url, CEdit& editControl);
    //void DisplayData(CEdit& editCtrl, const std::string& url);
    UINT_PTR m_nTimer;

    const std::string pitchURL = "http://localhost:5500/json/orientation/pitch-deg";
    const std::string groundelevURL = "http://localhost:5500/json/position/ground-elev-m";
    const std::string rollURL = "http://localhost:5500/json/orientation/roll-deg";
    const std::string speedURL = "http://localhost:5500/json/velocities/mach";
    const std::string altitudeURL = "http://localhost:5500/json/position/altitude-ft";
    const std::string longitudeURL = "http://localhost:5500/json/position/longitude-deg";
    const std::string latitudeURL = "http://localhost:5500/json/position/latitude-deg";
    const std::string airspeedURL = "http://localhost:5500/json/velocities/airspeed-kt";
public:
    afx_msg void OnEnChangePitchEdit();
    afx_msg void OnEnChangeYawEdit();
    afx_msg void OnEnChangeDataEdit();
    afx_msg void OnEnChangeRollEdit();
    afx_msg void OnEnChangeSpeedEdit();
    afx_msg void OnEnChangeAirspeedEdit();
    afx_msg void OnEnChangeLongitudeEdit();
};
