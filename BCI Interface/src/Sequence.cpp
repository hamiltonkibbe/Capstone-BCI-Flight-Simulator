/**********************************************************************

    Sequence.cpp
    Sequence class

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Sequence.h"


using namespace std;



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence::Sequence
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sequence::Sequence()
{
	raw_sequence.reserve(SEQUENCE_LENGTH);
	interp_sequence.reserve(SEQUENCE_LENGTH * 100);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence::~Sequence
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sequence::~Sequence()
{
	raw_sequence.clear();
	interp_sequence.clear();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence::setSeq
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sequence::setSeq(vector<int> seq)
{
	raw_sequence = seq;
	interpolate();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence::get_template_data
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float Sequence::get_template_data(int ch, int index)
{
	vector< vector<float> >* temp = seq_template.get_template();
	return temp->at(ch).at(index);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence::get_raw_template_data
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
vector< vector< vector<float> > >* Sequence::get_raw_template_data()
{
	return seq_template.get_raw_data();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence::getData
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Sequence::getData(unsigned int i)
{
	if (!interp_sequence.empty())
	{
		return interp_sequence.at(i);
	}
	else
	{
		return -1;				// ** Check for this case!! **
	}							// it will give a heads up if
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence::update
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sequence::update(void)
{
	interpolate();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence::interpolate
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sequence::interpolate(void)
{
	// Local variables
	unsigned int    count,
                    Fs,
                    bit_rate,
                    samples_per_bit;

	vector<int>::iterator it;

	// Get system parameters data
	//Fs = sysParams->getFs();
	Fs = 600;
	bit_rate = sysParams->getBitRate();

	// calculate samples per bit
	samples_per_bit = Fs / bit_rate;

	// load interp_sequence with the interpolated sequences
	for (it = raw_sequence.begin(); it < raw_sequence.end(); it++)
	{
		int value = *it;								// get each bit and
		for (count = 0; count < samples_per_bit; ++count)   // add it to the output
		{											// sequence samples_per_bit
			interp_sequence.push_back((float)value);		// times
		}
	}
}
