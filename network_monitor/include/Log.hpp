#pragma once

#include <boost/system/error_code.hpp>
#include <iomanip>
#include <iostream>
#include <string>

namespace NetworkMonitor {

void Log(const std::string& where, boost::system::error_code ec)
{
    std::cerr << "[" << std::setw(20) << where << "] " << (ec ? "Error: " : "OK") << (ec ? ec.message() : "")
              << std::endl;
}

} // namespace NetworkMonitor