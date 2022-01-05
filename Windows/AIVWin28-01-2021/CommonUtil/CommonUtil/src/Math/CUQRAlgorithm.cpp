// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//standard include files
#include <math.h>
#include <iostream>

using namespace std;

//math include files
#include "CUQRAlgorithm.h"
#include "CUSqMatrix.h"


namespace CommonUtil
{

//-----------------------------------------------------------------------------

void QRAlgorithm::qrForHessenberg(CommonUtil::SqMatrix &givenMatrix, size_t n,
							 std::vector<double> &realEigenValues,
								 std::vector<double> &imaginaryEigenValues)
{
	size_t nn,m,l,k,j,its,i,mmin;
	//p,q,r initialised for lint

	double z,y,x,w,v,u,t,s,r = 0,q = 0,p = 0,anorm;

	realEigenValues.resize(n+1);
	imaginaryEigenValues.resize(n+1);

	anorm = fabs(givenMatrix(1,1));
	for(i = 2; i <= n; i++)
	{
		for(j = (i-1); j <= n; j++)
		{
			anorm += fabs(givenMatrix(i,j));
		}
	}

	nn = n;
	t = 0.0;
	while( nn >= 1)
	{
		its = 0;
		do
		{
			for(l = nn; l >= 2; l--)
			{
				s = fabs(givenMatrix(l-1,l-1)) + fabs(givenMatrix(l,l));
				if(s == 0.0)
					s = anorm;
				if((double)(fabs(givenMatrix(l,l-1)) + s) == s)
					break;
			}
			x = givenMatrix(nn,nn);
			if(l == nn)
			{
				realEigenValues[nn] = x + t;
				imaginaryEigenValues[nn--] = 0.0;
			}
			else
			{
				y = givenMatrix(nn-1,nn-1);
				w = givenMatrix(nn, nn-1) * givenMatrix(nn-1, nn);
				if(l == (nn-1))
				{
					p = 0.5 * (y-x);
					q = p*p + w;
					z = sqrt(fabs(q));
					x += t;
					if(q >= 0.0)
					{
						z = p + sign(z,p);
						realEigenValues[nn-1] = x + z;
						realEigenValues[nn] = x + z;
						if(z) realEigenValues[nn] = x - w/z;
						imaginaryEigenValues[nn-1] = 0.0;
						imaginaryEigenValues[nn] = 0.0;
					}
					else
					{
						realEigenValues[nn-1] = x + p;
						realEigenValues[nn] = x + p;
						imaginaryEigenValues[nn-1] = -(imaginaryEigenValues[nn] = z);
					}
					nn -= 2;
				}
				else
				{
					//if(its == 30)
					//	cout<<"Too many iterations in qrForHessenberg";
					if(its == 10 || its == 20)
					{
						t += x;
						for(i = 1; i<=nn; i++)
							givenMatrix(i,i) -= x;
						s = fabs(givenMatrix(nn,nn-1)) + fabs(givenMatrix(nn-1,nn-2));
						y =x = 0.75*s;
						w = -0.4375*s*s;
					}
					++its;
					for(m = (nn-2); m>=1; m--)
					{
						z = givenMatrix(m,m);
						r = x - z;
						s = y - z;
						p =(r*s-w)/givenMatrix(m+1,m) + givenMatrix(m,m+1);
						q = givenMatrix(m+1, m+1) -z-r-s;
						r = givenMatrix(m+2, m+1);
						s = fabs(p) + fabs(q) + fabs(r);
						p /= s;
						q /= s;
						r /= s;
						if(m == 1)
							break;
						u = fabs(givenMatrix(m,m-1))*(fabs(q)+fabs(r));
						v = fabs(p)*(fabs(givenMatrix(m-1, m-1))
											* fabs(z) + fabs(givenMatrix(m+1, m+1)));
						if((double)(u+v) == v)
							break;
					}
					for(i=m+2;i<=nn;i++)
					{
						givenMatrix(i,i-2)=0.0;
						if(i != (m+2))
						givenMatrix(i,i-3)=0.0;
					}
					for(k=m;k<=nn-1;k++)
					{
						if(k!=m)
						{
							p=givenMatrix(k,k-1);
							q=givenMatrix(k+1,k-1);
							r=0.0;
							if(k!=(nn-1)) r = givenMatrix(k+2, k-1);
								if((x=fabs(p)+fabs(q)+fabs(r)) != 0.0)
								{
									p /= x;
									q /= x;
									r /= x;
								}
						}
						if((s=sign(sqrt(p*p+q*q+r*r),p)) != 0.0)
						{
							if(k == m)
							{
								if(l != m)
									givenMatrix(k,k-1) = -givenMatrix(k,k-1);
							}
							else
								givenMatrix(k,k-1) = -s*x;
							p += s;
							x = p/s;
							y = q/s;
							z = r/s;
							q /= p;
							r /= p;
							for( j=k; j<=nn; j++)
							{
								p = givenMatrix(k,j) + q * givenMatrix(k+1,j);
								if(k!=(nn-1))
								{
									p+=r*givenMatrix(k+2,j);
									givenMatrix(k+2,j) -=p*z;
								}
								givenMatrix(k+1,j) -=p*y;
								givenMatrix(k,j) -=p*x;
							}
							mmin = nn<k+3 ? nn: k+3;
							for(i=1; i<=mmin; i++)
							{
								p=x*givenMatrix(i,k) + y*givenMatrix(i,k+1);
								if(k != (nn-1))
								{
									p += z*givenMatrix(i,k+2);
									givenMatrix(i,k+2) -= p*r;
								}
								givenMatrix(i,k+1) -=p*q;
								givenMatrix(i,k) -=p;
							}
						}
					}
				}
			}
			//*******************modified for X64***********
			if(nn == 0)
				break;
			//**********************************************
		}while(l<nn-1);
	}

}

//-----------------------------------------------------------------------------

void QRAlgorithm::balance(CommonUtil::SqMatrix &givenMatrix, size_t n)
{
	int last,j,i;
	double s,r,g,f,c,sqrdx;
	const double RADIX = 2.0;
	sqrdx=RADIX*RADIX;
	last=0;
	while (last == 0)
	{
		last=1;
		for (i=1;i<=n;i++)
		{
			// Calculate row and column norms.
			r=c=0.0;
			for (j=1;j<=n;j++)
				if (j != i) 
				{
					c += fabs(givenMatrix(j,i));
					r += fabs(givenMatrix(i,j));
				}
				if (c && r)
				{
					// If both are nonzero,
					g=r/RADIX;
					f=1.0;
					s=c+r;
					while (c<g)
					{ 
						//And the integer power of the machine radix that
						//	comes closest to balancing the matrix. 
						f *= RADIX;
						c *= sqrdx;
					}
					g=r*RADIX;
					while (c>g) 
					{
						f /= RADIX;
						c /= sqrdx;
					}
					if ((c+r)/f < 0.95*s) 
					{
						last=0;
						g=1.0/f;
						//Apply similarity transformation.
						for (j=1;j<=n;j++) givenMatrix(i,j) *= g; 
						for (j=1;j<=n;j++) givenMatrix(j,i) *= f;
					}
				}
		}
	}
}

//-----------------------------------------------------------------------------

void QRAlgorithm::reduceHessenberg(CommonUtil::SqMatrix &givenMatrix, size_t n)
{
	int m,j,i;
	double y,x;
	for (m=2;m<n;m++)
	{// m is called r + 1 in the text.
		x=0.0;
		i=m;
		for (j=m;j<=n;j++)
		{
			//Find the pivot.
			if (fabs(givenMatrix(j,m-1)) > fabs(x))
			{
				x=givenMatrix(j,m-1);
				i=j;
			}
		}
		if (i != m) 
		{// Interchange rows and columns.
			for (j=m-1;j<=n;j++)
				std::swap(givenMatrix(i,j),givenMatrix(m,j));
				for (j=1;j<=n;j++)
					std::swap(givenMatrix(j,i),givenMatrix(j,m));
		}
		if (x)
		{
			//Carry out the elimination.
			for (i=m+1;i<=n;i++)
			{
				if ((y=givenMatrix(i,m-1)) != 0.0)
				{
					y /= x;
					givenMatrix(i,m-1)=y;
					for (j=m;j<=n;j++)
						givenMatrix(i,j) -= y*givenMatrix(m,j);
					for (j=1;j<=n;j++)
						givenMatrix(j,m) += y*givenMatrix(j,i);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

void QRAlgorithm::Execute(CommonUtil::SqMatrix &matrix, std::vector<double> &realEigenValues,
					 std::vector<double> &imaginaryEigenValues)
{
	CommonUtil::SqMatrix givenMatrix(matrix);
	size_t n = matrix.GetNumCols()-1;

	balance(givenMatrix, n);
	reduceHessenberg(givenMatrix, n);
	qrForHessenberg(givenMatrix, n, realEigenValues, imaginaryEigenValues);
}

//-----------------------------------------------------------------------------

void QRAlgorithm::DisplayMatrix( const CommonUtil::SqMatrix &m)
{
	cout<<endl<<endl;
	size_t rowSize = m.GetNumRows();
	size_t columnSize = m.GetNumCols();

	for(int i=0; i<rowSize; i++)
	{
		for(int j=0; j<columnSize; j++)
		{
			cout<< m(i,j) << " ";
		}
		cout<<endl;
	}
}

//-----------------------------------------------------------------------------

}