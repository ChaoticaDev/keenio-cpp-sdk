========================================================================
    UberSnip||Carrots_KeenIO C++ SDK
========================================================================

KeenIO C++ SDK and Query language.

This porject covers most of KeenIO's APIs, and also includes a query language similar to SQL which allows for quick, easy data extraction.

## WIKI
 Checkout the [wiki](https://github.com/UberSnip/keenio-cpp-sdk/wiki) for detailed implementation of this project :D
 
 [The C++ Framework.](https://github.com/UberSnip/keenio-cpp-sdk/wiki/KeenIO-CPP-SDK)
 
 ["keenQL" SQL for KEENIO.](https://github.com/UberSnip/keenio-cpp-sdk/wiki/keenQL)
 
 [keenQL filters.](https://github.com/UberSnip/keenio-cpp-sdk/wiki/keenQL::Filters)


## Example usage

	#include "KEENIO_SDK.h"
	int main() {
		KEENIO_CLIENT* kCLIENT = new KEENIO_CLIENT();
		kCLIENT->kHTTP.reqURL = "https://api.keen.io/3.0/projects/<project_id>/events";
		kCLIENT->kHTTP.addDefHeaders();

		kCLIENT->kHTTP.addParam("api_key", kCLIENT->kHTTP._masterKey);
		kCLIENT->kHTTP.addParam("event_collection", "pageview");
		kCLIENT->kHTTP.addParam("timezone", "UTC");
		kCLIENT->kHTTP.addParam("timeframe", "this_14_days");

		kCLIENT->request(kCLIENT->kHTTP);

		printf(kCLIENT->body.c_str());
		
		//QUERY URL:: https://api.keen.io/3.0/projects/<project_id>/queries/count?api_key=<key>&event_collection=pageview&timezone=UTC&timeframe=this_14_days
		return 1; // Returns {"result":170}
	}



Disclaimer: This product is in no way affiliated with KeenIO -- This is a community project, though, we would be very open to KeenIO picking up the project in the future! Keen? :D
