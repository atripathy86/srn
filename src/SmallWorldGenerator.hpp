#ifndef SMALLWORLD_HPP
#define SMALLWORLD_HPP

#include "ForwardDeclaration.hpp" //For u_8

class SmallWorldGraph {

	public:
	unsigned int num_vertices, num_edges;

	Edge *list_of_edges;
	unsigned char *weight;
	unsigned int degree_of_associativity_of_words;

	SmallWorldGraph( unsigned int num_vert, unsigned int node_degree, float sw_model_probability,unsigned int passed_degree_of_associativity_of_words );


	~SmallWorldGraph()
	{
#ifdef DEBUG
	printf("Started cleaning up for SmallWorldGraph\n");
#endif
		
		delete [] list_of_edges;
		delete [] weight;

#ifdef DEBUG
	printf("Finished cleaning up for SmallWorldGraph\n");
#endif

		return;
	}
};

SmallWorldGraph::SmallWorldGraph( unsigned int num_vert, unsigned int node_degree, float sw_model_probability,unsigned int passed_degree_of_associativity_of_words ){

		degree_of_associativity_of_words = passed_degree_of_associativity_of_words;
		//typedef boost::adjacency_list<vecS,vecS,bidirectionalS> AdjGraph;
		boost::minstd_rand gen;
		SWGen sw_edge_itr, sw_edge_itr_past;

		boost::graph_traits<AdjGraph>::edge_iterator ei, edge_end;

		num_vertices = num_vert;

		sw_edge_itr_past = SWGen();
		//SWGen sw_edge_itr_past();
		sw_edge_itr = SWGen(gen, num_vertices, node_degree, sw_model_probability);
		//SWGen sw_edge_itr(gen, num_vertices, node_degree, sw_model_probability);
		AdjGraph sw_g(sw_edge_itr, sw_edge_itr_past, num_vertices);

		list_of_edges = new Edge [num_vertices * 2* degree_of_associativity_of_words]; //These are fixed to scale the number of words
		weight = new unsigned char [num_vertices * 2* degree_of_associativity_of_words]; //These are fixed to scale the number of words
		
		if (!list_of_edges)	
		{
			 cout << "simulation: Error: memory could not be allocated for SmallWorldGraph::list_of_edges"; 
			 exit(1);
		}
		if (!weight)	
		{
			 cout << "simulation: Error: memory could not be allocated for SmallWorldGraph::weight"; 
			 exit(1);
		}

		num_edges=0;
		for(tie(ei, edge_end) = edges(sw_g); ei != edge_end; ++ei, num_edges = num_edges+2) {
			list_of_edges[num_edges] = Edge( source(*ei, sw_g), target(*ei,sw_g) );
			list_of_edges[num_edges+1] = Edge( target(*ei,sw_g) , source(*ei, sw_g) );
			weight[num_edges] = 1;
			weight[num_edges+1] = 1;
		}
		return;
};//End of constructor


#endif
