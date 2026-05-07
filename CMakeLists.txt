cmake_minimum_required(VERSION 3.10)
project(LFM_Radar_Processing VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread -march=native -Wall -Wextra")

# 包含目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 查找必要的包
find_package(Threads REQUIRED)
find_package(PkgConfig QUIET)

# 查找FFTW3
if(PkgConfig_FOUND)
    pkg_check_modules(FFTW3 QUIET fftw3f)
    if(NOT FFTW3_FOUND)
        find_path(FFTW3_INCLUDE_DIRS fftw3.h)
        find_library(FFTW3_LIBRARIES fftw3f)
    endif()
else()
    find_path(FFTW3_INCLUDE_DIRS fftw3.h)
    find_library(FFTW3_LIBRARIES fftw3f)
endif()

if(NOT FFTW3_LIBRARIES)
    message(FATAL_ERROR "FFTW3 not found. Please install libfftw3-dev")
endif()

include_directories(${FFTW3_INCLUDE_DIRS})

# 源文件列表
set(CORE_SOURCES
    src/signal_generator.cpp
    src/pulse_compression.cpp
    src/mtd_processor.cpp
    src/cacfar_detector.cpp
    src/thread_manager.cpp
    src/data_buffer.cpp
    src/config.cpp
)

# 主程序
add_executable(lfm_radar_processing 
    src/main.cpp
    ${CORE_SOURCES}
)

target_link_libraries(lfm_radar_processing
    PRIVATE Threads::Threads
    PRIVATE ${FFTW3_LIBRARIES}
    PRIVATE m
)

# 单元测试 - 信号生成测试
add_executable(test_signal 
    test/test_signal.cpp
    src/signal_generator.cpp
    src/data_buffer.cpp
    src/config.cpp
)
target_link_libraries(test_signal PRIVATE Threads::Threads)

# 单元测试 - 脉冲压缩测试
add_executable(test_compression 
    test/test_compression.cpp
    src/signal_generator.cpp
    src/pulse_compression.cpp
    src/data_buffer.cpp
    src/config.cpp
)
target_link_libraries(test_compression 
    PRIVATE Threads::Threads 
    PRIVATE ${FFTW3_LIBRARIES}
    PRIVATE m
)

# 单元测试 - MTD处理测试
add_executable(test_mtd 
    test/test_mtd.cpp
    src/mtd_processor.cpp
    src/signal_generator.cpp
    src/data_buffer.cpp
    src/config.cpp
)
target_link_libraries(test_mtd 
    PRIVATE Threads::Threads
    PRIVATE m
)

# 单元测试 - CA-CFAR检测器测试
add_executable(test_cacfar 
    test/test_cacfar.cpp
    src/cacfar_detector.cpp
    src/data_buffer.cpp
    src/config.cpp
)
target_link_libraries(test_cacfar 
    PRIVATE Threads::Threads
    PRIVATE m
)

# 集成测试 - 完整信号处理链
add_executable(integration_test 
    test/integration_test.cpp
    ${CORE_SOURCES}
)

target_link_libraries(integration_test
    PRIVATE Threads::Threads
    PRIVATE ${FFTW3_LIBRARIES}
    PRIVATE m
)

# 长时间运行测试
add_executable(longevity_test 
    test/longevity_test.cpp
    ${CORE_SOURCES}
)

target_link_libraries(longevity_test
    PRIVATE Threads::Threads
    PRIVATE ${FFTW3_LIBRARIES}
    PRIVATE m
)

# 安装
install(TARGETS lfm_radar_processing DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

# 输出配置信息
message(STATUS "=== LFM Radar Processing Build Configuration ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "FFTW3 include: ${FFTW3_INCLUDE_DIRS}")
message(STATUS "FFTW3 library: ${FFTW3_LIBRARIES}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "=== End Configuration ===")
