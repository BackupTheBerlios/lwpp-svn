
#ifndef _HLLIST_H
#define _HLLIST_H

//// List class [DOCUMENTME] ////

template<class Obj> class ListNode
{
public:
	Obj data;
	ListNode * prev;
	ListNode * next;
	ListNode(Obj const & o) : data(o), prev(0), next(0) {}
	ListNode(ListNode const &) { HLASSERT(0, "ListNode copy constructor called!"); }
	ListNode & operator = (ListNode const &) { HLASSERT(0, "ListNode copy constructor called!"); return *this; }
	~ListNode() {}
};

template<class Obj> class List
{
private:
	mutable ListNode<Obj> * current;
	mutable int currentIndex;
public:
	int count;
	ListNode<Obj> * head;
	ListNode<Obj> * tail;
	void addNode(ListNode<Obj> * n)
	{ 
		if (!tail) head = tail = n;
		else
		{
			tail->next = n;
			n->prev = tail;
			tail = n;
		}
		count ++;
		currentIndex = -1;
	}
	void add(Obj const & o) { addNode(new ListNode<Obj>(o)); }
	void remove(ListNode<Obj> * n)
	{
		HLASSERT(n, "List: removing null node!");
		if (n == head) head = n->next;
		if (n == tail) tail = n->prev;
		if (n->next) n->next->prev = n->prev;
		if (n->prev) n->prev->next = n->next;
		delete n;
		HLASSERT(!head->prev && !tail->next, "List: error removing!");
		count --;
		currentIndex = -1;
	}
	void destroy()
	{
		ListNode<Obj> * next;
		for (; head; head = next)
		{
			next = head->next; delete head;
		}
		head = tail = 0;
		count = 0;
		currentIndex = -1;
	}
	Obj operator [] (int i)
	{
		if (currentIndex == -1 || i < currentIndex) { current = head; currentIndex = 0; }
		for (; currentIndex < i; currentIndex ++) current = current->next;
		return current;
	}
	List() : current(0), currentIndex(-1), count(0), head(0), tail(0) {}
	~List()	{ destroy(); }
};

#endif
