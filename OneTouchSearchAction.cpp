#include "stdafx.h"
#include "OneTouchSearchAction.h"
#include "ClipboardBackup.h"
#include "URLEncode.h"

CStringW getClipboard() {
    // Return clipboard text in Unicode format
    CStringW strData;

	for (int retry = 0; retry < 5; ++retry) {
		Sleep(50);

		if (OpenClipboard(NULL)) {
			HANDLE hData = GetClipboardData(CF_UNICODETEXT);
			if (hData) {
				WCHAR* pchData = (WCHAR*)GlobalLock(hData);
				if (pchData) {
					strData = pchData;
					GlobalUnlock(hData);
				}
			}
			CloseClipboard();
		}
		if (strData.Trim().GetLength() > 0) {
			break;  // 成功
		}
	}
    return strData;
}

// Ctrl+Insert送信
bool SendCtrlIns() {
	const int key_count = 4;
	INPUT input[key_count] = { {INPUT_KEYBOARD}, {INPUT_KEYBOARD}, {INPUT_KEYBOARD}, {INPUT_KEYBOARD} };
	const WORD inputKey[2] = { VK_CONTROL, VK_INSERT };

	// ALT解除
	input[0].ki.wVk = VK_MENU;
	input[0].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, input, sizeof(INPUT));
	input[0].ki.dwFlags = 0;

	// Ctrl+Insert: 押下 → 解放
	input[0].ki.wVk = input[2].ki.wVk = inputKey[0];
	input[1].ki.wVk = input[3].ki.wVk = inputKey[1];
	input[2].ki.dwFlags = (input[3].ki.dwFlags |= KEYEVENTF_KEYUP);

	return SendInput(4, input, sizeof(INPUT)) != 0;
}

// Ctrl+C送信
bool SendCtrlC() {
	const int key_count = 4;
	INPUT input[4] = { {INPUT_KEYBOARD}, {INPUT_KEYBOARD}, {INPUT_KEYBOARD}, {INPUT_KEYBOARD} };
	const WORD inputKey[2] = { VK_CONTROL, _T('C') };

	// Ctrl+C: 押下 → 解放
	input[0].ki.wVk = input[2].ki.wVk = inputKey[0];
	input[1].ki.wVk = input[3].ki.wVk = inputKey[1];
	input[2].ki.dwFlags = (input[3].ki.dwFlags |= KEYEVENTF_KEYUP);

	return SendInput(4, input, sizeof(INPUT)) != 0;
}

CStringW getClipboardText() {
	// 【1段階目】Ctrl+Insert試行
	CStringW clipText;
	bool hasText = false;

	if (SendCtrlIns()) {
		clipText = getClipboard();
		if (!clipText.Trim().IsEmpty()) {
			return clipText;
		}
	}
	else {
		Sleep(100);  // クールダウン
	}

	// 【2段階目】Ctrl+Cフォールバック
	if (!hasText) {
		if (SendCtrlC()) {
			clipText = getClipboard();
		}
	}

	return clipText;
}

bool oneTouchSearch(const wchar_t* search_engine_url) {

	// Backup current clipboard (text & images should work)
	CClipboardBackup cbbackup;

	// Empty clipboard to check whether the selection doesn't exist
	::OpenClipboard(NULL);
	::EmptyClipboard();
	::CloseClipboard();

	CStringW clipText = getClipboardText();

	// Check that the returned string is not empty (= empty clipboard)
	if (clipText.Trim().IsEmpty() || clipText.Trim().GetLength() > 0) {

		// Encode the URL
		CURLEncode cEncoder;
		//CString clipTextURL = cEncoder.Encode(clipText);
		CString clipTextURL = cEncoder.URLEncode(clipText);

		// Prepends the query URL
		CString finalURL;
		finalURL.Append(search_engine_url);
		finalURL.Append(clipTextURL);

		// Open the default browser
		ShellExecute(NULL, _T("open"), finalURL, NULL, NULL, SW_SHOWNORMAL);

		// Restore the clipboard
		cbbackup.Restore();
		return TRUE;

	}
	else {

		// Restore the clipboard
		cbbackup.Restore();

		// No text to search
		return FALSE;
	}

}