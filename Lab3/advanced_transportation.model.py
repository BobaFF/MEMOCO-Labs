from docplex.mp.model import Model
import json
import sys

#Read data from a json file
with open(sys.argv[1] if len(sys.argv) == 2 else sys.exit("No input data"), 'r') as file:
    data = json.load(file)

#Data 
#Origins
I = data["Origins"]
#Destinations
J = data["Destinations"]

#Parameters
# capacity of origin i in I
O = {I[i] : data["O_Capacity"][i] for i in range(len(I))}
# request of destionation j in J
D = {J[j] : data["D_Request"][j] for j in range(len(J))}
#unit transportation cost from origin i in I to destination j in J
C = {(I[i],J[j]) : data["Costs"][i][j] for i in range(len(I)) for j in range(len(J))}

#Cost threshold: the pairs considered are only below a certain threshold
cost_threshold_percent = data["cost_threshold_percent"] 
ActiveOD_pairs = [(i,j) for i in I for j in J if C[i,j] <= cost_threshold_percent * max(C[o, d] for o in I for d in J)]

#decision domanin
dd = data["decision_domain"]

#Data Independent Model
m = Model(name="Transportation Model")

#Variables: amount to be transportated from i in I to j in J
#Variables are now created from the Active Pairs dict
if dd=='discrete':
    x = m.integer_var_dict(keys=ActiveOD_pairs, lb=0, ub=None, name="quantity transported")
elif dd=='continuous':
    x = m.continuous_var_dict(keys=ActiveOD_pairs, lb=0, ub=None, name="quantity transported")
else:
    x = m.binary_var_dict(keys=ActiveOD_pairs, name="quantity transported")

# O.F.
m.minimize(m.sum(C[i,j] * x[i,j] for (i,j) in ActiveOD_pairs))

#Contraints
#Constraints are now created using the Active Pairs dict
#C on Destinations
m.add_constraints_(m.sum(x[i,j] for i in I if (i,j) in ActiveOD_pairs) >= D[j] for j in J)
#C on Origins
m.add_constraints_(m.sum(x[i,j] for j in J if (i,j) in ActiveOD_pairs) <= O[i] for i in I)

m.print_information()

m.solve()

if m.solution == None:
    print('Problems! Status: ', m.get_solve_status())
else:
    print('Solved!')
    m.print_solution()

print("Considering additional constraints.")
#Additional Constraint 1
#If the cost of a link is at most LowCost, then the quantity transported on this link should be at least FlowMinOnLink
L = data["LowCost"]
T = data["FlowMinOnLink"]
m.add_constraints_(x[i,j] >= T for (i,j) in ActiveOD_pairs if C[i,j] <= L)

#Additional Constraint 2
#All the origins, except for SpecialOrigin, must deliver MinToSpecialDest to a SpecialDestination
sD = data["SpecialDestination"]
sO = data["SpecialOrigin"] 
min_sD = data["MinToSpecialDest"]
m.add_constraints(x[i, sD] >= min_sD for i in I if (i is not sO) and (i,sD) in ActiveOD_pairs)

#Addtional Constraint 3
# At least a SignificantNumber of origins must supply each destination no less than a SignificantFraction.
N = data["SignificantNumber"]
F = data["SignificantFraction"]
#We need a binary variable y
y = m.binary_var_dict(keys=ActiveOD_pairs, name='y')
#Set y values
for j in J:
    m.add_constraint_(m.sum(y[od] for od in ActiveOD_pairs if od[1] == j) >= N )
m.add_constraints_(x[i,j] >= F * D[j] * y[i,j] for (i,j) in ActiveOD_pairs)

m.print_information()

m.solve()

if m.solution == None:
    print('Problems! Status: ', m.get_solve_status())
else:
    print('Solved!')
    m.print_solution()
    
