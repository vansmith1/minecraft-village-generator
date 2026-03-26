#include "task_c.h"
#include "paths.h"
#include "plots.h"
#include "task_a.h"
#include <mcpp/mcpp.h>

int manhattanDistance(mcpp::Coordinate coordinate_a,
                        mcpp::Coordinate coordinate_b) {
    int dist = abs(coordinate_a.x - coordinate_b.x) 
                + abs(coordinate_a.y - coordinate_b.y) 
                + abs(coordinate_a.z - coordinate_b.z);

    // TEST 1: MANHATTAN DISTANCE CALCULATION
    /* std::cout << "Start Coords: (" << coordinate_a.x 
                    << ", " << coordinate_a.y 
                << ", " << coordinate_a.z << ")" << std::endl;
    std::cout << "End Coords: (" << coordinate_b.x 
                << ", " << coordinate_b.y 
                << ", " << coordinate_b.z << ")" 
                << std::endl;                      
    std::cout << "Manhattan distance: " << dist << std::endl; */

    return dist;
}

/*TODO*/
void place_waypoints(mcpp::MinecraftConnection &mc, 
                        std::vector<mcpp::Coordinate> &waypoints) {
    if (waypoints.size() > 0) {
        for (size_t i = 0; i < waypoints.size(); ++i) {
            mcpp::Coordinate waypoint = waypoints[i]; 
            waypoint.y++;
            mc.setBlock(waypoint, mcpp::Blocks::STONE_BRICKS);
            waypoint.y++;
            mc.setBlock(waypoint, mcpp::Blocks::OAK_FENCE);
            waypoint.y++;
            mc.setBlock(waypoint, mcpp::Blocks::OAK_FENCE);
            waypoint.y++;
            mc.setBlock(waypoint, mcpp::Blocks::GLOWSTONE);
            waypoint.x--;
            mc.setBlock(waypoint, mcpp::Blocks::OAK_FENCE);
            waypoint.x++;
            waypoint.x++;
            mc.setBlock(waypoint, mcpp::Blocks::OAK_FENCE);
            waypoint.x--;
            waypoint.z--;
            mc.setBlock(waypoint, mcpp::Blocks::OAK_FENCE);
            waypoint.z++;
            waypoint.z++;
            mc.setBlock(waypoint, mcpp::Blocks::OAK_FENCE);
            
            std::cout << "Built waypoint " << i + 1 << "." << std::endl;
        }
    }
    else {
        std::cout << "No waypoint to build." << std::endl;
    }
}

void connect_waypoints(mcpp::MinecraftConnection &mc, 
                        std::vector<mcpp::Coordinate> &waypoints) {
    std::vector<mcpp::Coordinate> unconnectedWaypoints;
    std::vector<mcpp::Coordinate> connectedWaypoints;

    for (size_t i = 0; i < waypoints.size(); ++i) {
        unconnectedWaypoints.push_back(waypoints[i]);
    }

    if (unconnectedWaypoints.size() < 2) {
        std::cout << "Not enough waypoints to connect!" << std::endl;
    }
    
    else { 
        connectedWaypoints.push_back(unconnectedWaypoints[0]);
        unconnectedWaypoints.erase(unconnectedWaypoints.begin());

        while (unconnectedWaypoints.size() > 0) {
            int rembJ = 0;
            int smallestDistance = manhattanDistance(
                            connectedWaypoints[0], unconnectedWaypoints[0]);

            for (size_t i = 0; i < connectedWaypoints.size(); ++i) {
                for (size_t j = 0; j < unconnectedWaypoints.size(); ++j) {
                    int distance = manhattanDistance(
                            connectedWaypoints[i], unconnectedWaypoints[j]);
                    if (distance < smallestDistance) {
                        smallestDistance = distance;
                        rembJ = j;
                    }
                }
            }

            connectedWaypoints.push_back(unconnectedWaypoints[rembJ]);
            unconnectedWaypoints.erase(unconnectedWaypoints.begin() + rembJ);
        }

        for (size_t i = 0; i < connectedWaypoints.size() - 1; ++i) {
            std::cout << "Builds path from " << connectedWaypoints[i] 
                        << " to " << connectedWaypoints[i + 1] << std::endl;
            Path pathway;
            pathway.start = connectedWaypoints[i];
            pathway.end = connectedWaypoints[i + 1];

            pathway.breadth_first_search(mc);
        } 
    }
}

void connect_buildings(mcpp::MinecraftConnection &mc, 
                        std::vector<mcpp::Coordinate> &waypoints, 
                         std::vector<Plot> plots) {

    if (plots.empty()) {
        std::cout << "Not enough houses to connect!" << std::endl;
    }

    else {
        if (waypoints.empty()) {
            std::cout << "No waypoints found, cannot connect houses." << std::endl;
            return;
        }

        else {
            for (size_t i = 0; i < plots.size(); ++i) {
                int smallestDistance = manhattanDistance(waypoints[0], 
                                        plots[i].entrance);
                int rembJ = 0;

                for (size_t j = 0; j < waypoints.size(); ++j) {
                    int distance = manhattanDistance(waypoints[j], 
                                                        plots[i].entrance);
                    if (distance < smallestDistance) {
                        smallestDistance = distance;
                        rembJ = j;
                    }
                }
            std::cout << "Building path from house " << i + 1 
                        << " entrance (" << plots[i].entrance.x << ", " 
                        << plots[i].entrance.y << ", " << plots[i].entrance.z 
                        << ") to waypoint (" << waypoints[rembJ].x << ", " 
                        << waypoints[rembJ].y << ", " << waypoints[rembJ].z 
                        << ")" << std::endl;

            Path pathway;
            pathway.start = plots[i].entrance;
            pathway.end = waypoints[rembJ];
            pathway.breadth_first_search(mc);  
            }
        }  
    }     
}
