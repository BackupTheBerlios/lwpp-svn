
#ifndef _HLTEMPLATES_H
#define _HLTEMPLATES_H

//
// this file contains my four favorite templated classes: Stack, Queue, Array, Array2
//

// [DOCUMENTME]

// [FIXME] put more assertions in

#ifndef _HLDEFS_H
#include <hldefs.h>
#endif
#ifndef _HLUTILS_H
#include <hlutils.h>
#endif
#include HLIOSTREAM_H
#include HLSTDLIB_H


template<class Obj> class Array;

template<class Obj> class Stack
{
private:
	Obj * _array;
	int _count, _capacity;
	int isFull() const { return _count == _capacity; }
	void enlarge();
public:
	Stack(int = 0);
	Stack(Stack const &);
	Stack & operator = (Stack const &);
	~Stack();
	int count() const { return _count; }
	int isEmpty() const { return !_count; }
	void push(Obj const &);
	Obj pop();
	Obj & top() { return _array[_count-1]; }
	Obj const & top() const { return _array[_count-1]; }
	Obj & get(int i) { return _array[i]; }
	Obj const & get(int i) const { return _array[i]; }
	Obj & second() { return _array[_count-2]; }
	Obj const & second() const { return _array[_count-2]; }
	void clear();
	void roast();
	void dump(Obj *) const;
	void swap(Stack &);
	void sort(int(*compare)(void const *, void const *)) { qsort(_array, _count, sizeof(Obj), compare); }
	void display(HLSTD::ostream &) const;
	void append(Obj const * s, int count)
	{
		while (_count + count > _capacity) enlarge();
		for (int i = 0; i < count; i ++) _array[_count++] = s[i];
	}
	void append(Stack const & s)
	{
		while (_count + s.count() > _capacity) enlarge();
		for (int i = 0; i < s.count(); i ++) _array[_count++] = s[i];
	}
	Obj & operator[] (int i) { return _array[i]; }
	Obj const & operator[] (int i) const { return _array[i]; }
	operator Obj * () { return _array; }
	operator Obj const * () const { return _array; }
	Stack & operator << (Obj const & o) { push(o); return *this; }
};

template<class Obj> class Queue
{
private:
	Obj * _array;
	int	_front, _count, _capacity;
public:
	Queue(int = 0);
	Queue(Queue const &);
	Queue & operator = (Queue const &);
	~Queue();
	int count() const { return _count; }
	int isEmpty() const { return !_count; }
private:
	int isFull() const { return (_count == _capacity); }
public:
	void enlarge();
	void defragment() const;
	void enqueue(Obj const &);	// insert object at back of queue
	void requeue(Obj const &);	// insert object at front of queue
	Obj dequeue();				// remove object from front of queue
	Obj unqueue();				// remove object from back of queue
	void dequeue(int);			// remove objects from front of queue
	void unqueue(int);			// remove objects from back of queue
	Obj & front() { return _array[_front]; }
	Obj const & front() const { return _array[_front]; }
	Obj & back() { return get(_count-1); }
	Obj const & back() const { return get(_count-1); }
	Obj & get(int);
	Obj const & get(int) const;
	Obj & operator[] (int i) { return get(i); }
	Obj const & operator[] (int i) const { return get(i); }
	operator Obj * () { defragment(); return _array; }
	operator Obj const * () const { defragment(); return _array; }
	void reverse();
	void swap(Queue &);
	void clear();
	void roast();
	void dump(Obj *) const;
	void display(HLSTD::ostream &) const;
	void sort(int(*)(void const *, void const *));
	void sortSubtract(Queue const &, int(*)(void const *, void const *));
	void sortIntersect(Queue const &, int(*)(void const *, void const *));
	int index(Obj const &, int(*)(void const *, void const *) = 0) const;
	void eat(Queue &);
	void append(Obj const * q, int count) { for (int i = 0; i < count; i ++) enqueue(q[i]); }
	void append(Queue const & q) { for (int i = 0; i < q.count(); i ++) enqueue(q[i]); }
	void append(Array<Obj> const &);
	Queue & operator << (Obj const & o) { enqueue(o); return *this; }
	Queue & operator << (Array<Obj> const & o) { append(o); return *this; }
	Queue & operator << (Queue<Obj> const & o) { append(o); return *this; }
};

template<class Obj> class Array
{
protected:
	Obj * _array;
	int	_count;
public:
	Array() : _array(0), _count(0) {}
	Array(int c) : _count(c) { _array = c ? new Obj[c] : 0; }
	Array(Array const &);
	Array(Obj * array, int c) : _array(array), _count(c) { }
	Array(Queue<Obj> & q) : _array(0), _count(0) { eat(q); }
	Array & operator = (Array const &);
	~Array() { delete[] _array; }
	int count() const { return _count; }
	void clear() { delete[] _array; _array = 0; _count = 0;}
	void roast();
	void sort(int(*compare)(void const *, void const *)) { qsort(_array, _count, sizeof(Obj), compare); }
	void init(int c) { if (c == _count) return; delete[] _array; _count = c; _array = new Obj[c]; }
	void resize(int c)
	{
		if (c == _count) return;
		Obj * array = new Obj[c];
		for (int i = 0; i < c && i < _count; i ++) array[i] = _array[i];
		delete[] _array;
		_count = c;
		_array = array;
	}
	void fill(Obj const & o) { for (int i = 0; i < _count; i ++) _array[i] = o; }
	void copy(Queue<Obj> const & q);
	void eat(Array & a);
	void eat(Queue<Obj> & q);
	void eat(Stack<Obj> & s);
	void eat(Obj * array, int c) { clear(); _array = array; _count = c; }
	Obj & get(int i) { return _array[i]; }
	Obj const & get(int i) const { return _array[i]; }
	Obj & last() { return _array[_count-1]; }
	Obj const & last() const { return _array[_count-1]; }
	void set(Obj const * array, int c) { init(c); for (int i = 0; i < c; i ++) _array[i] = array[i]; }
	void get(Obj * array, int c) const { for (int i = 0; i < c && i < _count; i ++) array[i] = _array[i]; }
	Array & operator << (Queue<Obj> & q) { eat(q); return *this; }
	int index(Obj const & o) const { return &o - _array; }
	void swap(Array &);
	operator Obj * () { return _array; }
	operator Obj const * () const { return _array; }
	void display(HLSTD::ostream &) const;
	void reverse() { for (int i = 0; i < _count/2; i ++) Swap(_array[i], _array[_count-i-1]); }
};

template<class Obj> class CyclicArray : public Array<Obj>
{
public:
	CyclicArray() {}
	CyclicArray(int c) : Array<Obj>(c) {}
	CyclicArray(Array<Obj> const & a) : Array<Obj>(a) {}
	CyclicArray(Obj const * array, int c) : Array<Obj>(array, c) { }
	CyclicArray(Queue<Obj> & q) : Array<Obj>(q) {}
	CyclicArray & operator = (Array<Obj> const & a) { Array<Obj>::operator=(a); return *this; }
	CyclicArray & operator = (CyclicArray const & a) { Array<Obj>::operator=(a); return *this; }
	Obj & get(int i) { return _array[CyclicMod(i, _count)]; }
	Obj const & get(int i) const { return _array[CyclicMod(i, _count)]; }
	Obj & operator[] (int i) { return _array[CyclicMod(i, _count)]; }
	Obj const & operator[] (int i) const { return _array[CyclicMod(i, _count)]; }
};

template <class Obj> class Array2
{
private:
	Obj * _array;
	int _xSize, _ySize;
public:
	Array2() : _array(0), _xSize(0), _ySize(0) {}
	Array2(int x, int y) : _array(0), _xSize(0), _ySize(0) { init(x, y); }
	Array2(Array2 const &);
	Array2 & operator = (Array2 const &);
	~Array2() { clear(); }
	int xSize() const { return _xSize; }
	int ySize() const { return _ySize; }
	void clear();
	void roast();
	void init(int, int);
	void fill(Obj const &);
	void swap(Array2 &);
	void eat(Array2 & a);
	void copyMajor(Array2 const &, int, int);
	void copyMinor(Array2 const &, int, int);
	Obj * getPtr() { return _array; }
	Obj const * getPtr() const { return _array; }
	Obj & get(int x, int y) { return _array[x * _ySize + y]; }
	Obj const & get(int x, int y) const { return _array[x * _ySize + y]; }
	Obj * const operator [] (int x)
	{
		HLASSERT(x >= 0 && x < _xSize, "Array2: access out of bounds");
		return &_array[_ySize * x];
	}
	Obj const * const operator [] (int x) const
	{
		HLASSERT(x >= 0 && x < _xSize, "Array2: access out of bounds");
		return &_array[_ySize * x];
	}
	void display(HLSTD::ostream &) const;
};


//// Stack Implementation ////

template<class Obj>
Stack<Obj>::Stack(int c) : _array(0), _count(0), _capacity(c)
{
	if (_capacity) _array = new Obj[_capacity];
}

template<class Obj>
Stack<Obj>::Stack(Stack const & s) : _array(0), _count(s._count), _capacity(s._capacity)
{
	if (_capacity)
	{
		_array = new Obj[_capacity];
		s.dump(_array);
	}
}

template<class Obj>
Stack<Obj> & Stack<Obj>::operator = (Stack<Obj> const & s)
{
	if (&s == this) return *this;
	delete[] _array;
	_array = 0;
	_count = s._count;
	_capacity = s._capacity;
	if (_capacity)
	{
		_array = new Obj[_capacity];
		s.dump(_array);
	}
	return *this;
}

template<class Obj>
Stack<Obj>::~Stack()
{
	delete[] _array;
}

template<class Obj>
void Stack<Obj>::clear()
{
	_count = 0;
}

template<class Obj>
void Stack<Obj>::enlarge()	// double the capacity.
{
	int newCapacity = _capacity ? _capacity * 2 : 4;
	Obj * newArray = new Obj[newCapacity];
	// copy the old array into the new array.
	dump(newArray);
	delete[] _array;
	_array = newArray;
	_capacity = newCapacity;
}

template<class Obj>
void Stack<Obj>::push(Obj const & o)
{
	if (isFull()) enlarge();
	_array[_count++] = o;
}

template<class Obj>
Obj Stack<Obj>::pop()
{
	return _array[--_count];
}

template<class Obj>
void Stack<Obj>::dump(Obj * ptr) const
{
	for (int i = 0; i < _count; i ++) ptr[i] = _array[i];
}

template<class Obj>
void Stack<Obj>::swap(Stack<Obj> & b)
{
	Swap(_array, b._array);
	Swap(_count, b._count);
	Swap(_capacity, b._capacity);
}

template <class Obj>
void Stack<Obj>::display(HLSTD::ostream & out) const
{
	out << count() << " items:" << endl;
	for (int t = 0; t < _count; t++) out << _array[t] << endl;
}

template<class Obj>
void Stack<Obj>::roast()
{
	for (int o = 0; o < count(); o ++) delete get(o);
	clear();
}

template<class Obj>
HLSTD::ostream & operator << (HLSTD::ostream & out, Stack<Obj> & s)
{
	s.display(out);
	return out;
}

template <class Obj>
inline int operator == (Stack<Obj> const & a, Stack<Obj> const & b)
{
	if (a.count() != b.count()) return 0;
	for (int i = 0; i < a.count(); i ++) if (a[i] != b[i]) return 0;
	return 1;
}

template <class Obj>
inline int operator != (Stack<Obj> const & a, Stack<Obj> const & b) { return !(a == b); }

template <class Obj>
HLSTD::ostream & operator << (HLSTD::ostream & out, Stack<Obj> const & s)
{
	s.display(out);
	return out;
}


//// Queue Implementation ////

template<class Obj>
Queue<Obj>::Queue(int c) : _front(0), _count(0), _capacity(c)
{
	_array = new Obj[_capacity];
}

template<class Obj>
Queue<Obj>::Queue(Queue const & q) : _front(0), _count(q._count), _capacity(q._capacity)
{
	_array = new Obj[_capacity];
	q.dump(_array);
}

template<class Obj>
Queue<Obj> & Queue<Obj>::operator = (Queue<Obj> const & q)
{
	if (&q == this) return *this;
	delete[] _array;
	_front = 0;
	_count = q._count;
	_capacity = q._capacity;
	_array = new Obj[_capacity];
	q.dump(_array);
	return *this;
}

template<class Obj>
Queue<Obj>::~Queue()
{
	delete[] _array;
}

template<class Obj>
void Queue<Obj>::clear()
{
	_count = 0;
}

template<class Obj>
void Queue<Obj>::defragment() const
{
	if (!_front) return;	// already defragged
	Obj * newArray;
	newArray = new Obj[_capacity];
	// copy the old array into the new array.
	dump(newArray);
	delete[] _array;
	((Queue<Obj>*)this)->_array = newArray;
	((Queue<Obj>*)this)->_front = 0;
}

template<class Obj>
void Queue<Obj>::enlarge()	// double the capacity.
{
	Obj * newArray;
	int newCapacity = _capacity ? _capacity * 2 : 4;
	newArray = new Obj[newCapacity];
	// copy the old array into the new array.
	dump(newArray);
	delete[] _array;
	_array = newArray;
	_front = 0;
	_capacity = newCapacity;
}

template<class Obj>
void Queue<Obj>::enqueue(Obj const & o)
{
	if (isFull()) enlarge();
	int c = _front + _count;
	if (c >= _capacity) c -= _capacity;
	_array[c] = o;
	_count ++;
}

template<class Obj>
void Queue<Obj>::requeue(Obj const & o)
{
	if (isFull()) enlarge();
	if (!_front) _front = _capacity - 1;
	else _front --;
	_array[_front] = o;
	_count ++;
}

template<class Obj>
Obj Queue<Obj>::dequeue()
{
	Obj r = _array[_front];
	if (++_front == _capacity) _front = 0;
	_count --;
	return r;
}

template<class Obj>
Obj Queue<Obj>::unqueue()
{
	int c = _front + _count - 1;
	if (c >= _capacity) c -= _capacity;
	Obj r = _array[c];
	_count --;
	return r;
}

template<class Obj>
void Queue<Obj>::dequeue(int n)
{
	if ((_front += n) >= _capacity) _front -= _capacity;
	_count -= n;
}

template<class Obj>
void Queue<Obj>::unqueue(int n)
{
	_count -= n;
}

template<class Obj>
Obj & Queue<Obj>::get(int i)
{
	int c = _front + i;
	if (c >= _capacity) c -= _capacity;
	return _array[c];
}

template<class Obj>
Obj const & Queue<Obj>::get(int i) const
{
	int c = _front + i;
	if (c >= _capacity) c -= _capacity;
	return _array[c];
}

template<class Obj>
void Queue<Obj>::dump(Obj * ptr) const
{
	for (int t = 0; t < _count; t ++) ptr[t] = get(t);
}

template<class Obj>
void Queue<Obj>::append(Array<Obj> const & a)
{
	for (int i = 0; i < a.count(); i ++) enqueue(a[i]);
}

template<class Obj>
void Queue<Obj>::eat(Queue & q)
{
	delete[] _array;
	_array = q._array;
	_front = q._front;
	_count = q._count;
	_capacity = q._capacity;
	q._array = 0;
	q._front = 0;
	q._count = 0;
	q._capacity = 0;
}

template<class Obj>
void Queue<Obj>::swap(Queue<Obj> & b)
{
	Swap(_array, b._array);
	Swap(_capacity, b._capacity);
	Swap(_front, b._front);
	Swap(_count, b._count);
}

template<class Obj>
void Queue<Obj>::roast()
{
	for (int o = 0; o < count(); o ++) delete get(o);
	clear();
}

template<class Obj>
void Queue<Obj>::reverse()
{
	Queue<Obj> q;
	while (count()) q.enqueue(unqueue());
	swap(q);
}

template<class Obj>
int Queue<Obj>::index(Obj const & o, int(*compare)(void const *, void const *)) const
{
	int i;
	for (i = 0; i < count(); i ++)
	{
		if (compare) { if (!compare(&o, &get(i))) return i; }
		else if (!memcmp(&o, &get(i), sizeof(Obj))) return i;
	}
	return -1;
}

template <class Obj>
void Queue<Obj>::display(HLSTD::ostream & out) const
{
	out << count() << " items:" << endl;
	for (int t = 0; t < count(); t++) out << get(t) << endl;
}

template <class Obj>
void Queue<Obj>::sort(int(*compare)(void const *, void const *))
{
	defragment();
	qsort(_array, count(), sizeof(Obj), compare);
}

template <class Obj>
void Queue<Obj>::sortSubtract(Queue const & bQ, int(*compare)(void const *, void const *))
{
	Array<Obj> a(*this);
	Array<Obj> b; b.copy(bQ);
	a.sort(compare);
	b.sort(compare);
	Queue<Obj> c;
	int ac, bc;
	for (ac = bc = 0; ac < a.count(); ac ++)
	{
		// skip up to possible match
		while (bc < b.count() && compare(&a[ac], &b[bc]) > 0) bc ++;
		if (bc == b.count() || compare(&a[ac], &b[bc]) < 0) c << a[ac];	// no match
	}
	swap(c);
}

template <class Obj>
void Queue<Obj>::sortIntersect(Queue const & bQ, int(*compare)(void const *, void const *))
{
	Array<Obj> a(*this);
	Array<Obj> b; b.copy(bQ);
	a.sort(compare);
	b.sort(compare);
	Queue<Obj> c;
	int ac, bc;
	for (ac = bc = 0; ac < a.count(); ac ++)
	{
		// skip up to possible match
		while (bc < b.count() && compare(&a[ac], &b[bc]) > 0) bc ++;
		if (bc < b.count() && !compare(&a[ac], &b[bc])) c << a[ac];	// match
	}
	swap(c);
}

template <class Obj>
HLSTD::ostream & operator << (HLSTD::ostream & out, Queue<Obj> const & s)
{
	s.display(out);
	return out;
}

template <class Obj>
inline int operator == (Queue<Obj> const & a, Queue<Obj> const & b)
{
	if (a.count() != b.count()) return 0;
	for (int i = 0; i < a.count(); i ++) if (a[i] != b[i]) return 0;
	return 1;
}

template <class Obj>
inline int operator != (Queue<Obj> const & a, Queue<Obj> const & b) { return !(a == b); }


//// Array Implementation ////

template <class Obj>
inline int operator == (Array<Obj> const & a, Array<Obj> const & b)
{
	if (a.count() != b.count()) return 0;
	for (int i = 0; i < a.count(); i ++) if (a[i] != b[i]) return 0;
	return 1;
}

template <class Obj>
inline int operator != (Array<Obj> const & a, Array<Obj> const & b) { return !(a == b); }

template<class Obj>
Array<Obj>::Array(Array const & a) : _count(a._count)
{
	_array = new Obj[_count];
	for (int i = 0; i < _count; i ++) _array[i] = a._array[i];
}

template<class Obj>
Array<Obj> & Array<Obj>::operator = (Array const & a)
{
	if (&a == this) return *this;
	_count = a._count;
	delete[] _array;
	_array = new Obj[_count];
	for (int i = 0; i < _count; i ++) _array[i] = a._array[i];
	return *this;
}

template<class Obj>
void Array<Obj>::eat(Array<Obj> & a)
{
	_count = a._count;
	a._count = 0;
	delete[] _array;
	_array = a._array;
	a._array = 0;
}

template<class Obj>
void Array<Obj>::eat(Queue<Obj> & q)
{
	_count = q.count();
	delete[] _array;
	_array = new Obj[_count];
	q.dump(_array);
	q.clear();
}

template<class Obj>
void Array<Obj>::eat(Stack<Obj> & s)
{
	_count = s.count();
	delete[] _array;
	_array = new Obj[_count];
	s.dump(_array);
	s.clear();
}

template<class Obj>
void Array<Obj>::copy(Queue<Obj> const & q)
{
	_count = q.count();
	delete[] _array;
	_array = new Obj[_count];
	q.dump(_array);
}

template<class Obj>
void Array<Obj>::roast()
{
	for (int o = 0; o < _count; o ++) delete _array[o];
	clear();
}

template <class Obj>
void Array<Obj>::display(HLSTD::ostream & out) const
{
	out << count() << " items:" << endl;
	for (int t = 0; t < _count; t++) out << _array[t] << endl;
}

template<class Obj>
void Array<Obj>::swap(Array & a)
{
	Swap(_array, a._array);
	Swap(_count, a._count);
}

template <class Obj>
HLSTD::ostream & operator << (HLSTD::ostream & out, Array<Obj> const & s)
{
	s.display(out);
	return out;
}


//// Array2 Implementation ////

template<class Obj>
Array2<Obj>::Array2(Array2 const & a) : _array(0), _xSize(0), _ySize(0)
{
	init(a._xSize, a._ySize);
	for (int i = 0; i < _xSize * _ySize; i ++) _array[i] = a._array[i];
}

template<class Obj>
Array2<Obj> & Array2<Obj>::operator = (Array2 const & a)
{
	if (&a == this) return *this;
	init(a._xSize, a._ySize);
	for (int i = 0; i < _xSize * _ySize; i ++) _array[i] = a._array[i];
	return *this;
}

template<class Obj>
void Array2<Obj>::clear()
{
	delete[] _array;
	_xSize = _ySize = 0;
	_array = 0;
}

template<class Obj>
void Array2<Obj>::init(int x, int y)
{
	if (_xSize == x && _ySize == y) return; 
	delete[] _array;
	_xSize = x;
	_ySize = y;
	_array = new Obj[x * y];
}

template<class Obj>
void Array2<Obj>::fill(Obj const & o)
{
	for (int i = 0; i < _xSize * _ySize; i ++) _array[i] = o;
}

template<class Obj>
void Array2<Obj>::roast()
{
	for (int i = 0; i < _xSize * _ySize; i ++) delete _array[i];
	clear();
}

template<class Obj>
void Array2<Obj>::swap(Array2 & a)
{
	Swap(_array, a._array);
	Swap(_xSize, a._xSize);
	Swap(_ySize, a._ySize);
}

template<class Obj>
void Array2<Obj>::eat(Array2 & a)
{
	_xSize = a._xSize;
	a._xSize = 0;
	_ySize = a._ySize;
	a._ySize = 0;
	delete[] _array;
	_array = a._array;
	a._array = 0;
}

template<class Obj>
void Array2<Obj>::copyMajor(Array2 const & a, int m, int am)
{
	for (int i = 0; i < _ySize && i < a._ySize; i ++) get(m, i) = a.get(am, i);
}

template<class Obj>
void Array2<Obj>::copyMinor(Array2 const & a, int m, int am)
{
	for (int i = 0; i < x && i < a.x; i ++) get(m, i) = a.get(am, i);
}

template <class Obj>
void Array2<Obj>::display(HLSTD::ostream & out) const
{
	out << _xSize << " x " << _ySize << " :" << endl;
	for (int t = 0; t < _xSize; t ++)
	{
		for (int i = 0; i < _ySize; i ++) out << get(t, i) << " ";
		out << endl;
	}
}

template <class Obj>
HLSTD::ostream & operator << (HLSTD::ostream & out, Array2<Obj> const & s)
{
	s.display(out);
	return out;
}

#endif
