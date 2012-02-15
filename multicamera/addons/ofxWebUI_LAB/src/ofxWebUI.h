#pragma once

#include "ofImage.h"
#include "ofUtils.h"
#include "ofURLFileLoader.h"

#include "ofxWebSocket.h"
//#include "ui.pb.h"

#include "Poco/Net/DNS.h"

template <class T>
class ofxWebUI
: public ofxWebSocketProtocol
{
public:
  ofxWebUI()
  {
    binary = true;
  }
  void setup(T& _pb)
  {  
    pb = &_pb;
    
    if (reactor != NULL)
    {
      std::string fqdn = Poco::Net::DNS::thisHost().name();
      url = "http://" + fqdn + ":" + ofToString(reactor->port) + "/";
    }
    
    ofRegisterURLNotification(this);
  }

  void launchBrowser()
  {
    ofLaunchBrowser(url);
  }

  void fetchQRcode()
  {
    std::stringstream googleChartsQRurl;
    googleChartsQRurl
    << "http://chart.googleapis.com/chart?"
    << "chs=320x320&"
    << "cht=qr&"
    << "chld=L|1&"
    << "choe=UTF-8&"
    << "chl=" << url;
    
    //  std::cout << urlencode(url) << std::endl;
    ofLoadURLAsync(googleChartsQRurl.str(), "qrcode");
  }

  void urlResponse(ofHttpResponse& response)
  {
    if(response.status == 200 && response.request.name == "qrcode")
      QRcode.loadImage(response.data);
    else
      std::cout
      << response.status << " " << response.error
      << std::endl;
  }

  T* pb;
  
  ofImage QRcode;

protected:
  void onopen(ofxWebSocketEvent& args)
  {
    if (pbSerialized.empty())
      pb->SerializeToString(&pbSerialized);
    
    args.conn.send(pbSerialized);
  }

  void onclose(ofxWebSocketEvent& args)
  {
    std::cout << "Connection closed" << std::endl;
  }
  
  void onmessage(ofxWebSocketEvent& args)
  {
    T pb_diff;
    
    if (pb_diff.ParseFromString(args.message))
    {
      pb->MergeFrom(pb_diff);
      
      std::string _pbSerialized;
      pb->SerializeToString(&_pbSerialized);
      
      if (_pbSerialized != pbSerialized)
      {
        pbSerialized = _pbSerialized;
        broadcast(args.message);
      }
    }
  }

private:
  std::string url;
  std::string pbSerialized;
};

static const std::string urlencode(const std::string& url);
