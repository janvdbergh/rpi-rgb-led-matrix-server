#include <boost/endian/arithmetic.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/crc.hpp>
#include "NetworkError.h"
#include "NetworkPacket.h"

uint32_t Packet::GetCRC() const {
    boost::crc_32_type result;
    result.process_bytes(_data.data(), _data.size());
    return static_cast<uint32_t>(result.checksum());
}

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

PacketReader &PacketReader::operator>>(const PacketReaderComplete &_) {
    if (_packet.GetSize() != _position) {
        boost::throw_exception(InvalidSizeError(_position, _packet.GetSize()));
    }

    return *this;
}

PacketReader &PacketReader::operator>>(std::string &string) {
    uint16_t length;
    (*this) >> length;

    char chars[length + 1];
    Read(chars, length);
    chars[length] = '\0';
    string = chars;

    return *this;
}

PacketReader &PacketReader::operator>>(boost::shared_ptr<const Image> &image) {
    uint16_t width, height;
    (*this) >> width >> height;
    ImageBuilder imageBuilder(width, height);

    for (uint16_t x = 0; x < width; x++) {
        for (uint16_t y = 0; y < height; y++) {
            uint8_t red, green, blue;
            (*this) >> red >> green >> blue;
            imageBuilder.SetPixel(x, y, red, green, blue);
        }
    }

    image = imageBuilder.Build();

    return *this;
}

template<typename T>
void PacketWriter::Write(T &value) {
    Write((void*)&value, sizeof(value));
}

void PacketWriter::Write(const void *data, size_t size) {
    size_t position = _data.size();
    _data.resize(position + size);
    memcpy(&_data.front() + position, data, size);
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

PacketWriter &PacketWriter::operator<<(uint16_t value) {
    Write(value);
    return *this;
}

PacketWriter &PacketWriter::operator<<(const std::string &value) {
    (*this) << (uint16_t) value.length();
    Write(value.c_str(), value.length());
    return *this;
}

PacketWriter &PacketWriter::operator<<(boost::shared_ptr<const Image> image) {
    (*this) << image->GetWidth() << image->GetHeight();

    for (uint16_t x = 0; x < image->GetWidth(); x++) {
        for (uint16_t y = 0; y < image->GetHeight(); y++) {
            (*this) << image->GetRed(x, y) << image->GetGreen(x, y) << image->GetBlue(x, y);
        }
    }

    return *this;
}

PacketWriter::operator Packet() const {
    return Packet(_data);
}
