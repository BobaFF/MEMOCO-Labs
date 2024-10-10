from docplex.mp.model import Model

m = Model(name="Diet model")

#Variables
xV = m.continuous_var(name="Kg of vegetables", lb=0, ub=None) 
xM = m.continuous_var(name="Kg of meat", lb=0, ub=None)
xF = m.continuous_var(name="Kg of fruit", lb=0, ub=None)

#Expression
revenue = 4 * xV + 10 * xM + 7 * xF

#Objective Function
m.minimize(revenue)

#Contraints
m.add_constraint(5 * xV + 15 * xM + 4 * xF >= 20, ctname="Proteins minimum supply")
m.add_constraint(6 * xV + 10 * xM + 5 * xF >= 30, ctname="Iron minimum supply")
m.add_constraint(5 * xV + 3 * xM + 12 * xF >= 10, ctname="Proteins minimum supply")

m.solve()

if m.solution == None:
    print("Problems! Status: ", m.get_solve_status())
else:
    print("Solved!")
    m.print_solution()