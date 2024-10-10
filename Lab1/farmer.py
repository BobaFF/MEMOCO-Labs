from docplex.mp.model import Model

m = Model(name="Farmer model")

#Variables
xT = m.continuous_var(name="hectare of Tomatoes", lb=0, ub=None) #up takes the default value: + infinity
xP =  m.continuous_var(name="hectare of Potatoes", lb=0, ub=None)

#Expression
revenue = 6000 * xT + 7000 * xP

#Objective Function
m.maximize(revenue)

#Contraints
m.add_constraint(xT + xP <= 11, ctname="land availability")
m.add_constraint(7 * xT  <= 70, ctname="tomato seeds availability")
m.add_constraint(3 * xP  <= 18, ctname="potato tubers availability")
m.add_constraint(10 * xT + 20 * xP <= 145, ctname="fertalizer availability")

#m.export_as_lp("farmerTom.lp")

m.solve()

if m.solution == None:
    print("Problems! Status: ", m.get_solve_status())
else:
    print('Solved!')
    m.print_solution()
    m.solution.export('farmerTom_sol')