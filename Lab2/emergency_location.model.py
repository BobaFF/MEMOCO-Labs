from docplex.mp.model import Model

#Resources
I = ['Loc. 1', 'Loc. 2', 'Loc. 3', 'Loc. 4', 'Loc. 5', 'Loc. 6']
#Products
J = ['Zone 1', 'Zone 2', 'Zone 3', 'Zone 4', 'Zone 5', 'Zone 6']

#Parameters
# unit cost of resource i in I
C = {i : 1 for i in I} #{'Loc. 1': 1, 'Loc. 2': 1, ...}

# Requested amount of j in J
R = {j : 1 for j in J} #{'Zone 1': 1, 'Zone 2': 1, ...}

# amount of request j in J satisfied by one unit of resource i in I
A = {
    ('Loc. 1', 'Zone 1'): 1, ('Loc. 1', 'Zone 2'): 1, ('Loc. 1', 'Zone 3'): 0, 
    ('Loc. 1', 'Zone 4'): 0, ('Loc. 1', 'Zone 5'): 0, ('Loc. 1', 'Zone 6'): 0,
    
    ('Loc. 2', 'Zone 1'): 1, ('Loc. 2', 'Zone 2'): 1, ('Loc. 2', 'Zone 3'): 1, 
    ('Loc. 2', 'Zone 4'): 0, ('Loc. 2', 'Zone 5'): 0, ('Loc. 2', 'Zone 6'): 1,
    
    ('Loc. 3', 'Zone 1'): 0, ('Loc. 3', 'Zone 2'): 0, ('Loc. 3', 'Zone 3'): 1, 
    ('Loc. 3', 'Zone 4'): 1, ('Loc. 3', 'Zone 5'): 0, ('Loc. 3', 'Zone 6'): 0,
    
    ('Loc. 4', 'Zone 1'): 0, ('Loc. 4', 'Zone 2'): 0, ('Loc. 4', 'Zone 3'): 1, 
    ('Loc. 4', 'Zone 4'): 1, ('Loc. 4', 'Zone 5'): 1, ('Loc. 4', 'Zone 6'): 0,
    
    ('Loc. 5', 'Zone 1'): 0, ('Loc. 5', 'Zone 2'): 0, ('Loc. 5', 'Zone 3'): 0, 
    ('Loc. 5', 'Zone 4'): 1, ('Loc. 5', 'Zone 5'): 1, ('Loc. 5', 'Zone 6'): 1,
    
    ('Loc. 6', 'Zone 1'): 0, ('Loc. 6', 'Zone 2'): 1, ('Loc. 6', 'Zone 3'): 0, 
    ('Loc. 6', 'Zone 4'): 0, ('Loc. 6', 'Zone 5'): 1, ('Loc. 6', 'Zone 6'): 1,
    }

decision_domain = "binary"

#Data Independent Model
m = Model(name='Minimum Cost Covering')

# Variables: amount of resource i in I
if decision_domain=="integer":
    x = m.integer_var_dict(keys=I, name='Resources x', lb=0, ub=None)
elif decision_domain=="continuous":
    x = m.continuous_var_dict(keys=I, name='Resources x', lb=0, ub=None)
else:
    x = m.binary_var_dict(keys=I, name='Resources x', lb=0, ub=None)
    
m.minimize(m.sum(C[i] * x[i] for i in I))

m.add_constraints(m.sum(A[i,j] * x[i] for i in I) >= R[j] for j in J)
    
m.print_information()

m.solve()

if m.solution == None:
    print("Problems! Status: ", m.get_solve_status())
else:
    print('Solved!')
    m.print_solution()
    for i in I:
        print(i, "is", "closed" if m.solution[x[i]] == 0 else "open")