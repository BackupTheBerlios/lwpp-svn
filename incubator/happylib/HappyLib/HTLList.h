
#ifndef _HTLLIST_H
#define _HTLLIST_H

// [DOCUMENTME]

#ifndef _HLDEFS_H
#include <hldefs.h>
#endif

template<class Obj> class List;

template<class Obj> class ListNode
{
	friend class List<Obj>;
protected:
	Obj _obj;
	ListNode * _prev;
	ListNode * _next;
public:
	ListNode(Obj const & _obj) : _obj(_obj), _prev(0), _next(0) {}
	ListNode(Obj const & _obj, ListNode * _prev, ListNode * _next)
		: _obj(_obj), _prev(_prev), _next(_next)
	{
		if (_prev) _prev->_next = this;
		if (_next) _next->_prev = this;
	}
	~ListNode() { if (this) delete _next; }
	// accessors
	ListNode * prev() { if (this) return _prev; return 0; }
	ListNode const * prev() const { if (this) return _prev; return 0; }
	ListNode * next() { if (this) return _next; return 0; }
	ListNode const * next() const { if (this) return _next; return 0; }
	Obj & obj() { return _obj; }
	Obj const & obj() const { return _obj; }
	// mutators
	void setPrevNext(ListNode * p, ListNode * n)
	{
		// detach from current location
		if (_prev) _prev->_next = _next;
		if (_next) _next->_prev = _prev;
		// attach to new location
		_prev = p;
		_next = n;
		if (_prev) _prev->_next = this;
		if (_next) _next->_prev = this;
	}
	// functions
	void roast() { if (this) { delete _obj; _next->roast(); } }
};

template<class Obj> class List
{
protected:
	ListNode<Obj> * _head;
	ListNode<Obj> * _tail;
public:
	List() : _head(0), _tail(0) {}
	~List() { delete _head; }
	// accessors
	ListNode<Obj> * first() { return _head; }
	ListNode<Obj> const * first() const { return _head; }
	// functions
	void clear() { delete _head; _head = _tail = 0; }
	void roast() { _head->roast(); clear(); }
	void insertAtHead(Obj const & o)
	{
		_head = new ListNode<Obj>(o, 0, _head);
		if (!_tail) _tail = _head;
	}
	void insertAtHead(ListNode<Obj> * node)
	{
		node->setPrevNext(0, _head);
		_head = node;
		if (!_tail) _tail = _head;
	}
	void insertAtTail(Obj const & o)
	{
		_tail = new ListNode<Obj>(o, _tail, 0);
		if (!_head) _head = _tail;
	}
	void insertAtTail(ListNode<Obj> * node)
	{
		node->setPrevNext(_tail, 0);
		_tail = node;
		if (!_head) _head = _tail;
	}
	void remove(ListNode<Obj> * node)
	{
		if (node == _head) _head = _head->_next;
		if (node == _tail) _tail = _tail->_prev;
		node->setPrevNext(0, 0);
	}
	void eat(List & list) { delete _head; _head = list._head; list._head = 0; }
	//void append(List const & list)	[FIXME]
	Queue & operator << (Obj const & o) { insertAtTail(o); return *this; }
	//Queue & operator << (List<Obj> const & list) { append(list); return *this; } [FIXME]
};

#endif
