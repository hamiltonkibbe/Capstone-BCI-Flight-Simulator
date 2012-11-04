/*
	Template Class
	(c) 2010 Hamilton Kibbe
*/

#ifndef __TEMPLATE_H_
#define __TEMPLATE_H_


#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include "SystemParameters.h"


#define THROWAWAY 10


// ====================================================================
// Class Declaration
// ====================================================================
class Template
{
private:

	int count;

	int length;

	vector< vector<float> > data;

	vector< vector < vector<float> > > raw_data;

	void normalize_rms();

public:

	Template();

	Template(int ch, int len);

	~Template();

	void add_data(vector< vector<float> > seq_templates);

	void set_data(vector< vector<float> > seq_templates);

	void generate_template();

	void reset();

	vector< vector<float> >* get_template() { return &data; }

	vector< vector< vector<float> > >* get_raw_data() { return &raw_data; }
};

#endif
