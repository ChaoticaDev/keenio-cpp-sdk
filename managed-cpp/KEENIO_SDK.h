
#include "App.g.h"
#include "external\JSON\cJSON.h"
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>


using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;

namespace KeenIOAPI{


	public ref class CLIENT sealed {

	Platform::String^ body;
	bool err = false;
	Platform::Object^ kHTTP;

	public:
		CLIENT() {}
		property Platform::String^ BodyResponse {
			Platform::String^ get() {
				return this->body;
			}

			void set(Platform::String^ val) {
				this->body = val;
			}
		}

		property bool APIError {
			bool get() {
				return this->err;
			}

			void set(bool val) {
				this->err = val;
			}
		}

		property Platform::Object^ http {
			Platform::Object^ get() {
				return this->kHTTP;
			}

			void set(Platform::Object^ val) {
				this->kHTTP = val;
			}
		}

};

	public ref class HTTP sealed {

	Platform::Object^ sdk_client;
	Platform::String^ reqURL;
	std::map<Platform::String^, Platform::String^> _headers;
	std::map<Platform::String^, Platform::String^> _params;

	bool recvraw(SOCKET socket, char *buf, int buflen)
	{
		unsigned char* p = (unsigned char*)malloc(sizeof(unsigned char*));
		while (buflen > 0)
		{
			int received = recv(socket, buf, buflen, 0);
			if (received < 1) return false;
			p += received;
			buflen -= received;
			//printf(".");
		}
		return true;
	}

	static inline bool is_base64(unsigned char c) {
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

	std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
		static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		std::string ret;
		int i = 0;
		int j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];

		while (in_len--) {
			char_array_3[i++] = *(bytes_to_encode++);
			if (i == 3) {
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i < 4); i++)
					ret += base64_chars[char_array_4[i]];
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 3; j++)
				char_array_3[j] = '\0';

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
				ret += base64_chars[char_array_4[j]];

			while ((i++ < 3))
				ret += "%3D";

		}

		return ret;

	}

	std::string base64_decode(std::string const& encoded_string) {
		static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		int in_len = encoded_string.size();
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		std::string ret;

		while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
			char_array_4[i++] = encoded_string[in_]; in_++;
			if (i == 4) {
				for (i = 0; i < 4; i++)
					char_array_4[i] = base64_chars.find(char_array_4[i]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++)
					ret += char_array_3[i];
				i = 0;
			}
		}

		if (i) {
			for (j = i; j < 4; j++)
				char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] = base64_chars.find(char_array_4[j]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
		}

		return ret;
	}
public:


	void addHeader(Platform::String^ head, Platform::String^ content) {
		this->_headers[head] = content;
	}

	void addDefHeaders(void) {
		this->addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
		this->addHeader("Accept-Encoding", "gzip, deflate, sdch");
		this->addHeader("Accept-Language", "en-US,en;q=0.8");
		this->addHeader("Connection", "keep-alive");
		this->addHeader("Host", "api.ubersnip.com");
		this->addHeader("Upgrade-Insecure-Requests", "1");
		this->addHeader("User-Agent", "Carrots/KeenIO HTTP-1.0");
	}

	void addParam(Platform::String^ head, Platform::String^ content) {
		this->_params[head] = content;
	}



	//ADD A PARAMETER TO MAP
	void addDataParam(Platform::String^ head, Platform::String^ event_name, Platform::String^ event_property) {

		int len_s = strlen(UberSnip::UTILS::STRING::StringToAscIIChars("{ \"" + event_name + "\" : \"" + event_property + "\" }").c_str());
		this->_params[head] = UberSnip::UTILS::STRING::StringFromAscIIChars(this->base64_encode((const unsigned char*)UberSnip::UTILS::STRING::StringToAscIIChars("{ \"" + event_name + "\" : \"" + event_property + "\" }").c_str(), len_s));

	}

	Platform::String^ headers() {
		Platform::String^ plain;

		for each (std::pair<Platform::String^, Platform::String^> header in this->_headers)
		{
			plain += header.first + ":" + header.second + "\r\n";
		}

		return plain;
	}

	Platform::String^ params() {
		Platform::String^ plain = this->reqURL;
		int index = 0;
		for each (std::pair<Platform::String^, Platform::String^> header in this->_params)
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


	void request()
	{
		WSADATA wsaData;
		SOCKET ConnectSocket = INVALID_SOCKET;
		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;
		char *sendbuf = "this is a test";
		char recvbuf[DEFAULT_BUFLEN] = "";
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
			dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
			return;
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
			dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
			return;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			if (ConnectSocket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
				return;
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

			dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
			return;
		}

		// Send an initial buffer

		Platform::String^ headers = "GET " + this->params();
		headers += "\r\n";
		headers += this->headers();

		iResult = send(ConnectSocket, UberSnip::UTILS::STRING::StringToAscIIChars(headers).c_str(), (int)strlen(UberSnip::UTILS::STRING::StringToAscIIChars(headers).c_str()), 0);

		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
			return;
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
			dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
			return;
		}


#ifdef DEBUG

		printf("Waiting for KeenIO's response...");

#else

#endif // DEBUG
		// Receive until the peer closes the connection
		//iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

		std::string rec;
		u_long iMode = 1;
		//ioctlsocket(ConnectSocket, FIONBIO, &iMode);

		while (recvraw(ConnectSocket, recvbuf, 1)) {
			rec += recvbuf;
		}



#ifdef DEBUG

		printf("Cleaning up");

#else

#endif // DEBUG
		//printf("\n\n%s\n\n", (char*)rec.c_str());
		closesocket(ConnectSocket);
		WSACleanup();

		CLIENT^ sdk = dynamic_cast<CLIENT^>(this->sdk_client);
		sdk->BodyResponse = UberSnip::UTILS::STRING::StringFromAscIIChars(rec);

		//int keenErr = UTILS::STRING::StringToAscIIChars(sdk_client->BodyResponse).find("\"error_code\":");
		//if (keenErr >= 0) {
		//sdk_client->APIError = true;
		//}
		return;
	}

	property Platform::Object^ SDKClient {
		Platform::Object^ get() {
			return this->sdk_client;
		}

		void set(Platform::Object^ val) {
			this->sdk_client = val;
		}
	}

	HTTP() {

	}

	property Platform::String^ RequestURL {
		Platform::String^ get() {
			return this->reqURL;
		}

		void set(Platform::String^ val) {
			this->reqURL = val;
		}
	}

};

	class KEENIO_CLIENT {

	public:
		HTTP^ Http = ref new HTTP();
		CLIENT^ Client = ref new CLIENT();



		KEENIO_CLIENT() {
			this->Client->http = ref new HTTP();
			dynamic_cast<HTTP^>(this->Client->http)->SDKClient = this->Client;
			this->Http = dynamic_cast<HTTP^>(this->Client->http);
		}

	};

	public ref class KEENIO_AUTH sealed {
		Platform::String^ _master_key;
		Platform::String^ _write_key;
		Platform::String^ _read_key;
	public:
		KEENIO_AUTH() {

		}


		property Platform::String^ MasterKey {
			Platform::String^ get() {
				return this->_master_key;
			}

			void set(Platform::String^ val) {
				this->_master_key = val;
			}
		}
	};

	public ref class KEENIO_GENERIC_PROPERTY sealed{
		Platform::String^ _property;
		Platform::String^ _value;
	public:
		KEENIO_GENERIC_PROPERTY() {

		}

		property Platform::String^ PropertyName {
			Platform::String^ get() {
				return this->_property;
			}

			void set(Platform::String^ val) {
				this->_property = val;
			}
		}

		property Platform::String^ PropertyValue {
			Platform::String^ get() {
				return this->_value;
			}

			void set(Platform::String^ val) {
				this->_value = val;
			}
		}
	};

	public ref class KEENIO_EVENT sealed {
		Platform::String^ _name;
		Windows::UI::Xaml::Interop::IBindableObservableVector^ _properties = ref new Platform::Collections::Vector<KEENIO_GENERIC_PROPERTY^>();
		Platform::String^ _url;
		Platform::String^ _proj_id;

			
		KEENIO_AUTH^ auth = ref new KEENIO_AUTH();

		bool verified = false;
		bool error = false;
	public:
		KEENIO_EVENT() {

		}

		void LoadProperties() {
			KEENIO_CLIENT * UberSnipAPI = new KEENIO_CLIENT();

			UberSnipAPI->Http->RequestURL = "https://api.keen.io/3.0/projects/" + _proj_id  + "/events/" + this->_name;
			UberSnipAPI->Http->addParam("api_key", this->auth->MasterKey);

			UberSnipAPI->Http->request();

			cJSON* response = cJSON_Parse(_StringToAscIIChars(UberSnipAPI->Client->BodyResponse));

			cJSON* arr_events;

			try {
				arr_events = cJSON_GetObjectItem(response, "properties");
			}
			catch (std::exception_ptr e) {
				this->verified = false;
				this->error = true;
				return;
			}

			for (cJSON* c_event = arr_events->child; c_event != nullptr; c_event = c_event->next) {
				KEENIO_GENERIC_PROPERTY^ k_prop = ref new KEENIO_GENERIC_PROPERTY();
				k_prop->PropertyName = _String(c_event->valuestring);
				k_prop->PropertyValue = _String(c_event->string);
				this->_properties->Append(k_prop);
			}

			this->verified = true;
			this->error = false;
		}

		property KEENIO_AUTH^ Auth {
			KEENIO_AUTH^ get() {
				return this->auth;
			}

			void set(KEENIO_AUTH^ val) {
				this->auth = val;
			}
		}

		property Platform::String^ Name {
			Platform::String^ get() {
				return this->_name;
			}

			void set(Platform::String^ val) {
				this->_name = val;
			}
		}

		property Platform::String^ ProjectID {
			Platform::String^ get() {
				return this->_proj_id;
			}

			void set(Platform::String^ val) {
				this->_proj_id = val;
			}
		}

		property Platform::String^ URL {
			Platform::String^ get() {
				return this->_url;
			}

			void set(Platform::String^ val) {
				this->_url = val;
			}
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Properties {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->_properties;
			}
		}
	};

	public ref class KEENIO_PROJECT sealed{
		Platform::String^ _events_url;
		Windows::UI::Xaml::Interop::IBindableObservableVector^ _events = ref new Platform::Collections::Vector<KEENIO_EVENT^>();
		Platform::String^ _id;
		Platform::String^ _org_id;
		Platform::String^ _name;
		Platform::String^ _queries_url;
		Platform::String^ _partners_url;
		Platform::String^ _url;

		KEENIO_AUTH^ auth = ref new KEENIO_AUTH();

		bool verified = false;
		bool error = false;

	public:
		KEENIO_PROJECT() {
				
		}

		void LoadProject() {
			KEENIO_CLIENT * UberSnipAPI = new KEENIO_CLIENT();

			UberSnipAPI->Http->RequestURL = "https://api.keen.io/3.0/projects/" + this->_id;
			UberSnipAPI->Http->addParam("api_key", this->MasterKey);

			UberSnipAPI->Http->request();


			cJSON* response = cJSON_Parse(_StringToAscIIChars(UberSnipAPI->Client->BodyResponse));

			cJSON* arr_events ;

			try {
				arr_events = cJSON_GetObjectItem(response, "events");
			}
			catch (std::exception_ptr e) {
				this->verified = false;
				this->error = true;
				return;
			}

			for (cJSON* c_event = arr_events->child; c_event != nullptr; c_event = c_event->next) {
				KEENIO_EVENT^ k_event = ref new KEENIO_EVENT();
				k_event->Name = _String(cJSON_GetObjectItem(c_event, "name")->valuestring);
				k_event->URL = _String(cJSON_GetObjectItem(c_event, "url")->valuestring);
				k_event->Auth = this->auth;
				k_event->ProjectID = this->_id;
				k_event->LoadProperties();
				this->_events->Append(k_event);
			}

			this->_name = _String(cJSON_GetObjectItem(response, "name")->valuestring);
			this->_org_id = _String(cJSON_GetObjectItem(response, "organization_id")->valuestring);
			this->_partners_url = _String(cJSON_GetObjectItem(response, "partners_url")->valuestring);
			this->_queries_url = _String(cJSON_GetObjectItem(response, "queries_url")->valuestring);
			this->_url = _String(cJSON_GetObjectItem(response, "url")->valuestring);

			this->verified = true;
			this->error = false;
		}

		void SendEvent(Platform::String^ collection, Platform::String^ event_name, Platform::String^ event_property) {

			if (!this->verified) {return;}

			KeenIOAPI::KEENIO_CLIENT * UberSnipAPI = new KeenIOAPI::KEENIO_CLIENT();

			UberSnipAPI->Http->RequestURL = "https://api.keen.io/3.0/projects/" + this->_id + "/events/" + collection;
			UberSnipAPI->Http->addParam("api_key", this->MasterKey);

			UberSnipAPI->Http->addDataParam("data", event_name, event_property);
			UberSnipAPI->Http->request();
		}

		property Platform::String^ EventsURL {
			Platform::String^ get() {
				return this->_events_url;
			}

			void set(Platform::String^ val) {
				this->_events_url = val;
			}
		}

		property Platform::String^ ID {
			Platform::String^ get() {
				return this->_id;
			}

			void set(Platform::String^ val) {
				this->_id = val;
			}
		}

		property Platform::String^ OrganizationID {
			Platform::String^ get() {
				return this->_org_id;
			}

			void set(Platform::String^ val) {
				this->_org_id = val;
			}
		}

		property Platform::String^ Name {
			Platform::String^ get() {
				return this->_name;
			}

			void set(Platform::String^ val) {
				this->_name = val;
			}
		}

		property Platform::String^ QueriesURL {
			Platform::String^ get() {
				return this->_queries_url;
			}

			void set(Platform::String^ val) {
				this->_queries_url = val;
			}
		}

		property Platform::String^ PartnersURL {
			Platform::String^ get() {
				return this->_partners_url;
			}

			void set(Platform::String^ val) {
				this->_partners_url = val;
			}
		}

		property Platform::String^ URL {
			Platform::String^ get() {
				return this->_url;
			}

			void set(Platform::String^ val) {
				this->_url = val;
			}
		}

		property Platform::String^ MasterKey {
			Platform::String^ get() {
				return this->auth->MasterKey;
			}

			void set(Platform::String^ val) {
				this->auth->MasterKey = val;
			}
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Events {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->_events;
			}
		}
	};

}
