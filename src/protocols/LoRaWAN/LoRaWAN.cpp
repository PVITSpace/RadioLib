#include "LoRaWAN.h"

/*LoRaWANBand_t EU868 {
  .numChannelSpans = 2,
  .downlinkChannelMod = 0xFF,
  .uplinkDefault = {
    {
      .numChannels = 3,
      .freqStart = 868.1,
      .freqStep = 0.2,
      .numDataRates = 6,
      .dataRates = {
        LORAWAN_DATA_RATE_SF_12 | LORAWAN_DATA_RATE_BW_125_KHZ,
        LORAWAN_DATA_RATE_SF_11 | LORAWAN_DATA_RATE_BW_125_KHZ,
        LORAWAN_DATA_RATE_SF_10 | LORAWAN_DATA_RATE_BW_125_KHZ,
        LORAWAN_DATA_RATE_SF_9 | LORAWAN_DATA_RATE_BW_125_KHZ,
        LORAWAN_DATA_RATE_SF_8 | LORAWAN_DATA_RATE_BW_125_KHZ,
        LORAWAN_DATA_RATE_SF_7 | LORAWAN_DATA_RATE_BW_125_KHZ
      }
    }, {
      .numChannels = 0,
      .freqStart = 0,
      .freqStep = 0,
      .numDataRates = 0,
      .dataRates = { }
    }
  },
  .uplinkAvailable = {
    .numChannels = 3,
    .freqStart = 868.1,
    .freqStep = 0.2,
    .numDataRates = 6,
    .dataRates = {
      LORAWAN_DATA_RATE_SF_12 | LORAWAN_DATA_RATE_BW_125_KHZ,
      LORAWAN_DATA_RATE_SF_11 | LORAWAN_DATA_RATE_BW_125_KHZ,
      LORAWAN_DATA_RATE_SF_10 | LORAWAN_DATA_RATE_BW_125_KHZ,
      LORAWAN_DATA_RATE_SF_9 | LORAWAN_DATA_RATE_BW_125_KHZ,
      LORAWAN_DATA_RATE_SF_8 | LORAWAN_DATA_RATE_BW_125_KHZ,
      LORAWAN_DATA_RATE_SF_7 | LORAWAN_DATA_RATE_BW_125_KHZ
    }
  },
  .downlinkDefault = {
    {
      .numChannels = 3,
      .freqStart = 868.1,
      .freqStep = 0.2,
      .numDataRates = 6,
      .dataRates = {
        LORAWAN_DATA_RATE_SF_12 | LORAWAN_DATA_RATE_BW_125_KHZ,
        LORAWAN_DATA_RATE_SF_11 | LORAWAN_DATA_RATE_BW_125_KHZ,
        LORAWAN_DATA_RATE_SF_10 | LORAWAN_DATA_RATE_BW_125_KHZ,
        LORAWAN_DATA_RATE_SF_9 | LORAWAN_DATA_RATE_BW_125_KHZ,
        LORAWAN_DATA_RATE_SF_8 | LORAWAN_DATA_RATE_BW_125_KHZ,
        LORAWAN_DATA_RATE_SF_7 | LORAWAN_DATA_RATE_BW_125_KHZ
      }
    }, {
      .numChannels = 0,
      .freqStart = 0,
      .freqStep = 0,
      .numDataRates = 0,
      .dataRates = { }
    }
  },
  .downlinkAvailable = {
    .numChannels = 3,
    .freqStart = 868.1,
    .freqStep = 0.2,
    .numDataRates = 6,
    .dataRates = {
      LORAWAN_DATA_RATE_SF_12 | LORAWAN_DATA_RATE_BW_125_KHZ,
      LORAWAN_DATA_RATE_SF_11 | LORAWAN_DATA_RATE_BW_125_KHZ,
      LORAWAN_DATA_RATE_SF_10 | LORAWAN_DATA_RATE_BW_125_KHZ,
      LORAWAN_DATA_RATE_SF_9 | LORAWAN_DATA_RATE_BW_125_KHZ,
      LORAWAN_DATA_RATE_SF_8 | LORAWAN_DATA_RATE_BW_125_KHZ,
      LORAWAN_DATA_RATE_SF_7 | LORAWAN_DATA_RATE_BW_125_KHZ
    }
  },
  .downlinkBackup = {
    .numChannels = 1,
    .freqStart = 869.858,
    .freqStep = 0,
    .numDataRates = 1,
    .dataRates = {
      LORAWAN_DATA_RATE_SF_12 | LORAWAN_DATA_RATE_BW_125_KHZ
    }
  }
};*/

uint8_t EU868::getDownlinkChannel(uint8_t txChan) {
  return(txChan);
}

LoRaWANNode::LoRaWANNode(PhysicalLayer* phy, LoRaWANBand_t* band) {
  _phy = phy;
  _band = band;
}
