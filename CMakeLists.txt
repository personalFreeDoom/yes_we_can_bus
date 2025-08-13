

cmake_minimum_required(VERSION 3.8)
project(CAN_validator)

# Imposta lo standard C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Impostazioni MSVC specifiche (debug info)
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,\
         $<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,\
         $<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Aggiungi l'eseguibile
add_executable(CAN_validator
  "CAN validator.cpp"
  
  "serializzazione_segnali/canAdapter.h"
  "serializzazione_segnali/canAdapter.cpp" 
  "serializzazione_segnali/canSignal.h"
  "serializzazione_segnali/adaptee/pCanAdaptee.cpp" 
  "serializzazione_segnali/adaptee/pCanAdaptee.h"
  "utils/generalUtils/generalUtils.h"
  "utils/generalUtils/generalUtils.cpp"
  "utils/logger/logger.h"
  "utils/logger/logger.cpp"
  "test/test.cpp" 
  "test/test.h" 
  "test/testDispatcher.h"
  "test/testDispatcher.cpp"
  "test/canTest/tests/testByte.h" 
  "test/canTest/tests/testByte.cpp"
)

# Includi la cartella dei file header esterni
target_include_directories(CAN_validator PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Pcan
  ${CMAKE_CURRENT_SOURCE_DIR}/serializzazione_segnali
  ${CMAKE_CURRENT_SOURCE_DIR}/serializzazione_segnali/adaptee
  ${CMAKE_CURRENT_SOURCE_DIR}/utils
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/logger
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/generalUtils
  ${CMAKE_CURRENT_SOURCE_DIR}/test/
  ${CMAKE_CURRENT_SOURCE_DIR}/test/canTest
  ${CMAKE_CURRENT_SOURCE_DIR}/test/canTest/tests
)

# Linka la libreria PCANBasic (solo se .lib è disponibile)
target_link_directories(CAN_validator PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Pcan
)
target_link_libraries(CAN_validator PRIVATE
  PCANBasic
)

# Copia la DLL accanto all'eseguibile (solo se esiste)
add_custom_command(TARGET CAN_validator POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/Pcan/PCANBasic.dll"
    "$<TARGET_FILE_DIR:CAN_validator>"
)
