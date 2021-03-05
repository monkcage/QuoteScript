#ifndef MDAPI_INC
#define MDAPI_INC

#include <iostream>
#include <sstream>
#include <atomic>
#include <random>
#include <functional>
#include <algorithm>


#ifdef __cplusplus
extern "C" {
#endif
#include "mongoose.h"
#ifdef __cplusplus
}
#endif


class MdApi {
    static const std::string HOST;
    static const std::string md_uri;
    static const std::string subfix_uri;
    static const std::string USER_AGENT;

public:
    MdApi()
        : m_stop(false)
        , m_random(std::uniform_int_distribution<unsigned>(10000000, 99999999))
    {
        mg_log_set("0");
        mg_mgr_init(&m_mgr);
        m_symbols.push_back("0000001"); // default symbol
        this->init();
    }

    ~MdApi()
    {
        mg_mgr_free(&m_mgr);
    }

    void init()
    {
        std::string symbols;
        for(auto&& symbol: m_symbols){
            symbols += symbol;
            symbols += ",";
        }
        std::string url {MdApi::HOST + MdApi::md_uri + symbols + MdApi::subfix_uri + std::to_string(m_random(m_engine))};
        m_current_url = url;
        mg_http_connect(&m_mgr, url.c_str(), MdApi::subscribe_handler, this);
    }

    void Subscribe(std::string symbol)
    {
        m_symbols.push_back(symbol);
    }

    void Run()
    {
        while(!m_stop) {
//            std::cout << "pool ..." << std::endl;
            mg_mgr_poll(&m_mgr, 1000000);
        }
    }

    static void subscribe_handler(struct mg_connection* conn, int type, void* ev_data, void* fn_data)
    {
        MdApi* api = reinterpret_cast<MdApi*>(fn_data);
        mg_http_message *msg = nullptr;
        if(type == MG_EV_CONNECT) {
            std::cout << "connect..." << std::endl;
            struct mg_str host = mg_url_host(MdApi::HOST.c_str());
            std::string header {"GET %s HTTP/1.0\r\n"
                                "Host: %.*s\r\n"
                                "Connection: kepp-alive\r\n"
                                "User-Agent: " + MdApi::USER_AGENT + "\r\n"
                                "Accept: */*\r\n"
                                "Referer: http://quotes.money.163.com\r\n"
                                "Accept-Encoding: gzip, deflate\r\n"
                                "Accept-Language: zh-CN,zh;q=0.9\r\n"
                                "\r\n"};
//            int ret = mg_printf(conn, "GET %s HTTP/1.1\r\n"
//                            "Host: %.*s\r\n"
//                            "\r\n", mg_url_uri(api->m_current_url.c_str()), int(host.len), host.ptr);
            std::cout << header.size() << std::endl;
            int ret = mg_printf(conn, header.c_str(), mg_url_uri(api->m_current_url.c_str()), int(host.len), host.ptr);
            std::cout << "ret : " << ret << std::endl;
        } else if (type == MG_EV_HTTP_MSG) {
            msg = reinterpret_cast<struct mg_http_message*>(ev_data);
            std::cout << msg->message.ptr << std::endl;

            struct mg_str host = mg_url_host(MdApi::HOST.c_str());
            std::string symbols;
            for(auto&& symbol: api->m_symbols){
                symbols += symbol;
                symbols += ",";
            }
            std::string url {MdApi::HOST + MdApi::md_uri + symbols + MdApi::subfix_uri + std::to_string(api->m_random(api->m_engine))};
            mg_printf(conn, "GET %s HTTP/1.1\r\n"
                            "Host: %.*s\r\n"
                            "\r\n", mg_url_uri(url.c_str()), int(host.len), host.ptr);
        } else if (type == MG_EV_ERROR) {
            std::cout << "error" << std::endl;
        } else if (type == MG_EV_CLOSE) {
            std::cout << "close" << std::endl;
        }
    }

private:
    std::atomic<bool> m_stop;
    std::string   m_current_url;
    struct mg_mgr m_mgr;
    std::default_random_engine m_engine;
    std::uniform_int_distribution<unsigned> m_random;
    std::vector<std::string> m_symbols;
};



#endif
