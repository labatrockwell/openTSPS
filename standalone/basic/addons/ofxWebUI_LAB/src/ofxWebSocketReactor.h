#pragma once

#include "ofThread.h"
#include "ofEvents.h"

extern "C" {
#include <libwebsockets.h>
}

class ofxWebSocketConnection;
class ofxWebSocketProtocol;

class ofxWebSocketReactor;

class ofxWebSocketReactor
: public ofThread
{
  friend class ofxWebSocketProtocol;
    
public:
  ofxWebSocketReactor();
  ~ofxWebSocketReactor();

  bool setup(const short _port=7681,
             const std::string sslCertFilename="libwebsockets-test-server.pem",
             const std::string sslKeyFilename="libwebsockets-test-server.key.pem");

  void exit();

  void registerProtocol(const std::string& name,
                        ofxWebSocketProtocol& protocol);

  void close(ofxWebSocketConnection* const conn);

  ofxWebSocketProtocol* const protocol(const unsigned int idx);
	
	short port;

//private:
  unsigned int _allow(ofxWebSocketProtocol* const protocol, const long fd);

  unsigned int _notify(ofxWebSocketConnection* const conn,
                       enum libwebsocket_callback_reasons const reason,
                       const char* const _message,
                       const unsigned int len);

  unsigned int _http(struct libwebsocket *ws,
                     const char* const url);

  std::string document_root;

  std::vector<std::pair<std::string, ofxWebSocketProtocol*> > protocols;

    static void dump_handshake_info(struct lws_tokens *lwst)
    {
        int n;
        static const char *token_names[WSI_TOKEN_COUNT] = {
            /*[WSI_TOKEN_GET_URI]		=*/ "GET URI",
            /*[WSI_TOKEN_HOST]		=*/ "Host",
            /*[WSI_TOKEN_CONNECTION]	=*/ "Connection",
            /*[WSI_TOKEN_KEY1]		=*/ "key 1",
            /*[WSI_TOKEN_KEY2]		=*/ "key 2",
            /*[WSI_TOKEN_PROTOCOL]		=*/ "Protocol",
            /*[WSI_TOKEN_UPGRADE]		=*/ "Upgrade",
            /*[WSI_TOKEN_ORIGIN]		=*/ "Origin",
            /*[WSI_TOKEN_DRAFT]		=*/ "Draft",
            /*[WSI_TOKEN_CHALLENGE]		=*/ "Challenge",
            
            /* new for 04 */
            /*[WSI_TOKEN_KEY]		=*/ "Key",
            /*[WSI_TOKEN_VERSION]		=*/ "Version",
            /*[WSI_TOKEN_SWORIGIN]		=*/ "Sworigin",
            
            /* new for 05 */
            /*[WSI_TOKEN_EXTENSIONS]	=*/ "Extensions",
            
            /* client receives these */
            /*[WSI_TOKEN_ACCEPT]		=*/ "Accept",
            /*[WSI_TOKEN_NONCE]		=*/ "Nonce",
            /*[WSI_TOKEN_HTTP]		=*/ "Http",
            /*[WSI_TOKEN_MUXURL]	=*/ "MuxURL",
        };
        
        for (n = 0; n < WSI_TOKEN_COUNT; n++) {
            if (lwst[n].token == NULL)
                continue;
            
            fprintf(stderr, "    %s = %s\n", token_names[n], lwst[n].token);
        }
    }
    
	struct libwebsocket_context *context;  
    
    
protected:
  unsigned int waitMillis;
  std::string interface;

private:
  void threadedFunction();  
  
  std::vector<struct libwebsocket_protocols> lws_protocols;
};

static vector<ofxWebSocketReactor *> reactors;

extern "C"
int
lws_callback(struct libwebsocket_context *context,
             struct libwebsocket *ws,
             enum libwebsocket_callback_reasons reason,
             void *user,
             void *_message, size_t len);
