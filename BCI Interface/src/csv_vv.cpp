#include "Wfilter/csv_vv.hpp"

void csv_vv(const char* f_name, std::vector<std::vector<double> > &dest)
{
	using namespace std;
	dest.clear();
	ifstream ifs(f_name);
	int n_channels = -1;//channels correspond to columns in vector "dest"
	while(ifs.good())
	{
		dest.push_back(vector<double>());
		if (n_channels > 0)
		{
			(dest.back()).reserve(n_channels);
		}
		string line;
		getline(ifs, line); //get line of text
		stringstream ss(line);
		string val;
		int num_commas;
		for(num_commas = 0; getline(ss, val, ','); ++num_commas) //get comma delimited unformatted
		{
			stringstream val_ss(val); //input/output stream from stuff between commas
			double value;
			val_ss >> value; //extract double
			(dest.back()).push_back(value);//add value to dest, the vector of vector<double>
		}
		if(dest.back().empty())//no data on this line
		{
			dest.pop_back();
		}
		else
		{

			if(num_commas > n_channels)//finding the maximum number of channels for all rows read so far
			{
				n_channels = num_commas;
			}
		}
	}
}

void csv_vv(const char* f_name, std::vector<std::vector<double> > &dest, char delim)
{
	using namespace std;
	dest.clear();
	ifstream ifs(f_name);
	int n_channels = -1;//channels correspond to columns in vector "dest"
	while(ifs.good())
	{
		dest.push_back(vector<double>());
		if (n_channels > 0)
		{
			(dest.back()).reserve(n_channels);
		}
		string line;
		getline(ifs, line); //get line of text
		stringstream ss(line);
		string val;
		int num_commas;
		for(num_commas = 0; getline(ss, val, delim); ++num_commas) //get comma delimited unformatted
		{
			stringstream val_ss(val); //input/output stream from stuff between commas
			double value;
			val_ss >> value; //extract double
			(dest.back()).push_back(value);//add value to dest, the vector of vector<double>
		}
		if(dest.back().empty())//no data on this line
		{
			dest.pop_back();
		}
		else
		{

			if(num_commas > n_channels)//finding the maximum number of channels for all rows read so far
			{
				n_channels = num_commas;
			}
		}
	}
}
