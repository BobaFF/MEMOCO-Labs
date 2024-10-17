from docplex.mp.model import Model

#Data
#Resources
I = ['T-Shirts', 'bags', 'drawings', 'labels', 'profiles', 'buttons'] 
#Products
J = ['New Shirts', 'New Bags']

#Parameters
# Availability of resource i in I
D = {
    'T-Shirts': 10, 
    'bags': 15, 
    'drawings': 32, 
    'labels': 24,
    'profiles': 40,
    'buttons': 15
    }
# unit Profit for product j in J
P = {
    'New Shirts': 6, 
    'New Bags': 4
    }
# amount of resource i in I required for each unit of product j in J
Q = {
        ('T-Shirts', 'New Shirts'): 1, ('bags', 'New Shirts'): 0, ('drawings', 'New Shirts'): 6, 
        ('labels', 'New Shirts'): 1, ('profiles', 'New Shirts'): 2, ('buttons', 'New Shirts'): 0,
        ('T-Shirts', 'New Bags'): 0, ('bags', 'New Bags'): 1, ('drawings', 'New Bags'): 3, 
        ('labels', 'New Bags'): 2, ('profiles', 'New Bags'): 2, ('buttons', 'New Bags'): 2,
    }

#Data Independent Model
m = Model(name='Optimal Production Mix')

# Variables: amount of product j in J
x = m.integer_var_dict(keys=J, name='Resource x', lb=0, ub=None)

#O.F.
m.maximize(m.sum(P[j] * x[j] for j in J))

#Constraints
m.add_constraints(m.sum(Q[i,j] * x[j] for j in J) <= D[i] for i in I)
    
m.print_information()

#m.export_as_lp('.')

m.solve()

if m.solution == None:
    print('Problems! Status: ', m.get_solve_status())
else:
    print('Solved!')
    m.print_solution()
    for j in J:
        print(m.solution[x[j]], 'is the quantity of', j)