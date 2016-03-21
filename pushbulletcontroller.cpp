#include "pushbulletcontroller.h"

#include <iostream> // TEMP!
#include <algorithm>

#define BOOL_STR(b) ((b)?"true":"false")

static inline void replaceAll(std::string &str, const std::string& from, const std::string& to)
{
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
}

PushBulletController::PushBulletController( string in_account, string in_password )
{
        account = in_account;
        password = in_password;
        curl_global_init( CURL_GLOBAL_ALL );
        curlHandler = curl_easy_init();
}

PushBulletController::PushBulletController()
{
        curl_global_init( CURL_GLOBAL_ALL );
        curlHandler = curl_easy_init();
}

bool PushBulletController::login()
{       
        return false;
}

void PushBulletController::setAccount( string input )
{
        this->account = input;
}

void PushBulletController::setPassword( string input )
{
        this->password = input;
}

void PushBulletController::setAccessToken( string in_accessToken )
{
        this->access_token = in_accessToken;
}

vector<Device> *PushBulletController::listDevices()
{
    this->setupCommonHeader();
    this->devices_list = new vector<Device>();

        //string devices_url( "https://api.pushbullet.com/v2/devices" );
        curl_easy_setopt( curlHandler, CURLOPT_URL, device_url.c_str() );

        CURLcode res = curl_easy_perform( curlHandler );
        if ( res == CURLE_OK &&
             jsonReader.parse( lastReturnedBuffer, jsonRoot, false ) ) {
                // jsonRoot should be now ready parsed tree of json.
               // cout << "RES is FINE, as:" << endl;
               // cout << lastReturnedBuffer << endl;
                Json::Value devices = jsonRoot["devices"];
                for ( unsigned int i = 0; i < devices.size(); i++ ) {
                        Json::Value cur = devices[i];
                        // Device::{iden|nickname|type}
                        Device *newDevice = new Device();
                        newDevice->iden = cur["iden"].asString();
                        newDevice->nickname = cur["nickname"].asString();
                        newDevice->type = cur["type"].asString();
                        this->devices_list->push_back( *newDevice );
                }
        }
        lastReturnedBuffer = "";
        return this->devices_list;
}

// Presumption: Device is set in this->deviceSelected.
CURLcode PushBulletController::push( string type, string title, string body )
{
        std::cout << "PushBulletController::push" << std::endl;
        lastReturnedBuffer = "";
        curl_easy_setopt( curlHandler, CURLOPT_URL, push_url.c_str() );
        // params should look like:
        //char *jsonObj = "{ \"body\" : \"test body\", \"title\" : \"test title\", \"type\" : \"note\" }";
        // Preprocessing: Escape '\n' for body:
        replaceAll( body, "\n", "\\n" );

        string jsonString = "{ \"body\" : \"" + body + "\", \"title\" : \"" + title + "\", \"type\" : \"" + type + "\" }";
        curl_easy_setopt( curlHandler, CURLOPT_POSTFIELDS, jsonString.c_str() );
        lastResult = curl_easy_perform( curlHandler );
        if ( lastResult == CURLE_OK ) {
                std::cout << lastReturnedBuffer << std::endl;
        }
        return lastResult;
        //  return CURLE_OK;
}


CURLcode PushBulletController::getPushes( bool avoid_deleted, int since )
{
// GET from: https://api.pushbullet.com/v2/pushes == push_url
    lastReturnedBuffer = "";
    this->setupCommonHeader();
//    curl_easy_cleanup( curlHandler );
//    curlHandler = curl_easy_init();
//    struct curl_slist *headers = NULL;
//    string header_str( "Accept-Token: " );
//    header_str += this->access_token;
//    headers = curl_slist_append( headers, header_str.c_str() );

//    if ( headers == NULL ) {
//            std::cout << "HEADER FXXKED" << std::endl;
//    }
//    header_str = "Content-Type: application/json";
//    headers = curl_slist_append( headers, header_str.c_str() );
//    curl_easy_setopt( curlHandler, CURLOPT_HTTPHEADER, headers );
//    curl_easy_setopt( curlHandler, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC );
//    curl_easy_setopt( curlHandler, CURLOPT_USERNAME, this->access_token.c_str() );
//    curl_easy_setopt( curlHandler, CURLOPT_PASSWORD, "any_password" );
//    curl_easy_setopt( curlHandler, CURLOPT_WRITEFUNCTION, WriteCallback );
//    curl_easy_setopt( curlHandler, CURLOPT_WRITEDATA, &lastReturnedBuffer );

      //  string since_str = string( "\"" + since + "\"" );
        string url = string( push_url + "?" + params_string( {
                                { "active", BOOL_STR( avoid_deleted ) },
                                { "modified_after", std::to_string( since ) },
                                        } ) );
        //const string url_copy = string( url );
        //char *output = curl_escape( url_copy.c_str(), url_copy.length );
        
        // URL is correct, if test by CURL command !!
        std::cout << "NEWURL:" << url << std::endl;

        curl_easy_setopt( curlHandler, CURLOPT_URL, url.c_str() );
        lastResult = curl_easy_perform( curlHandler );
        if ( lastResult == CURLE_OK ) {
                std::cout << lastReturnedBuffer << std::endl;
                jsonReader.parse( std::string( lastReturnedBuffer ), jsonRoot, false );
                // Now jsonRoot is ready to use for others.
        }
        return lastResult;
}

// ======================== PRIVATE UTILITIES ========================
void PushBulletController::setupCommonHeader()
{
        struct curl_slist *headers = NULL;
        string header_str( "Accept-Token: " );
        header_str += this->access_token;
        headers = curl_slist_append( headers, header_str.c_str() );
        if ( headers == NULL ) {
                std::cout << "HEADER FXXKED" << std::endl;
        }
        header_str = "Content-Type: application/json";
        headers = curl_slist_append( headers, header_str.c_str() );
        curl_easy_setopt( curlHandler, CURLOPT_HTTPHEADER, headers );
        curl_easy_setopt( curlHandler, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC );
        curl_easy_setopt( curlHandler, CURLOPT_USERNAME, this->access_token.c_str() );
        curl_easy_setopt( curlHandler, CURLOPT_PASSWORD, "any_password" );
        curl_easy_setopt( curlHandler, CURLOPT_WRITEFUNCTION, WriteCallback );
        curl_easy_setopt( curlHandler, CURLOPT_WRITEDATA, &lastReturnedBuffer );
}

CURLcode PushBulletController::post( string &params, string &url )
{
        lastReturnedBuffer = "";
        // params should look like:
        //char* jsonObj = "{ \"name\" : \"Pedro\" , \"age\" : \"22\" }";
        curl_easy_setopt( curlHandler, CURLOPT_URL, url.c_str() );

        // Setup "write-out" supports of libcurl.
        curl_easy_setopt( curlHandler, CURLOPT_WRITEFUNCTION, WriteCallback );
        curl_easy_setopt( curlHandler, CURLOPT_WRITEDATA, &lastReturnedBuffer );
        if ( params != "" )
                curl_easy_setopt( curlHandler, CURLOPT_POSTFIELDS, params.c_str() );
        lastResult = curl_easy_perform( curlHandler );
        return lastResult;
}
