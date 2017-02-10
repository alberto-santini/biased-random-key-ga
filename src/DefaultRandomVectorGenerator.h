//
// Created by alberto on 27/08/16.
//

#ifndef RKBGA_DEFAULTRANDOMVECTORGENERATOR_H
#define RKBGA_DEFAULTRANDOMVECTORGENERATOR_H

#include <cstdint>
#include <random>
#include <algorithm>
#include <functional>
#include "RandomVectorIndividual.h"

namespace bga {
    /**
     * Simple random-vector generator, which produces a random-key individual
     * of a given size, with random numbers in [0,1]. If the user has no particular
     * requirement on the construction of the individual besides the vector's
     * length, he can use this generator. Otherwise, he will have to implement
     * his own generator.
     */
    class DefaultRandomVectorGenerator {
        /**
         * Random-key vector length.
         */
        const uint32_t length;

        /**
         * Mersenne Twister used to generate the new individuals.
         */
        mutable std::mt19937 mt;

    public:
        using individual_type = RandomVectorIndividual;

        /**
         * New random vector generator for vectors of fixed length whose entries
         * are random numbers in [0,1].
         * @param length  The length of the generated vectors.
         */
        DefaultRandomVectorGenerator(uint32_t length) : length{length} {
            // Initialise the mersenne twister.
            std::mt19937::result_type random_data[std::mt19937::state_size];
            std::random_device source;
            std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
            auto seeds = std::seed_seq(std::begin(random_data), std::end(random_data));
            auto mt = std::mt19937(seeds);
        }

        /**
         * Generate a new random \class RandomVectorIndividual.
         */
        RandomVectorIndividual generate() const {
          auto dist = std::uniform_real_distribution<float>(0, 1);
          auto chromosome = std::vector<float>();
          chromosome.reserve(length);

          // Fill the chromosome with random numbers.
          for(auto i = 0u; i < length; i++) { chromosome.push_back(dist(mt)); }

          return RandomVectorIndividual{chromosome};
        }
    };
}

#endif //RKBGA_RANDOMVECTORGENERATOR_H
