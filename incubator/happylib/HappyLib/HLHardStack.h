
#ifndef _HLHARDSTACK_H
#define _HLHARDSTACK_H

// [DOCUMENTME]

template<class Obj, int N> class HardStack
{
private:
	Obj _array[N];
	int _top;
public:
	inline HardStack() : _top(-1) {}
	inline int const count() const { return _top + 1; }
	inline bool const isEmpty() const { return _top == -1; }
	inline void push(Obj const o) { _array[++ _top] = o; }
	inline Obj const pop() { return _array[_top --]; }
	inline Obj const top() const { return _array[_top]; }
	inline operator Obj * () { return _array; }
};

#endif
