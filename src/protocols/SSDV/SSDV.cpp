#include "SSDV.h"

SSDVClient::SSDVClient(RTTYClient* rtty) {
  _rtty = rtty;
}

int16_t SSDVClient::begin(const char* callsign) {
  // check source callsign length (6 characters max)
  if(strlen(callsign) > SSDV_MAX_CALLSIGN_LENGTH) {
    return(ERR_INVALID_CALLSIGN);
  }

  // encode callsign
  callsignEnc = encodeBase40(callsign);

  return(ERR_NONE);
}

int16_t SSDVClient::startTransfer(uint8_t mode, uint8_t* image, uint8_t imageID, uint16_t width, uint16_t height, uint8_t quality, uint8_t subsampling) {
  // check all parameters
  if(!((mode == SSDV_PACKET_TYPE_NORMAL) || (mode == SSDV_PACKET_TYPE_NO_FEC))) {
    return(ERR_INVALID_SSDV_MODE);
  }
  if((width % 16 != 0) || (height % 16 != 0)) {
    return(ERR_INVALID_IMAGE_SIZE);
  }
  RADIOLIB_CHECK_RANGE(quality, 0, 7, ERR_INVALID_IMAGE_QUALITY);
  RADIOLIB_CHECK_RANGE(subsampling, 0, 3, ERR_INVALID_SUBSAMPLING);

  // save the parameters
  _mode = mode;
  _img = image;
  _imgID = imageID;
  _w = width;
  _h = height;
  _qual = (quality ^ 4) << 3;
  _sub = subsampling;

  // initialize internal counters
  _packetID = 0;

  return(ERR_NONE);
}

void SSDVClient::sendPacket() {
  uint8_t buff[SSDV_PACKET_LENGTH];
  uint8_t* buffPtr = buff;

  // sync byte
  *buffPtr++ = SSDV_SYNC;

  // mode
  *buffPtr++ = _mode;

  // callsign
  memcpy(buffPtr, &callsignEnc, sizeof(callsignEnc));
  buffPtr += sizeof(callsignEnc);

  // image ID
  *buffPtr++ = _imgID;

  // packet ID
  memcpy(buffPtr, &_packetID, sizeof(_packetID));
  buffPtr += sizeof(_packetID);
  _packetID++;

  // width and height
  *buffPtr++ = _w;
  *buffPtr++ = _h;

  // flags - check if this is the last packet
  *buffPtr++ = _qual | _sub;

  // MCU offset and index

  // payload data

  // 32-bit CRC and Reed-Solomon FEC (normal mode only)
  uint32_t crc;
  if(_mode == SSDV_PACKET_TYPE_NORMAL) {
    // normal mode, use FEC
    crc = getChecksum(buff + SSDV_PAYLOAD_POS, SSDV_PAYLOAD_LEN_NORMAL);
    memcpy(buff + SSDV_CHECKSUM_POS_NORMAL, &crc, sizeof(uint32_t));

    encodeRS8(buff + 1, buff + SSDV_FEC_POS, 0);

  } else {
    // no-FEC mode, CRC only
    crc = getChecksum(buff + SSDV_PAYLOAD_POS, SSDV_PAYLOAD_LEN_NO_FEC);
    memcpy(buff + SSDV_CHECKSUM_POS_NO_FEC, &crc, sizeof(uint32_t));

  }

  // send the packet
  _rtty->write(buff, SSDV_PACKET_LENGTH);
}

/*
  Base-40 encoding implementation based on https://github.com/fsphil/ssd

  Licensed under GNU General Public License v3.0
  https://github.com/fsphil/ssdv/blob/035f920f5c96880bfd89d4469428b934e830c7c9/COPYING
*/
uint32_t SSDVClient::encodeBase40(char* str) {
  // sanity checks
  uint8_t len = strlen(str);
  if(len == 0) {
    return(0x00000000);
  } else if(len > 6) {
    return(0xFFFFFFFF)
  }

  // encode
  uint32_t enc = 0;
  for(int8_t i = len - 1; i >= 0; i--) {
    enc *= 40;
    if((str[i] >= 'A') && (str[i] <= 'Z')) {
      enc += str[i] - 'A' + 14;
    } else if((str[i] >= '0') && (str[i] <= '9')) {
      enc += str[i] - '0' + 1;
    }
  }

  return(enc);
}

/*
  SSDV CRC32 implementation from https://github.com/fsphil/ssd

  Licensed under GNU General Public License v3.0
  https://github.com/fsphil/ssdv/blob/035f920f5c96880bfd89d4469428b934e830c7c9/COPYING
*/
uint32_t SSDVClient::getChecksum(uint8_t* data, size_t len) {
  uint32_t crc, x;
	uint8_t i, *d;

	for(d = data, crc = SSDV_CRC32_INITIAL; len; len--) {
		x = (crc ^ *(d++)) & 0xFF;
		for(i = 8; i > 0; i--) {
			if(x & 1) {
        x = (x >> 1) ^ SSDV_CRC32_POLYNOMIAL;
      } else {
        x >>= 1;
      }
		}
		crc = (crc >> 8) ^ x;
	}

	return(crc ^ SSDV_CRC32_INITIAL);
}

int16_t mod255(int16_t val) {
  while(val >= 255) {
		val -= 255;
		val = (val >> 8) + (val & 255);
	}
	return(val);
}

/*
  SSDV Reed-Solomon forward error correction implementation from https://github.com/fsphil/ssd

  Licensed under GNU General Public License v3.0
  https://github.com/fsphil/ssdv/blob/035f920f5c96880bfd89d4469428b934e830c7c9/COPYING
*/
void SSDVClient::encodeRS8(uint8_t* data, uint8_t* parity, int16_t pad) {
  int16_t i, j;
	uint8_t feedback;

	memset(parity, 0, SSDV_RS_FEC_NROOTS * sizeof(uint8_t));

	for(i = 0; i < SSDV_RS_FEC_NN - SSDV_RS_FEC_NROOTS - pad; i++) {
		feedback = pgm_read_byte(&rs8_index_of[data[i] ^ parity[0]]);

    if(feedback != SSDV_RS_FEC_A0) { /* feedback term is non-zero */
			for(j = 1; j < SSDV_RS_FEC_NROOTS; j++) {
				parity[j] ^= pgm_read_byte(&rs8_alpha_to[mod255(feedback + pgm_read_byte(&rs8_poly[SSDV_RS_FEC_NROOTS - j]))]);
      }
		}

		/* Shift */
		memmove(&parity[0], &parity[1], sizeof(uint8_t) * (SSDV_RS_FEC_NROOTS - 1));
		if(feedback != SSDV_RS_FEC_A0) {
			parity[SSDV_RS_FEC_NROOTS - 1] = pgm_read_byte(&rs8_alpha_to[mod255(feedback + pgm_read_byte(&poly[0]))]);
		} else {
			parity[SSDV_RS_FEC_NROOTS - 1] = 0;
    }
	}
}
