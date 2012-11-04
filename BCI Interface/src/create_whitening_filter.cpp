#include "Wfilter/create_whitening_filter.hpp"
/*Filter preserves shape (constant group delay)
	if WH_FILTER_LP is 1 */
#define WH_FILTER_LP 0 

/*
void make_real(fftw_complex *H, int length)
assumptions:
	H[0] is the 0 frequency component
	H[length/2] is the last positive frequency component
	H[length/2 + 1] to H[length -1] are the negative fequency component
	H[length/2 + 1] would be the most negative frequency, corresponding to H[length/2 - 1]
goals:
	H[length - 1] should be the complex conjurgate of H[1], and so on until
	H[length/2 + 1] is the conjurgate H[length/2 - 1]
	H[length/2] should be made real. 
*/
void make_real(fftw_complex *H, int length)
{
	int i1 = 1, i2 = length - 1;
	while(i1 < length/2)
	{
		H[i2][0] = H[i1][0];
		H[i2][1] = -1.0 * H[i1][1];
		++i1;
		--i2;
	}
	std::complex<double> c;
	c.real(H[length/2][0]);
	c.imag(H[length/2][1]);
	H[length/2][0] = abs(c);
	H[length/2][1] = 0.0;
}


void create_whitening_filter(double * x, int x_length, double * dest, int n)
{
	using namespace std;
	ofstream ofs;

	fftw_complex* X_fftwc = new fftw_complex[x_length];

	//get fft of x
	fftw_plan p_forward = fftw_plan_dft_r2c_1d(x_length, x, X_fftwc, FFTW_ESTIMATE);
	fftw_execute(p_forward);
	fftw_destroy_plan(p_forward);
	
	//arrays for filter
	fftw_complex* H = new fftw_complex[x_length];
	fftw_complex* H_downsampled = new fftw_complex[n], *H_downsampled_it = H_downsampled;


	//geting an array of complex<double> from array of fftw_complex:
	//Make sure complex<double> is in memory an array of two doubles with real first.
	//According to information in the FFTW documentation, this is true for all major implementaitons.
	complex<double> *X = reinterpret_cast<complex<double>* > (X_fftwc), *H_begin = reinterpret_cast<complex<double>* > (H),
		*H_it = H_begin;

#if DEBUG_OUTPUT
	ofs.open("X.txt");
	copy(X, X + x_length, ostream_iterator<complex<double> >(ofs, "\n"));
	ofs.close();

	m_file("X.m", "X_cpp", X, x_length);
#endif

	//limits for fft magnitude
	double X_max = pow(10.0, 6);
	double X_min = pow(10.0, -6);

	//inversion of frequency components of X to get H
	for(const complex<double> *X_it = X; X_it < X + (x_length/2 + 1); ++X_it)
	{
#if WH_FILTER_LP
		if(abs(*X_it) > X_max)
		{
			*H_it = X_min;
		}
		else
		{

			if(abs(*X_it) < X_min)
			{
				*H_it = X_max;
			}
			else
			{
				*H_it = 1.0/abs(*X_it);
			}
		}
#else
		if(abs(*X_it) > X_max)
		{
			*H_it = polar(X_min, -1*arg(*X_it));
		}
		else
		{

			if(abs(*X_it) < X_min)
			{
				*H_it = polar(X_max, -1* arg(*X_it));
			}
			else
			{
				*H_it = 1.0/(*X_it);
			}
		}
#endif
		++H_it;
	}

#if DEBUG_OUTPUT
	ofs.open("H before impulse response made real.txt");
	copy(H_begin, H_begin + (x_length/2 + 1), ostream_iterator<complex<double> >(ofs, "\n"));
	ofs.close();
	m_file("Hbefore.m", "H_cpp_before", X, x_length);
#endif


	make_real(H, x_length);

#if DEBUG_OUTPUT
	ofs.open("H after impulse response made real.txt");
	copy(H_begin, H_begin+ x_length, ostream_iterator<complex<double> >(ofs, "\n"));
	ofs.close();
	m_file("Hafter.m", "H_cpp_after", H_begin, x_length);
#endif

	//to hold impulse response
	complex<double> * h = new complex<double>[n];
	fftw_complex * h_fftwc = reinterpret_cast<fftw_complex *> (h);


	if(x_length == n)
	{
		fftw_plan p_backward = fftw_plan_dft_1d(n, H, h_fftwc, FFTW_BACKWARD, FFTW_ESTIMATE);
		fftw_execute(p_backward);
		fftw_destroy_plan(p_backward);
	}
	else
	{
		//downsampling H in frequency domain
		for(fftw_complex* H_it2 = H; H_it2 < H + x_length; H_it2+= x_length/n)
		{
			(*H_downsampled_it)[0] = (*H_it2)[0];
			(*H_downsampled_it)[1] = (*H_it2)[1];
			++H_downsampled_it;
		}
		fftw_plan p_backward = fftw_plan_dft_1d(n, H_downsampled, h_fftwc, FFTW_BACKWARD, FFTW_ESTIMATE);
		fftw_execute(p_backward);
		fftw_destroy_plan(p_backward);
	}
	
#if DEBUG_OUTPUT
	ofs.open("h.txt");
	copy(h, h + n, ostream_iterator<complex<double> >(ofs, "\n"));
	ofs.close();
#endif

	//truncate the imaginary part, which should be close to zero
	for(complex<double> *h_it = h; h_it < h + n; ++h_it)
	{
		*dest = h_it->real()/n;
		++dest;
	}

	delete[](X_fftwc);
	delete[](H);
	delete[](H_downsampled);
}
