#ifndef TEST_BYTE
#define TEST_BYTE

#include "test.h"
#include "canAdapter.h"
#include "canSignal.h"
#include "logger.h"
#include <iostream>


class ByteTest final : public test {
public:
	//constructor, destructor, copy
	ByteTest() = default;
	~ByteTest() = default; //final class
	ByteTest(const ByteTest&) = delete;
	ByteTest(ByteTest&&) = delete;

public:
	void executeTest() override;
};


#endif