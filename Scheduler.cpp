#include "ForwardDeclaration.hpp"
#include "Scheduler.hpp"
#include "Event.hpp"
#include "Node.hpp"

#include "Functor.hpp" //Added, but remove if this gives problem, probably this is ok
#include "IO_Reporter.hpp"


Scheduler::~Scheduler()
	{
#ifdef DEBUG
	printf("Started cleaning up for Scheduler\nStarted cleaning up the event schedule\n%u residual events needs cleaning up\n",event_schedule.size() );
#endif
	run_event_itr = event_schedule.begin();
	for (unsigned int fi = event_schedule.size() ;fi>0 ;fi--,run_event_itr++ )
	{
		delete *run_event_itr;
	}
	event_schedule.clear();
	/*
		//Fire all the residual events to clean them up
		
		while (!event_schedule.empty())
		{
				(*((event_itr->content()).handler))();
				event_itr = event_schedule.erase(event_itr);
		}
	*/
#ifdef DEBUG
	printf("Finished cleaning up the event schedule\nStarted cleaning up the periodic schedule\n");
#endif
	run_periodic_itr = periodic_schedule.begin();
	for (unsigned int fi = periodic_schedule.size() ;fi>0 ;fi--,run_periodic_itr++ )
	{
		delete *run_periodic_itr;
	}
	periodic_schedule.clear();

#ifdef DEBUG
	printf("Finished cleaning up the periodic schedule\nFinished cleaning up for Scheduler\n");
#endif
	return;
};






//Scheduler methods
	//run()
	void Scheduler::run(Tick run_for_ticks){

		//std::list< Event >::iterator run_event_itr;

		event_to_compare_ptr->time = ticks=0;

#ifdef DEBUG_SCHED1
		printf("run():Entering, run_for_ticks is %u\n", run_for_ticks);
#endif

		//Event *event_to_return_ptr;

		while ( (ticks < run_for_ticks)   )
		{
#ifdef DEBUG
			//Remove later, only for Test
			std::cout<< "Ticks: "<< ticks <<std::endl; 
#endif
			execute_event_schedule();

			//if (ticks < run_for_ticks)
			execute_periodic_schedule();

			ticks++,event_to_compare_ptr->time++;

		}//End of main while loop

		return;
	} //End of run()





inline void Scheduler::execute_event_schedule()
{
	//printf("Event schedule execution starting....\n");
	
			//Find the events to execute
			for (run_event_itr = event_schedule.find(event_to_compare_ptr);
				run_event_itr != event_schedule.end() && (*run_event_itr)->time == ticks ; 
				)
			{
				//( *(    (run_event_itr->content()).handler    ))();
				// (*(*run_event_itr)).handler();
				
				(*(    (*(*run_event_itr)).handler    ))();

				event_to_delete_ptr = *run_event_itr;
				event_schedule.erase(run_event_itr++);
				 delete event_to_delete_ptr;

			}//End of running events
		
		//printf("Event schedule execution over\n");
	return;
}


inline void Scheduler::execute_periodic_schedule(){
	
		/* 
		while ( !(run_event_itr == periodic_schedule.end())  && 
 						(!( ( (ticks - (run_event_itr->content()).start_time )% (run_event_itr->content()).time ) ==0 )  
						||	(ticks < (run_event_itr->content()).start_time)  
						)
		)	
		 */
		//printf("Periodic schedule execution starting....\n");

			//Find the events to execute
			// run_periodic_itr is advanced to next BEFORE calling the handler because the handler may call
			// cancel_periodic() which erases and deletes the current element, invalidating run_periodic_itr.
			// Saving the next iterator first keeps iteration safe even when the current element is removed.
			//for (run_periodic_itr = periodic_schedule.begin(); // old: for-loop increment happens after handler, unsafe if handler erases current element
			//		run_periodic_itr != periodic_schedule.end() ;
			//		run_periodic_itr++
			//	)
			for (run_periodic_itr = periodic_schedule.begin();
					run_periodic_itr != periodic_schedule.end() ;
				)
 				{
					std::multiset<Event*, event_less>::iterator next_periodic_itr = std::next(run_periodic_itr); // save next before potential erase by handler

					if(  
							ticks && (
							     ( ( (ticks - (*run_periodic_itr)->start_time )% ( (*run_periodic_itr)->time ) ==0 )  ) ||
										(ticks <  (*run_periodic_itr)->start_time) 
							)  || 
							!ticks && !(*run_periodic_itr)->start_time
					  )
					{
					//( *(    (run_event_itr->content()).handler    ))();
					//	 (*(*run_periodic_itr)).handler();

						(*(  (*(*run_periodic_itr)).handler   ))();

					}//end if

					run_periodic_itr = next_periodic_itr; // advance using pre-saved iterator

				}//End of running events

			//printf("Periodic schedule execution over\n");

};




//All the methods below have been moved to hpp file for inlining, and ability to get called by other files

void  Scheduler::schedule_event(Event *passed_event_ptr){
	
		event_schedule.insert( passed_event_ptr );
		return;
};



void Scheduler::schedule_periodic(Event *passed_event_ptr){
 
   	periodic_schedule.insert( passed_event_ptr );
 	return;		
		
};


void Scheduler::cancel_periodic(Event *passed_event_ptr){
 
	periodic_schedule.erase( passed_event_ptr );
	delete passed_event_ptr;
 	return;		
 
};



void Scheduler::modify_periodic(Event *passed_event_ptr){
	printf("Scheduler::modify_periodic() not implemented\n");
	exit(1);
	return;
};



