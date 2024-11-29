/**
 * @file main.cpp
 * @brief 
 */


#include <stdexcept>
#include <ctime>
#include <sys/time.h>

#include "TSPSolver.h"

// error status and messagge buffer
int status;
char errmsg[255];


int main (int argc, char const *argv[])
{
  try
  {
    if (argc < 2) throw std::runtime_error("usage: ./main filename.dat");
    
    /// create the instance (reading data)
    TSP tspInstance;
    tspInstance.read(argv[1]);
    TSPSolution aSolution(tspInstance);
    
    /// initialize clocks for running time recording
    ///   two ways:
    ///   1) CPU time (t2 - t1)
    clock_t t1,t2;
    t1 = clock();
    ///   2) wall-clock time (tv2 - tv1)
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    
    /// create solver class
    TSPSolver tspSolver;
    /// initial solution (random)
    tspSolver.initRnd(aSolution);
    
    /// run the neighbourhood search
    TSPSolution bestSolution(tspInstance);
    tspSolver.solve(tspInstance,aSolution,bestSolution);
    
    /// final clocks
    t2 = clock();
    gettimeofday(&tv2, NULL);
    
    std::cout << "FROM solution: "; 
    aSolution.print();
    std::cout << "(value : " << tspSolver.evaluate(aSolution,tspInstance) << ")\n";
    std::cout << "TO   solution: "; 
    bestSolution.print();
    std::cout << "(value : " << tspSolver.evaluate(bestSolution,tspInstance) << ")\n";
    std::cout << "in " << (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6)) << " seconds (user time)\n";
    std::cout << "in " << (double)(t2-t1) / CLOCKS_PER_SEC << " seconds (CPU time)\n";
    
  }
  catch(std::exception& e)
  {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
  }
  return 0;
}
