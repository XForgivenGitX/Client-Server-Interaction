#pragma once
#include <client_module.hpp>
#include <protocol.hpp>
//TODO
//1.Очищать буффер cin
using namespace common;
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
                //патом
            }
            else
            {
                anet::send_receive::receive(socketData_, {authorization_handler, this});
            }
        }
    
        void authorization_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            auto resp = common::split_frame(socketData->receive_buffer_);
            if(error || !resp)
            {
                socketData->shutdown();
                return;
            }
            auto& [cmd, args] = resp.value();
            std::string name, pass;
            switch(cmd)
            {
                case command::REPEAT_LOG_IN_REQ:
                    std::cout << "Invalid username or password. Try again:\n";
                case command::LOG_IN_REQ:
                    std::cout << "Enter name:"; 
                        std::cin >> name;
                    std::cout << "Enter password:\n";
                        std::getline(std::cin, pass);
                    socketData->send_buffer_ = common::assemble_frame(command::LOG_IN_RESP, {name, pass});
                    anet::send_receive::send(socketData, {auth_response_handler, this});
                    break;
                
                case command::REPEAT_REG_REQ:
                    std::cout << "This name is occupied by another user, enter again:\n";
                
                case command::REGISTER_REQ:
                    std::cout << "Enter name:"; 
                        std::cin >> name;
                    std::cout << "Enter password:\n";
                        std::getline(std::cin, pass);
                    socketData->send_buffer_ = common::assemble_frame(command::REGISTER_RESP, {name, pass});
                    anet::send_receive::send(socketData, {auth_response_handler, this});
                    break;
                default:socketData->shutdown();
            }
        }

        void auth_response_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            anet::send_receive::receive(socketData_, {authorization_handler, this});
        }
    };
}