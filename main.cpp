#pragma once
#include "KEENIO_SDK.h"



#define KQL_EQUAL "eq" //fully supported
#define KQL_NOT_EQUAL "ne" //fully supported
#define KQL_GREATER_THAN "gt" //
#define KQL_GREATER_THAN_EQUAL "gte" //
#define KQL_LESS_THAN "lt" //
#define KQL_LESS_THAN_EQUAL "lte" //
#define KQL_CONTAINS "contains"
#define KQL_NOT_CONTAINS "not_contains"

int main() {
	KEENIO_QUERYLANGUAGE::KEENIO_QUERY* keenQL = new KEENIO_QUERYLANGUAGE::KEENIO_QUERY();
	keenQL->KEY("<key>");
	keenQL->QueryExec(".export=example_exp.txt extraction pageview(56b6369196773d7eaa5f4bca) event_collection=pageview timezone=UTC timeframe=this_100_days if keen.id>0 && path<>'//'");

	printf("---QUERY REQUEST---\n%s", (char*)keenQL->ProcessQuery().c_str());
	
	
	KEENIO_CLIENT* kCLIENT = new KEENIO_CLIENT();
	kCLIENT->kHTTP.reqURL = "https://api.keen.io/3.0/projects/PROJ_ID/events";
	kCLIENT->kHTTP.addDefHeaders();

	kCLIENT->kHTTP.addParam("api_key", kCLIENT->kHTTP._masterKey);
	kCLIENT->kHTTP.addParam("event_collection", "pageview");
	kCLIENT->kHTTP.addParam("timezone", "UTC");
	kCLIENT->kHTTP.addParam("timeframe", "this_14_days");

	kCLIENT->request(kCLIENT->kHTTP);

	printf(kCLIENT->body.c_str());
	return 0;
}
