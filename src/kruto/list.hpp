#ifndef KRLIST_HPP
#define KRLIST_HPP

#include <stdio.h>

namespace kruto {

template< typename T, const int GRANULARITY = 32 >
class List
{
public:
	List(void);
	~List(void);

	int add(const T &pItem);

	void clear(void);
	void dispose(void);

	bool isEmpty(void) const;
	int size(void) const;

	T &operator[](const int pIndex) const;
	T &operator[](const int pIndex);

	T &at(const int pIndex) const;

protected:
	List(const List<T, GRANULARITY> &pOther);
	List<T, GRANULARITY> &operator=(const List<T, GRANULARITY> &pOther);

private:
	void ensureSize(const int pSize);
	void resize(const int pSize);

private:
	int mSize;
	int mCapacity;
	T *mData;
};

template< typename T, const int GRANULARITY > inline
List<T, GRANULARITY>::List(void) : mSize(0), mCapacity(0), mData(NULL)
{
	// empty
}

template< typename T, const int GRANULARITY > inline
List<T, GRANULARITY>::~List(void)
{
	clear();	
}

template< typename T, const int GRANULARITY > inline
int List<T, GRANULARITY>::add(const T &pItem)
{
	ensureSize(mSize + 1);
	mData[mSize] = pItem;
	return mSize++;
}

template< typename T, const int GRANULARITY > inline
void List<T, GRANULARITY>::clear(void)
{
	if(mData != NULL)
	{
		delete [] mData;
		mData = NULL;

		mSize = 0;
		mCapacity = 0;
	}
}

template< typename T, const int GRANULARITY > inline
void List<T, GRANULARITY>::dispose(void)
{
	for(int i=0; i<mSize; i++)
	{
		delete mData[i];
		mData[i] = NULL;
	}
	mSize = 0;
}

template< typename T, const int GRANULARITY > inline
bool List<T, GRANULARITY>::isEmpty(void) const
{
	return mSize == 0;
}

template< typename T, const int GRANULARITY > inline
int List<T, GRANULARITY>::size(void) const
{
	return mSize;	 
}

template< typename T, const int GRANULARITY > inline
T &List<T, GRANULARITY>::operator[](const int pIndex) const
{
	return mData[pIndex];
}

template< typename T, const int GRANULARITY > inline
T &List<T, GRANULARITY>::operator[](const int pIndex)
{
	return mData[pIndex];
}

template< typename T, const int GRANULARITY > inline
T &List<T, GRANULARITY>::at(const int pIndex) const
{
	return mData[pIndex];
}

template< typename T, const int GRANULARITY > inline
void List<T, GRANULARITY>::ensureSize(const int pSize)
{
	if(pSize > mCapacity)
		resize(pSize);
}

template< typename T, const int GRANULARITY > inline
void List<T, GRANULARITY>::resize(const int pSize)
{
	T *oldData = mData;

	int mod = pSize % GRANULARITY;
	mCapacity = (mod == 0) ? pSize : pSize + GRANULARITY - mod;

	mData = new T[mCapacity];

	if(oldData != NULL)
	{
		for(int i=0; i<mSize; i++)
			mData[i] = oldData[i];

		delete [] oldData;
	}
}

}

#endif
