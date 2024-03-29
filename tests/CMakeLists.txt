add_executable(network_monitor_test
        WebSocketClientTest.cpp
        FileDownloaderTest.cpp
        TransportNetworkTest.cpp
)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
target_link_libraries(
  network_monitor_test
  PRIVATE GTest::gtest_main
          GTest::gmock_main
          GTest::gmock
          GTest::gtest
          Threads::Threads
          network_monitor
          )
          
target_compile_features(network_monitor_test PRIVATE cxx_std_17)
target_compile_definitions(network_monitor_test
    PRIVATE
        TESTS_CACERT_PEM="${CMAKE_CURRENT_SOURCE_DIR}/cacert.pem"
        TESTS_NETWORK_LAYOUT_JSON="${CMAKE_CURRENT_SOURCE_DIR}/network-layout.json"
        # BOOST_ASIO_ENABLE_HANDLER_TRACKING=1
)

add_test(NAME network_monitor_test COMMAND network_monitor_test)

add_custom_command(
  TARGET network_monitor_test
  POST_BUILD
  COMMAND ctest -C Debug -T test --output-on-failure)