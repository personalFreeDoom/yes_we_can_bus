#ifndef CAN_ADAPTER
#define CAN_ADAPTER

#include "canSignal.h"
#include <bitset>
#include <array>
#include <mutex>
#include <functional>
#include "logger.h"


class canAdapter {
public:
	/*public typedefs*/

protected:
	/*protected typedefs*/
	struct canMessage {
		uint32_t id;
		uint8_t  dlc;
		bool     extended;
		std::array<uint8_t, 8> payload;
	};
private:
	/*private typedefs*/
public:
	/*costructor, destructor, copy, move*/
	canAdapter();
	virtual ~canAdapter();
	canAdapter(const canAdapter&) = delete;
	canAdapter(canAdapter&&) = delete;
	canAdapter& operator = (const canAdapter&) = delete;
	canAdapter& operator = (canAdapter&&) = delete;
public:
	/*methods*/
	void initCanBuffers();
	//in: message to set 
	//in: signal to set
	//in: value to set
	void setCanSignal(const canSignals::canMessageMap_t&, const canSignals::canSignalMap_t&, const uint32_t&);
	//in: message to get
	//in: signal to get 
	//out: value (max 64 bit, dlc 8)
	uint64_t getCanSignal(const canSignals::canMessageMap_t& , const canSignals::canSignalMap_t&);

protected:
	/*********************************************************************
	 *mirror buffers, get and set are done in this buffers
	 ********************************************************************/
	std::vector<canMessage> txBuffer{canSignals::txBufferSize};
	std::vector<canMessage> rxBuffer{canSignals::rxBufferSize};

protected:
	/*******************************************************************
	*threads management
	*******************************************************************/
	std::mutex rxMutex;
	std::mutex txMutex;
	std::atomic<bool> stopThreadsRequest = false;
	std::thread rxThread;
	std::thread txThread;
	void threadsStart();

protected:
	/*pure virtual interface methods*/
	virtual void connectToDevice()      = 0;
	virtual void disconnectFromDevice() = 0;
	virtual void readFromDevice()       = 0;
	virtual void writeToDevice()        = 0;
};
#endif //CAN_ADAPTER
