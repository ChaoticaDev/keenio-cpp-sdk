========================================================================
    Carrots_KeenIO Project Overview
========================================================================

KeenIO C++ SDK (Community Supported).

This project is geared towards developers manging big data with C++ (KeenIO).
This project uses WinSock to connect to a server, and retrieve information.


## KEENIO_HTTP : HTTP sockets handler

## KEENIO_CLIENT : KeenIO http_client

## KEENIO_HTTP::reqURL : Request url


## Setting Keys;

	KEENIO_CLIENT kCLIENT; //KEENIO CLIENT
	
	kCLIENT->kHTTP->masterKey("<master_key>");
	
	kCLIENT->kHTTP->readKey("<read_key>");
	
	kCLIENT->kHTTP->writeKey("<write_key>");


## Adding headers
	
	kCLIENT->kHTTP.addHeader("User-Agent", "Carrots/KeenIO HTTP-1.0");


## Adding query params
	
	kCLIENT->kHTTP.addParam("api_key", kHTTP._masterKey);
	kCLIENT->kHTTP.addParam("timeframe", "this_14_days");

	//QUERY URL:: https://api.keen.io/3.0/projects/<project_id>/queries/count?api_key=mykey&timeframe=this_14_days

========================================================================
    Complete example
========================================================================

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

## KeenIO Query Language:
The goal is to build a complete query-language that eases the process of grabbing KeenIO data.

Syntax:

		https://api.keen.io/3.0/projects/56c6fe8690e4bd30596e08ff/queries/count?api_key=<master_key>&event_collection=media_play&timezone=UTC&timeframe=this_14_days
		
		<selector> <collection_name>(<master_key>) <attribute>=<value>
		
		Query Example: count media_play(<project_id>) event_collection=media_play timezone=UTC timeframe=this_14_days
		

In the current version, keenQL syntax is very sensitive to spacing. All `property=value` filters should contain no spaces, unless the value is surrounded by single quotes.

		property='my value' //is ok
		property<>5 //is ok
		property > 3 //is not ok
		property = 'my value' //is not ok
		propert=my value //is not ok

For now, the syntax scope is limited to `sum` and `count`.

Additionally, the only word ordering that matters is when adding filters (using the `if` statement). This must appear at the end.
		

Example:
		KEENIO_QUERYLANGUAGE* keenQL = new KEENIO_QUERYLANGUAGE();
		
		keenQL->KEY("<master_key>"); //MASTER KEY
		
		keenQL->QueryExec("count media_play(<project_id>) event_collection=media_play timezone=UTC timeframe=this_14_days if x>4 | x<100 | path='path to file'");

		printf("---QUERY REQUEST---\n%s", (char*)keenQL->ProcessQuery().c_str());
	
# Adding filters to keenQL queries
		if propertyname=property_value equals
		if propertyname<>property_value not equals
		if propertyname>property_value greater than
		if propertyname<property_value less than
		if propertyname>=property_value greater than equal to
		if propertyname<=property_value less than equal to
		if propertyname^=property_value contains
		if propertyname!^=property_value not contains
		example:
			//if property_name is greater than 45
			if property_name>45
			
			//if property_name contains string 'hello mate'
			if property_name^='hello mate'
			
			count media_play(<project_id>) event_collection=media_play timezone=UTC timeframe=this_14_days if username='Jane doe'

# Exporting report syntax

		.export=file_name.txt >> ...
	
		.export=results.txt >> count media_play(<project_id>) event_collection=media_play timezone=UTC timeframe=this_14_days
	

Disclaimer: This product is in no way affiliated with KeenIO -- This is a community project, though, we would be very open to KeenIO picking up the project in the future! Keen? :D
