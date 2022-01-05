//=============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
//=============================================================================

#ifndef _CUESSENCE_HANDLE_H_
#define _CUESSENCE_HANDLE_H_

/*
The class EssenceHandle is a template handle class that represents
reference counted proxy pointer implementation. The handle class
can be used with any other class that needs to support reference
counted handle - object mechanism.

The EssenceHandle class should always instantiated as objects and not
allocated as pointers.

The class to be used with EssenceHandle has to support reference
counting through following functions:
void AddRef() : increments the reference count
int Release() : decrements the reference count, returns updated count
int GetRefCount() : returns the current reference count
*/
template <class T>

class CommonUtilHandle
{
	T *m_Data;

	void bind(T* data) {
		if(m_Data && (0 == m_Data->Release()))
			delete m_Data;

		m_Data = data;
		if(m_Data)
			m_Data->AddRef();
	}

public:
	CommonUtilHandle() : m_Data(0) {
	}

	CommonUtilHandle(T* data) : m_Data(0) {
		bind(data);
	}

	CommonUtilHandle(CommonUtilHandle const& h) : m_Data(0) {
		bind(h.getData());
	}

	~CommonUtilHandle() {
		bind(0);
	}

	//operators overloaded for ease of use..
	CommonUtilHandle& operator = (CommonUtilHandle const& h) {
		if( this != &h )
		{
			bind(h.getData());
		}
		return *this;
	}

	T* operator -> () { return m_Data; }
	T const * operator -> () const { return m_Data; }
	T& operator *() { return *m_Data; }
	T const& operator *() const { return *m_Data; }
	T* getData() { return m_Data; }
	T* getData() const { return m_Data; }
	operator bool() const {
		if(m_Data)
			return (m_Data->GetRefCount() > 0) ? true : false;
		return false;
	}
};


#endif //_ESSENCE_HANDLE_H_
