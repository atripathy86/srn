#include "ForwardDeclaration.hpp"
//#include "Allheader.hpp"
#include "Infrastructure.hpp"

#include "Analyst.hpp"
#include "Scheduler.hpp"
#include "Database.hpp"
#include "IO_Reporter.hpp"
#include "Event.hpp"
#include "Node.hpp"

#include "OntologyRepository.hpp" //Got from hpp
#include "Input.hpp" //Got from hpp


Infrastructure::Infrastructure() {

	scheduler_ptr = new Scheduler(this);
	param_ptr =new InputParameters;
	IO_Reporter_ptr = new IO_Reporter(this);

	analyst.If_ptr = this;
	agent_id = 0;user_node_id = 3;
	//query_id=0;response_id=0;
	database_ptr = new Database(this);
#ifdef DEBUG
	event_id = 0;
#endif
	return;
};



Infrastructure::~Infrastructure()
	{
		
#ifdef DEBUG
	printf("Started cleaning up for Infrastructure\n");
#endif
		
		
		/*
		OntologyRepository *onto_repository_ptr;
		InputParameters *param_ptr;
		IO_Reporter *IO_Reporter_ptr;

		std::vector<ResourceNode *> list_of_resource_nodes;
		std::vector<RouterNode *> list_of_router_nodes;

		Database *database_ptr;
		*/

		delete param_ptr;
		delete IO_Reporter_ptr;
		delete onto_repository_ptr;
		delete database_ptr;
		delete scheduler_ptr;

		/*This statement now doesnt holde true because the functor doesnt delete the parent 
		element anymore, read the comment in functor code-
			Cleaning node list are not neccessary as these nodes get deleted when 
			the event schedule is cleaned, the functor destroys these node objects 
			through the heartbeat handler functor linkages */

 		std::vector<ResourceNode *>::iterator list_of_resource_nodes_itr;
		std::vector<RouterNode *>::iterator list_of_router_nodes_itr;
		std::vector<UserNode *>::iterator list_of_user_nodes_itr;
		//std::map<unsigned int, Message*>::iterator list_of_messages_itr;
		std::map<unsigned int, Query*>::iterator list_of_queries_itr;
		std::map<unsigned int, Response*>::iterator list_of_responses_itr;

		//std::set< Message*>::iterator list_of_messages_itr;

		unsigned int num_elements_to_clear;

  
		for (num_elements_to_clear = list_of_router_nodes.size(),list_of_router_nodes_itr = list_of_router_nodes.begin();
					num_elements_to_clear >0 ;
					num_elements_to_clear--,list_of_router_nodes_itr++ ) 
		{
 			delete (*list_of_router_nodes_itr);
		}
#ifdef DEBUG
	printf("Finished deleting all router nodes\n");
#endif
		
		for (num_elements_to_clear = list_of_resource_nodes.size(),list_of_resource_nodes_itr = list_of_resource_nodes.begin();
					num_elements_to_clear >0 ;
					num_elements_to_clear--,list_of_resource_nodes_itr++) 
		{
 			delete (*list_of_resource_nodes_itr);
		}
#ifdef DEBUG
	printf("Finished deleting all resource nodes\n");
#endif

		for (num_elements_to_clear = list_of_user_nodes.size(),list_of_user_nodes_itr = list_of_user_nodes.begin();
					num_elements_to_clear >0 ;
					num_elements_to_clear--,list_of_user_nodes_itr++) 
		{
 			delete (*list_of_user_nodes_itr);
		}
#ifdef DEBUG
	printf("Finished deleting all user nodes\n");
#endif

 		list_of_resource_nodes.clear();
		list_of_router_nodes.clear();
		list_of_user_nodes.clear();

#ifdef DEBUG
	printf("Finished cleaning up list of nodes\n");
#endif

/*
		delete user_node_ptr;
#ifdef DEBUG
	printf("Finished deleting user node\n");
#endif
*/


#ifdef DEBUG
	printf("Finished cleaning up for Infrastructure\n");
#endif
		return;
	};

