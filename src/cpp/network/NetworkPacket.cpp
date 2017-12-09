#include <boost/endian/arithmetic.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/crc.hpp>
#include "NetworkError.h"
#include "NetworkPacket.h"

class PacketReader {
public:
    explicit PacketReader(std::vector<char> data) : _data(std::move(data)), _position(0) {}

    CommandPtr ReadCommand();

    void complete();

private:
    std::vector<char> _data;
    uint32_t _position;

    template<typename T>
    void Read(T &resultHolder);

    void Read(void *dataHolder, size_t size);

    CommandCode ReadCommandCode();

    uint8_t ReadUint8();

    uint16_t ReadUint16();

    int16_t ReadInt16();

    std::string ReadString();

    ImagePtr ReadImage();
};

class PacketWriter {
public:
    PacketWriter() = default;

    const std::vector<char> &GetData() { return _data; }

    void Write(const CommandPtr &command);

private:
    std::vector<char> _data;

    template<typename T>
    void Write(const T &value);

    void Write(const void *data, size_t size);

    void Write(const CommandCode &commandCode);

    void Write(uint8_t uint8);

    void Write(uint16_t uint16);

    void Write(int16_t int16);

    void Write(const std::string &string);

    void Write(const ImagePtr &image);
};

uint32_t Packet::GetCRC() const {
    boost::crc_32_type result;
    result.process_bytes(_data.data(), _data.size());
    return static_cast<uint32_t>(result.checksum());
}

Packet::Packet(const CommandPtr &command) {
    PacketWriter writer;
    writer.Write(command);
    _data = writer.GetData();
}

CommandPtr Packet::GetCommand() const {
    PacketReader reader(_data);
    CommandPtr result = reader.ReadCommand();
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

ImagePtr PacketReader::ReadImage() {
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

CommandPtr PacketReader::ReadCommand() {
    CommandCode commandCode = ReadCommandCode();
    switch (commandCode) {
        case CommandCode::CLEAR:
            return CommandPtr(new ClearCommand());

        case CommandCode::SHOW:
            return CommandPtr(new ShowCommand());

        case CommandCode::COLOR:
            return CommandPtr(new ColorCommand(ReadUint8(), ReadUint8(), ReadUint8()));

        case CommandCode::PIXEL:
            return CommandPtr(new PixelCommand(ReadInt16(), ReadInt16()));

        case CommandCode::RECTANGLE:
            return CommandPtr(
                    new RectangleCommand(ReadInt16(), ReadInt16(), ReadUint16(), ReadUint16()));

        case CommandCode::DIGIT:
            return CommandPtr(new DigitCommand(ReadUint8(), ReadUint8()));

        case CommandCode::SMALL_TEXT:
            return CommandPtr(new SmallTextCommand(ReadInt16(), ReadInt16(), ReadString()));

        case CommandCode::LARGE_TEXT:
            return CommandPtr(new LargeTextCommand(ReadInt16(), ReadInt16(), ReadString()));

        case CommandCode::IMAGE:
            return CommandPtr(new ImageCommand(ReadInt16(), ReadInt16(), ReadString()));

        case CommandCode::ANIMATION:
            return CommandPtr(new AnimationCommand(ReadString()));

        case CommandCode::SLEEP:
            return CommandPtr(new SleepCommand(ReadUint16()));

        case CommandCode::COMPOSITE: {
            uint16_t numberOfCommands = ReadUint16();
            std::vector<CommandPtr> commands;
            commands.reserve(numberOfCommands);
            for (int i = 0; i < numberOfCommands; i++) {
                commands.push_back(ReadCommand());
            }

            return CommandPtr(new CompositeCommand(commands));
        }

        case CommandCode::DEFINE_IMAGE:
            return CommandPtr(new DefineImageCommand(ReadString(), ReadImage()));

        case CommandCode::DEFINE_ANIMATION:
            return CommandPtr(new DefineAnimationCommand(ReadString(), ReadCommand()));

        default:
            boost::throw_exception(UnknownCommandError(commandCode));
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

void PacketWriter::Write(const ImagePtr &image) {
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

void PacketWriter::Write(const CommandPtr &command) {
    Write(command->GetCode());

    switch (command->GetCode()) {
        case CommandCode::CLEAR:
        case CommandCode::SHOW: {
            // no extra data
            break;
        }

        case CommandCode::COLOR: {
            auto &colorCommand = (const ColorCommand &) *command;
            Write(colorCommand.GetRed());
            Write(colorCommand.GetGreen());
            Write(colorCommand.GetBlue());
            break;
        }

        case CommandCode::PIXEL: {
            auto &pixelCommand = (const PixelCommand &) *command;
            Write(pixelCommand.GetX());
            Write(pixelCommand.GetY());
            break;
        }

        case CommandCode::RECTANGLE: {
            auto &rectangleCommand = (const RectangleCommand &) *command;
            Write(rectangleCommand.GetX());
            Write(rectangleCommand.GetY());
            Write(rectangleCommand.GetWidth());
            Write(rectangleCommand.GetHeight());
            break;
        }

        case CommandCode::DIGIT: {
            auto &digitCommand = (const DigitCommand &) *command;
            Write(digitCommand.GetPosition());
            Write(digitCommand.GetDigit());
            break;
        }

        case CommandCode::SMALL_TEXT: {
            auto &smallTextCommand = (const SmallTextCommand &) *command;
            Write(smallTextCommand.GetX());
            Write(smallTextCommand.GetY());
            Write(smallTextCommand.GetText());
            break;
        }

        case CommandCode::LARGE_TEXT: {
            auto &largeTextCommand = (const LargeTextCommand &) *command;
            Write(largeTextCommand.GetX());
            Write(largeTextCommand.GetY());
            Write(largeTextCommand.GetText());
            break;
        }

        case CommandCode::IMAGE: {
            auto &drawImageCommand = (const ImageCommand &) *command;
            Write(drawImageCommand.GetX());
            Write(drawImageCommand.GetY());
            Write(drawImageCommand.GetName());
            break;
        }

        case CommandCode::ANIMATION: {
            auto &animationCommand = (const AnimationCommand &) *command;
            Write(animationCommand.GetName());
            break;
        }

        case CommandCode::SLEEP: {
            auto &sleepCommand = (const SleepCommand &) *command;
            Write(sleepCommand.GetMillis());
            break;
        }

        case CommandCode::COMPOSITE: {
            auto &compositeCommand = (const CompositeCommand &) *command;
            Write((uint16_t) compositeCommand.GetCommands().size());
            for (auto &it : compositeCommand.GetCommands()) {
                Write(it);
            }
            break;
        }

        case CommandCode::DEFINE_IMAGE: {
            auto &defineImageCommand = (const DefineImageCommand &) *command;
            Write(defineImageCommand.GetName());
            Write(defineImageCommand.GetImage());
            break;
        }

        case CommandCode::DEFINE_ANIMATION: {
            auto &defineAnimationCommand = (const DefineAnimationCommand &) *command;
            Write(defineAnimationCommand.GetName());
            Write(defineAnimationCommand.GetCommand());
            break;
        }

        default:
            boost::throw_exception(UnknownCommandError(command->GetCode()));
    }
}
