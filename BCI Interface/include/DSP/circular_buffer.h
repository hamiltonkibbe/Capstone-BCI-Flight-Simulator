/*=====================================================================

 DSP Library
 circular_buffer: A circular buffer class

 Copyright 2011 Hamilton Kibbe
 All rights reserved

=====================================================================*/

#ifndef DSP_CIRCULAR_BUFFER_H_
#define DSP_CIRCULAR_BUFFER_H_


// Includes
#include "DSP.h"
#include <vector>

//=====================================================================
//	circular_buffer class
//=====================================================================
using namespace std;

template <typename T>
class circular_buffer
{

public:

	typedef typename vector<T>::iterator CB_iterator;

	//-----------------------------------------------------------------
	//	circular_buffer(void)
	//	constructor
	//-----------------------------------------------------------------
	circular_buffer();

	//-----------------------------------------------------------------
	//	circular_buffer(unsigned int length)
	//	constructor with buffer size
	//-----------------------------------------------------------------
	circular_buffer(unsigned int length);


	//-----------------------------------------------------------------
	//	~circular_buffer(void)
	//	destructor
	//-----------------------------------------------------------------
	~circular_buffer();


	//-----------------------------------------------------------------
	//	unsigned int size()
	//	returns size of buffer in elements
	//-----------------------------------------------------------------
	unsigned int size();

	//-----------------------------------------------------------------
	//	DSP_Status resize(unsigned int length)
	//	adds value to buffer
	//-----------------------------------------------------------------
	DSP_Status resize(unsigned int length);

	//-----------------------------------------------------------------
	//	DSP_Status set_data(vector<T> data)
	//	copies data to buffer and sets iterator to end of data
	//-----------------------------------------------------------------
	DSP_Status set_data(vector<T>* data);

	//-----------------------------------------------------------------
	//	DSP_Status purge(void)
	//	clears buffer data
	//-----------------------------------------------------------------
	DSP_Status purge();

	//-----------------------------------------------------------------
	//	DSP_Status reset(void)
	//	resets buffer to un-initialized state
	//-----------------------------------------------------------------
	DSP_Status reset();

	//-----------------------------------------------------------------
	//	DSP_Status operator << (T value)
	//	add data to the circular buffer
	//-----------------------------------------------------------------
	DSP_Status operator << (T value);

	//-----------------------------------------------------------------
	//	DSP_Status operator = (T value)
	//	add data to the circular buffer
	//-----------------------------------------------------------------
	DSP_Status operator = (T value);

    //-----------------------------------------------------------------
	//	DSP_Status operator = (vector<T> *vec)
	//	add data to the circular buffer
	//-----------------------------------------------------------------
    DSP_Status operator = (vector<T>* vec);

	//-----------------------------------------------------------------
	//	T& operator [] (int index)
	//	returns value at absolute index
	//-----------------------------------------------------------------
	T& operator [] (unsigned int index);

	//-----------------------------------------------------------------
	//	T& at_n_minus(int offset)
	//	returns value at n-offset
	//-----------------------------------------------------------------
	T& at_n_minus(unsigned int offset);

	//-----------------------------------------------------------------
	//	T& at_n_plus(int offset)
	//	returns value at n+offset
	//-----------------------------------------------------------------
	T& at_n_plus(unsigned int offset);

	//-----------------------------------------------------------------
	//	T& get_buffer(void)
	//	returns reference to buffer data
	//-----------------------------------------------------------------
	T& get_buffer();


private:

	unsigned int buffer_length;

	CB_iterator it;

	vector<T> buffer;

};



using namespace std;


//=====================================================================
//	circular_buffer methods
//=====================================================================

//---------------------------------------------------------------------
template <typename T>
circular_buffer<T>::circular_buffer()
{
	buffer_length = 0;
	it = buffer.begin();
}


//---------------------------------------------------------------------
template <typename T>
circular_buffer<T>::circular_buffer(unsigned int length)
{
	buffer_length = length;

	buffer.resize(buffer_length);
	buffer.assign(buffer_length, 0.0);

	it = buffer.begin();
}


//---------------------------------------------------------------------
template <typename T>
circular_buffer<T>::~circular_buffer()
{
	if (!buffer.empty())
		buffer.clear();
}


//---------------------------------------------------------------------
template <typename T>
unsigned int circular_buffer<T>::size()
{
	return buffer.size();
}



//---------------------------------------------------------------------
template <typename T>
DSP_Status circular_buffer<T>::resize(unsigned int length)
{
	buffer_length = length;
	buffer.clear();

	try { buffer.resize(length); }
	catch (...){ return DSP_ERROR_BAD_ALLOC; }

	buffer.assign(buffer_length, 0.0);
	it = buffer.begin();

	return DSP_OK;
}


//---------------------------------------------------------------------
template <typename T>
DSP_Status circular_buffer<T>::set_data(vector<T> * data)
{
	int pos = 0;
	int data_size = data->size();
	CB_iterator location;

	// Error checking
	if(buffer.empty() || buffer_length == 0)
		return DSP_ERROR_NOT_INITIALIZED;

	if(data_size > buffer_length)
		return DSP_ERROR_OUT_OF_RANGE;


	// Initialize buffer
	buffer.clear();
	buffer.resize(buffer_length);
	buffer.assign(buffer_length, 0.0);

	// add data to buffer
	for (location = data->begin(); location != data->end(); ++location)
	{
		pos = location-data->begin();
		buffer.at(pos) = *location;
	}

	// Set iterator to last new data point
	it = buffer.begin() + data_size - 1;

	return DSP_OK;
}


//---------------------------------------------------------------------
template <typename T>
DSP_Status circular_buffer<T>::purge()
{
	// Error checking
	if(buffer.empty() || buffer_length = 0)
		return DSP_ERROR_NOT_INITIALIZED;

	// clear data and re-initialize buffer
	buffer.clear();
	buffer.resize(buffer_length);
	buffer.assign(buffer_length, 0.0);
	it = buffer.begin();

	return DSP_OK;
}


//---------------------------------------------------------------------
template <typename T>
DSP_Status circular_buffer<T>::reset()
{
	// Error checking
	if(buffer.empty() || buffer_length = 0)
		return DSP_ERROR_NOT_INITIALIZED;

	// Reset
	it = NULL;
	buffer_length = 0;
	buffer.clear();

	return DSP_OK;
}


//---------------------------------------------------------------------
template <typename T>
DSP_Status circular_buffer<T>::operator << (T value)
{
	// Error checking
	if(buffer.empty() || buffer_length == 0)
		return DSP_ERROR_NOT_INITIALIZED;

	// Wrap iterator
	if (++it == buffer.end())
		it = buffer.begin();

	// Add data
	*it = value;

	return DSP_OK;
}


//---------------------------------------------------------------------
template <typename T>
DSP_Status circular_buffer<T>::operator = (T value)
{
	// Error checking
	if(buffer.empty() || buffer_length == 0)
		return DSP_ERROR_NOT_INITIALIZED;

	// Wrap iterator
	if (++it == buffer.end())
		it = buffer.begin();

	// Add data
	*it = value;

	return DSP_OK;
}


//---------------------------------------------------------------------
template <typename T>
DSP_Status circular_buffer<T>::operator = (vector<T>* vec)
{
    it = buffer.begin();
    for (unsigned int data_index = 0; data_index < vec->size(); ++data_index)
    {
        *it++ = vec->at(data_index);
    }

    return DSP_OK;
}

//---------------------------------------------------------------------
template <typename T>
T& circular_buffer<T>::operator [] (unsigned int index)
{
	return buffer.at(index);
}


//---------------------------------------------------------------------
template <typename T>
T& circular_buffer<T>::at_n_minus(unsigned int offset)
{
	CB_iterator location = it;

	// handle out of range offsets
	while(offset >= buffer_length)
	{
		offset -= buffer_length;
	}

	// wrap around
	if (unsigned(location - buffer.begin()) < offset)
		return *(location + (buffer_length-offset));

	// return
	return *(location-offset);
}


//---------------------------------------------------------------------
template <typename T>
T& circular_buffer<T>::at_n_plus(unsigned int offset)
{

	CB_iterator location = it;

	// handle out of range offsets
	while(offset >= buffer_length)
	{
		offset -= buffer_length;
	}

	// wrap around
	if((unsigned)(buffer.end() - location) <= offset)
	{
		location += offset;
		location -= buffer_length;
	}
	else location += offset;

	// return
	return *(location);
}

//---------------------------------------------------------------------
template <typename T>
T& circular_buffer<T>::get_buffer()
{
	if (!buffer.empty())
		return &buffer[0];

	else
		return NULL;
}

#endif
