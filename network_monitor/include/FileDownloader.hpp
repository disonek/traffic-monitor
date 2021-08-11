#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

namespace NetworkMonitor {

bool DownloadFile(const std::string& fileUrl,
                  const std::filesystem::path& destination,
                  const std::filesystem::path& caCertFile = {});

nlohmann::json ParseJsonFile(const std::filesystem::path& source);
} // namespace NetworkMonitor