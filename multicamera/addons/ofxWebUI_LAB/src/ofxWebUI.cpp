#include "ofxWebUI.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
static const std::string
urlencode(const std::string& url)
{
  std::stringstream ss;
  static std::string okchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_.-~";
  size_t from = 0;
  size_t to = 0;
  
  while (from < url.size() && to != std::string::npos)
  {
    to = url.find_first_not_of(okchars, from);
    if (to == std::string::npos)
    {
      ss << url.substr(from, url.size()-from);
    }
    else {
      ss << url.substr(from, to-from);
      ss << "%" << std::hex << (unsigned int)url.at(to);
      from = to+1;
    }
  }
  return ss.str();
}
