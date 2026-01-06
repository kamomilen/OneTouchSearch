#include "stdafx.h"
#include "CSettingsDialog.h"
#include "afxdialogex.h"
#include "resource.h"
#include "WinUser.h"
#include "Regedit.h"

IMPLEMENT_DYNAMIC(CSettingsDialog, CDialog)

CSettingsDialog::CSettingsDialog(CWnd* pParent /*=nullptr*/)
    : CDialog(IDD_SETTINGS_DIALOG, pParent)
{
    //memset(m_bCheck, 0, sizeof(m_bCheck));
    //for (int i = 0; i < 5; ++i) {
    //    m_strEdit1[i] = _T("");
    //    m_strEdit2[i] = _T("");
    //}
}

void CSettingsDialog::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHECK0, m_check0);
    DDX_Control(pDX, IDC_EDIT1_0, m_edit1_0);
    DDX_Control(pDX, IDC_EDIT2_0, m_edit2_0);
    DDX_Control(pDX, IDC_CHECK1, m_check1);
    DDX_Control(pDX, IDC_EDIT1_1, m_edit1_1);
    DDX_Control(pDX, IDC_EDIT2_1, m_edit2_1);
    DDX_Control(pDX, IDC_CHECK2, m_check2);
    DDX_Control(pDX, IDC_EDIT1_2, m_edit1_2);
    DDX_Control(pDX, IDC_EDIT2_2, m_edit2_2);
    DDX_Control(pDX, IDC_CHECK3, m_check3);
    DDX_Control(pDX, IDC_EDIT1_3, m_edit1_3);
    DDX_Control(pDX, IDC_EDIT2_3, m_edit2_3);
    DDX_Control(pDX, IDC_CHECK4, m_check4);
    DDX_Control(pDX, IDC_EDIT1_4, m_edit1_4);
    DDX_Control(pDX, IDC_EDIT2_4, m_edit2_4);


}

BEGIN_MESSAGE_MAP(CSettingsDialog, CDialog)
    ON_BN_CLICKED(IDOK, &CSettingsDialog::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CSettingsDialog::OnInitDialog() {
    CDialog::OnInitDialog();

    // レジストリから読み込み
	Regedit regedit;
    regedit.readEnable(OTS_REG_ENABLE_K, m_bCheck[0]);
    regedit.readEnable(OTS_REG_ENABLE_1, m_bCheck[1]);
	regedit.readEnable(OTS_REG_ENABLE_2, m_bCheck[2]);
	regedit.readEnable(OTS_REG_ENABLE_3, m_bCheck[3]);
	regedit.readEnable(OTS_REG_ENABLE_4, m_bCheck[4]);


    regedit.readHotkey(OTS_REG_VALUE_HOTKEY_K, m_strEdit1[0]);
    regedit.readHotkey(OTS_REG_VALUE_HOTKEY_1, m_strEdit1[1]);
    regedit.readHotkey(OTS_REG_VALUE_HOTKEY_2, m_strEdit1[2]);
    regedit.readHotkey(OTS_REG_VALUE_HOTKEY_3, m_strEdit1[3]);
    regedit.readHotkey(OTS_REG_VALUE_HOTKEY_4, m_strEdit1[4]);

    regedit.readURL(OTS_REG_VALUE_URL_K, m_strEdit2[0]); ;
    regedit.readURL(OTS_REG_VALUE_URL_1, m_strEdit2[1]);
	regedit.readURL(OTS_REG_VALUE_URL_2, m_strEdit2[2]);
	regedit.readURL(OTS_REG_VALUE_URL_3, m_strEdit2[3]);
	regedit.readURL(OTS_REG_VALUE_URL_4, m_strEdit2[4]);

    // コントロール初期化
    m_check0.SetCheck(m_bCheck ? BST_CHECKED : BST_UNCHECKED);
	m_edit1_0.SetWindowTextW(m_strEdit1[0]);
	m_edit2_0.SetWindowTextW(m_strEdit2[0]);
	m_check1.SetCheck(m_bCheck[1] ? BST_CHECKED : BST_UNCHECKED);
	m_edit1_1.SetWindowTextW(m_strEdit1[1]);
	m_edit2_1.SetWindowTextW(m_strEdit2[1]);
	m_check2.SetCheck(m_bCheck[2] ? BST_CHECKED : BST_UNCHECKED);
	m_edit1_2.SetWindowTextW(m_strEdit1[2]);
	m_edit2_2.SetWindowTextW(m_strEdit2[2]);
	m_check3.SetCheck(m_bCheck[3] ? BST_CHECKED : BST_UNCHECKED);
	m_edit1_3.SetWindowTextW(m_strEdit1[3]);
	m_edit2_3.SetWindowTextW(m_strEdit2[3]);
	m_check4.SetCheck(m_bCheck[4] ? BST_CHECKED : BST_UNCHECKED);
	m_edit1_4.SetWindowTextW(m_strEdit1[4]);
	m_edit2_4.SetWindowTextW(m_strEdit2[4]);

    return TRUE;
}

void CSettingsDialog::OnBnClickedOk() {
    // 設定値取得
    m_bCheck[0] = (m_check0.GetCheck() == BST_CHECKED);
    m_edit1_0.GetWindowTextW(m_strEdit1[0]);
    m_edit1_0.GetWindowTextW(m_strEdit1[0]);
    m_edit2_0.GetWindowTextW(m_strEdit2[0]);
    m_bCheck[1] = (m_check1.GetCheck() == BST_CHECKED);
    m_edit1_1.GetWindowTextW(m_strEdit1[1]);
    m_edit2_1.GetWindowTextW(m_strEdit2[1]);
    m_bCheck[2] = (m_check2.GetCheck() == BST_CHECKED);
    m_edit1_2.GetWindowTextW(m_strEdit1[2]);
    m_edit2_2.GetWindowTextW(m_strEdit2[2]);
    m_bCheck[3] = (m_check3.GetCheck() == BST_CHECKED);
    m_edit1_3.GetWindowTextW(m_strEdit1[3]);
    m_edit2_3.GetWindowTextW(m_strEdit2[3]);
    m_bCheck[4] = (m_check4.GetCheck() == BST_CHECKED);
    m_edit1_4.GetWindowTextW(m_strEdit1[4]);
    m_edit2_4.GetWindowTextW(m_strEdit2[4]);

    // ここでレジストリ保存等を実行
    Regedit regedit;
	regedit.saveEnable(OTS_REG_ENABLE_K, m_bCheck[0]);
	regedit.saveEnable(OTS_REG_ENABLE_1, m_bCheck[1]);
	regedit.saveEnable(OTS_REG_ENABLE_2, m_bCheck[2]);
	regedit.saveEnable(OTS_REG_ENABLE_3, m_bCheck[3]);
	regedit.saveEnable(OTS_REG_ENABLE_4, m_bCheck[4]);
	regedit.saveHotkey(OTS_REG_VALUE_HOTKEY_K, m_strEdit1[0]);
	regedit.saveHotkey(OTS_REG_VALUE_HOTKEY_1, m_strEdit1[1]);
	regedit.saveHotkey(OTS_REG_VALUE_HOTKEY_2, m_strEdit1[2]);
	regedit.saveHotkey(OTS_REG_VALUE_HOTKEY_3, m_strEdit1[3]);
	regedit.saveHotkey(OTS_REG_VALUE_HOTKEY_4, m_strEdit1[4]);
	regedit.saveURL(OTS_REG_VALUE_URL_K, m_strEdit2[0].GetString());
	regedit.saveURL(OTS_REG_VALUE_URL_1, m_strEdit2[1].GetString());
	regedit.saveURL(OTS_REG_VALUE_URL_2, m_strEdit2[2].GetString());
	regedit.saveURL(OTS_REG_VALUE_URL_3, m_strEdit2[3].GetString());
	regedit.saveURL(OTS_REG_VALUE_URL_4, m_strEdit2[4].GetString());

    CDialog::OnOK();
}
