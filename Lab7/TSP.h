/**
 * @file TSP.h
 * @brief TSP data
 *
 */

#ifndef TSP_H
#define TSP_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

/**
 * Class that describes a TSP instance (a cost matrix, nodes are identified by integer 0 ... n-1)
 */
class TSP
{
public:
  TSP() : n(0) , infinite(1e10) { }
  int n; //number of nodes
  std::vector< std::vector<double> > cost;
  double infinite; // infinite value (an upper bound on the value of any feasible solution)

  void read(const char* filename)
  {
    std::ifstream in(filename);
    // read size
    in >> n;
    std::cout << "number of nodes n = " << n << std::endl;
    // read costs
    cost.resize(n);
    for (int i = 0; i < n; i++) {
      cost[i].reserve(n);
      for (int j = 0; j < n; j++) {
        double c;
        in >> c;
        cost[i].push_back(c);
      }
    }
    in.close();
    
    // set infinite value
    infinite = 0;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        infinite += cost[i][j];
      }
    }
    infinite *= 2;
  }

};

#endif /* TSP_H */
