#include <boost/crc.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/endian/arithmetic.hpp>

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
    std::cout << sizeof(unsigned long) << std::endl;

    boost::endian::little_uint16_at data = 1;
    uint32_t length = sizeof(data);
    
    boost::crc_32_type crc32;
    crc32.process_bytes(data.data(), length);
    unsigned long crc = crc32.checksum();

    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("localhost", "1236");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    std::vector<boost::asio::mutable_buffer> bufs;
    boost::system::error_code error;
    bufs.push_back(boost::asio::buffer(&length, sizeof(length)));
    bufs.push_back(boost::asio::buffer(&data, length));
    bufs.push_back(boost::asio::buffer(&crc, sizeof(crc)));

    boost::asio::write(socket, bufs, error);
    if (error) {
        std::cerr << error.message() << std::endl;
    }

    socket.close();

    return 0;
}