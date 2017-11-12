#include <boost/endian/arithmetic.hpp>
#include <boost/asio.hpp>
#include <iostream>
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

PacketReader &PacketReader::operator>>(Command &command) {
    boost::endian::little_uint16_at result;
    Read(result);
    command = (Command) (int) result;

    return *this;
}

PacketReader &PacketReader::operator>>(uint8_t &uint8) {
    Read(uint8);
    return *this;
}

PacketReader &PacketReader::operator>>(int16_t &int16) {
    boost::endian::little_int16_at result;
    Read(result);
    int16 = result;
    return *this;
}

PacketReader &PacketReader::operator>>(uint16_t &uint16) {
    boost::endian::little_uint16_at result;
    Read(result);
    uint16 = result;
    return *this;
}

PacketReader & PacketReader::operator>>(const PacketReaderComplete &_) {
    if (_packet.GetSize() != _position) {
        boost::throw_exception(InvalidSizeError(_position, _packet.GetSize()));
    }

    return *this;
}

PacketReader &PacketReader::operator>>(std::string &string) {
    uint16_t length;
    this->operator>>(length);

    char chars[length + 1];
    Read(chars, length);
    chars[length] = '\0';
    string = chars;

    std::cout << std::endl;

    return *this;
}

template<typename T>
void PacketWriter::Write(T &value) {
    size_t position = _data.size();
    _data.resize(position + sizeof(value));
    memcpy(&_data.front() + position, &value, sizeof(value));
}

PacketWriter &PacketWriter::operator<<(Command command) {
    auto value = (uint16_t) command;
    Write(value);
    return *this;
}

PacketWriter &PacketWriter::operator<<(uint8_t value) {
    Write(value);
    return *this;
}

PacketWriter &PacketWriter::operator<<(int16_t value) {
    Write(value);
    return *this;
}

PacketWriter &PacketWriter::operator<<(std::string value) {
    Write(value);
    return *this;
}

PacketWriter::operator Packet() const {
    return Packet(_data);
}
