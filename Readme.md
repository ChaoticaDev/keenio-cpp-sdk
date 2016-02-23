========================================================================
    DYNAMIC LINK LIBRARY : Carrots_KeenIO Project Overview
========================================================================

KeenIO C++ SDK (Community Supported).

This project is geared towards developers manging big data with C++ (KeenIO).
This project uses WinSock to connect to a server, and retrieve information.


## KEENIO_HTTP : HTTP sockets handler

## KEENIO_CLIENT : KeenIO Query Manager

## KEENIO_HTTP::reqURL : Request url


## Settings Keys;

	KEENIO_CLIENT kCLIENT; //KEENIO CLIENT
	
	kCLIENT.masterKey("<master_key>");
	
	kCLIENT.readKey("<read_key>");
	
	kCLIENT.writeKey("<write_key>");


## Adding headers
	KEENIO_HTTP kHTTP; //KEENIO_HTTP_CLIENT
	
	kHTTP.addHeader("User-Agent", "Carrots/KeenIO HTTP-1.0");


## Adding query params

	KEENIO_HTTP kHTTP; //KEENIO_HTTP_CLIENT
	
	kHTTP.addParam("api_key", kHTTP._masterKey);


========================================================================
    Complete example
========================================================================

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
		
		//QUERY URL:: https://api.keen.io/3.0/projects/56b6369196773d7eaa5f4bca/queries/count?api_key=<key>&event_collection=pageview&timezone=UTC&timeframe=this_14_days&filters=%5B%5D
		return 1; // Returns {"result":170}
	}
