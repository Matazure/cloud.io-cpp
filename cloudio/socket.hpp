#pragma  once

#include <map>
#include <websocket/websocket.hpp>
#include <json11/json11.hpp>

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
        
        typedef boost::signals2::signal<void (const std::string &)>     message_signal;
        typedef typename message_signal::slot_type                      message_slot;
        
        typedef boost::signals2::signal<void (const std::string &)>     event_signal;
        typedef typename event_signal::slot_type                        event_slot;
        
        typedef event_slot                                              callback_slot;
        typedef boost::signals2::signal<void (const std::string &, callback_slot)>  event_callback_signal;
        typedef typename event_callback_signal::slot_type               event_callback_slot;
        
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
        
        
        void emit(const std::string &type, const std::string &data){
            std::stringstream ss;
            json11::Json json({{"type", type}, {"data", data}});
            _sp_websocket->send(json.dump());
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
        
        void on_c(const std::string &type, event_callback_slot f){
            if (!_event_callbacks[type]){
                _event_callbacks[type] = make_shared<event_callback_signal>();
            }
            
            _event_callbacks[type]->connect(f);
        }
        
    private:
        void add_listeners(){
            auto self = shared_from_this();
            _sp_websocket->on_open([self](){
                self->fire_connect();
            });
            
            _sp_websocket->on_close([self](){
                self->fire_disconnect();
            });
            
            _sp_websocket->on_message([self](const std::string &msg){
                std::string err = "";
                auto json = json11::Json::parse(msg, err);
                auto type = json["type"];
                auto data = json["data"];
                auto callback = json["callback"];
                
                if (!type.is_null() && !data.is_null()){
                    if (!callback.is_null()){
                        self->fire_c(type.string_value(), data.string_value(), [self, callback](const std::string &s){
                            self->emit(callback.string_value(), s);
                        });
                    }else{
                        self->fire(type.string_value(), data.string_value());
                    }
                }
            });
        }
        
        void fire_connect(){
            (*_sp_connect_signal)();
        }
        
        void fire_disconnect(){
            (*_sp_disconnect_signal)();
        }
        
        void fire_message(const std::string &msg){
            (*_sp_message_signal)(msg);
        }
        
        void fire(const std::string &type, const std::string &data){
            auto sp_signal = _events[type];
            if (sp_signal){
                (*sp_signal)(data);
            }else{
                ////Log
            }
        }
        
        void fire_c(const std::string &type, const std::string data, callback_slot fn){
            auto sp_signal = _event_callbacks[type];
            if (sp_signal){
                (*sp_signal)(data, fn);
            }
        }
        
        
    private:
        io_service                      &_iosev;
        
        shared_ptr<websocket::socket>   _sp_websocket;
        
        shared_ptr<connect_signal>      _sp_connect_signal;
        shared_ptr<disconnect_signal>   _sp_disconnect_signal;
        shared_ptr<message_signal>      _sp_message_signal;
        
        std::map<std::string, shared_ptr<event_signal>>             _events;
        std::map<std::string, shared_ptr<event_callback_signal>>    _event_callbacks;
    };
    
    
}
