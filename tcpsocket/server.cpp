// -------------------
// server program
// Author: Rahil Shah
// -------------------

#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <chrono>
using boost::asio::ip::tcp;


class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
public:
    typedef std::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context) {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket& socket() {
        return socket_;
    }

    void start(const std::string& file_path) {
        std::ifstream file(file_path);
        if (!file) {
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return;
        }

        std::ostringstream ss;
        ss << file.rdbuf();
        message_ = ss.str();

        // Record the start time
        start_time_ = std::chrono::high_resolution_clock::now();

        boost::asio::async_write(socket_, boost::asio::buffer(message_),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

private:
    tcp_connection(boost::asio::io_context& io_context)
        : socket_(io_context) {
    }

    void handle_write(const boost::system::error_code& error, size_t /*bytes_transferred*/) {
        if (!error) {
            // Record the end time
            auto end_time = std::chrono::high_resolution_clock::now();
            // Calculate the elapsed time in milliseconds
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_).count();
            std::cout << "File transfer completed in " << duration << " milliseconds." << std::endl;
        }
    }

    tcp::socket socket_;
    std::string message_;
    std::chrono::high_resolution_clock::time_point start_time_;
};

class tcp_server {
public:
    tcp_server(boost::asio::io_context& io_context, const std::string& file_path)
        : io_context_(io_context),
          acceptor_(io_context, tcp::endpoint(tcp::v4(), 1234)),
          file_path_(file_path) {
        start_accept();
    }

private:
    void start_accept() {
        tcp_connection::pointer new_connection =
            tcp_connection::create(io_context_);

        acceptor_.async_accept(new_connection->socket(),
            boost::bind(&tcp_server::handle_accept, this, new_connection,
                boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code& error) {
        if (!error) {
            new_connection->start(file_path_);
        }
        start_accept();
    }

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    std::string file_path_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: server <file_path>\n";
            return 1;
        }

        boost::asio::io_context io_context;
        tcp_server server(io_context, argv[1]);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

