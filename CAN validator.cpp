#include "pCanAdaptee.h"
#include "testDispatcher.h"
#include "canSignal.h"
#include "testByte.h"
#include "logger.h"

int main(int argc, char** argv)
{
	/*Example usage:
      The ByteTest class initializes the CAN on pCan and uses the signal serializer and deserializer.
	*/
	testDispatcher* dispatcher = testDispatcher::getInstance();
	try {
		dispatcher->addTest<ByteTest>();
		dispatcher->executeTestsInQueue();
	}
	catch(const std::runtime_error& exception){
		Utils::Logger::log(Utils::Logger::logLevel_t::CRITICAL, LOGINVOKER) << exception.what() << std::endl;
		std::cout << exception.what() << std:: endl;
	}
	
	return 0;
}

