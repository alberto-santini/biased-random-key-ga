//
// Created by alberto on 26/08/16.
//

#ifndef RKBGA_RANDOM_VECTOR_INDIVIDUAL_H
#define RKBGA_RANDOM_VECTOR_INDIVIDUAL_H

#include <vector>
#include <random>
#include <cassert>

namespace bga {
    /**
     * This class represents a random-key individual, in which the chromosome is
     * a vector of random keys in [0,1]. These keys can be used, for example, to
     * encode a permutation, by sorting them and linking the original position of
     * a key to its position in the sorted vector.
     */
    class RandomVectorIndividual {
        /**
         * The chromosome, made by a vector of random keys.
         */
        const std::vector<float> chromosome;

    public:
        /**
         * Construct from an explicitely given chromosome.
         * @param chromosome    The random-key chromosome
         * @return              The newly built individual.
         */
        RandomVectorIndividual(std::vector<float> chromosome) : chromosome{chromosome} {}

        /**
         * Produces a new individual, via biased crossover of this individual with another one.
         * The crossover is done element-by-element on the elements of the chromosome, so that the child
         * inherits each element from one of the two parents.
         * @param other The other parent individual.
         * @param bias  A number in [0,1] that represents the probabilty that the child will
         *              inherit each element of its chromosome from this individual. Therefore,
         *              the child will have a probability of 1-bias of inheriting each element of
         *              its chromosome from the other parent.
         * @param mt    A Mersenne Twister used to toss the biased coin.
         * @return      The new child.
         */
        RandomVectorIndividual biased_crossover_with(const RandomVectorIndividual& other, float bias, std::mt19937& mt) const {
          assert(other.chromosome.size() == chromosome.size());
          assert(0 <= bias && bias <= 1);

          // Copy the current chromosome into the new one.
          auto new_chromosome = chromosome;

          // Gives uniformly distributed real numbers in [0,1], used for biased crossover.
          auto dist = std::uniform_real_distribution<float>(0, 1);

          for(auto i = 0u; i < chromosome.size(); i++) {
              // Toss the coin! :-)
              float p = dist(mt);

              if(p >= bias) {
                  // Change the i-th component taking it from the other parent.
                  new_chromosome[i] = other.chromosome[i];
              }
          }

          return RandomVectorIndividual{new_chromosome};
        }

        /**
         * Returns the i-th component of the chromosome.
         */
        float component(uint32_t i) const { return chromosome[i]; }
    };
}
#endif //RKBGA_RANDOM_VECTOR_INDIVIDUAL_H
