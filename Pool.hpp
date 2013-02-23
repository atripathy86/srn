#ifndef POOL_HPP
#define POOL_HPP
#include <list>

#include "ForwardDeclaration.hpp"
//#include "Event.hpp"

template < class SpecificElement> 
class Pool {

	unsigned int reload_size;
	SpecificElement *curr_element_ptr;

	std::list<SpecificElement *> available;

	void reload(unsigned int passed_reload_size){
		
		for (unsigned int fi= passed_reload_size;fi>0 ;fi-- )
		{
			curr_element_ptr = new SpecificElement();
			available.push_back(curr_element_ptr);
		}
		
	};

	public:

	Pool(unsigned int passed_pool_size, unsigned int passed_reload_size): reload_size(passed_reload_size) {

		reload(passed_pool_size);

	}

	
	SpecificElement * get(){

		if (!available.size()) reload(reload_size);
		
		curr_element_ptr = available.front();
		available.pop_front();

		return curr_element_ptr;
	}
	
	void return_freed(SpecificElement* &element_ptr){
		available.push_back(element_ptr);
		
	};

	int size(){ return available.size();}

};



#endif
