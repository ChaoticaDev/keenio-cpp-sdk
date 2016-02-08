#pragma once

#include <locale>
#include <codecvt>

using namespace std;

class STRING_UTILS {

public:
	static string gen_random_string(int len) {
		string s;
		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		for (int i = 0; i < len; ++i) {
			s += alphanum[rand() % (sizeof(alphanum) - 1)];
		}

		s[len] = 0;

		return s;
	}

	static string fromWString(wstring ws) {
		setlocale(LC_CTYPE, "");

		const std::string s(ws.begin(), ws.end());

		return s;
	}

	static wstring utf8toUtf16(const string & str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide = converter.from_bytes(str);

		return wide;
	}
};