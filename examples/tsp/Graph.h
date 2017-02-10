//
// Created by alberto on 27/08/16.
//

#ifndef RKBGA_GRAPH_H
#define RKBGA_GRAPH_H

#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>

namespace bga {
    namespace tsp {
        /**
         * This class models a simple graph, as a square distance matrix.
         */
        class Graph {
            /**
             * Distance matrix.
             */
            std::vector<std::vector<float>> distance;

        public:
            /**
             * Directly build the graph from a distance matrix.
             * @param distance  Square distance matrix.
             * @return          The corresponding Graph.
             */
            explicit Graph(std::vector<std::vector<float>> distance) : distance{distance} {
                // Check that the matrix is square.
                assert(std::all_of(distance.begin(), distance.end(), [&] (const auto& row) { return row.size() == distance.size(); }));
            }

            /**
             * Builds a graph from a TSPLIB instance file.
             * @param filename  The TSPLIB instance file name.
             * @return          The corresponding Graph.
             */
            explicit Graph(std::string filename);

            /**
             * Number of nodes in the graph.
             */
            uint32_t num_nodes() const { return distance.size(); }

            /**
             * Get the distance between two nodes.
             * @param i First node.
             * @param j Second node.
             */
            float get_distance(uint32_t i, uint32_t j) const;
        };
    }
}


#endif //RKBGA_GRAPH_H
