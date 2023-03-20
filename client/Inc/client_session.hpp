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
            auto resp = common::disassemble_frame(socketData->receive_buffer_);
            if(error || !resp)
            {
                socketData->shutdown();
                return;
            }
            auto& [cmd, args] = resp.value();
            std::string buff1, buff2;
            switch(cmd)
            {
                case command::ECHO_LOG_IN_REQ:
                    std::cout << "Invalid username or password. Try again:\n";
                case command::LOG_IN_REQ:
                    std::cout << "Name:"; 
                        std::getline(std::cin, buff1);
                    std::cout << "Pass:";
                        std::getline(std::cin, buff2);
                    socketData->send_buffer_ = common::assemble_frame(command::LOG_IN_RESP, {buff1, buff2});
                    anet::send_receive::send(socketData, {auth_response_handler, this});
                    break;
                
                
                case command::ECHO_REGISTER_REQ:
                    std::cout << "This name is occupied by another user. Use other:\n";        
                case command::REGISTER_REQ:
                    std::cout << "Name:"; 
                        std::getline(std::cin, buff1);
                    std::cout << "Pass:";
                        std::getline(std::cin, buff2);
                    socketData->send_buffer_ = common::assemble_frame(command::REGISTER_RESP, {buff1, buff2});
                    anet::send_receive::send(socketData, {auth_response_handler, this});
                    break;
                
                case command::SUCCESS_REG_RESP:
                case command::SUCCESS_LOG_IN_RESP:
                    std::cout << "Welcome to the lobby! Enter command:";
                    std::getline(std::cin, buff1);
                    if(buff1 == "join")
                    {
                        std::cout << "Room name:";
                        std::getline(std::cin, buff1);
                        socketData->send_buffer_ = common::assemble_frame(command::JOIN_ROOM_REQ, {buff1});
                        anet::send_receive::send(socketData, {auth_response_handler, this});
                    }
                    else if(buff1 == "create")
                    {
                        std::cout << "Room name:";
                        std::getline(std::cin, buff1);
                        socketData->send_buffer_ = common::assemble_frame(command::CREATE_ROOM_REQ, {buff1});
                        anet::send_receive::send(socketData, {auth_response_handler, this});
                    }
                    else
                    {
                        std::cout << "encorrect command.\n";
                    }
                default:socketData->shutdown();
            }
        }

        void auth_response_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            anet::send_receive::receive(socketData_, {authorization_handler, this});
        }
    };
}