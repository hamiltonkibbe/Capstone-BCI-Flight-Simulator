/*
 *  DEv_Filter.cpp
 *  DEv_filter
 *
 *  Created by Hamilton T Kibbe on 4/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "DEv_Filter.h"

using namespace std;
vector<float>* DEv_Filter::filter(vector<float>* input)
{
	Output.assign(input->size(),0);
	for(unsigned i = 0; i < input->size(); ++i)
	{
		for (unsigned j = 0; j < nCoefficients; ++j)
		{
			if((i-j >= 0) && (i-j < input->size()))
				Output.at(i) += input->at(i-j) * Coefficients.at(j);
		}
	}
	return &Output;
}


void DEv_Filter::train(vector<vector<float> > *sent_seq, vector<vector<float> > *rcvd_seq)
{
	double min[nCoefficients];
	double max[nCoefficients];

	FilterSolver solver(nCoefficients,100,sent_seq,rcvd_seq);

	for(unsigned i = 0; i < nCoefficients; ++i)
	{
		min[i] = -100.0;
		max[i] = 100.0;
	}

	solver.Setup(min,max,stBest1Exp,0.9,1);
	solver.Solve(nCoefficients * 600);

	double *solution = solver.Solution();

	Coefficients.assign(&solution[0],&solution[nCoefficients]);

	cout << "\n\nBest Coefficients:\n";
	for (unsigned i=0;i<Coefficients.size();++i)
		std::cout << "[" << i << "]: " << Coefficients.at(i) << endl;
}

