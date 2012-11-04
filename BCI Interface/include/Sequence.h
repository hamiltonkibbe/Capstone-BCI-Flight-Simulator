/*
	Sequence Class
	(c) 2010 Hamilton Kibbe
*/

#ifndef __SEQUENCE_H_
#define __SEQUENCE_H_


// ====================================================================
// Includes
// ====================================================================

// Libraries
#include <vector>
#include <iostream>

// Project-specific
#include "SystemParameters.h"
#include "Template.h"

// ====================================================================
// Macros
// ====================================================================


// ====================================================================
// Class Declaration
// ====================================================================
class Sequence
{
private:

	// Private members
	vector<int> raw_sequence;
	vector<float> interp_sequence;
	Template seq_template;
	SystemParameters * sysParams;

	// Internal manipulation
	void interpolate(void);

public:

	Sequence();
	~Sequence();

	// Methods
	void setSeq(vector<int> seq);
	void set_template(Template templ) { seq_template = templ; }
	int getData(unsigned int i);		// ** Always check for a return value of -1!!!!

	float get_template_data(int ch, int index);

	vector<vector<float> > *get_template() {return seq_template.get_template();}
	vector < vector <vector<float> > >* get_raw_template_data();
	void update(void);

	// Inline methods
	void setSysParams(SystemParameters * argParams)	{ sysParams = argParams; }
	unsigned int getDataLength(void)	{ return interp_sequence.size(); }
	unsigned int getRawDataLength(void)	{ return raw_sequence.size(); }
	vector<int>* getRawSequence(void) 	{ return &raw_sequence; }
	vector<float>* getInterpSequence(void) { return &interp_sequence; }



};

#endif
