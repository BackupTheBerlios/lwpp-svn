
#ifndef _HLCACHE_H
#define _HLCACHE_H


//// Cache class [DOCUMENTME] ////

template <class Obj>
class Cache
{
private:
	Obj * _array;
	int	_top, _capacity, _current;
public:
	Cache(Cache const &);
	Cache & operator = (Cache const &);
	Cache(int);
	~Cache();
	int count() const { return _top + 1; }
	int isEmpty() const { return _top == -1; }
	void clear() { _top = _current = -1; }
	void init(int);
	inline Obj const & cache(Obj const &);
	Obj & operator [] (int i) { if (_current-i < 0) i -= _top + 1; return _array[_current-i]; }
	Obj const & operator [] (int i) const { if (_current-i < 0) i -= _top + 1; return _array[_current-i]; }
};

template <class Obj>
Cache<Obj>::Cache(int cap) : _top(-1), _capacity(cap), _current(-1)
{
	_array = new Obj[_capacity];
}

template <class Obj>
void Cache<Obj>::init(int cap)
{
	_top = -1;
	_capacity = cap;
	_current = -1;
	delete[] _array;
	_array = new Obj[_capacity];
}

template<class Obj>
Cache<Obj>::Cache(Cache const & s) : _top(s._top), _capacity(s._capacity), _current(s._current)
{
	_array = new Obj[_capacity];
	for (int i = 0; i < _capacity; i ++) _array[i] = s._array[i];
}

template<class Obj>
Cache<Obj> & Cache<Obj>::operator = (Cache<Obj> const & s)
{
	_top = s._top;
	_capacity = s._capacity;
	_current = s._current;
	_array = new Obj[_capacity];
	for (int i = 0; i < _capacity; i ++) _array[i] = s._array[i];
}

template <class Obj>
inline Obj const & Cache<Obj>::cache(Obj const & obj)	// cyclic cache
{
	_current ++;
	if (_current >= _capacity) _current -= _capacity;
	if (_current > _top) _top = _current;
	_array[_current] = obj;
	return obj;
}

template <class Obj>
Cache<Obj>::~Cache()
{
	delete[] _array;
}

#endif
