#ifndef TASK_C
#define TASK_C

#include <vector>
#include <iostream>
#include "plots.h"
#include "paths.h"

/* 
* Contract:
*
* The parameters "coordinate_a" and "coordinate_b" 
* are assumed to be valid minecraft coordinates that are within world bounds.
*/
int manhattanDistance(mcpp::Coordinate coordinate_a, 
                        mcpp::Coordinate coordinate_b);

void place_waypoints(mcpp::MinecraftConnection &mc, 
                        mcpp::Coordinate coordinate) ;


/* 
* Contract:
*
* The parameter "&mc" is assumed to reference a 
* valid and active minecraft connection.
* The parameter "coordinate" is assumed to be a 
* valid minecraft coordinate within world bounds.
*/
void place_waypoints(mcpp::MinecraftConnection &mc, 
                        std::vector<mcpp::Coordinate> &waypoints);

/* 
* Contract:
*
* The parameter "&mc" is assumed to reference a 
* valid and active minecraft connection.
* The parameter "&waypoints" is assumed to be a vector 
* filled with valid minecraft coordinates.
*/
void connect_waypoints(mcpp::MinecraftConnection &mc, 
                        std::vector<mcpp::Coordinate> &waypoints);

/* 
* Contract:
*
* The parameter "&mc" is assumed to reference a valid 
* and active minecraft connection.
* The parameter "&plots" is assumed to be a vector filled 
* with valid minecraft coordinates. SWAPPED OUT FOR TESTING
*/
void connect_buildings(mcpp::MinecraftConnection &mc, 
                        std::vector<mcpp::Coordinate> &waypoints, 
                        std::vector<Plot> plots);

#endif
