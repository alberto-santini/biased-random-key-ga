//
// Created by alberto on 27/08/16.
//

#include "RandomVectorEvaluator.h"

namespace bga {
    namespace tsp {
        float RandomVectorEvaluator::evaluate(const bga::RandomVectorIndividual &individual) const {
            auto permutation = std::vector<uint32_t>(graph.num_nodes());
            std::iota(permutation.begin(), permutation.end(), 0u);
            std::sort(permutation.begin(), permutation.end(), [&] (auto i, auto j) { return individual.component(i) < individual.component(j); });

            auto cost = 0.0f;
            for(auto i = 0u; i < permutation.size() - 1; i++) {
                cost += graph.get_distance(permutation[i], permutation[i+1]);
            }
            cost += graph.get_distance(permutation[permutation.size() - 1], permutation[0]);

            return cost;
        }
    }
}