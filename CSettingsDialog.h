#pragma once
#include "afxwin.h"
#include "resource.h"

class CSettingsDialog : public CDialog
{
	DECLARE_DYNAMIC(CSettingsDialog)

public:
    CSettingsDialog(CWnd* pParent = nullptr);
    enum { IDD = IDD_SETTINGS_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

public:
    afx_msg void OnBnClickedOk();

    CButton m_check0, m_check1, m_check2, m_check3, m_check4;
    CEdit m_edit1_0, m_edit1_1, m_edit1_2, m_edit1_3, m_edit1_4;
    CEdit m_edit2_0, m_edit2_1, m_edit2_2, m_edit2_3, m_edit2_4;

    // ê›íËílï€ë∂óp
    bool m_bCheck[5] = { false };
    CString m_strEdit1[5] = { _T("") };
    CString m_strEdit2[5] = { _T("") };

    DECLARE_MESSAGE_MAP()
};

