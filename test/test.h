#ifndef TEST
#define TEST

#include <unordered_map>
#include "logger.h"
#include "canSignal.h"

class test {
public:
	/*typedefs*/
protected:
	/*typedefs*/
	enum class testResult {
		RUNNING,
		PASS,
		FAIL,
		NOTSTARTED
	};
	static const std::unordered_map<testResult, std::string> resultToString;
public:
	/*constructor, destructor, copy*/
	test() = default;
	virtual ~test() = default;
	test(const test&) = delete;
	test(test&&) = delete;
public:
	/*operators overload*/
	test& operator = (const test&) = delete;
	test& operator = (test&&) = delete;

public:
	/*pure virtual interface methods*/
	virtual void executeTest() = 0;

public:
	/*getters*/
	inline std::string getTestResult() const noexcept {return this->resultToString.at(result);}
	inline std::string getTestName()   const noexcept {return this->testName;}

protected:
	testResult result{testResult::NOTSTARTED};
	std::string testName{"test"};
protected:
	/*setters*/
	inline void setTestName(std::string name)  noexcept { testName = name; }
	inline void setTestResult(testResult result) noexcept { this->result = result; }
protected:
	/*ausiliary methods*/
	std::string buildSignalString(canSignals::messageName, canSignals::signalName) const;

};




#endif