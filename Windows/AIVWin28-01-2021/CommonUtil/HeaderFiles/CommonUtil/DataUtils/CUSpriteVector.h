// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//NOTE -This Vector class is designed to consume less memory to use in a large number of instances
//So the size/capacity of this vector is limited to range of unsigned short int i.e.0 to 65,535
#ifndef _SPRITE_VECTOR_H_
#define _SPRITE_VECTOR_H_

#include <iostream>
#include <stdlib.h>

using namespace std;

namespace CommonUtil
{
template < class T >
class SpriteVector
{
	T *m_elements;
	unsigned short int m_size;

public:

	SpriteVector()
	{
		m_size = 0;
		m_elements = nullptr;
	}

	~SpriteVector()
	{
		free(m_elements);
	}

	void push_back(T p)
	{
		++m_size;
		m_elements = (T*)realloc(m_elements,m_size*sizeof(T));
		m_elements[m_size-1] = p;
	}

	T back()const
	{
		return m_elements[m_size-1];
	}

	T begin()const
	{
		return m_elements[0];
	}

	void pop_back()
	{
		m_elements[m_size-1] = 0;
		--m_size;
	}
	
	size_t size()const
	{
		return (size_t)m_size;
	}

	void resize(size_t size)
	{
		for(size_t count = 0; count < size; ++count)
		{
			++m_size;
			m_elements = (T*)realloc(m_elements,m_size*sizeof(T));
			m_elements[m_size-1] = 0;
		}
	}

	T operator[](size_t i)const
	{
		if(i > m_size-1)
		{
			printf("Index is greater than size\n");
			return 0;
		}
		else
			return m_elements[i];
	}

	void erase(size_t index)
	{
		if(index < size())
		{
			for(size_t count = index; count < size(); ++count)
			{
				m_elements[count] = m_elements[count+1];
			}
			--m_size;
		}
	}

	void clear()
	{
		for(size_t count = 0; count < size(); ++count)
		{
			m_elements[count] = 0;
		}
		m_size = 0;
	}

};
}
#endif