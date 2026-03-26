#ifndef PATHS_H
#define PATHS_H

#include <mcpp/mcpp.h>

struct Node {
	Node* next;
	Node* prev;
    Node* parent;
	mcpp::Coordinate data;
} ;

class Queue {
    public:
	    Node* front_of_queue = nullptr;
	    Node* back_of_queue = nullptr;

        /* 
        * Contract:
        *
        * The parameter "coordinate" is assumed to be a 
        * valid minecraft coordinate within world bounds.
        * The parameter "parent" is assumed to be a node 
        * created with the same structure 
        * as "struct Node" above.
        */
        void enqueue(mcpp::Coordinate coordinate, Node* parent) ;

	    Node* dequeue() ;

        ~Queue() ;

        /* 
        * Contract:
        *
        * The parameters "visited" and "queue_front" are 
        * assumed to be nodes created with the same structure 
        * as "struct Node" above.
        * The parameter "coordinate" is assumed to be a 
        * valid minecraft coordinate within world bounds.
        */
        bool already_visited(Node* visited, Node* queue_front, 
                                mcpp::Coordinate coordinate) ;
} ;

class Path {
    public:
        mcpp::Coordinate start ;
        mcpp::Coordinate end ;

        /* 
        * Contract:
        *
        * The parameter "&mc" is assumed to reference a valid 
        * and active minecraft connection.
        *
        * Explanation:
        * The Breadth First Search (BFS) algorithm:
        * For each determined waypoint, I created a node, 
        * and made that the first node in a linked list. 
        * I then explored the neighbouring x and z blocks (+x, -x, +z, -z), 
        * filtering through flattest and potential non-water terrain, 
        * before adding to the list. 
        * I repeated that process until one of the neighbouring 
        * coordinates was the goal. 
        * Once found, I backtracked through the parent links for each node, 
        * placing a block in the server. 
        * If it is water or air, I place stone blocks underneath, to create
        * a supporting bridge for path to build.
        */
        void breadth_first_search(mcpp::MinecraftConnection &mc) ;

        /*
        * Contract:
        * The parameters x and z are assumed to be valid integers
        * The parameters cacheX, cacheY, and cacheZ are assumed 
        * to be valid, RAW arrays
        * as I cannot use STL arrays as per assignment guidelines. 
        * They must also be the same length.
        * The parameter cacheCoutn is assumed to be a non-negative integer.
        *
        * The parameter "&mc" is assumed to reference a valid 
        * and active minecraft connection.
        * 
        * Explanation:
        *
        * This function will remember each height value for each coordinate.
        * Calling getHeight is slowing down my bfs an insane amount, 
        * so i'll call this first to save time
        *
        */
        int getSavedHeight(int x, int z, int xList[], int yList[]
                                    , int zList[], int &count
                                    , mcpp::MinecraftConnection &mc);
        } ;
#endif
