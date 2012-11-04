#include "Wfilter/eeg_whitening_filter.hpp"
eeg_whitening_filter::eeg_whitening_filter(const char* file_name)
{
	using namespace std;

	vector<vector<double> > M;
	csv_vv(file_name, M, '\t');

	vector<double>::size_type col_size = M[0].size();
	channel_numbers.reserve(col_size);
	wf.reserve(col_size);

	//channel numbers in first row
	for(unsigned int col = 0; col < col_size; ++col)
	{

		channel_numbers.push_back(M[0][col]);
	}


	for(unsigned int col = 0; col < col_size; ++col)
	{
		//M.begin() + 1 to skip first row
		vv_row_iterator<double, vector<vector<double> >::iterator, long long> vv_row(M.begin() + 1, col);
		wf.push_back(new whitening_filter(vv_row, M.size()));
	}
}

void eeg_whitening_filter::load(const char* file_name)
{
	using namespace std;

	vector<vector<double> > M;
	csv_vv(file_name, M, '\t');

	for(unsigned int i = 0; i < wf.size(); ++i)
	{
		delete wf[i];
	}
	wf.clear();
	channel_numbers.clear();

	vector<double>::size_type col_size = M[0].size();
	channel_numbers.reserve(col_size);
	wf.reserve(col_size);

	//channel numbers in first row
	for(unsigned int col = 0; col < col_size; ++col)
	{

		channel_numbers.push_back(M[0][col]);
	}


	for(unsigned int col = 0; col < col_size; ++col)
	{
		//M.begin() + 1 to skip first row
		vv_row_iterator<double, vector<vector<double> >::iterator, long long>  vv_row(M.begin() + 1, col);
		whitening_filter * f = new whitening_filter(vv_row, static_cast<long long> (col_size));
		wf.push_back(f);
	}
}

void eeg_whitening_filter::save(const char* file_name)
{
	std::ofstream ofs(file_name);

	//channel numbers in first row
	for(unsigned int col = 0; col < channel_numbers.size(); ++col)
	{
		ofs << channel_numbers[col] << '\t';
	}

	cout << endl;

	//now wf data
	for(int row = 0; row < wf[0]->size(); ++row)
	{
		for(unsigned int col = 0; col < channel_numbers.size(); ++col)
		{
			ofs << (wf[col]->begin())[row] << '\t';
		}
		ofs << endl;
	}
}

eeg_whitening_filter* new_whitening_filter(SignalSource* src, SystemParameters * sysP, int downsample_factor)
{
    //my overly flexible solution for one indexing
    int *n = new int[NUMBER_OF_CHANNELS];
    for(int i = 0; i !=  NUMBER_OF_CHANNELS; ++i)
    {

        n[i] = i + 1;
    }
    return new eeg_whitening_filter(src, sysP, sysP->getBlockSize() * downsample_factor, sysP->getBlockSize(), n, NUMBER_OF_CHANNELS);
}



