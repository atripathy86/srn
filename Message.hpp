#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#define NODE_VISTED_LIST_SIZE	64
#define NODE_VISTED_LIST_BYTES	256 //64 *4 =256


//#include "ForwardDeclaration.hpp"

class Message {
	public:
	//unsigned int id;
	unsigned int source_node_id;
	ResourceNode* source_node_ptr;
	
	unsigned int destination_node_id;
	ResourceNode* destination_node_ptr;
	int only_to_node_type;
	unsigned int query_seq_num;
	Description description;
	unsigned int hop_count;
	unsigned int time_to_live;
	enum MessageType type;


#ifdef DETECT_LOOP
	unsigned int node_visited[NODE_VISTED_LIST_SIZE];
	//node_visted has 63 entries and one length of list field, node_visted[0] is the length of the list, as the TTL is small so we just take 63 to be conservative enough.
#endif


	//Message();
	//Message(unsigned int passed_id);
	Message::Message(){

			//id = passed_id;
			source_node_id =0;
			source_node_ptr = NULL;
			
			destination_node_id = 0;
			destination_node_ptr = NULL;
			
			query_seq_num = 0;
			
			hop_count = 0;
			time_to_live = 0;
			type = BLANK_MESSAGE;
			only_to_node_type = TO_RESOURCE_NODE | TO_ROUTER_NODE;

#ifdef DETECT_LOOP
			memset(	node_visited,0,NODE_VISTED_LIST_BYTES);//Set the list to zero, because 64 * sizeof(unsigned int) = 256
#endif			

			return;
		};

	virtual ~Message(){};
};

class Response:public Message {
	public:
	//Response();
	//Response(unsigned int passed_id);
	Response::Response(){type = RESPONSE;return;};

};

class Query:public Message {
	public:
	//int only_to_node_type;//Changed to bit flag type and pushed into base class
	//Query();
	//Query(unsigned int passed_id);
	Query::Query(){type = QUERY; return;};


};

class Feedback:public Message {
	public:
	//int only_to_node_type;//Changed to bit flag type and pushed into base class
	//Feedback();
	//Feedback(unsigned int passed_id);
	Feedback::Feedback(){type = FEEDBACK; return;};

};


#endif

