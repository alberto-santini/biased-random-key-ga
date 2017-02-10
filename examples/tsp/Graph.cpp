//
// Created by alberto on 27/08/16.
//

#include <iostream>
#include <fstream>
#include "Graph.h"

namespace bga {
    namespace tsp {
        float Graph::get_distance(uint32_t i, uint32_t j) const {
            assert(i < distance.size());
            assert(j < distance[i].size());
            return distance[i][j];
        }

        Graph::Graph(std::string filename) {
            auto is = std::ifstream(filename);

            if(is.fail()) {
                std::cerr << "Could not open file " << filename << std::endl;
                _Exit(1);
            }

            auto num_nodes = 0u;

            // The first number is the number of nodes.
            if(!(is >> num_nodes)) {
                std::cerr << "Could not read the number of nodes from" << filename <<  std::endl;
                _Exit(1);
            }

            if(num_nodes == 0u) {
                std::cerr << "Graph with 0 nodes? There is an error in " << filename << std::endl;
                _Exit(1);
            }

            // All other numbers represent node-to-node distances.
            distance = std::vector<std::vector<float>>(num_nodes, std::vector<float>(num_nodes, 0));

            // The distances are given in lower-triangular matrix.
            auto dist = 0.0f;
            auto nread = 0u;
            for(auto i = 0u; i < num_nodes; i++) {
                for(auto j = 0u; j <= i; j++) {
                    float dist = 0.0f;

                    if(!(is >> dist)) {
                        std::cerr << "Could not read distance (" << i << "," << j << ") from " << filename << std::endl;
                        _Exit(1);
                    }

                    distance[i][j] = dist;
                    distance[j][i] = dist;
                    ++nread;
                }
            }

            if(nread != num_nodes * (num_nodes + 1) / 2) {
                std::cerr << "Expected to read " << num_nodes * (num_nodes + 1) / 2 << " distances, read " << nread << std::endl;
                _Exit(1);
            }
        }
    }
}
