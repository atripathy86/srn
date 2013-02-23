#ifndef INFRASTRUCTURE_HPP
#define INFRASTRUCTURE_HPP

#include "ForwardDeclaration.hpp"
#include "Scheduler.hpp"
//#include "Database.hpp"
#include "Analyst.hpp"
//#include "Event.hpp"
//#include "OntologyRepository.hpp"
//#include "IO_Reporter.hpp"
//#include "Input.hpp"
#include "Node.hpp" //For the UserNode

class Infrastructure {
	public:
	Scheduler* scheduler_ptr;
	OntologyRepository *onto_repository_ptr;
	unsigned int agent_id, user_node_id ; //, query_id, response_id;
	InputParameters *param_ptr;
	IO_Reporter *IO_Reporter_ptr;
	
	//UserNode* user_node_ptr;

	struct capacity_data
	{
		unsigned int edge_list;
	} provisioned_capacity;

	std::vector<ResourceNode *> list_of_resource_nodes;
	std::vector<RouterNode *> list_of_router_nodes;
	std::vector<UserNode *> list_of_user_nodes;
	//std::map<unsigned int, Message*> list_of_messages;
	//std::map<unsigned int, Query*> list_of_queries;
	//std::map<unsigned int, Response*> list_of_responses;

	//std::set< Message*> list_of_messages;

	Analyst analyst;
	Database *database_ptr;

	Infrastructure();
	//Infrastructure(OntologyRepository &passed_onto_repositiory) {onto_repositiory_ptr = &passed_onto_repositiory;}
#ifdef DEBUG
	unsigned int event_id;
#endif

	~Infrastructure();

/*
#ifdef DEBUG_FAULT
	void print_list_of_queries(unsigned int start_position);
#endif
*/


};


#endif

