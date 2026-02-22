#ifndef ONTOLOGY_HPP
#define ONTOLOGY_HPP

#include <vector>
#include <list>

#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <boost/graph/edge_list.hpp>
#include <boost/graph/named_function_params.hpp>

#include <boost/config.hpp>
#include <boost/utility.hpp>             // for boost::tie

#include "ForwardDeclaration.hpp"

//using namespace boost;

class OntologyRepository {

	private:
	//std::list<Tag> nearest_k;
	//std::list<Tag>::iterator nearest_k_itr;
	//Description nearest_description;
	//unsigned int current_list_size;
	//unsigned int num_vertices_i;

	public:

	unsigned int num_vertices, num_edges;
	int diameter;

	std::vector< std::vector<int> > all_distances;
	std::vector< std::vector<Tag> > all_parents;
	
	OntologyRepository(Edge *list_of_edges, unsigned char *weight, unsigned int num_vert, unsigned int num_edg);
/*
	void generate_nearest_neighbors(Tag focus_vertice, unsigned int k, const std::vector<Tag> &neighbors, std::list<Tag> &nearest_k); 

	void generate_neighbors_to_explore(Tag focus_vertice, unsigned int radius, std::list<Tag> &nearest_k); 

	void generate_new_neighbors_to_explore(Tag focus_vertice, unsigned int inner_radius, unsigned int radius_increment, std::list<Tag> &nearest_k);//From Suneil

	int distance(Tag vertice1, Tag vertice2);
*/	
	~OntologyRepository();

};



#endif
