#include "ForwardDeclaration.hpp"
#include "Scheduler.hpp"
#include "Event.hpp"
#include "Node.hpp"

#include "List.hpp" //Got from hpp
#include "Functor.hpp" //Added, but remove if this gives problem, probably this is ok
#include "IO_Reporter.hpp"


Scheduler::~Scheduler()
	{
#ifdef DEBUG
	printf("Started cleaning up for Scheduler\n");
#endif
		event_schedule.clear();
		periodic_schedule.clear();

#ifdef DEBUG
	printf("Finished cleaning up for Scheduler\n");
#endif
	return;
	};






//Scheduler methods
	//run()
	void Scheduler::run(Tick run_for_ticks){

		//std::list< Event >::iterator run_event_itr;
		ElementHolder<Event>* run_event_itr;

		ticks=0;
#ifdef DEBUG_SCHED
		printf("run():Entering, run_for_ticks is %u\n", run_for_ticks);
#endif

#ifdef DEBUG
			int i=0;
#endif

		//Event *event_to_return_ptr;

		while (ticks < run_for_ticks)
		{
#ifdef DEBUG
			//Remove later, only for Test
			std::cout<< "Ticks: "<< ticks <<std::endl; 
#endif

			//Executing scheduled events
			run_event_itr = event_schedule.begin();

#ifdef DEBUG
			i=0;
#endif
#ifdef DEBUG_SCHED
			printf("Going to start scanning event schedule\n");
#endif
			//Find the events to execute
			//while (!event_schedule.empty() && (*event_itr).time <= ticks) 
			while (!event_schedule.empty() && ( run_event_itr->content() ).time <= ticks) 
			{

#ifdef DEBUG_SCHED
		//printf("Going to fire Event from Event schedule list position %d out of %d\n", i, event_schedule.size());
		//printf("Going to fire Event from Event schedule list position %d out of %u, event_id %u, node id %u, type is %d, time is %u\n", 
		//	i, event_schedule.size(), (run_event_itr->content()).event_id, (run_event_itr->content()).node_ptr->id, (run_event_itr->content()).node_ptr->node_type,(run_event_itr->content()).time );
		
		if (ticks == 50)
			// event_schedule.print_all_elements();
			//event_schedule.print_element(66);
			printf("Going to fire Event from Event schedule list position %d out of %u, event_id %u, time is %u\n", 
			i, event_schedule.size(), (run_event_itr->content()).event_id, (run_event_itr->content()).time );
		/**/
#endif

				//(*((*run_event_itr).handler))();
				//run_event_itr = event_schedule.erase(run_event_itr);

				//(*(((*run_event_itr).content()).handler))();
				(*((run_event_itr->content()).handler))();
#ifdef DEBUG_SCHED
		//printf("Finshed firing event from Event schedule position %d out of %d, event_id %u,node id was %u, time was %u\n",
				//	i, event_schedule.size(), (run_event_itr->content()).event_id,(run_event_itr->content()).node_ptr->id, (run_event_itr->content()).time);
		//event_schedule.print_element(66);
			if (ticks == 50)
			printf("Finshed firing event from Event schedule position %d out of %d, event_id %u,time was %u\n",
					i, event_schedule.size(), (run_event_itr->content()).event_id,(run_event_itr->content()).time);

#endif

				run_event_itr = event_schedule.erase(run_event_itr);

#ifdef DEBUG_SCHED
		printf("Finished erasing event from event schedule \n");
#endif


			}//End of running events
		

			//Executing periodic tasks
			run_event_itr = periodic_schedule.begin();

#ifdef DEBUG
			i=0;
#endif

			while ( !periodic_schedule.empty() && !(run_event_itr == periodic_schedule.end()) )
			{

#ifdef DEBUG_SCHED
		printf("Renetering periodic schedule loop, iteration %d \n", i);
		//printf("Time is %u , next periodic event id is %u, time is %u\n", ticks, 
			//(run_event_itr->content()).node_ptr->id, (run_event_itr->content()).time );
		printf("Time is %u , periodic event_id is %u, event time is %u\n", ticks, 
			(run_event_itr->content()).event_id, (run_event_itr->content()).time, (run_event_itr->content()).start_time );
		printf("Time mod is %u \n", (ticks % (run_event_itr->content()).time) );

#endif
				/*
				while ( !(run_event_itr == periodic_schedule.end())  &&  
					!( (ticks % (*run_event_itr).time) ==0 ) ) 	
				*/
				while ( !(run_event_itr == periodic_schedule.end())  && 
					 /* ( ((run_event_itr->content()).start_time==0)  ||  (ticks < (run_event_itr->content()).start_time) )&& */
					!( ( (ticks - (run_event_itr->content()).start_time)% (run_event_itr->content()).time) ==0 ) ) 
				{	

#ifdef DEBUG_SCHED
		printf("Time is %u , periodic event_id is %u, event time is %u, start_time is %u,\n", ticks, 
			(run_event_itr->content()).event_id, (run_event_itr->content()).time , (run_event_itr->content()).start_time );

		printf("Time mod is %u \n",  ((ticks - (run_event_itr->content()).start_time)% (run_event_itr->content()).time ) );
#endif
					//run_event_itr++;
					run_event_itr = run_event_itr->next();

#ifdef DEBUG_SCHED
		printf("Skipping iteration %d \n", i);
#endif


#ifdef DEBUG
					i++;
#endif
				}

				if (!(run_event_itr == periodic_schedule.end()))
				{

#ifdef DEBUG_SCHED
					//printf("Going to fire Event from Periodic schedule list position %d out of %d,event_id %u, the id of the event is %u, type is %d\n", 
						//i, periodic_schedule.size(), (run_event_itr->content()).event_id,(run_event_itr->content()).node_ptr->id , (run_event_itr->content()).node_ptr->node_type );
							//periodic_schedule.print_all_elements();
									//event_schedule.print_element(66);
					printf("Going to fire Event from Periodic schedule list position %d out of %d,event_id %u\n", 
						i, periodic_schedule.size(), (run_event_itr->content()).event_id );

#endif

					//(*((*run_event_itr).handler))();
					(*((run_event_itr->content()).handler))();

					//run_event_itr++;
					run_event_itr = run_event_itr->next();

#ifdef DEBUG_SCHED
					printf("Finshed firing event from Periodic schedule list position %d out of %d\n", i, periodic_schedule.size());
					i++;
					//event_schedule.print_all_elements();
							//event_schedule.print_element(66);


#endif
				}

			}//End of running periodic tasks
			ticks++;

		}//End of main while loop

		return;
	} //End of run()


//All the methods below have been moved to hpp file for inlining, and ability to get called by other files


	void Scheduler::schedule_event(Event &passed_event)
		{
#ifdef DEBUG_SCHED
				printf("schedule_event():Entering \n");
#endif
#ifdef DEBUG
						int i=0;
#endif
			
				event_itr = event_schedule.begin();

				//while (  !(event_itr == event_schedule.end()) && (*event_itr).time < passed_event.time &&
				while (  !(event_itr == event_schedule.end()) && (event_itr->content()).time <= passed_event.time &&
					!event_schedule.empty() ) {
					
						//event_itr++;
						event_itr = event_itr->next();

#ifdef DEBUG
							i++;
#endif

				}

					event_schedule.insert(event_itr, passed_event );
#ifdef DEBUG_SCHED
				printf("Inserted Event at Event schedule list position %d out of %d with event_id %u, time as %d\n", 
							i, event_schedule.size(), passed_event.event_id, passed_event.time );
				//event_schedule.print_all_elements();
#endif
		return;
	}



	void Scheduler::schedule_periodic(Event &passed_event)
		{

#ifdef DEBUG_SCHED
				printf("schedule_periodic():Entering \n");
#endif
#ifdef DEBUG
						int i=0;
#endif

				event_itr = periodic_schedule.begin();

				//while ( !(event_itr == periodic_schedule.end()) && (*event_itr).time < passed_event.time && 
				while ( !(event_itr == periodic_schedule.end()) && ((event_itr->content()).time <= passed_event.time ) && 
							!periodic_schedule.empty() ) {
					
					//event_itr++; 
					event_itr = event_itr->next();  

#ifdef DEBUG
					i++;
#endif

				}
#ifdef DEBUG_SCHED
				printf("schedule_periodic():Reached here 1 \n");
#endif

					periodic_schedule.insert(event_itr, passed_event );

#ifdef DEBUG_SCHED
				printf("Inserted Event at Periodic schedule list position %d out of %d with event_id %u, time as %d\n", 
							i, periodic_schedule.size(), passed_event.event_id, passed_event.time);
				//event_schedule.print_all_elements();
#endif
			return;
		}

	void Scheduler::cancel_periodic(Event &passed_event)
		{

			//std::list<Event>::iterator event_itr;

			//Get and make an event
			event_itr = periodic_schedule.begin();

			/*
			while ( !(event_itr == periodic_schedule.end()) && !((*event_itr).handler == passed_event.handler) &&
				!periodic_schedule.empty() ) event_itr++;
			*/
			/*
			while ( !(event_itr == periodic_schedule.end()) && !((event_itr->content()).handler == passed_event.handler) &&
				!periodic_schedule.empty() ) event_itr = event_itr->next();
			*/
			while ( !(event_itr == periodic_schedule.end()) && !((event_itr->content()) == passed_event) ) 
					event_itr = event_itr->next();
#ifdef DEBUG_SCHED
				printf("Scheduler::cancel_periodic(), Reached here 1\n");
#endif

			periodic_schedule.erase(event_itr);
#ifdef DEBUG_SCHED
				printf("Scheduler::cancel_periodic(), Reached here 2\n");
#endif
			return;
		}



	void Scheduler::modify_periodic(Event &passed_event)
		{

			//std::list<Event>::iterator event_itr;

			//Get and make an event
			event_itr = periodic_schedule.begin();

			/*
			while (  !(event_itr == periodic_schedule.end()) && !((*event_itr).handler == passed_event.handler) &&
				!periodic_schedule.empty() ) event_itr++;
			*/
			while (  !(event_itr == periodic_schedule.end()) && !((event_itr->content()) == passed_event) &&
				!periodic_schedule.empty() ) event_itr = event_itr->next();

			//(*event_itr).time = passed_event.time;
			(event_itr->content()).time = passed_event.time;
			return;
		}


