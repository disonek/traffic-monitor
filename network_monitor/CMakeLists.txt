
add_library(network_monitor STATIC
    src/WebSocketClient.cpp
    src/FileDownloader.cpp
)
    
target_compile_features(network_monitor
    PRIVATE
        cxx_std_17
)

target_compile_definitions(network_monitor
    PRIVATE
        $<$<PLATFORM_ID:Windows>:_WIN32_WINNT=${WINDOWS_VERSION}>
)

target_compile_options(network_monitor PUBLIC $<$<PLATFORM_ID:Windows>:/bigobj>)

find_package(Boost 1.74 REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

target_link_libraries(network_monitor
    PUBLIC
        OpenSSL::OpenSSL
        Boost::Boost
    PRIVATE
        CURL::CURL
)

target_include_directories(network_monitor 
    PUBLIC include
)