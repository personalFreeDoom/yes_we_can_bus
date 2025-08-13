#include "pCanAdaptee.h"
#include <string>
#include <queue>


pCanAdaptee::pCanAdaptee() {
	this->connectToDevice();
}

void pCanAdaptee::connectToDevice() {
	bool connected = false;
	TPCANStatus status{};
	for (auto usbInterface : this->pCanUsbInterfaces) {
		Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "Trying to connect on pcan deviceId:" << usbInterface << std::endl;
		status = CAN_Initialize(usbInterface, PCAN_BAUD_250K);
		if (status == PCAN_ERROR_OK) {
			this->deviceHandler = usbInterface;
			connected = true;
			Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "Connected on pcan deviceId:" << usbInterface << std::endl;
			break;
		}
	}
	if (!connected) {
		char statusString[256];
		CAN_GetErrorText(status, 0, statusString);
		Utils::Logger::log(Utils::Logger::logLevel_t::CRITICAL, LOGINVOKER) << "Not connected, pCan error: " << statusString << std::endl;
		throw std::runtime_error("Cannot find any pCanUsb device, not connected");
	}
	else {
		this->threadsStart();
	}
}

void pCanAdaptee::disconnectFromDevice() noexcept {
	TPCANStatus status;
	status = CAN_Uninitialize(this->deviceHandler);
	if (status == PCAN_ERROR_OK) {
		Utils::Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "pCanDevice disconnected correctly" << std::endl;
	}
	else {
		char statusString[256];
		CAN_GetErrorText(status, 0, statusString);
		Utils::Logger::log(Utils::Logger::logLevel_t::WARNING, LOGINVOKER) << "pCanDevice NOT disconnected correctly: " << statusString << std::endl;
	}
}

pCanAdaptee::pCanStatus pCanAdaptee::getStatus() const noexcept{
	pCanStatus status;
	status.status = CAN_GetStatus(this->deviceHandler);
	CAN_GetErrorText(status.status, 0, status.statusString);
	return status;
}

void pCanAdaptee::writeToDevice() noexcept {
	while (this->stopThreadsRequest == false) {
		std::vector<canMessage> localTxBuffer;
		{
			std::lock_guard<std::mutex> TxLock(this->txMutex);
			localTxBuffer = this->txBuffer;
		}
		for (auto& message : localTxBuffer) {
			TPCANMsg deviceMsg;
			deviceMsg.ID = message.id;
			if (message.extended) {
				deviceMsg.MSGTYPE = PCAN_MESSAGE_EXTENDED;
			}
			else {
				deviceMsg.MSGTYPE = PCAN_MESSAGE_STANDARD;
			}
			deviceMsg.LEN = message.dlc;
			std::copy(message.payload.begin(), message.payload.begin() + message.dlc, std::begin(deviceMsg.DATA));
			TPCANStatus sendResult;
			sendResult = CAN_Write(this->deviceHandler, &deviceMsg);
			if (sendResult != PCAN_ERROR_OK) {
				char resultString[256]{};
				CAN_GetErrorText(sendResult, 0, resultString);
				Utils::Logger::log(Utils::Logger::logLevel_t::WARNING, LOGINVOKER) << "Message send error id: " << message.id << "pCan error: " << resultString << std::endl;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void pCanAdaptee::readFromDevice() noexcept {
	while (this->stopThreadsRequest == false) {
		std::queue<canMessage> messagesFromDevice{};
		canMessage messageParsedFromDevice{};
		TPCANMsg receivedMessage{};
		TPCANStatus readResult{};
		char resultString[256]{};

		do {
			readResult = CAN_Read(this->deviceHandler, &receivedMessage, NULL);
			if (readResult != PCAN_ERROR_QRCVEMPTY) {
				switch (receivedMessage.MSGTYPE) {
				case PCAN_MESSAGE_STANDARD:
					messageParsedFromDevice.extended = false;
					break;
				case PCAN_MESSAGE_EXTENDED:
					messageParsedFromDevice.extended = true;
					break;
				default:
					continue;
					break;
				}
				messageParsedFromDevice.id = receivedMessage.ID;
				messageParsedFromDevice.dlc = receivedMessage.LEN;
				std::copy(std::begin(receivedMessage.DATA), std::begin(receivedMessage.DATA) + receivedMessage.LEN, messageParsedFromDevice.payload.begin());
				messagesFromDevice.push(messageParsedFromDevice);
			}
		} 
		while (readResult  != PCAN_ERROR_QRCVEMPTY);
		{
		    std::lock_guard<std::mutex>rxLock(this->rxMutex);
			while (!messagesFromDevice.empty()) {
				auto rxVectorIterator = std::find_if(this->rxBuffer.begin(), this->rxBuffer.end(), [&messagesFromDevice](const canMessage& messageinVect) {
					return messageinVect.id == messagesFromDevice.front().id;
					});
				if (rxVectorIterator != this->rxBuffer.end()) {
					std::copy(messagesFromDevice.front().payload.begin(), (messagesFromDevice.front().payload.begin() + messagesFromDevice.front().dlc), rxVectorIterator->payload.begin());
				}
				messagesFromDevice.pop();
			}
		}
	}
}