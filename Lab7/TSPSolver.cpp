/**
 * @file TSPSolver.cpp
 * @brief TSP solver (neighborhood search)
 *
 */

#include "TSPSolver.h"
#include <iostream>

bool TSPSolver::solve ( const TSP& tsp , const TSPSolution& initSol , TSPSolution& bestSol )
{
  try
  {
    bool stop = false;

    TSPSolution currSol(initSol);

    while ( ! stop ) {
      TSPMove move;
      TSPSolution neigSol(tsp);
      TSPSolution neigBest(currSol);
      for (int i_subs_init = 1; i_subs_init < currSol.sequence.size() - 2; i_subs_init++) {
        for (int i_subs_end = i_subs_init + 1; i_subs_end < currSol.sequence.size() - 1; i_subs_end++) {
          move.substring_begin = i_subs_init;
          move.substring_end = i_subs_end;
          neigSol = apply2optSwap(currSol, move);
          double neigCost = this->evaluate(neigSol, tsp);
          double neighDecrement = neigCost - this->evaluate(neigBest, tsp);
          if (neighDecrement < -1e-6) {
            neigBest = neigSol;
          }
        }
      }
      double currCost = this->evaluate(currSol, tsp);
      double bestCost = this->evaluate(neigBest, tsp);
      if (bestCost - currCost  < -1e-6) {
        currSol = neigBest;
      } else stop = true;
      
    }
    bestSol = currSol;
  }
  catch(std::exception& e)
  {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    return false;
  }
  return true;
}


TSPSolution TSPSolver::apply2optSwap(const TSPSolution& tspSol, const TSPMove& move) {
  TSPSolution newSol(tspSol);
  for(int i = move.substring_begin; i <= move.substring_end; i++){
    newSol.sequence[i] = tspSol.sequence[move.substring_end - (i - move.substring_begin)];
  }
  return newSol;
};

