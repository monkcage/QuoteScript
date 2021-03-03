
#include "MdApi.hpp"


int main()
{
    MdApi api;
//    api.init();
    api.Subscribe("0000001,hk00857,0600256,1000968,0600759,0601857");
    api.Run();
    return 0;
}
