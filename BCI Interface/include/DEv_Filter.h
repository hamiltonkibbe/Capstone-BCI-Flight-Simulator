/*
 *  DEv_Filter.h
 *  DEv_filter
 *
 *  Created by Hamilton T Kibbe on 4/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef DEV_FILTER_H_
#define DEV_FILTER_H_

#include <iostream>
#include <vector>
#include "FilterSolver.h"


class DEv_Filter
{
public:

	DEv_Filter(int nCoeffs): nCoefficients(nCoeffs){;}

	~DEv_Filter(){;}

	vector<float>* filter(vector<float>* input);

	void train(vector<vector<float> >* sent_seq, vector<vector<float> >* rcvd_seq);

    void set_Coefficients(vector<double>* coeff){Coefficients.assign(coeff->begin(),coeff->end());}

    vector<double> *get_Coefficients() { return &Coefficients;}

private:

	int nCoefficients;

	vector<double>	Coefficients;
	vector<float>	Output;
};

#endif
