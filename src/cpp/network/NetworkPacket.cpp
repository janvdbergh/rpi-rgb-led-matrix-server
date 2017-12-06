#include <boost/endian/arithmetic.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/crc.hpp>
#include "NetworkError.h"
#include "NetworkPacket.h"

class PacketReader {
public:
    explicit PacketReader(std::vector<char> data) : _data(std::move(data)), _position(0) {}

    CommandCode ReadCommandCode();

    uint8_t ReadUint8();

    uint16_t ReadUint16();

    int16_t ReadInt16();

    std::string ReadString();

    boost::shared_ptr<const Image> ReadImage();

    void complete();

private:
    std::vector<char> _data;
    uint32_t _position;

    template<typename T>
    void Read(T &resultHolder);

    void Read(void *dataHolder, size_t size);
};

class PacketWriter {
public:
    PacketWriter() = default;

    const std::vector<char> &GetData() { return _data; }

    void Write(const CommandCode &commandCode);

    void Write(uint8_t uint8);

    void Write(uint16_t uint16);

    void Write(int16_t int16);

    void Write(const std::string &string);

    void Write(const boost::shared_ptr<const Image> &image);

private:
    std::vector<char> _data;

    template<typename T>
    void Write(const T &value);

    void Write(const void *data, size_t size);
};

uint32_t Packet::GetCRC() const {
    boost::crc_32_type result;
    result.process_bytes(_data.data(), _data.size());
    return static_cast<uint32_t>(result.checksum());
}

Packet::Packet(const boost::shared_ptr<const Command>& command) {
    PacketWriter writer;
    writer.Write(command->GetCode());

    switch (command->GetCode()) {
        case CommandCode::CLEAR:
        case CommandCode::SHOW: {
            // no extra data
            break;
        }

        case CommandCode::COLOR: {
            auto &colorCommand = (const ColorCommand &) *command;
            writer.Write(colorCommand.GetRed());
            writer.Write(colorCommand.GetGreen());
            writer.Write(colorCommand.GetBlue());
            break;
        }

        case CommandCode::PIXEL: {
            auto &pixelCommand = (const PixelCommand &) *command;
            writer.Write(pixelCommand.GetX());
            writer.Write(pixelCommand.GetY());
            break;
        }

        case CommandCode::RECTANGLE: {
            auto &rectangleCommand = (const RectangleCommand &) *command;
            writer.Write(rectangleCommand.GetX());
            writer.Write(rectangleCommand.GetY());
            writer.Write(rectangleCommand.GetWidth());
            writer.Write(rectangleCommand.GetHeight());
            break;
        }

        case CommandCode::DIGIT: {
            auto &digitCommand = (const DigitCommand &) *command;
            writer.Write(digitCommand.GetPosition());
            writer.Write(digitCommand.GetDigit());
            break;
        }

        case CommandCode::SMALL_TEXT: {
            auto &smallTextCommand = (const SmallTextCommand &) *command;
            writer.Write(smallTextCommand.GetX());
            writer.Write(smallTextCommand.GetY());
            writer.Write(smallTextCommand.GetText());
            break;
        }

        case CommandCode::LARGE_TEXT: {
            auto &largeTextCommand = (const LargeTextCommand &) *command;
            writer.Write(largeTextCommand.GetX());
            writer.Write(largeTextCommand.GetY());
            writer.Write(largeTextCommand.GetText());
            break;
        }

        case CommandCode::DEFINE_IMAGE: {
            auto &defineImageCommand = (const DefineImageCommand &) *command;
            writer.Write(defineImageCommand.GetName());
            writer.Write(defineImageCommand.GetImage());
            break;
        }

        case CommandCode::DRAW_IMAGE: {
            auto &drawImageCommand = (const ImageCommand &) *command;
            writer.Write(drawImageCommand.GetX());
            writer.Write(drawImageCommand.GetY());
            writer.Write(drawImageCommand.GetName());
            break;
        }

        default:
            boost::throw_exception(UnknownCommandError(command->GetCode()));
    }

    _data = writer.GetData();
}

boost::shared_ptr<const Command> Packet::GetCommand() const {
    PacketReader reader(_data);

    boost::shared_ptr<const Command> result;

    CommandCode commandCode = reader.ReadCommandCode();
    switch (commandCode) {
        case CommandCode::CLEAR:
            result = boost::shared_ptr<Command>(new ClearCommand());
            break;

        case CommandCode::SHOW:
            result = boost::shared_ptr<Command>(new ShowCommand());
            break;

        case CommandCode::COLOR:
            result = boost::shared_ptr<Command>(new ColorCommand(reader.ReadUint8(), reader.ReadUint8(), reader.ReadUint8()));
            break;

        case CommandCode::PIXEL:
            result = boost::shared_ptr<Command>(new PixelCommand(reader.ReadInt16(), reader.ReadInt16()));
            break;

        case CommandCode::RECTANGLE:
            result = boost::shared_ptr<Command>(new RectangleCommand(reader.ReadInt16(), reader.ReadInt16(), reader.ReadUint16(), reader.ReadUint16()));
            break;

        case CommandCode::DIGIT:
            result = boost::shared_ptr<Command>(new DigitCommand(reader.ReadUint8(), reader.ReadUint8()));
            break;

        case CommandCode::SMALL_TEXT:
            result = boost::shared_ptr<Command>(new SmallTextCommand(reader.ReadInt16(), reader.ReadInt16(), reader.ReadString()));
            break;

        case CommandCode::LARGE_TEXT:
            result = boost::shared_ptr<Command>(new LargeTextCommand(reader.ReadInt16(), reader.ReadInt16(), reader.ReadString()));
            break;

        case CommandCode::DEFINE_IMAGE:
            result = boost::shared_ptr<Command>(new DefineImageCommand(reader.ReadString(), reader.ReadImage()));
            break;

        case CommandCode::DRAW_IMAGE:
            result = boost::shared_ptr<Command>(new ImageCommand(reader.ReadInt16(), reader.ReadInt16(), reader.ReadString()));
            break;

        default:
            boost::throw_exception(UnknownCommandError(commandCode));
    }

    reader.complete();
    return result;
}

template<typename T>
void PacketReader::Read(T &resultHolder) {
    Read(&resultHolder, sizeof(T));
}

void PacketReader::Read(void *dataHolder, size_t size) {
    size_t lastIndex = _position + size;
    if (lastIndex > _data.size()) {
        boost::throw_exception(IndexOutOfRangeError());
    }

    memcpy(dataHolder, &_data.front() + _position, size);
    _position += size;
}

CommandCode PacketReader::ReadCommandCode() {
    boost::endian::little_uint16_at result;
    Read(result);
    return (CommandCode) (int) result;
}

uint8_t PacketReader::ReadUint8() {
    uint8_t result;
    Read(result);
    return result;
}


uint16_t PacketReader::ReadUint16() {
    boost::endian::little_uint16_at result;
    Read(result);
    return result;
}


int16_t PacketReader::ReadInt16() {
    boost::endian::little_int16_at result;
    Read(result);
    return result;
}

std::string PacketReader::ReadString() {
    uint16_t length = ReadUint16();

    char chars[length + 1];
    Read(chars, length);
    chars[length] = '\0';

    std::string result = chars;
    return result;
}

boost::shared_ptr<const Image> PacketReader::ReadImage() {
    uint16_t width = ReadUint16();
    uint16_t height = ReadUint16();

    ImageBuilder imageBuilder(width, height);
    for (uint16_t x = 0; x < width; x++) {
        for (uint16_t y = 0; y < height; y++) {
            uint8_t red = ReadUint8();
            uint8_t green = ReadUint8();
            uint8_t blue = ReadUint8();
            imageBuilder.SetPixel(x, y, red, green, blue);
        }
    }

    return imageBuilder.Build();
}

void PacketReader::complete() {
    if (_data.size() != _position) {
        boost::throw_exception(InvalidSizeError(_position, _data.size()));
    }
}

template<typename T>
void PacketWriter::Write(const T &value) {
    Write((void *) &value, sizeof(value));
}

void PacketWriter::Write(const void *data, size_t size) {
    size_t position = _data.size();
    _data.resize(position + size);
    memcpy(&_data.front() + position, data, size);
}

void PacketWriter::Write(const std::string &string) {
    Write((uint16_t) string.length());
    Write(string.c_str(), string.length());
}

void PacketWriter::Write(const CommandCode &commandCode) {
    Write((uint16_t) commandCode);
}

void PacketWriter::Write(uint8_t uint8) {
    Write((boost::endian::little_uint8_at) uint8);
}

void PacketWriter::Write(uint16_t uint16) {
    Write((boost::endian::little_uint16_at) uint16);
}

void PacketWriter::Write(int16_t int16) {
    Write((boost::endian::little_int16_at) int16);
}

void PacketWriter::Write(const boost::shared_ptr<const Image> &image) {
    Write(image->GetWidth());
    Write(image->GetHeight());

    for (uint16_t x = 0; x < image->GetWidth(); x++) {
        for (uint16_t y = 0; y < image->GetHeight(); y++) {
            Write(image->GetRed(x, y));
            Write(image->GetGreen(x, y));
            Write(image->GetBlue(x, y));
        }
    }
}
