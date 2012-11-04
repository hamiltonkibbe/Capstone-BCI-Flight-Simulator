/**********************************************************************

    Detector.cpp
    Detector class

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Detector.h"




using namespace std;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Detector::Detector
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Detector::Detector()
{
	// Local variables
	source = NULL;
	sysParams = NULL;
	sequences = NULL;
	fg_interface = new Commands();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Detector::~Detector
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Detector::~Detector()
{
    delete fg_interface;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Detector::setSequences
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Detector::setSequences(Sequence_Wrapper *  argSequences)
{

	sequences = argSequences;

    // Resize circular buffers to DETECTION_PERIODS * sequence length
	for(unsigned int i = 0; i < NUMBER_OF_CHANNELS; ++i)
	{
		buffers[i].resize(sequences->length()*DETECTION_PERIODS);
	}
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Detector::run
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Detector::run()
{
	// Bring globals into scope
	extern GladeXML * gui;


	// Local Variables
	int                     channelIdx = 0;
	bool                    pressed = 1;

	vector<detector_vote>   votes(NUMBER_OF_CHANNELS);
	vector< vector<float> > temp_data;
    EEG_DEv_Filter filter(101);
    filter.train(sequences);
   // filter.load("../res/filter.coeffs");

	// Do as long as the button is pressed
	do
	{

		temp_data = get_one_period();   // Get one period of data, starting at a trigger pulse



		// Get vote for each channel
		for(channelIdx = 0; channelIdx < NUMBER_OF_CHANNELS; ++channelIdx)
		{
            // Run detector method on data for each channel
            votes.at(channelIdx) = detect_sequence(&filter.filter(&temp_data.at(0))->at(0),0);
			//votes.at(channelIdx) = detect_sequence(&temp_data.at(channelIdx).at(0), channelIdx);
        }

        // Detection callback
        detected(votes.at(0).sequence);

        // Check button
        gdk_threads_enter();
	    pressed = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(gui, "detect_button")));
        gdk_threads_leave();

	} while (pressed);

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Detector::detect_sequence
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
detector_vote Detector::detect_sequence(float* data, int channel)
{

	detector_vote vote;

	unsigned int	seq_number,
                    data_index,
                    template_index,
                    buffer_index,
                    seq_guess;

	long double     accumulator;
	long double     max = 0;

    char            guess_str[25];

    vector<long double> max_vector;

    // Load thresholds file
    ifstream thresholds_file("../res/thresholds.txt");
    istream_iterator<double> th_iterator(thresholds_file);
    vector<double> thresholds(th_iterator, istream_iterator<double>());


	unsigned int    sequence_length = sequences->length();

	for(data_index = 0; data_index < sequence_length; ++data_index)
	{
		buffers[channel] << data[data_index];       // Add the new data to the circular buffer
	}



    // Run correlation against each sequence
	for (seq_number = 0; seq_number < sequences->number(); ++seq_number)
	{

	    // Reset indices and accumulator
		buffer_index = 0;
		accumulator = 0;

        // Get the next sequence
		Sequence *seq = sequences->at(seq_number);

        // Loop through entire circular buffer and add
		for(data_index = 0; data_index < sequence_length * DETECTION_PERIODS; ++data_index)
		{
		    // Get index for template
			template_index = data_index;
			while (template_index >= sequence_length) template_index -= sequence_length;

            // Add correlation values to accumulator
			//accumulator += seq->get_template_data(channel,template_index) * buffers[channel].at_n_plus(++buffer_index);     // pre-increment buffer index as it
            accumulator += seq->getInterpSequence()->at(template_index);                                                                                                               // maintains reference to the last
                                                                                                                            // element added, we want to start at
                                                                                                                            // the beggining, viz. a trigger pulse
		}

        // Keep the maximum correlation value
        // and a reference to the current sequence
		if(accumulator > max)
		{
			max = accumulator;
			vote.sequence = seq_number + 1; // 1-based for output
			vote.max = max;
		}

		max_vector.push_back(abs(accumulator));

	}

    // Detect invalid votes
	if (vote.sequence > sequences->number())
	{
		vote.sequence =  0;
		vote.max = 0;
	}

	seq_guess = vote.sequence - 1;

	for(unsigned i = 0; i < sequences->number(); ++i)
	{
        if (i != seq_guess)
        {
            if(vote.max < thresholds.at(seq_guess) * max_vector.at(i)) vote.sequence = 0;
        }
	}

	cout << "Ch" << channel+1 << " vote: " << vote.sequence << " max: " << max << endl;
/*
    if (vote.sequence)
    {
        sprintf(guess_str, "Sequence: %i  Max: %f",vote.sequence,(double)max);
        gdk_threads_enter();    // Get GDK lock
        GUESS(guess_str);
        gdk_threads_leave();    // Release GDK lock
    }
*/

	return vote;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Detector::get_one_period
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
vector< vector<float> > Detector::get_one_period()
{
	unsigned int    ch_index,
                    norm_index;
    int             trigger_index;
	bool            done = 0;
	float           *channel_data;
	ChannelData     *cd, *cd_orig;

	vector< vector<float> > data(NUMBER_OF_CHANNELS,vector<float>(0));

	unsigned int block_size = sysParams->getBlockSize();
	unsigned int period_length = sequences->length();


	do	// Wait for a trigger pulse
	{
	    // Get block of data
	 	cd_orig = source->getData();
	 	if(filter)
	 	{
            cd =  new FilteredChannelData(cd_orig, filter);
	 	}
	 	else
	 	{
	 	    cd = cd_orig;
	 	}

	 	// Find trigger index
		trigger_index = find_trigger_index((float*)cd->getDataForChannel(TRIGGER_CHANNEL));
		if(trigger_index == -1 && filter)
            delete cd;

    } while (trigger_index == -1);


	for(ch_index = 0; ch_index < NUMBER_OF_CHANNELS; ++ch_index)
	{
        // Get data for each channel from data block
		channel_data = (float*)cd->getDataForChannel(ch_index + 1);

		// Start at trigger index and loop through the rest of the data
		for(norm_index = trigger_index; norm_index < block_size; ++norm_index)
		{
		    // Add each sample to the output matrix
			data[ch_index].push_back(channel_data[norm_index]);

            // check to see if we have a whole period
			if(data[ch_index].size() == period_length)
			{
				done = 1;
				break;
			}
		}
	}




	while (!done)	// Fill in rest of period
	{
	    if(filter)
            delete cd;
        // Get next block of data
        cd_orig = source->getData();
        if(filter)
	 	{
            cd =  new FilteredChannelData(cd_orig, filter);
	 	}
	 	else
	 	{
	 	    cd = cd_orig;
	 	}


		for(ch_index = 0; ch_index < NUMBER_OF_CHANNELS; ++ch_index)
		{
            // Get data for each channel from data block
			channel_data = (float*)cd->getDataForChannel(ch_index + 1);

			for(norm_index = 0; norm_index < block_size; ++norm_index)
			{
			    // Add each sample to the output matrix
				data[ch_index].push_back(channel_data[norm_index]);

                // Check to see if we have a whole period
				if(data[ch_index].size() == period_length)
				{
					done = 1;
					break;
				}
			}
		}
	}
	if(filter)
            delete cd;

	return data;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Detector::find_trigger_index
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Detector::find_trigger_index(float* data)
{
	// Local Variables
	unsigned int    index;
	unsigned int    blockSize = sysParams->getBlockSize();


	for(index=1; index < blockSize; ++index)
	{
		// find rising edge
		if(data[index]== 1 && data[index-1] == 0)
			return index;
	}

	return -1;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Detector::detected
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Detector::detected(int sequence)

{
	cout << "Detected sequence " << sequence << endl;

	// Send data to flightgear
	fg_interface->detect(sequence);
}






