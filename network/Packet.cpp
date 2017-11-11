#include <boost/endian/arithmetic.hpp>
#include <boost/asio.hpp>
#include "Errors.h"
#include "Packet.h"

template<typename T>
void PacketReader::Read(T &result_holder) {
    Read(&result_holder, sizeof(T));
}

void PacketReader::Read(void *result_holder, size_t size) {
    size_t lastIndex = _position + size;
    if (lastIndex > _packet.GetSize()) {
        boost::throw_exception(IndexOutOfRangeError());
    }

    memcpy(result_holder, &_packet._data.front() + _position, size);
    _position += size;
}

void PacketReader::ValidateCompletelyRead() {
    if (_packet.GetSize() != _position) {
        boost::throw_exception(InvalidSizeError(_position, _packet.GetSize()));
    }
}

Command PacketReader::NextCommand() {
    return (Command) NextUnsignedShort();
}

uint8_t PacketReader::NextByte() {
    boost::endian::little_uint8_at result;
    Read(result);
    return result;
}

int16_t PacketReader::NextShort() {
    boost::endian::little_int16_at result;
    Read(result);
    return result;
}

uint16_t PacketReader::NextUnsignedShort() {
    boost::endian::little_uint16_at result;
    Read(result);
    return result;
}

std::string PacketReader::NextString() {
    uint16_t length = NextUnsignedShort();
    char chars[length + 1];
    Read(chars, length);
    chars[length] = '\0';

    return std::string(chars);
}

template<typename T>
void PacketWriter::Write(T &value) {
    size_t position = _data.size();
    _data.resize(position + sizeof(value));
    memcpy(&_data.front() + position, &value, sizeof(value));
}

PacketWriter &PacketWriter::AddCommand(Command command) {
    auto value = (uint16_t) command;
    Write(value);
    return *this;
}

PacketWriter &PacketWriter::AddByte(uint8_t value) {
    Write(value);
    return *this;
}

PacketWriter &PacketWriter::AddShort(int16_t value) {
    Write(value);
    return *this;
}

PacketWriter &PacketWriter::AddString(std::string value) {
    Write(value);
    return *this;
}

Packet PacketWriter::CreatePacket() {
    return Packet(_data);
}
