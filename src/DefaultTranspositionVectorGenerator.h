//
// Created by alberto on 27/08/16.
//

#ifndef RKBGA_DEFAULTTRANSPOSITIONVECTORGENERATOR_H
#define RKBGA_DEFAULTTRANSPOSITIONVECTORGENERATOR_H

#include <random>
#include <algorithm>
#include <functional>
#include "TranspositionVectorIndividual.h"

namespace bga {
    /**
     * Simple transposition-vector generator, which produces an individual
     * of a given size (actually of size 2 * (size - 1)), with random integer
     * numbers in [0,size-1]. If the user has no particular requirement on the
     * construction of the individual, besides on the vector's length (and
     * therefore range in which the individuals are picked at random), he can
     * use this generator. Otherwise, he will have to implement his own generator.
     */
    class DefaultTranspositionVectorGenerator {
        /**
         * Number of items of which we want to represent a permutation.
         */
        const uint32_t nitems;

        /**
         * Mersenne Teister used to generate new individuals.
         */
        mutable std::mt19937 mt;

    public:
        using individual_type = TranspositionVectorIndividual;

        /**
         * New random vector generator for vectors of fixed length whose entries
         * are couples of items. The whole vector can be thought of as a permutation.
         * @param num_items   The number of items to permute. The vector contains
                              num_items pairs, and therefore 2*num_items elements.
         */
        DefaultTranspositionVectorGenerator(uint32_t nitems) : nitems{nitems} {
            // Initialise the mersenne twister.
            std::mt19937::result_type random_data[std::mt19937::state_size];
            std::random_device source;
            std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
            auto seeds = std::seed_seq(std::begin(random_data), std::end(random_data));
            auto mt = std::mt19937(seeds);
        }

        /**
         * Generate a new random \class TranspositionVectorIndividual.
         */
        TranspositionVectorIndividual generate() const {
          auto dist = std::uniform_int_distribution<uint32_t>(0, nitems - 1);
          auto chromosome = std::vector<uint32_t>();
          chromosome.reserve(2 * (nitems - 1));

          // Fill the chromosome with random numbers.
          for(auto i = 0; i < 2 * (nitems - 1); i++) { chromosome.push_back(dist(mt)); }

          return TranspositionVectorIndividual{chromosome};
        }
    };
}

#endif //RKBGA_DEFAULTTRANSPOSITIONVECTORGENERATOR_H
