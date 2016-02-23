// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CARROTS_KEENIO_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CARROTS_KEENIO_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CARROTS_KEENIO_EXPORTS
#define CARROTS_KEENIO_API __declspec(dllexport)
#else
#define CARROTS_KEENIO_API __declspec(dllimport)
#endif

// This class is exported from the Carrots_KeenIO.dll
class CARROTS_KEENIO_API CCarrots_KeenIO {
public:
	CCarrots_KeenIO(void);
	// TODO: add your methods here.
};

extern CARROTS_KEENIO_API int nCarrots_KeenIO;

CARROTS_KEENIO_API int fnCarrots_KeenIO(void);


class KEENIO_HTTP {
public:
	string reqURL;
	map<string, string> _headers;
	map<string, string> _params;

	string _writeKey = "cdb8c4e78721169c3dc475c8d417c6038427913cf21b6785d01b27eb1cb55093f01f40d4e72199120ba88cbb1c1a787c786c001cb12d3a54dc5b5b0d69296db93943eb83f3b73990dba14f91f325f9771d0bf336d4415f219ae6345e1fb61690";
	string _readKey = "e415c20339feba31336ddce0623be502b629716d30fb8c2de930c97683613f189239f29ecb1ee2aef2f7bc96f7b06d45ab3fd6aea5a221ff4cc3dc612dd3062c7536a97d2cabf469ff386e4a750e59f82e50b8f9699a88015585d35cc65abb70";
	string _masterKey = "436D4D2EF7282BA17F712515ED39224F0439F892CDAA81D05437DB7137BB2D9C";

	CARROTS_KEENIO_API string masterKey(string key = "") {
		if (key != "") {
			this->_masterKey = key;
		}
		return this->_masterKey;
	}

	CARROTS_KEENIO_API string readKey(string key = "") {
		if (key != "") {
			this->_readKey = key;
		}
		return this->_readKey;
	}

	CARROTS_KEENIO_API string writeKey(string key = "") {
		if (key != "") {
			this->_writeKey = key;
		}
		return this->_writeKey;
	}

	CARROTS_KEENIO_API void addHeader(string head, string content) {
		this->_headers[head] = content;
	}

	CARROTS_KEENIO_API void addParam(string head, string content) {
		this->_params[head] = content;
	}

	CARROTS_KEENIO_API string headers() {
		string plain;

		for each (std::pair<string, string> header in this->_headers)
		{
			plain += header.first + ":" + header.second + "\r\n";
		}

		return plain;
	}

	CARROTS_KEENIO_API string params() {
		string plain = this->reqURL + "?ref=ubsernip.net";
		for each (std::pair<string, string> header in this->_params)
		{
			plain += "&" + header.first + "=" + header.second;
		}

		return plain;
	}
};