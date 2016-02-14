// KeenIOSDKpp.cpp : Defines the entry point for the console application.
//

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <string>
#include "STRING_UTILS.h"
#include "KEENIO_SDK.h"


cJSON *root;

using namespace KEENIO_REST;
using namespace KEENIO_REST::ANALYSES;


int main()
{

	KEEN_IO keen;


	KEENIO_PROJECT* keenProj = new KEENIO_PROJECT();

	keenProj = keen.addProject();

	requestProject(keenProj, "56b6369196773d7eaa5f4bca", true, true);

	KEENIO_PROJECT* projReference = keen.getProject(0);

	KEENIO_REST::ANALYSES::KEENIO_QUERY q;
	q.requestSavedQueries("56b6369196773d7eaa5f4bca");
	cJSON* query = q.getQuery(0);

	//GET RESULTS OF UNKNOWN QUERY NAME, OR INDEX
	cJSON* queryJSON = q.getItemByName(query, "query_name");
	printf(q.savedQueryResults("56b6369196773d7eaa5f4bca", queryJSON).c_str());

	//GET RESULTS OF KNOWN QUERY NAME
	printf("\n\n%s\n\n", q.savedQueryResults("56b6369196773d7eaa5f4bca", "pageviews").c_str());

	return 0;
}

