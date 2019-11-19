#include "pushbulletcontroller.h"

#include <iostream> // TEMP!
#include <algorithm>

#define BOOL_STR(b) ((b)?"true":"false")

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
        this->setupCommonHeader( lastReturnedBuffer );
        this->devices_list = new vector<Device>();

        //string devices_url( "https://api.pushbullet.com/v2/devices" );
        curl_easy_setopt( curlHandler, CURLOPT_URL, device_url.c_str() );

        CURLcode res = curl_easy_perform( curlHandler );
        if ( res == CURLE_OK &&
             jsonReader->parse( lastReturnedBuffer.c_str(), lastReturnedBuffer.c_str() + lastReturnedBuffer.length(), &jsonRoot, nullptr ) ) {
                // jsonRoot should be now ready parsed tree of json.
                Json::Value devices = jsonRoot["devices"];
                for ( unsigned int i = 0; i < devices.size(); i++ ) {
                        Json::Value cur = devices[i];
                        // Device::{iden|nickname|type}
                        Device *newDevice = new Device();
                        if ( !cur["nickname"].asString().empty() ) {
                            newDevice->iden = cur["iden"].asString();
                            newDevice->nickname = cur["nickname"].asString();
                            newDevice->type = cur["type"].asString();
                            this->devices_list->push_back( *newDevice );
                        }
                }
                // Add "ALL" device
                Device *allDevice = new Device();
                allDevice->iden = string( "ALL" );
                allDevice->nickname = string( "ALL" );
                allDevice->type = string( "" );
                this->devices_list->push_back( *allDevice );
        }
        lastReturnedBuffer = "";
        return this->devices_list;
}

// Presumption: Device is set in this->deviceSelected.
CURLcode PushBulletController::push( string type, string title, string body, string device_iden )
{
        pthread_mutex_lock(&mutex);
        std::cout << "PushBulletController::push" << std::endl;
        lastReplyBuffer = "";
        this->setupCommonHeader( lastReplyBuffer );
        // lastReturnedBuffer = "";
        curl_easy_setopt( curlHandler, CURLOPT_URL, push_url.c_str() );
        // curl_easy_setopt( curlHandler, CURLOPT_WRITEDATA, &lastReplyBuffer );
        // params should look like:
        //char *jsonObj = "{ \"body\" : \"test body\", \"title\" : \"test title\", \"type\" : \"note\" }";
        // Preprocessing: Escape '\n' for body:
        replaceAll( body, "\n", "\\n" );
        string jsonString;
        if ( device_iden.empty() || device_iden == "ALL" ) {
            jsonString = "{ \"body\" : \"" + body +
                    "\", \"title\" : \"" + title +
                    "\", \"type\" : \"" + type +
                    "\" }";
        }
        else {
            jsonString = "{ \"body\" : \"" + body +
                    "\", \"title\" : \"" + title +
                    "\", \"type\" : \"" + type +
                    "\", \"device_iden\" : \"" + device_iden +
                    "\" }";
        }
        curl_easy_setopt( curlHandler, CURLOPT_POSTFIELDS, jsonString.c_str() );
        lastResult = curl_easy_perform( curlHandler );
        if ( lastResult == CURLE_OK ) {
                //     std::cout << lastReturnedBuffer << std::endl;
                std::cout << "PUSH REPLY:" << lastReplyBuffer << std::endl;
        }
        pthread_mutex_unlock(&mutex);
        return lastResult;
        //  return CURLE_OK;
}


CURLcode PushBulletController::getPushes( bool avoid_deleted, int since )
{
// GET from: https://api.pushbullet.com/v2/pushes == push_url
        pthread_mutex_lock(&mutex);

        lastReturnedBuffer = "";
        this->setupCommonHeader( lastReturnedBuffer );

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
        curl_easy_setopt( curlHandler, CURLOPT_HTTPGET, 1 );
        lastResult = curl_easy_perform( curlHandler );
        if ( lastResult == CURLE_OK ) {
                std::cout << "GetPush:" << lastReturnedBuffer << std::endl;
         //       jsonReader.parse( std::string( lastReturnedBuffer ), jsonRoot, false );
                jsonReader->parse( lastReturnedBuffer.c_str(), lastReturnedBuffer.c_str() + lastReturnedBuffer.length(), &jsonRoot, nullptr );

                // Now jsonRoot is ready to use for others.
        }
        pthread_mutex_unlock(&mutex);

        return lastResult;
}

// stub: typeIcon
string PushBulletController::registerDevice( string nickname, string manufacturer, string model, string typeIcon ) {
  pthread_mutex_lock(&mutex);
  lastReturnedBuffer = "";
  this->setupCommonHeader( lastReturnedBuffer );

  curl_easy_setopt( curlHandler, CURLOPT_URL, device_url.c_str() );
  curl_easy_setopt( curlHandler, CURLOPT_WRITEDATA, &lastReplyBuffer );
  // params should look like:
  //char *jsonObj = "{ \"body\" : \"test body\", \"title\" : \"test title\", \"type\" : \"note\" }";
  // Preprocessing: Escape '\n' for body:
  //replaceAll( body, "\n", "\\n" );
  string jsonString;
  jsonString = "{ \"nickname\" : \"" + nickname +
              "\", \"model\" : \"" + model +
              "\", \"manufacturer\" : \"" + manufacturer +
              "\", \"push_token\" : \"" + "" +
              "\", \"app_version\" : \"" + "8623" +
              "\", \"icon\" : \"" + "laptop" +
              "\", \"has_sms\" : " + "false" +
              " }";
  std::cout << "jsonString:   " << jsonString << std::endl;
  curl_easy_setopt( curlHandler, CURLOPT_POSTFIELDS, jsonString.c_str() );
  lastResult = curl_easy_perform( curlHandler );
  if ( lastResult == CURLE_OK ) {  // Register SUCCESS
          //     std::cout << lastReturnedBuffer << std::endl;
          std::cout << "Create-Device REPLY:" << lastReplyBuffer << std::endl;
          //    Parse the JSON response for "iden" value:
          if ( jsonReader->parse( lastReplyBuffer.c_str(), lastReplyBuffer.c_str() + lastReplyBuffer.length(), &jsonRoot, nullptr ) ) {
//                  Json::Value devices = jsonRoot["devices"];
//                  for ( unsigned int i = 0; i < devices.size(); i++ ) {
//                          Json::Value cur = devices[i];
//                          // Device::{iden|nickname|type}
//                          Device *newDevice = new Device();
//                          if ( !cur["nickname"].asString().empty() ) {
//                              newDevice->iden = cur["iden"].asString();
//                              newDevice->nickname = cur["nickname"].asString();
//                              newDevice->type = cur["type"].asString();
//                              this->devices_list->push_back( *newDevice );
//                          }
//                  }
//                  // Add "ALL" device
//                  Device *allDevice = new Device();
//                  allDevice->iden = string( "ALL" );
//                  allDevice->nickname = string( "ALL" );
//                  allDevice->type = string( "" );
//                  this->devices_list->push_back( *allDevice );
          }
  }
  pthread_mutex_unlock(&mutex);
  return "lastResult";
}

// ======================== PRIVATE UTILITIES ========================
void PushBulletController::setupCommonHeader( string &filling_buffer )
{
        struct curl_slist *headers = nullptr;
        string header_str( "Accept-Token: " );
        header_str += this->access_token;
        headers = curl_slist_append( headers, header_str.c_str() );
        if ( headers == nullptr ) {
                std::cout << "HEADER FXXKED" << std::endl;
        }
        header_str = "Content-Type: application/json";
        headers = curl_slist_append( headers, header_str.c_str() );
        curl_easy_setopt( curlHandler, CURLOPT_HTTPHEADER, headers );
        curl_easy_setopt( curlHandler, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC );
        curl_easy_setopt( curlHandler, CURLOPT_USERNAME, this->access_token.c_str() );
        curl_easy_setopt( curlHandler, CURLOPT_PASSWORD, "any_password" );
        curl_easy_setopt( curlHandler, CURLOPT_WRITEFUNCTION, WriteCallback );
        curl_easy_setopt( curlHandler, CURLOPT_WRITEDATA, &filling_buffer );
}
