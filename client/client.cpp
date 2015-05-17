// client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/format.hpp>

namespace asio_ip = boost::asio::ip;


int _tmain(int argc, _TCHAR* argv[])
{
	std::string server {"127.0.0.1"};
	std::string port {"8080"};
	boost::asio::io_service io_service;
    asio_ip::tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({ server, port });

	asio_ip::tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);
	std::vector<char> buff(10240);
	boost::system::error_code ec;
	size_t cnt {0};
	while(true)
	{
		std::string requers = (boost::format("test request %1%") % cnt++).str();
		socket.write_some(boost::asio::buffer(requers));
		socket.read_some(boost::asio::buffer(buff), ec);
		std::string str(&buff[0]);
		std::cout << str << std::endl;
//		Sleep(1000);
	}


    std::thread t([&io_service](){ io_service.run(); });


	t.join();

	return 0;
}

