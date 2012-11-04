/**********************************************************************

    Template.cpp
    Template class for EEG data

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Template.h"


using namespace std;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Template::Template
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Template::Template()
{	count = 0;
	length = 0;

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Template::Template
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Template::Template(int ch, int len)
{
    count = 0;
	length = 0;
	vector < vector<float> > temp(ch,vector<float>(len,0.0));	//Period->ch->data
	vector< vector< vector<float> > > temp_db(PERIODS_TO_AVERAGE,vector<vector<float> >(NUMBER_OF_CHANNELS,vector<float>(len,0.0)));
	data = temp;
	raw_data = temp_db;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Template::~Template
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Template::~Template()
{

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Template::set_data
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Template::set_data(const vector < vector<float> > seq_templates)
{
	data = seq_templates;
	count = 1;

	normalize_rms();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Template::add_data
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Template::add_data(vector< vector<float> > seq_templates)
{
	int ch_index,
		data_index;

	length = seq_templates[0].size();

	{
		for(data_index = 0; data_index < length; ++data_index)
		{
			for(ch_index = 0; ch_index < NUMBER_OF_CHANNELS; ++ch_index)
			{
			    // Add data to raw data template
				raw_data.at(count).at(ch_index).at(data_index) += seq_templates.at(ch_index).at(data_index);
			}
		}
		++count;
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Template::generate_template
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Template::generate_template()
{
	if (!count)
	{
		//ERROR
	}

	int ch_index,
		data_index,
		period_index;

    vector<float> sorted;
	vector< vector<long double> > acc(NUMBER_OF_CHANNELS,vector<long double>(length,0));


    for (ch_index = 0; ch_index < NUMBER_OF_CHANNELS; ++ch_index)
    {

        for (data_index = 0; data_index < length; ++data_index)
        {

            for (period_index = 0; period_index < count; ++ period_index)
                sorted.push_back(raw_data.at(period_index).at(ch_index).at(data_index));

            sort(sorted.begin(),sorted.end());

            for(unsigned int i = THROWAWAY; i < (unsigned)(count - THROWAWAY); ++i)
                acc.at(ch_index).at(data_index) += sorted.at(i);

            sorted.clear();

        }
    }


	// Average data
	for(ch_index = 0; ch_index < NUMBER_OF_CHANNELS; ++ch_index)
	{
		for(data_index = 0; data_index < length; ++data_index)
		{
			data[ch_index][data_index] = acc[ch_index][data_index]/(count - 2 * THROWAWAY);
		}
	}

	normalize_rms();
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Template::normalize_rms
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Template::normalize_rms()
{
    double norm_energy = 1.0;
    for(int ch = 0; ch != NUMBER_OF_CHANNELS; ++ch)
    {
        double energy = 0;
        for(unsigned int i = 0; i != data[ch].size(); ++i)
        {
            energy += pow(data[ch][i], 2);
        }
        double factor = sqrt(norm_energy/energy);
        for(unsigned int i = 0; i != data[ch].size(); ++i)
        {
            data[ch][i] *= factor;
        }
    }
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Template::reset
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Template::reset()
{
	count = 0;
	length = 0;
}



