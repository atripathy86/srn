#ifndef LIST_HPP
#define LIST_HPP

#include <cstdlib>

//template <class Element> class List ;
//template <class Element> class ElementHolder ;

//ElementHolder<Event> itr;

///List<Event>::iterator itr;

//ElementHolder<Event>::iterator == ElementHolder<Event> 

#ifdef DEBUG
	#include <typeinfo>
	#include "Message.hpp"
#endif


template <class Element>
class ElementHolder {
	//typedef ElementHolder<Element> iterator;
	public:
	Element *held_content_element_ptr;
	ElementHolder *next_holder_ptr;
	ElementHolder *prev_holder_ptr;
	ElementHolder() {
		held_content_element_ptr = NULL;
		next_holder_ptr = NULL;
		prev_holder_ptr = NULL;
		return;
	}
	
	Element& content();

	ElementHolder* next(){	
	
		return next_holder_ptr; 
	}; 
	//ElementHolder* operator++() { next();  }

	~ElementHolder()
	{
		delete held_content_element_ptr;
		return;
	}

};


template<class Element> 
inline Element& ElementHolder<Element>::content() { 

		return (*held_content_element_ptr);
	}


template <class Element > 
class List  {
	private:
		//Element *curr_element_ptr;
		ElementHolder<Element> *curr_element_holder_ptr;
		//ElementHolder *begin_holder_ptr;
		//ElementHolder *end_holder_ptr;
		unsigned int _size;
		ElementHolder<Element>* element_holders;
		ElementHolder<Element>* last_element_holder_ptr;
	public:
	List();
	ElementHolder<Element>* begin();
	ElementHolder<Element>* end();
	//ElementHolder<Element>* insert(ElementHolder<Element> *insert_position_ptr, Element &element_to_insert);
	void insert(ElementHolder<Element> *insert_position_ptr, Element &element_to_insert);
	ElementHolder<Element>* erase(ElementHolder<Element> *insert_position_ptr);
	//Element & pop();
	//void push(Element& element_to_push);

	bool empty() ;
	unsigned int size();
	void clear();

#ifdef DEBUG
	void print_all_elements();
	void print_element(unsigned int event_id_to_print);

#endif //for debug only

	~List()
	{

#ifdef DEBUG
	printf("Started cleaning up for List\n");
#endif
		
		clear();

#ifdef DEBUG
	printf("Finished cleaning up for List\n");
#endif
		return;
	}
};


/*
template<class Element> 
Element& List<Element>::pop()
{
	if (empty())
	{

		for (int i=0; i<1024; i++)
		{
			insert(begin(), *(new Element));
		}

	}
	curr_element_ptr = (begin()).held_content_element_ptr;
	element_holders = element_holders->next_holder_ptr;
	element_holders->prev_holder_ptr

	return *curr_element_ptr;
	
};


template<class Element> 
void List<Element>::push(Element& element_to_push)
{
	
	insert( begin(), element_to_push);
	
};

*/

template<class Element> 
List<Element>::List() {
	
	curr_element_holder_ptr = NULL;
	element_holders = NULL;
	last_element_holder_ptr = NULL;
	_size =0;
	return;
}

template<class Element> 
void List<Element>::clear() {

	while (element_holders != NULL)
	{
		curr_element_holder_ptr = element_holders->next_holder_ptr;
		delete element_holders;
		element_holders = curr_element_holder_ptr;

	}
	return;
}




template<class Element> 
inline ElementHolder<Element>* List<Element>::begin() {

	//return (!_size) ? NULL : element_holders;
	return element_holders;
}


template<class Element> 
inline ElementHolder<Element>* List<Element>::end() {

	return NULL;

}



template<class Element> 
//Complies to the STL semantics, return iterator to the element inserted
//inline ElementHolder<Element>* List<Element>::insert(ElementHolder<Element> *insert_position_ptr, Element &element_to_insert) {
inline void List<Element>::insert(ElementHolder<Element> *insert_position_ptr, Element &element_to_insert) {
	if (_size ==0 )
	{
		
		//Empty list
		element_holders = new ElementHolder<Element>;
		element_holders->prev_holder_ptr = NULL;
		element_holders->next_holder_ptr = NULL;
		last_element_holder_ptr = element_holders;
		
		element_holders->held_content_element_ptr = &element_to_insert;
		
		//Complies to the STL semantics, return iterator to the element inserted
		//curr_element_holder_ptr = element_holders;


	} else {

			if (insert_position_ptr ==NULL)
			{

				//This insertion point is at the end
				last_element_holder_ptr->next_holder_ptr = new ElementHolder<Element>;
				last_element_holder_ptr->next_holder_ptr->prev_holder_ptr = last_element_holder_ptr;
				last_element_holder_ptr->next_holder_ptr->next_holder_ptr = NULL;
				last_element_holder_ptr = last_element_holder_ptr->next_holder_ptr;

				last_element_holder_ptr->held_content_element_ptr = &element_to_insert;
				
				//Complies to the STL semantics, return iterator to the element inserted
 				//curr_element_holder_ptr = last_element_holder_ptr;
 								
			} else {

						//The insertion point is at the middle or front
						
						curr_element_holder_ptr =  insert_position_ptr->prev_holder_ptr;

						insert_position_ptr->prev_holder_ptr = new ElementHolder<Element>;


					if ( curr_element_holder_ptr !=NULL )
					{
						//The insert point is at the middle
						curr_element_holder_ptr->next_holder_ptr = insert_position_ptr->prev_holder_ptr;
						
					} else //The insertion point is at front, so update first element pointer
						element_holders = insert_position_ptr->prev_holder_ptr;
					

					insert_position_ptr->prev_holder_ptr->next_holder_ptr = insert_position_ptr;
					insert_position_ptr->prev_holder_ptr->prev_holder_ptr = curr_element_holder_ptr;

					insert_position_ptr->prev_holder_ptr->held_content_element_ptr = &element_to_insert;

					//Complies to the STL semantics, return iterator to the element inserted
					//curr_element_holder_ptr = insert_position_ptr->prev_holder_ptr;

			}
	}
	_size++;


#ifdef DEBUG_LIST
	print_all_elements();
#endif

	//Complies to the STL semantics, return iterator to the element inserted
	//return curr_element_holder_ptr;
	return;
}


template<class Element> 
inline ElementHolder<Element>* List<Element>::erase(ElementHolder<Element> *insert_position_ptr) {
	

	curr_element_holder_ptr = insert_position_ptr->next_holder_ptr;

	//Case the Event to erase is the first Event
	if ( insert_position_ptr->prev_holder_ptr != NULL )
	{
#ifdef DEBUG_LIST
				printf("List::erase(), Reached here 1\n");
#endif

 	  insert_position_ptr->prev_holder_ptr->next_holder_ptr = insert_position_ptr->next_holder_ptr;
	}
	else  
	{
#ifdef DEBUG_LIST
				printf("List::erase(), Reached here 2\n");
#endif

		element_holders = insert_position_ptr->next_holder_ptr;
	}

	//Case the Event to erase is the last Event
	if ( insert_position_ptr->next_holder_ptr != NULL )

	{
#ifdef DEBUG_LIST
				printf("List::erase(), Reached here 3\n");
#endif
		insert_position_ptr->next_holder_ptr->prev_holder_ptr = insert_position_ptr->prev_holder_ptr;
	}
	else  
	{
#ifdef DEBUG_LIST
				printf("List::erase(), Reached here 4\n");
#endif

		last_element_holder_ptr = insert_position_ptr->prev_holder_ptr;
	}

	//delete insert_position_ptr->held_content_element_ptr;
#ifdef DEBUG_LIST
				printf("List::erase(), Reached here 5\n");
#endif

	delete insert_position_ptr;
	_size--;

#ifdef DEBUG_LIST
				printf("List::erase(), Reached here 6\n");
#endif

	return curr_element_holder_ptr;

}


template<class Element> 
inline bool List<Element>::empty() {
	//return (element_holders == NULL)? 1 : 0;

	return (!_size) ? 1 : 0;

}

template<class Element> 
inline unsigned int List<Element>::size() {
#ifdef DEBUG_LIST
	printf("List:: size() called event_id %u, time %u\n", held_content_element_ptr->event_id, held_content_element_ptr->time);
#endif

	return _size;
}

#ifdef DEBUG

template<class Element> 
void List<Element>::print_all_elements() {
		unsigned int j=0;
		printf("Starting to print all elements in the schedule of size %u\n", size());
		curr_element_holder_ptr = begin();
		while (curr_element_holder_ptr != end() /*&& j < 50*/)
		{
			
			printf("position %u, event_id %u, time %u, start_time %u   \n", j, ((*curr_element_holder_ptr).content()).event_id,
				((*curr_element_holder_ptr).content()).time, ((*curr_element_holder_ptr).content()).start_time );
	
	/*
			printf("position %u, event_id %u, id %u ,type %d, time %u   \n", j, ((*curr_element_holder_ptr).content()).event_id,
				((*curr_element_holder_ptr).content()).node_ptr->id, 
							((*curr_element_holder_ptr).content()).node_ptr->node_type, ((*curr_element_holder_ptr).content()).time );
			
			printf("Going to touch ((*curr_element_holder_ptr).content()).query_to_handle_ptr \n");

			if ( ( ((*curr_element_holder_ptr).content()).query_to_handle_ptr == 0) )
			{
				printf("((*curr_element_holder_ptr).content()).query_to_handle_ptr ==0\n");
			}
			else	
			{	
				printf("List<Element>::print_all_elements():  position is %u, ", j);
				printf(" query_to_handle_ptr->source_node_ptr  %u, ", 
					((*curr_element_holder_ptr).content()).query_to_handle_ptr->source_node_ptr);
				printf(" query_to_handle_ptr->source_node_ptr->id  %u\n", 
					((*curr_element_holder_ptr).content()).query_to_handle_ptr->source_node_ptr->id);

					if	( ((*curr_element_holder_ptr).content()).query_to_handle_ptr->source_node_ptr->id >1000 )
					{
						printf("Problematic QueryHandler found\n");
						printf("position %u, id %u ,type %d, time %u   ", j, ((*curr_element_holder_ptr).content()).node_ptr->id, 
							((*curr_element_holder_ptr).content()).node_ptr->node_type, ((*curr_element_holder_ptr).content()).time );

					}
			}
	*/

			curr_element_holder_ptr = curr_element_holder_ptr->next();
			j++;
		}
		printf("Printing all elements of schedule is over\n");
		return;
}



template<class Element> 
void List<Element>::print_element(unsigned int event_id_to_print) {
		unsigned int j=0;
		printf("Starting to print event_id %u from event schedule size %u\n", event_id_to_print, size());
		curr_element_holder_ptr = begin();
		while (curr_element_holder_ptr != end() /*&& j < 50*/)
		{
			if ( ((*curr_element_holder_ptr).content()).event_id == event_id_to_print)
			{

			
				printf("event_id %u, position %u, id %u ,type %d, time %u, query_to_handle_ptr->source_node_id %u \n", ((*curr_element_holder_ptr).content()).event_id, j, 
					((*curr_element_holder_ptr).content()).node_ptr->id, 
							((*curr_element_holder_ptr).content()).node_ptr->node_type, ((*curr_element_holder_ptr).content()).time,
					((*curr_element_holder_ptr).content()).query_to_handle_ptr->source_node_id );
			}
		
			curr_element_holder_ptr = curr_element_holder_ptr->next();
			j++;
		}
		printf("Printing event_id %u of schedule is over\n", event_id_to_print);
		return;
}





#endif //for debug only



#endif //for entire hpp file

/*(  typeid( ((*curr_element_holder_ptr).content()).handler->fpt) == typeid(&QueryHandler::handler_for_response_to_sender) ) && */

