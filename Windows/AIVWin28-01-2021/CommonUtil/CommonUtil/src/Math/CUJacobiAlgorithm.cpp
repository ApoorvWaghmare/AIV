#include "CUJacobiAlgorithm.h"
#include "math.h"
#include <float.h>
#include "CUSqMatrix.h"

namespace CommonUtil
{
#define ROTATE(a,i,j,k,l) g =a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);\
a[k][l]=h+s*(g-h*tau);


//---------------------------------------------------------------------

bool JacobiAlgorithm::Execute(CommonUtil::SqMatrix &mat, 
							  std::vector<double> &eigenValues,
							  CommonUtil::SqMatrix &eigenVector)
{
	int nrot = 0;
	bool stat = computeEigenValues(mat,eigenValues,eigenVector,nrot);
	return stat;
}

//---------------------------------------------------------------------

bool JacobiAlgorithm::computeEigenValues(CommonUtil::SqMatrix &a, std::vector<double> &d, CommonUtil::SqMatrix &v, int &nrot)
{
	//This method computes eigenValues and eogenVectors of a real symetric matrix a[0...n-1][0...n-1].
	//On output, elements of a above the diagonal are destroyed d[0...n-1] returns the eigenvalues
	//of a. v[0..n-1][0...n-1] is matrix whose columns contain, on output, the normalised 
	//eigenvectors of a, nrot returns the number of Jacobi rotations that were required
	int i,j,ip,iq;
	double tresh, theta, tau, t, sm, s, h, g, c;

	int n = (int)d.size();
	std::vector<double> b(n), z(n);

	//Initialize ot the identity matrix
	for(ip = 0; ip < n; ip++)
	{
		for(iq = 0; iq < n; iq++)
		{
			v(ip,iq) = 0.0;
			v(ip,ip) = 1.0;
		}
	}
	//Initialize b and d to the diagonal of a
	for(ip = 0; ip < n; ip++)
	{
		b[ip] = d[ip] = a(ip,ip);
		z[ip] = 0.0;
	}
	nrot = 0;
	for(i = 1; i <= 50; i++)
	{
		//Sum magnitude of off-diagonal elements
		sm = 0.0;
		for(ip = 0; ip < n-1; ip++)
		{
			for(iq = ip+1; iq < n; iq++)
			{
				sm += fabs(a(ip,iq));
			}
		}
		if(sm == 0.0)
			return false; // the normal return, ehish relies on quadratic convergence to m/c underflow
		if(i < 4)
			tresh = 0.2 * sm /(double)(n * n);
		else 
			tresh = 0.0;
		for(int ip = 0; ip < n-1; ip++)
		{
			for(iq = ip+1; iq < n; iq++)
			{
				g = 100.0 * fabs(a(ip,iq));
				//After four sweeps, skip the roatation 
				//if the off-diagonal element is small
				if(i > 4 && (fabs(d[ip]) + g) == fabs(d[ip])
					&& (fabs(d[iq]) + g) == fabs(d[iq]))
					a(ip,iq) = 0.0;
				else if(fabs(a(ip,iq)) > tresh)
				{
					h = d[iq] - d[ip];
					if((fabs(h) + g) == fabs(h))
						t = (a(ip,iq)) / h;
					else 
					{
						theta = 0.5 * h / (a(ip,iq));
						t = 1.0 / (fabs(theta) + sqrt(1.0 + theta * theta));
						if(theta < 0.0)
							t = -t;
					}
					c = 1.0 / sqrt(1 + t*t);
					s = t * c;
					tau = s / (1.0 + c);
					h = t * a(ip,iq);
					z[ip] -= h;
					z[iq] += h;
					d[ip] -= h;
					d[iq] += h;
					a(ip,iq) = 0.0;
					for(j = 0; j < ip; j++)
						rotate(a, s, tau, j, ip, j, iq);
					for(j = ip+1; j < iq; j++)
						rotate(a, s, tau, ip, j, j, iq);
					for(j = iq+1; j < n; j++)
						rotate(a, s, tau, ip, j, iq, j);
					for(j = 0; j < n; j++)
						rotate(v, s, tau, j, ip, j, iq);
					++nrot;
				}
			}
		}
		for(int lp = 0; lp < n; lp++)
		{
			b[lp] += z[lp];
			d[lp] = b[lp];
			z[lp] = 0.0;
		}
	}
	return true;
}

//---------------------------------------------------------------------

void JacobiAlgorithm::rotate(CommonUtil::SqMatrix &a, const double s, const double tau, 
							const int i, const int j, const int k, const int m)
{
	double g, h;
	g = a(i,j);
	h = a(k,m);
	a(i,j) = g - s * (h + g * tau);
	a(k,m) = h + s * (g - h * tau);
}

//---------------------------------------------------------------------

}
