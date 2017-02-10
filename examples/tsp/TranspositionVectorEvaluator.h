//
// Created by alberto on 27/08/16.
//

#ifndef RKBGA_TRANSPOSITIONVECTOREVALUATOR_H
#define RKBGA_TRANSPOSITIONVECTOREVALUATOR_H

#include "Graph.h"
#include "../../src/TranspositionVectorIndividual.h"

namespace bga {
    namespace tsp {
        /**
         * This class represents an evaluator for \class TranspositionVectorIndividual that
         * interprets the random vector as a tour in a \class Graph, and calculates
         * the cost of the tour.
         */
        class TranspositionVectorEvaluator {
            /**
             * The underlying graph.
             */
            const Graph& graph;

        public:
            using individual_type = TranspositionVectorIndividual;

            TranspositionVectorEvaluator(const Graph& graph) : graph{graph} {}

            /**
             * Evaluates a \class TranspositionVectorIndividual.
             */
            float evaluate(const TranspositionVectorIndividual& individual) const;
        };
    }
}

#endif //RKBGA_TRANSPOSITIONVECTOREVALUATOR_H
