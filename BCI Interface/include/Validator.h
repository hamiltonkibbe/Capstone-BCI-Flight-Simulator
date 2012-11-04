/*=====================================================================

 Validator

 Validates calibration data


 Copyright (C) 2010 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

=====================================================================*/

#ifndef VALIDATOR_H_
#define VALIDATOR_H_

// ====================================================================
// Includes
// ====================================================================

// Libraries
#include <stdlib.h>
#include <math.h>

// Project-specific
#include "Sequence.h"
#include "Sequence_Wrapper.h"
#include "Template.h"
#include "DSP/circular_buffer.h"



// ====================================================================
// Class Declaration
// ====================================================================
class Validator
{
public:

	Validator();

	~Validator();

	void set_sequences(Sequence_Wrapper* seq);

	float* x_validate(unsigned int channel);

private:

	float *accuracy;

	Sequence_Wrapper *sequences;

	circular_buffer<float> buffers[NUMBER_OF_CHANNELS];

	unsigned int detect(vector< vector< vector<float> > > const* templates, vector< vector<float> > const* data, int channel);

	vector<vector<float> >* leave_one_out_data(Sequence * seq, unsigned int period);


};


#endif
