#ifndef INPUT_HPP
#define INPUT_HPP

#include<string>
	
class InputParameters{
	public:
	
		//Simulation parameters

		int network_struct_report_required;

		Tick snapshot_reporter_period;	
		Tick snapshot_reporter_start_time;
		Tick oneshot_reporter_time;

		Tick run_for_ticks;

		unsigned int bootstrap_threashold;
		
		//Message parameters
		unsigned int message_time_to_live;
		unsigned int query_time_to_live;
		
		//ResourceNode parameters
		unsigned int ResourceNode_vitality;
		unsigned int ResourceNode_extroversion;
		Tick ResourceNode_delay;
		Tick ResourceNode_response_timeout;
		unsigned int ResourceNode_max_description;

		int ResourceNode_known_routers_floor;
		int ResourceNode_known_routers_trim_threshold;
		int ResourceNode_known_routers_capacity;

		int ResourceNode_translation_radius;

		//RouterNode parameters
		unsigned int RouterNode_vitality;
		unsigned int RouterNode_extroversion;
		Tick RouterNode_delay;
		Tick RouterNode_response_timeout;
		unsigned int RouterNode_num_centroid_tags_to_explore_per_period;
		unsigned int RouterNode_num_exploration_queries_to_submit_per_period;
		int RouterNode_k;
		unsigned int RouterNode_duplicate_message_cache_timeout;
		unsigned int RouterNode_max_description;
		
		int RouterNode_known_routers_floor;
		int RouterNode_known_routers_trim_threshold;
		int RouterNode_known_routers_capacity;

		int RouterNode_routing_table_length_floor;
		int RouterNode_routing_table_length_trim_threshold;
		int RouterNode_routing_table_max_length;

		int RouterNode_routing_table_width_floor;
		int RouterNode_routing_table_width_threshold;
		int RouterNode_routing_table_min_num_resource_nodes;
		int RouterNode_routing_table_max_width;

		unsigned int UserNode_vitality;  
		unsigned int UserNode_extroversion;
		Tick UserNode_response_timeout;   
		unsigned int UserNode_num_queries_to_submit_per_period;  
	
		//Birth proxy agent for resource nodes parameters
		Tick Birth_ResourceNode_start_time;
		Tick Birth_ResourceNode_period;
		unsigned int Birth_ResourceNode_total_num_cycles;
		unsigned int Birth_ResourceNode_num_cycles_per_heartbeat;
		
		//Birth proxy agent for router node parameters
		Tick Birth_RouterNode_start_time;
		Tick Birth_RouterNode_period;
		unsigned int Birth_RouterNode_total_num_cycles;
		unsigned int Birth_RouterNode_num_cycles_per_heartbeat;
		
		Tick Birth_UserNode_start_time;
		Tick Birth_UserNode_period;

		//Ontology generator parameter
		unsigned int num_words;
		unsigned int degree_of_associativity_of_words;
		float sw_model_probability;
		
		//File names
		char input_onto_file_name[20];
		char output_onto_file_name[20];
		char output_onto_report_file_name[20];
		char output_network_file_name[20];
		char structural_report_file_name[20];
		char generation_performance_report_file_name[20];
		char operation_performance_report_file_name[20];
		char routing_table_content_report_file_name[20];

		//added 5/16/07 Night
		unsigned int DatabaseHistResolution;
		unsigned int DatabaseMaxHops;

		unsigned int DatabaseHistResolution_for_UserNode;
		unsigned int DatabaseMaxHops_for_UserNode;

		unsigned int trace_source_node_id;
		unsigned int trace_destination_node_id;
		Tag trace_tag;
		
		InputParameters();


	}; //close of class

		

#endif INPUT_HPP
		
