#ifndef HTTP_HPP
#define HTTP_HPP

#include <string>
#include <map>

#include <curlpp/cURLpp.hpp>

using namespace std;

namespace Hideyoshi {
    namespace Http {
        size_t writer(char *, size_t, size_t, void *);

        string * get(string, map<string, string>);
        string * post(string, map<string, string>, string);
    }
}

#endif
