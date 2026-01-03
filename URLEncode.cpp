// From:
// https://www.codeproject.com/Articles/19629/How-to-Encode-Decode-URLs-to-the-UTF8-Format-with

// URL Encoding/Decodeing class, supports Unicode and ANSI (MFC TCHAR)
// Written by Daniel Cohen Gindi, (danielgindi (at) gmail.com)
// If you have any comments or questions, feel free to email me.
// You may use this class for any purpose, commercial or personal.

#include "stdafx.h"
#include "URLEncode.h"

CString CURLEncode::URLEncode(const CString& str)
{
    CString result;
    UINT ansiCP = GetACP();  // TCHARがcharの場合のために保持

    for (int i = 0; i < str.GetLength(); ++i) {
        WCHAR wc;

        // TCHAR → UTF-16 (WCHAR)
        if (sizeof(TCHAR) == 1) {
            CHAR mb[2] = { (CHAR)str[i], 0 };
            MultiByteToWideChar(ansiCP, 0, mb, -1, &wc, 1);
        }
        else {
            wc = str[i];
        }

        // サロゲートペアなら2ユニットまとめて処理
        WCHAR utf16[3] = { 0 };
        int utf16Len = 1;
        utf16[0] = wc;

        if (i + 1 < str.GetLength()) {
            WCHAR low = str[i + 1];
            if (wc >= 0xD800 && wc <= 0xDBFF && low >= 0xDC00 && low <= 0xDFFF) {
                utf16[1] = low;
                utf16Len = 2;
                ++i;  // 低位サロゲートも消費
            }
        }

        // UTF-16 → UTF-8
        CHAR utf8[8] = { 0 };  // 4バイト文字 + NULL で十分
        int byteLen = WideCharToMultiByte(
            CP_UTF8,
            0,
            utf16,
            utf16Len,
            utf8,
            sizeof(utf8),
            NULL,
            NULL
        );

        for (int j = 0; j < byteLen; ++j) {
            BYTE b = (BYTE)utf8[j];

            // 安全文字はそのまま（RFC 3986）
            if ((b >= 'A' && b <= 'Z') || (b >= 'a' && b <= 'z') ||
                (b >= '0' && b <= '9') || b == '-' || b == '_' ||
                b == '.' || b == '~') {
                result += (TCHAR)b;
            }
            else {
                CString hex;
                hex.Format(_T("%%%02X"), b);
                result += hex;
            }
        }
    }

    return result;
}
