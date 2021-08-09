#pragma once

#include <filesystem>
#include <string>

namespace NetworkMonitor {

bool DownloadFile(const std::string& fileUrl,
                  const std::filesystem::path& destination,
                  const std::filesystem::path& caCertFile = {});

} // namespace NetworkMonitor