#include "paths.h"
#include <iostream>

void Queue::enqueue(mcpp::Coordinate coordinate, Node* parent) {
    Node* new_back_of_queue = new Node;
    new_back_of_queue->data = coordinate;
    new_back_of_queue->prev = back_of_queue;
    new_back_of_queue->next = nullptr;
    new_back_of_queue->parent = parent;

    if (back_of_queue != nullptr) {
        back_of_queue->next = new_back_of_queue;
    } 
    else {
        front_of_queue = new_back_of_queue;
    }

    back_of_queue = new_back_of_queue;
}

Node* Queue::dequeue() { 
    Node* parent = front_of_queue;
    front_of_queue = front_of_queue->next;

    if (front_of_queue != nullptr) {
        front_of_queue->prev = nullptr;
    }

    else {
        back_of_queue = nullptr;
    }
        
    return parent;
}

bool already_visited(Node* visited, 
                    Node* queue_front, 
                    mcpp::Coordinate coordinate) {
    Node* temp = visited;
    while (temp != nullptr) {
        if (temp->data.x == coordinate.x &&
            temp->data.y == coordinate.y &&
            temp->data.z == coordinate.z) {
            return true;
            }
        temp = temp->next;
    }
    return false;
}

Queue::~Queue() {
    Node* toDelete = front_of_queue;

    while (toDelete != nullptr) {
        Node* next = toDelete->next;
        delete toDelete;
        toDelete = next;
    }
}

int Path::getSavedHeight(int x, int z, int xList[], int yList[]
                                    , int zList[], int &count
                                    , mcpp::MinecraftConnection &mc) {
    int height = 0;
    bool found = false;

    for (int i = 0; i < count; i++) {
        if (xList[i] == x && zList[i] == z) {
            height = yList[i];
            found = true;
        }
    }

    if (found == false) {
        height = mc.getHeight(mcpp::Coordinate2D(x, z));
            if (count < 10000) {
            xList[count] = x;
            zList[count] = z;
            yList[count] = height;
            count++;
        }
    }

    return height;
}

void Path::breadth_first_search(mcpp::MinecraftConnection &mc) {
    //int stepCount = 0;
    Queue path;
    bool found = false;

    Node* start_node = new Node;
    start_node->data = start;
    start_node->parent = nullptr;
    start_node->next = nullptr;
    start_node->prev = nullptr;
        
    path.front_of_queue = start_node;
    path.back_of_queue = start_node;

    Node* end_node = nullptr;

    Node* visited_nodes = nullptr;

    int xList[10000];
    int yList[10000];
    int zList[10000];
    int count = 0;

    while (path.front_of_queue != nullptr && found == false) {
        Node* current = path.dequeue();
        mcpp::Coordinate current_coords = current->data;

        Node* visitedNode = new Node;
        visitedNode->data = current_coords;
        visitedNode->next = visited_nodes;
        visited_nodes = visitedNode;

        //stepCount++;
        
        if (current_coords.x == end.x && current_coords.z == end.z
                && std::abs(current_coords.y - end.y) <= 1) {
            std::cout << "Path is complete" << std::endl;
            found = true;
            end_node = current;
        }

        if (found == false) {
            mcpp::Coordinate north = {current_coords.x + 1, 
                                        current_coords.y, 
                                        current_coords.z};
            mcpp::Coordinate south = {current_coords.x - 1, 
                                        current_coords.y, 
                                        current_coords.z};
            mcpp::Coordinate east  = {current_coords.x, 
                                        current_coords.y, 
                                        current_coords.z + 1};
            mcpp::Coordinate west  = {current_coords.x, 
                                            current_coords.y, 
                                            current_coords.z - 1};
                if (already_visited(
                    visited_nodes, path.front_of_queue, north) == false) {
                    Node* newVisited = new Node;
                    newVisited->data = north;
                    newVisited->next = visited_nodes;
                    visited_nodes = newVisited;

                    north.y = getSavedHeight(north.x, north.z, xList, yList
                                                , zList, count
                                                , mc);

                    if (abs(north.y - current_coords.y) <= 1) {
                        path.enqueue(north, current);                
                    }
                }

                if (already_visited(
                    visited_nodes, path.front_of_queue, south) == false) {
                    Node* newVisited = new Node;
                    newVisited->data = south;
                    newVisited->next = visited_nodes;
                    visited_nodes = newVisited;

                    south.y = getSavedHeight(south.x, south.z, xList, yList
                                                , zList, count
                                                , mc);

                    if (abs(south.y - current_coords.y) <= 1) {
                        path.enqueue(south, current);                
                    }
                }
            
                if (already_visited(
                    visited_nodes, path.front_of_queue, east) == false) {
                    Node* newVisited = new Node;
                    newVisited->data = east;
                    newVisited->next = visited_nodes;
                    visited_nodes = newVisited;

                    east.y = getSavedHeight(east.x, east.z, xList, yList
                                                , zList, count
                                                , mc);

                    if (abs(east.y - current_coords.y) <= 1) {
                        path.enqueue(east, current);                
                    }
                }    

                if (already_visited(
                    visited_nodes, path.front_of_queue, west) == false) {
                    Node* newVisited = new Node;
                    newVisited->data = west;
                    newVisited->next = visited_nodes;
                    visited_nodes = newVisited;

                    west.y = getSavedHeight(west.x, west.z, xList, yList
                                                , zList, count
                                                , mc);

                    if (abs(west.y - current_coords.y) <= 1) {
                        path.enqueue(west, current);                
                    }
                } 
        }
    }
        
    if (found == false) {
        std::cout << "Path unable to form." << std::endl;
    }

    else if (found == true) {
        Node* currNode = end_node;

        while (currNode != nullptr) {
            int newY = currNode->data.y;
            mcpp::Coordinate toSet = {currNode->data.x, newY, 
                                        currNode->data.z};

            toSet.y--;

            mcpp::BlockType block = mc.getBlock(toSet);

            if (block == mcpp::Blocks::AIR
                || block == mcpp::Blocks::STILL_WATER 
                || block == mcpp::Blocks::FLOWING_WATER) {
                mc.setBlock(toSet, mcpp::Blocks::STONE);
            }  

            toSet.x--;

            mcpp::BlockType urmum = mc.getBlock(toSet);

            if (urmum == mcpp::Blocks::AIR
                || urmum == mcpp::Blocks::STILL_WATER 
                || urmum == mcpp::Blocks::FLOWING_WATER) {
                mc.setBlock(toSet, mcpp::Blocks::STONE);
            }  

            toSet.x++;

            toSet.z++;

            mcpp::BlockType next = mc.getBlock(toSet);

            if (next == mcpp::Blocks::AIR
                || next == mcpp::Blocks::STILL_WATER 
                || next == mcpp::Blocks::FLOWING_WATER) {
                mc.setBlock(toSet, mcpp::Blocks::STONE);
            }  

            toSet.z--;
            toSet.y++;

            mc.setBlock({currNode->data.x - 1, newY, currNode->data.z},
                            mcpp::Blocks::GRAVEL);
            mc.setBlock({currNode->data.x, newY, currNode->data.z},
                            mcpp::Blocks::GRAVEL);
            mc.setBlock({currNode->data.x, newY, currNode->data.z + 1},
                            mcpp::Blocks::GRAVEL);

            std::cout << "Placed block at (" 
                    << currNode->data.x << ", " 
                    << currNode->data.y << ", " 
                    << currNode->data.z << ")" << std::endl;
            currNode = currNode->parent;
        }
    }

    Node* temp = visited_nodes;

    while (temp != nullptr) {
        Node* next = temp->next;
        delete temp;
        temp = next;
    }

    visited_nodes = nullptr;
}
