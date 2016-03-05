#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>

using namespace std;


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 1000000
#define DEFAULT_PORT "80"

class KEENIO_HTTP {
public:
	string reqURL;
	map<string, string> _headers;
	map<string, string> _params;

	string _writeKey = "<write_key>";
	string _readKey = "<read_key>";
	string _masterKey = "<master_key>";

	void addDefHeaders(void) {
		this->addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
		this->addHeader("Accept-Encoding", "gzip, deflate, sdch");
		this->addHeader("Accept-Language", "en-US,en;q=0.8");
		this->addHeader("Connection", "keep-alive");
		this->addHeader("Host", "api.keen.io");
		this->addHeader("Upgrade-Insecure-Requests", "1");
		this->addHeader("User-Agent", "Carrots/KeenIO HTTP-1.0");
	}

	string masterKey(string key = "") {
		if (key != "") {
			this->_masterKey = key;
		}
		return this->_masterKey;
	}

	string readKey(string key = "") {
		if (key != "") {
			this->_readKey = key;
		}
		return this->_readKey;
	}

	string writeKey(string key = "") {
		if (key != "") {
			this->_writeKey = key;
		}
		return this->_writeKey;
	}

	void addHeader(string head, string content) {
		this->_headers[head] = content;
	}

	void addParam(string head, string content) {
		this->_params[head] = content;
	}

	string headers() {
		string plain;

		for each (std::pair<string, string> header in this->_headers)
		{
			plain += header.first + ":" + header.second + "\r\n";
		}

		return plain;
	}

	string params() {
		string plain = this->reqURL;
		int index = 0;
		for each (std::pair<string, string> header in this->_params)
		{
			if (index < 1) {
				plain += "?" + header.first + "=" + header.second;
			}
			else {
				plain += "&" + header.first + "=" + header.second;
			}
			index++;
		}

		return plain;
	}
};


class KEENIO_CLIENT {
public:
	string body;
	KEENIO_HTTP kHTTP;
	bool err = false;

	string* cleanUp(string* str) {
		string legal = "ABCDEFGHIJKLMNOPQRSTUVXYZabcdefghijklmnopqrstuvwxyz:;, []{}\"'\\/0123456789~`!@#$%^&*()_+|.<>";

		string cleanString;

		for (int i = 0; i < (int)str->size(); i++)
		{
			string search = str->substr(i, 1);
			int found = legal.find(search);

			if (found >= 0) {
				if (search != " ") {
					cleanString += search;
				}
			}
			else {
				break;
			}
		}

		*str = cleanString;

		return str;
	}
	int request(KEENIO_HTTP kHTTP)
	{
		WSADATA wsaData;
		SOCKET ConnectSocket = INVALID_SOCKET;
		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;
		char *sendbuf = "this is a test";
		char recvbuf[DEFAULT_BUFLEN];
		int iResult;
		int recvbuflen = DEFAULT_BUFLEN;


#ifdef DEBUG

		printf("WinSock initialized\n");

#else

#endif // DEBUG

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
#ifdef DEBUG

		printf("Resolving KeenIO IP");

#else

#endif // DEBUG

		// Resolve the server address and port
		iResult = getaddrinfo("108.168.254.50", DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			if (ConnectSocket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return 1;
			}
			else {
#ifdef DEBUG

				printf("Established socket @ https://api.keen.io/");

#else

#endif // DEBUG
			}

			// Connect to server.
			iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				continue;
			}
			else {
#ifdef DEBUG

				printf("Connected to KeenIO's servers");

#else

#endif // DEBUG
			}
			break;
		}

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			return 1;
		}

		// Send an initial buffer

		string headers = "GET " + kHTTP.params();
		headers += "\r\n";
		headers += kHTTP.headers();

		iResult = send(ConnectSocket, headers.c_str(), (int)strlen(headers.c_str()), 0);

		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		//printf("Bytes Sent: %ld\n", iResult);

#ifdef DEBUG

		printf("Sent HTTP request\n");

#else

#endif // DEBUG


		// shutdown the connection since no more data will be sent
		//iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}


#ifdef DEBUG

		printf("Waiting for KeenIO's response...");

#else

#endif // DEBUG
		// Receive until the peer closes the connection
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		string rec;

		for (int i = 0; i < (int)strlen(recvbuf); i++) {
			rec += recvbuf[i];
		}

#ifdef DEBUG

		printf("Cleaning up");

#else

#endif // DEBUG
		// cleanup
		this->cleanUp(&rec);
		closesocket(ConnectSocket);
		WSACleanup();

		this->body = rec;

		int keenErr = this->body.find("\"error_code\":");
		if (keenErr >= 0) {
			this->err = true;
		}
		return 0;
	}
};


class KEENIO_QUERYLANGUAGE {
private:

	map<int, string> statements;
	map<int, string> results;
	void query_exec() {

	}
	string api_key;

public:
	string queryURL;
	string QueryExec(string queryText) {

		int STMT_TYPE;

		if (queryText.find("COUNT") >= 0) {
			STMT_TYPE = 0;
		}
		else if (queryText.find("SUM") >= 0) {
			STMT_TYPE = 1;
		}
		else if (queryText.find("AVERAGE") >= 0) {
			STMT_TYPE = 2;
		}
		else {
			STMT_TYPE = 666;
		}

		char *str = (char*)queryText.c_str();
		char * pch;
		printf("\nSplitting string \"%s\" into tokens:\n", str);
		pch = strtok(str, " ");

		bool filterMode = false;
		string sel;
		string collection;
		string projectid;
		string qParams;

		string word;

		while (pch != NULL)
		{
			word = pch;

			if (!filterMode) {
				int setter = word.find("(");
				int psetter = word.find("=");
				int fmode = word.find("filters:");

				if (fmode >= 0) {
					filterMode = true;
					goto restart_ql_loop;
				}
				if (word == "count") {
					sel = word;
				}
				else if (word == "sum") {
					sel = word;
				}
				else if (word == "average") {
					sel = word;
				}
				else if (setter >= 0) {
					//IS SELECTOR

					int paren1 = word.find("(");
					collection = word.substr(0, paren1);

					int paren2 = word.find(")") - 1 - paren1;
					projectid = word.substr(paren1 + 1, paren2);
					//this->_query = collection;

				}
				else if (psetter >= 0) {
					//IS SELECTOR
					int sep = word.find("=");
					qParams += "&";
					qParams += word;
				}
			restart_ql_loop:
				int x = 0;

			}
			else {

			}

			pch = strtok(NULL, " ");
		}

		this->queryURL = queryText;
		this->query_exec();
		string qURL = "https://api.keen.io/3.0/projects/" + projectid;
		qURL += "/queries/" + sel;
		qURL += "?api_key=" + this->api_key;
		qURL += qParams;

		this->queryURL = qURL;

		return qURL;
	}

	void KEY(string key) {
		this->api_key = key;
	}

	string ProcessQuery() {
		KEENIO_CLIENT* kCLIENT = new KEENIO_CLIENT();
		kCLIENT->kHTTP.reqURL = (char*)this->queryURL.c_str();
		kCLIENT->kHTTP.addDefHeaders();

		kCLIENT->request(kCLIENT->kHTTP);

		return kCLIENT->body;
	}

	KEENIO_QUERYLANGUAGE() {

	}
};
