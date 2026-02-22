#include "ForwardDeclaration.hpp"
#include "Database.hpp"
#include "Input.hpp"

//#include <cmath>
#include "ForwardDeclaration.hpp" //Got from hpp
#include "Input.hpp" //Got from hpp
#include "Infrastructure.hpp" //Added , troubleshooot circular hpp depepndency if this one gives problem
 

Database::Database(Infrastructure* passed_If_ptr) //constructor
//Database::Database(InputParameters *passed_param_ptr)
{
		If_ptr = passed_If_ptr;
		param_ptr = If_ptr->param_ptr;
		//param_ptr = passed_param_ptr;

		Queries_sent_from_ResourceNode =0; //unique : (query_seq_no + source_node)
		Queries_sent_from_RouterNode =0 ; //unique

		Queries_forwarded_by_RouterNode_orgininated_from_ResourceNode =0;
		Queries_forwarded_by_RouterNode_orgininated_from_RouterNode =0;

		Responses_sent_from_ResourceNode =0;
		Responses_sent_from_RouterNode =0;

		//Num_responses_sent =0;// # unique -- node that generates the respnse + query seq that triggerred this response
		Num_of_good_responses =0 ;//rx decides whether it is a good response or not, not yet implemented with full meaning, for now any response received from a router node is a good response.

		Num_duplicate_message_dropped =0; 
		Num_message_dropped_for_TTL =0;
		Num_queries_dropped_for_TTL=0;
		Num_responses_dropped_for_TTL=0;
		
		Num_total_message_generated = 0; //Suneil-total load on the network,

/*
		DatabaseHistResolution=0;
		DatabaseMaxHops=0;
			
		DatabaseHistResolution=param_ptr->DatabaseHistResolution;			//user input param : Default is 5
		if (DatabaseHistResolution==0) //catch if defaults didn't come in for whatever reason
			DatabaseHistResolution=5;  //if so, give it the default

		DatabaseMaxHops=param_ptr->DatabaseMaxHops;							//user input param : Default is 100
		if (DatabaseMaxHops==0)		//catch if defaults didn't come in for whatever reason
			DatabaseMaxHops=100; 	//if so, give it the default
*/
		
		num_buckets = (param_ptr->DatabaseMaxHops/param_ptr->DatabaseHistResolution) ;	
		//rename the counter array -- to be called hops_before_response_ctr
		//add array -- longevity_of_message_ctr_for_TTL
		//modify function to calculate for both arrays. as second argument add the type --i.e which array to increment.

		hops_before_response_ctr= new unsigned int[num_buckets+2]; //the +2 : 1 - catch_all bucket, 1 - 0 hop bucket, just-in-case
		 if (!hops_before_response_ctr)	
		{
			 cout << "Database::Database():Error: memory could not be allocated for hops_before_response_ctr"; 
			 exit(1);
		}
		memset(hops_before_response_ctr,0,(num_buckets+2)*4);  //set all counters to zero
		
		longevity_of_message_ctr_for_TTL= new unsigned int[num_buckets+2]; //the +2 : 1 - catch_all bucket, 1 - 0 hop bucket, just-in-case
		 if (!longevity_of_message_ctr_for_TTL)
		{
			cout << "Database::Database():Error: memory could not be allocated for longevity_of_message_ctr_for_TTL"; 
			 exit(1);
		}
		memset(longevity_of_message_ctr_for_TTL,0,(num_buckets+2)*4);  //set all counters to zero

		longevity_of_message_ctr_for_loop = new unsigned int[num_buckets+2]; //the +2 : 1 - catch_all bucket, 1 - 0 hop bucket, just-in-case
		 if (!longevity_of_message_ctr_for_loop)
		{
			cout << "Database::Database():Error: memory could not be allocated for longevity_of_message_ctr_for_loop"; 
			 exit(1);
		}
		memset(longevity_of_message_ctr_for_loop,0,(num_buckets+2)*4);  //set all counters to zero


		//To record how many resoource exist with specific tag (the index), get total num of tags from ontology rep
		num_resource_with_specific_tag = new unsigned int[ param_ptr->num_words]; //the +2 : 1 - catch_all bucket, 1 - 0 hop bucket, just-in-case
		num_router_with_specific_tag = new unsigned int[ param_ptr->num_words]; //the +2 : 1 - catch_all bucket, 1 - 0 hop bucket, just-in-case

		 if (!num_resource_with_specific_tag || !num_router_with_specific_tag)
		{
			cout << "Database::Database():Error: memory could not be allocated for num_resource_with_specific_tag or num_router_with_specific_tag"; 
			 exit(1);
		}
		memset(num_resource_with_specific_tag,0, param_ptr->num_words*4);  //set all counters to zero
		memset(num_router_with_specific_tag,0, param_ptr->num_words*4);  //set all counters to zero


		Num_of_good_responses_for_specific_tag = new unsigned int[ param_ptr->num_words]; //the +2 : 1 - catch_all bucket, 1 - 0 hop bucket, just-in-case
		 if (!Num_of_good_responses_for_specific_tag)
		{
			cout << "Database::Database():Error: memory could not be allocated for Num_of_good_responses_for_specific_tag"; 
			 exit(1);
		}
		memset(Num_of_good_responses_for_specific_tag,0, param_ptr->num_words*4 );  //set all counters to zero

		Num_of_queries_for_specific_tag = new unsigned int[ param_ptr->num_words]; //the +2 : 1 - catch_all bucket, 1 - 0 hop bucket, just-in-case
		 if (!Num_of_queries_for_specific_tag)
		{
			cout << "Database::Database():Error: memory could not be allocated for Num_of_queries_for_specific_tag_from_UserNode"; 
			 exit(1);
		}
		memset(Num_of_queries_for_specific_tag,0, param_ptr->num_words*4 );  //set all counters to zero



	
		//UserNode counters

		Queries_sent_from_UserNode=0;
	
	   Num_responses_sent_for_UserNode=0;
	   Num_of_good_responses_for_UserNode=0;
		
	   Num_duplicate_message_dropped_for_UserNode=0;

	   Num_queries_dropped_for_TTL_for_UserNode=0; //Suneil | ctr number_queries dropped for ttl //figure out from where you are updating the above ctr.
	   Num_responses_dropped_for_TTL_for_UserNode=0;// Suneil | ctr number_responses dropped for ttl.

		Num_reponses_dropped_when_no_active_listeners =0;
		Num_reponses_dropped_when_no_active_listeners_for_UserNode =0;

	

	   num_buckets_for_UserNode= (param_ptr->DatabaseMaxHops_for_UserNode/param_ptr->DatabaseHistResolution_for_UserNode);

		hops_before_response_ctr_for_UserNode = new unsigned int[num_buckets_for_UserNode + 2]; 
		 if (!hops_before_response_ctr_for_UserNode)	
		{
			 cout << "Database::Database():Error: memory could not be allocated for hops_before_response_ctr_for_UserNode"; 
			 exit(1);
		}
		memset(hops_before_response_ctr_for_UserNode,0,(num_buckets_for_UserNode + 2)*4);  //set all counters to zero
		
		longevity_of_message_ctr_for_TTL_for_UserNode = new unsigned int[num_buckets_for_UserNode + 2]; 
		 if (!longevity_of_message_ctr_for_TTL_for_UserNode)
		{
			cout << "Database::Database():Error: memory could not be allocated for longevity_of_message_ctr_for_TTL_for_UserNode"; 
			 exit(1);
		}
		memset(longevity_of_message_ctr_for_TTL_for_UserNode,0,(num_buckets_for_UserNode + 2)*4);  //set all counters to zero

		
		longevity_of_message_ctr_for_loop_for_UserNode = new unsigned int[num_buckets_for_UserNode + 2]; 
		 if (!longevity_of_message_ctr_for_loop_for_UserNode)
		{
			cout << "Database::Database():Error: memory could not be allocated for longevity_of_message_ctr_for_loop_for_UserNode"; 
			 exit(1);
		}
		memset(longevity_of_message_ctr_for_loop_for_UserNode,0,(num_buckets_for_UserNode + 2)*4);  //set all counters to zero


		Num_of_good_responses_for_specific_tag_for_UserNode = new unsigned int[ param_ptr->num_words]; 
		 if (!Num_of_good_responses_for_specific_tag_for_UserNode)
		{
			cout << "Database::Database():Error: memory could not be allocated for Num_of_good_responses_for_specific_tag_for_UserNode"; 
			 exit(1);
		}
		memset(Num_of_good_responses_for_specific_tag_for_UserNode,0, param_ptr->num_words*4);  //set all counters to zero

		Num_of_queries_for_specific_tag_from_UserNode = new unsigned int[ param_ptr->num_words]; //the +2 : 1 - catch_all bucket, 1 - 0 hop bucket, just-in-case
		 if (!Num_of_queries_for_specific_tag_from_UserNode)
		{
			cout << "Database::Database():Error: memory could not be allocated for Num_of_queries_for_specific_tag_from_UserNode"; 
			 exit(1);
		}
		memset(Num_of_queries_for_specific_tag_from_UserNode,0, param_ptr->num_words*4);  //set all counters to zero

		//sm24, Amit - 25 Aug
		Num_total_queries_generated = 0;
		Num_total_responses_generated = 0;

		return;

}; //close of constructor

Database::~Database() //destructor
{
#ifdef DEBUG
	printf("Started cleaning up for Database\n");
#endif
	
	delete [] num_resource_with_specific_tag;

	delete[] longevity_of_message_ctr_for_TTL;	
	delete[] hops_before_response_ctr;	
	delete[] Num_of_good_responses_for_specific_tag;	
	delete [] Num_of_queries_for_specific_tag;

	//UserNode counters
	delete[] longevity_of_message_ctr_for_TTL_for_UserNode;	
	delete[] hops_before_response_ctr_for_UserNode;	
	delete[] Num_of_good_responses_for_specific_tag_for_UserNode;	
	delete [] Num_of_queries_for_specific_tag_from_UserNode;

#ifdef DEBUG
	printf("Finished cleaning up for Database\n");
#endif

	return;
}//close of destructor

/*
void Database::take_snapshot()
{
	
} //close of take_snapsot
*/

//Suneil, pls update the method with user node couunters
void Database::record_hops(unsigned int hop_count, unsigned int *passed_ctr)

{
	
	histogram_index = hop_count/ param_ptr->DatabaseHistResolution;

	/*
	float temp;
	int i;
	
	if (hop_count==0) //if zero hops, increment that
	{
		++(passed_ctr[0]); return;
	}
		else 
	*/			
		if (hop_count > param_ptr->DatabaseMaxHops ) //if a value greater than the permitted number of hops then push to overflow bucket
			{
				//++(passed_ctr[DatabaseMaxHops]); 
				(passed_ctr[histogram_index +1])++; 

				return;
			}
									
			else {//all other cases, increment appropriate bucket;
					//temp=(hop_count/DatabaseHistResolution); 
					//i=(int) ceil(temp);
					(passed_ctr[histogram_index])++;
					return;
				 }   
}//close of hops_at_resposnses


/*
//Suneil, pls update the method with user node couunters
void Database::hops_before_response(unsigned int hop_count, MessageType message_type)

{

	float temp;
	int i;
	switch (message_type) 
		{
	
				case QUERY: {//1=Query
						 if (hop_count==0) //if zero hops, increment that
								{
									++(longevity_of_message_ctr_for_TTL[0]); return;
								}
								
							else if (hop_count >DatabaseMaxHops ) //if a value greater than the permitted number of hops then push to overflow bucket
									{++(longevity_of_message_ctr_for_TTL[DatabaseMaxHops]); return;
									}
									
							else {//all other cases, increment appropriate bucket;
									temp=(hop_count/DatabaseHistResolution); 	
									i=(int) ceil(temp);
									++(longevity_of_message_ctr_for_TTL[i]);
									return;
								 }   
						 break;		 
						}
	
				case RESPONSE: { //2=Response
								 if (hop_count==0) //if zero hops, increment that
								{
									++(hops_before_response_ctr[0]); return;
								}
								
							else if (hop_count >DatabaseMaxHops ) //if a value greater than the permitted number of hops then push to overflow bucket
									{++(hops_before_response_ctr[DatabaseMaxHops]); return;
									}
									
							else {//all other cases, increment appropriate bucket;
									temp=(hop_count/DatabaseHistResolution); 	
									i=(int) ceil(temp);
									++(hops_before_response_ctr[i]);
									return;
								 }   
						 break;		 
						}		 
				default:{ cout<<"ERROR::Bad Type Identifier for type of drop at hops_at_response"; break; }
						
	     }//close of switch
}//close of hops_at_resposnses

*/


