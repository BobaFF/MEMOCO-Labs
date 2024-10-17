from docplex.mp.model import Model

#Data
#Resources
I = ['rose', 'lily', 'violet'] 
#Products
J = ['Parfume One', 'Parfume Two']

#Parameters
# Availability of resource i in I
D = {
    'rose': 27, 
    'lily': 21, 
    'violet': 9, 
    }
# unit Profit for product j in J
P = {
    'Parfume One': 130, 
    'Parfume Two': 100
    }
# amount of resource i in I required for each unit of product j in J
Q = {
        ('rose', 'Parfume One'): 1.5, 
        ('lily', 'Parfume One'): 1,
        ('violet', 'Parfume One'): 0.3, 
        
        ('rose', 'Parfume Two'): 1, 
        ('lily', 'Parfume Two'): 1,
        ('violet', 'Parfume Two'): 0.5, 
    }


decision_domain = 'integer'

#Data Independent Model
m = Model(name='Optimal Production Mix')

# Variables: amount of product j in J
if decision_domain=='integer':
    x = {j : m.integer_var(name='x({0})'.format(j)) for j in J}
elif decision_domain=='continuous':
    x = {j : m.continuous_var(name='x({0})'.format(j)) for j in J}
else:
    x = {j : m.binary_var(name='x({0})'.format(j)) for j in J}

# O.F.
m.maximize(m.sum(P[j] * x[j] for j in J))

# Contraints
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
        print(m.solution[x[j]], 'liters is the quantity of', j)