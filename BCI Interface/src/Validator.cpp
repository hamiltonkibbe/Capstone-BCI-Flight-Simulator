/**********************************************************************

    Validator.cpp
    Leave-one-out cross-validatation class for calibration statistics

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Validator.h"



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Validator::Validator
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Validator::Validator(){}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Validator::~Validator
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Validator::~Validator()
{
	//if (accuracy != NULL)
	//delete accuracy;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Validator::set_sequences
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Validator::set_sequences(Sequence_Wrapper* seq)
{
    sequences = seq;
    for(unsigned int i = 0; i < NUMBER_OF_CHANNELS; ++i)
    {
        buffers[i].resize(sequences->length()*DETECTION_PERIODS);
    }
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Validator::x_validate
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float* Validator::x_validate(unsigned int channel)
{


	unsigned int	seq_index,
                    period_index;

	unsigned int    correct_guesses = 0;
	unsigned int    sequence_count = sequences->number();
	unsigned int    correct_guesses_seq[sequence_count];
	float           *accuracy = new float[sequence_count + 1];

    Sequence        *seq;
    vector < vector<float> >    var_data;	//channel->index
    vector < vector<float> >    *temp_template,
                                *temp_data;

    vector< vector < vector<float> > >  var_template(sequence_count,vector<vector<float> >(PERIODS_TO_AVERAGE,vector<float>(0)));	// Sequence->Channel->Index


	for(seq_index = 0; seq_index < sequence_count; ++seq_index)
		correct_guesses_seq[seq_index] = 0;


	for(period_index = 0; period_index < PERIODS_TO_AVERAGE; ++period_index)
	{
        // Create matrix of Sequence->Channel->Data out of
		for(seq_index = 0; seq_index < sequence_count; ++seq_index)
		{
			seq = sequences->at(seq_index);
			temp_template = seq->get_template();
			var_template.at(seq_index).assign(temp_template->begin(),temp_template->end());
		}


		for(seq_index = 0; seq_index < sequence_count; ++seq_index)
		{
		    seq = sequences->at(seq_index);
		    temp_data = leave_one_out_data(seq,period_index);
            var_data.assign(temp_data->begin(), temp_data->end());

			if (detect(&var_template,&var_data,channel) == seq_index+1)
			{
				++correct_guesses;
				++correct_guesses_seq[seq_index];
			}
			//delete temp_template;
		}
	}

	for(seq_index = 0; seq_index < sequence_count; ++seq_index)
	{
		accuracy[seq_index] = (float)correct_guesses_seq[seq_index] / (1.0 * PERIODS_TO_AVERAGE);
	}

	accuracy[sequence_count] = (float)correct_guesses/(1.0*PERIODS_TO_AVERAGE * sequence_count);

	return accuracy;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Validator::detect
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int Validator::detect(vector<vector<vector<float> > > const* templates, vector<vector<float> > const* data, int channel)
{

	unsigned int    seq_index,
                    data_index,
                    template_index,
                    buffer_index,
                    period_index;
	int             guess = 0;
	long double     max = 0;
	long double     accumulator = 0;

    const float     *channel_data = &data->at(channel).at(0);
    unsigned int    sequence_length = sequences->length();


    for(period_index = 0; period_index < DETECTION_PERIODS; ++period_index)
    {
        // Add DETECTION_PERIODS periods of data to buffers
        for(data_index = 0; data_index< sequences->number(); ++data_index)
        {
            buffers[channel] << channel_data[data_index];
        }
    }


	for(seq_index = 0; seq_index < sequences->number(); ++seq_index)
	{
	    buffer_index = 0;
		accumulator = 0;

		for(data_index = 0; data_index < sequence_length * DETECTION_PERIODS; ++data_index)
		{
		    // Wrap template index
            template_index = data_index;
			while (template_index >= sequence_length) template_index -= sequence_length;

			accumulator += templates->at(seq_index).at(channel).at(template_index) * buffers[channel].at_n_plus(++buffer_index);
		}

		if (abs(accumulator) > max)
		{
			max = abs(accumulator);
			guess = seq_index + 1;
		}
	}

	return guess;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Validator::leave_one_out_data
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
vector < vector<float> >* Validator::leave_one_out_data(Sequence *  seq, unsigned  int period)
{
	vector<vector<vector<float> > >* raw_data = seq->get_raw_template_data();

    // return the data for the selected period
	return &raw_data->at(period);
}
