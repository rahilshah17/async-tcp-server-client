// ------------------------
// client code
// author: Rahil Shah
// ------------------------
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void save_video_file(const std::vector<char>& data, const std::string& file_name)
{
  std::ofstream file(file_name, std::ios::out | std::ios::binary);
  if (!file)
  {
    std::cerr << "Failed to open file for writing: " << file_name << std::endl;
    return;
  }

  file.write(data.data(), data.size());
}

void receive_file(tcp::socket& socket, const std::string& file_name)
{
  std::vector<char> buffer(1024);
  std::vector<char> file_data;
  boost::system::error_code error;

  while (true)
  {
    size_t len = socket.read_some(boost::asio::buffer(buffer), error);
    if (error == boost::asio::error::eof)
      break; // Connection closed cleanly by peer.
    else if (error)
      throw boost::system::system_error(error); // Some other error.

    file_data.insert(file_data.end(), buffer.begin(), buffer.begin() + len);
  }

  save_video_file(file_data, file_name);
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 4)
    {
      std::cerr << "Usage: video_client <host> <port> <output_file>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(argv[1], argv[2]);

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    receive_file(socket, argv[3]);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}

