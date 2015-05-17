// server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <thread>

//class session1
//  : public std::enable_shared_from_this<session1>
//{
//public:
//  session1(tcp::socket socket)
//    : socket_(std::move(socket))
//  {
//  }
//
//  void start()
//  {
//    do_read();
//  }
//
//private:
//  void do_read()
//  {
//    auto self(shared_from_this());
//    socket_.async_read_some(boost::asio::buffer(data_, max_length),
//        [this, self](boost::system::error_code ec, std::size_t length)
//        {
//          if (!ec)
//          {
//            do_write(length);
//          }
//        });
//  }
//
//  void do_write(std::size_t length)
//  {
//    auto self(shared_from_this());
//    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
//        [this, self](boost::system::error_code ec, std::size_t /*length*/)
//        {
//          if (!ec)
//          {
//            do_read();
//          }
//        });
//  }
//
//  tcp::socket socket_;
//  enum { max_length = 1024 };
//  char data_[max_length];
//};

class session: public std::enable_shared_from_this<session>
{
public:
	session(boost::asio::ip::tcp::socket sock)
		: socket(std::move(sock))
		, request_count(0)
	{

	}
	void start()
	{
		read();
	}

private:
	void read()
	{
		auto self (shared_from_this());
		buff.resize(1024);
		socket.async_read_some( boost::asio::buffer(buff),
								[this, self](boost::system::error_code ec, size_t length)
								{
									if (!ec)
									{
										//std::string msg(&buff[0], length);
										//std::cout << msg << std::endl;
										write();
									}
								});
	}

	void write()
	{
		auto self(shared_from_this());
		str = (boost::format("request count is %1%") % request_count++).str();
		boost::asio::async_write(socket, boost::asio::buffer(str, str.size()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
			{ if (!ec) read(); });
	}
	std::string str;
	int request_count;
	std::vector<char> buff;
	boost::asio::ip::tcp::socket socket;
};

class aac_server
{
public:
	aac_server(boost::asio::io_service& io_srv, uint16_t port)
		: acceptor(io_srv, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
		, socket(io_srv)
	{
		accept_connect();
	}
	~aac_server() = default;
private:
	void accept_connect()
	{
		acceptor.async_accept( socket, 
								[this](boost::system::error_code ec)
								{
									if (!ec)
									{
										std::make_shared<session>(std::move(socket))->start();
									}
									accept_connect();
								});
	}
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ip::tcp::socket socket;
};



int _tmain(int argc, _TCHAR* argv[])
{
 try
  {
    boost::asio::io_service io_service;

    aac_server s(io_service, 8080);
	std::thread th1([&](){ io_service.run();});
	std::thread th2([&](){ io_service.run();});
	std::thread th3([&](){ io_service.run();});
	std::thread th4([&](){ io_service.run();});
	std::thread th5([&](){ io_service.run();});
	std::thread th6([&](){ io_service.run();});
	std::thread th7([&](){ io_service.run();});
	std::thread th8([&](){ io_service.run();});
    th1.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

