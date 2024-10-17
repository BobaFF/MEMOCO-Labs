from docplex.mp.model import Model

#Data
#Resources
I = ["Vegetables", "Meat", "Fruit"] 
#Products
J = ['Protein', "Iron", "Calcium"]

#Parameters
# unit Cost of resource i in I
C = {
    "Vegetables": 4, 
    "Meat": 10, 
    "Fruit": 7, 
    }

# Requested amount of j in J
R = {
    'Protein':20,
    "Iron":30,
    "Calcium":10
    }
# Amount of request j in J satisfied by one unit of resource i in I
A = {
        ("Vegetables", 'Protein'): 5, ("Vegetables", 'Iron'): 6, ("Vegetables", 'Calcium'): 5,
        ("Meat", 'Protein'): 15, ("Meat", 'Iron'): 10, ("Meat", 'Calcium'): 3, 
        ("Fruit", 'Protein'): 4, ("Fruit", 'Iron'): 5, ("Fruit", 'Calcium'): 12, 
    }

#Data Independent Model
m = Model(name='Minimum Cost Covering')

# Variables domain
decision_domain = "continuous"

# Variables: amount of resource i in I
if decision_domain=="integer":
    x = m.integer_var_dict(keys=I, name='Resources x', lb=0, ub=None)
elif decision_domain=="continuous":
    x = m.continuous_var_dict(keys=I, name='Resources x', lb=0, ub=None)
else:
    x = m.binary_var_dict(keys=I, name='Resources x', lb=0, ub=None)

#O.F.
m.minimize(m.sum(C[i] * x[i] for i in I))

#Constraints
m.add_constraints(m.sum(A[i,j] * x[i] for i in I) >= R[j] for j in J)
    
m.print_information()

m.solve()

if m.solution == None:
    print("Problems! Status: ", m.get_solve_status())
else:
    print('Solved!')
    m.print_solution()
    for i in I:
        print(m.solution[x[i]], "is the quantity of", i)