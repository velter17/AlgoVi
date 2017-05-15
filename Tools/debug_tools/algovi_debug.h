#define debug ;

#include <bits/stdc++.h>

#define color(c) "[color=" << c << "]"
#define debug_clr color("yellow")
#define debug_cout cout << debug_clr
#define color_end "[color]"
#define dend color_end"\n"
#define debug_start debug_cout;
#define debug_end std::cout << dend;

#define ARGS_NUM(...) ARGS_NUM_IMPL_((0,__VA_ARGS__,9,8,7,6,5,4,3,2,1))
#define ARGS_NUM_IMPL_(arg) ARGS_NUM_IMPL arg
#define ARGS_NUM_IMPL(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9, N,...) N
#define FUNC_CALL(func, ...) FUNC_CALL_NUM(func, ARGS_NUM(__VA_ARGS__))
#define FUNC_CALL_NUM(func, args) FUNC_CALL_NUM_(func, args)
#define FUNC_CALL_NUM_(func, args) FUNC_CALL_NUM__(func, args)
#define FUNC_CALL_NUM__(func, args) func ## args

#define PRINT1(a) cout << #a << " = " << (a) << dend
#define PRINT2(a,b) cout << #a << " = " << (a) << "," << #b << " = " << (b) << dend
#define PRINT3(a,b,c) cout << #a << " = " << (a) << "," << #b << " = " << (b) << "," << #c << " = " << (c) << dend
#define PRINT4(a,b,c,d) cout << #a << " = " << (a) << "," << #b << " = " << (b) << "," << #c << " = " << (c) << "," << #d << " = " << (d) << dend
#define PRINT5(a,b,c,d,e) cout << #a << " = " << (a) << "," << #b << " = " << (b) << "," << #c << " = " << (c) << "," << #d << " = " << (d) << "," << #e << " = " << (e) << dend
#define PRINT(...) FUNC_CALL(PRINT, __VA_ARGS__)(__VA_ARGS__)

#define ELEM_COLOR color("#ccccff")

template <typename A, typename B>
std::ostream& operator<<(std::ostream& stream, const std::pair <A, B> &p)
{
	return stream << "{" << ELEM_COLOR << p.first << color_end << "," << ELEM_COLOR << p.second << color_end << "}";
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::vector <A>& v)
{
	stream << "[";
	for(std::size_t i = 0; i < v.size(); ++i)
	{
		stream << ELEM_COLOR << v[i] << color_end;
		if(i != v.size() - 1)
			stream << ", ";
	}
	stream << "]";
	return stream;
}

template <typename A, typename B>
std::ostream& operator<<(std::ostream& stream, const std::map<A, B>& mp)
{
	stream << "map[";
	bool first = true;
	for(const auto& itr : mp)
	{
		if(!first)
			stream << ", ";
		stream << ELEM_COLOR << itr << color_end;
	}
	stream << "]";
	return stream;
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::set <A>& st)
{
	stream << "set[";
	bool first = true;
	for(const auto& itr : st)
	{
		if(!first)
			stream << ", ";
		stream << ELEM_COLOR << itr << color_end;
		first = false;
	}
	stream << "]";
	return stream;
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::stack <A>& st)
{
	stream << "st[";
	std::stack <A> t = st;
	while(!t.empty())
	{
		stream << ELEM_COLOR << t.top() << color_end;
		t.pop();
		if(!t.empty())
			stream << " -> ";
	}
	stream << "]";
	return stream;
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::queue <A>& q)
{
	stream << "q[";
	std::queue <A> t = q;
	while(!t.empty())
	{
		stream << ELEM_COLOR << t.front() << color_end;
		t.pop();
		if(!t.empty())
			stream << " -> ";
	}
	stream << "]";
	return stream;
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::priority_queue <A>& q)
{
	stream << "pq[";
	std::priority_queue <A> t = q;
	while(!t.empty())
	{
		stream << ELEM_COLOR << t.front() << color_end;
		t.pop();
		if(!t.empty())
			stream << " -> ";
	}
	stream << "]";
	return stream;
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::deque <A>& q)
{
	stream << "dq[";
	std::deque <A> t = q;
	while(!t.empty())
	{
		stream << ELEM_COLOR << t.front() << color_end;
		t.pop_front();
		if(!t.empty())
			stream << " -> ";
	}
	stream << "]";
	return stream;
}