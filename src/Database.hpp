#ifndef DATABASE_HPP
#define DATABASE_HPP
//#include "ForwardDeclaration.hpp"
//#include "Input.hpp"

#include <cmath>
#include <stdio.h>


class Database {
	private:
	InputParameters *param_ptr;
	Infrastructure *If_ptr;
	int histogram_index;
	public:

	//Counters for internal queries

	unsigned int Queries_sent_from_ResourceNode;
	unsigned int Queries_sent_from_RouterNode;

	unsigned int Queries_forwarded_by_RouterNode_orgininated_from_ResourceNode;
	unsigned int Queries_forwarded_by_RouterNode_orgininated_from_RouterNode;

	unsigned int *Num_of_queries_for_specific_tag;

	unsigned int Responses_sent_from_ResourceNode;
	unsigned int Responses_sent_from_RouterNode;

	unsigned int Num_of_good_responses;
	
	
	unsigned int *num_resource_with_specific_tag;
	unsigned int *num_router_with_specific_tag;

	unsigned int *Num_of_good_responses_for_specific_tag;
	
	unsigned int Num_duplicate_message_dropped;
	unsigned int Num_message_dropped_for_TTL;
	unsigned int Num_looping_message_dropped; 

	unsigned int Num_queries_dropped_for_TTL; //Suneil | ctr number_queries dropped for ttl //figure out from where you are updating the above ctr.
	unsigned int Num_responses_dropped_for_TTL;// Suneil | ctr number_responses dropped for ttl.

	unsigned int Num_total_message_generated; //Suneil | add ctr: num_total_mesasge_generated; //total load on the network,

	/*
	unsigned int DatabaseHistResolution;	//user input param
	unsigned int DatabaseMaxHops;			//user input param

	unsigned int DatabaseHistResolution_for_UserNode;	//user input param
	unsigned int DatabaseMaxHops_for_UserNode;			//user input param
	*/

	//sm24
	unsigned int Num_total_queries_generated;
	unsigned int Num_total_responses_generated;
	
	unsigned int *hops_before_response_ctr; //used for the histogram
	unsigned int *longevity_of_message_ctr_for_TTL; //Suneil
	unsigned int *longevity_of_message_ctr_for_loop; 

	unsigned num_buckets;
	
	//Counters for external queries
	unsigned int Queries_sent_from_UserNode;
	unsigned int *Num_of_queries_for_specific_tag_from_UserNode;
	
	unsigned int Num_responses_sent_for_UserNode;
	unsigned int Num_of_good_responses_for_UserNode;

	unsigned int *Num_of_good_responses_for_specific_tag_for_UserNode;
		
	unsigned int Num_duplicate_message_dropped_for_UserNode;
	unsigned int Num_looping_message_dropped_for_UserNode;

	unsigned int Num_reponses_dropped_when_no_active_listeners;
	unsigned int Num_reponses_dropped_when_no_active_listeners_for_UserNode;

	unsigned int Num_queries_dropped_for_TTL_for_UserNode; //Suneil | ctr number_queries dropped for ttl //figure out from where you are updating the above ctr.
	unsigned int Num_responses_dropped_for_TTL_for_UserNode;// Suneil | ctr number_responses dropped for ttl.

	unsigned int Num_total_message_generated_for_UserNode; //Suneil | add ctr: num_total_mesasge_generated; //total load on the network,

	
	unsigned int DatabaseHistResolution_for_UserNode;	//user input param
	unsigned int DatabaseMaxHops_for_UserNode;			//user input param

	unsigned int *hops_before_response_ctr_for_UserNode; //used for the histogram
	unsigned int *longevity_of_message_ctr_for_TTL_for_UserNode; //Suneil
	unsigned int *longevity_of_message_ctr_for_loop_for_UserNode;  

	unsigned num_buckets_for_UserNode;

	Database(Infrastructure* passed_If_ptr); //does some param
	//Database(InputParameters *passed_param_ptr);
	~Database(); //explicit destructor, has a delete function for * array
	
	//void take_snapshot(); //creates a snapshot of all the above values when called
	//void hops_at_response(unsigned int hop_count, MessageType message_type); // creates the histrogram for average number of hops for good responses
	void record_hops(unsigned int hop_count, unsigned int *passed_ctr);



};

#endif  // #ifndef DATABASE_HPP

