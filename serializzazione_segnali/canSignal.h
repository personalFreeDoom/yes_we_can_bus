#ifndef CANSIGNALS
#define CANSIGNALS
#include <unordered_map>
#include <vector>
#include <string>


namespace canSignals {
	/*typedefs*/
	enum class messageName {
		BYTETESTTX,
		BYTETESTRX
	};
	enum class signalName {
	    BYTETESTBIT0,
		BYTETESTBIT1,
		BYTETESTBIT6
	};

	/*signal rappresentation*/
	struct canSignalMap_t {
		uint8_t  bitId;
		uint8_t  byteId;
		uint16_t sizeInBit;
	};
	/*message rappresentation*/
	struct canMessageMap_t {
		uint32_t messageId;
		uint8_t dlc;
		bool extended;
		std::unordered_map<signalName, canSignalMap_t> signals;
	};
	/*typedefs end*/

	/*maps*/
	const std::unordered_map<messageName, canMessageMap_t> messagesTx = {
		{messageName::BYTETESTTX, //per test testByte
			{.messageId = 0x18FEF1F0, .dlc = 8, .extended = true,
				.signals = {
					{signalName::BYTETESTBIT0,  {.bitId = 0, .byteId = 0, .sizeInBit = 8}},
					{signalName::BYTETESTBIT1,  {.bitId = 1, .byteId = 1, .sizeInBit = 8}},
					{signalName::BYTETESTBIT6,  {.bitId = 6, .byteId = 3, .sizeInBit = 8}}
				}
			}
		},
	};
	const size_t txBufferSize = canSignals::messagesTx.size();

	const std::unordered_map<messageName, canMessageMap_t> messagesRx = { 
		{messageName::BYTETESTRX,
			{.messageId = 0x205, .dlc = 8, .extended = false,
				.signals = {
					{signalName::BYTETESTBIT0, {.bitId = 0, .byteId = 0, .sizeInBit = 8}},
				}
			}
		},
	};
	const size_t rxBufferSize = canSignals::messagesRx.size();
}

#endif