//
// Created by alberto on 26/08/16.
//

#ifndef RKBGA_PARAMS_H
#define RKBGA_PARAMS_H

#include <cstdint>

namespace bga {
    /**
     * Parameters for the Biased Genetic Algorithm.
     */
    struct Params {
        /**
         * Total number of individuals in the population.
         */
        const uint32_t population_size;

        /**
         * Share of population that is considered elite (0 to 1).
         */
        const float elite_share;

        /**
         * Share of population that we replace anew at each generation (0 to 1).
         */
        const float replace_share;

        /**
         * Bias towards the elite parent, when doing crossover.
         */
        const float crossover_elite_bias;

        /**
         * Maximum number of generations (stopping criterion).
         */
        const uint32_t max_generations;

        /**
         * Maximum number of consecutive generations without improvement (stopping criterion).
         */
        const uint32_t max_generations_no_improvement;

        /**
         * Maximum running time in seconds (stopping criterion).
         */
        const uint32_t timeout_s;

        /**
         * How often should we call the visitor? (In number of iterations).
         */
        const uint32_t visitor_freq_iterations;

        Params( uint32_t population_size, float elite_share, float replace_share,
                float crossover_elite_bias, uint32_t max_generations,
                uint32_t max_generations_no_improvement, uint32_t timeout_s,
                uint32_t visitor_freq_iterations) :
                population_size{population_size}, elite_share{elite_share}, replace_share{replace_share},
                crossover_elite_bias{crossover_elite_bias}, max_generations{max_generations},
                max_generations_no_improvement{max_generations_no_improvement}, timeout_s{timeout_s},
                visitor_freq_iterations{visitor_freq_iterations} {}
    };
}

#endif //RKBGA_PARAMS_H
