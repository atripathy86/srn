#ifndef IO_REPORTER_HPP
#define IO_REPORTER_HPP


class IO_Reporter {
	
	SpecificFunctor<IO_Reporter> *functor_handler_ptr;
	Event *handler_schedule_event_ptr;

	Infrastructure* If_ptr;
	Tick period;
	Tick start_time;
	Tick oneshot_time;
	InputParameters *param_ptr;

	public:
	//SimulationPhase operational_phase;
	int operational_phase;


    IO_Reporter(Infrastructure* passed_If_ptr);
	~IO_Reporter();
	void read_input_parameter(char * passed_input_file_name, Infrastructure* passed_If_ptr);
	void verify_config_data(); //To verify the config data provided

	void write_input_parameter_on_screen_from(char * passed_input_file_name);
	
	void write_network_to_pajek_file(unsigned int num_vertices, unsigned int num_edges, 
				const Edge *list_of_edges, const unsigned char *weight, char *passed_output_file_name , Tick current_time);

	void write_structural_report(unsigned int num_vertices, int diameter, 
		const std::vector< std::vector<int> > &all_distances, 
		const std::vector< std::vector<Tag> > &all_parents, const unsigned int *distance_histogram, 
		const unsigned int *routing_table_length_histogram,
		const unsigned int *routing_table_width_histogram,
		const unsigned int *routing_table_max_width_histogram,
		char *passed_struct_report_file_name, Tick current_time );

	void write_performance_report(char *passed_perf_report_file_name , Tick current_time);

	void write_routing_table_content_report(char * passed_routing_table_content_report_file_name, 
					Tick passed_current_time);

	void schedule_periodic(Tick passed_period, Tick passed_start_time);
	void schedule_event(Tick  passed_oneshot_time);

	void heartbeat();

#ifdef DEBUG

	void print_all_nodes();

	void write_edge_list();

#endif

};



#endif
