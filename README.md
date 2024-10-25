# Methods and Models for Combinatorial Optimization

This repository contains the activities of the laboratory sessions held during the course **Methods and Models for Combinatorial Optimization** as part of the Master’s Degree in **Computer Science** at UniPD, taught by Professor Luigi De Giovanni for the academic year 2024/2025.
Laboratory sessions: 
- [Lab 1](#lab-1) - 10/10/2024

- [Lab 2](#lab-2) - 17/10/2024

- [Lab 3](#lab-3) - 24/10/2024

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
    
