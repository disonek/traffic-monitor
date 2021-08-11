#include <gtest/gtest.h>

#include <TransportNetwork.hpp>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

using NetworkMonitor::Id;
using NetworkMonitor::Line;
using NetworkMonitor::PassengerEvent;
using NetworkMonitor::Route;
using NetworkMonitor::Station;
using NetworkMonitor::TransportNetwork;

TEST(TransportNetworkTest, AddStation_basic)
{
    TransportNetwork nw{};
    bool ok{false};

    // Add a station.
    Station station{
        "station_000",
        "Station Name",
    };
    ok = nw.AddStation(station);
    EXPECT_TRUE(ok);
}

TEST(TransportNetworkTest, AddStation_duplicate_id)
{
    TransportNetwork nw{};
    bool ok{false};

    // Can't add the same station twice.
    Station station{
        "station_000",
        "Station Name",
    };
    ok = nw.AddStation(station);
    ASSERT_TRUE(ok);
    ok = nw.AddStation(station);
    EXPECT_TRUE(!ok);
}

TEST(TransportNetworkTest, AddStation_duplicate_name)
{
    TransportNetwork nw{};
    bool ok{false};

    // It's ok to add a station with the same name, but different ID.
    Station station0{
        "station_000",
        "Same Name",
    };
    ok = nw.AddStation(station0);
    ASSERT_TRUE(ok);
    Station station1{
        "station_001",
        "Same Name",
    };
    ok = nw.AddStation(station1);
    EXPECT_TRUE(ok);
}

TEST(TransportNetworkTest, AddLine_basic)
{
    TransportNetwork nw{};
    bool ok{false};

    // Add a line with 1 route.
    // route0: 0 ---> 1

    // First, add the stations.
    Station station0{
        "station_000",
        "Station Name 0",
    };
    Station station1{
        "station_001",
        "Station Name 1",
    };
    ok = true;
    ok &= nw.AddStation(station0);
    ok &= nw.AddStation(station1);
    ASSERT_TRUE(ok);

    // Then, add the line, with the two routes.
    Route route0{
        "route_000",
        "inbound",
        "line_000",
        "station_000",
        "station_001",
        {"station_000", "station_001"},
    };
    Line line{
        "line_000",
        "Line Name",
        {route0},
    };
    ok = nw.AddLine(line);
    EXPECT_TRUE(ok);
}

TEST(TransportNetworkTest, DISABLED_AddLine_shared_stations)
{
    TransportNetwork nw{};
    bool ok{false};

    // Define a line with 2 routes going through some shared stations.
    // route0: 0 ---> 1 ---> 2
    // route1: 3 ---> 1 ---> 2
    Station station0{
        "station_000",
        "Station Name 0",
    };
    Station station1{
        "station_001",
        "Station Name 1",
    };
    Station station2{
        "station_002",
        "Station Name 2",
    };
    Station station3{
        "station_003",
        "Station Name 3",
    };
    Route route0{
        "route_000",
        "inbound",
        "line_000",
        "station_000",
        "station_002",
        {"station_000", "station_001", "station_002"},
    };
    Route route1{
        "route_001",
        "inbound",
        "line_000",
        "station_003",
        "station_002",
        {"station_003", "station_001", "station_002"},
    };
    Line line{
        "line_000",
        "Line Name",
        {route0, route1},
    };
    ok = true;
    ok &= nw.AddStation(station0);
    ok &= nw.AddStation(station1);
    ok &= nw.AddStation(station2);
    ok &= nw.AddStation(station3);
    ASSERT_TRUE(ok);
    ok = nw.AddLine(line);
    EXPECT_TRUE(ok);
}

TEST(TransportNetworkTest, DISABLED_AddLine_missing_stations)
{
    TransportNetwork nw{};
    bool ok{false};

    // Define a line with 1 route.
    // route0: 0 ---> 1 ---> 2
    Station station0{
        "station_000",
        "Station Name 0",
    };
    Station station1{
        "station_001",
        "Station Name 1",
    };
    Station station2{
        "station_002",
        "Station Name 2",
    };
    Route route0{
        "route_000",
        "inbound",
        "line_000",
        "station_000",
        "station_002",
        {"station_000", "station_001", "station_002"},
    };
    Line line{
        "line_000",
        "Line Name",
        {route0},
    };

    // Expected fail: No stations in the network.
    ok = nw.AddLine(line);
    EXPECT_TRUE(!ok);

    // Expected fail: We add all stations except one.
    ok = true;
    ok &= nw.AddStation(station0);
    ok &= nw.AddStation(station1);
    ASSERT_TRUE(ok);
    ok = nw.AddLine(line);
    EXPECT_TRUE(!ok);

    // Expected success: We add the final station and try again.
    ok = nw.AddStation(station2);
    ASSERT_TRUE(ok);
    ok = nw.AddLine(line);
    EXPECT_TRUE(ok);
}

TEST(TransportNetworkTest, DISABLED_AddLine_duplicate)
{
    TransportNetwork nw{};
    bool ok{false};

    // Can't add the same line twice.
    Station station0{
        "station_000",
        "Station Name 0",
    };
    Station station1{
        "station_001",
        "Station Name 1",
    };
    Route route0{
        "route_000",
        "inbound",
        "line_000",
        "station_000",
        "station_001",
        {"station_000", "station_001"},
    };
    Line line{
        "line_000",
        "Line Name",
        {route0},
    };
    ok = true;
    ok &= nw.AddStation(station0);
    ok &= nw.AddStation(station1);
    ASSERT_TRUE(ok);
    ok = nw.AddLine(line);
    ASSERT_TRUE(ok);
    ok = nw.AddLine(line);
    EXPECT_TRUE(!ok);
}

TEST(TransportNetworkTest, DISABLED_PassengerEvents_basic)
{
    TransportNetwork nw{};
    bool ok{false};

    // Add a line with 1 route.
    // route0: 0 ---> 1 ---> 2
    Station station0{
        "station_000",
        "Station Name 0",
    };
    Station station1{
        "station_001",
        "Station Name 1",
    };
    Station station2{
        "station_002",
        "Station Name 2",
    };
    Route route0{
        "route_000",
        "inbound",
        "line_000",
        "station_000",
        "station_002",
        {"station_000", "station_001", "station_002"},
    };
    Line line{
        "line_000",
        "Line Name",
        {route0},
    };
    ok = true;
    ok &= nw.AddStation(station0);
    ok &= nw.AddStation(station1);
    ok &= nw.AddStation(station2);
    ASSERT_TRUE(ok);
    ok = nw.AddLine(line);
    ASSERT_TRUE(ok);

    // Check that the network starts empty.
    EXPECT_EQ(nw.GetPassengerCount(station0.id), 0);
    EXPECT_EQ(nw.GetPassengerCount(station1.id), 0);
    EXPECT_EQ(nw.GetPassengerCount(station2.id), 0);
    try
    {
        auto count{nw.GetPassengerCount("station_42")}; // Not in the network
        ASSERT_TRUE(false);
    }
    catch(const std::runtime_error& e)
    {
        ASSERT_TRUE(true);
    }

    // Record events and check the count.
    using EventType = PassengerEvent::Type;
    ok = nw.RecordPassengerEvent({station0.id, EventType::In});
    ASSERT_TRUE(ok);
    EXPECT_EQ(nw.GetPassengerCount(station0.id), 1);
    EXPECT_EQ(nw.GetPassengerCount(station1.id), 0);
    EXPECT_EQ(nw.GetPassengerCount(station2.id), 0);
    ok = nw.RecordPassengerEvent({station0.id, EventType::In});
    ASSERT_TRUE(ok);
    EXPECT_EQ(nw.GetPassengerCount(station0.id), 2);
    ok = nw.RecordPassengerEvent({station1.id, EventType::In});
    ASSERT_TRUE(ok);
    EXPECT_EQ(nw.GetPassengerCount(station0.id), 2);
    EXPECT_EQ(nw.GetPassengerCount(station1.id), 1);
    EXPECT_EQ(nw.GetPassengerCount(station2.id), 0);
    ok = nw.RecordPassengerEvent({station0.id, EventType::Out});
    ASSERT_TRUE(ok);
    EXPECT_EQ(nw.GetPassengerCount(station0.id), 1);
    ok = nw.RecordPassengerEvent({station2.id, EventType::Out}); // Negative
    ASSERT_TRUE(ok);
    EXPECT_EQ(nw.GetPassengerCount(station2.id), -1);
}

TEST(TransportNetworkTest, DISABLED_GetRoutesServingStation_basic)
{
    TransportNetwork nw{};
    bool ok{false};

    // Add a line with 1 route.
    // route0: 0 ---> 1 ---> 2
    // Plus a station served by no routes: 3.
    Station station0{
        "station_000",
        "Station Name 0",
    };
    Station station1{
        "station_001",
        "Station Name 1",
    };
    Station station2{
        "station_002",
        "Station Name 2",
    };
    Station station3{
        "station_003",
        "Station Name 3",
    };
    Route route0{
        "route_000",
        "inbound",
        "line_000",
        "station_000",
        "station_002",
        {"station_000", "station_001", "station_002"},
    };
    Line line{
        "line_000",
        "Line Name",
        {route0},
    };
    ok = true;
    ok &= nw.AddStation(station0);
    ok &= nw.AddStation(station1);
    ok &= nw.AddStation(station2);
    ok &= nw.AddStation(station3);
    ASSERT_TRUE(ok);
    ok = nw.AddLine(line);
    ASSERT_TRUE(ok);

    // Check the routes served.
    std::vector<Id> routes{};
    routes = nw.GetRoutesServingStation(station0.id);
    EXPECT_EQ(routes.size(), 1);
    EXPECT_TRUE(routes[0] == route0.id);
    routes = nw.GetRoutesServingStation(station1.id);
    EXPECT_EQ(routes.size(), 1);
    EXPECT_TRUE(routes[0] == route0.id);
    routes = nw.GetRoutesServingStation(station2.id);
    EXPECT_EQ(routes.size(), 1);
    EXPECT_TRUE(routes[0] == route0.id);
    routes = nw.GetRoutesServingStation(station3.id);
    EXPECT_EQ(routes.size(), 0);
}

TEST(TransportNetworkTest, DISABLED_GetRoutesServingStation_lone_station)
{
    TransportNetwork nw{};
    bool ok{false};

    // Add a single station.
    Station station0{
        "station_000",
        "Station Name 0",
    };
    ok = nw.AddStation(station0);
    ASSERT_TRUE(ok);

    // Check the routes served.
    auto routes{nw.GetRoutesServingStation(station0.id)};
    EXPECT_EQ(routes.size(), 0);
}

TEST(TransportNetworkTest, DISABLED_TravelTime_basic)
{
    TransportNetwork nw{};
    bool ok{false};

    // Add a line with 1 route.
    // route0: 0 ---> 1 ---> 2
    Station station0{
        "station_000",
        "Station Name 0",
    };
    Station station1{
        "station_001",
        "Station Name 1",
    };
    Station station2{
        "station_002",
        "Station Name 2",
    };
    Route route0{
        "route_000",
        "inbound",
        "line_000",
        "station_000",
        "station_002",
        {"station_000", "station_001", "station_002"},
    };
    Line line{
        "line_000",
        "Line Name",
        {route0},
    };
    ok = true;
    ok &= nw.AddStation(station0);
    ok &= nw.AddStation(station1);
    ok &= nw.AddStation(station2);
    ASSERT_TRUE(ok);
    ok = nw.AddLine(line);
    ASSERT_TRUE(ok);

    unsigned int travelTime{0};

    // Get travel time before setting it.
    travelTime = nw.GetTravelTime(station0.id, station1.id);
    EXPECT_EQ(travelTime, 0);

    // Cannot set the travel time between non-adjacent stations.
    ok = nw.SetTravelTime(station0.id, station2.id, 1);
    EXPECT_TRUE(!ok);

    // Set the travel time between adjacent stations.
    ok = nw.SetTravelTime(station0.id, station1.id, 2);
    EXPECT_TRUE(ok);
    EXPECT_EQ(nw.GetTravelTime(station0.id, station1.id), 2);

    // Set travel time between adjacend stations, even if they appear in the
    // reverse order in the route.
    ok = nw.SetTravelTime(station1.id, station0.id, 3);
    EXPECT_TRUE(ok);
    EXPECT_EQ(nw.GetTravelTime(station1.id, station0.id), 3);
}

TEST(TransportNetworkTest, DISABLED_TravelTime_over_route)
{
    TransportNetwork nw{};
    bool ok{false};

    // Add a line with 3 routes.
    // route0: 0 ---> 1 ---> 2 ---> 3
    // route1: 3 ---> 1 ---> 2
    // route2: 3 ---> 1 ---> 0
    Station station0{
        "station_000",
        "Station Name 0",
    };
    Station station1{
        "station_001",
        "Station Name 1",
    };
    Station station2{
        "station_002",
        "Station Name 2",
    };
    Station station3{
        "station_003",
        "Station Name 3",
    };
    Route route0{
        "route_000",
        "inbound",
        "line_000",
        "station_000",
        "station_003",
        {"station_000", "station_001", "station_002", "station_003"},
    };
    Route route1{
        "route_001",
        "inbound",
        "line_000",
        "station_003",
        "station_002",
        {"station_003", "station_001", "station_002"},
    };
    Route route2{
        "route_002",
        "inbound",
        "line_000",
        "station_003",
        "station_000",
        {"station_003", "station_001", "station_000"},
    };
    Line line{
        "line_000",
        "Line Name",
        {route0, route1, route2},
    };
    ok = true;
    ok &= nw.AddStation(station0);
    ok &= nw.AddStation(station1);
    ok &= nw.AddStation(station2);
    ok &= nw.AddStation(station3);
    ASSERT_TRUE(ok);
    ok = nw.AddLine(line);
    ASSERT_TRUE(ok);

    // Set all travel times.
    ok = true;
    ok &= nw.SetTravelTime(station0.id, station1.id, 1);
    ok &= nw.SetTravelTime(station1.id, station2.id, 2);
    ok &= nw.SetTravelTime(station2.id, station3.id, 3);
    ok &= nw.SetTravelTime(station3.id, station1.id, 4);
    ASSERT_TRUE(ok);

    // Check the cumulative travel times.
    unsigned int travelTime{0};
    // route0
    EXPECT_EQ(nw.GetTravelTime(line.id, route0.id, station0.id, station1.id), 1);
    EXPECT_EQ(nw.GetTravelTime(line.id, route0.id, station0.id, station2.id), 1 + 2);
    EXPECT_EQ(nw.GetTravelTime(line.id, route0.id, station0.id, station3.id), 1 + 2 + 3);
    EXPECT_EQ(nw.GetTravelTime(line.id, route0.id, station1.id, station3.id), 2 + 3);
    // route1
    EXPECT_EQ(nw.GetTravelTime(line.id, route1.id, station3.id, station1.id), 4);
    EXPECT_EQ(nw.GetTravelTime(line.id, route1.id, station3.id, station2.id), 4 + 2);
    // route2
    EXPECT_EQ(nw.GetTravelTime(line.id, route2.id, station3.id, station1.id), 4);
    EXPECT_EQ(nw.GetTravelTime(line.id, route2.id, station3.id, station0.id), 4 + 1);
    // Invalid routes
    // -- 3 -> 1 is possible, but only over route1 and route2.
    EXPECT_EQ(nw.GetTravelTime(line.id, route0.id, station3.id, station1.id), 0);
    // -- 1 -> 0 is possible, but only over route3.
    EXPECT_EQ(nw.GetTravelTime(line.id, route0.id, station1.id, station0.id), 0);
    EXPECT_EQ(nw.GetTravelTime(line.id, route0.id, station1.id, station1.id), 0);
}
