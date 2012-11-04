/*
 *  FilterSolver.h
 *  DEv_filter
 *
 *  Created by Hamilton T Kibbe on 4/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef FILTERSOLVER_H_
#define FILTERSOLVER_H_

#include <cmath>
#include <iostream>
#include <vector>
#include "DESolver.h"

using std::vector;

class FilterSolver
: public DESolver
{
public:
	FilterSolver(int coefficients,int pop, vector<vector<float> > *sent_seq, vector<vector<float> >*rcvd_seq)
	: DESolver(coefficients,pop), count(0) , sent_sequence(sent_seq), rcvd_sequence(rcvd_seq){;}

	double EnergyFunction(double trial[],bool &bAtSolution);

private:
	int						count;
	vector<vector<float> >	*sent_sequence,
							*rcvd_sequence;
};

#endif
