#include "ForwardDeclaration.hpp"
//#include "Allheader.hpp"
#include "Input.hpp"

InputParameters::InputParameters () //constructor	

{
			//Default Simulation Parameters : Should be overwritten if necessary from read_input_parameters()
			//Simulation parameters

			network_struct_report_required = 0;
			snapshot_reporter_period = 100;
			snapshot_reporter_start_time = 0;
			oneshot_reporter_time = 1;

			run_for_ticks = 205;

			bootstrap_threashold = 4;

			//Message parameters
			message_time_to_live = 5;
			query_time_to_live = 10;

			//ResourceNode parameters
			ResourceNode_vitality = 10;
			ResourceNode_extroversion = 3;
			ResourceNode_delay = 1;
			ResourceNode_response_timeout = 100;
			ResourceNode_max_description = 1;

			ResourceNode_known_routers_floor = 4;
			ResourceNode_known_routers_trim_threshold = 7;
			ResourceNode_known_routers_capacity  = 12;
		 
			ResourceNode_translation_radius = 1;

			//RouterNode parameters
			RouterNode_vitality = 20;
			RouterNode_extroversion = 1;
			RouterNode_delay = 1;
			RouterNode_response_timeout = 100;
			RouterNode_num_centroid_tags_to_explore_per_period = 2;
			RouterNode_num_exploration_queries_to_submit_per_period = 5;
			RouterNode_k = 3;
			RouterNode_duplicate_message_cache_timeout = 50;
			RouterNode_max_description = 9;

			RouterNode_known_routers_floor =4;
			RouterNode_known_routers_trim_threshold = 7;
			RouterNode_known_routers_capacity = 12;

			RouterNode_routing_table_length_floor = 8 ;
		    RouterNode_routing_table_length_trim_threshold = 12;
			RouterNode_routing_table_min_num_resource_nodes = 4;
		    RouterNode_routing_table_max_length = 15;

		    RouterNode_routing_table_width_floor = 4;
		    RouterNode_routing_table_width_threshold = 6;
		    RouterNode_routing_table_max_width = 10;

			UserNode_vitality = 10;
			UserNode_extroversion = 3;
			UserNode_response_timeout = 50;
			UserNode_num_queries_to_submit_per_period = 5;


			//Birth proxy agent for resource nodes parameters
			Birth_ResourceNode_start_time = 20;
			Birth_ResourceNode_period = 20;
			Birth_ResourceNode_total_num_cycles = 5;
			Birth_ResourceNode_num_cycles_per_heartbeat = 5;

			//Birth proxy agent for router node parameters
			Birth_RouterNode_start_time = 0;
			Birth_RouterNode_period = 30;
			Birth_RouterNode_total_num_cycles = 5;
			Birth_RouterNode_num_cycles_per_heartbeat = 5;

			//Birth proxy agent for user node
			Birth_UserNode_start_time = 100;
			Birth_UserNode_period = 100;

			//Ontology generator parameter
			num_words = 100;
			degree_of_associativity_of_words = 4;
			sw_model_probability = 0.02;

			//File names
			strcpy(input_onto_file_name,"ontoin");
			strcpy(output_onto_file_name,"ontoout.net");
			strcpy(output_onto_report_file_name,"ontorep");
			strcpy(output_network_file_name,"netout.net");
			strcpy(structural_report_file_name,"netstrucrep");
			strcpy(generation_performance_report_file_name,"gennetperfrep");
			strcpy(operation_performance_report_file_name,"oprnetperfrep");
			strcpy(routing_table_content_report_file_name,"netroutingtable");

			//Database related
			DatabaseHistResolution = 5;
			DatabaseMaxHops = 40;
			DatabaseHistResolution_for_UserNode = 5;
			DatabaseMaxHops_for_UserNode = 40;

			trace_source_node_id =0;
			trace_destination_node_id =0;
			trace_tag = 0;

			return;
} // close of constructor


