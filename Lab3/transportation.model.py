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

#decision domanin
dd = data["decision_domain"]

#Data Independent Model
m = Model(name="Transportation Model")

#Variables: amount to be transportated from i in I to j in J
if dd=='discrete':
    x = m.integer_var_matrix(keys1=I, keys2=J, lb=0, ub=None, name="quantity transported")
elif dd=='continuous':
    x = m.continuous_var_matrix(keys1=I, keys2=J, lb=0, ub=None, name="quantity transported")
else:
    x = m.binary_var_matrix(keys1=I, keys2=J, name="quantity transported")
    
# O.F.
m.minimize(m.sum(C[i,j] * x[i,j] for i in I for j in J))

#Contraints
#C on Destinations
m.add_constraints_(m.sum(x[i,j] for i in I) >= D[j] for j in J)
#C on Origins
m.add_constraints_(m.sum(x[i,j] for j in J) <= O[i] for i in I)

m.print_information()

m.solve()

if m.solution == None:
    print('Problems! Status: ', m.get_solve_status())
else:
    print('Solved!')
    m.print_solution()