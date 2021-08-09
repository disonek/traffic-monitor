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