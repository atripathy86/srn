#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "ForwardDeclaration.hpp"
//#include "List.hpp"
#include "Event.hpp"
 
 
class event_less{
	public:
	 bool operator()(Event * event_ptr1, Event * event_ptr2){

		return (*event_ptr1).time < (*event_ptr2).time ;
	 
	};

};




class Scheduler {

private:
//	ElementHolder<Event>* event_itr;
//	ElementHolder<Event>* run_event_itr;
#ifdef DEBUG
	int i;
#endif
	Infrastructure *If_ptr;


public:
	Tick ticks;
// 	List<Event> event_schedule;
//	List<Event> periodic_schedule;

//methods
	Scheduler(Infrastructure *passed_If_ptr){If_ptr = passed_If_ptr;event_to_compare_ptr =new Event;};
	Tick now() {return ticks;};
	void run(Tick run_for_ticks);

	void execute_event_schedule();
	void execute_periodic_schedule();

	void schedule_event(Event *passed_event_ptr);
	void schedule_periodic(Event *passed_event_ptr);
	void cancel_periodic(Event *passed_event_ptr);
	void modify_periodic(Event *passed_event_ptr);

	~Scheduler();

#ifdef DEBUG_FAULT
	void periodic_schedule_print() { periodic_schedule.print_all_elements();};
#endif

	//These items below are added for the optimized scheduler
	std::multiset<Event*, event_less>::iterator run_event_itr;
	std::multiset<Event* >::iterator run_periodic_itr;

	std::multiset<Event*, event_less> event_schedule;
	std::multiset<Event* > periodic_schedule;

	Event *event_to_compare_ptr;
	Event *event_to_delete_ptr;


};

#endif
