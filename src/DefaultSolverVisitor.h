//
// Created by alberto on 26/08/16.
//

#ifndef RKBGA_DEFAULTSOLVERVISITOR_H
#define RKBGA_DEFAULTSOLVERVISITOR_H

#include <string>
#include <fstream>
#include <iostream>
#include "IndividualWithObjValue.h"

namespace bga {
    /**
     * Utility class that implements a default solver for the visitor.
     * The solver is called before starting the algorithm, then after a certain
     * amount of iterations (multiple times), and finally at the end of the algorithm.
     *
     * @tparam Individual   The individual used in the Genetic Algorithm.
     */
    template<class Individual>
    class DefaultSolverVisitor {
        /**
         * Filename where we write the log.
         */
        const std::string outfile_name;

        /**
         * Outfile stream.
         */
        mutable std::ofstream* outfile;

    public:
        /**
         * Constructs a visitor that logs to file.
         * @param outfile_name  Filename of the logs file.
         */
        DefaultSolverVisitor(std::string outfile_name) : outfile_name{outfile_name}, outfile{new std::ofstream{outfile_name, std::ios::out}} {
            *outfile << "iteration,time,bestobj" << std::endl;
        }

        /**
         * Destructs the visitor and closes the file.
         */
        ~DefaultSolverVisitor() {
            outfile->close();
            delete outfile;
        }

        /**
         * Action to be invoked before the genetic part of the algorithm starts.
         * (I.e., after the initial population creation, but before the first crossover).
         * @param individual    The best individual in the intial population.
         */
        void at_start(const IndividualWithObjValue<Individual>& individual) const {
            *outfile << "0,0," << individual.objvalue << std::endl;
        }

        /**
         * Action to be invoked at certain intervals during the solution process.
         * @param individual        The best individual so far.
         * @param iteration         Current iteration.
         * @param elapsed_time_s    Current elapsed time.
         */
        void at_iteration(const IndividualWithObjValue<Individual>& individual, uint32_t iteration, float elapsed_time_s) const {
            *outfile << iteration << "," << elapsed_time_s << "," << individual.objvalue << std::endl;
        }

        /**
         * Action to be invoked at the end of the solution process.
         * @param individual        The best individual found.
         * @param iteration         Last iteration number.
         * @param elapsed_time_s    Total elapsed time.
         */
        void at_end(const IndividualWithObjValue<Individual>& individual, uint32_t iteration, float elapsed_time_s) const {
            std::cout << "Terminating after " << iteration << " iterations and " << elapsed_time_s << " seconds." << std::endl;
            std::cout << "Best objective value: " << individual.objvalue << std::endl;
        }
    };
}

#endif //RKBGA_SOLVERVISITOR_H
