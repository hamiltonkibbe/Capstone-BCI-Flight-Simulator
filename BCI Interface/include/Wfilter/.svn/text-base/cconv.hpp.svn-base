#ifndef CCONV_HPP
#define CCONV_HPP

inline int get_index(int i, int dist)
{
	if(i > -1 && i < dist)
	{
		return i;
	}
	int r= i % dist;
	if( r < 0)
	{
		return dist + r;
	}
	else
	{
		return r;
	}
}

template<typename T, typename It>
T periodic_zero_fill(It it,  int index, int len, int N)
{
	int i = get_index(index, N);
	if(i < len)
	{
		return it[i];
	}
	else
	{
		return 0.0;
	}

}
template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename OutputIterator>
void cconv(RandomAccessIterator1 s1, int len1, RandomAccessIterator2 s2, int len2, OutputIterator dest, int N)
{
	for(int n = 0; n < N; ++n)
	{
		double total = 0;
		for(int m = 0; m < len2; ++m)
		{
			total += s2[m] * periodic_zero_fill<double>(s1, n - m, len1, N);
		}
		*dest = total;
		++dest;
	}
}


#endif