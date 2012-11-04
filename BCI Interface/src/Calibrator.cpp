/**********************************************************************

    Calibrator.cpp
    Calibrator class

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Calibrator.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Calibrator::Calibrator
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Calibrator::Calibrator()
    : filter(NULL)
{
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Calibrator::~Calibrator
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Calibrator::~Calibrator()
{
    if(filter)
        delete filter;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Calibrator::calibrate
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Calibrator::calibrate(void)
{
	// Bring globals into scope
	extern GladeXML     *gui;

    // Locals
	unsigned int        seq_index,
                        count;

	char                label_text[50];

    bool                skip,
                        started,
                        paused;

	Sequence            *seq;

	GtkWidget           *label,
                        *notify,
                        *window,
                        *progress_bar;

	vector< vector<float> > temp;
	Template sequence_template(NUMBER_OF_CHANNELS,sequences->length());

	gdk_threads_enter();    // Get GDK lock
	progress_bar = glade_xml_get_widget(gui, "cal_progress_bar");
	window = glade_xml_get_widget(gui, "cal_progress");
	label = glade_xml_get_widget(gui, "cal_label");
	notify = glade_xml_get_widget(gui, "cal_notify");
    username = (const char*)gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "user_field")));
    gdk_threads_leave();    // Release GDK lock

	for(seq_index = 0; seq_index < sequences->number(); ++seq_index)	// calibrate for each sequence
	{
        // Reset calibration progress window
        sprintf(label_text, "Calibrate Sequence %d", seq_index+1);

        gdk_threads_enter();    // Get GDK lock
        gtk_widget_set_sensitive(glade_xml_get_widget(gui, "cal_skip"),TRUE);
        gtk_widget_set_sensitive(glade_xml_get_widget(gui, "cal_pause"),FALSE);
        gtk_label_set_text(GTK_LABEL(label),label_text);
        gdk_threads_leave();    // Release GDK lock

        update_cal_progress_window(-1);


        // wait for user to click start button
        do
        {
            gdk_threads_enter();    // Get GDK lock
            skip = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(gui, "cal_skip")));
            gdk_threads_leave();    // Release GDK lock

            if(skip)
            {
                gdk_threads_enter();    // Get GDK lock
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(gui, "cal_skip")),FALSE);
                gdk_threads_leave();

                ++seq_index;
                break;		// return to beginning
            }

            gdk_threads_enter();    // Get GDK lock
            started = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(gui, "cal_start")));
            gdk_threads_leave();    // Release GDK lock

        }while(!started);

        if (seq_index < sequences->number())
        {

            gdk_threads_enter();    // Get GDK lock
            gtk_widget_set_sensitive(glade_xml_get_widget(gui, "cal_start"), FALSE);
            gtk_widget_set_sensitive(glade_xml_get_widget(gui, "cal_skip"),FALSE);
            gtk_widget_set_sensitive(glade_xml_get_widget(gui, "cal_pause"),TRUE);
            gdk_threads_leave();    // Release GDK lock

            // Reset the template
            sequence_template.reset();

            // average PERIODS_TO_AVERAGE periods of EEG signal
            for(count = 0; count < PERIODS_TO_AVERAGE; ++count)
            {
                // Check pause button
                do
                {
                    gdk_threads_enter();
                    paused = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(gui, "cal_pause")));
                    gdk_threads_leave();
                    for(unsigned i = 0; i < 10000; ++i);
                } while (paused); // Wait in loop

                temp = get_one_period();

                if (!is_corrupt(temp) )
                {
                    sequence_template.add_data(temp);
                    update_cal_progress_window(count);

                }
                else{ --count; }
            }

            sequence_template.generate_template();
            seq = sequences->at(seq_index);
            seq->set_template(sequence_template);

            // Write cal file
            write_calibration_file(seq_index+1, sequence_template.get_template());
        }

		// Reset toggle button
		gdk_threads_enter();    // Get GDK lock
		gtk_widget_set_sensitive(glade_xml_get_widget(gui, "cal_start"), TRUE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(glade_xml_get_widget(gui, "cal_start")),FALSE);
        gdk_threads_leave();    // Release GDK lock
	}


	xval("xval.txt");
	// Hide widget
	gdk_threads_enter();    // Get GDK lock
	gtk_widget_hide(window);
	gdk_threads_leave();    // Release GDK lock

}


double ch_weight(int ch_index)
{
    return 1.0;
}

template<typename It2D, typename OutIt>
void maxtrix_element_product(It2D first1, It2D last1, It2D first2, OutIt out)
{
    for(It2D it = first1; it!= last1; ++it)
    {
        double acc = 0.0;
        for(int i = 0; i != it->size(); ++ i)
        {
            acc += (*it)[i] * (*first2)[i];
        }
        *out = acc;
        //*out = acc * ch_weight(it - first1);
        ++out;
        ++first2;
    }
}

void Calibrator::xval(const char* fname)
{
    ofstream ofs(fname);
    vector<vector<vector<float > > > *d = sequences->at(0)->get_raw_template_data();
    int size = d->size();
    vector<vector<vector<int> > > guesses; //omitted period, sequence template ommited from, then period index of guess
    for(int omit = 0; omit != size; ++ omit)
    {
        guesses.push_back(vector<vector<int > >());
        vector<vector<float> > ntemplate(NUMBER_OF_CHANNELS, vector<float>((*d)[0][0].size(), 0.0));
        for(int seq = 0; seq != sequences->number(); ++ seq)//seq is seq to omit from
        {
            guesses.back().push_back(vector<int>(size, -1));
            d = sequences->at(seq)->get_raw_template_data();
            for(int i = 0; i != size; ++i)
            {
                if(i == omit)
                    continue;
                for(int ch = 0; ch != NUMBER_OF_CHANNELS; ++ ch)
                {
                    for(int j = 0; j!= (*d)[i][ch].size(); ++ j)
                    {
                        ntemplate[ch][j] += (*d)[i][ch][j];
                    }
                }
            }
            for(int ch = 0; ch != NUMBER_OF_CHANNELS; ++ ch)
            {
                #ifndef MONOENERGETIC_TEMPLATE
                #define MONOENERGETIC_TEMPLATE 1
                #endif
                #if MONOENERGETIC_TEMPLATE
                //Normalize
                float energy = 0.0;
                for(int i = 0; i != ntemplate[ch].size(); ++i)
                {
                    energy += ntemplate[ch][i]* ntemplate[ch][i];
                }
                float factor = sqrt(energy);
                for(int i = 0; i != ntemplate[ch].size(); ++i)
                {
                    ntemplate[ch][i] /= (factor *(PERIODS_TO_AVERAGE -1));
                }
                #else
                //don't normalize
                for(int i = 0; i != ntemplate[ch].size(); ++i)
                {
                    ntemplate[ch][i] /= (PERIODS_TO_AVERAGE -1);
                }
                #endif
            }
            vector<vector<double> > scores;

            vector<double> blank_score_row(sequences->number(), 0.0);

            for(int i = 0; i != size; ++i)
            {
                scores.push_back(blank_score_row);
                for(int seq_index2 = 0 ; seq_index2 != sequences->number(); ++ seq_index2)
                {
                    vector<vector<float > > *t;
                    vector<double> products(NUMBER_OF_CHANNELS, 0.0);
                    if(seq == seq_index2)//if this is the sequence we made a new template for
                    {
                        t = &ntemplate;
                    }
                    else
                    {
                        t = sequences->at(seq_index2)->get_template();
                    }
                    maxtrix_element_product(t->begin(), t->end(), (*d)[i].begin(), products.begin());
                    double s = accumulate(products.begin(), products.end(), double(0.0));
                    scores.back()[seq_index2] = s;
                }
                int guess = std::max_element(scores.back().begin(), scores.back().end()) - scores.back().begin();
                (guesses.back().back())[i] = guess;
            }
        }

    }

    for(int seq = 0; seq != sequences->number(); ++ seq)
    {
        vector<int> accumulated_guesses(sequences->number(), 0);
        for(int i = 0; i != size; ++i)
        {
            for(int j = 0; j!= size; ++ j)
            {
                ++accumulated_guesses[guesses[i][seq][j]];
            }
        }
        int most_guesses = max_element(accumulated_guesses.begin(), accumulated_guesses.end()) - accumulated_guesses.begin();
        ofs << "ommiting from sequence " << seq << ", correct guesses: "
        << double(accumulated_guesses[seq]) *100.0/ (size * size) << " % " << endl;

        ofs << "most guesses for sequence: " << most_guesses << " guesses: "
        << double(accumulated_guesses[most_guesses]) *100.0/ (size * size) << " %"
        << endl << endl;
    }
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Calibrator::load_calibration_files
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Calibrator::load_calibration_files(void)
{

	// Bring globals into scope
	extern GladeXML* gui;

	// Locals
	unsigned int    seq_index,
                    ch_index,
                    end,
                    index,
                    idx,
                    count;
	float           value;
	char            filename[100];
	char            status_text[100];
	string          line;
	ifstream        file;
	Sequence        *seq;

	vector< vector<float> > temp(NUMBER_OF_CHANNELS,vector<float>(sequences->length(),0.0));
	Template sequence_template(NUMBER_OF_CHANNELS, sequences->length());

    gdk_threads_enter();    // Get GDK lock

    username = gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "user_field")));

    gdk_threads_leave();    // Release GDK lock


	for(seq_index = 0; seq_index < sequences->number(); ++seq_index)
	{

		count = 0;
		idx = 0;
		sequence_template.reset();

		// open cal file
		g_snprintf(filename,100,"../res/cal/%s_seq%d_%dbps.tmpl",username,seq_index+1,bit_rate);
		file.open(filename);


		while(file.good())
		{
			// Get a line from the file
			getline(file,line);

			// reset
			index = 0;

			if (!line.empty())			// check for last line
			{
				for(ch_index = 0; ch_index < NUMBER_OF_CHANNELS; ++ch_index)	// Get data for each channel
				{
					// Get a value from the string
					end = line.find(",",index,1);
					value = atof(line.substr(index,end-index).c_str());
					index = end+1;

					// add value to matrix
					temp.at(ch_index).at(idx) = value;
					//cout << temp.at(j).at(idx) << endl;
				}
			}
			++idx;
		}

		// Generate template and add it to the sequence
		sequence_template.set_data(temp);
		seq = sequences->at(seq_index);
		seq->set_template(sequence_template);

		// close file
		file.close();



	}
    g_snprintf(status_text,100,"Loaded %i templates for %s @%d bps",seq_index,username,bit_rate);

    gdk_threads_enter();    // Get GDK lock
    status(status_text);
    gdk_threads_leave();    // Release GDK lock

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Calibrator::write_callibration_file
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Calibrator::write_calibration_file(int seq,  vector< vector<float> >* temp)
{
	unsigned int    data_index,
                    ch_index;
	char filename[50];

	// Open cal file
	sprintf(filename, "../res/cal/%s_seq%d_%dbps.tmpl",username, seq, bit_rate);
	ofstream file(filename,ofstream::binary);

	// Write file
	for(data_index = 0; data_index < sequences->length(); ++data_index)		// samples
	{
		for(ch_index = 0; ch_index < NUMBER_OF_CHANNELS; ++ch_index)						// channels
		{
			//write data to file
			file << temp->at(ch_index).at(data_index) << ",";
		}

		file << endl;
	}
	// Close file
	file.close();
}




// ====================================================================
// Private Methods
// ====================================================================

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Calibrator::get_one_period
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
vector< vector<float> > Calibrator::get_one_period(void)
{
	unsigned int    ch_index,
                    norm_index;
    int             trigger_index;
	bool            done = 0;
	float           *channel_data;
	ChannelData     *cd, *cd_orig;
	unsigned int block_size = sysParams->getBlockSize();
	unsigned int period_length = sequences->length();
	vector< vector<float> > data(TRIGGER_CHANNEL,vector<float>(0));

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

	for(ch_index = 0; ch_index < TRIGGER_CHANNEL; ++ch_index)
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


		for(ch_index = 0; ch_index < TRIGGER_CHANNEL; ++ch_index)
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
		if(filter)
            delete cd;
	}

	return data;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Calibrator::find_trigger_index
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Calibrator::find_trigger_index(float* data)
{
	// Local Variables
	int index;

	int blockSize = sysParams->getBlockSize();

	for(index=1; index < blockSize; index++)
	{
		// find rising edge
		if(data[index] == 1 && data[index-1] == 0)
			return index;
	}

	return -1;
}




// ====================================================================
// Support Functions
// ====================================================================


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	is_corrupt
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool is_corrupt(vector< vector<float> > data)
{
	unsigned int ch_index,
                 data_index;

    // Loop through each sample in data
	for (ch_index = 0; ch_index < data.size(); ++ch_index)
	{
		for (data_index = 0; data_index < data[0].size(); ++data_index)
		{
			// this checks for NaN
			if (data[ch_index][data_index] != data[ch_index][data_index])
				return 1;
		}
	}
	return 0;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	update_cal_progress_window
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void update_cal_progress_window(int count)
{
    extern GladeXML *gui;
	char text[50];
	sprintf(text, "Averaged %d of %d periods", count+1, PERIODS_TO_AVERAGE);

	gdk_threads_enter();    // Get GDK lock
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(glade_xml_get_widget(gui, "cal_progress_bar")),((double)(count+1)*1.0/PERIODS_TO_AVERAGE));
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(glade_xml_get_widget(gui, "cal_progress_bar")),text);
	gdk_threads_leave();    // Release GDK lock
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	print accuracy statistics to console
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void print_stats(Sequence_Wrapper *sequences)
{
	unsigned int    ch_index,
                    seq_index;
	float* accuracy;
	Validator validate;
	validate.set_sequences(sequences);

	for(ch_index = 0; ch_index < NUMBER_OF_CHANNELS; ++ch_index)
	{
		accuracy = validate.x_validate(ch_index+1);
		cout << "Channel " << ch_index+1 << " Accuracy: " << endl;
		for(seq_index = 0; seq_index < sequences->number(); ++ seq_index)
			cout << "Sequence " << seq_index+1 << ": " << accuracy[seq_index] << endl;
		cout << "Overall: " << accuracy[sequences->number()] << endl << endl;
	}
}


