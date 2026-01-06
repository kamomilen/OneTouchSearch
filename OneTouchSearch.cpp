#include "stdafx.h"
#include "resource.h"
#include <vector>
#include "CSettingsDialog.h"
#include "Regedit.h"
#pragma comment(lib,"Version.lib")

// Define handles
HINSTANCE g_hInstance = (HINSTANCE)GetModuleHandle(NULL);
HWND g_hMainWnd = NULL;

// Define tray icon properties
// TODO: The tray icon class used doesn't support Unicode
CTrayIcon g_TrayIconOTS("One Touch Search", true, LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_ONETOUCHSEARCH)));

// Define OneTouchSearch function (implemented in OneTouchSearch.cpp)
bool oneTouchSearch(const wchar_t* search_engine_url);

// Get app version info from resource
bool GetAppVersion(CStringW &ver) {
	// Get the filename of the app
	TCHAR szFileName[MAX_PATH + 1] = {0};
	if (GetModuleFileName(NULL, szFileName, MAX_PATH) == 0)
		return false;

	// Get size of version resource
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(szFileName, &dwHandle);
	if (dwSize == 0)
		return false;

	// Get version info
	std::vector<BYTE> data(dwSize);
	if (!GetFileVersionInfo(szFileName, NULL, dwSize, &data[0]))
		return false;

	// Get the language-independent resource
	UINT uiVerLen = 0;
	VS_FIXEDFILEINFO* pFixedInfo = 0;
	if (VerQueryValue(&data[0], _T("\\"), (void**)&pFixedInfo, (UINT *)&uiVerLen) == 0)
		return false;

	// Return the string
	ver.Format(_T("%u.%u.%u"),
		HIWORD (pFixedInfo->dwProductVersionMS),
		LOWORD (pFixedInfo->dwProductVersionMS),
		HIWORD (pFixedInfo->dwProductVersionLS),
		LOWORD (pFixedInfo->dwProductVersionLS));
	return true;
}

// Handle tray icon events
void g_TrayIconOTS_OnMessage(CTrayIcon* pTrayIcon, UINT uMsg)
{
	switch (uMsg) {
		case WM_LBUTTONUP:
			// Disable left click or double click (no Windows will be visible)
			// TODO: Add configuration dialog on double click, to specify
			//       search engine to open and hotkey to use
			//       https://docs.microsoft.com/en-us/windows/win32/controls/create-a-hot-key-control
			break;
		case WM_RBUTTONUP:
			{
				POINT pt;
				if (GetCursorPos(&pt))
				{
					// Create tray popup menu
					HMENU menu = CreatePopupMenu();
					AppendMenu(menu, MF_STRING, 3, _T("Settings"));
					AppendMenu(menu, MF_STRING, 1, _T("About"));
					AppendMenu(menu, MF_SEPARATOR, 2, NULL);;
					AppendMenu(menu, MF_STRING, 2, _T("Exit"));

					// Wait popup menu click
					UINT cmd = TrackPopupMenu(menu, TPM_RETURNCMD|TPM_RIGHTBUTTON, pt.x, pt.y, 0, g_hMainWnd, NULL);
					if (cmd == 2)
						// Exit
						PostMessage(g_hMainWnd, WM_CLOSE, 0, 0);
					else if (cmd == 1) {
						// About
						CStringW appVer;
						std::wstring aboutMessage;
						aboutMessage.append(_T("One Touch Search(UTF-16 support)"));
						if (GetAppVersion(appVer)) {
							aboutMessage.append(_T(" v"));
							aboutMessage.append(appVer);
						}
						aboutMessage.append(_T("\n\nAllows to open the default web browser and search the currently selected text in any program when pressing CTRL+ALT+SHIFT+K (default hotkey).\n\nBind this shortcut to one of your mouse buttons with Logitech Options and you'll get the old OneTouchSearch feature back! Consider also running this program as Administrator to catch the input in both admin and non-admin apps."));
						MessageBox(g_hMainWnd, aboutMessage.c_str(), _T("One Touch Search"), MB_ICONINFORMATION | MB_OK);
					}
					else if (cmd == 3) {
						// Settings
						CSettingsDialog dlg;
						if (dlg.DoModal() == IDOK) {
							CStringW appVer;
							std::wstring aboutMessage;
							aboutMessage.append(_T("The registry has been set.\n\nplease restart the app."));
							MessageBox(g_hMainWnd, aboutMessage.c_str(), _T("One Touch Search"), MB_ICONINFORMATION | MB_OK);
							PostMessage(g_hMainWnd, WM_CLOSE, 0, 0);
						}
					}
				}
			}
			break;
	}
}

// Handle tray icon messages
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		case WM_DESTROY:
			// Quit the application
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Create the message-only window
bool CreateMainHiddenWnd() {
	
	static const wchar_t CLASS_NAME[] = _T("OneTouchSearchWndClass");

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = g_hInstance;
	wc.lpfnWndProc = &MainWndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	if (!RegisterClass(&wc))
		return false;

	g_hMainWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		_T("One Touch Search"),
		WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
		HWND_MESSAGE, // message-only
		NULL, g_hInstance, NULL);

	return true;
}

	// Main entry point
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Create a message-only window to handle the tray icon
	if (!CreateMainHiddenWnd())
		return -1;

	Regedit regedit;

	bool enable_K, enable_1, enable_2, enable_3, enable_4;
	std::wstring searchEngineURL_K, searchEngineURL_1, searchEngineURL_2, searchEngineURL_3, searchEngineURL_4;
	UINT hkModifiers_K, hkKey_K, hkModifiers_1, hkKey_1, hkModifiers_2, hkKey_2, hkModifiers_3, hkKey_3, hkModifiers_4, hkKey_4;

	// Kホットキー(Enable)
	if (!regedit.readEnable(OTS_REG_ENABLE_K, enable_K)) {
		enable_K = true;
		regedit.saveEnable(OTS_REG_ENABLE_K, enable_K);
	}
	// 1ホットキー(Enable) Option
	if (!regedit.readEnable(OTS_REG_ENABLE_1, enable_1)) {
		enable_1 = false;
		regedit.saveEnable(OTS_REG_ENABLE_1, enable_1);
	}
	// 2ホットキー(Enable) Option
	if (!regedit.readEnable(OTS_REG_ENABLE_2, enable_2)) {
		enable_2 = false;
		regedit.saveEnable(OTS_REG_ENABLE_2, enable_2);
	}
	// 3ホットキー(Enable) Option
	if (!regedit.readEnable(OTS_REG_ENABLE_3, enable_3)) {
		enable_3 = false;
		regedit.saveEnable(OTS_REG_ENABLE_3, enable_3);
	}
	// 4ホットキー(Enable) Option
	if (!regedit.readEnable(OTS_REG_ENABLE_4, enable_4)) {
		enable_4 = false;
		regedit.saveEnable(OTS_REG_ENABLE_4, enable_4);
	}

	// Kホットキー設定読み込み(URL)
	if (!regedit.readURL(OTS_REG_VALUE_URL_K, searchEngineURL_K)) {
		searchEngineURL_K = L"https://www.google.com/search?q=";  // デフォルトGoogle検索
		regedit.saveURL(OTS_REG_VALUE_URL_K, searchEngineURL_K);
	}

	// 1ホットキー設定読み込み(URL)
	if (!regedit.readURL(OTS_REG_VALUE_URL_1, searchEngineURL_1)) {
		searchEngineURL_1 = L"http://www.google.com/search?num=50&lr=lang_ja&q=";  // 日本語版Google検索(日本語のみ)
		regedit.saveURL(OTS_REG_VALUE_URL_1, searchEngineURL_1);
	}

	// 2ホットキー設定読み込み(URL)
	if (!regedit.readURL(OTS_REG_VALUE_URL_2, searchEngineURL_2)) {
		searchEngineURL_2 = L"https://x.com/search?q=";  // Xで検索
		regedit.saveURL(OTS_REG_VALUE_URL_2, searchEngineURL_2);
	}

	// 3ホットキー設定読み込み(URL)
	if (!regedit.readURL(OTS_REG_VALUE_URL_3, searchEngineURL_3)) {
		searchEngineURL_3 = L"https://ja.wikipedia.org/w/index.php?search=";  // Wikipedia（日本）検索
		regedit.saveURL(OTS_REG_VALUE_URL_3, searchEngineURL_3);
	}

	// 4ホットキー設定読み込み(URL)
	if (!regedit.readURL(OTS_REG_VALUE_URL_4, searchEngineURL_4)) {
		searchEngineURL_4 = L"https://www.amazon.co.jp/s?k=";  // amazon（日本）で検索
		regedit.saveURL(OTS_REG_VALUE_URL_4, searchEngineURL_4);
	}

	// Kホットキー（デフォルト: CTRL+ALT+SHIFT+K）
	if (!regedit.readHotkey(OTS_REG_VALUE_HOTKEY_K, hkModifiers_K, hkKey_K)) {
		hkModifiers_K = MOD_CONTROL | MOD_ALT | MOD_SHIFT;
		hkKey_K = _T('K');
		regedit.saveHotkey(OTS_REG_VALUE_HOTKEY_K, hkModifiers_K, hkKey_K);
	}

	// 1ホットキー（デフォルト: CTRL+ALT+SHIFT+1）
	if (!regedit.readHotkey(OTS_REG_VALUE_HOTKEY_1, hkModifiers_1, hkKey_1)) {
		hkModifiers_1 = MOD_CONTROL | MOD_ALT | MOD_SHIFT;
		hkKey_1 = _T('1');
		regedit.saveHotkey(OTS_REG_VALUE_HOTKEY_1, hkModifiers_1, hkKey_1);
	}

	// 2ホットキー（デフォルト: CTRL+ALT+SHIFT+2）
	if (!regedit.readHotkey(OTS_REG_VALUE_HOTKEY_2, hkModifiers_2, hkKey_2)) {
		hkModifiers_2 = MOD_CONTROL | MOD_ALT | MOD_SHIFT;
		hkKey_2 = _T('2');
		regedit.saveHotkey(OTS_REG_VALUE_HOTKEY_2, hkModifiers_2, hkKey_2);
	}

	// 3ホットキー（デフォルト: CTRL+ALT+SHIFT+3）
	if (!regedit.readHotkey(OTS_REG_VALUE_HOTKEY_3, hkModifiers_3, hkKey_3)) {
		hkModifiers_3 = MOD_CONTROL | MOD_ALT | MOD_SHIFT;
		hkKey_3 = _T('3');
		regedit.saveHotkey(OTS_REG_VALUE_HOTKEY_3, hkModifiers_3, hkKey_3);
	}

	// 4ホットキー（デフォルト: CTRL+ALT+SHIFT+4）
	if (!regedit.readHotkey(OTS_REG_VALUE_HOTKEY_4, hkModifiers_4, hkKey_4)) {
		hkModifiers_4 = MOD_CONTROL | MOD_ALT | MOD_SHIFT;
		hkKey_4 = _T('4');
		regedit.saveHotkey(OTS_REG_VALUE_HOTKEY_4, hkModifiers_4, hkKey_4);
	}

	// キー割当
	bool isHotKeyK = enable_K ? RegisterHotKey(NULL, 0, hkModifiers_K | MOD_NOREPEAT, hkKey_K) : TRUE;
	bool isHotKey1 = enable_1 ? RegisterHotKey(NULL, 1, hkModifiers_1 | MOD_NOREPEAT, hkKey_1) : TRUE;
	bool isHotKey2 = enable_2 ? RegisterHotKey(NULL, 2, hkModifiers_2 | MOD_NOREPEAT, hkKey_2) : TRUE;
	bool isHotKey3 = enable_3 ? RegisterHotKey(NULL, 3, hkModifiers_3 | MOD_NOREPEAT, hkKey_3) : TRUE;
	bool isHotKey4 = enable_4 ? RegisterHotKey(NULL, 4, hkModifiers_4 | MOD_NOREPEAT, hkKey_4) : TRUE;

	// ホットキー登録
	if (!isHotKeyK || !isHotKey1 || !isHotKey2 || !isHotKey3 || !isHotKey4) {
		MessageBox(g_hMainWnd,
			_T("One or more hotkeys are already in use.\nPlease change settings in registry."),
			_T("One Touch Search error"),
			MB_ICONEXCLAMATION | MB_OK);
		return -4;
	}

	// Set the listener for the tray icon
	g_TrayIconOTS.SetListener(g_TrayIconOTS_OnMessage);

	// Start the message pump
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.message == WM_HOTKEY) {
			bool success = false;
			const wchar_t* urlToUse = nullptr;

			// ホットキー判定
			if (msg.wParam == 0) {  // Kホットキー
				urlToUse = searchEngineURL_K.c_str();
			}
			else if (msg.wParam == 1) {  // 1ホットキー
				urlToUse = searchEngineURL_1.c_str();
			}
			else if (msg.wParam == 2) {  // 2ホットキー
				urlToUse = searchEngineURL_2.c_str();
			}
			else if (msg.wParam == 3) {  // 3ホットキー
				urlToUse = searchEngineURL_3.c_str();
			}
			else if (msg.wParam == 4) {  // 4ホットキー
				urlToUse = searchEngineURL_4.c_str();
			}

			if (urlToUse) {
				success = oneTouchSearch(urlToUse);
				if (!success) {
					g_TrayIconOTS.ShowBalloonTooltip("One Touch Search","No selected text found!", CTrayIcon::eTI_Info);
				}
			}
		}
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
