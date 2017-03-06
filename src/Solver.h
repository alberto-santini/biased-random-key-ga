//
// Created by alberto on 26/08/16.
//

#ifndef RKBGA_SOLVER_H
#define RKBGA_SOLVER_H

#include <set>
#include <chrono>
#include <future>
#include <type_traits>
#include "Params.h"
#include "DefaultSolverVisitor.h"
#include "IndividualWithObjValue.h"

namespace bga {
    /**
     * Solves a problem with a Biased Genetic Algorithm.
     * @tparam Generator    The class that can generate new (random) individuals, e.g. to initialise
     *                      the population.
     * @tparam Evaluator    The class that evaluates the objective function of an individual.
     * @tparam Visitor      The visitor, that can be used for logging, or showing stats to the user, etc.
     *
     * Contracts:
     *  1)  \tparam Generator must implement the method:
     *      Individual generate() const;
     *  2)  \tparam Evaluator must implement the method:
     *      float evaluate(const Individual&) const;
     *  3)  \tparam Generator and \tparam Evaluator must typedef individual_type
     *      which will be used as Individual; this typename must be the same for both
     *      the \tparam Generator and the \tparam Evaluator. Furthermore, Individual
     *      must implement the method:
     *      Individual biased_crossover_with(Individual, float, std::mt19937&) const;
     *  4)  \tparam Visitor must implement the methods:
     *      void at_start(const IndividualWithObjValue<Individual>&) const;
     *      void at_iteration(const IndividualWithObjValue<Individual>&, uint32_t, float) const;
     *      void at_end(const IndividualWithObjValue<Individual>&, uint32_t, float) const;
     */
    template<   class Generator,
                class Evaluator,
                class Visitor = DefaultSolverVisitor<typename Generator::individual_type>>
    class Solver {
        static_assert(std::is_same<typename Generator::individual_type, typename Evaluator::individual_type>::value,
            "Generator and Evaluator operate on different kind of individuals");

        using Individual = typename Generator::individual_type;
        using Population = std::multiset<IndividualWithObjValue<Individual>>;

        /**
         * Genetic Algorithm Parameters.
         */
        const Params& params;

        /**
         * Generator used to produce the initial population.
         */
        const Generator& generator;

        /**
         * Evaluator used to obtain the objective function of an individual.
         */
        const Evaluator& evaluator;

        /**
         * Visitor to be called at certain points during the solution process.
         */
        const Visitor& visitor;

        /**
         * Ordered list of individuals with their objective value, i.e. the population.
         */
        mutable Population population;

        /**
         * Size of the elite population.
         */
        const uint32_t elite_size;

        /**
         * Number of new individuals that enter the population at each generation.
         */
        const uint32_t new_individuals_size;

    public:
        /**
         * Initialise the algorithm solver.
         */
        Solver(const Params& params, const Generator& generator, const Evaluator& evaluator, const Visitor& visitor) :
            params{params}, generator{generator}, evaluator{evaluator}, visitor{visitor}, population{Population()},
            elite_size{static_cast<uint32_t>(params.population_size * params.elite_share)},
            new_individuals_size{static_cast<uint32_t>(params.population_size * params.replace_share)} {}

        /**
         * Runs the Genetic Algorithm.
         * @return  The best individual found.
         */
        IndividualWithObjValue<Individual> solve() const {
            uint32_t generation = 0;
            uint32_t generations_no_improv = 0;

            auto start_time = std::chrono::steady_clock::now();

            // Generate the initial population.
            add_new_individuals(population, params.population_size);
            assert(population.size() == params.population_size);

            // Call the visitor's start action, pass the best individual.
            visitor.at_start(*population.begin());

            while(generation < params.max_generations && generations_no_improv < params.max_generations_no_improvement) {
                auto current_time = std::chrono::steady_clock::now();
                auto elapsed_time_s = std::chrono::duration<float>(current_time - start_time).count();

                // Check for timeout.
                if(elapsed_time_s > params.timeout_s) { break; }

                // Evolve!
                auto new_generation = evolve_new_generation();
                assert(new_generation.size() == params.population_size);

                // Check whether there has been a (strict) improvement.
                if(new_generation.begin()->objvalue < population.begin()->objvalue) { generations_no_improv = 0; }
                else { ++generations_no_improv; }

                // Replace the old population with the new generation.
                population = new_generation;

                // Call the visitor, if requested.
                if(generation > 0 && generation % params.visitor_freq_iterations == 0) { visitor.at_iteration(*population.begin(), generation, elapsed_time_s); }

                ++generation;
            }

            auto end_time = std::chrono::steady_clock::now();
            auto total_time_s = std::chrono::duration<float>(end_time - start_time).count();

            // Call the visitor's end action, pass the best individual.
            visitor.at_end(*population.begin(), generation, total_time_s);

            // Return the best individual.
            return *population.begin();
        }

    private:
        /**
         * Adds newly created individuals to a population.
         */
        void add_new_individuals(Population& population, uint32_t how_many) const {
            auto individuals = std::vector<Individual>();
            auto evaluations = std::vector<std::future<float>>(how_many);
            individuals.reserve(how_many);

            auto starting_size = population.size();

            // Launch generators and evaluators (asynchronously).
            for(auto i = 0u; i < how_many; i++) {
                individuals.push_back(generator.generate());
            }

            for(auto i = 0u; i < how_many; i++) {
                evaluations[i] = std::async(std::launch::async, [i,this,&individuals] () -> float { return evaluator.evaluate(individuals[i]); });
            }

            // Get the futures from evaluation.
            for(auto i = 0u; i < how_many; i++) {
                population.insert(IndividualWithObjValue<Individual>(individuals[i], evaluations[i].get()));
            }

            assert(population.size() - starting_size == how_many);
        }

        /**
         * Fills up the population via cross-over.
         */
        void do_crossover(Population& new_generation) const {
            assert(population.size() == params.population_size);

            // The number of individuals we have to add is the total population size,
            // minus the number of elite (which we copied from the previous generation)
            // and of new individuals (which we created from scratch).
            auto non_elite_size = params.population_size - elite_size - new_individuals_size;

            // Random number generation.
            std::mt19937::result_type random_data[std::mt19937::state_size];
            std::random_device source;
            std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
            std::seed_seq seeds(std::begin(random_data), std::end(random_data));
            auto mt = std::mt19937(seeds);
            auto elite_rnd = std::uniform_int_distribution<uint32_t>(0, elite_size - 1);
            auto non_elite_rnd = std::uniform_int_distribution<uint32_t>(0, non_elite_size - 1);

            auto individuals = std::vector<Individual>();
            auto evaluations = std::vector<std::future<float>>(non_elite_size);
            individuals.reserve(non_elite_size);

            // Do crossover and get evaluations asyncronously.
            for(auto i = 0u; i < non_elite_size; i++) {
                auto elite_it = population.begin(), non_elite_it = population.begin();

                // Pick a random elite individual.
                std::advance(elite_it, elite_rnd(mt));

                // Pick a random non-elite (and non-new) individual.
                std::advance(non_elite_it, elite_size + non_elite_rnd(mt));

                // Do biased crossover of the elite and non-elite individuals.
                individuals.push_back(elite_it->individual.biased_crossover_with(non_elite_it->individual, params.crossover_elite_bias, mt));
            }

            for(auto i = 0u; i < non_elite_size; i++) {
                // Launch evaluation.
                evaluations[i] = std::async(std::launch::async, [i,this,&individuals] () -> float { return evaluator.evaluate(individuals[i]); });
            }

            assert(individuals.size() == evaluations.size());
            assert(new_generation.size() + individuals.size() == params.population_size);

            // Get futures.
            for(auto i = 0u; i < non_elite_size; i++) {
                new_generation.insert(IndividualWithObjValue<Individual>(individuals[i], evaluations[i].get()));
            }

            assert(new_generation.size() == params.population_size);
        }

        /**
         * Evolves the next generation of individuals.
         */
        Population evolve_new_generation() const {
            assert(population.size() == params.population_size);

            auto it = population.begin();
            std::advance(it, elite_size);

            // Copy the elite population into the new generation.
            auto new_generation = Population{population.begin(), it};

            // Insert the mutants (asyncronously).
            add_new_individuals(new_generation, new_individuals_size);

            assert(new_generation.size() == elite_size + new_individuals_size);

            // Fills the population with crossover.
            do_crossover(new_generation);

            assert(new_generation.size() == params.population_size);

            return new_generation;
        }
    };
}


#endif //RKBGA_SOLVER_H
