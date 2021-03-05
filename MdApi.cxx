
#include "MdApi.hpp"


const std::string MdApi::HOST {"http://api.money.126.net"};
//const std::string MdApi::HOST {"http://localhost:8888"};
const std::string MdApi::md_uri {"/data/feed/"};
const std::string MdApi::subfix_uri {"money.api?callback=_ntes_quote_callback"};




// http header
const std::string MdApi::USER_AGENT {"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.190 Safari/537.36"};
