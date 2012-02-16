#include "ofxWebSocket.h"
#include "ofxWebSocketUtil.h"

//--------------------------------------------------------------
ofxWebSocketConnection::ofxWebSocketConnection(ofxWebSocketReactor* const _reactor,
                                               ofxWebSocketProtocol* const _protocol,
                                               const bool _supportsBinary)
: reactor(_reactor)
, protocol(_protocol)
, ws(NULL)
, session(NULL)
, supportsBinary(_supportsBinary)
//, buf(LWS_SEND_BUFFER_PRE_PADDING+1024+LWS_SEND_BUFFER_POST_PADDING)
{
	if (_protocol != NULL){
		binary = _protocol->binary;
		bufsize = 1024;
		buf = (unsigned char*)calloc(LWS_SEND_BUFFER_PRE_PADDING+bufsize+LWS_SEND_BUFFER_POST_PADDING, sizeof(unsigned char));
	}
	
}

ofxWebSocketConnection::~ofxWebSocketConnection(){
	delete buf;
}
//--------------------------------------------------------------
void
ofxWebSocketConnection::close()
{
  if (reactor != NULL)
    reactor->close(this);
	
}

//--------------------------------------------------------------
void
ofxWebSocketConnection::send(const std::string& message)
{
	int n = 0;
	if(message.size() > 4096){
		return;
	}
	if(message.size() > bufsize){
		bufsize = bufsize+1024;
		buf = (unsigned char*)realloc(buf, bufsize + LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING*sizeof(unsigned char));
		cout << "ofxWebSocketConnection -- received large message, resizing buffer to " << bufsize << endl;
	}
	unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];
	
  if (binary)
  {
    //TODO: when libwebsockets has an API supporting something this, we should use it
    if (supportsBinary)
    {
      memcpy(p, message.c_str(), message.size());
      n = libwebsocket_write(ws, p, message.size(), LWS_WRITE_BINARY);
    }
    else {
      int encoded_len;
      //encoded_len = b64_encode_string(message.c_str(), message.size(), (char*)p, buf.size());
	  encoded_len = b64_encode_string(message.c_str(), message.size(), (char*)p, bufsize-LWS_SEND_BUFFER_PRE_PADDING-LWS_SEND_BUFFER_POST_PADDING);
      if (encoded_len > 0)
        n = libwebsocket_write(ws, p, encoded_len, LWS_WRITE_TEXT);
    }
  }
  else {
	  memcpy(p, message.c_str(), message.size());
	  n = libwebsocket_write(ws, p, message.size(), LWS_WRITE_TEXT);
  }
  
  if (n < 0)
    std::cout << "ERROR writing to socket" << std::endl;
}

//--------------------------------------------------------------
const std::string
ofxWebSocketConnection::recv(const std::string& message)
{
  std::string decoded = message;

  //TODO: when libwebsockets has an API
  // to detect binary support, we should use it
  if (binary && !supportsBinary)
  {
    //TODO: libwebsockets base64 decode is broken @2011-06-19
    //len = lws_b64_decode_string(message, decoded, len);
    int decoded_len = b64_decode_string(message.c_str(), &decoded[0], message.size());
    decoded.resize(decoded_len);
  }

  return decoded;
}



