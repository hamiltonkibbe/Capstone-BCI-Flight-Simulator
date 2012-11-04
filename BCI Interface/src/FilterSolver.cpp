/*
 *  FilterSolver.cpp
 *  DEv_filter
 *
 *  Created by Hamilton T Kibbe on 4/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "FilterSolver.h"



double FilterSolver::EnergyFunction(double trial[],bool &bAtSolution)
{
	double			result = 0;
	int				seq,
					nSeqs = rcvd_sequence->size(),
					len = rcvd_sequence->at(0).size();
	vector<float>	estimation(len,0);

	seq = count % nSeqs;


	for(unsigned i = 0; i < len; ++i)
	{
		for(unsigned j = 0; j < nDim; ++j)
		{
			if((i-j >= 0) && (i-j < len))
				estimation.at(i) += rcvd_sequence->at(seq).at(i-j) * trial[j];
		}
	}

	for(unsigned i = 0; i < len; ++i)
	{
		result += pow(sent_sequence->at(seq).at(i)-estimation.at(i),2);
	}

	if (count++ % nPop == 0)
	{

		std::cout << count / nPop + 1 << "  " << Energy() << std::endl;
		std::cout << estimation.at(0) << " " << estimation.at(1) << " " << estimation.at(2) << " " << estimation.at(3) << std::endl;
	}


	return result;

}
