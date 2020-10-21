#include <iostream>
#include <functional>
#include <atomic>

template <typename... Args>
class event_handler {
public:

	typedef std::function<void(Args...)> handler_func_type;
	handler_func_type m_handlerFunc;

	void operator()(Args... params) const{
	
	}
	typedef unsigned int handler_id_type;
	
	explicit event_handler(const handler_func_type & handlerFunc) : 
		m_handlerFunc(handlerFunc)
	{
		m_handlerId = ++m_handlerIdCounter;
	}

	bool operator==(const event_handler& other) const{
		return m_handlerId == other.m_handlerId;
	}

	handler_id_type id() const{
		return m_handlerId;
	}
private:
	handler_id_type m_handlerId;
	static std::atomic_uint m_handlerIdCounter;
};

template<typename... Args> 
std::atomic_uint event_handler<Args...>::m_handlerIdCounter(0);