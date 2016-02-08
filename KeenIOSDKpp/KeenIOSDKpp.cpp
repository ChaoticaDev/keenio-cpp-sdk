// KeenIOSDKpp.cpp : Defines the entry point for the console application.
//

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <string>
#include "STRING_UTILS.h"
#include "KEENIO_SDK.h"


	cJSON *root;

	using namespace KEENIO_REST;


int main()
{
	KEEN_IO keen;


	KEENIO_PROJECT* keenProj = new KEENIO_PROJECT();

	keenProj = keen.addProject();

	requestProject(keenProj, "5645481d59949a52de2df5ce", true, true);

	KEENIO_PROJECT* projReference = keen.getProject(0);
	
	//requestProject(keenProj, "5645481d59949a52de2df5ce", false, false);
	//requestCollections(keenProj, "5645481d59949a52de2df5ce");
	//requestEvents(keenProj, "5645481d59949a52de2df5ce");

	return 0;
}

