/**********************************************************************

    Sequence_Wrapper.cpp
    Sequence Wrapper class

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Sequence_Wrapper.h"



using namespace std;



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence_Wrapper::Sequence_Wrapper
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sequence_Wrapper::Sequence_Wrapper()
{
	isLoaded = 0;
	sequences.reserve(NUMBER_OF_SEQUENCES);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence_Wrapper::~Sequence_Wrapper
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sequence_Wrapper::~Sequence_Wrapper()
{
	unsigned int i;
	for (i = 0; i < sequences.size(); ++i)
	{
		delete sequences.at(i);
	}
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence_Wrapper::operator[]
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sequence* Sequence_Wrapper::operator[] (unsigned int i)
{
	return sequences.at(i);				// Return a pointer to the sequence
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence_Wrapper::length
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int Sequence_Wrapper::length(void)
{
	Sequence* seq;
	if (!sequences.empty())				// Make sure that the sequences
	{
		seq = sequences.at(0);						// have been loaded
		return seq->getDataLength();

	}

	else
	{
		return 0;
	}
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence_Wrapper::raw_length
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int Sequence_Wrapper::raw_length(void)
{
	Sequence* seq;
	if (!sequences.empty())				// Make sure that the sequences
	{
		seq = sequences.at(0);						// have been loaded
		return seq->getRawDataLength();

	}
	else
	{
		return 0;
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence_Wrapper::load_sequences_from_file
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sequence_Wrapper::load_sequences_from_file(const char* filename)
{
	// Bring globals into scope
	extern GladeXML * gui;

	// Local variables
	int count = 0;
	int BUFFER_SIZE = 100;
	char status_text[BUFFER_SIZE];
	char full_filename[BUFFER_SIZE];
	ifstream file;
	string seq;

	// Gui stuff
	gdk_threads_enter();    // Get GDK lock
	GtkWidget* button = glade_xml_get_widget(gui, "close_loader_button");
	GtkWidget* progress_bar = glade_xml_get_widget(gui, "seq_progress_bar");
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar),0.0);
    gdk_threads_leave();    // Release GDK lock

	if (isLoaded) purge();

	// Open the file
	g_snprintf(full_filename,BUFFER_SIZE,"../res/%s",filename);
	file.open(full_filename);

	// Load the sequences
	while (file.good()) //while we still have stuff to read
	{
		getline(file, seq);
		if (!file.eof())
		{
			load_sequence(seq);
			count++;

			// update progress window
			update_progress_window(count,progress_bar);
		}
	}

	// Finish GUI update
	g_snprintf(status_text,BUFFER_SIZE, "Loaded %d sequences. Length: %d", count,this->at(0)->getDataLength());

	gdk_threads_enter();    // Get GDK lock
	gtk_widget_hide(glade_xml_get_widget(gui, "seq_progress"));
	gtk_widget_set_sensitive(button,TRUE);
	status(status_text);
	gdk_threads_leave();    // Release GDK lock

	isLoaded = 1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence_Wrapper::load_sequence
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sequence_Wrapper::load_sequence(string seq)
{
	// Local variables
	char n;
	int number;
	vector<int> seq_from_file;
	Sequence* seqObj;
	stringstream str(seq);		// use string as a stream for parsing

	seq_from_file.reserve(SEQUENCE_LENGTH);

	while (str >> n)		// read the string in
	{
		if (n == ',')
		{
			// do nothing, ignore it
		}
		else if (n == '\n')
		{
			break;
		}
		else
		{
			string character(1,n);

            number = atoi(character.c_str());
            if (number == 0) number = -1;
			seq_from_file.push_back(number);
		}

	}

	if (seq_from_file.size() > 3)
	{
		seqObj = new Sequence;
		seqObj->setSysParams(sysParams);	// Set it up
		seqObj->setSeq(seq_from_file);		// add the data to it
		sequences.push_back(seqObj);		// load the new sequence into the wrapper
	}

	isLoaded = 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence_Wrapper::purge
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sequence_Wrapper::purge()
{
	unsigned int i;
		for (i = 0; i < sequences.size(); ++i)
		{
			delete sequences.at(i);
		}
		sequences.clear();
		sequences.reserve(NUMBER_OF_SEQUENCES);
		isLoaded = 0;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence_Wrapper::print_sequences
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sequence_Wrapper::print_sequences()
{
	// Local Variables
	unsigned int    seq_index,
                    data_index;

	Sequence * seq = this->at(0);
	vector<int>* values;
	cout << endl <<"- - - - - - - - - - - - - - - - - - - -" << endl;
	cout << "Sequence Length: " << seq->getDataLength() << endl;

	for (seq_index = 0; seq_index < this->number(); ++seq_index)
	{
		cout << seq_index+1 << ":  ";
		for(data_index = 0; data_index < this->raw_length(); ++data_index)
		{
			seq = this->at(seq_index);
			values = seq->getRawSequence();
			cout << values->at(data_index) << ",";
		}
		cout << endl;
	}
	cout << "- - - - - - - - - - - - - - - - - - - -" << endl << endl;

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// update_progress_window
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void update_progress_window(int count, GtkWidget* progress_bar)
{
	char label_text[50];

	// Update the progress bar
	double progress = count/34.0;
	sprintf(label_text, "Loaded %d of 34 sequences", count);

	gdk_threads_enter();    // Get GDK lock
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar),progress);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), label_text);
	gdk_threads_leave();    // Release GDK lock
}

