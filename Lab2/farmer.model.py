from docplex.mp.model import Model

#Data (can be loaded from db, txt files, ...)
#Resources
I = ['hectare of land', 'tomato seeds', 'potato tubers', 'fertilizer'] 
#Products
J = ['Tomatoes', 'Potatoes']

#Parameters
# Availability of resource i in I
D = {
    'hectare of land': 11, 
    'tomato seeds': 70, 
    'potato tubers': 18, 
    'fertilizer': 145
    }
# unit Profit for product j in J
P = {
    'Tomatoes': 6000, 
    'Potatoes': 7000
    }
# amount of resource i in I required for each unit of product j in J
Q = {
        ('hectare of land', 'Tomatoes'): 1, 
        ('tomato seeds', 'Tomatoes'): 7,
        ('potato tubers', 'Tomatoes'): 0, 
        ('fertilizer', 'Tomatoes'): 10,
        ('hectare of land', 'Potatoes'): 1, 
        ('tomato seeds', 'Potatoes'): 0,
        ('potato tubers', 'Potatoes'): 3, 
        ('fertilizer', 'Potatoes'): 20
    }

#Data Independent Model
m = Model(name='Optimal Production Mix')

# Variables: amount of product j in J
x = {j : m.continuous_var(name='x({0})'.format(j)) for j in J}

# O.F.
m.maximize(m.sum(P[j] * x[j] for j in J))

# Constraints
for i in I:
    m.add_constraint(m.sum(Q[i,j] * x[j] for j in J) <= D[i])
    
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