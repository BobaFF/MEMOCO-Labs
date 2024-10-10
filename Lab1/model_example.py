#DOcplex basic functions
from docplex.mp.model import Model

# Creating an "empty" model
m = Model(name="Example model")

#Defining a variable
c_var = m.continuous_var()
i_var = m.integer_var()
b_var = m.binary_var()
#Variables can have optional arguments like: name, lb (lower bound), ub (upper bound)
#Default values are: name = x1, x2.. ; lb = 0; ub = +infinity

#Defining expressions: functions of decision and/or usual variables
expr_1 = 6 * c_var + i_var - pow(3,2) * b_var
expr_2 = 10 * c_var 

#Creating a constraint
m.add_constraint(expr_1 <= expr_2) #or >= , ==

#Creating the objective function
m.minimize(expr_1) #or maximize

#Solving the model
m.solve()
#* Optional arguments: 
#   - log_outputs = True | False
#   - cplex_parameters = ...
#   ...*#

#Checking status of the solution (optimal, infeasible, unounded, ..)
if m.solution == None:
    print("Problems! Status: ", m.get_solve_status())

#Printing the solution:
if m.solution != None:
    m.print_solution() #standard info (status, o.f. and variables values)
    sol = m.solution
    print(sol[c_var]) #value of var c_var
    print(sol.get_objective_value()) #value of the o.f.

#Exporting the model 
m.export_as_lp(basename='filename', path='path', hide_user_names=False) #text file (e.g., LP format)

#Exporting the solution in json format
m.solution.export('filename')

#Exporting the solution in a string:
print(m.solution.to_string())