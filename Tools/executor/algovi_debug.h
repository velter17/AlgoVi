#define dbg ;

#include <bits/stdc++.h>

#define color(c) "[color=" << c << "]"
#define DBG_COLOR color("yellow")
#define dbgout cout << DBG_COLOR
#define dend "[color]"
#define dendl dend"\n"
#define dbg_start dbgout;
#define dbg_end std::cout << dend;

#define ARGS_NUM(...) ARGS_NUM_IMPL_((0,__VA_ARGS__,9,8,7,6,5,4,3,2,1))
#define ARGS_NUM_IMPL_(arg) ARGS_NUM_IMPL arg
#define ARGS_NUM_IMPL(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9, N,...) N
#define FUNC_CALL(func, ...) FUNC_CALL_NUM(func, ARGS_NUM(__VA_ARGS__))
#define FUNC_CALL_NUM(func, args) FUNC_CALL_NUM_(func, args)
#define FUNC_CALL_NUM_(func, args) FUNC_CALL_NUM__(func, args)
#define FUNC_CALL_NUM__(func, args) func ## args

#define dbg1(a) dbgout << #a << " = " << (a) << dendl
#define dbg2(a,b) dbgout << #a << " = " << (a) << ", " << #b << " = " << (b) << dendl
#define dbg3(a,b,c) dbgout << #a << " = " << (a) << ", " << #b << " = " << (b) << ", " << #c << " = " << (c) << dendl
#define dbg4(a,b,c,d) dbgout << #a << " = " << (a) << ", " << #b << " = " << (b) << ", " << #c << " = " << (c) << ", " << #d << " = " << (d) << dendl
#define dbg5(a,b,c,d,e) dbgout << #a << " = " << (a) << ", " << #b << " = " << (b) << ", " << #c << " = " << (c) << ", " << #d << " = " << (d) << ", " << #e << " = " << (e) << dendl
#define dbg(...) FUNC_CALL(dbg, __VA_ARGS__)(__VA_ARGS__)

#define element(e) color("#ccccff") << e << dend
#define containers_color color("#ff6666")

template <typename A, typename B>
std::ostream& operator<<(std::ostream& stream, const std::pair <A, B> &p)
{
	stream << containers_color;
	stream << element(p.first) << "," << element(p.second) << "}";
	stream << dend;
	return stream;
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::vector <A>& v)
{
	stream << containers_color;
	stream << "[";
	for(std::size_t i = 0; i < v.size(); ++i)
	{
		stream << element(v[i]);
		if(i != v.size() - 1)
			stream << ", ";
	}
	stream << "]";
	stream << dend;
	return stream;
}

template <typename A, typename B>
std::ostream& operator<<(std::ostream& stream, const std::map<A, B>& mp)
{
	stream << containers_color;
	stream << "map[";
	bool first = true;
	for(const auto& itr : mp)
	{
		if(!first)
			stream << ", ";
		stream << element(itr);
	}
	stream << "]";
	stream << dend;
	return stream;
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::set <A>& st)
{
	stream << containers_color;
	stream << "set[";
	bool first = true;
	for(const auto& itr : st)
	{
		if(!first)
			stream << ", ";
		stream << element(itr);
		first = false;
	}
	stream << "]";
	stream << dend;
	return stream;
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::stack <A>& st)
{
	stream << containers_color;
	stream << "st[";
	std::stack <A> t = st;
	while(!t.empty())
	{
		stream << element(t.top());
		t.pop();
		if(!t.empty())
			stream << " -> ";
	}
	stream << "]";
	stream << dend;
	return stream;
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::queue <A>& q)
{
	stream << containers_color;
	stream << "q[";
	std::queue <A> t = q;
	while(!t.empty())
	{
		stream << element(t.front());
		t.pop();
		if(!t.empty())
			stream << " -> ";
	}
	stream << "]";
	stream << dend;
	return stream;
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::priority_queue <A>& q)
{
	stream << containers_color;
	stream << "pq[";
	std::priority_queue <A> t = q;
	while(!t.empty())
	{
		stream << element(t.front());
		t.pop();
		if(!t.empty())
			stream << " -> ";
	}
	stream << "]";
	stream << dend;
	return stream;
}

template <typename A>
std::ostream& operator<<(std::ostream& stream, const std::deque <A>& q)
{
	stream << containers_color;
	stream << "dq[";
	std::deque <A> t = q;
	while(!t.empty())
	{
		stream << element(t.front());
		t.pop_front();
		if(!t.empty())
			stream << " -> ";
	}
	stream << "]";
	stream << dend;
	return stream;
}