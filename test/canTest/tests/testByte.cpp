#include "testByte.h"
#include "pCanAdaptee.h"

void ByteTest::executeTest() {
	using namespace Utils;
	using namespace canSignals;

	//setting name and status
	this->setTestName("Byte Test");
	this->setTestResult(testResult::RUNNING);

	//pointer to pCanAdaptee
	std::unique_ptr<canAdapter> can = std::make_unique<pCanAdaptee>();

	Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "STARTING TEST: " << this->getTestName();
	uint8_t valueToTest = 0XFF;
	//printing message and signal map
	Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "Sending value: " << valueToTest << "On:" <<
		this->buildSignalString(messageName::BYTETESTTX, signalName::BYTETESTBIT0) << std::endl;

	//setting can signal with a value
	can->setCanSignal(messagesTx.at(messageName::BYTETESTTX), messagesTx.at(messageName::BYTETESTTX).signals.at(signalName::BYTETESTBIT0), valueToTest);
	//waiting an arbitrary time for response
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//getting value of a can signal
	uint8_t receivedValue = can->getCanSignal(messagesRx.at(messageName::BYTETESTRX), messagesRx.at(messageName::BYTETESTRX).signals.at(signalName::BYTETESTBIT0));
	//evauling result
	if (valueToTest != receivedValue) {
		this->setTestResult(testResult::FAIL);
	}
	else {
		this->setTestResult(testResult::PASS);
	}
	Logger::log(Utils::Logger::logLevel_t::INFO, LOGINVOKER) << "Received value = " << receivedValue << "Expetded value: " << valueToTest <<
		"Test Result" << this->getTestResult();
}
