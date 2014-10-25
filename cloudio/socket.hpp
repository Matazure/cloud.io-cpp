#pragma  once

#include <map>
#include <websocket/websocket.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace cloudio{

    using std::shared_ptr;
    using std::make_shared;
    using namespace boost::asio;


    class socket : public std::enable_shared_from_this<socket>{
    public:
        typedef boost::signals2::signal<void (shared_ptr<socket>)>      connection_signal;

        typedef boost::signals2::signal<void ()>                        connect_signal;
        typedef typename connect_signal::slot_type                      connect_slot;

        typedef boost::signals2::signal<void ()>                        disconnect_signal;
        typedef typename disconnect_signal::slot_type                   disconnect_slot;

//        typedef boost::signals2::signal<void (error_code)>              error_signal;
//        typedef typename error_signal::slot_type                        error_slot;

        typedef boost::signals2::signal<void (const std::string &)>     message_signal;
        typedef typename message_signal::slot_type                      message_slot;

        typedef boost::signals2::signal<void (const std::string &)>     event_signal;
        typedef typename event_signal::slot_type                        event_slot;

//        typedef std::function<void (error_code ec)>                     send_callback_type;

    public:
        socket(io_service &iosev): _iosev(iosev), _sp_websocket(nullptr), _sp_connect_signal(new connect_signal),
        _sp_disconnect_signal(new disconnect_signal){ }

        void connect(const std::string &url){
            _sp_websocket = websocket::connect(_iosev, url);
            add_listeners();
        }

        void attach(shared_ptr<websocket::socket> sp_websocket){
            _sp_websocket = sp_websocket;
            add_listeners();
        }

//        void send(const std::string &msg){
//            _sp_websocket->send(msg)
//        }
//
//        void send(std::string &&msg){
//
//        }

        void emit(const std::string &type, const std::string &data){
            std::stringstream ss;
            boost::property_tree::ptree pt;
            pt.put("type", type);
            pt.put("data", data);
            boost::property_tree::json_parser::write_json(ss, pt);
            _sp_websocket->send(std::move(ss.str()));
        }

        std::string path() const                    { return _sp_websocket->path(); }

        void disconnect(){
            _sp_websocket->disconnect();
        }

        bool live() const{
            assert(false);  /// todo
        }

    public:

        void on_connect(connect_slot f)         { _sp_connect_signal->connect(f); }
        void on_disconnect(disconnect_slot f)   { _sp_disconnect_signal->connect(f); }
        void on_message(message_slot f)         { _sp_message_signal->connect(f); }

        void on(const std::string &type, event_slot f){
            if (!_events[type]){
                _events[type] = make_shared<event_signal>();
            }

            _events[type]->connect(f);
        }

    private:
        void add_listeners(){
            auto self = shared_from_this();
            _sp_websocket->on_open([self](){
                self->emit_connect();
            });

            _sp_websocket->on_close([self](){
                self->emit_disconnect();
            });

            _sp_websocket->on_message([self](const std::string &msg){
                std::istringstream ss(msg);
                boost::property_tree::ptree pt;
                boost::property_tree::json_parser::read_json(ss, pt);
                auto op_type = pt.get_optional<std::string>("type");
                auto op_data = pt.get_optional<std::string>("data");

                if (op_type && op_data){
                    self->_emit(*op_type, *op_data);
                }
            });
        }

        void emit_connect(){
            (*_sp_connect_signal)();
        }

        void emit_disconnect(){
            (*_sp_disconnect_signal)();
        }

        void emit_message(const std::string &msg){
            (*_sp_message_signal)(msg);
        }

        void _emit(const std::string &type, const std::string &data){
            auto sp_signal = _events[type];
            if (sp_signal){
                (*sp_signal)(data);
            }else{
                ////Log
            }
        }


    private:
        io_service                      &_iosev;

        shared_ptr<websocket::socket>   _sp_websocket;

        shared_ptr<connect_signal>      _sp_connect_signal;
        shared_ptr<disconnect_signal>   _sp_disconnect_signal;
        shared_ptr<message_signal>      _sp_message_signal;

        std::map<std::string, shared_ptr<event_signal>>  _events;
    };


}
