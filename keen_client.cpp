#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>

using namespace std;

#include "Carrots_KeenIO.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "80"

KEENIO_HTTP kHTTP;

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

class KEENIO_CLIENT {
public:
	string body;
	CARROTS_KEENIO_API int request(KEENIO_HTTP kHTTP)
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
		for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {

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
		rec = recvbuf;

		#ifdef DEBUG

			printf("Cleaning up");

		#else

		#endif // DEBUG
		// cleanup
		cleanUp(&rec);
		closesocket(ConnectSocket);
		WSACleanup();
		
		this->body = rec;

		return 0;
	}
};
