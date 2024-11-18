#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "../Lab5-6/cpxmacro.h"
using namespace std;


// Data
    const int I = 5;
    const char sites[I] = {'A', 'B', 'C', 'D', 'E'};
    const int J = 6;
    const char areas[J] = {'1', '2', '3', '4', '5', '6'};
    const double sgm[I][J] = {
        {10, 20, 16, 25, 0, 10},
        {0, 12, 18, 23, 11, 6},
        {21, 8, 5, 6, 23, 19},
        {16, 15, 15, 8, 14, 18},
        {21, 13, 13, 17, 18, 22}};
    const double T = 18; // minimum signal level required
    const double N = 2;  // maximum number of non-interfering signals
    int M[J];

    const int NAME_SIZE = 512;
    char name[NAME_SIZE];

void setupData() {
    for (int j = 0; j < J; j++)
    {
        int nonInterferingCount = 0;
        for (int i = 0; i < I; i++)
        {
            if (sgm[i][j] >= T)
                nonInterferingCount++;
        }
        M[j] = nonInterferingCount;
    } 
}

void setupLP(CEnv env, Prob lp) {
    setupData();
    int current_var_position = 0;
    //VARIABLES
    //Var z
    //1. Prepare data
    vector<double> obj (J, 1.0);
    vector<double> lb(J, 0.0);
    vector<double> ub(J, CPX_INFBOUND);
    vector<char> ztypes(J, 'B');
    vector <string> zNames(J);
    vector <char*> zPointerNames(J);
    for (int j = 0; j < J; j++) {
        zNames[j] = "z_" + string(1, areas[j]);
        zPointerNames[j] = &(zNames[j][0]);
    }
    //2. Insert data
    CHECKED_CPX_CALL(CPXnewcols, env, lp, J, &(obj[0]), (&lb[0]), (&ub[0]), (&ztypes[0]), (&zPointerNames[0]));

    {//Var x
    //1. Prepare data
    vector<double> obj (I, 0.0);
    vector<double> lb(I, 0.0);
    vector<double> ub(I, CPX_INFBOUND);
    vector<char> xtypes(I, 'B');
    vector <string> xNames(I);
    vector <char*> xPointerNames(I);
    for (int i = 0; i < I; i++) {
        xNames[i] = "x_" + string(1, sites[i]);
        xPointerNames[i] = &(xNames[i][0]);
    }
    //2. Insert data
    CHECKED_CPX_CALL(CPXnewcols, env, lp, J, &(obj[0]), (&lb[0]), (&ub[0]), (&xtypes[0]), (&xPointerNames[0]));
    }

    //CONSTRAINTS
    //sum x_i >= 1 z_j for all j in J
    {
        int rowcnt = 0;
        int nzcnt = 0;
        vector <double> rhs;
        vector <char> senses;
        vector <int> rmatbeg;
        vector <int> rmatind;
        vector <double> rmatval;
        for (int j = 0; j < J; j++) {
            int x_counter = 0;
            rmatbeg.push_back(nzcnt);
            for (int i = 0; i < I; i++ ) {
                if (sgm[i][j] < T) continue;
                rmatval.push_back(1.0);
                rmatind.push_back(J + i);
                x_counter ++;
                nzcnt ++;
            }
            rmatind.push_back(j);
            rmatval.push_back(-1.0);
            senses.push_back('G');
            rhs.push_back(0.0);
            rowcnt ++;
            nzcnt ++;
        }
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, rowcnt, nzcnt, &(rhs[0]) ,&(senses[0]), &(rmatbeg[0]), &(rmatind[0]), &(rmatval[0]), NULL, NULL);}

        //sum x_i >=  N + M_j(1 - z_j) for all j in J
    {
        int rowcnt = 0;
        int nzcnt = 0;
        vector <double> rhs;
        vector <char> senses;
        vector <int> rmatbeg;
        vector <int> rmatind;
        vector <double> rmatval;
        for (int j = 0; j < J; j++) {
            int x_counter = 0;
            rmatbeg.push_back(nzcnt);
            for (int i = 0; i < I; i++ ) {
                if (sgm[i][j] < T) continue;
                rmatval.push_back(1.0);
                rmatind.push_back(J + i);
                x_counter ++;
                nzcnt ++;
            }

            rmatind.push_back(j);
            rmatval.push_back(M[j]);

            senses.push_back('L');
            rhs.push_back(N + M[j]);
            rowcnt ++;
            nzcnt ++;
        }
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, rowcnt, nzcnt, &(rhs[0]) ,&(senses[0]), &(rmatbeg[0]), &(rmatind[0]), &(rmatval[0]), NULL, NULL);}

    //xE <= xD
    {double rhs[1] = {0};
    char senses[1] = {'L'};
    int idx[2] = {J + 4, J + 3};
    double coef[2] = {1.0, -1.0};
    int matbeg = 0;
    string name = "additional constraint";
    vector <char*> name_pointer(1, &(name[0]));
    CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, 2, &(rhs[0]) ,&(senses[0]), &matbeg, &(idx[0]), &(coef[0]), NULL, &(name_pointer[0]));}


    	// print (debug)
	CHECKED_CPX_CALL( CPXwriteprob, env, lp, "antennas.lp", NULL );
	/// status =      CPXwriteprob (env, lp, "myprob"    , filetype_str);
    }


// error status and messagge buffer (from cpxmacro.h)
int status;
char errmsg[BUF_SIZE];

int main (int argc, char const *argv[])
{
	try
	{
		///// init
		DECL_ENV( env );
		DECL_PROB( env, lp );

        CHECKED_CPX_CALL( CPXchgobjsen, env, lp, CPX_MAX );
		
		///// setup LP
		setupLP(env, lp);
        ///// optimize
		CHECKED_CPX_CALL( CPXmipopt, env, lp );
		
		///// print
		// print objective function value
		double objval;
		CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
		std::cout << "Objval: " << objval << std::endl;
		
		int n = CPXgetnumcols(env, lp);
		std::vector<double> varvals (n);
		int fromIdx = 0;
		int toIdx = n - 1;
		CHECKED_CPX_CALL(CPXgetx, env, lp, &varvals[0], fromIdx, toIdx);
		for (int p = 0; p < toIdx; ++p) {
			std::cout << "var in position "<< p << " has optimal value " << varvals[p] << std::endl;
		}

		// print overall solution information on a .sol file
		CHECKED_CPX_CALL( CPXsolwrite, env, lp, "antennas.sol" );
		
		///// free memory
		CPXfreeprob(env, &lp);
		CPXcloseCPLEX(&env);

		char towait; std::cin >> towait;
    }
    catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}
	return 0;
}