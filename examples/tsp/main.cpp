#include <iostream>

#include "../../DefaultTranspositionVectorGenerator.h"
#include "../../src/DefaultRandomVectorGenerator.h"
#include "../../TranspositionVectorIndividual.h"
#include "../../DefaultSolverVisitor.h"
#include "../../ParamsBuilder.h"
#include "../../Solver.h"

#include "Graph.h"
#include "RandomVectorEvaluator.h"
#include "TranspositionVectorEvaluator.h"

void solve_tsp_randomkey(std::string instance) {
    using namespace bga;
    using namespace bga::tsp;

    auto graph = Graph{instance};
    auto generator = DefaultRandomVectorGenerator{graph.num_nodes()};
    auto evaluator = RandomVectorEvaluator{graph};
    auto visitor = DefaultSolverVisitor<RandomVectorIndividual>{instance + "-results-rk.csv"};
    auto params = ParamsBuilder{}.with_timeout_s(60).with_visitor_freq_iterations(1).build();
    auto solver = Solver<DefaultRandomVectorGenerator, RandomVectorEvaluator>{
        params, generator, evaluator, visitor
    };

    solver.solve();
}

void solve_tsp_transposition(std::string instance) {
    using namespace bga;
    using namespace bga::tsp;

    auto graph = Graph{instance};
    auto generator = DefaultTranspositionVectorGenerator{graph.num_nodes()};
    auto evaluator = TranspositionVectorEvaluator{graph};
    auto visitor = DefaultSolverVisitor<TranspositionVectorIndividual>{instance + "-results-t.csv"};
    auto params = ParamsBuilder{}.with_timeout_s(60).with_visitor_freq_iterations(1).build();
    auto solver = Solver<DefaultTranspositionVectorGenerator, TranspositionVectorEvaluator>{
            params, generator, evaluator, visitor
    };

    solver.solve();
}

int main(int argc, char* argv[]) {
    solve_tsp_randomkey(std::string(argv[1]));
    solve_tsp_transposition(std::string(argv[1]));
    return 0;
}
