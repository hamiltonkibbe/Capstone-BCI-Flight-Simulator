/*
	Sequence Wrapper Class
	(c) 2010 Hamilton Kibbe
*/

#ifndef __SEQUENCE_WRAPPER_H_
#define	__SEQUENCE_WRAPPER_H_

// ====================================================================
// Includes
// ====================================================================

// Libraries
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <pthread.h>

// Project-specific
#include "Alert.h"
#include "SystemParameters.h"
#include "Sequence.h"




// ====================================================================
// Class Declaration
// ====================================================================
class Sequence_Wrapper
{

private:


	bool isLoaded;

	vector<Sequence *> sequences;

	SystemParameters * sysParams;

	void load_sequence(string seq);

	void purge();


public:

	Sequence_Wrapper();

	~Sequence_Wrapper();

	Sequence* operator[] (unsigned int i);

	unsigned int length();

	unsigned int raw_length();

	unsigned int number() { return sequences.size(); }

	void load_sequences_from_file(const char* filename);

	void print_sequences();

	bool is_loaded() { return isLoaded; }

	void setSystemParameters(SystemParameters * params) { sysParams = params; }

	Sequence * at(unsigned int i) { return sequences.at(i); }

};

// ====================================================================
// Callback Function Prototypes
// ====================================================================


void update_progress_window(int count, GtkWidget* progress_bar);

#endif
