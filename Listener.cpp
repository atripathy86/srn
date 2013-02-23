#include "ForwardDeclaration.hpp"
//#include "Allheader.hpp"
#include "Listener.hpp"
#include "Message.hpp"
#include "Node.hpp"
#include "Infrastructure.hpp"
#include "OntologyRepository.hpp"
#include "Input.hpp"

Listener::Listener(ResourceNode *passed_parent_node_ptr , unsigned int passed_query_seq_num ) 
	{
		query_seq_num = passed_query_seq_num;
		parent_node_ptr = passed_parent_node_ptr; 
		//expired =0;
		return;
	};


Listener::~Listener()
	{
			//unsigned int num_elements_to_clear;

			//std::list<Response*>::iterator responses_itr;
			responses_itr = responses.begin();
			for (unsigned int num_elements_to_clear = responses.size();
						num_elements_to_clear >0 ;
						num_elements_to_clear--,responses_itr++ )
			{
				//sm24, Amit - Aug 25, Do book keeping before deleting the response
				//parent_node_ptr->If_ptr->list_of_responses.erase((*responses_itr)->id);

				delete *responses_itr;
			}
			responses.clear();
			//Removes itself from the parent nodes listener array
			parent_node_ptr->listener_array.erase( query_seq_num);//?? This is not a good idea as trouble during destruction of resource node 
			return;
	};


void Listener::recv(Response &recvd_response) 
	{ 
		responses.push_back(&recvd_response);

#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER

		if (parent_node_ptr->id == parent_node_ptr->If_ptr->param_ptr->trace_source_node_id && 
				recvd_response.source_node_ptr->id == parent_node_ptr->If_ptr->param_ptr->trace_destination_node_id &&
					(*(recvd_response.description.begin())) == parent_node_ptr->If_ptr->param_ptr->trace_tag 	)
#endif

		{
		printf("Reached here 1\n");	

			printf("Ticks:%u : node %u,type %d, : Noted a response from node %u, type %d, for tag %u and query_seq_num %u\n",
				parent_node_ptr->If_ptr->scheduler_ptr->now(),
				parent_node_ptr->id,  parent_node_ptr->node_type, 
				//recvd_response.source_node_ptr->node_type, (*(recvd_response.description.begin())).first, 
				recvd_response.source_node_ptr->id, recvd_response.source_node_ptr->node_type, 
				(*(recvd_response.description.begin())), recvd_response.query_seq_num);
			//printf("Total response received so far at listner id %u is %u, the expiry status is %d\n", id, responses.size(), expired);
			printf("Ticks:%u : node %u,type %d : Total response received so far at listner id %u is %u\n", 
				parent_node_ptr->If_ptr->scheduler_ptr->now(),
				parent_node_ptr->id,  parent_node_ptr->node_type, 
				id, responses.size()  );
		}
#endif
		return;
	};


void Listener::handle_recvd_responses() {

    responses_itr = responses.begin();

	for (int i = responses.size() ;
				i>0 ;i--, responses_itr++ )
	    { 
			//curr_response_ptr = responses.front();
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER

		if (parent_node_ptr->id == parent_node_ptr->If_ptr->param_ptr->trace_source_node_id &&
				(*responses_itr)->source_node_ptr->id == parent_node_ptr->If_ptr->param_ptr->trace_destination_node_id &&
					(*((*responses_itr)->description.begin())) == parent_node_ptr->If_ptr->param_ptr->trace_tag )
#endif

		printf("Ticks:%u : node %u,type %d : Processing total %u responses at listener id %u for Tag %u, received responses from %u \n", 
			parent_node_ptr->If_ptr->scheduler_ptr->now(), parent_node_ptr->id,
			parent_node_ptr->node_type,responses.size(), id, (*((*responses_itr)->description.begin())) , 
			(*responses_itr)->source_node_ptr->id);


#endif



			parent_node_ptr->handle_recvd_response(*responses_itr);
			
			//parent_node_ptr->If_ptr->list_of_messages.erase(curr_response_ptr->id);
			//sm24
			//parent_node_ptr->If_ptr->list_of_responses.erase(curr_response_ptr->id);

			delete *responses_itr;
			//parent_node_ptr->If_ptr->list_of_messages.erase(curr_response_ptr);
			//responses.pop_front(); //This also deletes the response


		}//End of for loop

		responses.clear(); 
		
		//expired = 1;

		return;
};




/*
void Listener::handle_recvd_responses() {


	while(!responses.empty())
	    { 
			curr_response_ptr = responses.front();
			
#ifdef DEBUG_L2

#ifdef SELECTIVE_PACKET_FILTER

		if (parent_node_ptr->id == parent_node_ptr->If_ptr->param_ptr->trace_source_node_id &&
				curr_response_ptr->source_node_ptr->id == parent_node_ptr->If_ptr->param_ptr->trace_destination_node_id &&
					(*(curr_response_ptr->description.begin())) == parent_node_ptr->If_ptr->param_ptr->trace_tag )
#endif

		printf("Ticks:%u : node %u,type %d : Processing total %u responses at listener id %u for Tag %u, received responses from %u \n", 
			parent_node_ptr->If_ptr->scheduler_ptr->now(), parent_node_ptr->id,
			parent_node_ptr->node_type,responses.size(), id, (*(curr_response_ptr->description.begin())) , 
			curr_response_ptr->source_node_ptr->id);


#endif



			parent_node_ptr->handle_recvd_response(curr_response_ptr);
			
			//parent_node_ptr->If_ptr->list_of_messages.erase(curr_response_ptr->id);
			//sm24
			//parent_node_ptr->If_ptr->list_of_responses.erase(curr_response_ptr->id);

			delete curr_response_ptr;
			//parent_node_ptr->If_ptr->list_of_messages.erase(curr_response_ptr);
			//responses.pop_front(); //This also deletes the response


		}//End of while loop

		//expired = 1;

		return;
};
*/
