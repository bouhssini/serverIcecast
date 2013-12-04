#ifndef BASE64_H
#define BASE64_H
#include <string>

std::string base64_encode(std::string txt, unsigned int len);
std::string base64_decode(std::string const& s);

#endif // BASE64_H
