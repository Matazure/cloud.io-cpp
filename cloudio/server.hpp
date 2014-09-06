#pragma once

#include <cloudio/socket.hpp>
#include <websocket/server.hpp>
#include <cloudio/channel.hpp>

namespace cloudio{


    class server: public std::enable_shared_from_this<server>{
    public:
        typedef boost::signals2::signal<void (shared_ptr<socket>)>      connection_signal;
        typedef typename connection_signal::slot_type                   connection_slot;
        
        typedef boost::signals2::signal<void (void)>                    listening_signal;
        typedef typename listening_signal::slot_type                    listening_slot;

        server(io_service &iosev)
        : _iosev(iosev)
        , _sp_websocket_server(new websocket::server(iosev))
        , _sp_listening_signal(new listening_signal){ }

        void listen(size_t port){
            _sp_websocket_server->listen(port);
            auto self = shared_from_this();
            _sp_websocket_server->on_listening([self](){
                self->emit_listening();
            });
        }

        void on_connection(connection_slot f){
            of("/")->on_connection(f);
        }
        
        void on_listening(listening_slot f){
            _sp_listening_signal->connect(f);
        }

        shared_ptr<channel> of(const std::string &path){
            auto self = shared_from_this();
            _sp_websocket_server->of(path)->on_connection([self, path](shared_ptr<websocket::socket> sp_websocket){
                auto sp_socket = make_shared<socket>(self->_iosev);
                sp_socket->attach(sp_websocket);
                self->_sp_channels[path]->emit_connection(sp_socket);
            });

            if (_sp_channels[path]){
                return _sp_channels[path];
            }else{
                _sp_channels[path] = make_shared<channel>();
                return _sp_channels[path];
            }
        }
        
    private:
        
        void emit_listening(){
            (*_sp_listening_signal)();
        }

    private:
        io_service          &_iosev;
        
        shared_ptr<listening_signal>                    _sp_listening_signal;

        shared_ptr<websocket::server>                   _sp_websocket_server;
        std::map<std::string, shared_ptr<channel>>      _sp_channels;
    };


}
