#include <iostream>
#include <list>

#include "event_handler.h"

template<typename... Args> class event{
public:
	typedef event_handler<Args...> handler_type;

protected:
	typedef std::list<handler_type> handler_collection_type;

private:
	handler_collection_type m_handlers;

	typename handler_type::handler_id_type add(const handler_type& handler){
		std::lock_guard<std::mutex> lock(m_handlersLocker);

		m_handlers.push_back(handler);
		return handler.id();
	}
	inline typename handler_type::handler_id_type add(
		const typename handler_type::handler_func_type& handler){
			return add(handler_type(handler));
	}

	bool remove(const handler_type& handler){
		std::lock_guard<std::mutex> lock(m_handlersLocker);

		auto it = std::find(m_handlers.begin(), m_handlers.end(), handler);
		if (it != m_handlers.end()){
			m_handlers.erase(it);
			return true;
		}
		return false;
	}
	bool remove_id(const typename handler_type::handler_id_type& handlerId){
		std::lock_guard<std::mutex> lock(m_handlersLocker);

		auto it = std::find_if (m_handlers.begin(), m_handlers.end(),
			[handlerId](const handler_type& handler){return handler.id() == handlerId; });
		if(it !=m_handlers.end()){
			m_handlers.erase(it);
			return true;
		}
		return false;
	}

	mutable std::mutex m_handlersLocker;
};