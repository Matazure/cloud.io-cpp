#include <cloudio/server.hpp>

int main(){

    cloudio::io_service iosev;
    auto sp_server =  std::make_shared<cloudio::server>(iosev);
    sp_server->listen(4012);
    sp_server->of("/")->on_connection([](std::shared_ptr<cloudio::socket> sp_socket){
        sp_socket->on("test", [](const std::string &msg){
            std::cout << msg << std::endl;
        });

        sp_socket->emit("test", "I'm from cloudio server test of /, 我叫张志民.");

        sp_socket->on_disconnect([](){
            std::cout << "socket disconnect . " << std::endl;
        });
    });

    sp_server->of("/chat")->on_connection([](std::shared_ptr<cloudio::socket> sp_socket){
        sp_socket->on("echo", [](const std::string &msg){
            std::cout << msg << std::endl;
        });

        sp_socket->emit("echo", "I'm from cloudio cpp server echo of chat.");
    });

    iosev.run();

    return 0;
}
