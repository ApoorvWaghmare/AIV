// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//math include files
#include "CUHouseHolder.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

void Householder:: triad( SqMatrix &a , size_t n ,
									std::vector<double> &d , std::vector<double> &e)
{	
	d.resize(n);
	e.resize(n);
	
	size_t l,k,j,i;
	double scale,hh,h,g,f;

	for(i=n; i>=2;--i)
	{
		l = i-1;
		h= scale = 0.0;
		if( l > 1)
		{
			for(k=1; k<=l; ++k)
				scale += fabs(a(i-1,k-1));
			if(scale == 0.0)
				e[i-1] = a(i-1, l-1);
			else
			{
				for(k=1; k<=l;++k)
				{
					a(i-1,k-1) /= scale;
					h += a(i-1,k-1) * a(i-1,k-1);
				}
				f = a(i-1,l-1);
				g= (f>0.0 ? -sqrt(h) : sqrt(h));
				e[i-1] = scale*g;
				h -= f*g;
				a(i-1,l-1) = f-g;
				f=0.0;
				for(j=1; j<=l; ++j)
				{
					a(j-1,i-1) = a(i-1,j-1)/h;
					g=0.0;
					for(k=1; k<=j;++k)
						g+= a(j-1,k-1) *a(i-1,k-1);
					for(k = j+1; k<=l; ++k)
					g+= a(k-1,j-1) * a(i-1,k-1);
					e[j-1] = g/h;
					f += e[j-1]*a(i-1,j-1);
				}
				hh= f/(h+h);
				for(j=1; j<=l; ++j)
				{
				f= a(i-1,j-1);
				e[j-1] = g = e[j-1]- hh*f;
				for(k=1; k<=j; ++k)
					a(j-1,k-1) -= (f*e[k-1] + g*a(i-1,k-1));
				}
			}
		}
		else
			e[i-1] = a(i-1,l-1);

		d[i-1] = h;
		if (fabs(d[i - 1]) < CommonUtil::CommonConstants::ZERO)
			d[i-1] = 0.0;
	}

	d[0] = 0;
	e[0] = 0;
	for(i=1; i<=n; ++i)
	{
		l=i-1;
		if(d[i-1] != 0.0)
		{
			for(j=1; j<=l; ++j)
			{
				g=0.0;
				for(k=1;k<=l; ++k)
				
					g+= a(i-1,k-1) * a(k-1,j-1);
					for(k=1; k<=l; ++k)
						a(k-1,j-1) -= g*a(k-1,i-1);
			}
		}
		d[i-1] = a(i-1,i-1);
		if (fabs(d[i - 1]) < CommonUtil::CommonConstants::ZERO)
			d[i-1] = 0.0;
		a(i-1,i-1) = 1.0;
		for(j=1; j<=l; ++j)
			a(j-1,i-1) = a(i-1,j-1) = 0.0;
	}
}


//-----------------------------------------------------------------------------

void Householder:: triadQLI(std::vector<double> &d, std::vector<double> &e,
							size_t n, SqMatrix &z)
{
	size_t m, l, iter, i, k;
	double s,r,p,g,f,dd,c,b;
	
	for(i=2; i<=n; ++i)
		e[i-2] = e[i-1];
	e[n-1] = 0.0;
	
	for(l=1; l<=n; ++l)
	{
		iter = 0;
		do
		{
			for(m=l; m<=n-1; ++m)
			{
				dd = fabs(d[m-1]) + fabs(d[m]);
			if( fabs(e[m-1] + dd) == dd )
				break;
			}		
			if(m!=l)
			{
			if(iter > 30)
			{
				break;
			}
			iter++;
			g= (d[l]- d[l-1])/ (2.0*e[l-1]);
			r= pythag(g,1.0);
			g= d[m-1] - d[l-1] + e[l-1]/ (g+sign(r,g));
			s=c=1.0;
			p=0.0;
			for(i=m-1; i>=l; --i)
			{
				f = s* e[i-1];
				b = c*e[i-1];
				e[i] = (r=pythag(f,g));
				if(r==0)
				{
					d[i] -= p;
					if (fabs(d[i]) < CommonUtil::CommonConstants::ZERO)
						d[i] = 0.0;
					e[m-1] = 0.0;
					break;
				}
				s=f/r;
				c=g/r;
				g = d[i] - p;
				r = (d[i-1] - g)*s + 2.0 *c*b;
				d[i] = g+ (p=s*r);
				if (fabs(d[i]) < CommonUtil::CommonConstants::ZERO)
					d[i] = 0.0;
				g = c*r - b;
				for(k=1; k<=n; ++k)
				{
					f = z(k-1,i);
					z(k-1,i) = s*z(k-1,i-1) + c*f;
					z(k-1,i-1) = c*z(k-1,i-1) - s*f;
				}
			}
				if(r == 0.0 && i)continue;
				d[l-1] -= p;
				if (d[l - 1] < CommonUtil::CommonConstants::ZERO)
					d[l-1] = 0.0;
				e[l-1] = g;
				e[m-1] = 0.0;
			}
		}while(m!=l);
	}
}


//-----------------------------------------------------------------------------

double Householder::pythag(double a, double b)
{
	double absa,absb;
	absa=fabs(a);
	absb=fabs(b);
	if (absa > absb)
	return absa*sqrt(1.0+(absb/absa)*(absb/absa));
	else
	return (absb == 0.0 ? 0.0 : absb*sqrt(1.0+(absa/absb)*absa/absb));
}

//-----------------------------------------------------------------------------

void Householder::Execute(SqMatrix &matrix, std::vector<double> &eigenValues)
{
	std::vector<double> offDiagonalElements;

	size_t n = matrix.GetNumRows();
	triad(matrix, n, eigenValues, offDiagonalElements);
	triadQLI(eigenValues, offDiagonalElements, n, matrix);
}

//-----------------------------------------------------------------------------

void Householder::Execute1(const SqMatrix &matrix, std::vector<double> &eigenValues,
	SqMatrix &eigenVectorMatrix)
{
	eigenVectorMatrix = matrix;
	std::vector<double> offDiagonalElements;

	size_t n = matrix.GetNumRows();
	triad(eigenVectorMatrix, n, eigenValues, offDiagonalElements);
	triadQLI(eigenValues, offDiagonalElements, n, eigenVectorMatrix);
}

//-----------------------------------------------------------------------------

/*--------------------------- Methods In STEPReader -----------------------------

void Householder:: triad( SqMatrix &a , int n ,
									std::vector<double> &d , std::vector<double> &e)
{	
	d.resize(n);
	e.resize(n);
	
	int l,k,j,i;
	double scale,hh,h,g,f;

	for(i=n; i>=2;--i)
	{
		l = i-1;
		h= scale = 0.0;
		if( l > 1)
		{
			for(k=1; k<=l; ++k)
				scale += fabs(a(i-1,k-1));
			if(scale == 0.0)
				e[i-1] = a(i-1, l-1);
			else
			{
				for(k=1; k<=l;++k)
				{
					a(i-1,k-1) /= (float)(scale);
					h += a(i-1,k-1) * a(i-1,k-1);
				}
				f = a(i-1,l-1);
				g= (f>0.0 ? -sqrt(h) : sqrt(h));
				e[i-1] = scale*g;
				h -= f*g;
				a(i-1,l-1) = (float)(f-g);
				f=0.0;
				for(j=1; j<=l; ++j)
				{
					a(j-1,i-1) = (float)(a(i-1,j-1)/h);
					g=0.0;
					for(k=1; k<=j;++k)
						g+= a(j-1,k-1) *a(i-1,k-1);
					for(k = j+1; k<=l; ++k)
					g+= a(k-1,j-1) * a(i-1,k-1);
					e[j-1] = g/h;
					f += e[j-1]*a(i-1,j-1);
				}
				hh= f/(h+h);
				for(j=1; j<=l; ++j)
				{
				f= a(i-1,j-1);
				e[j-1] = g = e[j-1]- hh*f;
				for(k=1; k<=j; ++k)
					a(j-1,k-1) -= (float)((f*e[k-1] + g*a(i-1,k-1)));
				}
			}
		}
		else
			e[i-1] = a(i-1,l-1);
		d[i-1] = h;
	}

	d[0] = 0;
	e[0] = 0;
	for(i=1; i<=n; ++i)
	{
		l=i-1;
		if(d[i-1] != 0.0)
		{
			for(j=1; j<=l; ++j)
			{
				g=0.0;
				for(k=1;k<=l; ++k)
				
					g+= a(i-1,k-1) * a(k-1,j-1);
					for(k=1; k<=l; ++k)
						a(k-1,j-1) -= (float)(g*a(k-1,i-1));
			}
		}
		d[i-1] = a(i-1,i-1);
		a(i-1,i-1) = 1.0;
		for(j=1; j<=l; ++j)
			a(j-1,i-1) = a(i-1,j-1) = 0.0;
	}
}


//---------------------------------------------------------------------

void Householder:: triadQLI(std::vector<double> &d, std::vector<double> &e,
							int n, SqMatrix &z)
{
	int m, l, iter, i, k;
	double s,r,p,g,f,dd,c,b;
	
	for(i=2; i<=n; ++i)
		e[i-2] = e[i-1];
	e[n-1] = 0.0;
	
	for(l=1; l<=n; ++l)
	{
		iter = 0;
		do
		{
			for(m=l; m<=n-1; ++m)
			{
				dd = fabs(d[m-1]) + fabs(d[m]);
			if( fabs(e[m-1] + dd) == dd )
				break;
			}		
			if(m!=l)
			{
			assert(iter++ <= 30);
			g= (d[l]- d[l-1])/ (2.0*e[l-1]);
			r= pythag(g,1.0);
			g= d[m-1] - d[l-1] + e[l-1]/ (g+sign(r,g));
			s=c=1.0;
			p=0.0;
			for(i=m-1; i>=l; --i)
			{
				f = s* e[i-1];
				b = c*e[i-1];
				e[i] = (r=pythag(f,g));
				if(r==0)
				{
					d[i] -= p;
					e[m-1] = 0.0;
					break;
				}
				s=f/r;
				c=g/r;
				g = d[i] - p;
				r = (d[i-1] - g)*s + 2.0 *c*b;
				d[i] = g+ (p=s*r);
				g = c*r - b;
				for(k=1; k<=n; ++k)
				{
					f = z(k-1,i);
					z(k-1,i) = (float)(s*z(k-1,i-1) + c*f);
					z(k-1,i-1) = (float)(c*z(k-1,i-1) - s*f);
				}
			}
				if(r == 0.0 && i)continue;
				d[l-1] -= p;
				e[l-1] = g;
				e[m-1] = 0.0;
			}
		}while(m!=l);
	}
}
//----------------------------------------------------------------------------- */

//---------------------------------------------------------------------

}