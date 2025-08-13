# yes_we_can_bus

A C++23 system for CAN bus communication validation and testing using the Adapter pattern to interface with PCAN devices.

## Features

- **Adapter Pattern**: Clean implementation for interfacing with different CAN devices
- **Signal Serialization/Deserialization**: Automatic CAN signal handling with bit-level mapping
- **Modular Test System**: Framework for creating and executing automated tests
- **Advanced Logging**: Thread-safe logging system with automatic file rotation
- **Threading**: Separate CAN read/write management in dedicated threads

## Architecture

### Core Components

- **`canAdapter`**: Abstract base class defining the CAN communication interface
- **`pCanAdaptee`**: Specific implementation for PCAN USB devices
- **`canSignal.h`**: CAN message and signal definitions with precise mapping
- **Test System**: Modular framework for validation testing
- **Logger**: Thread-safe logging system with multiple level support

### Design Patterns Used

- **Adapter Pattern**: To interface with different types of CAN devices
- **Singleton Pattern**: For logger and test dispatcher
- **RAII**: Automatic resource management

## Requirements

- **Compiler**: C++23 compatible
- **Operating System**: Windows (for PCAN)
- **Hardware**: PCAN USB device
- **Libraries**: PCANBasic (included)

## Build

The project uses CMake:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Dependencies

- `PCANBasic.lib` - PCAN library (included in `/Pcan`)
- `PCANBasic.dll` - PCAN DLL (automatically copied)

## Usage

### Basic Example

```cpp
#include "pCanAdaptee.h"
#include "canSignal.h"

int main() {
    // Create PCAN adapter
    std::unique_ptr<canAdapter> can = std::make_unique<pCanAdaptee>();
    
    // Send a CAN signal
    uint8_t value = 0xFF;
    can->setCanSignal(
        canSignals::messagesTx.at(canSignals::messageName::BYTETESTTX),
        canSignals::messagesTx.at(canSignals::messageName::BYTETESTTX)
            .signals.at(canSignals::signalName::BYTETESTBIT0),
        value
    );
    
    // Read a CAN signal
    uint8_t received = can->getCanSignal(
        canSignals::messagesRx.at(canSignals::messageName::BYTETESTRX),
        canSignals::messagesRx.at(canSignals::messageName::BYTETESTRX)
            .signals.at(canSignals::signalName::BYTETESTBIT0)
    );
    
    return 0;
}
```

### CAN Signal Configuration

Signals are defined in `canSignal.h` with precise structures:

```cpp
struct canSignalMap_t {
    uint8_t  bitId;      // Bit position within byte
    uint8_t  byteId;     // Byte position within message
    uint16_t sizeInBit;  // Signal size in bits
};

struct canMessageMap_t {
    uint32_t messageId;  // CAN message ID
    uint8_t dlc;        // Data Length Code
    bool extended;      // Extended frame (29-bit ID)
    std::unordered_map<signalName, canSignalMap_t> signals;
};
```

## Test System

### Test Example (ByteTest)

The `test` and `ByteTest` classes are provided as **usage examples** of the system. They demonstrate how to:

- Create custom tests by inheriting from `test`
- Use the `testDispatcher` to execute tests in sequence
- Validate bidirectional CAN communications

```cpp
#include "testDispatcher.h"
#include "testByte.h"

int main() {
    testDispatcher* dispatcher = testDispatcher::getInstance();
    
    // Add tests to queue
    dispatcher->addTest<ByteTest>();
    
    // Execute all tests
    dispatcher->executeTestsInQueue();
    
    return 0;
}
```

### Creating Custom Tests

To create new tests, inherit from the `test` class:

```cpp
class MyTest : public test {
public:
    void executeTest() override {
        setTestName("My Test");
        setTestResult(testResult::RUNNING);
        
        // Test logic...
        
        setTestResult(testResult::PASS); // or FAIL
    }
};
```

## Logging

The system includes a thread-safe logger with automatic rotation:

```cpp
#include "logger.h"

// Basic logging
Utils::Logger::log(Utils::Logger::logLevel_t::INFO) << "Message" << std::endl;

// Logging with caller information
Utils::Logger::log(Utils::Logger::logLevel_t::WARNING, LOGINVOKER) 
    << "Warning!" << std::endl;
```

### Log Levels

- `INFO`: General information
- `DEBUG`: Debug information
- `WARNING`: Warnings
- `CRITICAL`: Critical errors

## Project Structure

```
CAN_validator/
├── serializzazione_segnali/
│   ├── canAdapter.h/cpp          # Base adapter class
│   ├── canSignal.h               # CAN signal definitions
│   └── adaptee/
│       └── pCanAdaptee.h/cpp     # PCAN implementation
├── utils/
│   ├── logger/                   # Logging system
│   └── generalUtils/             # General utilities
├── test/                         # Test framework (examples)
│   ├── test.h/cpp               # Base test class (EXAMPLE)
│   ├── testDispatcher.h/cpp     # Test dispatcher
│   └── canTest/tests/
│       └── testByte.h/cpp       # Example test (EXAMPLE)
├── Pcan/                        # PCAN libraries
└── CMakeLists.txt
```

## Notes

- **Thread Safety**: All CAN buffers are protected by mutexes
- **Error Handling**: Exceptions for connection errors and non-existent signals
- **Memory Management**: Smart pointers for automatic memory management
- **Portability**: Easily extensible for other CAN devices

## Provided Examples

The following classes are provided as **usage examples**:
- `test`: Base class for implementing custom tests
- `ByteTest`: Concrete example of CAN communication test
- `testDispatcher`: System for organizing and executing tests in sequence

These examples show best practices for using the system and can be used as templates for specific implementations.

## Contributing

Feel free to contribute! The code is designed to be extensible and modular.

## License

MIT License - See LICENSE file for details.

---

*Author: Vincenzo Ziccardi*  
*Email: vincenzoziccardi19@gmail.com*  
*Italy, 2025*
