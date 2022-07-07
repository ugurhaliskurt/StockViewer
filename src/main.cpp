
#include <time.h>

#include "yfapi.hpp"
#include <Server/RestServer.hpp>


using namespace std;

int main(int argc, char* argv[])
{
    string port = U("34568");
    
    utility::string_t address = U("http://127.0.0.1:");
    address.append(port);
    std::unique_ptr<handler> g_httpHandler = make_unique<handler>(address);
    g_httpHandler->on_initialize(address);


    yfapi::YahooFinanceAPI api; 
    api.set_interval(DAILY);

    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);

    int day = aTime->tm_mday;
    int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
    int year = aTime->tm_year + 1900; // Year is # years since 1900
    std::string date;
    if( day > 9 && month > 9 )
        date = std::to_string(year)+"-" + std::to_string(month)+"-"+ std::to_string(day);
    else if( day < 10 && month > 9 )
        date = std::to_string(year)+"-" + std::to_string(month)+"-0"+ std::to_string(day);
    else if( day > 10 && month < 9 )
        date = std::to_string(year)+"-0" + std::to_string(month)+"-"+ std::to_string(day);
    else
        date = std::to_string(year)+"-0" + std::to_string(month)+"-0"+ std::to_string(day);

    std::cout << std::endl << api.getProfitInUSD("SASA.IS", "2016-06-20","2022-07-01" , date , 10000)<< std::endl;

    std::cout << "Press ENTER to exit." << std::endl;
    std::string line;
    std::getline(std::cin, line);
    g_httpHandler->on_shutdown();
    return 0;
}
