//
// Created by alberto on 27/08/16.
//

#ifndef RKBGA_RANDOMVECTOREVALUATOR_H
#define RKBGA_RANDOMVECTOREVALUATOR_H

#include "Graph.h"
#include "../../src/RandomVectorIndividual.h"

namespace bga {
    namespace tsp {
        /**
         * This class represents an evaluator for \class RandomVectorIndividual that
         * interprets the random vector as a tour in a \class Graph, and calculates
         * the cost of the tour.
         */
        class RandomVectorEvaluator {
            /**
             * The underlying graph.
             */
            const Graph& graph;

        public:
            using individual_type = RandomVectorIndividual;

            RandomVectorEvaluator(const Graph& graph) : graph{graph} {}

            /**
             * Evaluates a \class RandomVectorIndividual.
             */
            float evaluate(const RandomVectorIndividual& individual) const;
        };
    }
}


#endif //RKBGA_RANDOMVECTOREVALUATOR_H
