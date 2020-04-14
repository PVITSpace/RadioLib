#ifndef _RADIOLIB_LORAWAN_H
#define _RADIOLIB_LORAWAN_H

#include "../../TypeDef.h"
#include "../PhysicalLayer/PhysicalLayer.h"

//https://github.com/Lora-net/LoRaMac-node

// preamble format
#define LORAWAN_LORA_SYNC_WORD                        0x34
#define LORAWAN_LORA_PREAMBLE_LEN                     8
#define LORAWAN_GFSK_SYNC_WORD                        0xC194C1
#define LORAWAN_GFSK_PREAMBLE_LEN                     5

// data rate field encoding                                           MSB   LSB   DESCRIPTION
#define LORAWAN_DATA_RATE_SF_12                       0b00000000  //  7     4     LoRaWAN spreading factor: SF12
#define LORAWAN_DATA_RATE_SF_11                       0b00010000  //  7     4                                         SF11
#define LORAWAN_DATA_RATE_SF_10                       0b00100000  //  7     4                                         SF10
#define LORAWAN_DATA_RATE_SF_9                        0b00110000  //  7     4                                         SF9
#define LORAWAN_DATA_RATE_SF_8                        0b01000000  //  7     4                                         SF8
#define LORAWAN_DATA_RATE_SF_7                        0b01010000  //  7     4                                         SF7
#define LORAWAN_DATA_RATE_FSK_50_K                    0b01100000  //  7     4                                         FSK @ 50 kbps
#define LORAWAN_DATA_RATE_BW_500_KHZ                  0b00000000  //  3     0     LoRaWAN bandwidth: 500 kHz
#define LORAWAN_DATA_RATE_BW_250_KHZ                  0b00000001  //  3     0                        250 kHz
#define LORAWAN_DATA_RATE_BW_125_KHZ                  0b00000010  //  3     0                        125 kHz

// to save space, channels are saved in "spans"
struct LoRaWANChannelSpan_t {
  uint8_t numChannels;            // total number of channels in the span
  float freqStart;                // center frequency of the first channel in span
  float freqStep;                 // frequency step between adjacent channels
  uint8_t numDataRates;           // number of datarates supported by the all channels in the span
  uint8_t dataRates[6];           // array of datarates supported by all channels in the span (no channel span has more than 6 allowed data rates)
};

struct LoRaWANBand_t {
  uint8_t numChannelSpans;                      // number of channel spans in the band
  LoRaWANChannelSpan_t uplinkDefault[2];        // default uplink (TX) channels (defined by LoRaWAN Regional Parameters)
  LoRaWANChannelSpan_t uplinkAvailable;         // available uplink (TX) channels (not defined by LoRaWAN Regional Parameters)
  LoRaWANChannelSpan_t downlinkDefault[2];      // default downlink (RX1) channels (defined by LoRaWAN Regional Parameters)
  LoRaWANChannelSpan_t downlinkAvailable;       // available downlink (RX1) channels (not defined by LoRaWAN Regional Parameters)
  LoRaWANChannelSpan_t downlinkBackup;          // backup downlink (RX2) channels - just a single channel, but using the same structure for convenience
};

struct EU868: public LoRaWANBand_t {
  uint8_t getDownlinkChannel(uint8_t txChan);   // method that returns RX1 channel number based on TX channel
  uint8_t getDownlinkDataRate(uint8_t offset);  // method that returns RX1 channel number based on TX channel
};

class LoRaWANNode {
  public:

    LoRaWANNode(PhysicalLayer* phy, LoRaWANBand_t* band);

  #ifndef RADIOLIB_GODMODE
    private:
  #endif
    PhysicalLayer* _phy;
    LoRaWANBand_t* _band;
};

#endif
