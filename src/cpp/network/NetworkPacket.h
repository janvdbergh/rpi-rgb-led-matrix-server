#ifndef DISPLAYSERVER_PACKET_H
#define DISPLAYSERVER_PACKET_H

#include <vector>
#include <string>
#include "../display/Image.h"
#include "../display/Command.h"

const uint32_t MAX_PACKET_SIZE = 8192;

class Packet {
public:
    explicit Packet(const boost::shared_ptr<const Command>& command);
    explicit Packet(std::vector<char> data): _data(std::move(data)) {}

    uint32_t GetSize() const { return static_cast<uint32_t>(_data.size()); }

    const std::vector<char> &GetData() const { return _data; }

    uint32_t GetCRC() const;

    boost::shared_ptr<const Command> GetCommand() const;
private:
    std::vector<char> _data;
};

#endif //DISPLAYSERVER_PACKET_H
