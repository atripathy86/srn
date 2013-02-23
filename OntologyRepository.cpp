#include "OntologyRepository.hpp"
#include "ForwardDeclaration.hpp"  //Got from hpp
//#include "Allheader.hpp"

OntologyRepository::OntologyRepository(Edge *list_of_edges, unsigned char *weight, unsigned int num_vert, unsigned int num_edg) {

	bool r;
	num_vertices = num_vert;
	num_edges = num_edg;
	diameter=0;
	//Making the matrices ready for bellman_ford for distance calculation from node 0
	std::vector<int> distance(num_vertices, std::numeric_limits<short>::max());
	std::vector<Tag> parent(num_vertices,-1);
	std::vector< std::vector<int> > distance_creator_vector(num_vertices, distance);
	all_distances = distance_creator_vector;
	std::vector< std::vector<Tag> > parent_creator_vector(num_vertices,parent);
	all_parents = parent_creator_vector;

	EdgeGraph bellman_g(list_of_edges, (list_of_edges + num_edges) );

	//Create the distance and parent matrix
    for (unsigned int j = 0; j < num_vertices; ++j)
	{
			all_distances[j][j] = 0;
			all_parents[j][j] = j;

		  r = bellman_ford_shortest_paths(bellman_g, int (num_vertices), 
					boost::weight_map(weight).distance_map(&((all_distances[j])[0])).predecessor_map(&((all_parents[j])[0])));

		 for (unsigned int k = 0; k < num_vertices; ++k)
			if (all_distances[j][k] > diameter) diameter = all_distances[j][k];
	}
	return;
};


OntologyRepository::~OntologyRepository()
	{
#ifdef DEBUG
	printf("Started cleaning up for OntologyRepository\n");
#endif
		
		std::vector< std::vector<int> >::iterator all_distances_itr;
		std::vector< std::vector<Tag> >::iterator all_parents_itr;

		unsigned int num_elements_to_clear;


		for (num_elements_to_clear = all_distances.size(),all_distances_itr = all_distances.begin();
						num_elements_to_clear >0 ;
						num_elements_to_clear--,all_distances_itr++ )
		{
			(*all_distances_itr).clear();
		}

		for (num_elements_to_clear = all_parents.size(),all_parents_itr = all_parents.begin();
						num_elements_to_clear >0 ;
						num_elements_to_clear--,all_parents_itr++ )
		{
			(*all_parents_itr).clear();
		}

		all_parents.clear();
		all_distances.clear();
#ifdef DEBUG
	printf("Finished cleaning up for OntologyRepository\n");
#endif
		return;
	}


/*

void OntologyRepository::generate_nearest_neighbors(Tag focus_vertice, unsigned int k, const std::vector<Tag> &neighbors, std::list<Tag> &nearest_k) {

		nearest_k.clear(); //The list object is made a permenant resource for reuse

		current_list_size=0;

		//for (unsigned int num_vertices_i=0;num_vertices_i < neighbors.size() ; num_vertices_i++)
		for (unsigned int num_vertices_i=0,fk = neighbors.size(); fk>0 ; num_vertices_i++,fk--)
		{
			nearest_k_itr = nearest_k.begin();
			if (current_list_size < k) //We havent recorded the nearest k yet, so fill it up
			{
				
				//Insert the node in the return list, at proper location
				//Find location to insert
				//while ( !(nearest_k_itr == nearest_k.end())  && 
				//	 all_distances[focus_vertice][neighbors[num_vertices_i]] < all_distances[focus_vertice][*nearest_k_itr]) nearest_k_itr++;
				//
				for( int i = nearest_k.size();
						i>0  && all_distances[focus_vertice][neighbors[num_vertices_i]] < all_distances[focus_vertice][*nearest_k_itr];
						i--	) nearest_k_itr++;

				nearest_k.insert(nearest_k_itr,neighbors[num_vertices_i]);
				current_list_size++;

			} else { //nearest k neighbors have been recorded, now lets adjust the list
				//First check whether the current neghbor will be within nearest k, if yes then
				Remove the biggest element in the list to create a space for a nearer neighbor //

				if ( all_distances[focus_vertice][neighbors[num_vertices_i]] < all_distances[focus_vertice][*nearest_k_itr])
				{
					nearest_k.pop_front();

					//Insert nearer neighbor
					//Find location to insert
					nearest_k_itr = nearest_k.begin();
					//while (!(nearest_k_itr == nearest_k.end())  && 
					//	all_distances[focus_vertice][neighbors[num_vertices_i]] < all_distances[focus_vertice][*nearest_k_itr]) nearest_k_itr++;
					//
					for( int i = nearest_k.size();
						i>0  && all_distances[focus_vertice][neighbors[num_vertices_i]] < all_distances[focus_vertice][*nearest_k_itr];
						i--	) nearest_k_itr++;

					nearest_k.insert(nearest_k_itr,neighbors[num_vertices_i]);

				} //end of if ( all_distances[focus_vertice][neighbors[i]] < *nearest_k_itr)

			}//end of if (current_list_size < k)
		

		}//end of for loop

		return;
}; 

int OntologyRepository::distance(Tag vertice1, Tag vertice2) {
	
	return 	all_distances[vertice1][vertice2];
	
};

void OntologyRepository::generate_neighbors_to_explore(Tag focus_vertice, unsigned int radius, std::list<Tag> &nearest_k) {
	
		nearest_k.clear();
		for (unsigned int num_vertices_i=0; num_vertices_i < num_vertices ; num_vertices_i++)
		{
			if ( all_distances[focus_vertice][num_vertices_i] <= radius)
				nearest_k.push_back( num_vertices_i );
		}
		
		return ;
}; 


//Code added by Suneil for the exploration around the known areas.
// added 10/18/07

void OntologyRepository::generate_new_neighbors_to_explore(Tag focus_vertice, unsigned int inner_radius, unsigned int radius_increment, std::list<Tag> &nearest_k) {
	
		nearest_k.clear();

		unsigned int outer_radius = inner_radius + radius_increment ;

		for (unsigned int num_vertices_i=0; num_vertices_i < num_vertices ; num_vertices_i++)
		{
			//we are trying to find those nodes that are outside the current radius, with the outer circle being the hop radius 
			// plus the radius_increment. But since we don't want to rediscover the already known nodes, we ask for those
			// nodes that are outside the inner_radius but inside the circle of inner_radius + radius_increment.
			// Circular strip of Inside radius = inner_radius, outer radius = inner_radius + radius_increment.
			if ( (all_distances[focus_vertice][num_vertices_i] > inner_radius) && (all_distances[focus_vertice][num_vertices_i] <= outer_radius) )
 					nearest_k.push_back( num_vertices_i );
 
		}
		
		return ;
}; 

*/
