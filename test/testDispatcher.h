#ifndef TEST_DISPATCHER
#define TEST_DISPATCHER

#include "test.h"
#include "logger.h"
#include <queue>

class testDispatcher final {
public:
	//istance getter
	static testDispatcher* getInstance();
	//for add a test
	template <typename TestType>
	typename std::enable_if<std::is_base_of<test, TestType>::value>::type
	addTest() {this->testQueue.push(std::make_unique <TestType>());}
	//for start tests in queue
	void executeTestsInQueue();
public:
	/*destructor, copy*/
	
	testDispatcher (const testDispatcher&) = delete;
	testDispatcher (testDispatcher&&) = delete;
public:
	/*operators overload*/
	testDispatcher& operator = (const testDispatcher&) = delete; 
	testDispatcher& operator = (testDispatcher&&) = delete;

private:
	//private contructor for singleton pattern
	testDispatcher() = default;
	~testDispatcher() = default;

private:
	/*containers*/
	std::queue<std::unique_ptr<test>> testQueue;
};

#endif