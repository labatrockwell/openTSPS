#include "ofxWebSocket.h"

#include "ofEvents.h"
#include "ofUtils.h"

ofxWebSocketReactor* ofxWebSocketReactor::_instance = NULL;

//--------------------------------------------------------------
ofxWebSocketReactor::ofxWebSocketReactor()
: context(NULL)
, waitMillis(50)
{}

//--------------------------------------------------------------
ofxWebSocketReactor::~ofxWebSocketReactor()
{
  exit();
}

//--------------------------------------------------------------
ofxWebSocketReactor& ofxWebSocketReactor::instance()
{
  if (_instance == NULL)
    _instance = new ofxWebSocketReactor();
  
  return *_instance;
}

//--------------------------------------------------------------
void
ofxWebSocketReactor::registerProtocol(const std::string& name,
                                      ofxWebSocketProtocol& protocol)
{
  protocol.idx = protocols.size()+1; // "http" is protocol 0
  protocol.reactor = this;
  protocols.push_back(make_pair(name, &protocol));
}

//--------------------------------------------------------------
ofxWebSocketProtocol* const
ofxWebSocketReactor::protocol(const unsigned int idx)
{
  return (idx < protocols.size())? protocols[idx].second : NULL;
}

//--------------------------------------------------------------
void
ofxWebSocketReactor::close(ofxWebSocketConnection* const conn)
{
  if (conn != NULL && conn->ws != NULL)
    libwebsocket_close_and_free_session(context, conn->ws, LWS_CLOSE_STATUS_NORMAL);
}

//--------------------------------------------------------------
void
ofxWebSocketReactor::setup(const short _port,
                           const std::string _sslCertFilename,
                           const std::string _sslKeyFilename)
{
  port = _port;  
  bool useSSL = (!_sslCertFilename.empty() && !_sslKeyFilename.empty());

  std::string sslCertPath, sslKeyPath;
  const char *_sslCertPath = NULL;
  const char *_sslKeyPath = NULL;
  if (useSSL)
  {
    if (_sslCertFilename.at(0) == '/')
      sslCertPath = _sslCertFilename;
    else
      sslCertPath = ofToDataPath(_sslCertFilename, true);
    _sslCertPath = sslCertPath.c_str();

    if (_sslKeyFilename.at(0) == '/')
      sslKeyPath = _sslKeyFilename;
    else
      sslKeyPath = ofToDataPath(_sslKeyFilename, true);
    _sslKeyPath = sslKeyPath.c_str();
  }  
  
  if (document_root.empty())
    document_root = "web";
  
  if (document_root.at(0) != '/')
    document_root = ofToDataPath(document_root, true);
    
  struct libwebsocket_protocols http_protocol = { "http", lws_callback, 0 };
  struct libwebsocket_protocols null_protocol = { NULL, NULL, 0 };

  lws_protocols.clear();
  lws_protocols.push_back(http_protocol);
  for (int i=0; i<protocols.size(); ++i)
  {
    struct libwebsocket_protocols lws_protocol = {
      protocols[i].first.c_str(),
      lws_callback,
      sizeof(ofxWebSocketConnection)
    };
    lws_protocols.push_back(lws_protocol);
  }
  lws_protocols.push_back(null_protocol);

  int opts = 0;
  context = libwebsocket_create_context(port, interface.c_str(),
                                        &lws_protocols[0],
                                        libwebsocket_internal_extensions,
                                        _sslCertPath, _sslKeyPath,
                                        -1, -1, opts);

	if (context == NULL)
    std::cerr << "libwebsocket init failed" << std::endl;
  else
    startThread(true, false); // blocking, non-verbose
}

//--------------------------------------------------------------
void
ofxWebSocketReactor::exit()
{
  if (context != NULL)
  {
      stopThread();
    libwebsocket_context_destroy(context);
    context = NULL;
  }
}

//--------------------------------------------------------------
void
ofxWebSocketReactor::threadedFunction()
{
  while (isThreadRunning())
  {
    for (int i=0; i<protocols.size(); ++i)
        if (protocols[i].second != NULL){
            //lock();
            protocols[i].second->execute();
            //unlock();
        }
    libwebsocket_service(context, waitMillis);
  }
}

//--------------------------------------------------------------
unsigned int
ofxWebSocketReactor::_allow(ofxWebSocketProtocol* const protocol, const long fd)
{
  std::string client_ip(128, 0);
  std::string client_name(128, 0);

  libwebsockets_get_peer_addresses((int)fd,
                                   &client_name[0], client_name.size(),
                                   &client_ip[0], client_ip.size());
  return protocol->_allowClient(client_name, client_ip);
}

//--------------------------------------------------------------
unsigned int
ofxWebSocketReactor::_notify(ofxWebSocketConnection* const conn,
                             enum libwebsocket_callback_reasons const reason,
                             const char* const _message,
                             const unsigned int len)
{
    
  if (conn == NULL || conn->protocol == NULL)
    return 1;

  std::string message;
  if (_message != NULL && len > 0)
    message = std::string(_message, len);

  ofEvent<ofxWebSocketEvent> evt;
  ofxWebSocketEvent args(*conn, message);

  if      (reason==LWS_CALLBACK_ESTABLISHED)
    ofNotifyEvent(conn->protocol->onopenEvent, args);
  else if (reason==LWS_CALLBACK_CLOSED)
    ofNotifyEvent(conn->protocol->oncloseEvent, args);
  else if (reason==LWS_CALLBACK_SERVER_WRITEABLE)
    ofNotifyEvent(conn->protocol->onidleEvent, args);
  else if (reason==LWS_CALLBACK_BROADCAST)
    ofNotifyEvent(conn->protocol->onbroadcastEvent, args);
  else if (reason==LWS_CALLBACK_RECEIVE)
    ofNotifyEvent(conn->protocol->onmessageEvent, args);

  return 0;
}

//--------------------------------------------------------------
unsigned int
ofxWebSocketReactor::_http(struct libwebsocket *ws,
                           const char* const _url)
{
  std::string url(_url);
  if (url == "/")
    url = "/index.html";

    // why does this need to be done?
    
    if (document_root.empty())
        document_root = "web";
    
    if (document_root.at(0) != '/')
        document_root = ofToDataPath(document_root, true);
    
  std::string ext = url.substr(url.find_last_of(".")+1);
  std::string file = document_root+url;
  std::string mimetype = "text/html";

  if (ext == "ico")
    mimetype = "image/x-icon";
  if (ext == "manifest")
    mimetype = "text/cache-manifest";
  if (ext == "swf")
    mimetype = "application/x-shockwave-flash";
  if (ext == "js")
    mimetype = "application/javascript";
  if (ext == "css")
    mimetype = "text/css";

  if (libwebsockets_serve_http_file(ws, file.c_str(), mimetype.c_str()))
  {
    std::cerr
    << "Failed to send HTTP file " << file << " for " << url
    << std::endl;
  } else {
      cout<<"served file "<<file<<endl;
  }
}

extern "C"
int
lws_callback(struct libwebsocket_context* context,
             struct libwebsocket *ws,
             enum libwebsocket_callback_reasons reason,
             void *user,
             void *data, size_t len)
{
  const struct libwebsocket_protocols* lws_protocol = libwebsockets_get_protocol(ws);
    int idx = lws_protocol? lws_protocol->protocol_index : 0;
    
  ofxWebSocketReactor* const reactor = ofxWebSocketReactor::_instance;
//  ofxWebSocketProtocol* const protocol = reactor->protocols[idx-1].second;
  ofxWebSocketProtocol* const protocol = reactor->protocol(idx-1);
  ofxWebSocketConnection** const conn_ptr = (ofxWebSocketConnection**)user;
  ofxWebSocketConnection* conn;
    
  if (reactor != NULL)
  {
    if (reason == LWS_CALLBACK_ESTABLISHED)
      *conn_ptr = new ofxWebSocketConnection(reactor, protocol);
    else if (reason == LWS_CALLBACK_CLOSED)
      if (*conn_ptr != NULL)
        delete *conn_ptr;

    switch (reason)
    {
        case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
        //TODO: what are the use cases for this callback?
        //1:
            
            //dump_handshake_info((struct lws_tokens *)(long)user);
        return 0;

      case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
        if (protocol != NULL)
          return reactor->_allow(protocol, (int)(long)user)? 0 : 1;
        else
          return 0;
    case LWS_CALLBACK_HTTP:
        return reactor->_http(ws, (char*)data);

    case LWS_CALLBACK_ESTABLISHED:
    case LWS_CALLBACK_CLOSED:
    case LWS_CALLBACK_SERVER_WRITEABLE:
    case LWS_CALLBACK_RECEIVE:
    case LWS_CALLBACK_BROADCAST:
        conn = *(ofxWebSocketConnection**)user;
        if (conn && conn->ws != ws) conn->ws = ws;
        return reactor->_notify(conn, reason, (char*)data, len);
        
      default:
        return 0;
    }
  } else {
      ofLog(OF_LOG_ERROR, "Reactor is null?");
  }

  return 1; // FAIL (e.g. unhandled case/break in switch)
}
