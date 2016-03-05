#pragma once
#include "KEENIO_SDK.h"

int main() {
	KEENIO_QUERYLANGUAGE* keenQL = new KEENIO_QUERYLANGUAGE();
	keenQL->KEY("<master_key>");
	keenQL->QueryExec("count media_play(<project_id>) event_collection=media_play timezone=UTC timeframe=this_14_days");

	printf("---QUERY REQUEST---\n%s\n\n\n--RAW REQUEST---\n", (char*)keenQL->ProcessQuery().c_str());
	
	
	KEENIO_CLIENT* kCLIENT = new KEENIO_CLIENT();
	kCLIENT->kHTTP.reqURL = "https://api.keen.io/3.0/projects/<project_id>/events";
	kCLIENT->kHTTP.addDefHeaders();

	kCLIENT->kHTTP.addParam("api_key", kCLIENT->kHTTP._masterKey);
	kCLIENT->kHTTP.addParam("event_collection", "pageview");
	kCLIENT->kHTTP.addParam("timezone", "UTC");
	kCLIENT->kHTTP.addParam("timeframe", "this_14_days");

	kCLIENT->request(kCLIENT->kHTTP);

	printf(kCLIENT->body.c_str());
	return 0;
}
