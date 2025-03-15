#include "ai_generator.h"
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include "json.hpp"
using json = nlohmann::json;

void loadAssemblyJSONExplicit(const char* filename, json& jv)
{
    try
    {
        std::ifstream infile(filename);
        infile >> jv;
    }
    catch(const std::exception& e)
    {
        printf("Error parsing JSON: %s\n", e.what());
        exit(1);
    }
}

size_t Callback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string APIRequest(const std::string& url, json requestJson) {
    CURL* curl = curl_easy_init();

    std::string responseString;
    std::string cookies = "JSESSIONID=D615365543DCB29EA86678F21F5D1627";
    std::string jsonPayload = requestJson.dump();
    
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Cookie: " + cookies).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
    }

    // Cleanup
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return responseString;
}

int main(int argc, char** argv, char **envp)
{
    std::string metadata, type;
    
    AIValueSetGenerator generator;

    //parse the JSON 
    json jv = nullptr;
    loadAssemblyJSONExplicit(argv[1], jv);
    bsqon::AssemblyInfo::parse(jv, generator.assembly);
    ValueSetGeneratorEnvironment venv{"var", {}, GenerateContext{}};

    const bsqon::Type* loadtype = generator.assembly.lookupTypeKey(argv[2]);
    
    if(loadtype->isUnresolved()) {
        printf("Invalid 'loadtype' -- %s\n", argv[2]);
        exit(1);
    }

    GenerateContext ctx;
    ctx.extendWithEnclosingType(loadtype);
    auto pp = generator.generateType(loadtype, venv);

    json js = pp.components[0]->options[0]->toJSON();
    std::cout<<js.dump()<<std::endl;
    // std::string url = "http://localhost:50117/customer/cart";
    // std::cout<<APIRequest(url, js)<<std::endl;

    fflush(stdout);
    exit(0);
}


/*
curl 'http://localhost:50117/register' \
--header 'Content-Type: application/json' \
--data-raw '{
    "email": "newuser@example.com",
    "name": "John Doe",
    "password": "securepassword123",
    "phone": "+1 (234)-567-8900",
    "address": "123 Main Street, City, Country"
}'
*/ 
