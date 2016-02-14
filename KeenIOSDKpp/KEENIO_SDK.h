#pragma once


#include "cJSON.h"

using namespace std;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

#define keenproperty string


class KEENIO_PROPERTY {

public:
	string* property_name;
	string* url;


	void set(string p, string u) {
		this->property_name = new string(p);
		this->url = new string(u);
	}

};


class KEENIO_EVENT {
private:
	int _count = 0;
	map<int, KEENIO_PROPERTY> properties_MAPINT;

public:
	map<keenproperty, KEENIO_PROPERTY> properties;

	KEENIO_EVENT* add(KEENIO_PROPERTY* kprop) {
		this->properties[*kprop->property_name] = *kprop;
		this->properties_MAPINT[this->_count] = *kprop;

		this->_count++;


		return this;
	}

	KEENIO_PROPERTY get(keenproperty keen_property) {
		return this->properties[keen_property];
	}

	int count() {
		return this->_count;
	}

	void clear() {
		this->properties.clear();
		this->properties_MAPINT.clear();
	}
};

/*
class KEENIO_EVENTS {
private:
int _count = 0;

public:
KEENIO_EVENT events;


void add(int index, string param_n, string param_v) {
this->events[index][param_n] = param_v;

this->_count++;
}


void add(map<string, string> event) {
this->events[_count] = event;

this->_count++;
}

int count() {
return this->_count;
}

};
*/


class KEENIO_COLLECTION {

private:
	int _count = 0;
	map<int, map<string, string>> properties_MAPINT;

public:
	string name;
	string url;
	map<string, string> properties;


	void addProperty(string keen_property, string keen_value) {
		this->properties[keen_property] = keen_value;
		this->properties_MAPINT[_count][keen_property] = keen_value;

		this->_count++;
	}

	void clear() {
		this->properties.clear();
	}

	int count() {
		return this->_count;
	}
};

class KEENIO_PROJECT {

private:
	int keenevent_count = 0;
	size_t* keencollection_count = new size_t();

public:
	string partners_url = "";
	string name = "";
	string url = "";
	map<int, KEENIO_EVENT> events;
	string events_url = "";
	string id = "";
	string organization_id = "";
	string queries_url = "";
	string api_key = "";
	map<int, KEENIO_COLLECTION> collections;
	string saved_queries_url = "";

	KEENIO_EVENT* addEvent(KEENIO_PROPERTY* kprop = NULL) {
		keenevent_count++;

		if (kprop == NULL) {

			*kprop->property_name = (const string)STRING_UTILS::gen_random_string(20);
		}


		return this->events[keenevent_count].add(kprop);

	}

	KEENIO_COLLECTION* addCollection(KEENIO_COLLECTION* kCollection = NULL) {
		size_t* sz = new size_t();
		*sz = collections.size();


		if (kCollection == NULL) {

			kCollection->name = (const string)STRING_UTILS::gen_random_string(20);
		}

		this->collections[*this->keencollection_count] = *kCollection;

		*this->keencollection_count = collections.size();
		return &this->collections[*this->keencollection_count - 1];
	}

	void clearEvents() {
		this->events.clear();
		this->keenevent_count = 0;
		//http request
	}

	void clearCollections() {
		this->collections.clear();
		this->keencollection_count = new size_t();
		//http request
	}

};

class KEENIO_URLS {
	string inspectAllProjects = "https://api.keen.io/3.0/projects?api_key={MASTER_KEY}";
	string inspectProject = "https://api.keen.io/3.0/projects/{PROJECT_ID}?api_key={MASTER_KEY}";
};



class KEEN_IO {

private:
	int _count = 0;

public:
	map <int, KEENIO_PROJECT*> projects;
	map<string, KEENIO_PROJECT*> projects_MAP_S;

	KEENIO_PROJECT* addProject(KEENIO_PROJECT* keen_project = NULL) {

		if (keen_project == NULL) {
			keen_project = new KEENIO_PROJECT();
		}

		this->projects[this->_count] = keen_project;
		this->projects_MAP_S[keen_project->name] = keen_project;

		this->_count++;

		return this->projects[this->_count - 1];
	}

	KEENIO_PROJECT* getProject(int index) {

		return this->projects[index];
	}

	/*KEENIO_PROJECT* getProject(string project_name) {

	return this->projects_MAP_S[project_name];
	}*/

	int projectCount() {
		return this->_count;
	}

};

namespace KEENIO_REST {

	namespace AUTH {
		const string MASTER_KEY = "";
		const string READ_KEY = "";
		const string WRITE_KEY = "";
	};

	namespace ANALYSES {

		class KEENIO_QUERY {
		public:
			string query_name;
			map<string, cJSON*> queries;
			map<int, cJSON*> queries_MAPINT;

			cJSON* getItemByName(cJSON* item, string name) {
				while (item != NULL) {
					if (item->string != NULL) {
						if (item->string == name) {

							return item;
						}
					}


					item = item->next;
				}
			}

			string toString(cJSON* jsonObj) {
				return cJSON_Print(jsonObj);
			}

			void requestSavedQueries(string project_id) {
			RESTART:
				string collection;
				string url = "https://api.keen.io/3.0/projects/" + project_id + "/queries/saved?api_key=" + AUTH::MASTER_KEY;

				web::uri* uriL;
				utility::string_t tt;
				wstring tg = STRING_UTILS::utf8toUtf16(url);

				tt.append(tg);
				uriL = new web::uri(tg);

				http_client client(tg);
				Concurrency::task<web::http::http_response> resp = client.request(methods::GET);
				collection = STRING_UTILS::fromWString(resp.get().to_string());

				int startpos = collection.find("[");
				int startpos2 = collection.find("{");

				if (startpos2 > -1 && startpos2 < startpos) {
					startpos = startpos2;
				}

				if (!startpos) {
					startpos = collection.find("{");
				}

				if (startpos < 0) {

					printf("\nKEENIO REST ERROR!.... TRYING AGAIN!\n");
					goto RESTART;
				}

				collection = collection.substr(startpos, collection.length() - 2);

				cJSON *root = cJSON_Parse(collection.c_str());

				int childCount = cJSON_GetArraySize(root);
				for (int i = 0; i < childCount; i++) {
					cJSON* tmpJsonObj = cJSON_GetArrayItem(cJSON_GetArrayItem(root, i), 0);
					queries[this->getItemByName(tmpJsonObj, "query_name")->valuestring] = tmpJsonObj;
					queries_MAPINT[queries_MAPINT.size()] = queries[this->getItemByName(tmpJsonObj, "query_name")->valuestring];
				}

				return;

			}

			cJSON* getQuery(int index) {
				return queries_MAPINT[index];
			}

			string savedQueryResults(string project_id, cJSON* jsonObj) {
			RESTART:
				string collection;
				string url = "https://api.keen.io/3.0/projects/" + project_id + "/queries/saved/" + jsonObj->valuestring + "/result?api_key=" + AUTH::MASTER_KEY;

				web::uri* uriL;
				utility::string_t tt;
				wstring tg = STRING_UTILS::utf8toUtf16(url);

				tt.append(tg);
				uriL = new web::uri(tg);

				http_client client(tg);
				Concurrency::task<web::http::http_response> resp = client.request(methods::GET);
				collection = STRING_UTILS::fromWString(resp.get().to_string());

				int startpos = collection.find("[");
				int startpos2 = collection.find("{");

				if (startpos2 > -1 && startpos2 < startpos) {
					startpos = startpos2;
				}

				if (!startpos) {
					startpos = collection.find("{");
				}

				if (startpos < 0) {

					printf("\nKEENIO REST ERROR!.... TRYING AGAIN!\n");
					goto RESTART;
				}

				collection = collection.substr(startpos, collection.length() - 2);

				cJSON *root = cJSON_Parse(collection.c_str());

				return collection;

			}

			string savedQueryResults(string project_id, string query_name) {
			RESTART:
				string collection;
				string url = "https://api.keen.io/3.0/projects/" + project_id + "/queries/saved/" + query_name + "/result?api_key=" + AUTH::MASTER_KEY;

				web::uri* uriL;
				utility::string_t tt;
				wstring tg = STRING_UTILS::utf8toUtf16(url);

				tt.append(tg);
				uriL = new web::uri(tg);

				http_client client(tg);
				Concurrency::task<web::http::http_response> resp = client.request(methods::GET);
				collection = STRING_UTILS::fromWString(resp.get().to_string());

				int startpos = collection.find("[");
				int startpos2 = collection.find("{");

				if (startpos2 > -1 && startpos2 < startpos) {
					startpos = startpos2;
				}

				if (!startpos) {
					startpos = collection.find("{");
				}

				if (startpos < 0) {

					printf("\nKEENIO REST ERROR!.... TRYING AGAIN!\n");
					goto RESTART;
				}

				collection = collection.substr(startpos, collection.length() - 2);

				cJSON *root = cJSON_Parse(collection.c_str());

				return collection;

			}
		};

	}

	static void requestEvents(KEENIO_PROJECT* keenProj, string project_id, bool localLoad = false, string response_body = "") {
	RESTART:
		string collection;
		if (localLoad) {
			string fUrl = "https://api.keen.io/3.0/projects/" + project_id + "?api_key=" + KEENIO_REST::AUTH::MASTER_KEY;

			web::uri* uriL;
			utility::string_t tt;
			wstring tg = STRING_UTILS::utf8toUtf16(fUrl);

			tt.append(tg);
			uriL = new web::uri(tg);

			http_client client(tg);
			Concurrency::task<web::http::http_response> resp = client.request(methods::GET);
			collection = STRING_UTILS::fromWString(resp.get().to_string());

			int startpos = collection.find("[");
			int startpos2 = collection.find("{");

			if (startpos2 > -1 && startpos2 < startpos) {
				startpos = startpos2;
			}

			if (!startpos) {
				startpos = collection.find("{");
			}

			if (startpos < 0) {

				printf("\nKEENIO REST ERROR!.... TRYING AGAIN!\n");
				goto RESTART;
			}

			collection = collection.substr(startpos, collection.length() - 2);
		}
		else {
			collection = response_body;
		}

		cJSON *root2 = cJSON_Parse(collection.c_str());
		cJSON *collect = cJSON_GetArrayItem(root2, 1);
		cJSON *collect2 = collect->next;
		collect2 = collect2->next;
		collect2 = collect2->next;
		collect2 = collect2->next;
		collect2 = collect2->next;
		collect2 = collect2->next;
		collect2 = collect2->next;
		collect2 = collect2->next;

		int collect_size = cJSON_GetArraySize(collect2);
		(*keenProj).clearEvents();
		for (int i = 0; i < collect_size; i++) {
			//collect = cJSON_GetArrayItem(root2, 0);

			cJSON* props = cJSON_GetArrayItem(collect2, i);
			int propssize = cJSON_GetArraySize(props);
			KEENIO_EVENT kCollection;

			KEENIO_PROPERTY kProp;
			kProp.property_name = new string(cJSON_GetArrayItem(props, 0)->valuestring);
			kProp.url = new string(cJSON_GetArrayItem(props, 1)->valuestring);


			(*keenProj).addEvent(&kProp);
		}
	}

	static void requestCollections(KEENIO_PROJECT *keenProj, string project_id) {
	RESTART:
		string fUrl = "https://api.keen.io/3.0/projects/" + project_id + "/events?api_key=" + KEENIO_REST::AUTH::READ_KEY;

		web::uri* uriL;
		utility::string_t tt;
		wstring tg = STRING_UTILS::utf8toUtf16(fUrl);

		tt.append(tg);
		uriL = new web::uri(tg);

		http_client client(tg);
		Concurrency::task<web::http::http_response> resp = client.request(methods::GET);
		string collection = STRING_UTILS::fromWString(resp.get().to_string());

		int startpos = collection.find("[");
		int startpos2 = collection.find("{");

		if (startpos2 > -1 && startpos2 < startpos) {
			startpos = startpos2;
		}

		if (!startpos) {
			startpos = collection.find("{");
		}

		if (startpos < 0) {

			printf("\nKEENIO REST ERROR!.... TRYING AGAIN!\n");
			goto RESTART;
		}

		collection = collection.substr(startpos, collection.length() - 2);

		cJSON *root2 = cJSON_Parse(collection.c_str());
		cJSON *collect = cJSON_GetArrayItem(root2, 0);


		int root2_size = cJSON_GetArraySize(root2);
		(*keenProj).clearCollections();
		for (int i = 0; i < root2_size; i++) {
			collect = cJSON_GetArrayItem(root2, 0);

			cJSON* props = cJSON_GetArrayItem(collect, i);
			int propssize = cJSON_GetArraySize(props);
			KEENIO_COLLECTION kCollection;

			kCollection.url = cJSON_GetObjectItem(collect, "url")->valuestring;
			kCollection.name = cJSON_GetObjectItem(collect, "name")->valuestring;
			for (int ii = 0; ii < propssize; ii++) {
				kCollection.addProperty(cJSON_GetArrayItem(props, ii)->string, cJSON_GetArrayItem(props, ii)->valuestring);
			}
			(*keenProj).addCollection(&kCollection);
		}
	}

	static void requestProject(KEENIO_PROJECT* keenProj, string project_id, bool loadCollections = false, bool loadEvents = false) {
	RESTART:


		string fUrl = "https://api.keen.io/3.0/projects/" + project_id + "?api_key=" + KEENIO_REST::AUTH::MASTER_KEY;

		web::uri* uriL;
		utility::string_t tt;
		wstring tg = STRING_UTILS::utf8toUtf16(fUrl);

		tt.append(tg);
		uriL = new web::uri(tg);

		http_client client(tg);


		Concurrency::task<web::http::http_response> resp = client.request(methods::GET);

		wstring responseBody = resp.get().to_string();

		string body = STRING_UTILS::fromWString(responseBody);

		int startpos = body.find("[");
		int startpos2 = body.find("{");

		if (startpos2 > -1 && startpos2 < startpos) {
			startpos = startpos2;
		}

		if (!startpos) {
			startpos = body.find("{");
		}

		if (startpos < 0) {

			printf("\nKEENIO REST ERROR!.... TRYING AGAIN!\n");
			goto RESTART;
		}

		body = body.substr(startpos, body.length() - 1);

		cJSON *root = cJSON_Parse(body.c_str());

		cJSON *proj = cJSON_GetArrayItem(root, 0);
		cJSON *projD;

		projD = proj;
		//for (; projD = proj->next )
		(*keenProj).partners_url = projD->valuestring;

		projD = projD->next;
		(*keenProj).name = projD->valuestring;
		projD = projD->next;
		(*keenProj).url = projD->valuestring;
		projD = projD->next;
		projD = projD->next;
		(*keenProj).events_url = projD->valuestring;
		projD = projD->next;
		(*keenProj).id = projD->valuestring;
		projD = projD->next;
		(*keenProj).organization_id = projD->valuestring;
		projD = projD->next;
		(*keenProj).queries_url = projD->valuestring;
		projD = projD->next;
		(*keenProj).api_key = projD->valuestring;
		projD = projD->next;
		projD = projD->next;
		(*keenProj).saved_queries_url = projD->valuestring;

		if (loadCollections) {
			requestCollections(keenProj, project_id);
		}

		if (loadEvents) {
			requestEvents(keenProj, project_id, false, body);
		}

	}
}