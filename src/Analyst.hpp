//#include "Allheader.hpp"
#ifndef ANALYST_HPP
#define ANALYST_HPP

#include "ForwardDeclaration.hpp" //For u_8

class Analyst {
	int provisioned;
	
	public:
	int  network_struct_report_required;

	unsigned int num_vertices, num_edges;
	std::vector< std::vector< int > > all_distances;
	std::vector< std::vector< Tag > > all_parents;
	int diameter;


	Edge *list_of_edges;
	unsigned char *weight;
	unsigned int *distance_histogram;

	unsigned int *routing_table_length_histogram;
	unsigned int *routing_table_width_histogram;
	unsigned int *routing_table_max_width_histogram;

	Infrastructure *If_ptr;

	Analyst();
	void extract_edge_list();
	int generate_distance_matrix();
	//void write_performance_report(char *performance_report_file_name, Tick passed_current_time );
	
	~Analyst();

};

#endif  // #ifndef ANALYST_HPP
