#pragma once

//--------------------------------------------------------------
unsigned int b64_decode_string(const char* message,
                               char* decoded,
                               const unsigned int len);

//--------------------------------------------------------------
unsigned int b64_encode_string(const char* message,
                               const unsigned int len,
                               char* encoded,
                               const unsigned int max_encoded_len);
