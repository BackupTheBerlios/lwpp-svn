
#ifndef _HTL_DEVECTOR
#define _HTL_DEVECTOR

#include <vector>
#include <HLUtils.h>

// [DOCUMENTME] double ended vector

template <class Obj> class devector	// [TESTME]
{
private:
	typedef typename std::vector<Obj>::size_type size_type;
	std::vector<Obj> vec;
	size_type first, count;
public:
	devector() : first(0), count(0) {}
	size_type size() const
	{
		return count;
	}
	void reserve(size_type t)
	{
		vec.reserve(t);
	}
	void defragmentFull()
	{
		// degfragment the vector using 'count' swaps
		size_type beginPos = 0;
		size_type endPos = count;
		while (true)
		{
			// compute how much we should swap this pass
			size_type const swapLeft = first - beginPos;
			size_type const swapRight = endPos - first;
			size_type const swapAmount = Min(swapLeft, swapRight);
			if (!swapAmount) break;	// all done
			// perform the swaps
			for (size_type i = 0; i < swapAmount; ++ i)
				Swap(vec[beginPos + i], vec[endPos - swapAmount + i]);
			// continue on the appropriate side
			if (swapLeft == swapAmount) endPos -= swapAmount;
			else beginPos += swapAmount;
		}
		first = 0;
	}
	void clear()
	{
		vec.clear();
		first = count = 0;
	}
	void push_back(Obj const & o)
	{
		if (count == vec.size())
		{
			defragmentFull();
			vec.push_back(Obj());
		}
		operator[](count ++) = o;
	}
	void push_front(Obj const & o)
	{
		if (count == vec.size())
		{
			defragmentFull();
			vec.push_back(Obj());
		}
		++ count;
		if (!first) first = vec.size() - 1;
		else -- first;
		vec[first] = o;
	}
	void pop_back()
	{
		-- count;
	}
	void pop_front()
	{
		-- count;
		if (++ first == vec.size()) first = 0;
	}
	Obj & operator[](size_type i)
	{
		if ((i += first) >= vec.size()) i -= vec.size();
		return vec[i];
	}
	Obj const & operator[](size_type i) const
	{
		if ((i += first) >= vec.size()) i -= vec.size();
		return vec[i];
	}
	Obj & front()
	{
		return vec[first];
	}
	Obj const & front() const
	{
		return vec[first];
	}
	Obj & back()
	{
		size_type i = count - 1;
		if ((i += first) >= vec.size()) i -= vec.size();
		return vec[i];
	}
	Obj const & back() const
	{
		size_type i = count - 1;
		if ((i += first) >= vec.size()) i -= vec.size();
		return vec[i];
	}
};

#endif
