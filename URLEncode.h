// From:
// https://www.codeproject.com/Articles/19629/How-to-Encode-Decode-URLs-to-the-UTF8-Format-with

// URL Encoding/Decodeing class, supports Unicode and ANSI (MFC TCHAR)
// Written by Daniel Cohen Gindi, (danielgindi (at) gmail.com)
// If you have any comments or questions, feel free to email me.
// You may use this class for any purpose, commercial or personal.

#pragma once

#include "stdafx.h"

class CURLEncode
{

public:
	CString URLEncode(const CString& str);

};
