#pragma once

#include <cloudio/server.hpp>

namespace cloudio{

    shared_ptr<server> listen(io_service &iosev, size_t port){
        auto sp_server = make_shared<server>(iosev);
        sp_server->listen(port);

        return sp_server;
    }

    shared_ptr<socket> connect(io_service &iosev, const std::string &url){
        auto sp_socket = make_shared<socket>(iosev);
        sp_socket->connect(url);

        return sp_socket;
    }

}
