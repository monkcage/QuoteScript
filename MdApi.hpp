#ifndef MDAPI_INC
#define MDAPI_INC

#include <iostream>
#include <sstream>
#include <atomic>

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
    {
        mg_log_set("0");
        mg_mgr_init(&m_mgr);
    }

    ~MdApi()
    {
        mg_mgr_free(&m_mgr);
    }

    void init()
    {
        std::string base {"https://www.eastmoney.com"};
        m_current_url = base;
        mg_http_connect(&m_mgr, base.c_str(), MdApi::fake_router_handler, this);
    }

    void Subscribe(std::string symbol)
    {
        std::stringstream ss;
        std::string url { MdApi::HOST + MdApi::md_uri + symbol + MdApi::subfix_uri};
        m_current_url = url;
        std::cout << m_current_url << std::endl;
        mg_http_connect(&m_mgr, url.c_str(), MdApi::subscribe_handler, this);
    }

    void Run()
    {
        while(!m_stop) {
            std::cout << "pool ..." << std::endl;
            mg_mgr_poll(&m_mgr, 1000);
        }
    }

    static void fake_router_handler(struct mg_connection* conn, int type, void* ev_data, void* fn_data)
    {
        MdApi* api = reinterpret_cast<MdApi*>(fn_data);
        mg_http_message *msg = nullptr;
        if(type == MG_EV_CONNECT) {
            std::cout << "connect ..." << std::endl;
            struct mg_str host = mg_url_host(MdApi::HOST.c_str());
//            std::string header {"GET %s HTTP/1.0\r\n"
//                                "Host: %.*s\r\n"
//                                "Connection: kepp-alive\r\n"
//                                "User-Agent: " + MdApi::USER_AGENT + "\r\n"
//                                "Accept: */*\r\n"
//                                "Accept-Encoding: gzip, deflate\r\n"
//                                "Accept-Language: zh-CN,zh;q=0.9\r\n"};
//            std::cout << header.c_str() << std::endl;
            mg_printf(conn, "GET %s HTTP/1.0\r\n"
                            "Host: %.*s\r\n"
                            "\r\n", mg_url_uri(api->m_current_url.c_str()), int(host.len), host.ptr);
//            mg_printf(conn, header.c_str(), mg_url_uri(api->m_current_url.c_str()), int(host.len), host.ptr);
        } else if (type == MG_EV_HTTP_MSG) {
            std::cout << "message " << std::endl;
            msg = reinterpret_cast<struct mg_http_message*>(ev_data);
//            printf("%.*s", (int) msg->message.len, msg->message.ptr);
            std::cout << msg->message.ptr << std::endl;
//            std::cout << msg->body.ptr << std::endl;
//            std::cout << msg->uri.ptr << std::endl;
//            for(int i=0; i<MG_MAX_HTTP_HEADERS; ++i)
//            {
//                std::cout << msg->headers[i].name.ptr << " : " << msg->headers[i].value.ptr << std::endl;
//            }
            std::cout << "recved ..." << std::endl;
        } else if (type == MG_EV_ERROR) {
            std::cout << "error ..." << std::endl;
        } else if (type == MG_EV_CLOSE) {
            std::cout << "close ..." << std::endl;
        } else {
            std::cout << "unknwon error : " << type << std::endl;
        }
    }

    static void subscribe_handler(struct mg_connection* conn, int type, void* ev_data, void* fn_data)
    {
        MdApi* api = reinterpret_cast<MdApi*>(fn_data);
        mg_http_message *msg = nullptr;
        if(type == MG_EV_CONNECT) {
            std::cout << "connect..." << std::endl;
            struct mg_str host = mg_url_host(MdApi::HOST.c_str());
//            std::string header {"GET %s HTTP/1.0\r\n"
//                                "Host: %.*s\r\n"
//                                "Connection: kepp-alive\r\n"
//                                "User-Agent: " + MdApi::USER_AGENT + "\r\n"
//                                "Accept: */*\r\n"
//                                "Referer: http://quotes.money.163.com\r\n"
//                                "Accept-Encoding: gzip, deflate\r\n"
//                                "Accept-Language: zh-CN,zh;q=0.9\r\n"};
            mg_printf(conn, "GET %s HTTP/1.0\r\n"
                            "Host: %.*s\r\n"
                            "\r\n", mg_url_uri(api->m_current_url.c_str()), int(host.len), host.ptr);
//            mg_printf(conn, header.c_str(), mg_url_uri(api->m_current_url.c_str()), int(host.len), host.ptr);
        } else if (type == MG_EV_HTTP_MSG) {
            msg = reinterpret_cast<struct mg_http_message*>(ev_data);
            std::cout << msg->message.ptr << std::endl;
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
};



#endif
