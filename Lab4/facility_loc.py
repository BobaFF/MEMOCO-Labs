from docplex.mp.model import Model
import json

#Read data from a json file
with open('facility_loc_basic_and_plus.json', 'r') as file:
    data = json.load(file)

#Data
I_names = data["I_names"]
I = range(len(I_names))
# lists of fixed costs, variable costs and revenues for each location (euro per 100sqm)
W = data["W"]

F = data["f_list"]

C = data["c_list"]

R = data["r_list"]

K = data["max_num_open"]

minLoc = data["min_num_open"]

epsilon = data["min_size_to_open"]

#Parameters
#we specialize the big M for each constraints
M = [W - F[i]/C[i] for i in I]

#Model
m = Model(name=data["modelname"])

#Variables
#Creates many var as many elements in I
x = m.continuous_var_list(I, lb = 0, name = 'x')
y = m.binary_var_list(I, name='y')

#o.f.
#both R and x are lists, we can access their elements just with their index, which is a number
m.maximize(m.sum(R[i] * x[i] for i in I))

#Constraints
m.add_constraint_(m.sum(C[i] * x[i] + F[i] * y[i] for i in I) <= W)
#big M constraints
m.add_constraints_(x[i] <= M[i] * y[i] for i in I)

m.add_constraint_(m.sum(y[i] for i in I) <= K)

#to open at least n loc
m.add_constraint_(m.sum(y[i] for i in I) >= minLoc)
#
m.add_constraints(x[i] >= epsilon * y[i]  for i in I)

if m.solve():
    m.print_solution(print_zeros=True) #to print all the variables
    print(f"The size of {I_names[5]} is {m.solution[x[5]]}")
else:
    print("Not solved: ", m.get_solve_status())
