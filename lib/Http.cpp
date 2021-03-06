#include "Http.hpp"

#include <list>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

namespace Hideyoshi {
    namespace Http {
        size_t writer(char *ptr, size_t size, size_t nmemb, void *body) {
            string *strptr = (string *) body;

            strptr->append(ptr);

            return nmemb;
        }

        string * post(string url, map<string, string> headers, string body) {
            cURLpp::Easy req;
            string *res = new string();
            list<string> header;

            cURLpp::options::WriteFunctionCurlFunction cb(writer);
            cURLpp::OptionTrait<void *, CURLOPT_WRITEDATA> data(res);
            cURLpp::options::PostFields fields(body);
            cURLpp::options::PostFieldSize fieldsLen(body.size());

            req.setOpt<cURLpp::options::Url>(url);
            req.setOpt(cb);
            req.setOpt(data);
            req.setOpt(fields);
            req.setOpt(fieldsLen);

            for (auto hdr : headers) {
                header.push_back(hdr.first + ": " + hdr.second);
            }

            req.setOpt<cURLpp::options::HttpHeader>(header);

            req.perform();

            return res;
        }

        string * get(string url, map<string, string> headers) {
            cURLpp::Easy req;
            string *body = new string();
            list<string> header;

            cURLpp::options::WriteFunctionCurlFunction cb(writer);
            cURLpp::OptionTrait<void *, CURLOPT_WRITEDATA> data(body);

            req.setOpt<cURLpp::options::Url>(url);
            req.setOpt(cb);
            req.setOpt(data);

            for (auto hdr : headers) {
                header.push_back(hdr.first + ": " + hdr.second);
            }

            req.setOpt<cURLpp::options::HttpHeader>(header);

            req.perform();

            return body;
        }
    }
}
