
#include "MdApi.hpp"
#include <random>


int main()
{
    MdApi api;
    api.Subscribe("hk00857,0600256,1000968,0600759,0601857");
    api.Run();
    return 0;
}
