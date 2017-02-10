//
// Created by alberto on 26/08/16.
//

#ifndef RKBGA_PARAMSBUILDER_H
#define RKBGA_PARAMSBUILDER_H

#include <cstdint>
#include <limits>
#include "Params.h"

namespace bga {
    /**
     * Builder utility class for \class Params.
     */
    class ParamsBuilder {
        uint32_t population_size;
        float elite_share;
        float replace_share;
        float crossover_elite_bias;
        uint32_t max_generations;
        uint32_t max_generations_no_improvement;
        uint32_t timeout_s;
        uint32_t visitor_freq_iterations;

    public:
        /**
         * Initialises the builder with default values for the parameters.
         */
        ParamsBuilder() :   population_size{250}, elite_share{0.2}, replace_share{0.1},
                            crossover_elite_bias{0.7}, max_generations{std::numeric_limits<uint32_t>::max()},
                            max_generations_no_improvement{std::numeric_limits<uint32_t>::max()},
                            timeout_s{std::numeric_limits<uint32_t>::max()}, visitor_freq_iterations{1000} {}

        ParamsBuilder& with_population_size(uint32_t population_size) { this->population_size = population_size; return *this; }
        ParamsBuilder& with_elite_share(float elite_share) { this->elite_share = elite_share; return *this; }
        ParamsBuilder& with_replace_share(float replace_share) { this->replace_share = replace_share; return *this; }
        ParamsBuilder& with_crossover_elite_bias(float crossover_elite_bias) { this->crossover_elite_bias = crossover_elite_bias; return *this; }
        ParamsBuilder& with_max_generations(uint32_t max_generations) { this->max_generations = max_generations; return *this; }
        ParamsBuilder& with_max_generations_no_improvement(uint32_t max_generations_no_improvement) { this->max_generations_no_improvement = max_generations_no_improvement; return *this; }
        ParamsBuilder& with_timeout_s(uint32_t timeout_s) { this->timeout_s = timeout_s; return *this; }
        ParamsBuilder& with_visitor_freq_iterations(uint32_t visitor_freq_iterations) { this->visitor_freq_iterations = visitor_freq_iterations; return *this; }
        Params build() { return Params{population_size, elite_share, replace_share, crossover_elite_bias, max_generations, max_generations_no_improvement, timeout_s, visitor_freq_iterations}; }
    };
}

#endif //RKBGA_PARAMSBUILDER_H
