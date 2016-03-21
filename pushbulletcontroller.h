#ifndef PUSHBULLETCONTROLLER_H
#define PUSHBULLETCONTROLLER_H

#include <string>
#include <vector>
#include <map>
#include "Device.h"
#include <curl/curl.h>
#include <json/json.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

static string push_url( "https://api.pushbullet.com/v2/pushes" );
static string device_url( "https://api.pushbullet.com/v2/devices" );

class PushBulletController
{
private:
        CURL *curlHandler;
        CURLcode lastResult;
        void saveLoginDetails();

        CURLcode post( string &params, string &url );
        void setupCommonHeader();

public:
        PushBulletController( string in_account, string in_password );
        PushBulletController();

        string account;
        string password;
        string access_token;
        Json::Value jsonRoot;   // will contains the root value after parsing.
        Json::Reader jsonReader;
        vector<Device> *devices_list;
        vector<Device>::const_iterator deviceSelected;
        string lastReturnedBuffer;

        void setAccount( string in_account );
        void setPassword( string in_password );
        void setAccessToken( string in_accessToken );
        bool login();  // This will store id_token on succeed try, return FALSE on failure.

        vector<Device> *listDevices();
				
        CURLcode push( string type, string title, string body );
        CURLcode getPushes( bool avoid_deleted, int since ); // Official: active:=whether to avoid deleted pushes.
};

struct MemoryStruct {
				char *memory;
				size_t size;
};

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
				((std::string*)userp)->append((char*)contents, size * nmemb);
				return size * nmemb;
}

typedef std::map<std::string, std::string> Params;

static std::string params_string(Params const &params)
{
        if(params.empty()) return "";
        Params::const_iterator pb= params.cbegin(), pe= params.cend();
        std::string data= pb-> first+ "="+ pb-> second;
        ++ pb;
        if(pb== pe) return data;
        for(; pb!= pe; ++ pb)
                data+= "&"+ pb-> first+ "="+ pb-> second;
        return data;
}

#endif // PUSHBULLETCONTROLLER_H
