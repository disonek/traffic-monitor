#include <gtest/gtest.h>

#include <FileDownloader.hpp>
#include <filesystem>
#include <fstream>
#include <string>

TEST(FileDownloader, basic)
{
    const std::string fileUrl{"https://ltnm.learncppthroughprojects.com/network-layout.json"};
    const auto destination{std::filesystem::temp_directory_path() / "network-layout.json"};

    // Download the file.
    bool downloaded{NetworkMonitor::DownloadFile(fileUrl, destination, TESTS_CACERT_PEM)};
    EXPECT_TRUE(downloaded);
    EXPECT_TRUE(std::filesystem::exists(destination));

    // Check the content of the file.
    // We cannot check the whole file content as it changes over time, but we
    // can at least check some expected file properties.
    {
        const std::string expectedString{"\"stations\": ["};
        std::ifstream file{destination};
        std::string line{};
        bool foundExpectedString{false};
        while(std::getline(file, line))
        {
            if(line.find(expectedString) != std::string::npos)
            {
                foundExpectedString = true;
                break;
            }
        }
        EXPECT_TRUE(foundExpectedString);
    }

    // Clean up.
    std::filesystem::remove(destination);
}

TEST(FileDownloader, downloadAndParseFile)
{
    const std::string fileUrl{"https://ltnm.learncppthroughprojects.com/network-layout.json"};
    const auto destination{TESTS_NETWORK_LAYOUT_JSON};

    bool downloaded{NetworkMonitor::DownloadFile(fileUrl, destination, TESTS_CACERT_PEM)};
    EXPECT_TRUE(downloaded);
    EXPECT_TRUE(std::filesystem::exists(destination));

    auto parsed = NetworkMonitor::ParseJsonFile(destination);

    EXPECT_TRUE(parsed.is_object());
    EXPECT_TRUE(parsed.contains("lines"));
    EXPECT_TRUE(parsed.at("lines").size() > 0);
    EXPECT_TRUE(parsed.contains("stations"));
    EXPECT_TRUE(parsed.at("stations").size() > 0);
    EXPECT_TRUE(parsed.contains("travel_times"));
    EXPECT_TRUE(parsed.at("travel_times").size() > 0);

    std::filesystem::remove(destination);
}
