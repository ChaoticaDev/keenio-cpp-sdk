========================================================================
    UberSnip||Carrots_KeenIO C++ SDK
========================================================================

KeenIO C++ SDK and Query language.

This project covers KeenIO's APIs, and also includes a query language similar to SQL which allows for quick, easy data extraction.

## WIKI
 Checkout the [wiki](https://github.com/UberSnip/keenio-cpp-sdk/wiki) for detailed implementation of this project :D
 
 [The C++ Framework.](https://github.com/UberSnip/keenio-cpp-sdk/wiki/KeenIO-CPP-SDK)
 
 ["keenQL" SQL for KEENIO.](https://github.com/UberSnip/keenio-cpp-sdk/wiki/keenQL)
 
 [keenQL filters.](https://github.com/UberSnip/keenio-cpp-sdk/wiki/keenQL-Filters)
 
 [keenQL Global Variables.](https://github.com/UberSnip/keenio-cpp-sdk/wiki/keenQL-Global-Variables)


## Example usage

	#pragma once
	#include "KEENIO_SDK.h"

	// SEND DATA TO KEENIO
	//EVENT = app_event, DATA = 'xLAUNCH' Base64Encoded
	int sendData(){
		KEENIO_CLIENT* kCLIENT = new KEENIO_CLIENT();
		kCLIENT->kHTTP.reqURL = "https://api.keen.io/3.0/projects/PROJECT_ID/events";
		kCLIENT->kHTTP.addDefHeaders();

		kCLIENT->kHTTP.addParam("api_key", "<master_key>");

		string dataParam[2] = { "app_event", "xLAUNCH" };
		kCLIENT->kHTTP.addDataParam("data", dataParam);
		kCLIENT->method(KEENIO_HTTP_GET);
		kCLIENT->request(kCLIENT->kHTTP);

		printf(kCLIENT->body.c_str());
	
		return 1;
	}

	int main() {
		KEENIO_QUERYLANGUAGE::KEENIO_QUERY* keenQL = new KEENIO_QUERYLANGUAGE::KEENIO_QUERY();
		keenQL->KEY("<write_key>");
		keenQL->QueryExec(".export=example_exp.txt extraction pageview({$PROJECT_ID}) event_collection=pageview timezone=UTC timeframe=this_100_days if keen.id>0 && path<>'//'");

		printf("---QUERY REQUEST---\n%s", (char*)keenQL->ProcessQuery().c_str());
	
	
		KEENIO_CLIENT* kCLIENT = new KEENIO_CLIENT();
		kCLIENT->kHTTP.reqURL = "https://api.keen.io/3.0/projects/PROJECT_ID/events";
		kCLIENT->kHTTP.addDefHeaders();

		kCLIENT->kHTTP.addParam("api_key", "<read_key>");
		kCLIENT->kHTTP.addParam("event_collection", "pageview");
		kCLIENT->kHTTP.addParam("timezone", "UTC");
		kCLIENT->kHTTP.addParam("timeframe", "this_14_days");

		kCLIENT->method(KEENIO_HTTP_GET);
		kCLIENT->request(kCLIENT->kHTTP);

		printf(kCLIENT->body.c_str());
	
		sendData();
	
		return 0;
	}



Disclaimer: This product is in no way affiliated with KeenIO -- This is a community project, though, we would be very open to KeenIO picking up the project in the future! Keen? :D
