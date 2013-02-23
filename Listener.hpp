#ifndef LISTENER_HPP
#define LISTENER_HPP


//class Listener:public Handler {
class Listener {
		//Response *curr_response_ptr;
	
		//RouterNode *router_node_ptr; //Is this required ?

		unsigned int query_seq_num;

	public: 

#ifdef DEBUG
		unsigned int id; //For troubleshooting
#endif


 		ResourceNode *parent_node_ptr; //Update this
 		//int expired;
		std::list<Response*> responses;
		std::list<Response*>::iterator responses_itr;
		Listener(ResourceNode *passed_parent_node_ptr , unsigned int passed_query_seq_num );
		void recv(Response &recvd_response);
		void handle_recvd_responses();

		~Listener();

};



#endif

