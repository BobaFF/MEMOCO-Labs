# Methods and Models for Combinatorial Optimization

This repository contains the activities of the laboratory sessions held during the course **Methods and Models for Combinatorial Optimization** as part of the Master’s Degree in **Computer Science** at UniPD, taught by Professor Luigi De Giovanni for the academic year 2024/2025.
Laboratory sessions: 
- [Lab 1](#lab-1) - 10/10/2024

- [Lab 2](#lab-2) - 17/10/2024

- [Lab 3](#lab-3) - 24/10/2024

- [Lab 4](#lab-4) - 7/11/2024

- [Lab 5-6](#lab-5-6) - 7/11/2024 & 14/11/2024

- [Lab 7](#lab-7) - 28/11/2024

- [More models](#more-models)

## Lab 1 
### Environment:

> **Note:** Python versions above 3.11 are not supported by CPLEX.

- **Python version:** 3.9.6
- **Libraries:** `docplex`, `cplex`

### Exercises:

- `model_example.py`: Basic functions in DOcplex.
- `farmer.py`: A model based on the farmer problem.
- `diet.py`: A model based on the diet problem.

## Lab 2
### Environment:

Same as Lab 1.

### Exercises:
All exercises contain data that is separated from their respective models. The models are designed to be general and independent from the data, but both are in the same file.
- `farmer.model.py`: Farmer problem and Optimal Production Mix Model. 
- `parfume.model.py`:  Parfume problem and Optimal Production Mix Model. 
- `money_maker.model.py`:  Money makers problem and Optimal Production Mix Model. This model uses loops inside DOcplex shortcuts on variables and constraints, such as `continuous_var_dict()` and `add_constraints()`.
- `diet.model.py`: Diet problem  and Minimum Cost Covering Model, utilizing DOcplex shortcuts.
- `emergency_location.model.py`: Emergency Location problem  and Minimum Cost Covering Model, utilizing DOcplex shortcuts.

## Lab 3
### Environment:

Same as Lab 1.

### Exercises:
Differently from the previous lab, data and models are separated into different files.
- `transportation.model.py`: implementation of the basic transportation model. To solve the model using the `refrigerator.json` data, pass its name as an argument when launching the program. 
- `advanced_transportation.model.py`: implementation of an advanced transportation model, including multiple different constraints. It removes expensive links that exceed a parametrized _threshold_ of the maximum cost. 
The constraints are:
    1. **Minimum cost on link**: if the cost of link from _i_ to _j_ is at most _LowCost_, then the flow on this link should be at least _LowCostMinOnLink_;
    2. **Special origin & destination**: destination _SpecialDestination_ should receive at least _MinToSpecialDest_ units from each origin, except for origin _SpecialOrigin_;
    3. **Significant supply**: At least a _SignificantNumber_ of origins must significantly supply each destination. "Significantly supply" means no less than a _SignificantFraction_.
    
## Lab 4
### Environment:
Same as Lab 1.

### Exercises:
- `facility_loc.py`: implementation of the facility locations problem; it solves the model using the `facility_loc_basic_and_plus.json` data. In this model we use lists, which are more efficient than dictionaries: the creation and resolution of the model is way faster for this reason.

## Lab 5-6
### Environment:
- **C++ version:** 16.0.0
- **Libraries:** `Cplex C APIs`

> **Note:** Prof. De Giovanni provided us with `cpxmacro.h`, which containins macros to save us some times, and a `makefile` to easily compile the program.

### Exercises:
- `moving_scaffolds.cpp`: implementation of the "Moving scaffolds between yards" problem.

## Lab 7
### Environment:
- **C++ version:** 16.0.0

The professor provided us with the following files for the implementation of the local search algorithm:  
- `TSP.h`: a class that represents the TSP problem. It reads data from an argument `.dat` file.  
- `TSPSolution.h`: a class that represents a solution for a TSP problem. A solution is represented as a path in a vector of `n + 1` elements, where the first and last elements are set to `0` (representing a closed loop).  
- `TSPSolver.h`/`TSPSolver.cpp`: a class that solves the TSP problem using neighborhood search and 2-opt moves. The solver is based on a local search strategy.

The objective of the lab is to implement the `solve()` function in the `TSPSolver` class, along with any additional helper functions required.



The aim of the lab is to implement the `solve()` function in the ``TSPSolver`` class, along with any additional helper functions needed.


```c++ 
TSPSolution apply2optSwap(const TSPSolution& , const TSPMove&)
```
This helper function applies a 2-opt swap, which reverses a substring of the solution's path. The parameters are:  
- `solution`: the `TSPSolution` to which the swap is applied.  
- `move`: a `TSPMove` struct containing the start and end indices of the substring to be reversed.  

The function returns a new `TSPSolution` instance.
 
```c++
bool solve(const TSP& tsp , const TSPSolution& initSol , TSPSolution& bestSol)
```

The `apply2optSwap` function should be used inside two nested loops, as all possible neighbors must be considered.
The following constraints apply:  
1. The indices must not be equal, as we are not interested in substrings of length `1`.  
2. The first and last elements of `sol.sequence` must remain unchanged because they are fixed. 
3. The start index of the substring, `i`, should range from `1` to `n-2` (excluding).  
4. The end index of the substring, `j`, should range from `i + 1` to `n-1` (excluding).

*E.g.: `sol.sequence.size() = n + 1`
 `sol.sequence = < 0, 5, 9, ..., 6, 3, 0 >`*, indexes are ``<0, 1, 2, ..., n-2, n-1, n>``: 

After both loops, we check if the best neighbor represents an improvement over the current solution.  
- If no improvement is found, the search is complete, and the local optimum has been reached.  
- Otherwise, the current solution is updated with the best neighbor, and the process is repeated.

> **Note:** The evaluation method is in the solver, not in the solution.

### Considerations

The current implementation works, but it is not efficient. To improve efficiency, we should adopt an incremental approach. 

Generating all possible solutions is unnecessary. Instead, we are only interested in evaluating them. Therefore, it is not required to explicitly create a neighbor to evaluate it.  

To make the implementation more efficient, we should focus on identifying the best neighbor directly. Once the best neighbor is identified, we can move to it without generating all possible neighbors.  

The key is to determine the best move. When the best move is identified, we generate only the corresponding solution, avoiding the need to evaluate all possible moves.  

Using the following formula, if the variation exceeds the current `bestDecrement` we can immediately determine the correct move.

Reversing sequence between *i* and *j* in the sequence ![Formula](https://latex.codecogs.com/png.latex?%5Clangle%201%2C%20%5Cdots%2C%20h%2C%20i%2C%20%5Cdots%2C%20j%2C%20l%2C%20%5Cdots%2C%201%20%5Crangle)


![Formula](https://latex.codecogs.com/png.latex?C_\text{new}=C_\text{old}-c_{hi}-c_{jl}+c_{hj}+c_{il})


## More models
### Envirnomnet:
- **C++ version:** 16.0.0
- **Libraries:** `Cplex C APIs`
More models implemented using Cplex C APIs.