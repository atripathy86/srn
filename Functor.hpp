//#include "Allheader.hpp"
#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include <typeinfo> //To differenciate functor for Nodes

#include "ForwardDeclaration.hpp"
#include "Infrastructure.hpp"
#include "Node.hpp"
#include "Listener.hpp"
#include <iostream>

class Functor {
	public:
	virtual void operator()() = 0;
	virtual ~Functor()
	{
		return;
	}
};

template <class TClass> class SpecificFunctor: public Functor {
	public:

	void (TClass::*fpt)();
	TClass* pt2Object;
	//bool kill_object_on_death;

	SpecificFunctor(TClass* _pt2Object, void(TClass::*_fpt)());
	
	virtual void operator()();

	/*Specifically for Nodes, IO_Reporter - do not allow the functor to delete the parent object
	instead clear the event list manually at exit. This will allow the parent 
	objects to remain alive even they use one shot timers. This might be causing us the bug when
	IO_Reporter is called it throws error. For all other objects (Listener,QueryHandler) kill the object
	*/

	~SpecificFunctor()
	{
		
		/*
		if (  (typeid(*pt2Object) != typeid(BirthAgent<ResourceNode>)) && (typeid(*pt2Object) != typeid(BirthAgent<RouterNode>)) &&
				(typeid(*pt2Object) != typeid(IO_Reporter))
			&& (typeid(*pt2Object) != typeid(RouterNode))  && (typeid(*pt2Object) != typeid(ResourceNode))  )
		*/
		if (  (typeid(*pt2Object) == typeid(Listener)) || (typeid(*pt2Object) == typeid(QueryHandler)) )
	//	if (  typeid(*pt2Object) == typeid(QueryHandler) )//Listeners will be killed during resource node heartbeat cleanup
		{
			//td::cout<<"Deleting "<<typeid(*pt2Object).name()<<std::endl;
			delete pt2Object;
		}
		return;
	}
 };

template <class TClass> 
inline 
SpecificFunctor<TClass>::SpecificFunctor(TClass* _pt2Object, void(TClass::*_fpt)() /*,bool passed_kill_object_on_death = 1*/) {
		pt2Object = _pt2Object;
		fpt= _fpt;
		//kill_object_on_death = passed_kill_object_on_death;
		return;
	};

template <class TClass>
inline 
void SpecificFunctor<TClass>::operator()() {
		(*pt2Object.*fpt)();
	return;
};


#endif
