
#include <algorithm>

//For onto
#include <utility>                        
#include <vector>						  
#include <list>
#include <map>

#include <cstdlib> //For random generator
#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <boost/graph/edge_list.hpp>
#include <boost/graph/named_function_params.hpp>

#include <boost/config.hpp>
#include <boost/utility.hpp>             // for boost::tie

#include <boost/graph/graph_traits.hpp>  // for boost::graph_traits
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/small_world_generator.hpp>
#include <boost/random/linear_congruential.hpp>

#include <boost/graph/edge_list.hpp>

//For std::numeric_limits<short>::max() & memset()
#include <limits>

#include "ForwardDeclaration.hpp"//Forward declaration to get rid of circular dependency
#include "Node.hpp"
#include "ProxyAgent.hpp"


#include "SmallWorldGenerator.hpp"

#include "OntologyRepository.hpp"
#include "OperationSimulator.hpp"
#include "IO_Reporter.hpp"
#include "Scheduler.hpp"

#include "Infrastructure.hpp"
#include "Input.hpp" //This contains the list of input parameters
#include "Functor.hpp"
#include "Listener.hpp"
#include "Message.hpp"
#include "Event.hpp"
#include "Database.hpp"
#include "Analyst.hpp"


//Suneil's code
unsigned int get_dist_mat(SmallWorldGraph *our_SmallWorldGraph);
std::vector< std::vector< int > > swg_all_distances;
std::vector< std::vector< Tag > > swg_all_parents;
//Suneil's code

//Infrastructure *g_If_ptr;

int main(int argc, char *argv[])
{
	//Description dummy_description;

	//BootstrapNodeList dummy_bootstrapNodeList;

	unsigned int ontology_network_diameter;

	//char input_file_name[20]; // too small for paths like logs/<name>/run.dat
	char input_file_name[256];
	unsigned int num_vertices;
	//Create infrastructure
	Infrastructure our_Infrastructure;

	our_Infrastructure.analyst.If_ptr = &our_Infrastructure;

	printf("Created Infrastructure for simulation...\n");
    
	if(argc<2)
    {
		printf("Provide the config input file name : ");
		scanf("%s", input_file_name);
    }
	else 
		strcpy(input_file_name,argv[1]);
 

	//Read all parameters from a file/ console
	our_Infrastructure.IO_Reporter_ptr->read_input_parameter(input_file_name, &our_Infrastructure );

	// Database was constructed before params were read; reinit re-allocates its
	// param-dependent arrays with the actual (post-parse) values.
	our_Infrastructure.database_ptr->reinit();

	printf("\nThe input parameters read from file :%s are below \n", input_file_name);

	our_Infrastructure.IO_Reporter_ptr->write_input_parameter_on_screen_from(input_file_name );

	num_vertices =	our_Infrastructure.param_ptr->Birth_ResourceNode_total_num_cycles + 
						our_Infrastructure.param_ptr->Birth_RouterNode_total_num_cycles;

	
	/*
	struct provisioned_capacity
	{
		unsigned int edge_list;
	};
	*/
	if (our_Infrastructure.param_ptr->network_struct_report_required)
 			our_Infrastructure.provisioned_capacity.edge_list = num_vertices *  
					our_Infrastructure.param_ptr->RouterNode_routing_table_max_width * 
						our_Infrastructure.param_ptr->RouterNode_routing_table_max_length; //Need to fix this patch thing with proper expression
 	
	else our_Infrastructure.provisioned_capacity.edge_list = 2;

	//Provisioning the Analyst
 
		our_Infrastructure.analyst.list_of_edges= new Edge [our_Infrastructure.provisioned_capacity.edge_list];
		 if (!our_Infrastructure.analyst.list_of_edges)	
		{
			 cout << "simulation: Error: memory could not be allocated for Analyst::list_of_edges"; 
			 exit(1);
		}

		our_Infrastructure.analyst.weight = new u_8 [our_Infrastructure.provisioned_capacity.edge_list];
		 if (!our_Infrastructure.analyst.weight)	
		{
			 cout << "simulation: Error: memory could not be allocated for Analyst::weight"; 
			 exit(1);
		}
	
		if (!our_Infrastructure.param_ptr->network_struct_report_required) {

			//memset(our_Infrastructure.analyst.list_of_edges, 0, our_Infrastructure.provisioned_capacity.edge_list * 4); // BUG: hardcoded *4 wrong when sizeof(Edge)!=4 or sizeof(u_8)!=4
			//memset(our_Infrastructure.analyst.weight, 0, our_Infrastructure.provisioned_capacity.edge_list * 4);       // BUG: u_8=unsigned char=1 byte, *4 overflows 3x
			memset(our_Infrastructure.analyst.list_of_edges, 0, our_Infrastructure.provisioned_capacity.edge_list * sizeof(Edge));
			memset(our_Infrastructure.analyst.weight,      0, our_Infrastructure.provisioned_capacity.edge_list * sizeof(u_8));

		}

		//if (our_Infrastructure.param_ptr->network_struct_report_required)
			our_Infrastructure.analyst.distance_histogram = new unsigned int [std::numeric_limits<short>::max()+1];
		//else our_Infrastructure.analyst.distance_histogram = new unsigned int [2];//Dont allocate much if structural reports are not required

		 if (!our_Infrastructure.analyst.distance_histogram)	
		{
			 cout << "simulation: Error: memory could not be allocated for Analyst::distance_histogram"; 
			 exit(1);
		}


		our_Infrastructure.analyst.routing_table_length_histogram = new unsigned int [our_Infrastructure.param_ptr->RouterNode_routing_table_max_length+1];
		 if (!our_Infrastructure.analyst.routing_table_length_histogram)	
		{
			 cout << "simulation: Error: memory could not be allocated for Analyst::routing_table_length_histogram"; 
			 exit(1);
		}


		our_Infrastructure.analyst.routing_table_width_histogram = new unsigned int [our_Infrastructure.param_ptr->RouterNode_routing_table_max_width+1];
		 if (!our_Infrastructure.analyst.routing_table_width_histogram)	
		{
			 cout << "simulation: Error: memory could not be allocated for Analyst::routing_table_width_histogram"; 
			 exit(1);
		}


		our_Infrastructure.analyst.routing_table_max_width_histogram = new unsigned int [our_Infrastructure.param_ptr->RouterNode_routing_table_max_width+1];
		 if (!our_Infrastructure.analyst.routing_table_max_width_histogram)	
		{
			 cout << "simulation: Error: memory could not be allocated for Analyst::routing_table_max_width_histogram"; 
			 exit(1);
		}




	if (our_Infrastructure.param_ptr->snapshot_reporter_period > 0)
	{
		printf("\n\nScheduling periodic snap shot reporter with periodicity %u, starting at %u\n", 
			our_Infrastructure.param_ptr->snapshot_reporter_period, our_Infrastructure.param_ptr->snapshot_reporter_start_time);

		our_Infrastructure.IO_Reporter_ptr->schedule_periodic(our_Infrastructure.param_ptr->snapshot_reporter_period, 
						our_Infrastructure.param_ptr->snapshot_reporter_start_time);
	}
	
	if (our_Infrastructure.param_ptr->oneshot_reporter_time > 0)
	{
		printf("Scheduling one shot reporter at time %u\n", our_Infrastructure.param_ptr->oneshot_reporter_time);

		our_Infrastructure.IO_Reporter_ptr->schedule_event(our_Infrastructure.param_ptr->oneshot_reporter_time);
	}
	
	printf("\n\nStarting to build the ontology.....\n");

	SmallWorldGraph our_SmallWorldGraph(our_Infrastructure.param_ptr->num_words,our_Infrastructure.param_ptr->degree_of_associativity_of_words,
						our_Infrastructure.param_ptr->sw_model_probability, our_Infrastructure.param_ptr->degree_of_associativity_of_words) ;

	our_Infrastructure.IO_Reporter_ptr->write_network_to_pajek_file(our_SmallWorldGraph.num_vertices,our_SmallWorldGraph.num_edges, our_SmallWorldGraph.list_of_edges, our_SmallWorldGraph.weight, 
										our_Infrastructure.param_ptr->output_onto_file_name, 0); 

///Suneil's code
	
	printf("Finished building the ontology, wrote to file :%s\n", our_Infrastructure.param_ptr->output_onto_file_name);

    ontology_network_diameter = get_dist_mat(&our_SmallWorldGraph);
	cout<<"Ontology Network Diameter = "<<ontology_network_diameter<<std::endl;

    printf("Finished analysing the ontology, writing ontology analysis to file :%s\n", our_Infrastructure.param_ptr->output_onto_report_file_name);
    
    our_Infrastructure.IO_Reporter_ptr->write_structural_report(our_SmallWorldGraph.num_vertices,
		ontology_network_diameter, swg_all_distances, swg_all_parents, 
		NULL, NULL,NULL, NULL,
		our_Infrastructure.param_ptr->output_onto_report_file_name, 0);

 ///Suneil's code
   


	printf("Starting network generation phase of the simulation...\n");

	//Generate or Load ontology from file
	our_Infrastructure.onto_repository_ptr 
		= new OntologyRepository(our_SmallWorldGraph.list_of_edges, 
				our_SmallWorldGraph.weight, our_SmallWorldGraph.num_vertices, our_SmallWorldGraph.num_edges);


	printf("Starting network generation process...\n");
#ifdef DEBUG
		enum NodeType primary_node, birthagent_resource_node, birthagent_router_node, birthagent_user_node, user_node, resource_node, router_node;

		primary_node = PRIMARY_NODE; birthagent_resource_node = BIRTHAGENT_RESOURCE_NODE; 
		birthagent_router_node = BIRTHAGENT_ROUTER_NODE; resource_node = RESOURCE_NODE;
		router_node = ROUTER_NODE; birthagent_user_node = BIRTHAGENT_USER_NODE;
		user_node = USER_NODE;

		printf("PRIMARY NODE has type %d,  BIRTHAGENT_ROUTER_NODE has type %d, BIRTHAGENT_RESOURCE_NODE has type %d, BIRTHAGENT_USER_NODE has type %d, ROUTER NODE has type %d, RESOURCE NODE has type %d, USER_NODE has type %d, \n", 
					primary_node ,  birthagent_router_node, birthagent_resource_node, birthagent_user_node, router_node, resource_node, user_node );
#endif
	//Create all types of proxy agents (for all type of nodes)


	//Router node injection
	BirthAgent<RouterNode> our_BirthAgent_RouterNode(

			our_Infrastructure.param_ptr->bootstrap_threashold,
			our_Infrastructure.param_ptr->Birth_RouterNode_start_time, 
			our_Infrastructure.param_ptr->Birth_RouterNode_period , 
			our_Infrastructure.param_ptr->Birth_RouterNode_total_num_cycles,
			our_Infrastructure.param_ptr->Birth_RouterNode_num_cycles_per_heartbeat,


			our_Infrastructure.param_ptr->message_time_to_live,

			
			our_Infrastructure.param_ptr->RouterNode_known_routers_floor,
			our_Infrastructure.param_ptr->RouterNode_known_routers_trim_threshold,
			our_Infrastructure.param_ptr->RouterNode_known_routers_capacity,

			our_Infrastructure.param_ptr->ResourceNode_translation_radius,

			our_Infrastructure.param_ptr->RouterNode_routing_table_length_floor,
			our_Infrastructure.param_ptr->RouterNode_routing_table_length_trim_threshold,
			our_Infrastructure.param_ptr->RouterNode_routing_table_max_length,

			our_Infrastructure.param_ptr->RouterNode_routing_table_width_floor,
			our_Infrastructure.param_ptr->RouterNode_routing_table_width_threshold,
			our_Infrastructure.param_ptr->RouterNode_routing_table_min_num_resource_nodes,
			our_Infrastructure.param_ptr->RouterNode_routing_table_max_width,


			our_Infrastructure.param_ptr->RouterNode_max_description,
			our_Infrastructure.param_ptr->RouterNode_k, 
			our_Infrastructure.param_ptr->RouterNode_num_centroid_tags_to_explore_per_period,
			our_Infrastructure.param_ptr->RouterNode_num_exploration_queries_to_submit_per_period,

			our_Infrastructure.param_ptr->RouterNode_vitality, 
			our_Infrastructure.param_ptr->RouterNode_extroversion, 
			our_Infrastructure.param_ptr->RouterNode_delay,
			our_Infrastructure.param_ptr->RouterNode_response_timeout,

			&our_Infrastructure  );


	BirthAgent<ResourceNode> our_BirthAgent_ResourceNode(
		
			our_Infrastructure.param_ptr->bootstrap_threashold,
			our_Infrastructure.param_ptr->Birth_ResourceNode_start_time, 
			our_Infrastructure.param_ptr->Birth_ResourceNode_period , 
			our_Infrastructure.param_ptr->Birth_ResourceNode_total_num_cycles,
			our_Infrastructure.param_ptr->Birth_ResourceNode_num_cycles_per_heartbeat,

			our_Infrastructure.param_ptr->message_time_to_live,

			
			our_Infrastructure.param_ptr->ResourceNode_known_routers_floor,
			our_Infrastructure.param_ptr->ResourceNode_known_routers_trim_threshold,
			our_Infrastructure.param_ptr->ResourceNode_known_routers_capacity,

			our_Infrastructure.param_ptr->ResourceNode_translation_radius,

			0,
			0,
			0,

			0,
			0,
			0,
			0,


			our_Infrastructure.param_ptr->ResourceNode_max_description, /*For now assume only a single tag*/
			0,
			0,
			0,

			our_Infrastructure.param_ptr->ResourceNode_vitality, 
			our_Infrastructure.param_ptr->ResourceNode_extroversion,
			our_Infrastructure.param_ptr->ResourceNode_delay,
			our_Infrastructure.param_ptr->ResourceNode_response_timeout,

			&our_Infrastructure  );

	//Want report for generation and operations only
	our_Infrastructure.IO_Reporter_ptr->operational_phase =  GENERATION_PHASE | OPERATION_PHASE ;

	//our_Infrastructure.scheduler_ptr->run(our_Infrastructure.param_ptr->generation_phase_run_for_ticks + 1 );

	//printf("Finished network generation phase of the simulation...\n\n");

	
	//printf("Starting network operation process...\n");


	BirthAgent<UserNode> our_BirthAgent_UserNode(
		
			0,
			our_Infrastructure.param_ptr->Birth_UserNode_start_time, 
			our_Infrastructure.param_ptr->Birth_UserNode_period, 
			1,
			1,

			our_Infrastructure.param_ptr->query_time_to_live,

			0,
			0,
			0,
			
			0,

			0,
			0,
			0,

			0,
			0,
			0,
			0,


			0,  
			0,
			0,
			our_Infrastructure.param_ptr->UserNode_num_queries_to_submit_per_period,

			our_Infrastructure.param_ptr->UserNode_vitality, 
			our_Infrastructure.param_ptr->UserNode_extroversion,
			0,
			our_Infrastructure.param_ptr->UserNode_response_timeout,

			&our_Infrastructure  );


	/*
	our_Infrastructure.user_node_ptr = new UserNode(
								our_Infrastructure.param_ptr->message_time_to_live, 
								0, 
								0, 
								0,
								our_Infrastructure.param_ptr->UserNode_num_queries_to_submit_per_period,
								dummy_description, 
								(std::numeric_limits<unsigned int>::max() - 2), 
								our_Infrastructure.param_ptr->UserNode_vitality, 
								0, 
								0, 
								our_Infrastructure.param_ptr->UserNode_response_timeout, 
								dummy_bootstrapNodeList, 
								&our_Infrastructure 
		);
	*/

	//Want report for operations only
	//our_Infrastructure.IO_Reporter_ptr->operational_phase = OPERATION_PHASE ;

	printf("Staring network generation and operation phase of the simulation together (based on start time of user node birth agents)...\n\n");

	//Run simulation
	our_Infrastructure.scheduler_ptr->run(our_Infrastructure.param_ptr->run_for_ticks + 1 );

	printf("Finished network generation & operation phase of the simulation...\n\n");

	printf("Deleteing all memory objects created in simulation main program.....\n");
	
	std::vector< std::vector< int > >::iterator swg_all_distances_itr = swg_all_distances.begin();
	//while (swg_all_distances_itr != swg_all_distances.end())
	for (unsigned int i= swg_all_distances.size();
			i >0;i-- )
	{
		(*swg_all_distances_itr).clear();
		swg_all_distances_itr++;
	}
	swg_all_distances.clear();

	std::vector< std::vector< Tag > >::iterator swg_all_parents_itr = swg_all_parents.begin();
	//while (swg_all_parents_itr != swg_all_parents.end())
	for (unsigned int i= swg_all_parents.size();
			i >0;i-- )
	{
		(*swg_all_parents_itr).clear();
		swg_all_parents_itr++;
	}
	swg_all_parents.clear();


	printf("Finished deleting all memory objects to reclaim memory from main program\n");

	return 0;
}




unsigned int get_dist_mat(SmallWorldGraph *our_SmallWorldGraph)
{
	
	bool r;
	
	//Making the matrices ready for bellman_ford for distance calculation from node 0
	std::vector<int> distance(our_SmallWorldGraph->num_vertices, std::numeric_limits<short>::max());
	std::vector<Tag> parent(our_SmallWorldGraph->num_vertices,-1);
	std::vector< std::vector<int> > distance_creator_vector(our_SmallWorldGraph->num_vertices, distance);
	//thanks to deep copy !!
	swg_all_distances = distance_creator_vector;
	std::vector< std::vector<Tag> > parent_creator_vector(our_SmallWorldGraph->num_vertices,parent);
	//thanks to deep copy !!
	swg_all_parents = parent_creator_vector;
     unsigned int diameter=0;
	EdgeGraph bellman_g(our_SmallWorldGraph->list_of_edges, (our_SmallWorldGraph->list_of_edges + our_SmallWorldGraph->num_edges) );

	//Create the distance and parent matrix
    for (int j = 0; j < our_SmallWorldGraph->num_vertices; ++j)
	{
			swg_all_distances[j][j] = 0;
			swg_all_parents[j][j] = j;

		  r = bellman_ford_shortest_paths(bellman_g, int (our_SmallWorldGraph->num_vertices), 
					boost::weight_map(our_SmallWorldGraph->weight).distance_map(&((swg_all_distances[j])[0])).predecessor_map(&((swg_all_parents[j])[0])));

		 for (int k = 0; k < our_SmallWorldGraph->num_vertices; ++k)
			if (swg_all_distances[j][k] > diameter) diameter = swg_all_distances[j][k];
	}
	
	return diameter;
	
	
}


/*
//Input parameter list

//Simulation parameters
unsigned int run_for_ticks = 1000;
unsigned int bootstrap_threashold = 5;

//Message parameters
unsigned int message_time_to_live = 100;

//ResourceNode parameters
int ResourceNode_vitality = 10;
int ResourceNode_extroversion = 3;
Tick ResourceNode_response_timeout = 100;

//RouterNode parameters
int RouterNode_vitality = 10;
int RouterNode_extroversion = 3;
Tick RouterNode_response_timeout = 100;
int RouterNode_k = 3;
unsigned int RouterNode_routing_table_max_length = 1000;


//Birth proxy agent for resource nodes parameters
Tick Birth_ResourceNode_start_time = 200;
Tick Birth_ResourceNode_period = 20;
unsigned int Birth_ResourceNode_num_cycles = 10;

//Birth proxy agent for router node parameters
Tick Birth_RouterNode_start_time = 0;
Tick Birth_RouterNode_period = 100;
unsigned int Birth_RouterNode_num_cycles = 5;

//Ontology generator parameter
unsigned int num_words = 100;
unsigned int degree_of_associativity_of_words = 4;
float sw_model_probability = 0.02;

//File names
char input_onto_file_name[20] = "ontoint";
char output_onto_file_name[20] = "ontoout";
char output_network_file_name[20] = "netout";
char structural_report_file_name[20] = "strucrep";
char performance_report_file_name[20] = "perfrep";

*/

