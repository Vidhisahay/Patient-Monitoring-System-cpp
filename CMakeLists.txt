cmake_minimum_required(VERSION 3.16)
project(PatientMonitoringSystemCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_SANITIZERS "Enable address/undefined sanitizers" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)

if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES clang-tidy)
  if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-warnings-as-errors=*")
  else()
    message(FATAL_ERROR "ENABLE_CLANG_TIDY is ON but clang-tidy was not found")
  endif()
endif()

if(ENABLE_SANITIZERS)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

add_library(monitoring_core
  src/alert.cpp
  src/logger.cpp
  src/patient_state.cpp
  src/sensor.cpp
)
target_include_directories(monitoring_core PUBLIC include)

add_executable(monitor src/main.cpp)
target_link_libraries(monitor PRIVATE monitoring_core)

enable_testing()

add_executable(test_alert tests/test_alert.cpp)
target_link_libraries(test_alert PRIVATE monitoring_core)
add_test(NAME alert_engine_behavior COMMAND test_alert)

add_executable(test_threaded_sensors tests/test_threaded_sensors.cpp)
target_link_libraries(test_threaded_sensors PRIVATE monitoring_core)
add_test(NAME threaded_sensor_updates COMMAND test_threaded_sensors)
