//
// Created by alberto on 26/08/16.
//

#ifndef RKBGA_INDIVIDUALWITHOBJVALUE_H
#define RKBGA_INDIVIDUALWITHOBJVALUE_H

#include <tuple>

namespace bga {
    /**
     * Utility class that simply stores an individual and its objective value.
     * @tparam Individual   The individual used in the Genetic Algorithm.
     */
    template<class Individual>
    struct IndividualWithObjValue {
        const Individual individual;
        const float objvalue;

        IndividualWithObjValue(Individual individual, float objvalue) : individual{individual}, objvalue{objvalue} {}

        /**
         * Compares individuals by objective value.
         */
        bool operator<(const IndividualWithObjValue& other) const { return objvalue < other.objvalue; }
    };
}

#endif //RKBGA_INDIVIDUALWITHFITNESS_H
