
#ifndef _HLNTREE_H
#define _HLNTREE_H

// [DOCUMENTME]

#ifndef _HLTEMPLATES_H
#include <hltemplates.h>
#endif

template<int N, class Obj> class NTree
{
private:
	Array<Obj> array;
public:
	Obj & operator[](int index)
	{
		while (index >= array.count()) addLevel();	// expand if necessary
		return array[index];
	}
	Obj const & operator[](int index) const
	{
		return array[index];
	}
	int depth(int index) const
	{
		int d = 0;
		while (index)
		{
			d ++;
			index = (index - 1) / N;
		}
		return d;
	}
	int childIndex(int parentIndex, int child) const
	{
		return parentIndex * N + child + 1;
	}
	void addLevel()
	{
		if (!array.count())
		{
			array.init(1);
			return;
		}
		Array<Obj> newArray(array.count() * N);
		for (int i = 0; i < array.count(); i ++)
			newArray[i] = array[i];
		array.swap(newArray);
	}
	/* [FIXME] we may want to pack, need better test
	void pack()
	{
		Obj o();
		int i;
		for (i = array.count() - 1; i >= 0; i --)
			if (array[i] != o) break;
		Array<Obj> newArray(i + 1);
		for (i = 0; i < newArray.count(); i ++)
			newArray[i] = array[i];
		array.swap(newArray);
	}
	*/
};

#endif
