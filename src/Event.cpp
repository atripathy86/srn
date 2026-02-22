#include "ForwardDeclaration.hpp"
//#include "Allheader.hpp"
#include "Event.hpp"
#include "Functor.hpp"
#include "Node.hpp"
#include "Infrastructure.hpp"

Event::Event()
{
	time = 0;
	start_time =0;
	handler = NULL;

#ifdef DEBUG
	node_ptr = NULL;
	//Message *message_to_handle_ptr;
	event_id = 0;
#endif

	return;
};

Event::Event(Functor &passedFunctor){
		handler = &passedFunctor;
#ifdef DEBUG
	//message_to_handle_ptr = 0;
	event_id = 0;
#endif

	return;
};


Event::~Event()
	{
		delete handler;
		return;
	};

