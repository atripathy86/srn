#include <typeinfo>
#include <algorithm> //For merge
#include <iterator>
#include <functional>
#include <numeric>


//#include "Allheader.hpp"

#include "ForwardDeclaration.hpp"
#include "Node.hpp"
#include "Infrastructure.hpp"
#include "Listener.hpp"
#include "Message.hpp"
#include "Functor.hpp"
#include "OntologyRepository.hpp"
#include "Scheduler.hpp"
#include "Input.hpp"
#include "Database.hpp"


//*************These are for trimming routing table and known routers list *********************

KnownRouterIndexElement::KnownRouterIndexElement(RouterNode *passed_RouterNode_ptr   ): RouterNode_ptr(passed_RouterNode_ptr), 
				id(passed_RouterNode_ptr->id), birth_ticks( passed_RouterNode_ptr->If_ptr->scheduler_ptr->now()  ) {};
 

inline void ResourceNode::trim_known_routers(){
	//KnownRoutersIndex::iterator &known_routers_index_itr = *known_routers_index_itr_ptr;
	KnownRoutersIndexIterator known_routers_index_itr, known_routers_index_begin;
	known_routers_index_itr = known_routers_index_begin = (*known_routers_index_ptr).index.begin();
 	//Locate the range
	for (int fi = (*known_routers_index_ptr).size(); fi > known_routers_floor ;fi--, known_routers_index_itr++  )
  	{
		known_routers.erase((*known_routers_index_itr)->id);
		delete (*known_routers_index_itr);
	}
	(*known_routers_index_ptr).erase( known_routers_index_begin, known_routers_index_itr  );
	
	return;
};

//New
inline void RouterNode::trim_routing_table_length(){
	
 	//routing_table_itr =  routing_table.begin(); //*(routing_table[tag])
	routing_table_index_itr =  routing_table_index.begin();
	//Locate the range
	for (int fi = routing_table_index.size(); fi > routing_table_length_floor ;fi--, routing_table_index_itr++  )
	{
 		delete (*(routing_table.find(*routing_table_index_itr))).second;
		routing_table.erase( *routing_table_index_itr );
			
		//Remove from knowlede vector also
		/*
		knowledge_itr = knowledge.begin();
		for (int fj= knowledge.size();fj >0 && *knowledge_itr != *routing_table_index_itr ;fj--,knowledge_itr++  ){};
		knowledge.erase( knowledge_itr );
		*/

		knowledge.erase(remove(knowledge.begin(), knowledge.end(), *routing_table_index_itr), knowledge.end());

	}
  	routing_table_index.erase( routing_table_index.begin(), routing_table_index_itr );
 	
	return;
	
};


inline void RouterNode::trim_routing_table_specific_width(RoutingTableRow * row_to_trim_ptr){
	
	
	Destinations &destinations_to_trim = row_to_trim_ptr->destinations;
/*
	printf("Ticks %u: Node %u: RouterNode::trim_routing_table_specific_width(), size before trimming is %d, routing_table_width_floor is %d, routing_table_min_num_resource_nodes is %d, num of res is %d\n", 
		If_ptr->scheduler_ptr->now(), id, destinations_to_trim.size(),routing_table_width_floor, routing_table_min_num_resource_nodes, row_to_trim_ptr->num_resource_nodes );

	print_destinations(destinations_to_trim);
*/

	routing_table_entry_itr =  destinations_to_trim.begin(); //*(routing_table[tag])
	//routing_table_entry_to_remove_itr = routing_table_entry_itr;
	//Locate the range
     
	for (int fi = destinations_to_trim.size(); 
			fi > routing_table_width_floor && routing_table_entry_itr != destinations_to_trim.end()  ;
				routing_table_entry_itr++  ) 
	{
		//printf(" %u(%u) ", (*routing_table_entry_itr)->id, (*routing_table_entry_itr)->node_type );
		if ( ((*routing_table_entry_itr)->node_type == ROUTER_NODE ) || 
					row_to_trim_ptr->num_resource_nodes > routing_table_min_num_resource_nodes   ) 
		{
			//printf(" -erased ");

			//destinations_to_trim.erase( routing_table_entry_itr ); // BUG: iterator invalidated; line below used to read node_type after free
			bool erased_is_resource = ((*routing_table_entry_itr)->node_type == RESOURCE_NODE); // save before erase
			destinations_to_trim.erase( routing_table_entry_itr );
		
			//if ((*routing_table_entry_itr)->node_type == RESOURCE_NODE)	row_to_trim_ptr->num_resource_nodes--; // UAF: iterator invalid after erase
			if (erased_is_resource)	row_to_trim_ptr->num_resource_nodes--;

			fi--;
			routing_table_entry_itr = destinations_to_trim.begin();//routing_table_entry_itr;
		}
		
		//printf(", ");
		
 	};
	
	//printf("\n");

  	//printf("Ticks %u: Node %u: RouterNode::trim_routing_table_specific_width(), size after trimming is %d, num of res is %d\n", If_ptr->scheduler_ptr->now(), id, destinations_to_trim.size(), row_to_trim_ptr->num_resource_nodes );
	//print_destinations(destinations_to_trim);

	return;
};



inline void RouterNode::trim_routing_table_all_widths(){
	
	//for_each(routing_table.begin(), routing_table.end(), RouterNode::trim_routing_table_specific_col);
	routing_table_itr = routing_table.begin();

	for (int fi = routing_table.size();fi >0 ;fi--, routing_table_itr++ )
	{
		if ( (*routing_table_itr).second->destinations.size() > routing_table_width_threshold )
		{
			trim_routing_table_specific_width( (*routing_table_itr).second );
		}
	}

	return;
};


//*************Above these are for trimming routing table and known routers list *********************









inline 
QueryHandler::QueryHandler()
{
		/*expired =0;*/
		response_to_send_ptr = NULL;
		query_to_send_ptr = NULL;
		return;
};

/*
#ifdef DEBUG
inline 
QueryHandler::QueryHandler(Infrastructure *passed_If_ptr)
{
		If_ptr = passed_If_ptr;
		response_to_send_ptr = NULL;
		query_to_send_ptr = NULL;
		return;
};
#endif
*/

/*

QueryHandler::~QueryHandler()
{
	if (response_to_send_ptr) delete response_to_send_ptr;
	else delete query_to_send_ptr;
	//if (query_to_send_ptr) delete query_to_send_ptr;
	return;
};
*/

inline
void QueryHandler::transmit_response()
{
		//Send response
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( response_to_send_ptr->destination_node_ptr->id == parent_node_ptr->If_ptr->param_ptr->trace_source_node_id && 
				parent_node_ptr->id == parent_node_ptr->If_ptr->param_ptr->trace_destination_node_id &&
	           (*(response_to_send_ptr->description.begin())) == parent_node_ptr->If_ptr->param_ptr->trace_tag   )
#endif

	printf("Ticks:%u : Node %u, type %d: submitting response with query seq num %u, to node %u, type %d to node %u, type %d\n", 
			parent_node_ptr->If_ptr->scheduler_ptr->now(), parent_node_ptr->id, parent_node_ptr->node_type,
			response_to_send_ptr->query_seq_num, response_to_send_ptr->destination_node_ptr->id , response_to_send_ptr->destination_node_ptr->node_type,
				response_to_send_ptr->destination_node_ptr->id, response_to_send_ptr->destination_node_ptr->node_type	);
#endif
	response_to_send_ptr->destination_node_ptr->submit_response( *response_to_send_ptr );
	//expired = 1;
	return;
};

inline
void QueryHandler::transmit_query()
{

#ifdef DEBUG_L2


#ifdef SELECTIVE_PACKET_FILTER
	if (  query_to_send_ptr->source_node_ptr->id == parent_node_ptr->If_ptr->param_ptr->trace_source_node_id  &&
			(*(query_to_send_ptr->description.begin())) == parent_node_ptr->If_ptr->param_ptr->trace_tag	)
#endif

			printf("Ticks:%u : Node %u, type %d forwarding query with query_seq num %u, source node %u, type %d, with tag %u to node %u, type %d\n", 
			parent_node_ptr->If_ptr->scheduler_ptr->now(), parent_node_ptr->id, parent_node_ptr->node_type,
			query_to_send_ptr->query_seq_num, query_to_send_ptr->source_node_ptr->id , query_to_send_ptr->source_node_ptr->node_type,
				(*(query_to_send_ptr->description.begin())),
				query_to_send_ptr->destination_node_ptr->id, query_to_send_ptr->destination_node_ptr->node_type	);
#endif
	
	query_to_send_ptr->destination_node_ptr->submit_query( *query_to_send_ptr );
	//expired = 1;
	return;
};







Node::Node(Infrastructure* passed_If_ptr) 
{
	If_ptr = passed_If_ptr;
	node_type = PRIMARY_NODE;
	return;
};


ResourceNode::ResourceNode( unsigned int passed_message_time_to_live, 

			NodeType passed_node_type_to_pass_on,

			int passed_known_routers_floor,
			int passed_known_routers_trim_threshold,
			int passed_known_routers_capacity,

			int	passed_translation_radius,

			int passed_routing_table_length_floor,
			int passed_routing_table_length_trim_threshold,
			int passed_routing_table_max_length,

			int passed_routing_table_width_floor,
			int passed_routing_table_width_threshold,
			int passed_routing_table_min_num_resource_nodes,
			int passed_routing_table_max_width,

			int passed_k, int passed_num_centroid_tags_to_explore_per_period,
			int passed_num_queries_to_submit_per_period,
			Description &passed_description, unsigned int passed_id, unsigned int passed_vitality, int passed_extroversion,
			Tick passed_delay,
			Tick passed_response_timeout, BootstrapNodeList &passed_bootstrap_nodes,
			Infrastructure* passed_If_ptr) : Node(passed_If_ptr) 
	{
			node_type = passed_node_type_to_pass_on;
			translation_radius = passed_translation_radius;

		//For trimming
			known_routers_floor = passed_known_routers_floor;
			known_routers_trim_threshold = passed_known_routers_trim_threshold;
			known_routers_capacity = passed_known_routers_capacity;
			this_ptr = this; 
			known_routers_index_ptr = new KnownRoutersIndexEncapsulator(this);

		num_queries_to_submit_per_period = passed_num_queries_to_submit_per_period;
		message_time_to_live = passed_message_time_to_live;
		id = passed_id;
		vitality = passed_vitality;
		extroversion = passed_extroversion;
		delay = passed_delay;
		current_query_sequence_id = 0;
		response_timeout = passed_response_timeout;

		description = passed_description;
		bootstrap_nodes = passed_bootstrap_nodes;

		current_query_sequence_id =0;
 		/*Push it in resource node list for time being, will check and remove later in router node constructor if needed*/

		If_ptr->list_of_resource_nodes.push_back(this);

#ifdef DEBUG
				listener_seq_id = 0;
#endif


#ifdef DEBUG
		printf("Ticks:%u : Node %u : Node being created...\n",
			If_ptr->scheduler_ptr->now(), id );
#endif

#ifdef DEBUG_NODE

		printf(" passed_message_time_to_live %u, passed_routing_table_capacity %u, passed_k %d, \n \
			passed_id %u, passed_vitality %d, passed_extroversion %d, passed_response_timeout %u, \n ",
			 
			 passed_message_time_to_live, passed_routing_table_capacity, passed_k,
			passed_id, passed_vitality, passed_extroversion, passed_response_timeout );
	
#endif

		begin_life(/*bootstrap_nodes*/);
		return;
	};

	ResourceNode::~ResourceNode()
	{
		cleanup();
	}

inline void ResourceNode::cleanup(){

#ifdef DEBUG_NODE
			printf("Resource node id %u getting deleted\n", /*(*bootstrap_nodes_itr)->id, */ id);
#endif
		
		description.clear();
		bootstrap_nodes.clear();

		/*for (num_elements  = query_bag_for_advertising.size(),query_itr = query_bag_for_advertising.begin();
					num_elements >0 ;
					num_elements--,query_itr++ )*/
		query_itr = query_bag_for_advertising.begin();
		for (int fi  = query_bag_for_advertising.size();
					fi >0 ;
					fi--,query_itr++ )
		{
			//sm24
			//If_ptr->list_of_queries.erase((*query_itr)->id);
			delete (*query_itr);
		}
		query_bag_for_advertising.clear(); //Requires cleaning of Queries

#ifdef DEBUG_NODE
			printf("~ResourceNode(): Reached here, cleaning Listneners...\n");
#endif

 		/*for (num_elements = listener_array.size(),listener_itr = listener_array.begin();
					num_elements >0 ;
					num_elements--,listener_itr++ )*/
		listener_itr = listener_array.begin();
		for (int fi = listener_array.size();
					fi >0 ;
					fi--,listener_itr++ )
 			delete (*listener_itr).second;

 		listener_array.clear();// Requires cleaning of Listener pointers

		known_routers.clear();
		//query_handler_array.clear();

#ifdef DEBUG_NODE
			printf("Finished cleaning up for Router node %u \n",  id);
#endif
		return;
	}

	RouterNode::~RouterNode()
	{
#ifdef DEBUG_NODE
			printf("Router node id %u getting deleted\n", id);
#endif

		/*for (num_elements = query_bag_for_exploring.size(),query_itr = query_bag_for_exploring.begin();
					num_elements >0 ;
					num_elements--,query_itr++ )*/
		query_itr = query_bag_for_exploring.begin();
		for (int fi = query_bag_for_exploring.size();
					fi >0 ;
					fi--,query_itr++ )
		{
			//If_ptr->list_of_queries.erase((*query_itr)->id);
			delete (*query_itr);
		}
		query_bag_for_exploring.clear(); //Requires cleaning of Queries

		nearest_k.clear();
		knowledge.clear();
		resources_attached.clear();
		routers_attached.clear();
		descriptions_to_explore.clear();

		RoutingTableIterator routing_table_itr = routing_table.begin();
		//while (routing_table_itr != routing_table.end())
		for(int fi=routing_table.size();fi>0;fi--)
		{
			((*routing_table_itr).second)->destinations.clear();
			routing_table_itr++;
		}

		routing_table.clear();

#ifdef DUPPACK_SET
		query_already_received_set.clear();
#endif

#ifdef DUPPACK_CACHE
		message_key_cache.clear();
#endif
#ifdef DEBUG_NODE
			printf("Finished cleaning up for Router node %u \n",  id);
#endif
		cleanup();

		return;
	}

inline void ResourceNode::begin_life(/*BootstrapNodeList &bootstrap_nodes*/) {
	/*typedef std::list< RouterNode * > BootstrapNodeList;*/


	bootstrap_action();

	//To do, Schedule itself
	//Create an event
	handler_schedule_event_ptr = new Event;

#ifdef DEBUG
	handler_schedule_event_ptr->event_id = If_ptr->event_id;
	If_ptr->event_id++;
	handler_schedule_event_ptr->node_ptr = this;
#endif

	//Create a functor, assign the object and its handling method corresponding to the event 	
	/*SpecificFunctor(TClass* _pt2Object, void(TClass::*_fpt)()); */
	SpecificFunctor<ResourceNode> *functor_resource_node_ptr = 
		new SpecificFunctor<ResourceNode>( this, &ResourceNode::heartbeat );
			
	handler_schedule_event_ptr->handler= functor_resource_node_ptr;
	//Setup the periodicty
	handler_schedule_event_ptr->time = vitality;
	//Schedule the periodic event
	If_ptr->scheduler_ptr->schedule_periodic( handler_schedule_event_ptr); 

	return;

};


inline void ResourceNode::bootstrap_action(){

				BootstrapNodeList::iterator bootstrap_nodes_itr = bootstrap_nodes.begin();
				//For each bootstrap node
				//while (bootstrap_nodes_itr != bootstrap_nodes.end())
				for(int fi= bootstrap_nodes.size();fi>0;fi--)
				{

					//router_node_to_attach_to_ptr = dynamic_cast<RouterNode*>(*bootstrap_nodes_itr);
					router_node_to_attach_to_ptr = *bootstrap_nodes_itr;

#ifdef DEBUG_NODE
						printf("Inserting router node %u, type %u to known list of %u \n", router_node_to_attach_to_ptr->id, 
								router_node_to_attach_to_ptr->node_type, id);
#endif

					//Mark this as known anyway
					known_routers.insert( 
								make_pair( (*router_node_to_attach_to_ptr).id ,  router_node_to_attach_to_ptr )
							); 
#ifdef DEBUG_L1

#ifdef SELECTIVE_PACKET_FILTER
					if (id == If_ptr->param_ptr->trace_source_node_id && 
						router_node_to_attach_to_ptr->id == If_ptr->param_ptr->trace_destination_node_id )
#endif

						printf("Ticks:%u : Node %u requesting router node %u to attach it \n", 
							If_ptr->scheduler_ptr->now(),id, router_node_to_attach_to_ptr->id );
#endif

					if (! (*router_node_to_attach_to_ptr).attach_request(this)) 
						std::cout << "Node : "<< id << "failed to bootstrap with routernode : "<<
						(*router_node_to_attach_to_ptr).id 	<<std::endl;
					else /* 	std::map< unsigned int, RouterNode *> known_routers;  */
					{

#ifdef DEBUG_NODE
						printf("Bootstraping to router node : %u succeeded \n", router_node_to_attach_to_ptr->id);
#endif
					}

					bootstrap_nodes_itr++;

				}
 
#ifdef DEBUG
						printf("Ticks:%u : Node %u: The routers known to this node are : ",If_ptr->scheduler_ptr->now(),id );
						print_map_known_routers( known_routers );
#endif


				//Update the query bag for self advertising purpose
					update_query_bag(description, query_bag_for_advertising );


#ifdef DEBUG_NODE
					printf("Ticks:%u : Node %u,  node type : %d : finished updating query bag\n", 
							If_ptr->scheduler_ptr->now(),id, node_type);
#endif
				

}


inline
void ResourceNode::heartbeat() {

#ifdef DEBUG_NODE
		printf("Ticks:%u : Node %u, node type %d : ResourceNode::heartbeat()... \n", 
					If_ptr->scheduler_ptr->now(),id, node_type);
#endif

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
		if (id == If_ptr->param_ptr->trace_source_node_id  )
#endif
		{
			printf("Ticks:%u : Node %u: ResourceNode::heartbeat()......\n", If_ptr->scheduler_ptr->now(),id );
		}
#endif

	periodic_housekeeping();

	//Advertise itself
	seek_new_routers_to_attach_to();


#ifdef DEBUG_NODE


#ifdef SELECTIVE_PACKET_FILTER
	 if (id == If_ptr->param_ptr->trace_source_node_id  )
#endif

		print_listener_status();  
#endif

#ifdef DEBUG_NODE
		printf("ResourceNode::heartbeat(): Node heartbeat of node id %u, node type : %d, finished seeking routers to attach\n", id, node_type);
#endif
	return;
};

//Known router trimming
inline
void ResourceNode::periodic_housekeeping(){

	if (known_routers.size() > known_routers_capacity) trim_known_routers();

};




inline
void RouterNode::heartbeat() {

	//print_routing_table();

#ifdef DEBUG_NODE
		printf("RouterNode::heartbeat(): Node heartbeat of node id %u, node type : %d \n", id, node_type);
#endif

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
		if (id == If_ptr->param_ptr->trace_source_node_id  )
#endif
		{
			printf("Ticks:%u : Node %u: RouterNode::heartbeat()......\n", If_ptr->scheduler_ptr->now(),id );
		}
#endif
 
	periodic_housekeeping();

	//Advertise itself
	seek_new_routers_to_attach_to();

#ifdef DEBUG_NODE

#ifdef SELECTIVE_PACKET_FILTER
	 if (id == If_ptr->param_ptr->trace_source_node_id  )
#endif

		print_listener_status();  
#endif

	//Try to update its routing table (brought in from the RouterNode() )
	
	/*
			//Update router's resources that will be used to update the routing table, removed from RouterNode()
			update_descriptions_to_explore();
			update_query_bag(descriptions_to_explore, query_bag_for_exploring);
			
			curr_query_bag_for_exploring_itr = query_bag_for_exploring.begin();

			//Try to update its routing table
			seek_new_resources_to_update_routing_table();

	*/

	//Update router's resources that will be used to update the routing table
	update_descriptions_to_explore();

	/*Update the query bag for only those tags for which queries can be send (withing budget),
	insert at the end of the list, then if budget is left out, send queries from the front of the list
	based on the given list itr, the list itr needs to be updated based on the index
	*/

	seek_new_resources_to_update_routing_table();


#ifdef DEBUG_NODE
		printf("RouterNode::heartbeat(): Node heartbeat of node id %u, node type : %d, finished seeking routers to attach\n", id, node_type);
#endif
	return;
};


inline void ResourceNode::seek_new_routers_to_attach_to() {

	/* Insert itself to its own known_routers list, this will ensure that the router sends queries 
		to all the other router nodes it knows through its routing table, in addition to its known_router list
		this also requires that the rouuter should not respond to itself !! Fix that also in RouterNode::submit_query method*/
			//If itself is not inserted then insert itself, i.e. first time
		if ( (node_type == ROUTER_NODE) && ( known_routers.find( id ) == known_routers.end() ) 	)
		{
				
				router_node_to_attach_to_ptr = dynamic_cast<RouterNode*>(this);

				//	std::map< unsigned int, RouterNode *> known_routers;
				known_routers.insert( 
						//make_pair( id ,  *(&router_node_to_attach_to_ptr) )
						make_pair( id ,   router_node_to_attach_to_ptr  )
					); 
		}
	

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
		if (id == If_ptr->param_ptr->trace_source_node_id  )
#endif


		{

			printf("Ticks:%u : Node %u: The routers attached to this node are : ", If_ptr->scheduler_ptr->now(),id );
			print_map_known_routers( known_routers );
			printf("Ticks:%u : Node %u: Advertising phase: Seeking new routers, sending %u queries, other routers should start receiving them...\n",
				If_ptr->scheduler_ptr->now(),id , query_bag_for_advertising.size() );
		}
#endif
	
	query_itr = query_bag_for_advertising.begin();

	send_queries_from(query_bag_for_advertising, query_itr, query_bag_for_advertising.size());
	
	return;
}




inline void ResourceNode::send_queries_from(std::list<Query*> &query_bag, std::list<Query*>::iterator & curr_query_bag_itr , int num_queries_to_send) {
	

			//For each query 
			//query_itr = query_bag.begin();
			//while ( query_itr !=  query_bag.end()  )
			//for (num_elements = 0 ; num_elements < num_queries_to_send;num_elements++ )
			for(int fi=0;fi<num_queries_to_send;fi++)
			{

				//if (curr_query_bag_itr == query_bag.end()) // BUG: wrap-around did not guard against empty bag
				if (curr_query_bag_itr == query_bag.end()) {
								curr_query_bag_itr = query_bag.begin();
								if (curr_query_bag_itr == query_bag.end()) break; // empty bag: avoid null-deref
				}


#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
							if ( (id == If_ptr->param_ptr->trace_source_node_id  && 
									(*((*curr_query_bag_itr)->description.begin())) == If_ptr->param_ptr->trace_tag	) 

#ifdef ALL_USERNODE_PACKETS
									|| ( node_type == USER_NODE )
#endif
										)

#endif

							printf("Ticks:%u : Node id %u, node type : %d :  allocating a listener with id %u, for tag %u, query seq num to use is %u, scheduling its time out, the event_id is %u\n", 
								If_ptr->scheduler_ptr->now(),id, node_type, listener_seq_id, (*((*curr_query_bag_itr)->description.begin())), 
								current_query_sequence_id, If_ptr->event_id );
#endif
							
						//Setup a dedicated listener for this query
						listener_ptr = new Listener(this, current_query_sequence_id);

#ifdef DEBUG
						listener_ptr->id = listener_seq_id; //For troubleshooting
						listener_seq_id++;
#endif


						//Insert Listener in the array
						//current_query_sequence_id_ptr = new int;
						//*current_query_sequence_id_ptr = current_query_sequence_id;
						//listener_array.insert( make_pair(*current_query_sequence_id_ptr, listener_ptr) ); 
						//The line below replaces the three lines above, this cut downs memory leakage due to (new int)
						listener_array.insert( make_pair(current_query_sequence_id, listener_ptr) );

						//Schedule the time out for this listener;
						handler_schedule_event_ptr = new Event;

#ifdef DEBUG
					handler_schedule_event_ptr->event_id = If_ptr->event_id;
					If_ptr->event_id++;
					handler_schedule_event_ptr->node_ptr = this;
					//handler_schedule_event_ptr->message_to_handle_ptr = 0;

#endif

						/*SpecificFunctor(TClass* _pt2Object, void(TClass::*_fpt)());*/
						functor_listener_ptr = new SpecificFunctor<Listener>( listener_ptr,
													 &Listener::handle_recvd_responses );

						handler_schedule_event_ptr->handler= functor_listener_ptr;
						handler_schedule_event_ptr->time = response_timeout + If_ptr->scheduler_ptr->now();
						If_ptr->scheduler_ptr->schedule_event( handler_schedule_event_ptr); 

						//Identify a router from the aquinstance list, submit a query to it
						router_node_address_itr = known_routers.begin();
						//while (router_node_address_itr != known_routers.end())
						for(int fi=known_routers.size();fi>0;fi--)
						{
							//Make a new Query
							query_ptr = new Query();
							//query_ptr = new Query(If_ptr->query_id);
							//sm24

							//If_ptr->list_of_queries.insert(make_pair(If_ptr->query_id,query_ptr));
							//If_ptr->query_id++;
							If_ptr->database_ptr->Num_total_queries_generated++;
							//SM: ADD PROBE - new message generated by router--node : update total message generation
							//Suneil Jul-1
							
							switch (node_type)
							{
								case RESOURCE_NODE:
									If_ptr->database_ptr->Queries_sent_from_ResourceNode++;
									If_ptr->database_ptr->Num_total_message_generated++;
									If_ptr->database_ptr->Num_of_queries_for_specific_tag[*( (*curr_query_bag_itr)->description.begin() )]++;
								break;
								case ROUTER_NODE:
									If_ptr->database_ptr->Queries_sent_from_RouterNode++;
									If_ptr->database_ptr->Num_total_message_generated++;
									If_ptr->database_ptr->Num_of_queries_for_specific_tag[*( (*curr_query_bag_itr)->description.begin() )]++;

								break;
								case USER_NODE:
									//For external queries from user node
									If_ptr->database_ptr->Queries_sent_from_UserNode++;
									If_ptr->database_ptr->Num_total_message_generated_for_UserNode++;
									If_ptr->database_ptr->Num_of_queries_for_specific_tag_from_UserNode[*( (*curr_query_bag_itr)->description.begin() )]++;

							
							}
							
							//*query_ptr = *(*query_itr);
							*query_ptr = *(*curr_query_bag_itr); //This is where the problem lies
							
							//(*query_ptr).id = If_ptr->query_id - 1; // This solves the previous problem, that id was rewritten when the whole query obj was copied

							query_ptr->query_seq_num = current_query_sequence_id;

							query_ptr->destination_node_ptr = (*router_node_address_itr).second;
							//query_ptr->destination_node_id = query_ptr->destination_node_ptr->id;
							query_ptr->destination_node_id = (*router_node_address_itr).first; //Reduces memory dereferencing and cache hit


							/* 	std::map< unsigned int, RouterNode *> known_routers;*/
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
								if ( (id == If_ptr->param_ptr->trace_source_node_id) 
										&& (*((*query_ptr).description.begin())) == If_ptr->param_ptr->trace_tag
#ifdef ALL_USERNODE_PACKETS
										/*|| ( node_type == USER_NODE ) */  
#endif
											
										)

#endif
								{

										printf("Ticks:%u : Node %u: Sending query to router id %u, query_seq_num as %u, filter %u, with description tags :", 
										If_ptr->scheduler_ptr->now(),id ,(*(*router_node_address_itr).second).id, query_ptr->query_seq_num, query_ptr->only_to_node_type  );
										print_map_description( (*curr_query_bag_itr)->description );
								}
#endif

							query_ptr->destination_node_ptr->submit_query(*query_ptr);

							router_node_address_itr++;
						}//end of loop for routers

						//query_itr++;
						curr_query_bag_itr++;
						current_query_sequence_id++;

			}//end of loop for queries


	return;
}



inline void RouterNode::update_descriptions_to_explore() {
	
	/*
	For each description item (tag), get all the tags from ontology repository within "extroversion" hops 
	from the given description item, then load these descriptions in the "descriptions_to_explore"
	*/

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( id == If_ptr->param_ptr->trace_source_node_id  || id == If_ptr->param_ptr->trace_destination_node_id  ) 
#endif

			printf("Ticks:%u : Node %u: Exploration phase ...\n", If_ptr->scheduler_ptr->now(),id );

#endif


	/*for (num_elements  = num_centroid_tags_to_explore_per_period;
					num_elements > 0 ;
					num_elements--,description_budget_itr++ )*/
	for (int fi= num_centroid_tags_to_explore_per_period;
					fi > 0 ;
					fi--,description_budget_itr++ )
	{
			//If all description centroid topics are scanned once, then increment the radius and start scanning again
			if (description_budget_itr == description.end())
			{
				semantic_space_inner_radius = semantic_space_inner_radius + extroversion;
				description_budget_itr = description.begin();
			}

			/*If_ptr->onto_repository_ptr->generate_new_neighbors_to_explore( (*description_budget_itr), semantic_space_inner_radius,
								extroversion,  nearest_k);*/
             generate_new_neighbors_to_explore( (*description_budget_itr) , semantic_space_inner_radius,
								extroversion /*,  nearest_k*/);

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( id == If_ptr->param_ptr->trace_source_node_id  || id == If_ptr->param_ptr->trace_destination_node_id  ) 
#endif
			printf("Ticks:%u : Node %u: RouterNode %u, type %d : for centroid tag %u and inner radius %u, the nearest_k contents are : ",
					If_ptr->scheduler_ptr->now(),id, id, node_type, *description_budget_itr, semantic_space_inner_radius);

#endif

			/*for (num_elements1  = nearest_k.size(),nearest_k_itr = nearest_k.begin();
							num_elements1 > 0 ;
							num_elements1--,nearest_k_itr++ )*/
			nearest_k_itr = nearest_k.begin();
			for (int fj  = nearest_k.size();
							fj > 0 ;
							fj--,nearest_k_itr++ )
			{				
							//Attempts to insert, if matching item does not exist then returns true
							//and the item is inserted in the temp_description_to_explore
							//This filter out only the new tags that were not explored earlier
							if ( (descriptions_to_explore.insert( *nearest_k_itr )).second )
								temp_descriptions_to_explore.insert( *nearest_k_itr ) ;

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
			if ( id == If_ptr->param_ptr->trace_source_node_id  || id == If_ptr->param_ptr->trace_destination_node_id  ) 
#endif
				printf("%u ,", *nearest_k_itr);

#endif
			}


#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( id == If_ptr->param_ptr->trace_source_node_id  || id == If_ptr->param_ptr->trace_destination_node_id  ) 

#endif
		printf("\n");
#endif



	}



#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( id == If_ptr->param_ptr->trace_source_node_id  || id == If_ptr->param_ptr->trace_destination_node_id  ) 
	{
		printf("Ticks:%u : Node %u: RouterNode %u, type %d : the descriptions_to_explore conents are : ",
					If_ptr->scheduler_ptr->now(),id, id, node_type);

		/*	for (num_elements  = descriptions_to_explore.size(),description_itr = descriptions_to_explore.begin();
					num_elements > 0 ;
					num_elements--,description_itr++ )*/
		description_itr = descriptions_to_explore.begin();
		for (int fk  = descriptions_to_explore.size();
					fk > 0 ;
					fk--,description_itr++ )
							printf("%u ,", *description_itr);
		printf("\n");

		printf("Ticks:%u : Node %u: RouterNode %u, type %d : the temp_descriptions_to_explore contents are : ",
					If_ptr->scheduler_ptr->now(),id, id, node_type);


		/*	for (num_elements  = temp_descriptions_to_explore.size(),
					description_itr = temp_descriptions_to_explore.begin();
					num_elements > 0 ;
					num_elements--,description_itr++ )*/
		
		description_itr = temp_descriptions_to_explore.begin();
		for (int fk  = temp_descriptions_to_explore.size();
					fk > 0 ;
					fk--,description_itr++ )
							printf("%u ,", *description_itr);
		printf("\n");

	}
#endif

#endif

};





inline void RouterNode::seek_new_resources_to_update_routing_table() {
		

#ifdef DEBUG_NODE
			printf("\nNode id %u, node type : %d :  seek_new_resources_to_update_routing_table(): finished updating query bag\n", id, node_type);
#endif
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
		if (id == If_ptr->param_ptr->trace_source_node_id   )
#endif

		{

			printf("Ticks:%u : Node %u: The routers attached to this node are : ",If_ptr->scheduler_ptr->now(),id  );
			print_map_known_routers( known_routers );
			printf("Ticks:%u : Node %u: Exploration phase : Seeking new resources and routers, sending %u queries, other resources and routers should start receiving them...\n"
						, If_ptr->scheduler_ptr->now(),id , num_queries_to_submit_per_period);

		}
#endif

	
		for ( num_elements1  = num_queries_to_submit_per_period ; 
					num_elements1 > 0 && ! (temp_descriptions_to_explore.empty()) ;
					num_elements1--)
				{

								query_ptr = new Query();
								//query_ptr = new Query(If_ptr->query_id);
								//sm24

								//If_ptr->list_of_queries.insert(make_pair(If_ptr->query_id,query_ptr));


								//If_ptr->query_id++;
													
								query_ptr->source_node_id = id;
								query_ptr->source_node_ptr = this;
								query_ptr->time_to_live = message_time_to_live;
								switch (node_type)
								{
									case RESOURCE_NODE:
										query_ptr->only_to_node_type = TO_ROUTER_NODE;
									break;
 
									case USER_NODE:
										query_ptr->only_to_node_type = TO_RESOURCE_NODE;

								}

								query_ptr->description.insert( *(temp_descriptions_to_explore.begin()) ); //Copy semantics so should not be probblem

								query_bag_for_exploring.push_back(query_ptr);
								temp_query_bag_for_exploring.push_back(query_ptr);
								
								temp_descriptions_to_explore.erase( (temp_descriptions_to_explore.begin()) );

				
				}// Loop on description

	query_itr = temp_query_bag_for_exploring.begin();
	send_queries_from(temp_query_bag_for_exploring, query_itr, temp_query_bag_for_exploring.size() );
	temp_query_bag_for_exploring.clear();

	//If residual budget is left out then send queries from old queries from query_bag_for_exploring
	query_itr = query_bag_for_exploring.begin();
	if (num_elements1)
	     send_queries_from(query_bag_for_exploring, query_itr, num_elements1 );

	return;		
		
};



inline void ResourceNode::update_query_bag(Description &from_description, std::list<Query*> &to_query_bag){

				description_itr = from_description.begin();
#ifdef DEBUG_NODE
				if (node_type == USER_NODE)
					printf("Ticks:%u : Node id %u, node type : %d : update_query_bag_for_advertising() : total description size is %d, Tag values are: ",
							If_ptr->scheduler_ptr->now(),id, node_type, description.size() );
#endif
				
				//while ( description_itr != from_description.end())
				for(int fi=from_description.size();fi>0;fi--)
				{
					//tag_to_use_for_query_ptr = new Tag;//This was source of memeory leak !! removed it
					//*tag_to_use_for_query_ptr = (*description_itr);

					query_ptr = new Query();
					//query_ptr = new Query(If_ptr->query_id);
					//sm24

					//If_ptr->list_of_queries.insert(make_pair(If_ptr->query_id,query_ptr));


					//If_ptr->query_id++;
										
					query_ptr->source_node_id = id;
					query_ptr->source_node_ptr = this;
					query_ptr->time_to_live = message_time_to_live;
					switch (node_type)
					{
						case RESOURCE_NODE:
							query_ptr->only_to_node_type = TO_ROUTER_NODE;
						break;
						/* The Query normaly has this fixed to both nodes, so save a few instructions here by not repeating
						case ROUTER_NODE:
							query_ptr->only_to_node_type = TO_RESOURCE_NODE | TO_ROUTER_NODE;
						break;
						*/

						case USER_NODE:
							query_ptr->only_to_node_type = TO_RESOURCE_NODE;

					}

					//query_ptr->description.insert( *tag_to_use_for_query_ptr );
					query_ptr->description.insert( *description_itr ); //Copy semantics so should not be probblem

					to_query_bag.push_back(query_ptr);

					description_itr++;

#ifdef DEBUG_NODE
				if (node_type == USER_NODE)
					printf(" %d ", *description_itr);
#endif



				}// Loop on description

#ifdef DEBUG_NODE
				if (node_type == USER_NODE)
					printf("\n");
#endif


	return;
}; //end of update_query_bag_for_advertising()




inline void ResourceNode::submit_response(Response &submitted_response) {

	submitted_response.hop_count++;

#ifdef DEBUG_L2
 
#ifdef SELECTIVE_PACKET_FILTER
			if (id == If_ptr->param_ptr->trace_source_node_id && 
				submitted_response.source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id &&
				(*(submitted_response.description.begin())) == If_ptr->param_ptr->trace_tag	)
#endif

		printf("Ticks:%u : Node %u: Response with query_seq_num %u, Tag %u, received by Node id %u, node type %d, with source node %u, type %d\n", 
			//If_ptr->scheduler_ptr->now(),submitted_response.query_seq_num, (*(submitted_response.description.begin())).first , id, node_type, submitted_response.source_node_ptr->id, submitted_response.source_node_ptr->node_type );
			If_ptr->scheduler_ptr->now(),id, submitted_response.query_seq_num, (*(submitted_response.description.begin())) , id, node_type, submitted_response.source_node_ptr->id, submitted_response.source_node_ptr->node_type );
	//printf("Response received by Node id %u, node type : %d \n", id, node_type);
	//if (submitted_response.source_node_id == id) printf("This router node has submitted a response to itself\n");
#endif

	
	if ( submitted_response.destination_node_id != id )
		{	std::cout << "Error: This node :" << id << 
				" should not receive this response from node :"
					<< submitted_response.source_node_id << ", the destination actually is "
					<<submitted_response.destination_node_id  <<"source of memory leakage "<<std::endl;

			//sm24
			//If_ptr->list_of_responses.erase(submitted_response.id);
			delete &submitted_response;

			//If_ptr->list_of_messages.erase(&submitted_response);

			cout<<"Deleted the response"<<endl;
			return;
		}

	listener_itr = listener_array.find( submitted_response.query_seq_num );

	//	std::map<unsigned int, Listener> listener_array;

	if ( listener_itr != listener_array.end() )  
	{
		(*((*listener_itr).second)).recv(submitted_response);
		
	}
	else {
		/*
 		std::cout << "Ticks:"<<If_ptr->scheduler_ptr->now()<<" : Node "<<id<<": May be an error, : This node doesn't have listener to receive response from node, or listner expired due to small response timeout "
			<< submitted_response.source_node_id << " , query_seq_num is "<< submitted_response.query_seq_num <<
				", for Tag "<<(*(submitted_response.description.begin()))<<std::endl;
		*/
			//sm24
			//If_ptr->list_of_responses.erase(submitted_response.id);
			delete &submitted_response;
			If_ptr->database_ptr->Num_reponses_dropped_when_no_active_listeners++;

			//If_ptr->list_of_messages.erase(&submitted_response);

			//cout<<"Deleted the response"<<endl;
		}
	return;
};



inline void ResourceNode::submit_query(Query &submitted_query) {
	
	submitted_query.hop_count++;


#ifdef DEBUG_L2
		//printf("ResourceNode::submit_response: Reached here \n");

#ifdef SELECTIVE_PACKET_FILTER
		if ( submitted_query.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id   && 
				id == If_ptr->param_ptr->trace_destination_node_id &&
				(*(submitted_query.description.begin())) == If_ptr->param_ptr->trace_tag	)
#endif

		printf("Ticks:%u : Node %u: Query with query_seq_num %u, Tag %u, received by Node id %u, node type %d, with source node node %u, type %d\n", 
			//If_ptr->scheduler_ptr->now(),submitted_query.query_seq_num, (*(submitted_query.description.begin())).first, id, 
		If_ptr->scheduler_ptr->now(),id,submitted_query.query_seq_num, (*(submitted_query.description.begin())), id, 
			node_type, submitted_query.source_node_ptr->id, submitted_query.source_node_ptr->node_type );

	//printf("Response received by Node id %u, node type : %d \n", id, node_type);
	//if (submitted_response.source_node_id == id) printf("This router node has submitted a response to itself\n");
#endif

	/*
	if (submitted_query.source_node_id != id) submitted_query.hop_count++; //Why is this like this ?? What is the point of incrementing hoop count again ?
	else cout <<"Error : this resource node has submitted a query to itself\n"<< endl;
	*/
	if (submitted_query.source_node_id == id) 
	{
		cout <<"Error : this resource node has submitted a query to itself\n"<< endl;
		goto drop_query;
	}


	//if (submitted_query.hop_count >= submitted_query.time_to_live)
	if ( can_accept_message(submitted_query) )
	{

		//description_matched = verify_against_description(Query &submitted_query);
		/*
		If matched, then allocate a handler with response action
		if not matched then drop it is not for this resource node
		*/

#ifdef DEBUG_NODE
				printf("ResourceNode::submit_query(): Reached here 1: Query received by Node id %u, node type : %d : ", id, node_type);
#endif

			//if ( ( submitted_query.source_node_id != id) && verify_against_description(submitted_query) )
			if ( verify_against_description(submitted_query) ) //No need for submitted_query.source_node_id != id, as that is already checked above
			{

#ifdef DEBUG_NODE
					printf("ResourceNode::submit_query(): Reached here 2 : Query received by Node id %u, node type : %d :  allocating a query handler, scheduling its response action\n", id, node_type);
#endif

				response_action(submitted_query);
			} 
		
	}

	drop_query:
		//Now drop the message
		//sm24
		//If_ptr->list_of_queries.erase(submitted_query.id);
		delete &submitted_query;

		//If_ptr->list_of_messages.erase(&submitted_query);

		//Also report this to the database that query has been deleted (for matching of memory leakage audit purpose

		return;
};



inline void ResourceNode::response_action(Query &query_to_handle) {

	/*Check for message only_to_node_type filter, if not meant for router 
	then this function should have no impact (returns) */

	if (	(query_to_handle.source_node_ptr->node_type== USER_NODE) &&
			!(
			((query_to_handle.only_to_node_type & TO_ROUTER_NODE) && (node_type == ROUTER_NODE)) ||

			((query_to_handle.only_to_node_type & TO_RESOURCE_NODE) && (node_type == RESOURCE_NODE))
			)
		)
	{

		return;
	}

	//send response to sender
	//Construct a response
	response_ptr = new Response();

	//sm24
	//If_ptr->list_of_responses.insert(make_pair(If_ptr->response_id,response_ptr));

	//If_ptr->response_id++;
	If_ptr->database_ptr->Num_total_responses_generated++;
	//SM: ADD PROBE : Update num responses sent: 'total-responses'


	switch (query_to_handle.source_node_ptr->node_type)
	{
		case RESOURCE_NODE:
			If_ptr->database_ptr->Responses_sent_from_ResourceNode++;
			If_ptr->database_ptr->Num_total_message_generated++;
			If_ptr->database_ptr->record_hops(query_to_handle.hop_count,If_ptr->database_ptr->hops_before_response_ctr ); //Suneil
		break;

		case ROUTER_NODE:
			If_ptr->database_ptr->Responses_sent_from_RouterNode++;
			If_ptr->database_ptr->Num_total_message_generated++;
			If_ptr->database_ptr->record_hops(query_to_handle.hop_count,If_ptr->database_ptr->hops_before_response_ctr ); //Suneil

		break;
		case USER_NODE:
			//For external queries from user node
			If_ptr->database_ptr->Num_responses_sent_for_UserNode++;
			If_ptr->database_ptr->Num_total_message_generated_for_UserNode++;
			If_ptr->database_ptr->record_hops(query_to_handle.hop_count,If_ptr->database_ptr->hops_before_response_ctr_for_UserNode ); //Suneil
			
	}
							
	response_ptr->time_to_live = message_time_to_live;

	response_ptr->source_node_id = id;
	response_ptr->source_node_ptr = this;

	response_ptr->destination_node_id = query_to_handle.source_node_id;
	response_ptr->destination_node_ptr = query_to_handle.source_node_ptr;

	response_ptr->query_seq_num =  query_to_handle.query_seq_num;
	response_ptr->description = query_to_handle.description ;
	response_ptr->hop_count = 0;


	//Transfer the list content to the response, so that src node could infer from that
#ifdef DETECT_LOOP
	//unsigned int node_visited[64];
	memcpy( response_ptr->node_visited, query_to_handle.node_visited, NODE_VISTED_LIST_BYTES);
#endif

	query_handler_ptr = new QueryHandler();
	query_handler_ptr->response_to_send_ptr = response_ptr;

		//Schedule the wake up time for this QueryHandler;
		handler_schedule_event_ptr = new Event;

#ifdef DEBUG_NODE

#ifdef SELECTIVE_PACKET_FILTER
		if ( query_to_handle.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id && 
				id == If_ptr->param_ptr->trace_destination_node_id &&
				(*(query_to_handle.description.begin())) == If_ptr->param_ptr->trace_tag )
#endif

	printf("Ticks:%u : Node %u: ResourceNode::response_action(): query description %u copied to response description %u\n",
			//(*(query_to_handle.description.begin())).first, (*(response_ptr->description.begin())).first );
			If_ptr->scheduler_ptr->now(),id,(*(query_to_handle.description.begin())), (*(response_ptr->description.begin())) );
#endif


#ifdef DEBUG
	handler_schedule_event_ptr->event_id = If_ptr->event_id;
	If_ptr->event_id++;
	handler_schedule_event_ptr->node_ptr = this;
	//handler_schedule_event_ptr->message_to_handle_ptr = &query_to_handle;
	query_handler_ptr->parent_node_ptr = this;
#endif

		//Set up handler with submit response function
		functor_handler_ptr = new SpecificFunctor<QueryHandler>( query_handler_ptr,
									 &QueryHandler::transmit_response);
	
		handler_schedule_event_ptr->handler= functor_handler_ptr;
		handler_schedule_event_ptr->time = delay + If_ptr->scheduler_ptr->now();
		If_ptr->scheduler_ptr->schedule_event( handler_schedule_event_ptr); 


		return;
};




inline	int ResourceNode::verify_against_description(Query &submitted_query) {
	
	/*For now we define success as if atleast the query's description and node's 
	description	matches for one tag */
	//For each tag in the query, try to identify a matching tag in the node's description
	description_itr = submitted_query.description.begin();
	//while (description_itr != submitted_query.description.end())
	for(int fi= submitted_query.description.size();fi>0;fi--)
	{
		//Check if there is a single match

 		if ( description.find( (*description_itr) ) != description.end() )  return 1;

 		//if (  *( (relevance( *description_itr )).begin()) <= translation_radius ) return 1;

		description_itr++;
	}


	return 0;
};



inline	bool ResourceNode::can_accept_message(Message &submitted_message){
	
	//Check for TTL
	if (submitted_message.hop_count > submitted_message.time_to_live)
	{
		//Update the TTL counters in database, Report dropping due to TTL

#ifdef DEBUG_L2 

#ifdef SELECTIVE_PACKET_FILTER
		if ( submitted_message.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id   && 
				id == If_ptr->param_ptr->trace_destination_node_id &&
				(*(submitted_message.description.begin())) == If_ptr->param_ptr->trace_tag 	)
#endif

			printf(", but dropped as it has expired TTL, the hop count was %u, the TTL was %u\n",
					submitted_message.hop_count, submitted_message.time_to_live );
#endif

		switch (submitted_message.source_node_ptr->node_type)
			{
				case RESOURCE_NODE:
				case ROUTER_NODE:
					switch (submitted_message.type )
					{
						case QUERY:
							If_ptr->database_ptr->Num_queries_dropped_for_TTL++;
						break;
						case RESPONSE:
							If_ptr->database_ptr->Num_responses_dropped_for_TTL++;
					}
				break;
				case USER_NODE:
					//For external queries from user node
					switch (submitted_message.type )
					{
						case QUERY:
							If_ptr->database_ptr->Num_queries_dropped_for_TTL_for_UserNode++;
						break;
						case RESPONSE:
							If_ptr->database_ptr->Num_responses_dropped_for_TTL_for_UserNode++;
					}
		
			}//End of outer switch


		return 0;
	}

	return 1;
	
};



inline	bool RouterNode::can_accept_message(Message &submitted_message){
	

	//Check for TTL
	if (submitted_message.hop_count > submitted_message.time_to_live)
	{
		//Update the TTL counters in database

#ifdef DEBUG_L2 

#ifdef SELECTIVE_PACKET_FILTER
			if (submitted_message.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id &&
					(*(submitted_message.description.begin())) == If_ptr->param_ptr->trace_tag	)
#endif

			printf(", but dropped as it has expired TTL, the hop count was %u, the TTL was %u\n",
					submitted_message.hop_count, submitted_message.time_to_live );
#endif


		switch (submitted_message.source_node_ptr->node_type)
			{
				case RESOURCE_NODE:
				case ROUTER_NODE:
					switch (submitted_message.type )
					{
						case QUERY:
							If_ptr->database_ptr->Num_queries_dropped_for_TTL++;
						break;
						case RESPONSE:
							If_ptr->database_ptr->Num_responses_dropped_for_TTL++;
					}
				break;
				case USER_NODE:
					//For external queries from user node
					switch (submitted_message.type )
					{
						case QUERY:
							If_ptr->database_ptr->Num_queries_dropped_for_TTL_for_UserNode++;
						break;
						case RESPONSE:
							If_ptr->database_ptr->Num_responses_dropped_for_TTL_for_UserNode++;
					}
		
			}//End of outer switch


		return 0;
	}

	//Check for duplicate packets

#ifdef DUPPACK_SET
	query_key = make_pair( submitted_message.source_node_ptr->id, submitted_message.query_seq_num );
	if ( query_already_received_set.find( query_key ) 
				!= query_already_received_set.end() )
	{
		//Update the duplicate message counters in database

#ifdef DEBUG_L2 

#ifdef SELECTIVE_PACKET_FILTER
			if ( (submitted_message.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id ) &&
				(*(submitted_message.description.begin())) == If_ptr->param_ptr->trace_tag  )
#endif

			printf(", but dropped as it is duplicate, identified by DUPPACK_SET\n" );
#endif

			switch (submitted_message.source_node_ptr->node_type)
			{
				case RESOURCE_NODE:
				case ROUTER_NODE:
					If_ptr->database_ptr->Num_duplicate_message_dropped++;
					If_ptr->database_ptr->record_hops(submitted_message.hop_count,If_ptr->database_ptr->longevity_of_message_ctr_for_TTL ); //Suneil
				break;
				case USER_NODE:
					//For external queries from user node
					If_ptr->database_ptr->Num_duplicate_message_dropped_for_UserNode++;
					If_ptr->database_ptr->record_hops(submitted_message.hop_count,If_ptr->database_ptr->longevity_of_message_ctr_for_TTL_for_UserNode ); //Suneil
			}//End of switch

		return 0;
	}
#ifdef DEBUG_L2 
	else

#ifdef SELECTIVE_PACKET_FILTER
			if ( (submitted_message.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id ) &&
				(*(submitted_message.description.begin())) == If_ptr->param_ptr->trace_tag  )
#endif

			printf(", unique message\n" );
#endif
	query_already_received_set.insert( query_already_received_set.begin(), query_key );

#endif //End of DUPPACK_SET

#ifdef DUPPACK_CACHE

	//Check for duplicate packets
	/*message_key = Message_key( submitted_message.source_node_ptr->id, 
			submitted_message.query_seq_num,  If_ptr->god.now() + If_ptr->param_ptr->RouterNode_duplicate_message_cache_timeout );*/
	//message_key.id = submitted_message.source_node_id;
	//message_key.seq_num = submitted_message.query_seq_num;
	message_key.key.first = submitted_message.source_node_id;
	message_key.key.second = submitted_message.query_seq_num;

	message_key.time_to_expire = If_ptr->scheduler_ptr->now() + If_ptr->param_ptr->RouterNode_duplicate_message_cache_timeout;
		
	if ( message_key_cache.find( message_key ) != message_key_cache.end() )
	{
		//Update the duplicate message counters in database

#ifdef DEBUG_L2 

#ifdef SELECTIVE_PACKET_FILTER
		if (submitted_message.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id &&
				(*(submitted_message.description.begin())) == If_ptr->param_ptr->trace_tag	)
#endif

			printf(", but dropped as it is duplicate, identified by DUPPACK_CACHE\n" );
#endif

			switch (submitted_message.source_node_ptr->node_type)
			{
				case RESOURCE_NODE:
				case ROUTER_NODE:
					If_ptr->database_ptr->Num_duplicate_message_dropped++;
					If_ptr->database_ptr->record_hops(submitted_message.hop_count,If_ptr->database_ptr->longevity_of_message_ctr_for_TTL ); //Suneil

				break;
				case USER_NODE:
					//For external queries from user node
					If_ptr->database_ptr->Num_duplicate_message_dropped_for_UserNode++;
					If_ptr->database_ptr->record_hops(submitted_message.hop_count,If_ptr->database_ptr->longevity_of_message_ctr_for_TTL_for_UserNode ); //Suneil
		
			}//End of switch



		return 0;
	}
#ifdef DEBUG_L2 
	else
#ifdef SELECTIVE_PACKET_FILTER
		if (submitted_message.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id &&
				(*(submitted_message.description.begin())) == If_ptr->param_ptr->trace_tag	)
#endif

			printf(", unique message\n" );
#endif

	message_key_cache.insert( message_key_cache.begin(), message_key );
#endif //End of DUPPACK_CACHE


//added Suneil 9/28/07 
#ifdef DETECT_LOOP

	//Check for duplicate receipt in the node.
	//Duplicate receipt is searched for by traversing a list of visited nodes from the node_visited[].
	//node_visited has 63 entries and one length field. node_visited[0] is the length field.
	//Duplicate packets are dropped.

/*First check whether the current node is in the list - if list length is 0 then not in the list, if non 0 length then actually search
if present in the list, then mark it as duplicate packet and return 0 (that the packet cant be accepted)
if not present in the list then insert the node id at the position given by length field
*/
	if(submitted_message.node_visited[0])
	{

		//Actually search the list
		for (int i=1; i <= submitted_message.node_visited[0]; i++)
		{
			if (submitted_message.node_visited[i]== id)
			{
				//Dup packet !!
#ifdef DEBUG_L2 

#ifdef SELECTIVE_PACKET_FILTER
				if (submitted_message.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id &&
						(*(submitted_message.description.begin())) == If_ptr->param_ptr->trace_tag	)
#endif

				printf(", but dropped as it going in loops, identified by DETECT_LOOP\n" );
#endif

					switch (submitted_message.source_node_ptr->node_type)
					{
						case RESOURCE_NODE:
						case ROUTER_NODE:
							If_ptr->database_ptr->Num_looping_message_dropped++;
							If_ptr->database_ptr->record_hops(submitted_message.hop_count,If_ptr->database_ptr->longevity_of_message_ctr_for_loop ); //Suneil

						break;
						case USER_NODE:
							//For external queries from user node
							If_ptr->database_ptr->Num_looping_message_dropped_for_UserNode++;
							If_ptr->database_ptr->record_hops(submitted_message.hop_count,If_ptr->database_ptr->longevity_of_message_ctr_for_loop_for_UserNode ); //Suneil
				
					}//End of switch


				return 0;
			}
		}

	}

	submitted_message.node_visited[0]++;
	submitted_message.node_visited[submitted_message.node_visited[0]] = id;

#ifdef DEBUG_L2 
#ifdef SELECTIVE_PACKET_FILTER
	if (submitted_message.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id &&
			(*(submitted_message.description.begin())) == If_ptr->param_ptr->trace_tag	)
#endif
			printf(", non-looping message\n" );
#endif

#endif //close of DETECT_LOOP  	


	return 1;
	
};



RouterNode::RouterNode( unsigned int passed_message_time_to_live,

			NodeType passed_node_type_to_pass_on,
			int passed_known_routers_floor,
			int passed_known_routers_trim_threshold,
			int passed_known_routers_capacity,

			int	passed_translation_radius,

			int passed_routing_table_length_floor,
			int passed_routing_table_length_trim_threshold,
			int passed_routing_table_max_length,

			int passed_routing_table_width_floor,
			int passed_routing_table_width_threshold,
			int passed_routing_table_min_num_resource_nodes,
			int passed_routing_table_max_width,


			int passed_k, int passed_num_centroid_tags_to_explore_per_period,
			int passed_num_queries_to_submit_per_period,
			Description &passed_description, unsigned int passed_id, unsigned int passed_vitality,  int passed_extroversion,
			Tick passed_delay,
			Tick passed_response_timeout, BootstrapNodeList &passed_bootstrap_nodes,
			Infrastructure* passed_If_ptr ) : 
		ResourceNode( passed_message_time_to_live, 

			passed_node_type_to_pass_on,
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

			passed_k, passed_num_centroid_tags_to_explore_per_period,
			passed_num_queries_to_submit_per_period,
			passed_description, passed_id, passed_vitality, passed_extroversion, 
			passed_delay,
			passed_response_timeout, passed_bootstrap_nodes,
			passed_If_ptr ) 

			,routing_table_index(( routing_table_rows_greater_than(this_ptr) )) //For trimming  
		{ 
			routing_table_length_floor = passed_routing_table_length_floor;
			routing_table_length_trim_threshold = passed_routing_table_length_trim_threshold;
			routing_table_max_length = passed_routing_table_max_length;

			routing_table_width_floor = passed_routing_table_width_floor;
			routing_table_width_threshold = passed_routing_table_width_threshold;
			routing_table_min_num_resource_nodes = passed_routing_table_min_num_resource_nodes;
			routing_table_max_width = passed_routing_table_max_width;
	
			k= passed_k; /*routing_table_capacity = passed_routing_table_capacity; */ routing_table_size =0;
			
 			//Insert itself in the infrastructure holder std::map<unsigned int, RouterNode *> list_of_router_nodes; */
			//Remove itself from resource list, to avoid duplication
			If_ptr->list_of_resource_nodes.pop_back();
			If_ptr->list_of_router_nodes.push_back( this ); 

#ifdef DEBUG_NODE
		printf("Node %u inserted itself to the router list\n", id);
#endif
#ifdef DEBUG_L1
		printf("Ticks:%u : Node %u: This created node %u is a router node\n",If_ptr->scheduler_ptr->now(),id, id);
#endif
#ifdef DEBUG_L2
		print_routing_table();
#endif
			num_centroid_tags_to_explore_per_period = passed_num_centroid_tags_to_explore_per_period;
			semantic_space_inner_radius = 0;//To ensure that explorations starts with focus vertices
			description_budget_itr = description.begin();

			descriptions_to_explore = passed_description; // This will ensure that temp_descriptions_to_explore does not get loaded with description items
			
			all_distances_ptr = &(If_ptr->onto_repository_ptr->all_distances);
			num_vertices_at_onto_ptr = &(If_ptr->onto_repository_ptr->num_vertices) ;
			return;
		};




inline void RouterNode::submit_query(Query &submitted_query)
{

#ifdef DEBUG_L2 

#ifdef SELECTIVE_PACKET_FILTER
		if ( submitted_query.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id  
				&& (*(submitted_query.description.begin())) == If_ptr->param_ptr->trace_tag )
#endif

		{

			/*

			printf("Ticks:%u ",If_ptr->scheduler_ptr->now());
			printf(": Query with query_seq_num %u Tag %u, ",submitted_query.query_seq_num, (*(submitted_query.description.begin())) );
			printf(" received by Router Node id %u, type %d with source node %u, type %d\n", 
				id, node_type, submitted_query.source_node_id, submitted_query.source_node_ptr->node_type );
			*/

			printf("Ticks:%u : Node %u: Query with query_seq_num %u Tag %u,  received by Router Node id %u, type %d with source node %u, type %d, tag %u\n", 
				//If_ptr->scheduler_ptr->now(),submitted_query.query_seq_num, (*(submitted_query.description.begin())).first ,
				If_ptr->scheduler_ptr->now(),id,submitted_query.query_seq_num, (*(submitted_query.description.begin())) , 
			id, node_type, submitted_query.source_node_id, submitted_query.source_node_ptr->node_type,(*(submitted_query.description.begin())) );
		//printf("RouterNode::submit_query(): Query content: node id %u, seq num %u \n", submitted_query.source_node_id, submitted_query.query_seq_num);
		}
#endif

/*
	if (submitted_query.source_node_id != id) submitted_query.hop_count++; //Why is this like this ?? See comments in similar situation in resource node
#ifdef DEBUG_NODE 
	else cout <<"This router node has submitted a query to itself\n"<< endl;
#endif
*/

	if (submitted_query.source_node_id != id) 
			submitted_query.hop_count++;
#ifdef DEBUG_NODE 
		else std::cout <<"This router node has submitted a query to itself\n"<< endl;
#endif

	//Checks for TTL
	if (can_accept_message(submitted_query))
	{

	//Match against description	
	//description_matched = verify_against_description(Query &submitted_query);

	/*
	If matched, then allocate a handler with response action
	if not matched then setup handler with multicast action
	*/
			//Ensure that the query submitted to itself (router node) is not matched with its own description
			if ( ( submitted_query.source_node_id != id) && verify_against_description(submitted_query) )
			{

#ifdef DEBUG_L2
	
#ifdef SELECTIVE_PACKET_FILTER
			if ( submitted_query.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id  &&
					(*(submitted_query.description.begin())) == If_ptr->param_ptr->trace_tag	)
#endif

				//printf("Match found for Tag %u, so will be sending a response\n", (*(submitted_query.description.begin())).first );
				printf("Ticks:%u : Node %u: Match found for Tag %u, so will be sending a response back to query orginator node %u, type %u\n", 
						If_ptr->scheduler_ptr->now(),id,(*(submitted_query.description.begin())),
						submitted_query.source_node_ptr->id, submitted_query.source_node_ptr->node_type);
#endif

				response_action(submitted_query);



			} 

			selective_multicast_action(submitted_query);


	}

		//Now drop it anyway
		//sm24
		//If_ptr->list_of_queries.erase(submitted_query.id);
		delete &submitted_query;


		//If_ptr->list_of_messages.erase(&submitted_query);

		//Also report this to the database that query has been deleted (for matching of memory leakage audit purpose
		
		return;
}

//This method decides the routing of packets
inline void RouterNode::selective_multicast_action(Query &query_to_handle) {

		description_itr = query_to_handle.description.begin();
//printf("Reached here 1\n"); 
		//while ( description_itr != query_to_handle.description.end())
		for(int fi=query_to_handle.description.size();fi>0;fi--)
		{
//printf("Reached here 2\n"); 
			//nearest_k = If_ptr->onto_repository_ptr->nearest_neighbors( (*description_itr),k, knowledge  );
			//If_ptr->onto_repository_ptr->generate_nearest_neighbors( (*description_itr),k, knowledge, nearest_k  );
			generate_nearest_neighbors( (*description_itr)/*,k, knowledge, nearest_k */  );
//printf("Reached here 3\n"); 
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if (  query_to_handle.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id  &&
			(*description_itr) == If_ptr->param_ptr->trace_tag	)
#endif
		{
			printf("Ticks:%u : Node %u: For query_seq num %u, source node %u, Tag %u matched to top k =%u routing table Tags:",
				If_ptr->scheduler_ptr->now(), id,
				query_to_handle.query_seq_num, query_to_handle.source_node_ptr->id , (*description_itr), k	);

			print_nearest_k();

			printf("Ticks:%u : Node %u: For query_seq num %u, source node %u, Tag %u matched to top k =%u routing table tag-node pair entries : ", 
			If_ptr->scheduler_ptr->now(), id,
			query_to_handle.query_seq_num, query_to_handle.source_node_ptr->id , (*description_itr), k	);
	
		}
#endif

			nearest_k_itr = nearest_k.begin();
 			//for (int i=nearest_k.size(); nearest_k_itr != nearest_k.end() ; /*i++,*/ nearest_k_itr++)
			/*for(num_elements = nearest_k.size(),nearest_k_itr = nearest_k.begin();
				num_elements >0 ; num_elements--, nearest_k_itr++)*/

			for (int i = nearest_k.size(); i >0 ; i--,nearest_k_itr++ )
			{
 				routing_table_entry_itr = (routing_table[*nearest_k_itr])->destinations.begin();
 				//while (routing_table_entry_itr != (routing_table[*nearest_k_itr]).end())
				for(int fi= (routing_table[*nearest_k_itr])->destinations.size();fi>0;fi--)
				{
 					/*
					This if block implements the filter, if the query is meant only for a particular class of node, 
					it goes only to the nodes of that class.
					Simiilarly the filter ensures that any packet from a node is not forwarded back to that node
					*/

					/*
					if ( ( (*routing_table_entry_itr)->node_type == query_to_handle.only_to_node_type) &&
							( (*routing_table_entry_itr)->id != query_to_handle.source_node_id) )
					*/
					if ( routing_filter( (*routing_table_entry_itr), query_to_handle) )
					{

 						query_ptr = new Query();
						//query_ptr = new Query(If_ptr->query_id);
						//sm24

						//If_ptr->list_of_queries.insert(make_pair(If_ptr->query_id,query_ptr));
						//If_ptr->query_id++;
						//If_ptr->database_ptr->Num_total_queries_generated++;
						//Suneil: SM: ADD PROBE -- query generation -- update total messages generated.
						switch (query_to_handle.source_node_ptr->node_type)
							{
								case RESOURCE_NODE:
									If_ptr->database_ptr->Queries_forwarded_by_RouterNode_orgininated_from_ResourceNode++;
									If_ptr->database_ptr->Num_total_message_generated++;

								case ROUTER_NODE:
									If_ptr->database_ptr->Queries_forwarded_by_RouterNode_orgininated_from_RouterNode++;
									If_ptr->database_ptr->Num_total_message_generated++;

								break;
								case USER_NODE:
									//For external queries from user node
									If_ptr->database_ptr->Num_total_message_generated_for_UserNode++;
							}

						/*Make a deep copy of the received query, to multicast it all the nodes 
						in the row of the routing table */
						*query_ptr = query_to_handle;  

						query_ptr->destination_node_ptr = *routing_table_entry_itr;
						query_ptr->destination_node_id = (*routing_table_entry_itr)->id;


#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if (  query_to_handle.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id  &&
			(*description_itr) == If_ptr->param_ptr->trace_tag	)
#endif
		printf(" tag %u:next hop node %u, type %u,", *nearest_k_itr, (*routing_table_entry_itr)->id, (*routing_table_entry_itr)->node_type );

#endif


						//Seems 
						query_handler_ptr = new QueryHandler();
						query_handler_ptr->query_to_send_ptr = query_ptr;

						//Schedule the wake up time for this QueryHandler;
						handler_schedule_event_ptr = new Event;

#ifdef DEBUG
	handler_schedule_event_ptr->event_id = If_ptr->event_id;
	If_ptr->event_id++;
	handler_schedule_event_ptr->node_ptr = this;
	//handler_schedule_event_ptr->message_to_handle_ptr = &query_to_handle;
	query_handler_ptr->parent_node_ptr = this;
#endif

						//Setup handler with multicast action
						functor_handler_ptr = new SpecificFunctor<QueryHandler>(  query_handler_ptr,
												 &QueryHandler::transmit_query);

						handler_schedule_event_ptr->handler =  functor_handler_ptr;
						handler_schedule_event_ptr->time = delay + If_ptr->scheduler_ptr->now();
						If_ptr->scheduler_ptr->schedule_event( handler_schedule_event_ptr); 
 					}


					routing_table_entry_itr++;


				}
			}




#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if (  query_to_handle.source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id  &&
			(*description_itr) == If_ptr->param_ptr->trace_tag	)
#endif

		printf("\n");
#endif


			description_itr++;
		}// Loop on description

		return;

};


inline bool RouterNode::routing_filter( ResourceNode *resource_node_entry_in_routing_table_ptr, Query &query_to_route )
{
	//	This if block implements the filter, if the query is meant only for a particular class of node, 
	//it goes only to the nodes of that class.
	//Simiilarly the filter ensures that any packet from a node is not forwarded back to that node
	/*
	switch (query_to_route.only_to_node_type)
	{
		case TO_RES_NODE :

			break;
		case  TO_RTR_NODE :

			break;
		case  TO_RES_RTR_NODE :

		break;
		default :
			return 0;
	}
	*/
	/*This ensures that the a query is forwarded to the node types which is mentioned int the message filter (see Message.hpp) 
	is not forwarded to its source ever */

	return (resource_node_entry_in_routing_table_ptr->node_type & query_to_route.only_to_node_type) &&
			( resource_node_entry_in_routing_table_ptr->id != query_to_route.source_node_id); 
	
};


inline int RouterNode::attach_request(ResourceNode* passed_node_ptr) {    

//printf("Before attaching RT is - \n"); print_routing_table();

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( passed_node_ptr->id == If_ptr->param_ptr->trace_source_node_id && 
				id == If_ptr->param_ptr->trace_destination_node_id  )
#endif

			printf("Ticks:%u : Node %u : Attach request received from node %u, type %d", 
				If_ptr->scheduler_ptr->now(),id, passed_node_ptr->id ,passed_node_ptr->node_type);
#endif

 	if ( can_accept(passed_node_ptr) )
	{
 
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if (  passed_node_ptr->id == If_ptr->param_ptr->trace_source_node_id && 
				id == If_ptr->param_ptr->trace_destination_node_id  )
#endif


	printf(" for Tags : ", 
				id, passed_node_ptr->id ,passed_node_ptr->node_type);
#endif


	//	if ( routing_table_size >= routing_table_capacity ) free_space_in_routing_table();
	
		//Get the passed node's description's tags, for each tag, take action
		description_itr = passed_node_ptr->description.begin();
		//while (description_itr != passed_node_ptr->description.end())
		for(int fi=passed_node_ptr->description.size() ;fi>0;fi--)
		{
 		
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( passed_node_ptr->id == If_ptr->param_ptr->trace_source_node_id && 
				id == If_ptr->param_ptr->trace_destination_node_id  )
#endif

	//printf("%u ", (*description_itr).first);
	printf("%u ", (*description_itr));
#endif
			
			//Check if there is a single match
			//if ( description.find( (*description_itr).first ) == description.end() ) //Why are we matching against description, should match against routing table
			//if ( routing_table.find( (*description_itr).first ) == routing_table.end() )
			if ( routing_table.find( (*description_itr) ) == routing_table.end() )
			{
 				//No match , so new row in the routing table is needed
				
 
				//But check for space for a new row/rows
				//RT length trimming
				if (routing_table.size() >= routing_table_max_length ) trim_routing_table_length();

				//Insert in knowledge repository and in new routing table row
				//knowledge.push_back((*description_itr).first);
				knowledge.push_back((*description_itr));
 
				//Also push the tags in its own description, so that the routing table entries also becomes description
				//descriptions_to_explore.insert((*description_itr));
				if ( (descriptions_to_explore.insert( *description_itr )).second )
						temp_descriptions_to_explore.insert( *description_itr ) ; //This is added to adapt it to the incremental exploration
 
				//Create a routing table row entry
				//routing_table_row_obj_ptr = new list<ResourceNode*>;
				//routing_table_row_obj.clear();

				routing_table_row_obj_ptr = new RoutingTableRow(this_ptr);
				(routing_table_row_obj_ptr->destinations).insert(passed_node_ptr);
 				//Increment the resource counter
				if (passed_node_ptr->node_type == RESOURCE_NODE) routing_table_row_obj_ptr->num_resource_nodes++;

				//routing_table_row_obj.push_back( passed_node_ptr );

				routing_table.insert( make_pair( (*description_itr), routing_table_row_obj_ptr) );
 				
 				//Need to insert the new tag in the routing_table_index also
				routing_table_index.insert(*description_itr);
 
 
			} 
			else
			{
 				
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( passed_node_ptr->id == If_ptr->param_ptr->trace_source_node_id && 
				id == If_ptr->param_ptr->trace_destination_node_id  )
#endif

		printf("(matched) ");
#endif
 				
 				routing_table_row_obj_ptr =  (*(routing_table).find(*description_itr) ).second;
 
				//Specific Width trimming
 				if (routing_table_row_obj_ptr->destinations.size() >= routing_table_max_width) 
 						trim_routing_table_specific_width(routing_table_row_obj_ptr); 
 
 				//Tag already exists, so just enter in the suitable row, only if the node is not already present, if not present then increment the res node counter
 				if( (routing_table_row_obj_ptr->destinations).insert(passed_node_ptr).second && (passed_node_ptr->node_type == RESOURCE_NODE) ) 
					//If true Increment the resource node counter
						routing_table_row_obj_ptr->num_resource_nodes++;
 				

			}


#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if (  passed_node_ptr->id == If_ptr->param_ptr->trace_source_node_id && 
				id == If_ptr->param_ptr->trace_destination_node_id  )
#endif
	
	printf(", ");
#endif

			description_itr++;
		}		

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( passed_node_ptr->id == If_ptr->param_ptr->trace_source_node_id && 
				id == If_ptr->param_ptr->trace_destination_node_id  )
#endif

		printf(" : suceeded");
#endif
		//printf("After attaching RT is - \n"); print_routing_table();

		//routing_table_capacity++;
		return 1;
	} 
	else 
	{

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( passed_node_ptr->id == If_ptr->param_ptr->trace_source_node_id && 
				id == If_ptr->param_ptr->trace_destination_node_id )
#endif

		printf(" : failed");
#endif
		//printf("After attaching RT is - \n"); print_routing_table();

		return 0;
	}
}



inline void RouterNode::periodic_housekeeping() {
	
	//RT length trimming
	if (routing_table.size() >= routing_table_length_trim_threshold ) trim_routing_table_length();

	//Cleanup the duplicate_message_cache

#ifdef DUPPACK_CACHE

#ifdef DEBUG_L2
#ifdef SELECTIVE_PACKET_FILTER
		if (id == If_ptr->param_ptr->trace_source_node_id  )
#endif
		printf("Ticks %u: Node %u: RouterNode::periodic_housekeeping(): Cleaning up cache....\n",
					If_ptr->scheduler_ptr->now(), id );
#endif

	message_key_cache_itr = message_key_cache.begin();
	//message_key_to_remove_itr = message_key_cache_itr;
	Tick curr_time = If_ptr->scheduler_ptr->now();

	//while ( message_key_cache_itr != message_key_cache.end() )
	for(int fi = message_key_cache.size();fi>0;fi--)
	{
		//message_key_cache_itr++;
		if ( (*message_key_cache_itr).time_to_expire <= curr_time )
		{
			//message_key_cache.erase(message_key_to_remove_itr);	
			//message_key_to_remove_itr = message_key_cache.begin();
			message_key_cache.erase(message_key_cache_itr++);	
			fi--;
		}
		else {

			++message_key_cache_itr;
		}

	}
#endif
	return;
};

inline
void ResourceNode::handle_recvd_response(Response *curr_response_ptr) {
	
			/* Check whether the response is from router or resource node. 
			There should not be any response from any resource node - why ??? with such protocol the routing table can not be populated !!
			because only the resource seeks to attach, not router !! Ensure that the old protocol isnt working- now it is ok
			because in the query update bag, the queries now have the filter fixed to bboth res and rtr nodes, earlier it was only to rtr nodes
			*/
			/*
			if (curr_response_ptr->source_node_ptr->node_type != ROUTER_NODE)
			{
				std::cout<<"Error: The response to seek_router_node() query is not from a router node, troubleshoot" <<endl;

			}
			else */
			//{
			/*Resource node should attach only if the response is from router node,
			if from another resource node then it should do nothing */
			if (curr_response_ptr->source_node_ptr->node_type == ROUTER_NODE)
			{
	
/*
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
		if (	id == If_ptr->param_ptr->trace_source_node_id && 
				curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id &&
					(*(curr_response_ptr->description.begin())) == If_ptr->param_ptr->trace_tag 	)
#endif

		printf("Ticks:%u : Node %u: Processing response received from node %u, type %d, tag %u, query_seq_num %u \n",
			If_ptr->scheduler_ptr->now(),id, curr_response_ptr->source_node_ptr->id, curr_response_ptr->source_node_ptr->node_type, 
			//(*(curr_response_ptr->description.begin())).first, curr_response_ptr->query_seq_num);
			(*(curr_response_ptr->description.begin())), curr_response_ptr->query_seq_num);
#endif
*/

				//No need to check if USER_NODE, becoz this virtual methhod will only kick in if RESOURCE_NODE
						//Report num of response
						//Suneil
							If_ptr->database_ptr->Num_of_good_responses++; //This ok for time being as we consider all reponses received as good response
							If_ptr->database_ptr->Num_of_good_responses_for_specific_tag[*(curr_response_ptr->description.begin())]++;
						//SM:ADD PROBE- update good-response counter.

					
					
					/* 
					Update resource nodes addressbook - known_routers
					check if the current response is from a router node that it has already attached to in past.
					if that router node is not in the attached to list then attach to it, else ignore that response
					Also check that for an entry which is itself there should not be any action taken-> this is 
					not a problem as we ensure that a router/resource node does not respond to itself
					*/
					//if (  ( known_routers.find( (responses.front()).source_node_id ) == 
					if (  ( known_routers.find( curr_response_ptr->source_node_id ) == 
							known_routers.end() ) 	)
					{
						//New router, so send attach request
#ifdef DEBUG_LISTENER
							printf("Listener::handle_recvd_responses() : ((responses.front()).source_node_ptr)->id is %u, type is %d \n",
								//((responses.front()).source_node_ptr)->id, ((responses.front()).source_node_ptr)->node_type	);
								curr_response_ptr->source_node_ptr->id, curr_response_ptr->source_node_ptr->node_type	);
#endif
							
							//Dynamic ptr casting to derived class
							//router_node_ptr = dynamic_cast<RouterNode *>(curr_response_ptr->source_node_ptr);

							
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
		if (id == If_ptr->param_ptr->trace_source_node_id && 
				curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id &&
				(*(curr_response_ptr->description.begin())) == If_ptr->param_ptr->trace_tag	)
#endif

		printf("Ticks:%u : Node %u: Sending attach request to node %u, type %u for tag %u query_seq_num %u\n",
			If_ptr->scheduler_ptr->now(),id,curr_response_ptr->source_node_ptr->id, curr_response_ptr->source_node_ptr->node_type,  
			//(*(curr_response_ptr->description.begin())).first, curr_response_ptr->query_seq_num,
			(*(curr_response_ptr->description.begin())), curr_response_ptr->query_seq_num );
#endif

							//router_node_ptr->attach_request(this);
							curr_response_ptr->source_node_ptr->attach_request(this); 

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( id == If_ptr->param_ptr->trace_source_node_id && 
				curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id  )
#endif

		printf(" for Tag %u\n ",(*(curr_response_ptr->description.begin())) );//This is to ensure that attache request is related to a tag
#endif
							/*
							known_routers.insert( 
								make_pair( curr_response_ptr->source_node_id , 
								dynamic_cast< RouterNode* >( curr_response_ptr->source_node_ptr ) )			
							);
							*/

							curr_router_ptr = dynamic_cast< RouterNode* >( curr_response_ptr->source_node_ptr );

							known_routers.insert( make_pair( curr_response_ptr->source_node_id , curr_router_ptr)	);

							//This is a new router, so insert it in the known_routers_index
							(*known_routers_index_ptr).insert(curr_router_ptr);

					} 
#ifdef DEBUG_L2

					else 

#ifdef SELECTIVE_PACKET_FILTER
						if (id == If_ptr->param_ptr->trace_source_node_id && 
								curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id &&
								(*(curr_response_ptr->description.begin())) == If_ptr->param_ptr->trace_tag 	)
#endif


						printf("Ticks:%u : Node %u: Seems this router node %u is already known to %u and was attached to %u earlier, so not sending attach request now\n", 
							If_ptr->scheduler_ptr->now(),id, curr_response_ptr->source_node_ptr->id, id, curr_response_ptr->source_node_ptr->id);
#endif


			}
				
			/*If this node is also a router node then, insert the sender in it routing table, 
			and known resource node list, description etc. All these are in the method */
			if (node_type == ROUTER_NODE) update_routing_table_action(curr_response_ptr);

	return;	
};



inline void RouterNode::update_routing_table_action(Response *curr_response_ptr) {
	
	switch (curr_response_ptr->source_node_ptr->node_type)
	{
		case RES_NODE:
				/* 
				check if the current response is from a resource node that it has already attached to its own routing table in past.
				if that resource node is not in the attached to list then attach to it, else ignore that response
				*/
				if (  ( resources_attached.find( curr_response_ptr->source_node_id ) == 
						resources_attached.end() ) 	)
				{

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
					if ( (id == If_ptr->param_ptr->trace_source_node_id || id == If_ptr->param_ptr->trace_destination_node_id )  && 
							(curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id ||
								curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id	)&&
								(*(curr_response_ptr->description.begin())) == If_ptr->param_ptr->trace_tag )
#endif

					printf("Ticks:%u : Node %u : Inserting this resource node %u, type %u for tag %u query_seq_num %u, to its own (node %u, type %d) routing table\n",
						If_ptr->scheduler_ptr->now(),id, curr_response_ptr->source_node_ptr->id, curr_response_ptr->source_node_ptr->node_type,  
						//(*(curr_response_ptr->description.begin())).first, curr_response_ptr->query_seq_num,
						(*(curr_response_ptr->description.begin())), curr_response_ptr->query_seq_num,
						id, node_type);
#endif
					//New resource node, so send attach request to itself
					attach_request(curr_response_ptr->source_node_ptr);


#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( (id == If_ptr->param_ptr->trace_source_node_id || id == If_ptr->param_ptr->trace_destination_node_id )  && 
							(curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id ||
								curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id	)&&
								(*(curr_response_ptr->description.begin())) == If_ptr->param_ptr->trace_tag )
#endif

		printf(" for Tag %u\n",(*(curr_response_ptr->description.begin())) );//This is to ensure that attache request is related to a tag
#endif



					//Insert into the known resource list
					resources_attached.insert( 
						make_pair( curr_response_ptr->source_node_id , curr_response_ptr->source_node_ptr  )			
										);
					
				}
#ifdef DEBUG_L2

					else 

#ifdef SELECTIVE_PACKET_FILTER
						if (id == If_ptr->param_ptr->trace_source_node_id && 
								curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id &&
								(*(curr_response_ptr->description.begin())) == If_ptr->param_ptr->trace_tag)
#endif

						printf("Ticks:%u : Node %u : Seems this resource node %u is already known to this node %u and was inserted in the routing table earlier, so not inserting again\n", 
							If_ptr->scheduler_ptr->now(),id ,curr_response_ptr->source_node_ptr->id, id);
#endif
		break;
		case RTR_NODE:
					/* 
					check if the current response is from a router node that it has already attached to its own routing table in past.
					if that router node is not in the attached to list then attach to it, else ignore that response
					Also check that for an entry which is itself there should not be any action taken-> this is 
					not a problem as we ensure that a router/resource node does not respond to itself
					*/
					//if (  ( known_routers.find( (responses.front()).source_node_id ) == 
					if (  ( routers_attached.find( curr_response_ptr->source_node_id ) == 
							routers_attached.end() ) 	)
					{
						//New router, so send attach request
						
							
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
					if ( (id == If_ptr->param_ptr->trace_source_node_id || id == If_ptr->param_ptr->trace_destination_node_id )  && 
							(curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id ||
								curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id	)&&
								(*(curr_response_ptr->description.begin())) == If_ptr->param_ptr->trace_tag )
#endif

						printf("Ticks:%u : Node %u : Inserting this router node %u, type %u for tag %u query_seq_num %u, to its own (node %u, type %d) routing table\n",
							If_ptr->scheduler_ptr->now(),id ,curr_response_ptr->source_node_ptr->id, curr_response_ptr->source_node_ptr->node_type,  
							//(*(curr_response_ptr->description.begin())).first, curr_response_ptr->query_seq_num,
							(*(curr_response_ptr->description.begin())), curr_response_ptr->query_seq_num,
							id, node_type);
#endif

							
							attach_request(curr_response_ptr->source_node_ptr); 

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
	if ( (id == If_ptr->param_ptr->trace_source_node_id || id == If_ptr->param_ptr->trace_destination_node_id )  && 
							(curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id ||
								curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_source_node_id	)&&
								(*(curr_response_ptr->description.begin())) == If_ptr->param_ptr->trace_tag )
#endif

		printf(" for Tag %u\n",(*(curr_response_ptr->description.begin())) );//This is to ensure that attache request is related to a tag
#endif


							routers_attached.insert( 
								make_pair( curr_response_ptr->source_node_id , 
								dynamic_cast< RouterNode* >( curr_response_ptr->source_node_ptr ) )			
							);
							

					} 
#ifdef DEBUG_L2

					else {

#ifdef SELECTIVE_PACKET_FILTER
						if (id == If_ptr->param_ptr->trace_source_node_id && 
							 curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id &&
								(*(curr_response_ptr->description.begin())) == If_ptr->param_ptr->trace_tag)
#endif

						printf("Ticks:%u : Node %u : Seems this router node %u is already known to this node %u and was inserted in the routing table earlier, so not inserting again\n",
							If_ptr->scheduler_ptr->now(),id , curr_response_ptr->query_seq_num,curr_response_ptr->source_node_ptr->id, id, id);
					}
#endif
	}
	
	
	
	return;	

};


//Need to fix these two later
//inline void RouterNode::free_space_in_routing_table() { return;};
inline int RouterNode::can_accept(Node* passed_node_ptr) {return 1;};




inline void RouterNode::generate_nearest_neighbors(Tag focus_vertice /*, int k, const std::vector<Tag> &neighbors, std::list<Tag> &nearest_k*/) {

		nearest_k.clear(); //The list object is made a permenant resource for reuse
 		//unsigned int current_list_size=0;

		//for (unsigned int num_vertices_i=0;num_vertices_i < neighbors.size() ; num_vertices_i++)
		for (unsigned int num_vertices_io=0,current_list_size=0,fko = knowledge.size(); fko>0 ; num_vertices_io++,fko--)
		{
			nearest_k_itr = nearest_k.begin();
			if (current_list_size < k) //We havent recorded the nearest k yet, so fill it up
			{
				
				//Insert the node in the return list, at proper location
				//Find location to insert
				for( int i = nearest_k.size();
						i>0  && (*all_distances_ptr)[focus_vertice][knowledge[num_vertices_io]] < (*all_distances_ptr)[focus_vertice][*nearest_k_itr];
						i--	) nearest_k_itr++;

				nearest_k.insert(nearest_k_itr,knowledge[num_vertices_io]);
				current_list_size++;

			} else { //nearest k neighbors have been recorded, now lets adjust the list
				/*First check whether the current neghbor will be within nearest k, if yes then
				Remove the biggest element in the list to create a space for a nearer neighbor */

				if ( (*all_distances_ptr)[focus_vertice][knowledge[num_vertices_io]] < (*all_distances_ptr)[focus_vertice][*nearest_k_itr])
				{
					nearest_k.pop_front();

					//Insert nearer neighbor
					//Find location to insert
					nearest_k_itr = nearest_k.begin();
					for( int i = nearest_k.size();
						i>0  && (*all_distances_ptr)[focus_vertice][knowledge[num_vertices_io]] < (*all_distances_ptr)[focus_vertice][*nearest_k_itr];
						i--	) nearest_k_itr++;

					nearest_k.insert(nearest_k_itr,knowledge[num_vertices_io]);

				} //end of if ( all_distances[focus_vertice][neighbors[i]] < *nearest_k_itr)

			}//end of if (current_list_size < k)
		

		}//end of for loop

		return;
}; 

inline int RouterNode::distance(Tag vertice1, Tag vertice2) {
	
	return 	(*all_distances_ptr)[vertice1][vertice2];
	
};

inline void RouterNode::generate_neighbors_to_explore(Tag focus_vertice, int radius /*, std::list<Tag> &nearest_k*/) {
	
		nearest_k.clear();
		for (unsigned int num_vertices_io=0; num_vertices_io < *num_vertices_at_onto_ptr ; num_vertices_io++)
		{
			if ( (*all_distances_ptr)[focus_vertice][num_vertices_io] <= radius)
				nearest_k.push_back( num_vertices_io );
		}
		
		return ;
}; 


//Code added by Suneil for the exploration around the known areas.
// added 10/18/07

inline void RouterNode::generate_new_neighbors_to_explore(Tag focus_vertice,  int inner_radius, int radius_increment /*, std::list<Tag> &nearest_k*/) {
 
		nearest_k.clear();

		int outer_radius = inner_radius + radius_increment ;

		for (unsigned int num_vertices_io=0; num_vertices_io < *num_vertices_at_onto_ptr ; num_vertices_io++)
		{
			//we are trying to find those nodes that are outside the current radius, with the outer circle being the hop radius 
			// plus the radius_increment. But since we don't want to rediscover the already known nodes, we ask for those
			// nodes that are outside the inner_radius but inside the circle of inner_radius + radius_increment.
			// Circular strip of Inside radius = inner_radius, outer radius = inner_radius + radius_increment.
			if ( ((*all_distances_ptr)[focus_vertice][num_vertices_io] > inner_radius) && ((*all_distances_ptr)[focus_vertice][num_vertices_io] <= outer_radius) )
 					nearest_k.push_back( num_vertices_io );
 
		}
		
		return ;
}; 








UserNode::UserNode(unsigned int passed_message_time_to_live, 
			
			NodeType passed_node_type_to_pass_on,

			int passed_known_routers_floor,
			int passed_known_routers_trim_threshold,
			int passed_known_routers_capacity,

			int	passed_translation_radius,

			int passed_routing_table_length_floor,
			int passed_routing_table_length_trim_threshold,
			int passed_routing_table_max_length,

			int passed_routing_table_width_floor,
			int passed_routing_table_width_threshold,
			int passed_routing_table_min_num_resource_nodes,
			int passed_routing_table_max_width,

			int passed_k,  int passed_num_centroid_tags_to_explore_per_period,
			 int passed_num_queries_to_submit_per_period,
			Description &passed_description, unsigned int passed_id, unsigned int passed_vitality,  int passed_extroversion,
			Tick passed_delay,
			Tick passed_response_timeout, BootstrapNodeList &passed_bootstrap_nodes,
			Infrastructure* passed_If_ptr ) : 
		ResourceNode(passed_message_time_to_live, 
		
			passed_node_type_to_pass_on,

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


			passed_k, passed_num_centroid_tags_to_explore_per_period,
			passed_num_queries_to_submit_per_period,
			passed_description, passed_id, passed_vitality, passed_extroversion, 
			passed_delay,
			passed_response_timeout, passed_bootstrap_nodes,
			passed_If_ptr )
		{
 			//Get the user noode out of the resource node list
			If_ptr->list_of_resource_nodes.pop_back();
			If_ptr->list_of_user_nodes.push_back(this);

		};



void UserNode::heartbeat(){

#ifdef DEBUG
		printf("Ticks:%u : Node %u, node type : %d: UserNode::heartbeat()......  \n", If_ptr->scheduler_ptr->now(),id, node_type);
#endif
	

	/*
	Choose k random tags, load them in the description (clear description before that), 
	update query bag, generate random bootstrap nodes, and send the queries to those nodes
	*/


	generate_random_description(num_queries_to_submit_per_period);

	//Flush the query_bag_for_advertising, to get started with new queries
	query_bag_for_advertising.clear();

	update_query_bag(description, query_bag_for_advertising );
	generate_random_bootstrap_nodes(extroversion);
	query_itr = query_bag_for_advertising.begin();
	send_queries_from( query_bag_for_advertising, query_itr, query_bag_for_advertising.size() );
	
};

inline
void UserNode::handle_recvd_response(Response *curr_response_ptr) {


	//Check for errors, or malformed responses
	if (curr_response_ptr->source_node_ptr->node_type != RESOURCE_NODE || curr_response_ptr->destination_node_id != id )
		std::cout<<"Error: UserNode::handle_recvd_response(): malformed response received at UserNode"<<std::endl;
	else
	{
		
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER
			if (( id == If_ptr->param_ptr->trace_source_node_id  && 
				curr_response_ptr->source_node_ptr->id == If_ptr->param_ptr->trace_destination_node_id &&
				(*(curr_response_ptr->description.begin())) == If_ptr->param_ptr->trace_tag	) 

#ifdef ALL_USERNODE_PACKETS
					|| ( node_type == USER_NODE ) 
#endif
					)

#endif
		printf("Ticks:%u : Node %u: Noted successful recall from node %u, type %u for tag %u query_seq_num %u\n",
			If_ptr->scheduler_ptr->now(),id,curr_response_ptr->source_node_ptr->id, curr_response_ptr->source_node_ptr->node_type,  
			(*(curr_response_ptr->description.begin())), curr_response_ptr->query_seq_num );
#endif

		
		//Report good responses
		If_ptr->database_ptr->Num_of_good_responses_for_UserNode++;
		If_ptr->database_ptr->Num_of_good_responses_for_specific_tag_for_UserNode[*(curr_response_ptr->description.begin())]++;
	}
};




void inline UserNode::generate_random_description(unsigned int num_description) {
		int i=0;
#ifdef DEBUG
		printf("Ticks:%u : Node %u: User Queries submitted to network for tags :",If_ptr->scheduler_ptr->now(),id  );
#endif
		description.clear();

#ifdef DEBUG_L2
#ifdef SELECTIVE_PACKET_FILTER
		if (If_ptr->param_ptr->trace_source_node_id == 4294967292)
			{
				description.insert( If_ptr->param_ptr->trace_tag  );
				printf(" %u,", If_ptr->param_ptr->trace_tag);
				i++;
			}
#endif
#endif

		//Fill the description with random tags
		//for (int i=0;i<num_description ; i++ )
		for (;i<num_description ; ) //This is to ensure that the UserNode sends atleast the query with the designated tracer tag
		{
			//generated_random_tag =((rand() % If_ptr->onto_repository_ptr->num_vertices)+1);
			generated_random_tag = (rand() % If_ptr->onto_repository_ptr->num_vertices);

			//generated_random_description.insert( make_pair( generated_random_tag, generated_random_tag) );
			if (description.find(generated_random_tag) == description.end())
			{
				description.insert( generated_random_tag  );
				i++;
#ifdef DEBUG
				printf(" %u,", generated_random_tag);
#endif
			}
		}

#ifdef DEBUG
		printf("\n");
#endif
		return;
	};


void inline UserNode::generate_random_bootstrap_nodes(unsigned int num_bootstrap_nodes) {

			
			//Flush the bootstrap list from the old cycle
			known_routers.clear();
			index_of_nodes_to_bootstrap_to.clear();

#ifdef DEBUG
		printf("Ticks:%u : Node %u: User Queries will be submitted to routers with ids :",If_ptr->scheduler_ptr->now(),id);
#endif

			for (int i=0;i<num_bootstrap_nodes ;) {

 
				generated_random_index =( ( rand() % If_ptr->list_of_router_nodes.size() ) );

 				if (index_of_nodes_to_bootstrap_to.find(generated_random_index) == index_of_nodes_to_bootstrap_to.end())
				{
 					//index_of_nodes_to_bootstrap_to.insert( make_pair(generated_random_index, generated_random_index) ); 
					index_of_nodes_to_bootstrap_to.insert( generated_random_index ); 
					router_node_to_attach_to_ptr = If_ptr->list_of_router_nodes[ generated_random_index ];

#ifdef DEBUG
					printf(" %u", router_node_to_attach_to_ptr->id );
#endif

					//nodes_to_bootstrap_to.push_back( &(*(If_ptr->list_of_router_nodes[ generated_random_index ])) );
					known_routers.insert( 
								make_pair( generated_random_index ,  router_node_to_attach_to_ptr )
						); 

					i++;
				}
#ifdef DEBUG_FAULT1
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




#ifdef DEBUG


inline void ResourceNode::print_query(Query &submitted_query){
	printf("source_node_id %u, destination_node_id %u, query_seq_num %u,hop_count %u, time_to_live %u, type %d\n",
		 submitted_query.source_node_id, submitted_query.destination_node_id, 
		submitted_query.query_seq_num, submitted_query.hop_count, submitted_query.time_to_live, submitted_query.type ); 

/*
	printf("Verifying pointers \n");
	printf("source_node_ptr->id %u, destination_node_ptr->id %u\n", submitted_query.source_node_ptr->id, 
		submitted_query.destination_node_ptr->id);
*/
	return;
}



/*
void ResourceNode::print_listener_status(){
	
		//	std::map<unsigned int, Listener> listener_array;
	
	listener_itr = listener_array.begin();
	
	printf("Tick: %u , Node %u, type %d : Printing listener status.....\n", If_ptr->scheduler_ptr->now(), id, node_type );

	while (listener_itr != listener_array.end() )
	{
		printf("Tick: %u , Node %u, type %d : Listener id %u has %u responses in it, expiry status %d\n",
		If_ptr->scheduler_ptr->now(), id, node_type,
			(*listener_itr).second->id, (*listener_itr).second->responses.size(), (*listener_itr).second->expired );

		listener_itr++;
	}
		
	return;
};
*/
#endif

#ifdef DEBUG_L2

inline void RouterNode::print_destinations(Destinations &destinations){

	printf("Destinations are : ");
	routing_table_entry_itr =  destinations.begin(); 
	for (int fi = destinations.size();fi >0 ;fi --, routing_table_entry_itr++ )
	{
		printf(" %u (%u), ", (*routing_table_entry_itr)->id, (*routing_table_entry_itr)->node_type );
	}
	printf("\n");
	return;
}


inline void RouterNode::print_routing_table_index(){

	printf("routing_table_index contents are : ");
	routing_table_index_itr =  routing_table_index.begin();
	while(routing_table_index_itr !=  routing_table_index.end() )
	{
		printf(" %u, ", *routing_table_index_itr);
		routing_table_index_itr++;
	}


}




inline void RouterNode::print_routing_table()
	{
	//
					RoutingTableIterator print_routing_table_row_itr;
					RoutingTableRowIterator print_routing_table_entry_itr; 

					print_routing_table_row_itr = routing_table.begin();

					printf("Ticks:%u : Node %u: Printing routing table of router node: ",If_ptr->scheduler_ptr->now(),id); 
					if (!routing_table.empty())
					{
						cout<<"Node id : Row i : Res node num : Description tag : List of Node id(node_type) that are attached....."<<std::endl;
						int i=0;
						while (print_routing_table_row_itr != routing_table.end())
						{
							cout<<"Node :"<<id<<": Row "<<i<<" : Tag "<<(*print_routing_table_row_itr).first<<" :Res num " <<(*print_routing_table_row_itr).second->num_resource_nodes ;
												

							print_routing_table_entry_itr = (*print_routing_table_row_itr).second->destinations.begin() ;
							
							while ( print_routing_table_entry_itr != (*print_routing_table_row_itr).second->destinations.end()  )
							{
								cout<<" id "<<(*print_routing_table_entry_itr)->id<<"("<<(*print_routing_table_entry_itr)->node_type<<") ,";

								print_routing_table_entry_itr++;
							}
							cout<<std::endl;
							print_routing_table_row_itr++;
							i++;
						}
					} else printf("Routing table is empty\n");

			//
			return;
	}

inline void RouterNode::print_nearest_k()
	{

			
			nearest_k_itr = nearest_k.begin();

			//for (/*int i=0*/;/*i< k*/nearest_k_itr != nearest_k.end(); /*i++, */nearest_k_itr++)
			/*for(num_elements = nearest_k.size(); num_elements >0 ; num_elements--, nearest_k_itr++)*/
			for (int i = nearest_k.size(); i >0; i--, nearest_k_itr++)
				printf(" %u,",*nearest_k_itr);
			printf("\n");
		

		return;
	}


#endif

