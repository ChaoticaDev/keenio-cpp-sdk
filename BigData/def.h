extern __declspec(dllimport) class KEENIO_HTTP {
public:
	string reqURL;
	map<string, string> _headers;
	map<string, string> _params;

	string _writeKey = "cdb8c4e78721169c3dc475c8d417c6038427913cf21b6785d01b27eb1cb55093f01f40d4e72199120ba88cbb1c1a787c786c001cb12d3a54dc5b5b0d69296db93943eb83f3b73990dba14f91f325f9771d0bf336d4415f219ae6345e1fb61690";
	string _readKey = "e415c20339feba31336ddce0623be502b629716d30fb8c2de930c97683613f189239f29ecb1ee2aef2f7bc96f7b06d45ab3fd6aea5a221ff4cc3dc612dd3062c7536a97d2cabf469ff386e4a750e59f82e50b8f9699a88015585d35cc65abb70";
	string _masterKey = "436D4D2EF7282BA17F712515ED39224F0439F892CDAA81D05437DB7137BB2D9C";

	void addHeader(string head, string content);
	void addParam(string head, string content);

	string headers();
	string params();

	string masterKey(string key = "");
	string readKey(string key = "");
	string writeKey(string key = "");
};
extern __declspec(dllimport) class KEENIO_CLIENT {
public:
	string body;
	int request(KEENIO_HTTP kHTTP);
};
