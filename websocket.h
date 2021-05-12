#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <libwebsockets.h>
#include <ctime>
#include <pushbulletcontroller.h>

#define KGRN "\033[0;32;32m"
#define KCYN "\033[0;36m"
#define KRED "\033[0;32;31m"
#define KYEL "\033[1;33m"
#define KBLU "\033[0;32;34m"
#define KCYN_L "\033[1;36m"
#define KBRN "\033[0;33m"
#define RESET "\033[0m"

static int destroy_flag = 0;
static int connection_flag = 0;
static int lastSyncTimeStamp = 0;   // Init to start up time for first time.
//static int writeable_flag = 0;

extern PushBulletController *pb_handler;

struct session_data {
        int fd;
};

struct pthread_routine_tool {
        struct lws_context *context;
        struct lws *wsi;
};

static int websocket_write_back(struct lws *wsi_in, char *str, int str_size_in)
{
        if (str == NULL || wsi_in == NULL)
                return -1;

        int n;
        int len;
        char *out = NULL;

        if (str_size_in < 1)
                len = strlen(str);
        else
                len = str_size_in;

        out = (char *)malloc(sizeof(char)*(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING));
        //* setup the buffer*/
        memcpy (out + LWS_SEND_BUFFER_PRE_PADDING, str, len );
        //* write out*/
        n = lws_write( wsi_in, (unsigned char *)out + LWS_SEND_BUFFER_PRE_PADDING, len, LWS_WRITE_TEXT );

        printf( "[websocket_write_back] %s\n", str );
        //* free the buffer*/
        free(out);

        return n;
}


static int ws_service_callback(
        struct lws *wsi,
        enum lws_callback_reasons reason, void *user,
        void *in, size_t len)
{

  switch ( reason ) {

  case LWS_CALLBACK_CLIENT_ESTABLISHED:
    printf( "    Connect with server success.\n" );
    connection_flag = 1;
    break;

  case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
    printf( "    Connect with server error.\n" );
    destroy_flag = 1;
    connection_flag = 0;
    break;

  case LWS_CALLBACK_CLOSED:
    printf( "    LWS_CALLBACK_CLOSED\n" );
    destroy_flag = 1;
    connection_flag = 0;
    break;

  case LWS_CALLBACK_CLIENT_RECEIVE: {
    // THIS WON'T DO ON REAL-TIME because it's NOT main thread.
    //printf( "    Client recvived:%s\n", (char *)in);
    // How to issue KDE 4 notification:
    //  system( "kdialog --passivepopup 'who is there'" );

    std::string in_string( (char *)in );
    std::string nop_str( "nop" );

    if ( in_string.find( nop_str ) != string::npos ) {
      // Type = NOP
      // No news is good news
    }
    else if ( in_string.find( "mirror" ) != string::npos ) {
      // Type = Mirror.
      // This is broadcasted from other device.
      Json::CharReaderBuilder builder;
      Json::CharReader *reader = builder.newCharReader();
      Json::Value root;
      const char *in_string_beg = in_string.c_str();
      std::string *err_str = new std::string();
      reader->parse( in_string_beg, in_string_beg + in_string.length(), &root, err_str );
      std::cout<< in_string << std::endl;
      Json::Value push = root["push"];
      Json::Value body = push["body"];
      // What about knotify4
      Json::Value push_type = push["type"];
      if ( push_type.asString() != "dismissal" ) {
	std::string display_cmd( "kdialog --passivepopup '" + body.asString() + "'" );
	std::cout << body.asString() << std::endl;
	system( display_cmd.c_str() );
      }
      int lastEpoch = push["modified"].asInt();
      if ( lastSyncTimeStamp < lastEpoch )
	lastSyncTimeStamp = lastEpoch;
    }
    else {
      // Normal / !Mirrored
      pb_handler->getPushes( true, lastSyncTimeStamp );
      Json::Value push_bodies = pb_handler->jsonRoot["body"];
      Json::Value pushes = pb_handler->jsonRoot["pushes"];
      Json::Value cur;
      for ( unsigned i = 0; i< pushes.size(); i++ ) {
	cur = pushes[i];
	// It could be a URL OR a normal BODY:
	std::string body = cur["body"].asString();
	if ( body == "" )
	  body = cur["url"].asString();

	if ( cur["dismissed"].asString() == "false" ) {  // Don't display the dismissed!
	  std::string display_cmd( "kdialog --passivepopup '" + body + "'" );
	  std::cout << display_cmd << endl;
	  system( display_cmd.c_str() );
	}
      }
      //std::string lastEpoch = cur["modified"].asString();
      cur = pushes[0];
      int lastEpoch = cur["modified"].asInt();
      if ( lastEpoch > lastSyncTimeStamp ) {
	lastSyncTimeStamp = lastEpoch;
	lastSyncTimeStamp++;
      }
    }
    break;
  }
  case LWS_CALLBACK_CLIENT_WRITEABLE :
    printf( "    On writeable is called. send byebye message\n" );
    //     websocket_
    // (wsi, "Byebye! See you later", -1);
    //     writeable_flag = 1;
    break;

  default:
    break;
  }

  return 0;
}

//static void *pthread_routine(void *tool_in)
static void *pthread_routine(void *tool_in)
{
//        struct pthread_routine_tool *tool = (pthread_routine_tool *)tool_in;

        printf( "[pthread_routine] Good day. This is pthread_routine.\n" );

//    //* waiting for connection with server done.*/
//    while(!connection_flag)
//        usleep(1000*20);

//    //*Send greeting to server*/
//    printf(KBRN"[pthread_routine] Server is ready. send a greeting message to server.\n"RESET);
//    websocket_write_back(tool->wsi, "Good day", -1);

//    printf(KBRN"[pthread_routine] sleep 2 seconds then call onWritable\n"RESET);
//    sleep(1);
//    printf(KBRN"------------------------------------------------------\n"RESET);
//    sleep(1);
//    //printf(KBRN"[pthread_routine] sleep 2 seconds then call onWritable\n"RESET);

//    //*involked wriable*/
//    printf(KBRN"[pthread_routine] call on writable.\n"RESET);
//    lws_callback_on_writable(tool->wsi);
        return NULL;
}

struct lws_context *context;

/*
        protocol.name  = "PushBullet protocol";
        protocol.callback = &ws_service_callback;
        protocol.per_session_data_size = sizeof(struct session_data);
        protocol.rx_buffer_size = 0;
        protocol.id = 0;
        protocol.user = NULL;
*/
/*  LATEST DEFINITION:
 struct lws_protocols {
          const char *name;
          lws_callback_function *callback;
          size_t per_session_data_size;
          size_t rx_buffer_size;
          unsigned int id;
          void *user; 
          size_t tx_packet_size;
  }; */ 
static const struct lws_protocols protocols[] = {
						 {
						  "PushBullet protocol",
						  //callback_dumb_increment,
						  &ws_service_callback,
						  sizeof( struct session_data ),  // per_session_data_size
						  0,  // rx_buffer_size
						  0,  // id: IGNORED by libwebsocket
						  NULL,  // user: IGNORED by libwebsocket
						  0   // tx_packet_size: HOW THE HELL SHOULD I KNOW?
						 },
						 /* { */
						 /* 	"lws-mirror-protocol", */
						 /* 	callback_lws_mirror, */
						 /* 	0, */
						 /* 	128, */
						 /* }, { */
						 /* 	"lws-test-raw-client", */
						 /* 	callback_test_raw_client, */
						 /* 	0, */
						 /* 	128 */
						 /* }, */
						 { NULL, NULL, 0, 0, 0, NULL, 0 } /* end */
};

int wss_connect()
{
        struct lws_context_creation_info info;
        struct lws *wsi = NULL;
        //struct lws_protocols protocol;
        lastSyncTimeStamp = std::time( 0 );

        memset( &info, 0, sizeof info );
        info.port = CONTEXT_PORT_NO_LISTEN;
        info.iface = NULL;
	info.protocols = protocols;
        info.ssl_cert_filepath = NULL;
        info.ssl_private_key_filepath = NULL;
        //info.extensions = lws_get_internal_extensions();
        info.gid = -1;
        info.uid = -1;
        info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

        /* protocol.name  = "PushBullet protocol"; */
        /* protocol.callback = &ws_service_callback; */
        /* protocol.per_session_data_size = sizeof(struct session_data); */
        /* protocol.rx_buffer_size = 0; */
        /* protocol.id = 0; */
        /* protocol.user = NULL; */

        context = lws_create_context(&info);
        printf( "[Main] context created.\n" );

        if (context == NULL) {
                printf( "[Main] context is NULL.\n" );
                return -1;
        }

        char path[46];
        strcpy( path, "/websocket/" );
        strcpy( path + 11, pb_handler->access_token.c_str() );
//        wsi = lws_client_connect(context, "stream.pushbullet.com", 443, 2, // 2:WSS
//                                 path, "stream.pushbullet.com:443", NULL,
//                                 protocol.name, -1);
        lws_client_connect_info lws_client_info;
        memset( &lws_client_info, 0, sizeof lws_client_info );
        lws_client_info.context = context;
        lws_client_info.address = "stream.pushbullet.com";
        lws_client_info.port = 443;
        lws_client_info.ssl_connection = LCCSCF_USE_SSL |
            LCCSCF_ALLOW_SELFSIGNED |
            LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;  // for WSS connection
        lws_client_info.path = path;
        lws_client_info.host = "stream.pushbullet.com";
        lws_client_info.origin = NULL;
        //lws_client_info.protocol = protocol.name;
	lws_client_info.protocol = protocols[0].name;
        lws_client_info.ietf_version_or_minus_one = -1;
        lws_client_connect_info *ptr = &lws_client_info;

        wsi = lws_client_connect_via_info( ptr );  // CURRENT WSI FAIL AT SSL_new !!! CHECK Vhost !!
        if ( wsi == NULL ) {
                printf( "[Main] wsi create error.\n" );
                return -1;
        }

        struct pthread_routine_tool tool;
        tool.wsi = wsi;
        tool.context = context;

        pthread_t pid;
        pthread_create(&pid, NULL, pthread_routine, &tool);
        pthread_detach(pid);


        // Original: A ENDLESS while loop for polling !
        while( !destroy_flag )
        {
                lws_service(context, 100);
        }

        //   lws_context_destroy(context);

        return 0;
}

#endif // WEBSOCKET_H
