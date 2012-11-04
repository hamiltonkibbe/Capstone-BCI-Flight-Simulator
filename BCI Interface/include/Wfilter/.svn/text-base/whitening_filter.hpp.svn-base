#ifndef WHITENING_FILTER_HPP
#define WHITENING_FILTER_HPP
#include "create_whitening_filter.hpp"
#include "cconv.hpp"
#include <algorithm>

class whitening_filter 
{
private:
	double *h, *h_end;
public:
	whitening_filter(double * data, int data_size, int filter_size)
		: h(new double[filter_size]), h_end(h + filter_size)
	{
		create_whitening_filter(data, data_size, h, filter_size);
	}

	template<typename It, typename Size>
	whitening_filter(It impulse_response, Size filter_size)
		: h(new double[filter_size]), h_end(h + filter_size)
	{
		for(double* hi=h; hi < h + filter_size; ++hi)
		{
			*hi = *impulse_response;
			++impulse_response;
		}
	}

	whitening_filter& operator=(const whitening_filter& rhs)
	{
		if(rhs.size() != this->size())
		{
			delete[] h;
			h = new double[rhs.size()];
		}
		std::copy(rhs.h, rhs.h_end, h);
		return *this;
	}

	void new_filter(double * data, int data_size, int filter_size)
	{
		if(filter_size != this->size())
		{
			delete[] h;
			h = new double[filter_size];
		}
		create_whitening_filter(data, data_size, h, filter_size);
	}

	template<typename It, typename size>
	void load_filter(It * impulse_response, size filter_size)
	{
		//allocate if necessary
		if(filter_size != this->size())
		{
			delete[] h;
			h = new double[filter_size];
		}
		//copy data into filter impulse response h
		for(double* hi=h; hi < h + filter_size; ++hi)
		{
			*hi = *impulse_response;
			++impulse_response;
		}
	}

	void apply(const double * input, int input_size, double *out)
	{
		int size = this->size();
		cconv(input, input_size, h, size, out, size);
	}

	//returns pointer to begining of impulse reponse
	const double * begin() const
	{
		return h;
	}

	//returns pointer to one past last element of h
	const double * end() const
	{
		return h_end;
	}

	int size() const
	{
		return h_end - h;
	}

	~whitening_filter()
	{
		delete[] h;
	}
	
};

#endif