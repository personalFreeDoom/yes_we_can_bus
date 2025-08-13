#include "testDispatcher.h"

using namespace Utils;

testDispatcher* testDispatcher::getInstance() {
	static testDispatcher instance;
	return &instance;
}

void testDispatcher::executeTestsInQueue() {
	Logger::log(Logger::logLevel_t::INFO, LOGINVOKER) << "Starting tests" << std::endl;
	while (!this->testQueue.empty()) {
		Logger::log(Logger::logLevel_t::INFO, LOGINVOKER) << "Starting test:" << testQueue.front()->getTestName() << std::endl;
		this->testQueue.front()->executeTest();
		Logger::log(Logger::logLevel_t::INFO, LOGINVOKER) << "Test finished:" << testQueue.front()->getTestName() << " Result:" << testQueue.front()->getTestResult() << std::endl;
		this->testQueue.pop();
	}
	Logger::log(Logger::logLevel_t::INFO, LOGINVOKER) << "all tests finished" << std::endl;
}