#pragma once
#include <client_module.hpp>

//TODO
//1.Очищать буффер cin

namespace client
{
    class client_session
    {
        anet::socket_data_ptr socketData_;
    
    public:
        client_session(io__::io_context &ios, anet::end_point_wrapper endPoint, std::string name)
            : socketData_(anet::make_socket_data(ios.get_executor()))
        {
            anet::connection::connection_request(anet::make_socket_data(socketData_, endPoint), {&connection_handler, this});
        }

        void connection_handler(anet::socket_data_endpoint_ptr &&socketDataEndPoint, const boost::system::error_code &error)
        {
            if(error)
            {
                throw "Technical work is being carried out on the server.\
                        Access is temporarily unavailable. Exit...";
            }
            else
            {
                std::cout << "Create a new account / log in to an existing [0/1]?\n";
                std::cin.get() ? log_in_account() : create_account();
            }
        }

        void create_account()
        {
            create_name();
        }
        
        void create_name()
        {
            std::cout << "Enter name(5-20ch):";
            std::cin >> socketData_->send_buffer_;
            //anet::send_receive::send(socketData_, {authirization_handler, this});
        }
        
        void create_name_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            if(error)
            {
                throw "Error send data. Exit...";
            }
            else
            {
                
            }
        }
        void log_in_account()
        {

        }
    };
    
    
    
    
#if 0
    class client_session
    {
    public:
        io__::io_context &ios_;
        anet::end_point_wrapper endPoint_;
        std::string name_;
        anet::socket_data_ptr socketData_;
    
    public:
        client_session(io__::io_context &ios, anet::end_point_wrapper endPoint, std::string name)
            : ios_(ios), endPoint_(endPoint), name_(name), socketData_(nullptr){}
    
    public:
        void start()
        {
            anet::connection::connection_request(anet::make_socket_data(ios_, endPoint_), {connection_handler, this});
        }
        
        void connection_handler(anet::socket_data_endpoint_ptr &&socketDataEndPoint, const boost::system::error_code &error)
        {
            auto& socketData = socketDataEndPoint->socketData_;
            if(error)
            {
                std::cerr << "Connection error, system: " << error.message() << ".\n";
                exit(0);
            }
            else
            {
                std::cout << "Successfully joined, have a nice chat!\n";
                socketData->send_buffer_ = name_;
                anet::send_receive::send(socketData, {authirization_handler, this});
            }
        }

        void authirization_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            if(error) socketData->shutdown(); 
            else 
            {
                socketData_ = socketData;
                anet::send_receive::receive(socketData, {receive_message_handler, this});
            }
        }

        void receive_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            if(error) socketData->shutdown();
            else 
            {
                std::cout << socketData->receive_buffer_ << '\n';
                anet::send_receive::receive(socketData, {receive_message_handler, this});
            }
        }

        void send_message(std::string message)
        {
            socketData_->send_buffer_ = message;
            anet::send_receive::send(socketData_, {send_message_handler, this});
        }
    
        void send_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {

        }
    };
#endif
}