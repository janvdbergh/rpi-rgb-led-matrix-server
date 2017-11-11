#ifndef DISPLAYSERVER_PACKET_H
#define DISPLAYSERVER_PACKET_H

#include <vector>
#include <string>

const uint32_t MAX_PACKET_SIZE = 8192;

enum Command {
    CLEAR,
    SHOW,
    COLOR,
    PIXEL,
    RECTANGLE,
    DIGIT,
    SMALL_TEXT,
    LARGE_TEXT
};

class Packet {
public:
    explicit Packet(std::vector<char> data) { _data = std::move(data); }

    uint32_t GetSize() const { return static_cast<uint32_t>(_data.size()); }

    const std::vector<char>& GetData() const { return _data; }

private:
    std::vector<char> _data;

    friend class PacketReader;
};

class PacketReader {
public:
    explicit PacketReader(const Packet &packet) : _packet(packet), _position(0) {}

    Command NextCommand();

    uint8_t NextByte();

    int16_t NextShort();

    std::string NextString();

    void ValidateCompletelyRead();

private:
    const Packet &_packet;
    uint32_t _position;

    template<typename T>
    void Read(T &resultHolder);

    void Read(void *result_holder, size_t size);

    uint16_t NextUnsignedShort();
};

class PacketWriter {
public:
    PacketWriter() = default;

    PacketWriter &AddCommand(Command command);

    PacketWriter &AddByte(uint8_t value);

    PacketWriter &AddShort(int16_t value);

    PacketWriter &AddString(std::string value);

    Packet CreatePacket();

private:
    std::vector<char> _data;

    template<typename T>
    void Write(T &value);
};

#endif //DISPLAYSERVER_PACKET_H
