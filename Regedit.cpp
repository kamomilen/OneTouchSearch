#include "stdafx.h"
#include <atlconv.h>
#include "Regedit.h"


// ヘルパー関数
void Regedit::saveURL(const wchar_t* regValue, const std::wstring& url) {
	HKEY hKey;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\OneTouchSearch", 0, NULL, 0,
		KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS) {
		RegSetValueEx(hKey, regValue, 0, REG_SZ,
			(LPBYTE)url.c_str(), (url.size() + 1) * sizeof(wchar_t));
		RegCloseKey(hKey);
	}
}

void Regedit::saveHotkey(const wchar_t* regValue, CString& data) {
	DWORD ddata = (DWORD)_tcstoul(data, NULL, 16);
	HKEY hKey;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\OneTouchSearch", 0, NULL, 0,
		KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS) {
		RegSetValueEx(hKey, regValue, 0, REG_DWORD, (BYTE*)&ddata, sizeof(ddata));
		RegCloseKey(hKey);
	}
}

void Regedit::saveHotkey(const wchar_t* regValue, UINT modifiers, UINT key) {
	DWORD hkValue = MAKELONG(key, modifiers);
	HKEY hKey;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\OneTouchSearch", 0, NULL, 0,
		KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS) {
		RegSetValueEx(hKey, regValue, 0, REG_DWORD, (BYTE*)&hkValue, sizeof(hkValue));
		RegCloseKey(hKey);
	}
}

void Regedit::saveEnable(const wchar_t* regValue, bool key) {
	DWORD hkValue = key;
	HKEY hKey;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\OneTouchSearch", 0, NULL, 0,
		KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS) {
		RegSetValueEx(hKey, regValue, 0, REG_DWORD, (BYTE*)&hkValue, sizeof(hkValue));
		RegCloseKey(hKey);
	}
}

// Read search engine URLs from registry
bool Regedit::readURL(const wchar_t* regValue, CString& url) {
	std::wstring wurl;
	wurl.resize(1023);
	DWORD size = wurl.size() * sizeof(wchar_t);
	if (RegGetValue(REG_HK, OTS_REG_KEY, regValue, RRF_RT_REG_SZ, NULL, &wurl[0], &size) == ERROR_SUCCESS) {
		wurl.resize(size / sizeof(wchar_t) - 1);
		url = wurl.c_str();
		return true;
	}
	return false;	
}

bool Regedit::readURL(const wchar_t* regValue, std::wstring& url) {
	url.resize(1023);
	DWORD size = url.size() * sizeof(wchar_t);
	if (RegGetValue(REG_HK, OTS_REG_KEY, regValue, RRF_RT_REG_SZ, NULL, &url[0], &size) == ERROR_SUCCESS) {
		url.resize(size / sizeof(wchar_t) - 1);
		return true;
	}
	return false;
}

// ホットキー設定読み込み
bool Regedit::readHotkey(const wchar_t* regValue, CString& data) {
	DWORD ddata = (DWORD)_tcstoul(data, NULL, 16);
	DWORD size = sizeof(ddata);
	if (RegGetValue(REG_HK, OTS_REG_KEY, regValue, RRF_RT_REG_DWORD, NULL, &ddata, &size) == ERROR_SUCCESS) {
		data.Format(_T("%08X"), ddata);
		return true;
	}
	return false;
}

// ホットキー設定読み込み
bool Regedit::readHotkey(const wchar_t* regValue, UINT& modifiers, UINT& key) {
	DWORD data;
	DWORD size = sizeof(data);
	if (RegGetValue(REG_HK, OTS_REG_KEY, regValue, RRF_RT_REG_DWORD, NULL, &data, &size) == ERROR_SUCCESS) {
		modifiers = HIWORD(data);
		key = LOWORD(data);
		return true;
	}
	return false;
}

// ホットキー設定読み込み
bool Regedit::readEnable(const wchar_t* regValue, bool& key) {
	DWORD data;
	DWORD size = sizeof(data);
	if (RegGetValue(REG_HK, OTS_REG_KEY, regValue, RRF_RT_REG_DWORD, NULL, &data, &size) == ERROR_SUCCESS) {
		key = (bool)LOWORD(data);
		return true;
	}
	return false;
}

void splitDWORDToHotkey(std::wstring data, UINT& modifiers, UINT& key) {
	DWORD ddata = std::stoul(data);
	modifiers = HIWORD(ddata);
	key = LOWORD(ddata);
}
