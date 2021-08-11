#include "TransportNetwork.hpp"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

using NetworkMonitor::Id;
using NetworkMonitor::Line;
using NetworkMonitor::PassengerEvent;
using NetworkMonitor::Route;
using NetworkMonitor::Station;
using NetworkMonitor::TransportNetwork;

// Station — Public methods

bool Station::operator==(const Station& other) const
{
    return false;
}

// Route — Public methods

bool Route::operator==(const Route& other) const
{
    return false;
}

// Line — Public methods

bool Line::operator==(const Line& other) const
{
    return false;
}

// TransportNetwork — Public methods

TransportNetwork::TransportNetwork() = default;

TransportNetwork::~TransportNetwork() = default;

TransportNetwork::TransportNetwork(const TransportNetwork& copied) = default;

TransportNetwork::TransportNetwork(TransportNetwork&& moved) = default;

TransportNetwork& TransportNetwork::operator=(const TransportNetwork& copied) = default;

TransportNetwork& TransportNetwork::operator=(TransportNetwork&& moved) = default;

bool TransportNetwork::AddStation(const Station& station)
{
    return false;
}

bool TransportNetwork::AddLine(const Line& line)
{
    return false;
}

bool TransportNetwork::RecordPassengerEvent(const PassengerEvent& event)
{
    return false;
}

long long int TransportNetwork::GetPassengerCount(const Id& station) const
{
    return 0;
}

std::vector<Id> TransportNetwork::GetRoutesServingStation(const Id& station) const
{
    return std::vector<Id>{};
}

bool TransportNetwork::SetTravelTime(const Id& stationA, const Id& stationB, const unsigned int travelTime)
{
    return false;
}

unsigned int TransportNetwork::GetTravelTime(const Id& stationA, const Id& stationB) const
{
    return 0;
}

unsigned int TransportNetwork::GetTravelTime(const Id& line,
                                             const Id& route,
                                             const Id& stationA,
                                             const Id& stationB) const
{
    return 0;
}

// TransportNetwork — Private methods

std::vector<std::shared_ptr<TransportNetwork::GraphEdge>>::const_iterator TransportNetwork::GraphNode::FindEdgeForRoute(
    const std::shared_ptr<RouteInternal>& route) const
{
    return std::vector<std::shared_ptr<TransportNetwork::GraphEdge>>::const_iterator{};
}

std::shared_ptr<TransportNetwork::GraphNode> TransportNetwork::GetStation(const Id& stationId) const
{
    return nullptr;
}

std::shared_ptr<TransportNetwork::LineInternal> TransportNetwork::GetLine(const Id& lineId) const
{
    return nullptr;
}

std::shared_ptr<TransportNetwork::RouteInternal> TransportNetwork::GetRoute(const Id& lineId, const Id& routeId) const
{
    return nullptr;
}

bool TransportNetwork::AddRouteToLine(const Route& route, const std::shared_ptr<LineInternal>& lineInternal)
{
    return false;
}