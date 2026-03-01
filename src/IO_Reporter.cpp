#include "ForwardDeclaration.hpp"
#include "Input.hpp"
#include "IO_Reporter.hpp"
#include "Event.hpp"
#include "Infrastructure.hpp"
#include "Functor.hpp"
#include "Node.hpp" //Added later
//#include "Scheduler.hpp"

//#include "SmallWorldGenerator.hpp"


#include <sstream>
#include <iostream>
#include <fstream>
#include <sys/stat.h>  // mkdir

#include "ForwardDeclaration.hpp"
#include "Database.hpp"
#include "Message.hpp"//Added for the NODE_VISTED_LIST_SIZE


IO_Reporter::IO_Reporter(Infrastructure* passed_If_ptr)
{
		If_ptr = passed_If_ptr;
		return;
};

IO_Reporter::~IO_Reporter()
{
	return;
};


void IO_Reporter::read_input_parameter(char *passed_input_file_name, Infrastructure* passed_If_ptr) {
	
	If_ptr = passed_If_ptr;
	param_ptr = If_ptr->param_ptr;
	std::istringstream *ss_ptr; 
		std::string smline;
		std::ifstream file (passed_input_file_name,std::ios::in);
		if (file.is_open())
		{
		while(!file.eof())
			{
			 float float_value; unsigned int value; char temp[1000];char temp2[100];
			 smline.clear();
			 getline(file,smline);

			 // Strip Windows-style \r (CRLF files on Linux leave \r at end of line)
			 if (!smline.empty() && smline[smline.size()-1] == '\r')
			 	smline.erase(smline.size()-1);
			 // Truncate at '#' so inline comments don't interfere with parsing
			 {
			 	std::string::size_type hash_pos = smline.find('#');
			 	if (hash_pos != std::string::npos)
			 		smline.erase(hash_pos);
			 }

			 //std::istringstream ss(smline);
			 ss_ptr = new istringstream(smline);
			 // Old code read temp unconditionally then fell into the if/else chain; this caused
			 // the classic while(!eof) bug: on the extra post-EOF iteration getline returns "",
			 // >>temp fails and leaves temp with its previous stack value, >>value yields 0,
			 // and the last-parsed parameter gets silently overwritten with 0.
			 // Fix: skip the if/else chain entirely if >>temp fails (empty or EOF line).
			 if (!((*ss_ptr)>>temp)) { delete ss_ptr; continue; } // skip blank / unreadable lines

			   if (strcmp(temp, "network_struct_report_required") ==0 ) { (*ss_ptr)>>value;  param_ptr->network_struct_report_required = value ;If_ptr->analyst.network_struct_report_required = value; } 

					else if (strcmp(temp, "snapshot_reporter_period") ==0 ) { (*ss_ptr)>>value;  param_ptr->snapshot_reporter_period = value    ; } 
					else if (strcmp(temp, "snapshot_reporter_start_time") ==0 ) { (*ss_ptr)>>value;  param_ptr->snapshot_reporter_start_time = value    ; } 
					else if (strcmp(temp, "oneshot_reporter_time") ==0 ) { (*ss_ptr)>>value;  param_ptr->oneshot_reporter_time = value    ; }
	
			
					else if (strcmp(temp, "run_for_ticks") ==0 ) { (*ss_ptr)>>value;  param_ptr->run_for_ticks = value; }
					else if (strcmp(temp, "bootstrap_threashold") ==0 ) { (*ss_ptr)>>value;  param_ptr->bootstrap_threashold = value ; }
					else if (strcmp(temp, "message_time_to_live") ==0 ) { (*ss_ptr)>>value;  param_ptr->message_time_to_live = value ; }
					else if (strcmp(temp, "query_time_to_live") ==0 ) { (*ss_ptr)>>value;  param_ptr->query_time_to_live = value ; }

				   	else if (strcmp(temp, "ResourceNode_vitality") ==0 ) { (*ss_ptr)>>value; param_ptr->ResourceNode_vitality = value ; }
					else if (strcmp(temp, "ResourceNode_extroversion") ==0 ) { (*ss_ptr)>>value;  param_ptr->ResourceNode_extroversion = value    ; }
					else if (strcmp(temp, "ResourceNode_delay") ==0 ) { (*ss_ptr)>>value;  param_ptr->ResourceNode_delay = value    ; }
					else if (strcmp(temp, "ResourceNode_response_timeout") ==0 ) { (*ss_ptr)>>value;  param_ptr->ResourceNode_response_timeout = value    ; }
					else if (strcmp(temp, "ResourceNode_max_description") ==0 ) { (*ss_ptr)>>value;  param_ptr->ResourceNode_max_description = value    ; }

					else if (strcmp(temp, "ResourceNode_known_routers_floor") ==0 ) { (*ss_ptr)>>value;  param_ptr->ResourceNode_known_routers_floor = value    ; }
					else if (strcmp(temp, "ResourceNode_known_routers_trim_threshold") ==0 ) { (*ss_ptr)>>value;  param_ptr->ResourceNode_known_routers_trim_threshold = value    ; }
					else if (strcmp(temp, "ResourceNode_known_routers_capacity") ==0 ) { (*ss_ptr)>>value;  param_ptr->ResourceNode_known_routers_capacity = value    ; }
					else if (strcmp(temp, "ResourceNode_translation_radius") ==0 ) { (*ss_ptr)>>value;  param_ptr->ResourceNode_translation_radius = value    ; }
					
					else if (strcmp(temp, "RouterNode_vitality") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_vitality = value    ; }
					else if (strcmp(temp, "RouterNode_extroversion") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_extroversion = value    ; }
					else if (strcmp(temp, "RouterNode_delay") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_delay = value    ; }
					else if (strcmp(temp, "RouterNode_response_timeout") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_response_timeout = value    ; }
					else if (strcmp(temp, "RouterNode_num_centroid_tags_to_explore_per_period") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_num_centroid_tags_to_explore_per_period = value    ; }
					else if (strcmp(temp, "RouterNode_num_exploration_queries_to_submit_per_period") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_num_exploration_queries_to_submit_per_period = value    ; }
					else if (strcmp(temp, "RouterNode_k") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_k = value    ; }
					else if (strcmp(temp, "RouterNode_duplicate_message_cache_timeout") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_duplicate_message_cache_timeout = value    ; }
					else if (strcmp(temp, "RouterNode_max_description") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_max_description = value    ; }

					else if (strcmp(temp, "RouterNode_known_routers_floor") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_known_routers_floor = value    ; }
					else if (strcmp(temp, "RouterNode_known_routers_trim_threshold") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_known_routers_trim_threshold =value;} //addded 5/16 :: 23:25hrs
					else if (strcmp(temp, "RouterNode_known_routers_capacity") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_known_routers_capacity = value    ; }

					else if (strcmp(temp, "RouterNode_routing_table_length_floor") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_routing_table_length_floor =value;} //addded 5/16 :: 23:25hrs
					else if (strcmp(temp, "RouterNode_routing_table_length_trim_threshold") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_routing_table_length_trim_threshold = value    ; }
					else if (strcmp(temp, "RouterNode_routing_table_min_num_resource_nodes") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_routing_table_min_num_resource_nodes =value;} //addded 5/16 :: 23:25hrs
					else if (strcmp(temp, "RouterNode_routing_table_max_length") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_routing_table_max_length =value;} //addded 5/16 :: 23:25hrs
					
					else if (strcmp(temp, "RouterNode_routing_table_width_floor") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_routing_table_width_floor = value    ; }
					else if (strcmp(temp, "RouterNode_routing_table_width_threshold") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_routing_table_width_threshold =value;} //addded 5/16 :: 23:25hrs
					else if (strcmp(temp, "RouterNode_routing_table_max_width") ==0 ) { (*ss_ptr)>>value;  param_ptr->RouterNode_routing_table_max_width = value    ; }

					else if (strcmp(temp, "UserNode_vitality") ==0 ) { (*ss_ptr)>>value;  param_ptr->UserNode_vitality = value    ; }
					else if (strcmp(temp, "UserNode_extroversion") ==0 ) { (*ss_ptr)>>value;  param_ptr->UserNode_extroversion = value    ; }
					else if (strcmp(temp, "UserNode_response_timeout") ==0 ) { (*ss_ptr)>>value;  param_ptr->UserNode_response_timeout = value    ; }
					else if (strcmp(temp, "UserNode_num_queries_to_submit_per_period") ==0 ) { (*ss_ptr)>>value;  param_ptr->UserNode_num_queries_to_submit_per_period = value    ; }
					

					else if (strcmp(temp, "Birth_ResourceNode_start_time") ==0 ) { (*ss_ptr)>>value;  param_ptr->Birth_ResourceNode_start_time = value    ; }
					else if (strcmp(temp, "Birth_ResourceNode_period") ==0 ) { (*ss_ptr)>>value;  param_ptr->Birth_ResourceNode_period = value    ; }
					else if (strcmp(temp, "Birth_ResourceNode_total_num_cycles") ==0 ) { (*ss_ptr)>>value;  param_ptr->Birth_ResourceNode_total_num_cycles = value    ; }
					else if (strcmp(temp, "Birth_ResourceNode_num_cycles_per_heartbeat") ==0 ) { (*ss_ptr)>>value;  param_ptr->Birth_ResourceNode_num_cycles_per_heartbeat = value    ; }
					
					else if (strcmp(temp, "Birth_RouterNode_start_time") ==0 ) { (*ss_ptr)>>value;  param_ptr->Birth_RouterNode_start_time = value ; }
					else if (strcmp(temp, "Birth_RouterNode_period") ==0 ) { (*ss_ptr)>>value;  param_ptr->Birth_RouterNode_period = value; }
					else if (strcmp(temp, "Birth_RouterNode_total_num_cycles") ==0 ) { (*ss_ptr)>>value;  param_ptr->Birth_RouterNode_total_num_cycles = value    ; }
					else if (strcmp(temp, "Birth_RouterNode_num_cycles_per_heartbeat") ==0 ) { (*ss_ptr)>>value;  param_ptr->Birth_RouterNode_num_cycles_per_heartbeat = value    ; }

					else if (strcmp(temp, "Birth_UserNode_start_time") ==0 ) { (*ss_ptr)>>value;  param_ptr->Birth_UserNode_start_time = value ; }
					else if (strcmp(temp, "Birth_UserNode_period") ==0 ) { (*ss_ptr)>>value;  param_ptr->Birth_UserNode_period = value ; }
					

					else if (strcmp(temp, "num_words") ==0 ) { (*ss_ptr)>>value;  param_ptr->num_words = value    ; }
					else if (strcmp(temp, "degree_of_associativity_of_words") ==0 ) { (*ss_ptr)>>value;  param_ptr->degree_of_associativity_of_words = value    ; }
					else if (strcmp(temp, "sw_model_probability") ==0 ) { (*ss_ptr)>>float_value;  param_ptr->sw_model_probability = float_value ; }
					
					
					else if (strcmp(temp, "input_onto_file_name") ==0 ) { (*ss_ptr)>>temp2; strcpy(param_ptr->input_onto_file_name,temp2);  ; }
					else if (strcmp(temp, "output_onto_file_name") ==0 ) { (*ss_ptr)>>temp2;strcpy(param_ptr->output_onto_file_name, temp2) ; }
					else if (strcmp(temp, "output_onto_report_file_name") ==0 ) { (*ss_ptr)>>temp2;strcpy(param_ptr->output_onto_report_file_name , temp2 ) ; } //added 5/17 ::00:36hrs
					else if (strcmp(temp, "output_network_file_name") ==0 ) { (*ss_ptr)>>temp2;strcpy( param_ptr->output_network_file_name  , temp2 ) ; }
					else if (strcmp(temp, "structural_report_file_name") ==0 ) { (*ss_ptr)>>temp2;strcpy( param_ptr->structural_report_file_name  , temp2 ) ; }
					else if (strcmp(temp, "generation_performance_report_file_name") ==0 ) { (*ss_ptr)>>temp2;strcpy(param_ptr->generation_performance_report_file_name  , temp2 ) ; }
					else if (strcmp(temp, "operation_performance_report_file_name") ==0 ) { (*ss_ptr)>>temp2;strcpy(param_ptr->operation_performance_report_file_name  , temp2 ) ; }
					else if (strcmp(temp, "routing_table_content_report_file_name") ==0 ) { (*ss_ptr)>>temp2;strcpy(param_ptr->routing_table_content_report_file_name , temp2 ) ; } //added 5/17 ::00:36hrs
					
	
					else if (strcmp(temp, "DatabaseHistResolution") ==0 ) { (*ss_ptr)>>value;  param_ptr->DatabaseHistResolution = value    ; }//added 6/22 ::09:00hrs
					else if (strcmp(temp, "DatabaseMaxHops") ==0 ) { (*ss_ptr)>>value;  param_ptr->DatabaseMaxHops = value    ; }//added 6/22 ::09:00hrs
					else if (strcmp(temp, "DatabaseHistResolution_for_UserNode") ==0 ) { (*ss_ptr)>>value;  param_ptr->DatabaseHistResolution_for_UserNode = value    ; }//added 6/22 ::09:00hrs
					else if (strcmp(temp, "DatabaseMaxHops_for_UserNode") ==0 ) { (*ss_ptr)>>value;  param_ptr->DatabaseMaxHops_for_UserNode = value    ; }//added 6/22 ::09:00hrs

					else if (strcmp(temp, "trace_source_node_id") ==0 ) { (*ss_ptr)>>value;  param_ptr->trace_source_node_id = value ; } 
					else if (strcmp(temp, "trace_destination_node_id") ==0 ) { (*ss_ptr)>>value;  param_ptr->trace_destination_node_id = value    ; } 
					else if (strcmp(temp, "trace_tag") ==0 ) { (*ss_ptr)>>value;  param_ptr->trace_tag = value    ; } 

					else if (strcmp(temp, "output_dir") ==0 ) { (*ss_ptr)>>temp2; strcpy(param_ptr->output_dir, temp2); } 


			 // else {std::cout<<std::endl<<"Unexpected Parameter: Read: "<<smline<<std::endl;}

				delete ss_ptr;

		   } //close of while 
		} //close of if

		else {
			std::cout<<std::endl<<"Unable to open config file, using default values"<<std::endl;
			
			}
		file.close();

		verify_config_data();

		// Create output directory (no-op if it already exists)
		mkdir(param_ptr->output_dir, 0755);

		return;	
};

//This one validates the config data
inline void IO_Reporter::verify_config_data(){
	
	if (param_ptr->message_time_to_live >= NODE_VISTED_LIST_SIZE || param_ptr->query_time_to_live >= NODE_VISTED_LIST_SIZE )
	{
		printf("Error: the time to live parameter should be greater than NODE_VISTED_LIST_SIZE\n");
		exit(1);
	}
	
	
};

void IO_Reporter::write_input_parameter_on_screen_from(char * passed_input_file_name ) {
			/*
				snapshot_reporter_period 10
				snapshot_reporter_start_time 1
				oneshot_reporter_time 0

				run_for_ticks 100 
				bootstrap_threashold 4 
				message_time_to_live 3  # For queries and responses inside the network
				query_time_to_live 10   # For user queries


				ResourceNode_vitality 20 
				ResourceNode_extroversion 3  #
				ResourceNode_delay 5
				ResourceNode_response_timeout  10  # This param should be sufficiently big to handle the delay in the network, else will kill listeners even before all response arrive, and throw "May be an error.."
				ResourceNode_max_description 1

				RouterNode_vitality 30 
				RouterNode_extroversion 1  # This one decides the radius from the centroid topics to explore, to get resources, a bigger value may cause better recall performance, for example value 1 doesnt recall any resource with Tag 12
				RouterNode_num_centroid_tags_to_explore_per_period 2 # This parameter limits how many centroid topics around which exploration will be done per period 
				RouterNode_num_exploration_queries_to_submit_per_period 5 # This parameter limits how many queries the router node can submit for exploration
				RouterNode_delay 5
				RouterNode_response_timeout 30  # This param should be suffciently big to handle the delay in the network, else will kill listeners even before all response arrive, and throw "May be an error.."
				RouterNode_k 3 
				RouterNode_duplicate_message_cache_timeout 0
				RouterNode_max_description 5
				RouterNode_routing_table_max_length 8 
				RouterNode_routing_table_max_width 8

				UserNode_vitality 50 
				UserNode_extroversion 3  # This one decides how many routers nodes to bootstrap to
				UserNode_response_timeout  30 # This param should be sufficiently big to handle the delay in the network, else will kill listeners even before all response arrive, and throw "May be an error.."
				UserNode_num_queries_to_submit_per_period 5


				Birth_ResourceNode_start_time 0 
				Birth_ResourceNode_period 10 
				Birth_ResourceNode_total_num_cycles 4
				Birth_ResourceNode_num_cycles_per_heartbeat 4

				Birth_RouterNode_start_time 0 
				Birth_RouterNode_period 10 
				Birth_RouterNode_total_num_cycles 4 
				Birth_RouterNode_num_cycles_per_heartbeat 4

				Birth_UserNode_start_time 25
				Birth_UserNode_period 20 

				num_words 40 
				degree_of_associativity_of_words 4
				sw_model_probability 0.02

				input_onto_file_name ontoin
				output_onto_file_name ontoout.net
				output_onto_report_file_name ontorep
				output_network_file_name netout.net
				structural_report_file_name netstrucrep 
				generation_performance_report_file_name perfrep
				operation_performance_report_file_name oprnetperfrep
				routing_table_content_report_file_name netroutingtable

				DatabaseHistResolution 1
				DatabaseMaxHops 100
				DatabaseHistResolution_for_UserNode 1
				DatabaseMaxHops_for_UserNode 100

				trace_source_node_id 0
				trace_destination_node_id 3
				trace_tag 6
			*/

			std::cout<<" ** Parameters after reading from file **"<<std::endl<<std::endl;
		
			std::cout<<"network_struct_report_required = "<<param_ptr->network_struct_report_required<<std::endl<<std::endl;

			std::cout<<"snapshot_reporter_period = "<<param_ptr->snapshot_reporter_period<<std::endl;
			std::cout<<"snapshot_reporter_start_time = "<<param_ptr->snapshot_reporter_start_time<<std::endl;
			std::cout<<"oneshot_reporter_time = "<<param_ptr->oneshot_reporter_time<<std::endl<<std::endl;

			
			std::cout<<"run_for_ticks = "<<param_ptr->run_for_ticks<<std::endl<<std::endl;

			std::cout<<"bootstrap_threashold = "<<param_ptr->bootstrap_threashold<<std::endl;
			std::cout<<"message_time_to_live = "<<param_ptr->message_time_to_live<<std::endl;
			std::cout<<"query_time_to_live = "<<param_ptr->query_time_to_live<<std::endl<<std::endl;

			std::cout<<"ResourceNode_vitality = "<<param_ptr->ResourceNode_vitality<<std::endl;
			std::cout<<"ResourceNode_extroversion = "<<param_ptr->ResourceNode_extroversion<<std::endl;
			std::cout<<"ResourceNode_delay = "<<param_ptr->ResourceNode_delay<<std::endl;
			std::cout<<"ResourceNode_response_timeout = "<<param_ptr->ResourceNode_response_timeout<<std::endl;
			std::cout<<"ResourceNode_max_description = "<<param_ptr->ResourceNode_max_description<<std::endl<<std::endl;

			std::cout<<"ResourceNode_known_routers_floor = "<<param_ptr->ResourceNode_known_routers_floor<<std::endl;
			std::cout<<"ResourceNode_known_routers_trim_threshold = "<<param_ptr->ResourceNode_known_routers_trim_threshold<<std::endl;
			std::cout<<"ResourceNode_known_routers_capacity = "<<param_ptr->ResourceNode_known_routers_capacity<<std::endl<<std::endl;

			std::cout<<"ResourceNode_translation_radius = "<<param_ptr->ResourceNode_translation_radius<<std::endl<<std::endl;
		
			std::cout<<"RouterNode_vitality = "<<param_ptr->RouterNode_vitality<<std::endl;
			std::cout<<"RouterNode_extroversion = "<<param_ptr->RouterNode_extroversion<<std::endl;
			std::cout<<"RouterNode_delay = "<<param_ptr->RouterNode_delay<<std::endl;
			std::cout<<"RouterNode_response_timeout = "<<param_ptr->RouterNode_response_timeout<<std::endl<<std::endl;

			std::cout<<"RouterNode_num_centroid_tags_to_explore_per_period = "<<param_ptr->RouterNode_num_centroid_tags_to_explore_per_period<<std::endl<<std::endl;

			std::cout<<"RouterNode_num_exploration_queries_to_submit_per_period = "<<param_ptr->RouterNode_num_exploration_queries_to_submit_per_period<<std::endl;
			std::cout<<"RouterNode_k = "<<param_ptr->RouterNode_k<<std::endl<<std::endl;

			std::cout<<"RouterNode_duplicate_message_cache_timeout = "<<param_ptr->RouterNode_duplicate_message_cache_timeout<<std::endl;
			std::cout<<"RouterNode_max_description = "<<param_ptr->RouterNode_max_description<<std::endl<<std::endl;

			std::cout<<"RouterNode_known_routers_floor = "<<param_ptr->RouterNode_known_routers_floor<<std::endl;
			std::cout<<"RouterNode_known_routers_trim_threshold = "<<param_ptr->RouterNode_known_routers_trim_threshold<<std::endl;
			std::cout<<"RouterNode_known_routers_capacity = "<<param_ptr->RouterNode_known_routers_capacity<<std::endl<<std::endl;

			std::cout<<"RouterNode_routing_table_length_floor = "<<param_ptr->RouterNode_routing_table_length_floor<<std::endl;
			std::cout<<"RouterNode_routing_table_length_trim_threshold = "<<param_ptr->RouterNode_routing_table_length_trim_threshold<<std::endl;
			std::cout<<"RouterNode_routing_table_max_length = "<<param_ptr->RouterNode_routing_table_max_length<<std::endl<<std::endl;
			
			std::cout<<"RouterNode_routing_table_width_floor = "<<param_ptr->RouterNode_routing_table_width_floor<<std::endl;
			std::cout<<"RouterNode_routing_table_width_threshold = "<<param_ptr->RouterNode_routing_table_width_threshold<<std::endl;
			std::cout<<"RouterNode_routing_table_min_num_resource_nodes = "<<param_ptr->RouterNode_routing_table_min_num_resource_nodes<<std::endl<<std::endl;
			std::cout<<"RouterNode_routing_table_max_width = "<<param_ptr->RouterNode_routing_table_max_width<<std::endl<<std::endl;

			std::cout<<"UserNode_vitality = "<<param_ptr->UserNode_vitality<<std::endl;
			std::cout<<"UserNode_extroversion = "<<param_ptr->UserNode_extroversion<<std::endl;
			std::cout<<"UserNode_response_timeout = "<<param_ptr->UserNode_response_timeout<<std::endl;
			std::cout<<"UserNode_num_queries_to_submit_per_period = "<<param_ptr->UserNode_num_queries_to_submit_per_period<<std::endl;


			std::cout<<"Birth_ResourceNode_start_time = "<<param_ptr->Birth_ResourceNode_start_time<<std::endl;
			std::cout<<"Birth_ResourceNode_period = "<<param_ptr->Birth_ResourceNode_period<<std::endl;
			std::cout<<"Birth_ResourceNode_total_num_cycles = "<<param_ptr->Birth_ResourceNode_total_num_cycles<<std::endl;
			std::cout<<"Birth_ResourceNode_num_cycles_per_heartbeat = "<<param_ptr->Birth_ResourceNode_num_cycles_per_heartbeat<<std::endl;

			std::cout<<"Birth_RouterNode_start_time = "<<param_ptr->Birth_RouterNode_start_time<<std::endl;
			std::cout<<"Birth_RouterNode_period = "<<param_ptr->Birth_RouterNode_period<<std::endl;
			std::cout<<"Birth_RouterNode_total_num_cycles = "<<param_ptr->Birth_RouterNode_total_num_cycles<<std::endl;
			std::cout<<"Birth_RouterNode_num_cycles_per_heartbeat = "<<param_ptr->Birth_RouterNode_num_cycles_per_heartbeat<<std::endl;
			
			std::cout<<"Birth_UserNode_start_time = "<<param_ptr->Birth_UserNode_start_time<<std::endl;
			std::cout<<"Birth_UserNode_period = "<<param_ptr->Birth_UserNode_period<<std::endl;


			std::cout<<"num_words = "<<param_ptr->num_words<<std::endl;
			std::cout<<"degree_of_associativity_of_words = "<<param_ptr->degree_of_associativity_of_words<<std::endl;
			std::cout<<"sw_model_probability = "<<param_ptr->sw_model_probability<<std::endl;

			std::cout<<"input_onto_file_name = "<<param_ptr->input_onto_file_name<<std::endl;
			std::cout<<"output_onto_file_name = "<<param_ptr->output_onto_file_name<<std::endl;
			std::cout<<"output_onto_report_file_name ="<<param_ptr->output_onto_report_file_name<<std::endl;
			std::cout<<"output_network_file_name = "<<param_ptr->output_network_file_name<<std::endl;

			std::cout<<"structural_report_file_name = "<<param_ptr->structural_report_file_name<<std::endl;
			std::cout<<"generation_performance_report_file_name = "<<param_ptr->generation_performance_report_file_name<<std::endl;
			std::cout<<"operation_performance_report_file_name = "<<param_ptr->operation_performance_report_file_name<<std::endl;
			std::cout<<"routing_table_content_report_file_name ="<<param_ptr->routing_table_content_report_file_name<<std::endl;

			std::cout<<"DatabaseHistResolution = "<<param_ptr->DatabaseHistResolution<<std::endl;
			std::cout<<"DatabaseMaxHops ="<<param_ptr->DatabaseMaxHops<<std::endl;
			std::cout<<"DatabaseHistResolution_for_UserNode = "<<param_ptr->DatabaseHistResolution_for_UserNode<<std::endl;
			std::cout<<"DatabaseMaxHops_for_UserNode ="<<param_ptr->DatabaseMaxHops_for_UserNode<<std::endl;

			std::cout<<"trace_source_node_id = "<<param_ptr->trace_source_node_id<<std::endl;
			std::cout<<"trace_destination_node_id = "<<param_ptr->trace_destination_node_id<<std::endl;
			std::cout<<"trace_tag = "<<param_ptr->trace_tag<<std::endl;

			return;
};




void IO_Reporter::write_network_to_pajek_file(unsigned int num_vertices, unsigned int num_edges, const Edge *list_of_edges, 
	const unsigned char * weight, char *passed_output_file_name, Tick passed_current_time) {
				/* PAJEK DOES NOT LIKE UNIX FORMAT :: USE \r\n ONLY, DO NOT USE std::endl */	

			#ifdef DEBUG
				printf("called :write_network_to_pajek_file with num_vertices=%d num_edge=%d\n",num_vertices,num_edges);
			#endif	
			std::ofstream file;
			char filename[256];
			//snprintf(filename,27,"%u_%s",passed_current_time,passed_output_file_name); // old: no output dir
			snprintf(filename,256,"%s/%u_%s",param_ptr->output_dir,passed_current_time,passed_output_file_name);


			std::FILE * fileptr;
			fileptr=fopen(filename,"w");

			if (!fileptr)
			{
				printf("\nUnable to open file to write network : Aborting WRITE\n"); 
				return;
			}
			else{	
			#ifdef DEBUG 
				printf("\nStarting to write Vetex List\n");
			#endif	

			fprintf(fileptr,"*Vertices %d \r\n",num_vertices);
			for (int i=0;i<num_vertices;i++)
				fprintf(fileptr,"%d \r\n",i+1);

			#ifdef DEBUG 
				printf("\nFinished writing Vertices");
			#endif	

			#ifdef DEBUG 
				printf("\nStarting to write Arc List");
			#endif	
				
			fprintf(fileptr,"*Arcs \r\n");
			for (unsigned int i=0 ;i<num_edges;i++ ,list_of_edges++,weight++)
				{
					fprintf(fileptr,"%u  %u  %u \r\n",(((*list_of_edges).first)+1),(((*list_of_edges).second)+1),(*weight));

				}
			
			#ifdef DEBUG 
				printf("\nFinished writing arcs");
			#endif	
				 
				} //close of else


			fclose(fileptr);
 			#ifdef DEBUG 
				printf("\nFinished write_network_to_pajek_file\n");
			#endif	
			
			return;
};






void IO_Reporter::write_structural_report(unsigned int num_vertices, int diameter, const std::vector< std::vector<int> > &all_distances, 
		const std::vector< std::vector<Tag> > &all_parents, const unsigned int *distance_histogram, 
		const unsigned int *routing_table_length_histogram,
		const unsigned int *routing_table_width_histogram,
		const unsigned int *routing_table_max_width_histogram,
		char *passed_struct_report_file_name, Tick passed_current_time)
		{ 
			
			
			#ifdef DEBUG
				printf("called :write_structural_report with num_vertices = %d , diameter = %d\n",num_vertices,diameter);
			#endif	
			char filename[256];
			//snprintf(filename,30,"%ld_%s",passed_current_time,passed_struct_report_file_name); // old: no output dir
			snprintf(filename,256,"%s/%ld_%s",param_ptr->output_dir,passed_current_time,passed_struct_report_file_name);
		
			std::FILE * fileptr;
			fileptr=fopen(filename,"w");
			
			if (!fileptr)
			{	
				printf("\nUnable to open file to write network : Aborting WRITE\n"); 
				return;
			}
			else{	

					std::vector< std::vector<int> >::iterator row_it;
					 
					std::vector< std::vector<int> >::iterator row_end;
					 
					std::vector<int>::iterator col_it;
					std::vector<int>::iterator col_end;
					
					//int i,j;



					if (routing_table_length_histogram !=NULL)
					{
						#ifdef DEBUG
							printf("Starting to write routing table length histogram \n");
						#endif	

						fprintf(fileptr,"***Routing table length histogram**\nLength : Freq \n");
										
						for(unsigned int j=0;j<If_ptr->param_ptr->RouterNode_routing_table_max_length+1;j++)
								if (routing_table_length_histogram[j])
									 fprintf(fileptr,"%5u  : %5u \n",j,routing_table_length_histogram[j]); 
								
						fprintf(fileptr,"\n");    
							
					}

					if (routing_table_width_histogram !=NULL)
					{
						#ifdef DEBUG
							printf("Starting to write routing table width histogram \n");
						#endif	

						fprintf(fileptr,"***Routing table width histogram**\nWidth : Freq \n");
										
						for(unsigned int j=0;j<If_ptr->param_ptr->RouterNode_routing_table_max_width+1;j++)
								if (routing_table_width_histogram[j])
									 fprintf(fileptr,"%5u : %5u \n",j,routing_table_width_histogram[j]); 
								
						fprintf(fileptr,"\n");    
							
					}

					if (routing_table_max_width_histogram !=NULL)
					{
						#ifdef DEBUG
							printf("Starting to write routing table max width histogram \n");
						#endif	

						fprintf(fileptr,"***Routing table max width histogram**\nMax width : Num of pairs \n");
										
						for(unsigned int j=0;j<If_ptr->param_ptr->RouterNode_routing_table_max_width+1;j++)
								if (routing_table_max_width_histogram[j])
									 fprintf(fileptr,"%5u     : %5u \n",j,routing_table_max_width_histogram[j]); 
								
						fprintf(fileptr,"\n");    
							
					}

					//Is structural report is noot required do not write the following rest of the items
					if (If_ptr->param_ptr->network_struct_report_required) {

							if (distance_histogram !=NULL)
							{
								#ifdef DEBUG
									printf("Starting to write distance histogram \n");
								#endif	

								fprintf(fileptr,"***Distance distribution/histogram**\nDistance : Num of pairs \n");
												
								for(int j=0;j<diameter+1;j++)
										if (distance_histogram[j])
											 fprintf(fileptr,"%5u    : %5u \n",j,distance_histogram[j]); 
										
								fprintf(fileptr,"\n");    
									
							}


					#ifdef DEBUG
							printf("Writing Num Vertices \n");
					#endif	
							fprintf(fileptr,"num_vertices = %d \n",num_vertices);
							fprintf(fileptr,"Network Diameter is %d \n",diameter);

					
					#ifdef DEBUG
						   printf("Writing Distance Vectors \n");
					#endif	

							fprintf(fileptr,"***Distance matrix**\n");

							//fprintf(fileptr,"Row-1 and Col-1 are vertex number\n");
							fprintf(fileptr,"#            ");
							for (unsigned int i=0;i<num_vertices;i++)
										fprintf(fileptr,"Nd/Tg%5u : ",i+1); //print all the column headers.
							fprintf(fileptr,"\n");

							for (unsigned int i=0;i<num_vertices;i++)
								{
									fprintf(fileptr,"Nd/Tg%4u :",i+1); //print the vertex-no for that row.
									for(unsigned int j=0;j<num_vertices;j++)
										fprintf(fileptr,"    %5u    ",(all_distances[i][j]));
									fprintf(fileptr,"\n"); 
								
								}
							fprintf(fileptr,"\n\n"); 
							#ifdef DEBUG
								printf("Done writing distance matrix \n");
							#endif	

							#ifdef DEBUG
								printf("Starting to write Parent matrix \n");
							#endif	
							
							fprintf(fileptr,"***Parents**\n");
							fprintf(fileptr,"#            ");
							for (unsigned int i=0;i<num_vertices;i++)
										fprintf(fileptr,"Nd/Tg%4u : ",i); //print all the column headers.
							fprintf(fileptr,"\n");

							for (unsigned int i=0;i<num_vertices;i++)
								{
									fprintf(fileptr,"Nd/Tg %5u :",i); //print the vertex-no for that row.
											
									for(unsigned int j=0;j<num_vertices;j++)
										fprintf(fileptr,"    %5u   ",all_parents[i][j]);
								 fprintf(fileptr,"\n");
								
								}
							fprintf(fileptr,"\n\n"); 

							#ifdef DEBUG
								printf("Done writing Parent matrix \n");
							#endif	
								

							#ifdef DEBUG
								printf("End of structural report writer\n");
							#endif	

					}//if (our_Infrastructure.param_ptr->network_struct_report_required)

			 }//close of else, for file opening verifcations

			fclose(fileptr);

			return;
			
};



/**/

void IO_Reporter::write_performance_report(char *passed_perf_report_file_name , Tick current_time){

	//This would print all the database counters to a file, with prefix as time, and suffix as passed_perf_report_file_name
	//similar to IO_Reporter::write_structural_report(....) 


	//Suneil : Added:July-21st :1:45pm 
	unsigned int sum_num_resources, sum_num_good_responses,sum_num_queries ;
	float recall_rate_per_query;
	char filename[256];
	//snprintf(filename,30,"%ld_%s",current_time,passed_perf_report_file_name); // old: no output dir
	snprintf(filename,256,"%s/%ld_%s",param_ptr->output_dir,current_time,passed_perf_report_file_name);

	std::FILE * fileptr;
	fileptr=fopen(filename,"w");	
	if (!fileptr)
	{	
		printf("\nUnable to open file to write network : Aborting WRITE\n");
		return;
	}

	else{
			
			fprintf(fileptr,"***Database Report at time %u  ticks *** \n\n The following figures are for the message packets, not unique user level queries \n\n",current_time);
				
			fprintf(fileptr,"Queries_sent_from_ResourceNode : %u \n",If_ptr->database_ptr->Queries_sent_from_ResourceNode);
			fprintf(fileptr,"Queries_sent_from_RouterNode : %u \n\n",If_ptr->database_ptr->Queries_sent_from_RouterNode);

			fprintf(fileptr,"Queries_forwarded_by_RouterNode_orgininated_from_ResourceNode : %u \n",If_ptr->database_ptr->Queries_forwarded_by_RouterNode_orgininated_from_ResourceNode);
			fprintf(fileptr,"Queries_forwarded_by_RouterNode_orgininated_from_RouterNode : %u \n\n",If_ptr->database_ptr->Queries_forwarded_by_RouterNode_orgininated_from_RouterNode); 
					
			fprintf(fileptr,"Responses_sent_from_ResourceNode : %u \n",If_ptr->database_ptr->Responses_sent_from_ResourceNode);
			fprintf(fileptr,"Responses_sent_from_RouterNode : %u \n\n",If_ptr->database_ptr->Responses_sent_from_RouterNode); 

			fprintf(fileptr,"Num_of_good_responses : %u   # These are the response received from router nodes only, as resources/ routers are interested to identify router to whom they can attach to \n\n",If_ptr->database_ptr->Num_of_good_responses); 

			fprintf(fileptr,"Num_duplicate_message_dropped : %u \n",If_ptr->database_ptr->Num_duplicate_message_dropped);
			fprintf(fileptr,"Num_looping_message_dropped : %u \n",If_ptr->database_ptr->Num_looping_message_dropped);
			fprintf(fileptr,"Num_queries_dropped_for_TTL  : %u \n\n",If_ptr->database_ptr->Num_queries_dropped_for_TTL);;
	
				fprintf(fileptr,"Num_reponses_dropped_when_no_active_listeners  : %u \n\n",If_ptr->database_ptr->Num_reponses_dropped_when_no_active_listeners);;
	
			fprintf(fileptr,"Num_total_message_generated (due to internal network ops - query, forwarding, response) : %u \n\n",If_ptr->database_ptr->Num_total_message_generated);			

			fprintf(fileptr,"Queries_sent_from_UserNode  (query packets) : %u\n",If_ptr->database_ptr->Queries_sent_from_UserNode);
			fprintf(fileptr,"Num_responses_sent_for_UserNode : %u\n",If_ptr->database_ptr->Num_responses_sent_for_UserNode);
			fprintf(fileptr,"Num_of_good_responses_for_UserNode : %u\n",If_ptr->database_ptr->Num_of_good_responses_for_UserNode);
			fprintf(fileptr,"Num_duplicate_message_dropped_for_UserNode : %u\n", If_ptr->database_ptr->Num_duplicate_message_dropped_for_UserNode);
			fprintf(fileptr,"Num_looping_message_dropped_for_UserNode : %u\n", If_ptr->database_ptr->Num_looping_message_dropped_for_UserNode);
			fprintf(fileptr,"Num_queries_dropped_for_TTL_for_UserNode  : %u\n", If_ptr->database_ptr->Num_queries_dropped_for_TTL_for_UserNode);
			fprintf(fileptr,"Num_responses_dropped_for_TTL_for_UserNode : %u\n\n",If_ptr->database_ptr->Num_responses_dropped_for_TTL_for_UserNode);
			
			fprintf(fileptr,"Num_total_message_generated_for_UserNode (query, forwarding, response) : %u \n\n",If_ptr->database_ptr->Num_total_message_generated_for_UserNode);
					
			//sm24
			fprintf(fileptr,"Num_total_queries_generated (rtr, res, user): %u  # This is total of all resource, router, user nodes\n",If_ptr->database_ptr->Num_total_queries_generated);
			fprintf(fileptr,"Num_total_responses_generated (rtr, res, user): %u  # This is total of all resource, router, user nodes \n\n",If_ptr->database_ptr->Num_total_responses_generated);
			
			fprintf (fileptr,"Analysis of Query-Response statistics across all tags for internal network operations (advertising operations only) :\nContents of num_resource_with_specific_tag \n");
			fprintf (fileptr,"Tag : res: rtr: queries: good responses from routers\n");


			sum_num_resources=0,sum_num_good_responses=0,sum_num_queries=0;
			for(unsigned int i=0; i<(If_ptr->param_ptr->num_words);i++)
			{
				if (If_ptr->database_ptr->num_resource_with_specific_tag[i] + If_ptr->database_ptr->num_router_with_specific_tag[i] )
				{
					fprintf(fileptr,"%4d:%4u:%4u:%8u:%5u \n",
						i,
						If_ptr->database_ptr->num_resource_with_specific_tag[i], 
						If_ptr->database_ptr->num_router_with_specific_tag[i], 
						If_ptr->database_ptr->Num_of_queries_for_specific_tag[i],
						If_ptr->database_ptr->Num_of_good_responses_for_specific_tag[i]);
				}
				if ( (If_ptr->database_ptr->num_resource_with_specific_tag[i] + If_ptr->database_ptr->num_router_with_specific_tag[i] )>=2 &&
						If_ptr->database_ptr->num_router_with_specific_tag[i] > 0	)
				{
					sum_num_resources = sum_num_resources + If_ptr->database_ptr->num_router_with_specific_tag[i] ;
					sum_num_good_responses = sum_num_good_responses + If_ptr->database_ptr->Num_of_good_responses_for_specific_tag[i];
					sum_num_queries = sum_num_queries + If_ptr->database_ptr->Num_of_queries_for_specific_tag[i] *
								(If_ptr->database_ptr->num_resource_with_specific_tag[i] + If_ptr->database_ptr->num_router_with_specific_tag[i] );

				}
			}


			fprintf(fileptr,"\nValues for recall rate calculation for network internal operation (advertising operations only) \nsum_num_good_responses = %u , sum_num_resources = %u , sum_num_queries = %u , RouterNode_k = %u \n",
				sum_num_good_responses,
				sum_num_resources,
				sum_num_queries,
				If_ptr->param_ptr->RouterNode_k);
			
			if (sum_num_queries)
			{
				recall_rate_per_query = ((float) sum_num_good_responses);
				recall_rate_per_query = recall_rate_per_query /sum_num_queries ;


				fprintf(fileptr,"Recall rate per query for internal network operations = %.4f \n\n",recall_rate_per_query);

			} else { 
					fprintf(fileptr,"Recall rate per query not yet available \n\n");
					}
					

			fprintf(fileptr,"Analysis of Query-Response time response for network's  internal operation: Contents of hops_before_response_ctr \n");
			fprintf(fileptr,"Parameters used to generate the histogram : DatabaseHistResolution : %u, DatabaseMaxHops : %u ,num_buckets : %u \n", 
				If_ptr->param_ptr->DatabaseHistResolution,
				If_ptr->param_ptr->DatabaseMaxHops,
				If_ptr->database_ptr->num_buckets);

			fprintf(fileptr,"Histogram contents - \nHops :Freq \n");
			
			for(int i=0;i<(If_ptr->database_ptr->num_buckets +1);i++)
				if(If_ptr->database_ptr->hops_before_response_ctr[i]) 
					 fprintf(fileptr,"%5d:%5u\n",
						i,
						If_ptr->database_ptr->hops_before_response_ctr[i] );
			
			fprintf(fileptr,"\nAnalysis of duplicate packet's longevity (hops before they were identified as dup packets and dropped) \nfor network's  internal operation: Contents of longevity_of_message_ctr_for_TTL\n");
			fprintf(fileptr,"Parameters used to generate the histogram : DatabaseHistResolution : %u , DatabaseMaxHops : %u , num_buckets : %u \n", 
				If_ptr->param_ptr->DatabaseHistResolution,
				If_ptr->param_ptr->DatabaseMaxHops,
				If_ptr->database_ptr->num_buckets);

			fprintf(fileptr,"Histogram contents - \nHops :Freq \n");
			
			for(int i=0;i<(If_ptr->database_ptr->num_buckets +1);i++)
				if(If_ptr->database_ptr->longevity_of_message_ctr_for_TTL[i])
					 fprintf(fileptr,"%5d:%5u\n",
						i,
						If_ptr->database_ptr->longevity_of_message_ctr_for_TTL[i] );
			
			fprintf(fileptr,"\nAnalysis of Query-Response time response for user queries: Contents of hops_before_response_ctr_for_UserNode \n");
			fprintf(fileptr,"Parameters used to generate the histogram : DatabaseHistResolution_for_UserNode : %u , DatabaseMaxHops_for_UserNode : %u , num_buckets_for_UserNode : %u \n",
				If_ptr->param_ptr->DatabaseHistResolution_for_UserNode,
				If_ptr->param_ptr->DatabaseMaxHops_for_UserNode,
				If_ptr->database_ptr->num_buckets_for_UserNode);

			fprintf(fileptr,"Histogram contents - \nHops :Freq \n");
			
			for(int i=0;i<(If_ptr->database_ptr->num_buckets_for_UserNode +1);i++)
				if(If_ptr->database_ptr->hops_before_response_ctr_for_UserNode[i])	
					fprintf(fileptr,"%5d:%5u\n",
						i,
						If_ptr->database_ptr->hops_before_response_ctr_for_UserNode[i] );
			
		
			fprintf(fileptr,"\nAnalysis of duplicate packet's longevity (hops before they were identified as dup packets and dropped) \n");
			fprintf(fileptr,"for user queries: Contents of hops_before_response_ctr_for_UserNode \n");
			fprintf(fileptr,"Parameters used to generate the histogram : \nDatabaseHistResolution_for_UserNode : %u, DatabaseMaxHops_for_UserNode : %u, num_buckets_for_UserNode : %u\n",
				If_ptr->param_ptr->DatabaseHistResolution_for_UserNode,
				If_ptr->param_ptr->DatabaseMaxHops_for_UserNode,
				If_ptr->database_ptr->num_buckets_for_UserNode);

			fprintf(fileptr,"Histogram contents - \nHops :Freq \n");
			
			for(int i=0;i<(If_ptr->database_ptr->num_buckets_for_UserNode +1);i++)
				if(If_ptr->database_ptr->longevity_of_message_ctr_for_TTL_for_UserNode[i])	
				    fprintf(fileptr,"%5d:%5u\n",
						i,
						If_ptr->database_ptr->longevity_of_message_ctr_for_TTL_for_UserNode[i]);
			
			fprintf(fileptr,"\nAnalysis of Query-Response statistics for user queries across all tags for which a resource exists :\n");
			fprintf(fileptr,"Contents of Num_of_good_responses_for_specific_tag_for_UserNode \n");
			fprintf(fileptr,"Tag  : res : queries: responses \n");
			
			sum_num_resources =0,sum_num_good_responses=0,sum_num_queries=0;
			for(unsigned int i=0; i<(If_ptr->param_ptr->num_words);i++)
			{
				if (If_ptr->database_ptr->Num_of_queries_for_specific_tag_from_UserNode[i] && 
								If_ptr->database_ptr->num_resource_with_specific_tag[i])
				{
					fprintf(fileptr,"%5d:%5u:%8u:%5u\n",
						i,
						If_ptr->database_ptr->num_resource_with_specific_tag[i],
						If_ptr->database_ptr->Num_of_queries_for_specific_tag_from_UserNode[i],
						If_ptr->database_ptr->Num_of_good_responses_for_specific_tag_for_UserNode[i]);
					
					sum_num_resources = sum_num_resources + If_ptr->database_ptr->num_resource_with_specific_tag[i];
					sum_num_good_responses = sum_num_good_responses + If_ptr->database_ptr->Num_of_good_responses_for_specific_tag_for_UserNode[i];
					sum_num_queries = sum_num_queries + If_ptr->database_ptr->Num_of_queries_for_specific_tag_from_UserNode[i] *
											If_ptr->database_ptr->num_resource_with_specific_tag[i] ;

				}
			}


			fprintf(fileptr,"\nValues for recall rate calculation for user queries \nsum_num_good_responses = %u , sum_num_resources = %u, sum_num_queries = %u, RouterNode_k= %u \n",
			sum_num_good_responses,
			sum_num_resources,
			sum_num_queries,
			If_ptr->param_ptr->RouterNode_k);
			
			if (sum_num_queries)
			{
				
				recall_rate_per_query = ((float) sum_num_good_responses);
				recall_rate_per_query = recall_rate_per_query /sum_num_queries ;

				fprintf(fileptr,"Recall rate per query for user queries = %f \n\n",recall_rate_per_query);
			} else fprintf(fileptr,"Recall rate per query for external queries not yet available \n\n");

		
 			fprintf(fileptr,"Analysis of Query-Response statistics across all tags for user queries :\n");
			fprintf(fileptr,"Contents of Num_of_good_responses_for_specific_tag_for_UserNode\n");
			fprintf(fileptr,"Tag  : res : queries: responses \n");

			sum_num_good_responses=0,sum_num_queries=0;
			for(unsigned int i=0;i<(If_ptr->param_ptr->num_words);i++)
			{
				if (If_ptr->database_ptr->Num_of_queries_for_specific_tag_from_UserNode[i] )
				{
 					fprintf(fileptr,"%5d:%5u:%8u:%5u\n",i,If_ptr->database_ptr->num_resource_with_specific_tag[i],
					If_ptr->database_ptr->Num_of_queries_for_specific_tag_from_UserNode[i],
					If_ptr->database_ptr->Num_of_good_responses_for_specific_tag_for_UserNode[i]);

					sum_num_good_responses = sum_num_good_responses + If_ptr->database_ptr->Num_of_good_responses_for_specific_tag_for_UserNode[i];
					sum_num_queries = sum_num_queries + If_ptr->database_ptr->Num_of_queries_for_specific_tag_from_UserNode[i];

				}
			}

 			fprintf(fileptr,"\nValues for hit/success rate calculation for user queries \nsum_num_good_responses = %u , sum_num_queries = %u \n",
				sum_num_good_responses, sum_num_queries);
			
			if (sum_num_queries)
			{
				
				recall_rate_per_query = ((float) sum_num_good_responses);
				recall_rate_per_query = recall_rate_per_query /sum_num_queries ;

 				fprintf(fileptr,"Hit rate per query for user queries = %.4f \n\n",recall_rate_per_query);

			} else	fprintf(fileptr,"Hit rate per query for external queries not yet available \n\n");
					

		}//close of else
	//file.close();
	fclose(fileptr);
	return;
};

/**/


void IO_Reporter::write_routing_table_content_report(char * passed_routing_table_content_report_file_name, Tick passed_current_time) 
	{

				RoutingTableIterator print_routing_table_row_itr;
				RoutingTableRowIterator print_routing_table_entry_itr; 
				std::vector<RouterNode *>::iterator list_of_router_nodes_itr;
				//unsigned int i;

				char fname[256];
				//snprintf(fname,30,"%d_%s",passed_current_time,passed_routing_table_content_report_file_name); // old: no output dir
				snprintf(fname,256,"%s/%d_%s",param_ptr->output_dir,passed_current_time,passed_routing_table_content_report_file_name);
 				std::FILE * fileptr;
				fileptr=fopen(fname,"w");
				if(!fileptr)
					{
						printf("\nError: Unable to open file to write routing table contents : Aborting WRITE\n");
						return;
					}
				else
					{	
#ifdef DEBUG
							printf("Writing Routing Table Entry\n");
#endif	
							
							//The code below will print the routing table in a file with the passed given name

							list_of_router_nodes_itr = If_ptr->list_of_router_nodes.begin();
							//while ( list_of_router_nodes_itr != If_ptr->list_of_router_nodes.end() )
							for (unsigned int m= (If_ptr->list_of_router_nodes).size(); 
								m>0; m--) 
							{

								if (!(*list_of_router_nodes_itr)->routing_table.empty())
								{

									print_routing_table_row_itr = (*list_of_router_nodes_itr)->routing_table.begin();

 									fprintf(fileptr,"Printing routing table of router node: %u \n",(*list_of_router_nodes_itr)->id);
 									fprintf(fileptr,"Node id  : Row i   :Tag     : RsNdNum :List of Node id(node_type) that are attached...\n");
									//i=0;
									//while (print_routing_table_row_itr != (*list_of_router_nodes_itr)->routing_table.end())
									for (unsigned int j= (*list_of_router_nodes_itr)->routing_table.size(),i=0; 
										j>0; j--,i++)
									{
 										fprintf(fileptr,"Node%5u: Row %4u:Tag %4u:  %4u   :",
											(*list_of_router_nodes_itr)->id,i,(*print_routing_table_row_itr).first,(*print_routing_table_row_itr).second->num_resource_nodes );

										print_routing_table_entry_itr = (*print_routing_table_row_itr).second->destinations.begin() ;

										//while ( print_routing_table_entry_itr != (*print_routing_table_row_itr).second.end()  )
										for (unsigned int k= ((*print_routing_table_row_itr).second)->destinations.size(); 
											k>0; k--)
										{
 											fprintf(fileptr," %u(%u), ",(*print_routing_table_entry_itr)->id,(*print_routing_table_entry_itr)->node_type);
											print_routing_table_entry_itr++;
										}
 										fprintf(fileptr,"\n");
										print_routing_table_row_itr++;
										//i++;
									}

 								} else  
										fprintf(fileptr,"Routing table of router node %u is empty \n",(*list_of_router_nodes_itr)->id);
 
								list_of_router_nodes_itr++;

								fprintf(fileptr,"\n"); //Prints a line after each node's routing table content

							}
				
			   }//close of else	
 			fclose(fileptr);

			return;
	};







void IO_Reporter::schedule_periodic(Tick passed_period, Tick passed_start_time)
{
	
	period = passed_period;
	start_time = passed_start_time;


	//Schedule itself
	//Create an event
	handler_schedule_event_ptr = new Event;

#ifdef DEBUG
	handler_schedule_event_ptr->event_id = If_ptr->event_id;
	If_ptr->event_id++;
#endif

	//Create a functor, assign the object and its handling method corresponding to the event
	functor_handler_ptr = new SpecificFunctor<IO_Reporter>( this, &IO_Reporter::heartbeat );
							
	handler_schedule_event_ptr->handler= functor_handler_ptr;
	//Setup the periodicty
	handler_schedule_event_ptr->time = period;
	handler_schedule_event_ptr->start_time = start_time;
	//Schedule the periodic event
	If_ptr->scheduler_ptr->schedule_periodic( handler_schedule_event_ptr); 

	return;
};


void IO_Reporter::schedule_event( Tick passed_oneshot_time)
{
	
	//Schedule itself
	//Create an event
	handler_schedule_event_ptr = new Event;

#ifdef DEBUG
	handler_schedule_event_ptr->event_id = If_ptr->event_id;
	If_ptr->event_id++;
#endif

	//Create a functor, assign the object and its handling method corresponding to the event
	functor_handler_ptr = new SpecificFunctor<IO_Reporter>( this, &IO_Reporter::heartbeat );
							
	handler_schedule_event_ptr->handler= functor_handler_ptr;
	//Setup the periodicty
	handler_schedule_event_ptr->time = passed_oneshot_time;
	//Schedule the periodic event
	If_ptr->scheduler_ptr->schedule_event( handler_schedule_event_ptr); 

#ifdef DEBUG_IO
	printf( " Created EventReporter: with passed_time = %u \n", passed_oneshot_time );
#endif
	return;
};




void IO_Reporter::heartbeat()
{
	
	printf("Reporting network status at time %u\n", If_ptr->scheduler_ptr->now() );




	//Extract the network to make edge list , which will be used for shortest distance generation
	(If_ptr->analyst).extract_edge_list();



	if (operational_phase & GENERATION_PHASE)
	{

			#ifdef DEBUG_IO
					printf("Analyst::heartbeat(): Wiriting edge_list....\n");

				write_edge_list();

			#endif

			//Dont generate pajek file if edge list is not generated in the first place.
			if (If_ptr->param_ptr->network_struct_report_required){

				printf("Dumping the network to pajek file...\n");

				write_network_to_pajek_file( (If_ptr->analyst).num_vertices,(If_ptr->analyst).num_edges,
								(If_ptr->analyst).list_of_edges, 
								(If_ptr->analyst).weight, param_ptr->output_network_file_name, If_ptr->scheduler_ptr->now() ); 

				printf("Finished dumpting the network in file : %u_%s\n",If_ptr->scheduler_ptr->now(), param_ptr->output_network_file_name);
			}

				printf("Analysing the generated network...\n");

				//Generating distance matrix and network diameter
			if (If_ptr->param_ptr->network_struct_report_required)
						printf("The diameter of the generated network is %d\n", (If_ptr->analyst).generate_distance_matrix() );


			#ifdef DEBUG_IO
				printf("IO_Reporter::heartbeat():Writing edge_list....\n");

				write_edge_list();

			#endif

				printf("Dumping the network structural report to file...\n");


				write_structural_report( (If_ptr->analyst).num_vertices,
					(If_ptr->analyst).diameter, 
					(If_ptr->analyst).all_distances, (If_ptr->analyst).all_parents,
					(If_ptr->analyst).distance_histogram,
					(If_ptr->analyst).routing_table_length_histogram,
					(If_ptr->analyst).routing_table_width_histogram,
					(If_ptr->analyst).routing_table_max_width_histogram,
					param_ptr->structural_report_file_name, If_ptr->scheduler_ptr->now() );



				printf("Finished dumping the network structural report to file : %u_%s\n", If_ptr->scheduler_ptr->now(), param_ptr->structural_report_file_name);

				printf("Dumping the routing table contents to file...\n");

				write_routing_table_content_report(param_ptr->routing_table_content_report_file_name, If_ptr->scheduler_ptr->now());

				printf("Finished dumping routing table contents to file : %u_%s\n", If_ptr->scheduler_ptr->now(), param_ptr->routing_table_content_report_file_name);

    			printf("Dumping the network generation performance to file...\n");

				write_performance_report(param_ptr->generation_performance_report_file_name, If_ptr->scheduler_ptr->now());

				printf("Finished dumping the network generation performance to file : %u_%s\n", If_ptr->scheduler_ptr->now(), param_ptr->generation_performance_report_file_name);




			#ifdef DEBUG_FAULT
				//print_all_nodes();
			#endif
	}
/*
	if (operational_phase & OPERATION_PHASE)
	{

		printf("Dumping the network operation performance to file...\n");

		write_performance_report(param_ptr->operation_performance_report_file_name, If_ptr->scheduler_ptr->now());

		printf("Finished dumping the network operation performance to file : %u_%s\n", If_ptr->scheduler_ptr->now(), param_ptr->operation_performance_report_file_name);

	}
*/
	// Option B live visualization: emit one JSON line per heartbeat to stderr.
	// Pipe with:  ./simulation config/config.dat 2>&1 1>/dev/null | python3 viz.py
	// or:         ./simulation config/config.dat 2>viz_stream.jsonl &  tail -f viz_stream.jsonl | python3 viz.py
	{
		unsigned int q  = If_ptr->database_ptr->Queries_sent_from_UserNode;
		unsigned int gr = If_ptr->database_ptr->Num_of_good_responses_for_UserNode;
		unsigned int tm = If_ptr->database_ptr->Num_total_message_generated_for_UserNode;
		float recall   = q ? 100.0f * (float)gr / (float)q : 0.0f;
		float overhead = q ? (float)tm / (float)q           : 0.0f;
		fprintf(stderr,
			"{\"t\":%u,\"nodes\":{\"res\":%u,\"rtr\":%u,\"usr\":%u},"
			"\"queries\":%u,\"good_resp\":%u,\"total_msg\":%u,"
			"\"dupes_dropped\":%u,\"loop_dropped\":%u,\"ttl_dropped\":%u,"
			"\"diameter\":%d,\"recall_pct\":%.1f,\"overhead\":%.2f}\n",
			If_ptr->scheduler_ptr->now(),
			(unsigned int)If_ptr->list_of_resource_nodes.size(),
			(unsigned int)If_ptr->list_of_router_nodes.size(),
			(unsigned int)If_ptr->list_of_user_nodes.size(),
			q, gr, tm,
			If_ptr->database_ptr->Num_duplicate_message_dropped,
			If_ptr->database_ptr->Num_looping_message_dropped,
			If_ptr->database_ptr->Num_queries_dropped_for_TTL,
			If_ptr->analyst.diameter,
			recall, overhead);
	}

	return;
};


#ifdef DEBUG

void IO_Reporter::write_edge_list() {

	for (unsigned int i = 0;i < (If_ptr->analyst).num_edges ;i++ )
	{
		printf( "%u : %u -> %u : %d\n",i,((If_ptr->analyst).list_of_edges[i]).first,
					((If_ptr->analyst).list_of_edges[i]).second, (If_ptr->analyst).weight[i] );

	}
	printf("Finished wrtiting edge_list....\n");

	return;
};


void IO_Reporter::print_all_nodes() {
		
		std::vector<ResourceNode *>::iterator list_of_resource_nodes_itr;
		std::vector<RouterNode *>::iterator list_of_router_nodes_itr;
		ResourceNode * resource_node_ptr;
		RouterNode * router_node_ptr;

		unsigned int num_router_nodes = If_ptr->list_of_router_nodes.size();
		unsigned int num_resource_nodes = If_ptr->list_of_resource_nodes.size();

		//unsigned int i;
 			printf("Printing all nodes....\n");

 			printf("RouterNodes : ");

		list_of_router_nodes_itr = If_ptr->list_of_router_nodes.begin();
		for (unsigned int i=0; i < num_router_nodes ;i++, list_of_router_nodes_itr++ ) 
		{
 			printf(" %u,", (*list_of_router_nodes_itr)->id);
 
		}
 			
			printf("\n");
		
			printf("ResourceNodes : ");

		list_of_resource_nodes_itr = If_ptr->list_of_resource_nodes.begin();
		for (unsigned int i=0 ;
					i<num_resource_nodes ;i++, list_of_resource_nodes_itr++ )
		{
			printf(" %u,",  (*list_of_resource_nodes_itr)->id);
 
		}	
			printf("\n");

	return;
};

#endif




