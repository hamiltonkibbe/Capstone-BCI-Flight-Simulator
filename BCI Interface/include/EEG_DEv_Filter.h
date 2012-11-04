/*
 *  EEG_DEv_Filter.h
 *  DEv_filter
 *
 *  Created by Hamilton T Kibbe on 4/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef EEG_DEV_FILTER_H_
#define EEG_DEV_FILTER_H_

#include <fstream>
#include <iterator>

#include "DEv_Filter.h"
#include "Sequence_Wrapper.h"

class EEG_DEv_Filter
{
public:

	EEG_DEv_Filter(int order);

	~EEG_DEv_Filter();

	void train(Sequence_Wrapper *sequences);

	void load(const char* fname);

	void save(const char* fname);

	vector<float> *filter(vector<float> *input);

private:

	DEv_Filter *Filter;

};

#endif
