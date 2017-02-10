//
// Created by alberto on 27/08/16.
//

#include "TranspositionVectorEvaluator.h"

namespace bga {
    namespace tsp {
        float bga::tsp::TranspositionVectorEvaluator::evaluate(const bga::TranspositionVectorIndividual &individual) const {
            auto permutation = std::vector<uint32_t>(graph.num_nodes());
            std::iota(permutation.begin(), permutation.end(), 0u);

            for(auto i = 0u; i < 2 * (graph.num_nodes() - 1); i+= 2) {
                std::swap(permutation[individual.component(i)], permutation[individual.component(i + 1)]);
            }

            auto cost = 0.0f;
            for(auto i = 0u; i < permutation.size() - 1; i++) {
                cost += graph.get_distance(permutation[i], permutation[i+1]);
            }
            cost += graph.get_distance(permutation[permutation.size() - 1], permutation[0]);

            return cost;
        }
    }
}
