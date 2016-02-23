#include <string>
#include <map>


using namespace std;

#include "def.h"
int main() {
	KEENIO_HTTP kHTTP;
	KEENIO_CLIENT kCLIENT;
	kHTTP.reqURL = "/3.0/projects/56b6369196773d7eaa5f4bca/queries/count";
	kHTTP.addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	kHTTP.addHeader("Accept-Encoding", "gzip, deflate, sdch");
	kHTTP.addHeader("Accept-Language", "en-US,en;q=0.8");
	kHTTP.addHeader("Connection", "keep-alive");
	kHTTP.addHeader("Host", "api.keen.io");
	kHTTP.addHeader("Upgrade-Insecure-Requests", "1");
	kHTTP.addHeader("User-Agent", "Carrots/KeenIO HTTP-1.0");

	kHTTP.addParam("api_key", kHTTP._masterKey);
	kHTTP.addParam("event_collection", "pageview");
	kHTTP.addParam("timezone", "UTC");
	kHTTP.addParam("timeframe", "this_14_days");

	kCLIENT.request(kHTTP);

	printf(kCLIENT.body.c_str());

	return 1;
}