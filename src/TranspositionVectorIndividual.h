//
// Created by alberto on 26/08/16.
//

#ifndef RKBGA_TRANSPOSITIONVECTORINDIVIDUAL_H
#define RKBGA_TRANSPOSITIONVECTORINDIVIDUAL_H

#include <cstdint>
#include <vector>
#include <random>
#include <cassert>

namespace bga {
    /**
     * This class represents a transposition individual, in which the chromosome is
     * an even-sized vector of unsigned integers. This numbers can be thought of as
     * a succession of pairs (i,j). Each pair can be seen as a swap of some element
     * in position i, with some element in position j. Therefore, n-1 pairs can be
     * used to encode any permutation of n elements.
     */
    class TranspositionVectorIndividual {
        /**
         * The chromosome, made by a vector of unsigned integers.
         */
        const std::vector<uint32_t> chromosome;

    public:
        /**
         * Construct from an explicitely given chromosome.
         * @param chromosome    The transposition chromosome.
         * @return              The newly created individual.
         */
        TranspositionVectorIndividual(std::vector<uint32_t> chromosome) : chromosome{chromosome} {
            // Assert that the chromosome's length is even, since it is made of pairs.
            assert(chromosome.size() % 2 == 0);
        }

        /**
         * Produces a new individual, via biased crossover of this individual with another one.
         * The crossover is done pair-by-pair on the elements of the chromosome, so that the child
         * inherits each pair from one of the two parents.
         * @param other The other parent individual.
         * @param bias  A number in [0,1] that represents the probabilty that the child will
         *              inherit each pair of its chromosome from this individual. Therefore,
         *              the child will have a probability of 1-bias of inheriting each pair of
         *              its chromosome from the other parent.
         * @param mt    A Mersenne Twister used to tosse the biased coin.
         * @return      The new child.
         */
        TranspositionVectorIndividual biased_crossover_with(const TranspositionVectorIndividual &other, float bias, std::mt19937& mt) const {
          assert(other.chromosome.size() == chromosome.size());
          assert(0 <= bias && bias <= 1);

          // Copy the current chromosome into the new one.
          auto new_chromosome = chromosome;

          // Gives uniformly distributed real numbers in [0,1], used for biased crossover.
          auto dist = std::uniform_real_distribution<float>(0, 1);

          for(auto i = 0u; i < chromosome.size(); i += 2) {
              // Toss the coin! :-)
              float p = dist(mt);

              if(p >= bias) {
                  // Change the i-th and (i+1)-th components taking them from the other parent.
                  new_chromosome[i] = other.chromosome[i];
                  new_chromosome[i+1] = other.chromosome[i+1];
              }
          }

          return TranspositionVectorIndividual{new_chromosome};
        }

        /**
         * Return the i-th component of the chromosome.
         */
        uint32_t component(uint32_t i) const { return chromosome[i]; }
    };
}

#endif //RKBGA_TRANSPOSITIONVECTORINDIVIDUAL_H
