#ifndef SSIGNAL_H_
#define SSIGNAL_H_

#include"Configer.h"
#define emit

template<typename _Fty>
class SSignal;
/*统计SSignal需要的参数个数*/
//template<typename T> struct count_arg;
//
//template<typename R, typename ...Args>
//struct count_arg<SSignal<R(Args ...)>>
//{
//	static const size_t value = sizeof ...(Args);
//};
/*统计std::funcation需要的参数个数*/
template<typename T> struct count_arg;

template<typename R, typename ...Args>
struct count_arg<std::function<R(Args ...)>>
{
	static const size_t value = sizeof ...(Args);
};


template<typename _Fty>
class SSignal
{
public:
	using ConnectionID = size_t;
	SSignal();
	SSignal::ConnectionID connect(const std::function<_Fty>& func);
	void disConnect(const ConnectionID& id);
	void disConnect(const std::function<_Fty>& func);
	template<typename ...Args>
	void operator()(Args...args);	//发射信号
private:
	std::vector<std::function<_Fty>> m_slots;
};


#endif // !FONT_H_

template<typename _Fty>
inline SSignal<_Fty>::SSignal()
{
}

template<typename _Fty>
inline typename SSignal<_Fty>::ConnectionID SSignal<_Fty>::connect(const std::function<_Fty>& func)
{
	m_slots.push_back(func);
	return ConnectionID(m_slots.size() -1);
}

template<typename _Fty>
inline void SSignal<_Fty>::disConnect(const ConnectionID& id)
{
	m_slots.erase(m_slots.begin() + id);
}

template<typename _Fty>
inline void SSignal<_Fty>::disConnect(const std::function<_Fty>& func)
{
	auto it = std::find_if(m_slots.begin(), m_slots.end(), [=](const std::function<_Fty>& f)
		{
			return f.target_type() == func.target_type();
		});
	if (it != m_slots.end())
	{
		m_slots.erase(it);
	}
}

template<typename _Fty>
template<typename ...Args>
inline void SSignal<_Fty>::operator()(Args ...args)
{
	constexpr int argsCnt = sizeof...(Args);
	constexpr int needArgsCnt = count_arg<std::function<_Fty>>::value;
	static_assert(argsCnt == needArgsCnt,"signal and slot The parameters do not match");
	

	for (std::function<_Fty>& slot : m_slots)
	{
		//int argsCnt = sizeof...(Args);
		//int needArgsCnt = count_arg<decltype(slot)>::value;
		//if (needArgsCnt != argsCnt)
		//{
		//	printf("槽函数需要%d个参数,实际传递了%d个参数", needArgsCnt, argsCnt);
		//	//std::cerr << "槽函数需要"<< needArgsCnt<<"" << std::endl;
		//	return;
		//}

		slot(std::forward<Args>(args)...);
	}
}
