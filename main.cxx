
#include "MdApi.hpp"
#include <random>


int main()
{
    MdApi api;
//    api.Subscribe("0000001,hk00857,0600256,1000968,0600759,0601857");
//    api.Run();
    std::default_random_engine e;
    std::uniform_int_distribution<unsigned> u(10000000, 99999999);
    for(int i=0; i<10; ++i){
        std::cout << u(e) << std::endl;
    }
    return 0;
}
