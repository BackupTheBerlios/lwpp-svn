
#ifndef _HLSTRING_H
#define _HLSTRING_H

#ifndef _HLTEMPLATES_H
#include "HLTemplates.h"
#endif

#include HLSTDIO_H
#include <string.h>


// [DOCUMENTME] zero-terminated string, with length prefix too

// [FIXME][LATER] make a StringRef that has most of the same functionality, but doesn't own the data

// [FIXME][NOW] really needs to be template<class T> String, where T can be char or wchar_t where available,
//              meaning that string.h is out of the question!
//              Then for conversion from char to wchar_t (or back) we need warnings and functions...
//              also, check how well iostream deals with wchar_t.. it had better work!

class String : public Array<char>
{
private:
	void reverse() {}
public:
	String() : Array<char>(1) { get(0) = 0; }
	String(String const & s) : Array<char>(s) {}
	String const & operator = (String const & s) { Array<char>::operator = (s); return *this; }
	String(char const * s) { set(s, strlen(s)+1); }
	String(Array<char> const & q)
	{
		int len;
		for (len = 0; len < q.count() && q[len]; len ++);
		Array<char> newString(len+1);
		for (int i = 0; i < len; i ++) newString[i] = q[i];
		newString[len] = 0;
		swap(newString);
	}
	String(Queue<char> const & q)
	{
		int len;
		for (len = 0; len < q.count() && q[len]; len ++);
		Array<char> newString(len+1);
		for (int i = 0; i < len; i ++) newString[i] = q[i];
		newString[len] = 0;
		swap(newString);
	}
	String(char const * s, int maxLen)
	{
		int len;
		for (len = 0; len < maxLen && s[len]; len ++);
		Array<char> newString(len+1);
		for (int i = 0; i < len; i ++) newString[i] = s[i];
		newString[len] = 0;
		swap(newString);
	}
	String(double d)
	{
		char buff[128];
		sprintf(buff, "%f", d);
		set(buff, strlen(buff)+1);
	}
	String(int i)
	{
		char buff[128];
		sprintf(buff, "%i", i);
		set(buff, strlen(buff)+1);
	}
	String(unsigned int i)
	{
		char buff[128];
		sprintf(buff, "%u", i);
		set(buff, strlen(buff)+1);
	}
	String(char c)
	{
		init(2);
		get(0) = c;
		get(1) = 0;
	}
	String const & operator = (char const * s) { set(s, strlen(s)+1); return *this; }
	String const & operator = (Array<char> const & a) { return *this = String(a); }
	String const & operator = (Queue<char> const & q) { return *this = String(q); }
	String & operator += (String const & b)
	{
		int i;
		Array<char> newString(length() + b.count());
		for (i = 0; i < length(); i ++) newString[i] = get(i);
		for (i = 0; i < b.count(); i ++) newString[i + length()] = b[i];
		swap(newString);
		return *this;
	}
	String & operator += (char const * b)
	{
		return operator += (String(b));
	}
	String & operator += (char c)
	{
		Array<char> newString(count()+1);
		for (int i = 0; i < count()-1; i ++) newString[i] = get(i);
		newString[count()-1] = c;
		newString[count()] = 0;
		swap(newString);
		return *this;
	}
	void readLine(HLSTD::istream & in)
	{
		if (in.eof())
		{
			*this = "";
			return;
		}
		Queue<char> q;
		while (IsEOL(in.peek())) in.get();		// eat EOL
		while (!IsEOL(in.peek()))				// read until EOL
		{
			char c = in.get();
			if (in.eof()) break;
			q << c;
		}
		*this = q;
	}
	void readWord(HLSTD::istream & in)
	{
		if (in.eof())
		{
			*this = "";
			return;
		}
		Queue<char> q;
		while (IsWhite(in.peek())) in.get();	// eat white
		while (!IsWhite(in.peek()))				// read until white
		{
			char c = in.get();
			if (in.eof()) break;
			q << c;
		}
		*this = q;
	}
	void display(HLSTD::ostream & out) const
	{
		for (int i = 0; i < count()-1; i ++) out << get(i);
	}
	int length() const { return count() - 1; }
	String left(int len) const
	{
		if (len <= 0) return "";
		if (len >= length()) return *this;
		Array<char> s(len+1);
		for (int i = 0; i < len; i ++) s[i] = get(i);
		s[len] = 0;
		return s;
	}
	String right(int len) const
	{
		if (len <= 0) return "";
		if (len >= length()) return *this;
		Array<char> s(len+1);
		for (int i = 0; i < len; i ++) s[i] = get(i+length()-len);
		s[len] = 0;
		return s;
	}
	String before(int count) const
	{
		return left(length()-count);
	}
	String after(int count) const
	{
		return right(length()-count);
	}
	String substr(int first, int last) const
	{
		if (first >= length() || last < 0 || last < first) return "";
		if (first < 0) first = 0;
		if (last >= length()) last = length() - 1;
		Queue<char> s;
		for (int i = first; i <= last; i ++) s << get(i);
		return s;
	}
	String until(char c) const
	{
		Queue<char> s;
		for (int i = 0; i < length() && get(i) != c; i ++) s << get(i);
		return s;
	}
	String until(String const & s) const
	{
		Queue<char> u;
		int slen = s.length();
		for (int i = 0; i + slen <= length() && memcmp(&get(i), s, slen); i ++) u << get(i);
		return u;
	}
	String untilLast(char c) const
	{
		Queue<char> s;
		int i;
		for (i = length() - 1; i >= 0 && get(i) != c; i --);
		for (int j = 0; j < i; j ++) s << get(j);
		return s;
	}
	String untilLast(String const & s) const
	{
		// [OPTIMIZEME]
		String t = *this;
		while (t.contains(s)) t = t.since(s);
		return left(length() - s.length() - t.length());
	}
	String since(char c) const
	{
		Queue<char> s;
		int i;
		for (i = 0; i < length() && get(i) != c; i ++);
		for (++ i; i < length(); i ++) s << get(i);
		return s;
	}
	String since(String const & s) const
	{
		Queue<char> u;
		int i;
		int slen = s.length();
		for (i = 0; i + slen <= length() && memcmp(&get(i), s, slen); i ++);
		for (i += slen; i < length(); i ++) u << get(i);
		return u;
	}
	String sinceLast(char c) const
	{
		Queue<char> s;
		int i;
		for (i = length() - 1; i >= 0 && get(i) != c; i --);
		for (++ i; i < length(); i ++) s << get(i);
		return s;
	}
	String sinceLast(String const & s) const
	{
		// [OPTIMIZEME]
		if (!contains(s)) return *this;
		return since(s).sinceLast(s);
	}
	String reversed() const
	{
		Queue<char> s;
		for (int i = length() - 1; i >= 0; i --) s << get(i);
		return s;
	}
	String lower() const
	{
		String s(*this);
		for (int i = 0; i < s.length(); i ++)
			if (IsUpper(s[i])) s[i] = char(int(s[i]) + int('a') - int('A'));
		return s;
	}
	String stripped(char c) const
	{
		Queue<char> s;
		for (int i = 0; i < length(); i ++) s << get(i);
		while (s.count() && s.front() == c) s.dequeue();
		while (s.count() && s.back() == c) s.unqueue();
		return s;
	}
	String stripped(char const * list) const
	{
		Queue<char> s;
		for (int i = 0; i < length(); i ++) s << get(i);
		while (s.count())
		{
			int c;
			for (c = 0; list[c]; c ++) if (s.front() == list[c]) { s.dequeue(); break; }
			if (!list[c]) break;
		}
		while (s.count())
		{
			int c;
			for (c = 0; list[c]; c ++) if (s.back() == list[c]) { s.unqueue(); break; }
			if (!list[c]) break;
		}
		return s;
	}
	String packed(char c) const
	{
		Queue<char> s;
		int okay = 0;
		for (int i = 0; i < length(); i ++)
		{
			if (get(i) == c)
			{
				if (okay) s << c;
				okay = 0;
			}
			else
			{
				okay = 1;
				s << get(i);
			}
		}
		return s;
	}
	void replace(char from, char to)
	{
		for (int i = 0; i < length(); i ++) if (get(i) == from) get(i) = to;
	}
	int contains(char c) const
	{
		for (int i = 0; i < length(); i ++) if (get(i) == c) return 1;
		return 0;
	}
	int contains(String const & s) const
	{
		int slen = s.length();
		for (int i = 0; i + slen <= length(); i ++) if (!memcmp(&get(i), s, slen)) return 1;
		return 0;
	}
	int find(char c) const
	{
		for (int i = 0; i < length(); i ++) if (get(i) == c) return i;
		return -1;
	}
	int find(String const & s) const
	{
		int slen = s.length();
		for (int i = 0; i + slen <= length(); i ++) if (!memcmp(&get(i), s, slen)) return i;
		return -1;
	}
	int startsWith(String const & s) const
	{
		int slen = s.length();
		if (slen > length()) return 0;
		return (!memcmp(&get(0), s, slen));
	}
	int endsWith(String const & s) const
	{
		int slen = s.length();
		if (slen > length()) return 0;
		return (!memcmp(&get(length()-slen), s, slen));
	}
	static int IsUpper(char c)
	{
		if (c >= 'A' && c <= 'Z') return 1;
		return 0;
	}
	static int IsLower(char c)
	{
		if (c >= 'a' && c <= 'z') return 1;
		return 0;
	}
	static int IsAlphaNum(char c)
	{
		if (c >= 'a' && c <= 'z') return 1;
		if (c >= 'A' && c <= 'Z') return 1;
		if (c >= '0' && c <= '9') return 1;
		return 0;
	}
	static int IsSpace(char c)
	{
		return c == ' ' || c == '\t';
	}
	static int IsEOL(char c)
	{
		return c == '\n' || c == '\r';
	}
	static int IsWhite(char c)
	{
		return IsSpace(c) || IsEOL(c);
	}
};

inline HLSTD::ostream & operator << (HLSTD::ostream & out, String const & s) { s.display(out); return out; }
inline HLSTD::istream & operator >> (HLSTD::istream & in, String & s) { s.readWord(in); return in; }
inline String operator + (String a, String const & b) { return a += b; }
inline String operator + (char const * a, String const & b) { return String(a) += b; }
inline String operator + (String a, char const * b) { return a += b; }
inline bool operator < (String const & a, String const & b) { return strcmp(a, b) < 0; }
inline bool operator == (String const & a, char const * b) { return !strcmp(a, b); }
inline bool operator != (String const & a, char const * b) { return !!strcmp(a, b); }
inline bool operator == (String const & a, String const & b) { return !strcmp(a, b); }
inline bool operator != (String const & a, String const & b) { return !!strcmp(a, b); }

#endif
