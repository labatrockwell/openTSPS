#include "encode.h"
#include "decode.h"

#include <sstream>
#include <string.h>

//--------------------------------------------------------------
unsigned int b64_decode_string(const char* message,
                               char* decoded,
                               const unsigned int len)
{
  std::stringstream encoded_stream;
  encoded_stream << message;
  
  std::stringstream decoded_stream;
  base64::decoder decoder;
  
  decoder.decode(encoded_stream, decoded_stream);
  
  const std::string& _decoded = decoded_stream.str();
  memcpy(decoded, _decoded.c_str(), _decoded.size());
  
  return _decoded.size();
}

//--------------------------------------------------------------
unsigned int b64_encode_string(const char* message,
                               const unsigned int len,
                               char* encoded,
                               const unsigned int max_encoded_len)
{
  std::stringstream binary_stream;
  binary_stream << std::string(message, len);
  
  std::stringstream encoded_stream;
  base64::encoder encoder;
  
  encoder.encode(binary_stream, encoded_stream);
  
  const std::string& _encoded = encoded_stream.str();
  if (_encoded.size() <= max_encoded_len)
    memcpy(encoded, _encoded.c_str(), _encoded.size());
  
  return _encoded.size();
}
