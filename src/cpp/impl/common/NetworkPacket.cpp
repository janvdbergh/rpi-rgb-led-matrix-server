#include <common/NetworkPacket.h>
#include <boost/crc.hpp>

uint32_t NetworkPacket::get_crc() const {
	boost::crc_32_type result;
	result.process_bytes(_data.data(), _data.size());
	return static_cast<uint32_t>(result.checksum());
}
