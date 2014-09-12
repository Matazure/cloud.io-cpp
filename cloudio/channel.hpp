#pragma  once

#include <cloudio/socket.hpp>

namespace cloudio{

    class channel{
    public:
        typedef boost::signals2::signal<void (shared_ptr<socket>)>      connection_signal;
        typedef typename connection_signal::slot_type                   connection_slot;

        channel(): _sp_connection_signal(new connection_signal){}

        void on_connection(connection_slot f){
            _sp_connection_signal->connect(f);
        }

        void emit_connection(shared_ptr<socket> sp_socket){
            (*_sp_connection_signal)(sp_socket);
        }

    private:
        shared_ptr<connection_signal>                                   _sp_connection_signal;
    };

}
