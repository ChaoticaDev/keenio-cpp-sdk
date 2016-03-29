========================================================================
    UberSnip||Carrots_KeenIO C++ SDK
========================================================================

KeenIO C++ SDK and Query language.

This project covers KeenIO's APIs, and also includes a query language similar to SQL which allows for quick, easy data extraction.

This API connects to KeenIO over Winsock.

## WIKI
 Checkout the [wiki](https://github.com/UberSnip/keenio-cpp-sdk/wiki) for detailed implementation of this project :D
 
 [The C++ Framework.](https://github.com/UberSnip/keenio-cpp-sdk/wiki/KeenIO-CPP-SDK)
 
 ["keenQL" SQL for KEENIO.](https://github.com/UberSnip/keenio-cpp-sdk/wiki/keenQL)
 
 [keenQL filters.](https://github.com/UberSnip/keenio-cpp-sdk/wiki/keenQL-Filters)
 
 [keenQL Global Variables.](https://github.com/UberSnip/keenio-cpp-sdk/wiki/keenQL-Global-Variables)
 

# The KeenIO Client
The KeenIO Client is the main handler for the KeenIO C++ API. Handles include by not limited to
 Keen related data such as keys, events, etc.

## The KeenIO HTTP Client
The HTTP client handles `network` and request related information such as headers, api parameters, and sending/receiving requests. (WinSock!)

### Adding headers

Adding headers ensures that KeenIO understands your request. The following headers are required:

> ("Accept-Encoding", "gzip, deflate, sdch")

> ("Accept-Language", "en-US,en;q=0.8")

> ("Connection", "keep-alive")

> ("Host", "api.keen.io")

> ("User-Agent", "Carrots/KeenIO HTTP-1.0")

Minimal headers can be added via function `KEENIO_HTTP::addDefHeaders()`

	class KEENIO_HTTP{
	
		void addDefHeaders(void);
		
	}
	
	//Example
	KEENIO_CLIENT* kCLIENT;
	kCLIENT->kHTTP.addDefHeaders();
	
### Adding URL Parameters
After connecting to the KeenIO API server, this client requests resources via URL. `IE: https://api.keen.io/3.0/projects/PROJECT_ID/events?api_key=1234&timeframe=this_14_days`

Parameters are added in the same manner as `headers`.

	class KEENIO_HTTP{
		void addParam(string head, string content);
	}	

	//Example
	KEENIO_CLIENT* kCLIENT;
	kCLIENT->kHTTP.addParam("api_key", "1234");
	
	
#### Adding Base64 parameters.
Data parameters are simply put, URL encoded to Base64, which are required when sending data via `GET`.

	class KEENIO_HTTP{
		void addDataParam(string head, string content);
	}	

	//Example
	KEENIO_CLIENT* kCLIENT;
	kCLIENT->kHTTP.addDataParam("api_key", "1234");
	
	

## Getting project resources

A quick snippet with minimal required code

	//KEENIO_SDK CLIENT
	KEENIO_CLIENT* kCLIENT = new KEENIO_CLIENT();
	
	//Set the request URL
	kCLIENT->kHTTP.reqURL = "https://api.keen.io/3.0/projects/PROJECT_ID/events";
	
	//Add minimal required headers
	kCLIENT->kHTTP.addDefHeaders();

	//ADD Parameter api_key
	kCLIENT->kHTTP.addParam("api_key", "<read_key>");
	kCLIENT->request(kCLIENT->kHTTP);

## Sending an event to KeenIO

	KEENIO_CLIENT* kCLIENT = new KEENIO_CLIENT();
	kCLIENT->kHTTP.reqURL = "https://api.keen.io/3.0/projects/PROJECT_ID/events";
	kCLIENT->kHTTP.addDefHeaders();

	kCLIENT->kHTTP.addParam("api_key", "<master_key>");

	kCLIENT->kHTTP.addDataParam("data", {"app_event","xLAUNCH"} );
	kCLIENT->method(KEENIO_HTTP_GET);
	kCLIENT->request(kCLIENT->kHTTP);

	printf(kCLIENT->body.c_str());
		
	
## Complete example	

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



Disclaimer: This product is in no way affiliated with KeenIO -- This is a community project, though, we would be very open to KeenIO picking up the project in the future! :D
