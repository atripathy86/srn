#include "ForwardDeclaration.hpp"
//#include "Allheader.hpp"
#include "Analyst.hpp"
#include "Infrastructure.hpp"
#include "Node.hpp"

#include "Input.hpp" //For supporting resetting of the rpouting table histogram counter arrays

#ifdef DEBUG
#include "IO_Reporter.hpp"
#include <limits>
#endif

#include <cstring>

#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <boost/graph/edge_list.hpp>
#include <boost/graph/named_function_params.hpp>

#include <boost/config.hpp>
#include <boost/utility.hpp>             // for boost::tie


//Analyst::Analyst(Infrastructure * passed_If_ptr) {If_ptr = passed_If_ptr;}
Analyst::Analyst(){
	
		//list_of_edges = NULL;
		//weight = NULL;
		diameter = 0;
		num_edges = 0;
		provisioned = 0;
		return;
	};


Analyst::~Analyst() 
	{

#ifdef DEBUG
 		printf("Starting cleaning up Analyst\n");
#endif
 
		//Cleanup these datastructures only if they were used !
		if (network_struct_report_required) {
  		printf("Reached here 3, network_struct_report_required %d\n", network_struct_report_required);

			for (int i = 0; i <num_vertices ;i++ )
			{
				(all_distances[i]).clear();
				(all_parents[i]).clear();
				//(distance_histogram[i]).clear();

			}
			
			all_distances.clear();
			all_parents.clear();
		}

#ifdef DEBUG

 		printf("Finished cleaning up all_distances, all_parents vectors\n");
#endif

		
		delete [] distance_histogram;

		
#ifdef DEBUG
		printf("Finished cleaning up distance_histogram\n");
#endif
		
		delete [] list_of_edges;	

#ifdef DEBUG
		printf("Finished cleaning up list_of_edges\n");
#endif
		
		delete [] weight;		

#ifdef DEBUG
		printf("Finished cleaning up weight\n");

 	printf("Finished cleaning up Analyst\n");
#endif


 		return;
	};


void Analyst::extract_edge_list() {
		//std::list<RouterNode *>::iterator list_of_router_nodes_itr;
		/* 	std::map<unsigned int, RouterNode *> list_of_router_nodes; */

		std::vector< RouterNode *>::iterator list_of_router_nodes_itr;
		/*
		RoutingTable routing_table;
		list<Tag> knowledge;
		list<ResourceNode*> *routing_table_row_obj_ptr;

		typedef std::pair<Tag, Tag> Edge;

		Infrastructure - std::vector<RouterNode *> list_of_router_nodes;

		*/
		RoutingTableIterator routing_table_itr;
		RoutingTableRowIterator routing_table_row_itr;

		//For extracting edges from resource node's reverse routing table (known router list)
		/*
		Infrastructure - std::vector<RouterNode *> list_of_router_nodes;

		std::map< unsigned int, RouterNode *> known_routers;
		*/
		std::vector<ResourceNode *>::iterator list_of_resource_nodes_itr;
		std::map< unsigned int, RouterNode*>::iterator known_routers_itr;

		//.For the Routing table histogram
		unsigned int curr_routing_table_max_width;
		memset(routing_table_length_histogram,0,(If_ptr->param_ptr->RouterNode_routing_table_max_length+1)*4);  //set all counters to zero
		//memset(routing_table_width_histogram,0,(If_ptr->param_ptr->RouterNode_routing_table_max_length+1)*4);  // BUG: width histogram allocated max_width+1, not max_length+1
		//memset(routing_table_max_width_histogram,0,(If_ptr->param_ptr->RouterNode_routing_table_max_length+1)*4);  // BUG: same
		memset(routing_table_width_histogram,     0, (If_ptr->param_ptr->RouterNode_routing_table_max_width+1)*4);
		memset(routing_table_max_width_histogram, 0, (If_ptr->param_ptr->RouterNode_routing_table_max_width+1)*4);

		num_vertices = If_ptr->list_of_router_nodes.size() + If_ptr->list_of_resource_nodes.size();
		
#ifdef DEBUG
	printf("Analyst::extract_edge_list() : Wrtiting edge_list....\n");
#endif
				
		num_edges=0;
		list_of_router_nodes_itr = If_ptr->list_of_router_nodes.begin();
		//Go to each router node
		//while( list_of_router_nodes_itr != (If_ptr->list_of_router_nodes).end() )
		for (unsigned int i= (If_ptr->list_of_router_nodes).size(); 
				i>0 ; i--)
		{


			routing_table_itr = (*list_of_router_nodes_itr)->routing_table.begin();
			
			//For finding current routing table max width
			curr_routing_table_max_width =0;


			//For each router node, traverse the routing table, for each node entry add an edge
			//while( routing_table_itr != (*list_of_router_nodes_itr)->routing_table.end() )
			for (unsigned int j= (*list_of_router_nodes_itr)->routing_table.size(); 
				j>0; j--)
			{


						
					//No need to extract edge if it is not required
					if (If_ptr->param_ptr->network_struct_report_required)
					{
 
						routing_table_row_itr = ((*routing_table_itr).second)->destinations.begin();


						/*
						current router node is (*list_of_router_nodes_itr), its id is list_of_router_nodes_itr->id
						its routing table is list_of_router_nodes_itr->routing_table,
						each row in that routing table is 

						*/

							//while (routing_table_row_itr !=  ((*routing_table_itr).second).end())
							for (unsigned int k= ((*routing_table_itr).second)->destinations.size(); 
								k>0; k--)
							{

								(list_of_edges[num_edges]).first = (*list_of_router_nodes_itr)->id ;

								(list_of_edges[num_edges]).second = (*routing_table_row_itr)->id ;

								weight[num_edges] = 1;

								routing_table_row_itr++;
								/*
								//Protection against memory underprovisioning
								if (num_edges >= (If_ptr->provisioned_capacity.edge_list - 1) )
								{
									printf("Run time error 1, need more memory to extract edges, exiting from Analyst::extract_edge_list()\n");
									exit(1);
								}
								*/

								num_edges++;

							}

					}//if (If_ptr->param_ptr->network_struct_report_required)

					/*

					if (((*routing_table_itr).second)->destinations.size() > If_ptr->param_ptr->RouterNode_routing_table_max_width)
					{
						printf("Run time error 4, need more memory for routing_table_width_histogram, index is %d, exiting from Analyst::extract_edge_list()\n", ((*routing_table_itr).second)->destinations.size());
						exit(1);
					}
					*/

					//Generate the max width for this routing table row
					if (curr_routing_table_max_width < ((*routing_table_itr).second)->destinations.size() )
							curr_routing_table_max_width = ((*routing_table_itr).second)->destinations.size();
					//Record the freq of width -- guard restored: actual size can exceed allocated max with large birth-cycle configs
					if (((*routing_table_itr).second)->destinations.size() <= (unsigned int)If_ptr->param_ptr->RouterNode_routing_table_max_width)
						routing_table_width_histogram[ ((*routing_table_itr).second)->destinations.size()]++;

					routing_table_itr++;
			}
			/*
					if ( (*list_of_router_nodes_itr)->routing_table.size() > If_ptr->param_ptr->RouterNode_routing_table_max_length )
					{
						printf("Run time error 5, need more memory for routing_table_length_histogram, index is %d , exiting from Analyst::extract_edge_list()\n",
							(*list_of_router_nodes_itr)->routing_table.size() );
						exit(1);
					}
			*/
			//Record the max width for this routing table row -- guard restored
			if (curr_routing_table_max_width <= (unsigned int)If_ptr->param_ptr->RouterNode_routing_table_max_width)
				routing_table_max_width_histogram[ curr_routing_table_max_width ]++;
			//Record routing_table_length -- guard restored
			if ((*list_of_router_nodes_itr)->routing_table.size() <= (unsigned int)If_ptr->param_ptr->RouterNode_routing_table_max_length)
				routing_table_length_histogram[(*list_of_router_nodes_itr)->routing_table.size()]++;


			list_of_router_nodes_itr++;

		}

		//No need to extract edge if it is not required
		if (If_ptr->param_ptr->network_struct_report_required)
		{
			//Go to each resource node's known_router list, and draw edges from that resource node to those router nodes in that list
			
				list_of_resource_nodes_itr = If_ptr->list_of_resource_nodes.begin();

				//while( list_of_resource_nodes_itr != (If_ptr->list_of_resource_nodes).end() )
				for (unsigned int i= (If_ptr->list_of_resource_nodes).size(); 
						i>0; i--)
				{
					known_routers_itr = (*list_of_resource_nodes_itr)->known_routers.begin();

					//while (known_routers_itr != (*list_of_resource_nodes_itr)->known_routers.end())
						for (unsigned int j= (*list_of_resource_nodes_itr)->known_routers.size(); 
							j>0; j--)
						{
							list_of_edges[num_edges] = Edge( (*list_of_resource_nodes_itr)->id , (*known_routers_itr).first  );
										
							weight[num_edges] = 1;

							known_routers_itr++;
							num_edges++;
							
							/*
								//Protection against memory underprovisioning
								if (num_edges >= (If_ptr->provisioned_capacity.edge_list - 1) )
								{
									printf("Run time error 2, need more memory to extract edges, exiting from Analyst::extract_edge_list()\n");
									exit(1);
								}
							*/
						}
					list_of_resource_nodes_itr++;
				}

		}//if if (If_ptr->param_ptr->network_struct_report_required)

	return;
};


int Analyst::generate_distance_matrix() {
	
/*
	//Making the matrices ready for bellman_ford for distance calculation from node 0
	std::vector<int> distance(num_vertices, std::numeric_limits<short>::max());
	std::vector<Tag> parent(num_vertices,-1);
	std::vector< std::vector<int> > distance_creator_vector(num_vertices, distance);
	all_distances = distance_creator_vector;
	std::vector< std::vector<Tag> > parent_creator_vector(num_vertices,parent);
	all_parents = parent_creator_vector;

	EdgeGraph bellman_g(list_of_edges, (list_of_edges + num_edges) );

	//Create the distance and parent matrix
    for (int j = 0; j < num_vertices; ++j)
	{
			all_distances[j][j] = 0;
			all_parents[j][j] = j;

		  r = bellman_ford_shortest_paths(bellman_g, int (num_vertices), 
					boost::weight_map(weight).distance_map(&((all_distances[j])[0])).predecessor_map(&((all_parents[j])[0])));

		 for (int k = 0; k < num_vertices; ++k)
			if (all_distances[j][k] > diameter) diameter = all_distances[j][k];
	}

*/
				//printf("Reached here 1...\n");

	bool r;
	//unsigned int j,k;
	//Making the matrices ready for bellman_ford for distance calculation from node 0
	std::vector<int> distance(num_vertices, std::numeric_limits<short>::max());
	std::vector<Tag> parent(num_vertices,-1);
	std::vector< std::vector<int> > distance_creator_vector(num_vertices, distance);
	//thanks to deep copy !!
	all_distances = distance_creator_vector;
	std::vector< std::vector<Tag> > parent_creator_vector(num_vertices,parent);
	//thanks to deep copy !!
	all_parents = parent_creator_vector;

	EdgeGraph bellman_g(list_of_edges, (list_of_edges + num_edges) );
	diameter =0;

	memset(distance_histogram,0,(std::numeric_limits<short>::max()+1)*4);  //set all counters to zero

	//Create the distance and parent matrix
    for (unsigned int j = 0; j < num_vertices; ++j)
	{
			
			all_distances[j][j] = 0;
			all_parents[j][j] = j;

		  r = bellman_ford_shortest_paths(bellman_g, int (num_vertices), 
					boost::weight_map(weight).distance_map(&((all_distances[j])[0])).predecessor_map(&((all_parents[j])[0])));
				//printf("Reached here 2...%d\n",j);

		for (unsigned int k = 0; k < num_vertices; ++k)
		{
			/*
			if ((all_distances[j][k]) > (std::numeric_limits<short>::max()) )
			{
				printf("Run time error 3, need more memory for distance_histogram, index is %d , exiting from Analyst::generate_distance_matrix()\n",
								(all_distances[j][k]) );
				exit(1);
			}
			*/

			if (all_distances[j][k] > diameter) diameter = all_distances[j][k];
			distance_histogram[(all_distances[j][k])]++;
							//printf("Reached here 3...%d\n",k);

		}

	}
	
	return diameter;
};



/*
void Analyst::write_performance_report(char *performance_report_file_name , Tick passed_current_time) {
	
		
	return;
};

*/
