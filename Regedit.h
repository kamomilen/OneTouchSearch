#pragma once
#include "stdafx.h"
#include <string>

// Define registry values
static const HKEY REG_HK = HKEY_CURRENT_USER;
static const wchar_t* OTS_REG_KEY = L"Software\\OneTouchSearch";
static const wchar_t* OTS_REG_ENABLE_K = L"EnableHotkey";  // K用(デフォルト)
static const wchar_t* OTS_REG_ENABLE_1 = L"EnableHotkey1";  // 1用
static const wchar_t* OTS_REG_ENABLE_2 = L"EnableHotkey2";  // 2用
static const wchar_t* OTS_REG_ENABLE_3 = L"EnableHotkey3";  // 3用
static const wchar_t* OTS_REG_ENABLE_4 = L"EnableHotkey4";  // 4用
static const wchar_t* OTS_REG_VALUE_URL_K = L"SearchEngineURL";  // K用(デフォルト)
static const wchar_t* OTS_REG_VALUE_URL_1 = L"SearchEngineURL_1";  // 1用
static const wchar_t* OTS_REG_VALUE_URL_2 = L"SearchEngineURL_2";  // 2用
static const wchar_t* OTS_REG_VALUE_URL_3 = L"SearchEngineURL_3";  // 3用
static const wchar_t* OTS_REG_VALUE_URL_4 = L"SearchEngineURL_4";  // 4用
static const wchar_t* OTS_REG_VALUE_HOTKEY_K = L"Hotkey";         // K用(デフォルト)
static const wchar_t* OTS_REG_VALUE_HOTKEY_1 = L"Hotkey_1";         // 2用
static const wchar_t* OTS_REG_VALUE_HOTKEY_2 = L"Hotkey_2";         // 2用
static const wchar_t* OTS_REG_VALUE_HOTKEY_3 = L"Hotkey_3";         // 3用
static const wchar_t* OTS_REG_VALUE_HOTKEY_4 = L"Hotkey_4";         // 4用

class Regedit
{
private:

public:
	void saveURL(const wchar_t* regValue, const std::wstring& url);
	void saveHotkey(const wchar_t* regValue, CString& data);
	void saveHotkey(const wchar_t* regValue, UINT modifiers, UINT key);
	void saveEnable(const wchar_t* regValue, bool key);
	bool readURL(const wchar_t* regValue, CString& url);
	bool readURL(const wchar_t* regValue, std::wstring& url);
	bool readHotkey(const wchar_t* regValue, CString& data);
	bool readHotkey(const wchar_t* regValue, UINT& modifiers, UINT& key);
	bool readEnable(const wchar_t* regValue, bool& key);
	void splitDWORDToHotkey(std::wstring data, UINT& modifiers, UINT& key);
};