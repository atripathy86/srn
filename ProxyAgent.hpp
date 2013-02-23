/*To do
	random description generator
	complete creation of nodes

*/

/*
class ProxyAgent:private Node {
	
	Tick period;
	Tick start_time;
	unsigned int num_cycles;
	ProxyAgent(	Tick passed_period, Tick passed_start_time,unsigned int passed_num_cycles)
	{ period =passed_period;  start_time= passed_start_time;  num_cycles = passed_num_cycles;};

};
*/
#ifndef PROXYAGENT_HPP
#define PROXYAGENT_HPP

#include "ForwardDeclaration.hpp"

#include "Node.hpp"
#include "Listener.hpp"
#include "Event.hpp"
#include "Infrastructure.hpp"
#include "OntologyRepository.hpp"

#include "Database.hpp" //For num_resource_with_specific_tag

template < class SpecificNode> 
class BirthAgent : public Node {

	Tick period;
	Tick start_time;
	unsigned int total_budget;
	int num_cycles_per_heartbeat;

	//unsigned int budget_for_router;
	int budget_for_per_heartbeat;

	SpecificNode *agent_ptr;
	//RouterNode* router_node_ptr;

	int bootstrap_threashold;
	unsigned int message_time_to_live;

			NodeType node_type_to_pass_on;
			int  known_routers_floor;
			int  known_routers_trim_threshold;
			int  known_routers_capacity;

			int  translation_radius;

			int  routing_table_length_floor;
			int  routing_table_length_trim_threshold;
			int  routing_table_max_length;

			int  routing_table_width_floor;
			int  routing_table_width_threshold;
			int routing_table_min_num_resource_nodes;
			int  routing_table_max_width;

	int k;
	int num_centroid_tags_to_explore_per_period;
	int num_queries_to_submit_per_period;

	int max_num_description;
	unsigned int vitality;
	int extroversion;
	Tick delay;
	Tick response_timeout;
	Infrastructure* If_ptr;

	Description generated_random_description; 
	Tag generated_random_tag;

	RouterNode *generated_random_bootstrap_node_ptr;
	unsigned int generated_random_index;
	BootstrapNodeList nodes_to_bootstrap_to;
	//std::map<unsigned int, unsigned int> index_of_nodes_to_bootstrap_to;
	std::set<unsigned int> index_of_nodes_to_bootstrap_to;

	SpecificFunctor<BirthAgent> *functor_handler_ptr;
	Event *handler_schedule_event_ptr;

	
	public:
	BirthAgent( unsigned int passed_bootstrap_threashold, Tick passed_start_time, Tick passed_period, 
			unsigned int passed_total_num_cycles, int passed_num_cycles_per_heartbeat,
			unsigned int passed_message_time_to_live,
			
			int  passed_known_routers_floor,
			int  passed_known_routers_trim_threshold,
			int  passed_known_routers_capacity,
		
			int  passed_translation_radius,

			int  passed_routing_table_length_floor,
			int  passed_routing_table_length_trim_threshold,
			int  passed_routing_table_max_length,

			int  passed_routing_table_width_floor,
			int  passed_routing_table_width_threshold,
			int  passed_routing_table_min_num_resource_nodes,
			int  passed_routing_table_max_width,

			int passed_max_num_description,
			int passed_k, int passed_num_centroid_tags_to_explore_per_period,
			int passed_num_queries_to_submit_per_period,
			unsigned int passed_vitality, int passed_extroversion,
			Tick passed_delay,
			Tick passed_response_timeout,
			Infrastructure* passed_If_ptr ); 
	
	void setup(unsigned int passed_bootstrap_threashold, Tick passed_start_time, Tick passed_period, 
			unsigned int passed_total_num_cycles, int passed_num_cycles_per_heartbeat,
			unsigned int passed_message_time_to_live,
		
			int  passed_known_routers_floor,
			int  passed_known_routers_trim_threshold,
			int  passed_known_routers_capacity,
		
			int  passed_translation_radius,

			int  passed_routing_table_length_floor,
			int  passed_routing_table_length_trim_threshold,
			int  passed_routing_table_max_length,

			int  passed_routing_table_width_floor,
			int  passed_routing_table_width_threshold,
			int  passed_routing_table_min_num_resource_nodes,
			int  passed_routing_table_max_width,

			int passed_max_num_description,
			int passed_k,int passed_num_centroid_tags_to_explore_per_period,
			int passed_num_queries_to_submit_per_period,
			unsigned int passed_vitality, int passed_extroversion,
			Tick passed_delay,
			Tick passed_response_timeout,
			Infrastructure* passed_If_ptr ); 

	void generate_random_description(int num_description);

	void generate_random_bootstrap_nodes( int num_bootstrap_nodes) ;

	void heartbeat();

	~BirthAgent()
	{
#ifdef DEBUG
	printf("Started cleaning up for BirthAgent\n");
#endif
			nodes_to_bootstrap_to.clear();
			index_of_nodes_to_bootstrap_to.clear();
			generated_random_description.clear();

#ifdef DEBUG
	printf("Finished cleaning up for BirthAgent\n");
#endif

	}
};


template<class SpecificNode> 
BirthAgent<SpecificNode>::BirthAgent( unsigned int passed_bootstrap_threashold, Tick passed_start_time, Tick passed_period, 
			unsigned int passed_total_num_cycles,  int passed_num_cycles_per_heartbeat,
			unsigned int passed_message_time_to_live,
			
			int  passed_known_routers_floor,
			int  passed_known_routers_trim_threshold,
			int  passed_known_routers_capacity,
		
			int  passed_translation_radius,

			int  passed_routing_table_length_floor,
			int  passed_routing_table_length_trim_threshold,
			int  passed_routing_table_max_length,

			int  passed_routing_table_width_floor,
			int  passed_routing_table_width_threshold,
			int  passed_routing_table_min_num_resource_nodes,
			int  passed_routing_table_max_width,

			int passed_max_num_description,
			int passed_k, int passed_num_centroid_tags_to_explore_per_period,
			int passed_num_queries_to_submit_per_period,
			unsigned int passed_vitality,  int passed_extroversion, 
			Tick passed_delay,
			Tick passed_response_timeout,
			Infrastructure* passed_If_ptr ) : Node(passed_If_ptr)
		{

			if (typeid(SpecificNode) == typeid(ResourceNode) ) 
			{
				node_type = BIRTHAGENT_RESOURCE_NODE; node_type_to_pass_on = RESOURCE_NODE;
				id = std::numeric_limits<unsigned int>::max();
			}
			else if (typeid(SpecificNode) == typeid(RouterNode)) 
				{
					node_type = BIRTHAGENT_ROUTER_NODE; node_type_to_pass_on = ROUTER_NODE;
 					id = std::numeric_limits<unsigned int>::max() - 1;
 				}
				else if (typeid(SpecificNode) == typeid(UserNode))
				{
					node_type = BIRTHAGENT_USER_NODE; node_type_to_pass_on = USER_NODE;
 					id = std::numeric_limits<unsigned int>::max() - 2;
				}
			
			setup( passed_bootstrap_threashold,  passed_start_time,  passed_period, 
			 passed_total_num_cycles, passed_num_cycles_per_heartbeat,
			 passed_message_time_to_live,
 			
			  passed_known_routers_floor,
			  passed_known_routers_trim_threshold,
			  passed_known_routers_capacity,
		
			  passed_translation_radius,

			  passed_routing_table_length_floor,
			  passed_routing_table_length_trim_threshold,
			  passed_routing_table_max_length,

			  passed_routing_table_width_floor,
			  passed_routing_table_width_threshold,
			  passed_routing_table_min_num_resource_nodes,
			  passed_routing_table_max_width,

			 passed_max_num_description,
			 passed_k, passed_num_centroid_tags_to_explore_per_period,
			 passed_num_queries_to_submit_per_period,
			 passed_vitality, passed_extroversion, 
			 passed_delay,
			 passed_response_timeout,
			 passed_If_ptr );

			return;
		};


template<class SpecificNode> 
void inline BirthAgent<SpecificNode>::setup(unsigned int passed_bootstrap_threashold, Tick passed_start_time, Tick passed_period, 
			unsigned int passed_total_num_cycles, int passed_num_cycles_per_heartbeat,
			unsigned int passed_message_time_to_live,
			
			int  passed_known_routers_floor,
			int  passed_known_routers_trim_threshold,
			int  passed_known_routers_capacity,
			
			int  passed_translation_radius,

			int  passed_routing_table_length_floor,
			int  passed_routing_table_length_trim_threshold,
			int  passed_routing_table_max_length,

			int  passed_routing_table_width_floor,
			int  passed_routing_table_width_threshold,
			int  passed_routing_table_min_num_resource_nodes,
			int  passed_routing_table_max_width,

			int passed_max_num_description,
			int passed_k,int passed_num_centroid_tags_to_explore_per_period,
			int passed_num_queries_to_submit_per_period,
			unsigned int passed_vitality, int passed_extroversion, 
			Tick passed_delay,
			Tick passed_response_timeout,
			Infrastructure* passed_If_ptr )
			
			{
				
					//agent_ptr =NULL;
					period =passed_period;  start_time= passed_start_time;  
					total_budget = passed_total_num_cycles; num_cycles_per_heartbeat = passed_num_cycles_per_heartbeat;

					bootstrap_threashold = passed_bootstrap_threashold;
					start_time= passed_start_time; period = passed_period;
					message_time_to_live = passed_message_time_to_live;

				  known_routers_floor = passed_known_routers_floor;
				  known_routers_trim_threshold = passed_known_routers_trim_threshold;
				  known_routers_capacity = passed_known_routers_capacity;
			 
				  translation_radius = 	passed_translation_radius;

				  routing_table_length_floor = passed_routing_table_length_floor;
				  routing_table_length_trim_threshold = passed_routing_table_length_trim_threshold;
				  routing_table_max_length = passed_routing_table_max_length;

				  routing_table_width_floor = passed_routing_table_width_floor;
				  routing_table_width_threshold = passed_routing_table_width_threshold;
				  routing_table_min_num_resource_nodes = passed_routing_table_min_num_resource_nodes;
				  routing_table_max_width = passed_routing_table_max_width;


					max_num_description = passed_max_num_description; k= passed_k; 
					num_centroid_tags_to_explore_per_period = passed_num_centroid_tags_to_explore_per_period;
					num_queries_to_submit_per_period = passed_num_queries_to_submit_per_period; 
					vitality = passed_vitality;
					extroversion = passed_extroversion;
					delay = passed_delay;
					response_timeout = passed_response_timeout;
					If_ptr = passed_If_ptr;

					//Schedule itself
					//Create an event
					handler_schedule_event_ptr = new Event;

#ifdef DEBUG
					handler_schedule_event_ptr->event_id = If_ptr->event_id;
					If_ptr->event_id++;
					//handler_schedule_event_ptr->node_ptr = this;
#endif

					//Create a functor, assign the object and its handling method corresponding to the event
					functor_handler_ptr = new SpecificFunctor<BirthAgent>( this, &BirthAgent<SpecificNode>::heartbeat );
							
					handler_schedule_event_ptr->handler= functor_handler_ptr;
					//Setup the periodicty
					handler_schedule_event_ptr->time = period;
					handler_schedule_event_ptr->start_time = start_time;

					//Schedule the periodic event
					If_ptr->scheduler_ptr->schedule_periodic( handler_schedule_event_ptr); 

#ifdef DEBUG_PROXY
					printf( " Created BirthAgent: with \n passed_bootstrap_threashold = %u , passed_start_time %u \n ,\
							passed_period %u, passed_num_cycles %u, passed_message_time_to_live %u \n \
							passed_routing_table_capacity %u, passed_max_num_description %u, passed_k %d, passed_vitality %d, \n \
								passed_delay, passed_extroversion %d, passed_response_timeout %u \n", 
						
						passed_bootstrap_threashold, passed_start_time, 
						passed_period, passed_num_cycles, passed_message_time_to_live, 
						passed_routing_table_capacity, passed_max_num_description, passed_k, passed_vitality, 
						passed_delay, passed_extroversion, passed_response_timeout );
#endif
					return;
			}; 






template<class SpecificNode> 
void inline BirthAgent<SpecificNode>::generate_random_description( int num_description) {

#ifdef DEBUG
		printf("Generated random descriptions are :");
#endif
		generated_random_description.clear();

		//Fill the description with random tags
		for (int i=0;i<num_description ;i++ )
		{
			//generated_random_tag =((rand() % If_ptr->onto_repository_ptr->num_vertices)+1);
			generated_random_tag = (rand() % If_ptr->onto_repository_ptr->num_vertices);

			//generated_random_description.insert( make_pair( generated_random_tag, generated_random_tag) );
			generated_random_description.insert( generated_random_tag  );

			//Also update the counter which records the num of resources for a given tag
			if (node_type == BIRTHAGENT_RESOURCE_NODE)
				If_ptr->database_ptr->num_resource_with_specific_tag[ generated_random_tag ]++;

			if (node_type == BIRTHAGENT_ROUTER_NODE)
				If_ptr->database_ptr->num_router_with_specific_tag[ generated_random_tag ]++;

#ifdef DEBUG
			printf(" %u", generated_random_tag);
#endif

		}

#ifdef DEBUG
		printf("\n");
#endif
		return;
	};

template<class SpecificNode> 
void inline BirthAgent<SpecificNode>::generate_random_bootstrap_nodes( int num_bootstrap_nodes) {

#ifdef DEBUG
			printf("\nEnough bootstrap routers can be found\n");
 			printf("Bootstrap nodes for this agent are :");
#endif
			
			//Flush the bootstrap list from the old cycle
			nodes_to_bootstrap_to.clear();
			index_of_nodes_to_bootstrap_to.clear();

#ifdef DEBUG_PROXY
			printf("\nBoot strap list cleared\n");
#endif

			for (int i=0;i<num_bootstrap_nodes ;) {

 
				generated_random_index =( ( rand() % If_ptr->list_of_router_nodes.size() ) );

 				if (index_of_nodes_to_bootstrap_to.find(generated_random_index) == index_of_nodes_to_bootstrap_to.end())
				{
 					//index_of_nodes_to_bootstrap_to.insert( make_pair(generated_random_index, generated_random_index) ); 
					index_of_nodes_to_bootstrap_to.insert( generated_random_index ); 
#ifdef DEBUG
					printf(" id %u, type %d", (If_ptr->list_of_router_nodes[ generated_random_index ])->id, 
								(If_ptr->list_of_router_nodes[ generated_random_index ])->node_type );
#endif

					//nodes_to_bootstrap_to.push_back( &(*(If_ptr->list_of_router_nodes[ generated_random_index ])) );
					nodes_to_bootstrap_to.push_back(  (If_ptr->list_of_router_nodes[ generated_random_index ])  );
					i++;
				}
#ifdef DEBUG_PROXY
				else 
				{
					printf("skipped %u ,", generated_random_index);
				}
#endif
			}
		
#ifdef DEBUG
		
		printf("\n");
#endif

		return;

};


template<class SpecificNode> 
void inline BirthAgent<SpecificNode>::heartbeat(){

#ifdef DEBUG
				printf("BirthAgent :heartbeat node id %u, node type %d : fired at %u ticks \n", id, node_type, If_ptr->scheduler_ptr->now());
#endif

		budget_for_per_heartbeat = num_cycles_per_heartbeat;

		//Check if enough router exist for bootstrapping, if not then create them
		if(  If_ptr->list_of_router_nodes.size() < bootstrap_threashold )
		{

			switch (node_type)
			{

				case BIRTHAGENT_RESOURCE_NODE:
				 
#ifdef DEBUG
					printf("Not enough bootstrap routers can be found, waiting for bootstrap routers to be added...\n");
#endif
					
					return;
				break;
				 
				case BIRTHAGENT_ROUTER_NODE:
						
#ifdef DEBUG
						printf("Not enough router nodes existing to bootstrap, so creating some intial bootstrap router nodes \n");
#endif
			
					
						for (int i=0; ( i < bootstrap_threashold ) && ( total_budget > 0 ) && ( budget_for_per_heartbeat > 0 ); 
												i++, total_budget--,budget_for_per_heartbeat--)
						{

							/*
							unsigned int passed_message_time_to_live, unsigned int passed_routing_table_capacity, 
							int passed_k, Description passed_description, unsigned int passed_id, int passed_vitality, 
							int passed_extroversion, Tick passed_response_timeout, 
							BootstrapNodeList &passed_bootstrap_nodes, Infrastructure* passed_If_ptr
							*/
							generate_random_description(max_num_description);

 							agent_ptr = new SpecificNode(message_time_to_live, 

								node_type_to_pass_on,
								known_routers_floor,
								known_routers_trim_threshold,
								known_routers_capacity,

								translation_radius,

								routing_table_length_floor,
								routing_table_length_trim_threshold,
								routing_table_max_length,

								routing_table_width_floor,
								routing_table_width_threshold,
								routing_table_min_num_resource_nodes,
								routing_table_max_width,

								k, num_centroid_tags_to_explore_per_period, num_queries_to_submit_per_period,
								generated_random_description, If_ptr->agent_id, vitality, extroversion, 
								delay, response_timeout, nodes_to_bootstrap_to, If_ptr );

								/* Push in the router node generated in the previous cycle, 
								this will avoid bootstrapping to itself */
								nodes_to_bootstrap_to.push_back( If_ptr->list_of_router_nodes.back());

							If_ptr->agent_id++;
						}
				
			}//end of switch

		}			


	for (; ( total_budget > 0 ) && ( budget_for_per_heartbeat > 0 ); total_budget--,budget_for_per_heartbeat-- )
	{
		//
				//Create bootstrap nodes depending on whether this is a router or resource birth agent
				switch (node_type)
				{
					case BIRTHAGENT_USER_NODE:

						nodes_to_bootstrap_to.clear();

					break;

					case BIRTHAGENT_RESOURCE_NODE:
					//If there is not enough router nodes available in the network, then wait in case this is a resource node birth agent
					if ( If_ptr->list_of_router_nodes.size() < bootstrap_threashold )
					{
#ifdef DEBUG
						printf("Not enough bootstrap routers can be found, waiting for bootstrap routers to be added...\n");
#endif
						return;
					}

					case BIRTHAGENT_ROUTER_NODE:
						generate_random_bootstrap_nodes(bootstrap_threashold);

				}


			/*
			ResourceNode(unsigned int passed_message_time_to_live, unsigned int passed_routing_table_capacity, 
			unsigned int passed_k,unsigned int passed_num_centroid_tags_to_explore_per_period,
			unsigned int passed_num_queries_to_submit_per_period,
			Description &passed_description, unsigned int passed_id, unsigned int passed_vitality, unsigned int passed_extroversion,
			Tick passed_delay,
			Tick passed_response_timeout, BootstrapNodeList &passed_bootstrap_nodes,
			Infrastructure* passed_If_ptr );
			*/
				
				//Create bootstrap nodes depending on whether this is a router or resource birth agent
				switch (node_type)
				{
					
					case BIRTHAGENT_USER_NODE:

						generated_random_description.clear();


 						agent_ptr = new SpecificNode(message_time_to_live, 
							
									node_type_to_pass_on,
									known_routers_floor,
									known_routers_trim_threshold,
									known_routers_capacity,

									translation_radius,

									routing_table_length_floor,
									routing_table_length_trim_threshold,
									routing_table_max_length,

									routing_table_width_floor,
									routing_table_width_threshold,
									routing_table_min_num_resource_nodes,
									routing_table_max_width,

									k, 0, num_queries_to_submit_per_period,
									generated_random_description, (std::numeric_limits<unsigned int>::max() - If_ptr->user_node_id), 
									vitality, extroversion, 
									delay, response_timeout, nodes_to_bootstrap_to, If_ptr );

						If_ptr->user_node_id++;
					
					break;
				
					
					case BIRTHAGENT_RESOURCE_NODE:
						generate_random_description(1);

						agent_ptr = new SpecificNode(message_time_to_live, 
							
								node_type_to_pass_on,
								known_routers_floor,
								known_routers_trim_threshold,
								known_routers_capacity,

								translation_radius,

								routing_table_length_floor,
								routing_table_length_trim_threshold,
								routing_table_max_length,

								routing_table_width_floor,
								routing_table_width_threshold,
								routing_table_min_num_resource_nodes,
								routing_table_max_width,


							k, 0, num_queries_to_submit_per_period,
							generated_random_description, If_ptr->agent_id, vitality, extroversion, delay, response_timeout,
							 nodes_to_bootstrap_to, If_ptr );

						If_ptr->agent_id++;

					break;

					case BIRTHAGENT_ROUTER_NODE:
						generate_random_description(max_num_description);

						agent_ptr = new SpecificNode(message_time_to_live, 
							
								node_type_to_pass_on,
								known_routers_floor,
								known_routers_trim_threshold,
								known_routers_capacity,

								translation_radius,

								routing_table_length_floor,
								routing_table_length_trim_threshold,
								routing_table_max_length,

								routing_table_width_floor,
								routing_table_width_threshold,
								routing_table_min_num_resource_nodes,
								routing_table_max_width,


							k, num_centroid_tags_to_explore_per_period, num_queries_to_submit_per_period,
							generated_random_description, If_ptr->agent_id, vitality, extroversion, delay, response_timeout,
							 nodes_to_bootstrap_to, If_ptr );

						If_ptr->agent_id++;

				}//End of switch


#ifdef DEBUG
				printf("Successfully created node %u of type %d\n", agent_ptr->id, agent_ptr->node_type);
#endif


	}//end of for loop for num_cycles_per_heartbeat

	//If all cycles/budget have been exhausted then cancel the peridic task
	//	if (total_cycle_counter >= total_num_cycles )
	if (total_budget <=0 )
		{
			//Cancel the periodic event
			If_ptr->scheduler_ptr->cancel_periodic( handler_schedule_event_ptr); 

#ifdef DEBUG
			printf("Canceled birth agent node %u, type  %d\n", id, node_type);
#endif
		}

	return;
};






#endif


