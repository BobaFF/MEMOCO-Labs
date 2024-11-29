/**
 * @file TSPSolver.h
 * @brief TSP solver (neighborhood search)
 *
 */

#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include <vector>

#include "TSPSolution.h"

/**
 * Class representing substring reversal move
 */
typedef struct move {
  int			substring_begin;
  int			substring_end;
} TSPMove;

/**
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class TSPSolver
{
public:
  /** Constructor */
  TSPSolver ( ) { }
  /**
   * evaluate a solution
   * @param sol: solution to be evaluated
   * @param TSP: TSP data
   * @return the value of the solution
   */
  double evaluate ( const TSPSolution& sol , const TSP& tsp ) const {
    // 0 1 2 ... n-1 n are index
    // 0 6 7 ... 12  0 are cities in position i
    double total = 0.0;
    for ( uint i = 0 ; i < sol.sequence.size() - 1 ; ++i ) {
      int from = sol.sequence[i]  ;
      int to   = sol.sequence[i+1];
      total += tsp.cost[from][to];
    }
    return total;
  }
  /**
   * initialize a solution as a random sequence by random swaps
   * @param sol solution to be initialized
   * @return true if everything OK, false otherwise
   */
  bool initRnd ( TSPSolution& sol ) {
    srand(time(NULL));
    for ( uint i = 1 ; i < sol.sequence.size() ; ++i ) {
      // intial and final position are fixed (initial/final node remains 0)
      int idx1 = rand() % (sol.sequence.size()-2) + 1;
      int idx2 = rand() % (sol.sequence.size()-2) + 1;
      int tmp = sol.sequence[idx1];
      sol.sequence[idx1] = sol.sequence[idx2];
      sol.sequence[idx2] = tmp;
    }
    std::cout << "### "; sol.print(); std::cout << " ###" << std::endl;
    return true;
  }
  /**
   * search for a good tour by neighbourhood search
   * @param TSP TSP data
   * @param initSol initial solution
   * @param bestSol best found solution (output)
   * @return true id everything OK, false otherwise
   */
  bool solve ( const TSP& tsp , const TSPSolution& initSol , TSPSolution& bestSol );

protected:
  /**
  * Apply the 2 opt substring reversal
  * @param TSPSolution tspSol current solution
  * @param TSPMove move to be performed
  * @return TSPSolution a new solution 
  */
  TSPSolution apply2optSwap(const TSPSolution& , const TSPMove&);

};

#endif /* TSPSOLVER_H */
