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

bool Station::operator==(const Station& other) const
{
    return id == other.id;
}

bool Route::operator==(const Route& other) const
{
    return id == other.id;
}

bool Line::operator==(const Line& other) const
{
    return id == other.id;
}

bool TransportNetwork::AddStation(const Station& station)
{
    if(GetStation(station.id) != nullptr)
    {
        return false;
    }

    stations_.emplace(station.id, std::make_shared<GraphNode>(GraphNode{station.id, station.name, 0, {}}));
    return true;
}

bool TransportNetwork::AddLine(const Line& line)
{
    if(GetLine(line.id) != nullptr)
    {
        return false;
    }

    auto lineInternal{std::make_shared<LineInternal>(LineInternal{line.id, line.name, {}})};
    for(const auto& route : line.routes)
    {
        bool ok{AddRouteToLine(route, lineInternal)};
        if(!ok)
        {
            return false;
        }
    }
    lines_.emplace(line.id, std::move(lineInternal));

    return true;
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
    auto stationIt = stations_.find(stationId);
    if(stationIt == end(stations_))
    {
        return nullptr;
    }

    return stationIt->second;
}

std::shared_ptr<TransportNetwork::LineInternal> TransportNetwork::GetLine(const Id& lineId) const
{
    auto lineIt = lines_.find(lineId);
    if(lineIt == end(lines_))
    {
        return nullptr;
    }
    return lineIt->second;
}

std::shared_ptr<TransportNetwork::RouteInternal> TransportNetwork::GetRoute(const Id& lineId, const Id& routeId) const
{
    return nullptr;
}

bool TransportNetwork::AddRouteToLine(const Route& route, const std::shared_ptr<LineInternal>& lineInternal)
{
    if(lineInternal->routes.find(route.id) != end(lineInternal->routes))
    {
        return false;
    }

    std::vector<std::shared_ptr<GraphNode>> stops{};
    stops.reserve(route.stops.size());

    for(const auto& stopsId : route.stops)
    {
        const auto station{GetStation(stopsId)};
        if(station == nullptr)
        {
            return false;
        }
        stops.push_back(station);
    }

    auto routeInternal{std::make_shared<RouteInternal>(RouteInternal{route.id, lineInternal, std::move(stops)})};

    for(size_t idx{0}; idx < routeInternal->stops.size() - 1; ++idx)
    {
        const auto& thisStop{routeInternal->stops[idx]};
        const auto& nextStop{routeInternal->stops[idx + 1]};

        thisStop->edges.emplace_back(std::make_shared<GraphEdge>(GraphEdge{routeInternal, nextStop, 0}));
    }

    lineInternal->routes[route.id] = std::move(routeInternal);
    return true;
}