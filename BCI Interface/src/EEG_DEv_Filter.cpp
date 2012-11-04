/*
 *  EEG_DEv_Filter.cpp
 *  DEv_filter
 *
 *  Created by Hamilton T Kibbe on 4/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "EEG_DEv_Filter.h"

EEG_DEv_Filter::EEG_DEv_Filter(int order)
{
	Filter = new DEv_Filter(order-1);
}

EEG_DEv_Filter::~EEG_DEv_Filter()
{
	delete Filter;
}

vector<float>* EEG_DEv_Filter::filter(vector<float>* input)
{
	return Filter->filter(input);
}

void EEG_DEv_Filter::train(Sequence_Wrapper *sequences)
{
	unsigned nSequences = sequences->number();

	vector< vector<float> > sent_sequences(nSequences),
							rcvd_sequences(nSequences);


	for(unsigned i = 0; i < nSequences; ++i)
	{
	    Sequence * seq = sequences->at(i);
		sent_sequences.at(i).assign(seq->getInterpSequence()->begin(),seq->getInterpSequence()->end());
		rcvd_sequences.at(i).assign(seq->get_template()->at(0).begin(),seq->get_template()->at(0).end());
	}

	Filter->train(&sent_sequences,&rcvd_sequences);
    save("../res/filter.coeffs");
}



void EEG_DEv_Filter::load(const char* fname)
{
    ifstream ifs(fname);
    istream_iterator<double> in(ifs);
    vector<double> coeffs(in,istream_iterator<double>());

    Filter->set_Coefficients(&coeffs);

    ifs.close();
}



void EEG_DEv_Filter::save(const char* fname)
{
    ofstream ofs(fname);

    vector<double>::iterator it;
    for(it = Filter->get_Coefficients()->begin(); it < Filter->get_Coefficients()->end(); it++)
    {
        ofs << *it << endl;
    }

    ofs.close();
}


