#include <string>
#include <memory>

#include "parser.h"
#include "event_manager.h"
#include "gpl_statement.h"
#include "gpl_assert.h"

using namespace std;

/* static */ Event_manager *Event_manager::m_instance = 0;

/* static */ Event_manager * Event_manager::instance()
{
  if (!m_instance)
    m_instance = new Event_manager();
  return m_instance;
}

Event_manager::Event_manager()
{
}

Event_manager::~Event_manager()
{
	for(EventHandlerMap::iterator map_it = _eventMap.begin(); 
				map_it != _eventMap.end(); map_it++)
	{
		EventHandlerList* pList = map_it->second;
		pList->clear();
		delete pList;
	}
	_eventMap.clear();
}


void Event_manager::execute_handlers(Window::Keystroke keystroke)
{
	TRACE_VERBOSE("Event_manager::execute_handlers - Keystroke " << (int)keystroke)

	EventHandlerMap::iterator map_it = _eventMap.find(keystroke);
	if(map_it == _eventMap.end()) return;

	TRACE_VERBOSE("Event_manager::execute_handlers - Event Handlers Found");

	EventHandlerList* pList = map_it->second;
	TRACE_VERBOSE("Event_manager::execute_handlers - Count: " << pList->size())

	int i = 0;
	for(EventHandlerList::iterator handler_it = pList->begin(); 
				handler_it != pList->end(); handler_it++)
	{
		TRACE_VERBOSE("Event_manager::execute_handlers - Executing Handler #" << i++)
		(*handler_it)->execute();		
	}
}

void Event_manager::add_handler(Window::Keystroke keystroke, const std::shared_ptr<statement_block>& handler)
{
	TRACE_VERBOSE("Event_manager::add_handler - Keystroke: " << (int)keystroke)

	EventHandlerMap::iterator map_it = _eventMap.find(keystroke);
	if(map_it == _eventMap.end())
	{
		EventHandlerList* pList = new EventHandlerList();
		pList->push_back(handler);
		_eventMap.insert(std::make_pair(keystroke, pList));
	}
	else
	{
		EventHandlerList* pList = map_it->second;
		pList->push_back(handler);	
	}
}

