#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "../Lab5-6/cpxmacro.h"

using namespace std;
//DATA
const int I = 3;
const int J = 2;
const string resources[I] = {"Rose", "Lily", "Violet"};
const string products[J] = {"One", "Two"};

const double D[I] = {27.0, 21.0, 9.0};
const double P[J] = {130.0, 100.0};
const double Q[I][J] = {
    {1.5, 1},
    {1, 1},
    {0.3, 0.5}
};

// error status and messagge buffer (from cpxmacro.h)
int status;
char errmsg[BUF_SIZE];



void setupLP(CEnv env, Prob lp) {
    //Variables
    //1. Prepare data
    int ccnt = J;
    vector<double> lb(ccnt, 0.0);
    vector<double> ub(ccnt, CPX_INFBOUND);
    vector<char> types(ccnt, ('C'));
    vector<string> strnames(ccnt);
    vector<char*> colnames(ccnt);
    for (int j = 0; j < ccnt; ++j) {
        strnames[j]= "Parfume_"+ products[j];
        colnames[j] = const_cast<char *>(strnames[j].c_str());
        
    }
    //2. Insert data
    CHECKED_CPX_CALL(CPXnewcols, env, lp, ccnt, &(P[0]), lb.data(), ub.data(), types.data(), colnames.data());

    //Constraints
    //1. Prepare data
    int colcnt = 0;
    int rowcnt = I;
    int nzcnt = I*J;
    vector<char> senses(I, 'L');
    vector<int> rmatbeg(I);
    vector<int> rmatind(I*J);
    vector<double> rmatval(I*J);
    vector<char*>rownames(I);
    int idx = 0;
    for (int i = 0; i < I; i++){
        rmatbeg[i] = idx; //0 
        for (int j = 0; j < J; j++) {
            rmatind[idx] = j; //0
            rmatval[idx] = Q[i][j];   
            idx++;
        }
        rownames[i] = const_cast<char *>(resources[i].c_str());
    }

    //2. Insert data
    CHECKED_CPX_CALL(CPXaddrows, env, lp, colcnt, rowcnt, nzcnt, &(D[0]), senses.data(), rmatbeg.data(), rmatind.data(), rmatval.data(), NULL, rownames.data());

    // print (debug)
	CHECKED_CPX_CALL( CPXwriteprob, env, lp, "parfume.lp", NULL );
	/// status =      CPXwriteprob (env, lp, "myprob"    , filetype_str);
}

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
		for (int p = 0; p < n; ++p) {
			std::cout << "var in position "<< p << " has optimal value " << varvals[p] << std::endl;
		}

		// print overall solution information on a .sol file
		CHECKED_CPX_CALL( CPXsolwrite, env, lp, "parfume.sol" );
		
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