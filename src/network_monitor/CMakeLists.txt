add_library(network_monitor)

target_include_directories(network_monitor PUBLIC .)

target_sources(network_monitor PRIVATE 
    WebSocketClient.hpp
    WebSocketClient.cpp)
    
target_compile_features(network_monitor
    PRIVATE
        cxx_std_17
)

target_compile_definitions(network_monitor
    PRIVATE
        $<$<PLATFORM_ID:Windows>:_WIN32_WINNT=${WINDOWS_VERSION}>
)

find_package(Boost 1.74 REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
target_link_libraries(network_monitor
    PUBLIC
        Boost::Boost
        OpenSSL::OpenSSL
)