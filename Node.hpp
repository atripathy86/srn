/* To do
	attach_after seek new routers
	check acceptability criteria to accept a node in router
	Manage routing table size
	bootstrap to more than one node
*/
//#include "Allheader.hpp"
#ifndef NODE_HPP
#define NODE_HPP
#include "ForwardDeclaration.hpp"
#include "Infrastructure.hpp"
//#include "Input.hpp"

/*
typedef std::map<Tag, std::list<ResourceNode*> > RoutingTable ;
typedef std::list<ResourceNode*> RoutingTableRow ;
typedef RoutingTable::iterator RoutingTableIterator ;
typedef RoutingTableRow::iterator RoutingTableRowIterator ;
*/

//*************These are for trimming routing table and known routers list *********************

inline bool greater_than(std::multiset< int > multiset1, std::multiset< int > multiset2){

			std::multiset< int >::iterator multiset1_itr = multiset1.begin(), multiset2_itr = multiset2.begin();
			int fi,fj;

 			for ( fi = multiset1.size(),fj = multiset2.size(); fi>0 && fj>0 ;fi--,fj--)
			{
 
				if (*multiset1_itr == *multiset2_itr)
				{
					multiset1_itr++ , multiset2_itr++;
				}
				else {
				
 					//return *multiset1_itr < *multiset2_itr ; //ascending
					return *multiset1_itr > *multiset2_itr ;//descending
				}
			}

			//Haven't returned yet and the foor loop exited, so that means the two input multiset have equal entries atleast the common portions 
 			//return (fi<fj);//asecending
			return (fi>fj);//descending
};

inline bool greater_than_strict(std::multiset< int > multiset1, std::multiset< int > multiset2, unsigned int id1, unsigned int id2){

			std::multiset< int >::iterator multiset1_itr = multiset1.begin(), multiset2_itr = multiset2.begin();
			int fi,fj;

 			for ( fi = multiset1.size(),fj = multiset2.size(); fi>0 && fj>0 ;fi--,fj--)
			{
 
				if (*multiset1_itr == *multiset2_itr)
				{
					multiset1_itr++ , multiset2_itr++;
				}
				else {
				
 					//return *multiset1_itr < *multiset2_itr ; //ascending
					return *multiset1_itr > *multiset2_itr ;//descending
				}
			}

			//Haven't returned yet and the foor loop exited, so that means the two input multiset have equal entries atleast the common portions 
 			if (fi==fj)
 			{
				//If they are equal, that means the relevance multiset is exactly the same, in that case we need to enforce stricter ordering based on node ids
				return id1> id2; //Descending
 			}
			//return (fi<fj);//asecending
			return (fi>fj);//descending

};



//if return is 0 then neither greater or equal, if returned 1 then equal, if 2 then greater
inline int equal_and_greater(std::multiset< int > multiset1, std::multiset< int > multiset2){

			std::multiset< int >::iterator multiset1_itr = multiset1.begin(), multiset2_itr = multiset2.begin();
			int fi,fj;
 
			for ( fi = multiset1.size(),fj = multiset2.size(); fi>0 && fj>0 ;fi--,fj--)
			{
				if (*multiset1_itr == *multiset2_itr)
				{
					multiset1_itr++ ; multiset2_itr++;  
				}
				else {
					return ((*multiset1_itr > *multiset2_itr) ? 2 : (*multiset1_itr == *multiset2_itr)?  1 : 0) ;
				}
 			}
			
			return (fi < fj) ? 0: (fi > fj) ? 2: 1;

};




//*************Above these are for trimming routing table and known routers list *********************




//class QueryHandler:public Handler {
class QueryHandler {	
	public:
#ifdef DEBUG
	ResourceNode *parent_node_ptr; //Update this
	//Infrastructure *If_ptr;
#endif
	//int expired;
		
	Response *response_to_send_ptr;
	Query *query_to_send_ptr;
	

	QueryHandler();
/*
#ifdef DEBUG
	QueryHandler(Infrastructure *passed_If_ptr);
#endif
*/

	//~QueryHandler();

	void transmit_response();
	void transmit_query();

};


 class Node {
	
	public:
	unsigned int id;

	QueryHandler *query_handler_ptr;
	Listener *listener_ptr;

	Event *handler_schedule_event_ptr;
	Infrastructure *If_ptr;
	NodeType node_type;

	Node(Infrastructure* passed_If_ptr);
	virtual void heartbeat(){};
	virtual ~Node(){};

};



class ResourceNode:public Node {
	
	public:
	//unsigned int num_elements;
	unsigned int num_elements1;

	unsigned int num_queries_to_submit_per_period;


#ifdef DEBUG
	unsigned int listener_seq_id; //For troubleshooting
#endif

	std::vector< std::vector<int> > *all_distances_ptr;
	unsigned int *num_vertices_at_onto_ptr ;


	//int description_matched;

	Query *query_ptr;
	Response *response_ptr;
	//std::list<Tag>* neighboring_tags_ptr;

	BootstrapNodeList bootstrap_nodes;
	
	Tag *tag_to_use_for_query_ptr;
	//int *current_query_sequence_id_ptr;

	SpecificFunctor<QueryHandler> *functor_handler_ptr;
	
	SpecificFunctor<Listener> *functor_listener_ptr;

	std::list<Query*> query_bag_for_advertising;
	//bool query_with_tag_does_not_exist;

	std::list<Query*>::iterator query_itr;
	Description::iterator description_itr;

	std::map<unsigned int, Listener*> listener_array;
	std::map< unsigned int, RouterNode *> known_routers;

	std::map<unsigned int,Listener*>::iterator listener_itr, listener_to_remove_itr;
	std::map< unsigned int, RouterNode *>::iterator router_node_address_itr;

	RouterNode *router_node_to_attach_to_ptr;

	//std::list<QueryHandler*> query_handler_array;
	//std::list<QueryHandler*>::iterator query_handler_itr, query_handler_to_remove_itr;


	//void drop_expired_listeners();

	void attach_listener(Listener &listener, unsigned int query_sequence_id);
	//void attach_query_handler(QueryHandler *passed_query_handler_ptr);
	//void drop_expired_query_handlers();

	void seek_new_routers_to_attach_to();
	//void update_query_bag_for_advertising();

	void update_query_bag(Description &from_description, std::list<Query*> &to_query_bag);



	int verify_against_description(Query &submitted_query);
	void response_action(Query &query_to_handle);

	virtual void handle_recvd_response(Response *curr_response_ptr);
	virtual void update_routing_table_action(Response *curr_response_ptr){};//=0;
	virtual int attach_request(ResourceNode* passed_node_ptr){};//=0;Added to avoid dynamic cast

	void send_queries_from(std::list<Query*> &query_bag , std::list<Query*>::iterator &curr_query_bag_itr,  int num_queries_to_send );

	virtual bool can_accept_message(Message &submitted_message);

	unsigned int vitality;
	unsigned int extroversion;
	Tick delay;
	Description description;
	
	unsigned int message_time_to_live;
	//unsigned int routing_table_capacity;
	unsigned int current_query_sequence_id;
	Tick response_timeout;

	ResourceNode(unsigned int passed_message_time_to_live, 
			
			NodeType node_type_to_pass_on,
			int known_routers_floor,
			int known_routers_trim_threshold,
			int known_routers_capacity,

			int	translation_radius,

			int routing_table_length_floor,
			int routing_table_length_trim_threshold,
			int routing_table_max_length,

			int routing_table_width_floor,
			int routing_table_width_threshold,
			int routing_table_min_num_resource_nodes,
			int routing_table_max_width,

			int passed_k, int passed_num_centroid_tags_to_explore_per_period,
			int passed_num_queries_to_submit_per_period,
			Description &passed_description, unsigned int passed_id, unsigned int passed_vitality, int passed_extroversion,
			Tick passed_delay,
			Tick passed_response_timeout, BootstrapNodeList &passed_bootstrap_nodes,
			Infrastructure* passed_If_ptr );

	void begin_life(/*BootstrapNodeList &bootstrap_nodes*/);
	virtual void bootstrap_action();
	void heartbeat();
	void submit_response(Response &submitted_response);
	virtual void submit_query(Query &submitted_query);

	virtual void periodic_housekeeping();//{}; Activated housekeeping for resource node also
	void cleanup();

#ifdef DEBUG
	void print_query(Query &submitted_query);
	void print_listener_status();
#endif

	~ResourceNode();


//*************These are for trimming routing table and known routers list *********************
		int known_routers_floor;
		int known_routers_trim_threshold;
		int known_routers_capacity;

		int	translation_radius;

	ResourceNode *this_ptr;

	RouterNode *curr_router_ptr;

	KnownRoutersIndexEncapsulator *known_routers_index_ptr;
	//void *known_routers_index_itr_ptr;

	std::multiset< int > relevance( Tag tag1){
				std::multiset< int > temp_multiset;
				description_itr = description.begin();
				
				for (int fi= description.size();fi>0 ;fi--, description_itr++ )
					temp_multiset.insert( ((*all_distances_ptr)[*description_itr][tag1] ));
				return temp_multiset;

			};


	std::multiset<int > relevance( Description description2){
				std::multiset<  int > temp_multiset;
				Description::iterator description2_itr;  
				description_itr = description.begin();

				for (int fi= description.size();fi>0 ;fi--, description_itr++ )
				{
					description2_itr = description2.begin();
					for (int fj= description2.size();fj>0 ;fj--, description2_itr++ ) 
					{
						temp_multiset.insert( ((*all_distances_ptr)[*description_itr][*description2_itr] ));

					}
				}
				return temp_multiset;

			};

	void trim_known_routers();


//*************Above these are for trimming routing table and known routers list *********************


};


	//Copy from here
	class routing_table_rows_greater_than {
			public:
			ResourceNode *parent_ptr;
 			routing_table_rows_greater_than(ResourceNode *passed_parent_ptr):parent_ptr(passed_parent_ptr){};//parent_ptr= passed_parent_ptr;};
			//bool operator()(Tag tag1, Tag tag2) // STL containers require comparator to be const-invocable (C++17 static_assert)
		bool operator()(Tag tag1, Tag tag2) const
				{
					return greater_than((*parent_ptr).relevance( tag1), (*parent_ptr).relevance( tag2));
				}
		};

/*
	class routing_table_cols_greater_than {
			public:
			ResourceNode *parent_ptr;
 			routing_table_cols_greater_than(ResourceNode *passed_parent_ptr):parent_ptr(passed_parent_ptr){};//{parent_ptr =passed_parent_ptr;};
			bool operator()(ResourceNode* ResourceNode_ptr1, ResourceNode* ResourceNode_ptr2) //const
				{
					return greater_than((*parent_ptr).relevance( ResourceNode_ptr1->description ), (*parent_ptr).relevance( ResourceNode_ptr2->description ));
				}
		};
*/
		class routing_table_cols_greater_than_strict {
			public:
			ResourceNode *parent_ptr;
 			routing_table_cols_greater_than_strict(ResourceNode *passed_parent_ptr):parent_ptr(passed_parent_ptr){};//{parent_ptr =passed_parent_ptr;};
			bool operator()(ResourceNode* ResourceNode_ptr1, ResourceNode* ResourceNode_ptr2) const //const
				{
					return greater_than_strict((*parent_ptr).relevance( ResourceNode_ptr1->description ), (*parent_ptr).relevance( ResourceNode_ptr2->description ),
													ResourceNode_ptr1->id, ResourceNode_ptr2->id);
				}
		};





//Old ones from ver 1.15
/*
typedef std::multiset<ResourceNode*  > Destinations;
typedef std::map<Tag, RoutingTableRow*   > RoutingTable ;
//typedef RoutingTableRow::iterator RoutingTableRowIterator;
typedef RoutingTable::iterator RoutingTableIterator ;
typedef Destinations::iterator RoutingTableRowIterator;
*/

//*************These are for trimming routing table and known routers list *********************

//typedef std::multiset<ResourceNode* , routing_table_cols_greater_than > Destinations;
typedef std::set<ResourceNode* , routing_table_cols_greater_than_strict > Destinations;

class RoutingTableRow {
	public:
	Destinations destinations;
	unsigned int num_resource_nodes;
	RoutingTableRow(ResourceNode* this_ptr)  :  destinations( (routing_table_cols_greater_than_strict(this_ptr)) ) {num_resource_nodes = 0;};
	void insert(ResourceNode* &res_ptr) { destinations.insert(res_ptr); }
};

typedef std::map<Tag, RoutingTableRow*  > RoutingTable ; //New
typedef std::multiset<Tag , routing_table_rows_greater_than > RoutingTableIndex ; //new
typedef Destinations::iterator RoutingTableRowIterator;
typedef RoutingTable::iterator RoutingTableIterator ;
typedef RoutingTableIndex::iterator RoutingTableIndexIterator ; //New



//*************Above these are for trimming routing table and known routers list *********************







class UserNode:public ResourceNode {
	
	std::set<unsigned int> index_of_nodes_to_bootstrap_to;
	Tag generated_random_tag;
	unsigned int generated_random_index;

	//RouterNode *generated_random_bootstrap_node_ptr;

	public:
	UserNode(unsigned int passed_message_time_to_live,
			
			NodeType node_type_to_pass_on,
			int known_routers_floor,
			int known_routers_trim_threshold,
			int known_routers_capacity,

			int	translation_radius,

			int routing_table_length_floor,
			int routing_table_length_trim_threshold,
			int routing_table_max_length,

			int routing_table_width_floor,
			int routing_table_width_threshold,
			int routing_table_min_num_resource_nodes,
			int routing_table_max_width,

		int passed_k, int passed_num_centroid_tags_to_explore_per_period,
		int passed_num_queries_to_submit_per_period,
		Description &passed_description, unsigned int passed_id, unsigned int passed_vitality, int passed_extroversion, 
			Tick passed_delay,
			Tick passed_response_timeout, BootstrapNodeList &passed_bootstrap_nodes,
			Infrastructure* passed_If_ptr);
	
	void bootstrap_action(){};
	void generate_random_description(unsigned int num_description);
	void generate_random_bootstrap_nodes(unsigned int num_bootstrap_nodes) ;

	void heartbeat();
	void handle_recvd_response(Response *curr_response_ptr);
	~UserNode(){ index_of_nodes_to_bootstrap_to.clear();cleanup(); };

};


#ifdef DUPPACK_CACHE
class Message_key {

	public:
	//unsigned int id;
	//unsigned int seq_num;
	std::pair<unsigned int, unsigned int> key;
	Tick time_to_expire;
	
	Message_key(){key.first =0;key.second =0;time_to_expire=0; };
	Message_key(const Message_key& passed_message_key){
			key = passed_message_key.key;
			//id = passed_message_key.id;
			//seq_num = passed_message_key.seq_num;
			time_to_expire = passed_message_key.time_to_expire;
		};

	Message_key(unsigned int passed_id,unsigned int passed_seq_num, Tick passed_time_to_expire){ 
			//id = passed_id; 
			//seq_num = passed_seq_num; 
			key = make_pair( passed_id, passed_seq_num);
			time_to_expire = passed_time_to_expire;
		};

	friend bool operator==(const Message_key& k1,const Message_key& k2){
           //return k1.id==k2.id && k1.seq_num==k2.seq_num;
		   return k1.key == k2.key;
		}
	friend bool operator!=(const Message_key& k1,const Message_key& k2){
           //return k1.id!=k2.id || k1.seq_num!=k2.seq_num;
		   return k1.key != k2.key;
		}
	friend bool operator>(const Message_key& k1,const Message_key& k2){
           //return k1.id>k2.id ;
		   //return k1.seq_num>k2.seq_num ;
		   return k1.key>k2.key;

		}
	friend bool operator<(const Message_key& k1,const Message_key& k2){
         // return k1.id<k2.id ;
		  // return k1.seq_num<k2.seq_num ;
		  return k1.key<k2.key;
		}
};

#endif



class RouterNode:public ResourceNode {
	
	private:
	std::list<Tag> nearest_k;
	std::list<Tag>::iterator nearest_k_itr;

	unsigned int routing_table_size;
	//std::list<ResourceNode*> *routing_table_row_obj_ptr;
	//std::list<ResourceNode*> routing_table_row_obj;
	RoutingTableRow *routing_table_row_obj_ptr;

	std::map< unsigned int, RouterNode *> routers_attached;
	std::map< unsigned int, ResourceNode *> resources_attached;
	std::list<Query*> query_bag_for_exploring;
	std::list<Query*>::iterator curr_query_bag_for_exploring_itr;
	//std::list<Query*>::reverse_iterator query_rev_itr;

	std::list<Query*> temp_query_bag_for_exploring;
	std::vector<Tag> knowledge;

	//std::list<ResourceNode*>::iterator routing_table_entry_itr; //From QueryHandler
	RoutingTableRowIterator routing_table_entry_itr; //From QueryHandler

	Description descriptions_to_explore;
	Description temp_descriptions_to_explore;

    unsigned int num_centroid_tags_to_explore_per_period;

	Description::iterator description_budget_itr;//Has memory and tracks from which centroid topic to start again

 	unsigned int semantic_space_inner_radius;

	//For duplicate message testing
#ifdef DUPPACK_SET
	QueryAlreadyReceived query_already_received_set;
	QueryAlreadyReceived::iterator query_already_received_set_itr;
	NodeId_QuerySeqNum query_key;
#endif

#ifdef DUPPACK_CACHE
	std::set<Message_key> message_key_cache;
	std::set<Message_key>::iterator message_key_cache_itr;//, message_key_to_remove_itr ;
	Message_key message_key;
#endif

	void update_descriptions_to_explore();

	void seek_new_resources_to_update_routing_table();
	//void free_space_in_routing_table();
	int can_accept(Node* passed_node_ptr);
	void selective_multicast_action(Query &query_to_handle);
	bool can_accept_message(Message &submitted_message);

	//OntologyRepository services
	void generate_nearest_neighbors(Tag focus_vertice /*, int k, const std::vector<Tag> &neighbors, std::list<Tag> &nearest_k*/); 

	void generate_neighbors_to_explore(Tag focus_vertice , int radius /*, std::list<Tag> &nearest_k*/); 

	void generate_new_neighbors_to_explore(Tag focus_vertice , int inner_radius, int radius_increment /*, std::list<Tag> &nearest_k */);//From Suneil

	int distance(Tag vertice1, Tag vertice2);


	public:
	//RoutingTable routing_table;
	RoutingTable routing_table;

	unsigned int k;
	//unsigned int routing_table_capacity; //Moved to ResourceNode
	RouterNode(unsigned int passed_message_time_to_live, 
		
		NodeType node_type_to_pass_on,
		int known_routers_floor,
		int known_routers_trim_threshold,
		int known_routers_capacity,

		int	translation_radius,	

		int routing_table_length_floor,
		int routing_table_length_trim_threshold,
		int routing_table_max_length,

		int routing_table_width_floor,
		int routing_table_width_threshold,
		int routing_table_min_num_resource_nodes,
		int routing_table_max_width,

		int passed_k, int passed_num_centroid_tags_to_explore_per_period,
		 int passed_num_queries_to_submit_per_period,
		Description &passed_description, unsigned int passed_id, unsigned int passed_vitality,  int passed_extroversion, 
			Tick passed_delay,
			Tick passed_response_timeout, BootstrapNodeList &passed_bootstrap_nodes,
			Infrastructure* passed_If_ptr);
	
	void heartbeat();

	void submit_query(Query &submitted_query);
	int attach_request(ResourceNode* passed_node_ptr);
	bool routing_filter( ResourceNode *resource_node_entry_in_routing_table_ptr, Query &query_to_route );
	void periodic_housekeeping();

	void update_routing_table_action(Response *curr_response_ptr);

	~RouterNode();


#ifdef DEBUG_L2
	void print_routing_table();
	void print_nearest_k();
	void print_routing_table_index();
	void print_destinations(Destinations &destinations);
#endif

//*************These are for trimming routing table and known routers list *********************

	std::vector<Tag>::iterator knowledge_itr;

	int routing_table_length_floor;
	int routing_table_length_trim_threshold;
	int routing_table_max_length;

	int routing_table_width_floor;
	int routing_table_width_threshold;
	int routing_table_min_num_resource_nodes;
	int routing_table_max_width;

	RoutingTableIndex routing_table_index;

	RoutingTableIterator routing_table_itr ;

	RoutingTableIndexIterator routing_table_index_itr ;//New
	//RoutingTableRowIterator routing_table_entry_to_remove_itr;

	RouterNode(Infrastructure *passed_If_ptr);
	
	void trim_routing_table_length();

	//void trim_routing_table_specific_width(Destinations &destinations_to_trim);
	void trim_routing_table_specific_width(RoutingTableRow* row_to_trim_ptr);

	void trim_routing_table_all_widths();



};
  




class KnownRouterIndexElement {
	public:
	RouterNode *RouterNode_ptr;
	unsigned int id;
	unsigned int performance;
	//Ticks birth_ticks;
	unsigned int birth_ticks;
	KnownRouterIndexElement(RouterNode *passed_RouterNode_ptr  );
};


class known_routers_index_less_than {
		public:
		ResourceNode *parent_ptr;
		known_routers_index_less_than (ResourceNode *passed_parent_ptr):parent_ptr(passed_parent_ptr){};//parent_ptr= passed_parent_ptr;};
		bool operator()(KnownRouterIndexElement* element_ptr1, KnownRouterIndexElement* element_ptr2) const {

			switch (equal_and_greater( (*(element_ptr1->RouterNode_ptr)).relevance( (*parent_ptr).description ), (*(element_ptr2->RouterNode_ptr)).relevance( (*parent_ptr).description ) ) )
			{
				case 0:
						return 0;
					break;

				case 1:

						return (element_ptr1->birth_ticks < element_ptr2->birth_ticks) ? 1 : 0;
					break;

				case 2:

						return 1;
			}
				
		}
		
};


typedef std::multiset< KnownRouterIndexElement*, known_routers_index_less_than > KnownRoutersIndex;
typedef KnownRoutersIndex::iterator KnownRoutersIndexIterator;

//class KnownRoutersIndexIterator: public KnownRoutersIndex::iterator;

class KnownRoutersIndexEncapsulator {

	KnownRouterIndexElement* index_element_ptr;

	public:
	KnownRoutersIndex  index;

	KnownRoutersIndexEncapsulator(ResourceNode* passed_ResourceNode_ptr):index(( known_routers_index_less_than(passed_ResourceNode_ptr) )){}; 
	
	void insert(RouterNode* RouterNode_ptr){

		index_element_ptr = new KnownRouterIndexElement(RouterNode_ptr /*,RouterNode_ptr->If_ptr->scheduler_ptr->now()*/);
	
		index.insert(index_element_ptr);

	}

	int size(){ int temp; temp = index.size(); return temp; }
	//KnownRoutersIndex::iterator begin(){return index.begin();}
	KnownRoutersIndexIterator begin(){return index.begin();}
	void erase(KnownRoutersIndexIterator &start_itr, KnownRoutersIndexIterator &end_itr){
		
		index.erase(start_itr, end_itr);
	}
};







#endif

