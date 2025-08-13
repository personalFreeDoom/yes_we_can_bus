#include "test.h"
#include <sstream>

const std::unordered_map<test::testResult, std::string> test::resultToString{
    {testResult::RUNNING,    "Test Running"},
	{testResult::PASS,       "Test Pass"},
    {testResult::FAIL,       "Test Fail"},
    {testResult::NOTSTARTED, "Test Not Started"},
};

std::string test::buildSignalString(canSignals::messageName message, canSignals::signalName signal) const {
	using namespace canSignals;
	std::ostringstream stream;
	if (canSignals::messagesTx.contains(message)) {
		stream << "\n\tId: " << messagesTx.at(message).messageId <<
			"\n\tBitId: " << messagesTx.at(message).signals.at(signal).bitId <<
			"\n\tByteId: " << messagesTx.at(message).signals.at(signal).byteId <<
			"\n\tSize: " << messagesTx.at(message).signals.at(signal).sizeInBit;
	}
	else {
		if (canSignals::messagesRx.contains(message)) {
			stream << "\n\tId: " << messagesRx.at(message).messageId <<
				"\n\tBitId: " << messagesRx.at(message).signals.at(signal).bitId <<
				"\n\tByteId: " << messagesRx.at(message).signals.at(signal).byteId <<
				"\n\tSize: " << messagesRx.at(message).signals.at(signal).sizeInBit << std::endl;
		}
		else {
			return "";
		}
	}
	return stream.str();
}