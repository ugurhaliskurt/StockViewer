
#include <time.h>


#include <Server/RestServer.hpp>


using namespace std;

int main(int argc, char* argv[])
{
    string port = U("34568");
    
    utility::string_t address = U("http://127.0.0.1:");
    address.append(port);
    std::unique_ptr<handler> g_httpHandler = make_unique<handler>(address);
    g_httpHandler->on_initialize(address);

    std::cout << "Press ENTER to exit." << std::endl;
    std::string line;
    std::getline(std::cin, line);
    g_httpHandler->on_shutdown();
    return 0;
}
