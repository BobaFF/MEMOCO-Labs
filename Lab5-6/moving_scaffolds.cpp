/**
 * @file moving_scaffolds.cpp
 * By prof. De Giovanni
 * @brief 
 */

#include <cstdio>
#include <iostream>
#include <vector>
#include "cpxmacro.h"

using namespace std;

// error status and messagge buffer (from cpxmacro.h)
int status;
char errmsg[BUF_SIZE];

// data
const int I = 3;
const int J = 3;
const char nameI[I] = { 'A', 'B', 'C' }; // origins
const char nameJ[J] = { '1', '2', '3' }; // destinations
const double D[I] = { 7000.0, 6000.0, 4000.0 }; // Availability (origins)
const double R[J] = { 8000.0, 5000.0, 4000.0 }; // Demand (destinations)
//                         A    B    C
const double C[I][J] = { {9.0, 6.0, 5.0},   // 1
                         {7.0, 4.0, 9.0},   // 2
                         {4.0, 6.0, 3.0} }; // 3
const double K = 10000.0;
const double F = 50.0;
const double N = 4.0;
const double L = 65.0;

///EXTENSION
const double od_cost_max = 7;  ///EXT1: no flow if origin-destination cost > od_cost_max
const double od_cost_low = 6;  ///EXT2: parameter N refers to the max number of trucks on *low-cost* O/D links
								/// sum_{ij : c_ij <= 6} y_ij <= N + z
const int NAME_SIZE = 512;
char name[NAME_SIZE];

/*MAP FOR X, Y and Z VARS: declare maps*/
vector<vector<int> > map_x;	// x_ij ---> map_x[i][j]	
vector<vector<int> > map_y;	// y_ij ---> map_y[i][j]	
int map_z; // z ---> map_z

void setupLP(CEnv env, Prob lp)
{
    ///////////////////////// create variables with "newcols"
    //
    //    status =      CPXnewcols (env, lp, ccnt, obj, lb, ub, xctype, colname);
    //
    // all variables will be created and added (in the *order* they are created) 
    // to an "internal-to-cplex" array (each variable will be identified by the related
    // position, i.e., index in the internal array)
  
    int current_var_position = 0; //keep the position of the next variable in the internal array

	/*MAP FOR x VARS: initial memory allocation for map vector*/
	map_x.resize(I);
	for ( int i = 0 ; i < I ; ++i ) {
		map_x[i].resize(J);
		for ( int j = 0 ; j < J ; ++j ) {
			map_x[i][j] = -1;
		}
	} 
 	
	// add x vars [in the objective function: sum{i,j} C_ij x_ij + ...]
	// Remark: to simplify our life, we create variables one at a time.
	// We may create vectors of obj, lb, ub, xctype and colname to
	// create more variables at a time and gain further efficiency.
	for (int i = 0; i < I; i++)
	{
		for (int j = 0; j < J; j++)
		{
			if ( C[i][j] > od_cost_max ) continue;			///EXT1
			
			char xtype = 'I';
			double lb = 0.0;
			double ub = CPX_INFBOUND; // --///EXT1 with variables fixed to 0.0-- C[i][j] <= od_cost_max ? CPX_INFBOUND : 0.0;
			snprintf(name, NAME_SIZE, "x_%c_%c", nameI[i], nameJ[j]);
			char* xname = (char*)(&name[0]);
			CHECKED_CPX_CALL( CPXnewcols, env, lp, 1   , &(C[i][j]), &lb, &ub, &xtype, &xname );
			/// status =      CPXnewcols (env, lp, ccnt, obj      , lb  , ub, xctype, colname);
			
			map_x[i][j] = current_var_position ++;
		}
	}
	
	/*MAP FOR y VARS: initial memory allocation for map vector*/
	map_y.resize(I);
	for ( int i = 0 ; i < I ; ++i ) {
		map_y[i].resize(J);
		for ( int j = 0 ; j < J ; ++j ) {
			map_y[i][j] = -1;
		}
	} 
	for (int i = 0; i < I; i++)
	{
		for (int j = 0; j < J; j++)
		{
			if ( C[i][j] > od_cost_max ) continue;			///EXT1
			
			char ytype = 'B';
			double lb = 0.0;
			double ub = 1; // --///EXT1 with variables fixed to 0.0-- C[i][j] <= od_cost_max ? CPX_INFBOUND : 0.0;
			snprintf(name, NAME_SIZE, "y_%c_%c", nameI[i], nameJ[j]);
			char* yname = (char*)(&name[0]);
			CHECKED_CPX_CALL( CPXnewcols, env, lp, 1   , &(F), &lb, &ub, &ytype, &yname );
			/// status =      CPXnewcols (env, lp, ccnt, obj      , lb  , ub, xctype, colname);
			
			map_y[i][j] = current_var_position ++;
		}
	}

	
	// add y vars [in o.f.: ... + F sum{ij} y_ij + ... ] 
	//TODO...

	
	// add z var [in o.f.: ... + (L-F) z ]
	{
		char z_type = 'B';
		double obj = L - F;
		double lb = 0.0;
		double ub = 1.0;
		snprintf(name, NAME_SIZE, "z");
		char* zname = (char*)(&name[0]);
		CHECKED_CPX_CALL( CPXnewcols, env, lp, 1   , &(obj), &lb, &ub, &z_type, &zname );
		map_z = current_var_position ++;
	}


	
	///////////////////////// create constraints with "addrows"
	//
	//    status = CPXaddrows (env, lp, colcnt, rowcnt, nzcnt, rhs, sense, rmatbeg, rmatind, rmatval , newcolname, newrowname);
	//
	
	// add request constraints (destinations) [ forall j, sum{i: x_ij exists} x_ij >= R_j ]
	// Remark: to simplify our life, we create one contraint at a time. 
	// We may create vectors of rhs, sense and newrowname, as well as a 
	// sparse matrix representaion (by means of rmatbeg, rmatind, rmatval)
	// to create more contraints at a time and gain further efficiency.
	// Notice that we may even create new variables with suitably prepared
	// arguments, i.e., create all the model with a single call.
	for (int j = 0; j < J; j++)
	{
		std::vector<int> idx;//(I);	rmatind						///EXT1
		std::vector<double> coef;//(I, 1.0);	rmatval			///EXT1
		char sense = 'G';
		for (int i = 0; i < I; i++)
		{
			if ( map_x[i][j] < 0 ) continue;  				///EXT1 - we skip the variables that do not exist
			
			idx.push_back(map_x[i][j]); //i*J + j; // corresponds to variable x_ij - we use the map to retrieve the constraints of the variables
			coef.push_back(1.0);
		}
		int matbeg = 0;
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0     , 1     , idx.size(), &R[j], &sense, &matbeg, &idx[0], &coef[0], NULL      , NULL      );
    	/// status =      CPXaddrows (        env, lp, colcnt, rowcnt, nzcnt     , rhs  , sense , rmatbeg, rmatind, rmatval , newcolname, newrowname);
	}

	// add capacity constraints (origin) [ forall i, sum{j: x_ij exists} x_ij <= D_i ]
	for (int i = 0; i < I; i++)
	{
		std::vector<int> idx;
		std::vector<double> coef;
		char sense = 'G';
		for (int j = 0; j < J; j++)
		{
			if ( map_x[i][j] < 0 ) continue;  				///EXT1
			
			idx.push_back(map_x[i][j]); //i*J + j; // corresponds to variable x_ij
			coef.push_back(1.0);
		}
		int matbeg = 0;
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0     , 1     , idx.size(), &D[i], &sense, &matbeg, &idx[0], &coef[0], NULL      , NULL      );
    	/// status =      CPXaddrows (        env, lp, colcnt, rowcnt, nzcnt     , rhs  , sense , rmatbeg, rmatind, rmatval , newcolname, newrowname);
	}


	// add linking constraints (forall i,j: x_ij and y_ij exist, x_ij - K y_ij <= 0 -- all variables on the left side!!!)
	for (int i = 0; i < I; i++) {
		for (int j = 0; j < J; j++) {
			if (map_x[i][j]  < 0 || map_y[i][j] < 0) continue; 
			
			std::vector<int> idx(2);
			std::vector<double> coef(2);
			char sense = 'L';
			idx[0] = map_x[i][j];
			idx[1] = map_x[i][j];
			coef[0] = 1.0;
			coef[1] = -K;
			double rhs = 0;
			int matbeg = 0;
			CHECKED_CPX_CALL( CPXaddrows, env, lp, 0     , 1     , idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL      , NULL      );
		}
	}


	
	// add counting constraint (sum_ij y_ij - z <= N)
						/// EXT2: sum_{ij : c_ij <= od_cost_low} y_ij - z <= N
	// I need to create a vector of indixes of all the variables and one with the coefficients of all that variables
	std::vector<int> idx;
	std::vector<double> coef;
	for (int i = 0; i < I; i++) {
		for (int j = 0; j < J; j++) {
			if (map_y[i][j] < 0) continue;
			idx.push_back(map_y[i][j]);
			coef.push_back(1.0);
			//now i have added all the indexes of the y_ij variables.
		}
	}
	//now i need to add z.
	idx.push_back(map_z);
	coef.push_back(-1.0); //The coefficeint of z is -1
	char sense = 'L';
	int matbeg = 0; //If we add one constraint it will always be 0.
	CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &N, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);

	/// EXT2: sum_{ij : c_ij <= od_cost_low} y_ij - z <= N
	std::vector<int> idx;
	std::vector<double> coef;
	for (int i = 0; i < I; i++) {
		for (int j = 0; j < J; j++) {
			if (map_y[i][j] < 0 || C[i][j] >= od_cost_low) continue;
			idx.push_back(map_y[i][j]);
			coef.push_back(1.0);
			//now i have added all the indexes of the y_ij variables.
		}
	}
	//now i need to add z.
	idx.push_back(map_z);
	coef.push_back(-1.0); //The coefficeint of z is -1
	char sense = 'L';
	int matbeg = 0; //If we add one constraint it will always be 0.
	CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &N, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);

	
	
	// add condition constraint (y_A2 + y_B2 <= 1)
	//of course it makes sense only if both variables exists, so we need to check it
	if (map_y[0][1] >= 0 && map_y[1][1] >= 0) {
		std::vector<int> idx;
		std::vector<double> coef(2, 1.0);
		idx.push_back(map_y[0][1]);
		idx.push_back(map_y[1][1]);
		char sense = 'L';
		int matbeg = 0; // If we add one constraint it will always be 0.
		double rhs = 1.0;
		CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
	}

	// print (debug)
	CHECKED_CPX_CALL( CPXwriteprob, env, lp, "moving_scaffolds.lp", NULL );
	/// status =      CPXwriteprob (env, lp, "myprob"    , filetype_str);
}


int main (int argc, char const *argv[])
{
	try
	{
		///// init
		DECL_ENV( env );
		DECL_PROB( env, lp );
		
		///// setup LP
		setupLP(env, lp);
		
		///// optimize
		CHECKED_CPX_CALL( CPXmipopt, env, lp );
		
		///// print
		// print objective function value
		double objval;
		CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
		std::cout << "Objval: " << objval << std::endl;
		
		// print value of decision variables
		/// USE:        status =    CPXgetx (env, lp, varVals, fromIdx, toIdx);
		int n = CPXgetnumcols(env, lp);
		std::vector<double> varvals (n);
		int fromIdx = 0;
		int toIdx = n - 1;
		CHECKED_CPX_CALL(CPXgetx, env, lp, &varvals[0], fromIdx, toIdx);
		for (int p = 0; p < n; ++p) {
			std::cout << "var in position "<< p << " has optimal value " << varvals[p] << std::endl;
		}

		double vofz;
		CHECKED_CPX_CALL(CPXgetx, env, lp, &vofz, map_z, map_z);
		std::cout << "We " << (vofz <= 0.1 ? "do not" : "") << "use an addtional constraint" << std::endl;


		// print overall solution information on a .sol file
		CHECKED_CPX_CALL( CPXsolwrite, env, lp, "moving_scaffolds.sol" );
		
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

