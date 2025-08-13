#include "canAdapter.h"
#include <iostream>
#include "logger.h"

canAdapter::canAdapter() {
	this->initCanBuffers();
}

canAdapter::~canAdapter() {
		Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "Asking for CAN set get threads to stop" << std::endl;
		this->stopThreadsRequest = true;
		if (rxThread.joinable()){
			Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "Waiting for CAN get thread to stop" << std::endl;
			rxThread.join();
			Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "CAN get thread stopped" << std::endl;
		}
		if (txThread.joinable()) {
			Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "Waiting for CAN set thread to stop" << std::endl;
			txThread.join();
			Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "CAN set thread stopped" << std::endl;
		}
		Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "CAN set get threads stopped succesfully" << std::endl;
}

void canAdapter::initCanBuffers() {
	Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "Init CAN buffers" << std::endl;
	auto txIter = txBuffer.begin();
	for (auto element : canSignals::messagesTx) {
		txIter->id = element.second.messageId;
		txIter->dlc = element.second.dlc;
		txIter->extended = element.second.extended;
		txIter++;
	}

	auto rxIter = rxBuffer.begin();
	for (auto& element : canSignals::messagesRx) {
		rxIter->id = element.second.messageId;
		rxIter->dlc = element.second.dlc;
		rxIter->extended = element.second.extended;
		rxIter++;
	}
	Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "Init CAN buffers end" << std::endl;
}

void canAdapter::setCanSignal(const canSignals::canMessageMap_t& message, const canSignals::canSignalMap_t& signal, const uint32_t& valueToSet) {
	uint8_t byteId = signal.byteId; 
	uint8_t bytesToWrite = ((signal.sizeInBit +signal.bitId + 7) / 8) - (signal.bitId/8);
	uint64_t mask = ((1ULL << signal.sizeInBit) - 1) << signal.bitId;
	uint64_t value = (static_cast<uint64_t>(valueToSet) & ((1ULL << signal.sizeInBit) - 1)) << signal.bitId;
	
	std::lock_guard<std::mutex> txLock(this->txMutex);
	
	auto txVectorIterator = std::find_if(this->txBuffer.begin(), this->txBuffer.end(), [&message](const canMessage& messageinVect) {
		return messageinVect.id == message.messageId;
		});
	if (txVectorIterator == txBuffer.end()) {
		Utils::Logger::log(Utils::Logger::logLevel_t::CRITICAL, LOGINVOKER) << "Element called not present in TxBuffer, throwing exception" << std::endl;
		throw std::out_of_range("Element not present in TxBuffer");
		return;
	}
	for (uint8_t bytesWritten = 0; bytesWritten < bytesToWrite; bytesWritten++) {
		uint8_t currentByte = byteId + bytesWritten;
		uint8_t byteMask = ~(mask >> (bytesWritten * 8));
		uint8_t byteValue = value >> (bytesWritten * 8);
		txVectorIterator->payload.at(currentByte) &= byteMask;
		txVectorIterator->payload.at(currentByte) |= byteValue;
	}
}

uint64_t canAdapter::getCanSignal(const canSignals::canMessageMap_t& message, const canSignals::canSignalMap_t& signal) {
	uint64_t returnValue  = 0;
	uint8_t  bytesToWrite = ((signal.sizeInBit + signal.bitId + 7) / 8) - (signal.bitId / 8);

	std::lock_guard<std::mutex> rxLock(this->rxMutex);

	auto rxVectorIterator = std::find_if(this->rxBuffer.begin(), this->rxBuffer.end(), [&message](const canMessage& messageInVect) {
		return messageInVect.id == message.messageId;
		});
	if (rxVectorIterator == rxBuffer.end()) {
		Utils::Logger::log(Utils::Logger::logLevel_t::CRITICAL, LOGINVOKER) << "Element called not present in TxBuffer, throwing exception" << std::endl;
		throw std::out_of_range("Element not present in RxBuffer");
		return 0;
	}
	memcpy(&returnValue, &rxVectorIterator->payload[signal.byteId], bytesToWrite);
	returnValue >>= signal.bitId;
	returnValue &= (1ULL << signal.sizeInBit) - 1;
	return returnValue;
}

void canAdapter::threadsStart() {
	Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "Starting set signal and get signal threads" << std::endl;
	rxThread = std::thread([this]() {this->readFromDevice();});
	txThread = std::thread([this]() {this->writeToDevice();});
	Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "Set signal and get signal threads started" << std::endl;
}