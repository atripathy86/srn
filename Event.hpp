#ifndef EVENT_HPP
#define EVENT_HPP
//#include "Functor.hpp"
//#include "Node.hpp"

class Event {
	public:
	Tick time;
	Tick start_time;
	Functor *handler;

#ifdef DEBUG
	Node *node_ptr;
	//Message *message_to_handle_ptr;
	unsigned int event_id;
#endif

	Event();
	Event(Functor &passedFunctor);

	~Event();
	
	friend bool operator==(const Event& e1,const Event& e2){
           return e1.time==e2.time && e1.start_time==e2.start_time && e1.handler ==e2.handler;
		}
};

#endif //#ifndef EVENT_HPP

