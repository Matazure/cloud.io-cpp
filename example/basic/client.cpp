#include <cloudio/socket.hpp>

int main(){

    cloudio::io_service iosev;

    auto sp_socket = std::make_shared<cloudio::socket>(iosev);
    sp_socket->connect("ws://127.0.0.1:4012/");
    sp_socket->emit("test", "Hi, I'm cloudio client. ");
    sp_socket->on("test", [](const std::string s){
        std::cout << s << std::endl;
    });
    
    iosev.run();

    return 0;
}
