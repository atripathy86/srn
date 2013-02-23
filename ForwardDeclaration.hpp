#ifndef FORWARDDECLARATION_HPP
#define FORWARDDECLARATION_HPP

#include <utility>                        
#include <vector>						  
#include <list>
#include <map>
#include <set>
#include <typeinfo>


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/small_world_generator.hpp>
#include <boost/random/linear_congruential.hpp>

#include <boost/graph/edge_list.hpp>


#define  RES_NODE 1
#define  RTR_NODE 2
#define  RES_RTR_NODE 3
#define  GENERATION_PHASE 1
#define  OPERATION_PHASE 2


enum NodeType {PRIMARY_NODE,  RESOURCE_NODE, ROUTER_NODE , USER_NODE, BIRTHAGENT_USER_NODE,  BIRTHAGENT_RESOURCE_NODE, BIRTHAGENT_ROUTER_NODE };
enum MessageType { BLANK_MESSAGE, QUERY, RESPONSE, FEEDBACK };
//enum MessageDestinationFilter { TO_RESOURCE_NODE = TO_RES_NODE, TO_ROUTER_NODE = TO_RTR_NODE };
enum MessageDestinationFilter { TO_RESOURCE_NODE = 1, TO_ROUTER_NODE = 2 };
//enum SimulationPhase { GENERATION_PHASE = 1, OPERATION_PHASE =2 };

class Analyst;

class Database;
class Event;
class Functor;

class Message;
class Query;
class Response;
class Feedback;


class Node;
class ResourceNode;
class RouterNode;
class UserNode;

//class Handler;
class Listener;
class QueryHandler;

class Infrastructure;
class IO_Reporter;
class InputParameters;
class Scheduler;

class SmallWorldGraph;
class OntologyRepository;

template <class TClass> class SpecificFunctor;
template <class SpecificNode> class BirthAgent ;


//typedef unsigned Tag;
typedef unsigned short Tag;
//typedef unsigned int Tag;

typedef unsigned char u_8;
typedef unsigned int Tick; 

//typedef std::vector< Tag > Description; 	

//typedef std::map< Tag, Tag > Description;
typedef std::set< Tag > Description;

typedef std::list< RouterNode * > BootstrapNodeList;

//typedef std::pair< Tag, Tag > Edge;
typedef std::pair< Tag, Tag > Edge;

//typedef std::pair<unsigned int, unsigned int> Edge;
typedef boost::adjacency_list<> AdjGraph;

typedef boost::small_world_iterator< boost::minstd_rand, AdjGraph > SWGen;


//For Bellman
typedef boost::edge_list<Edge*> EdgeGraph;


//For router state
#ifdef DUPPACK_SET
typedef std::pair< unsigned int, unsigned int > NodeId_QuerySeqNum;
typedef std::set< NodeId_QuerySeqNum > QueryAlreadyReceived;
#endif


using namespace std;

//void operation_simulation();

//*************These are for trimming routing table and known routers list *********************
class KnownRouterIndexElement ;
class KnownRoutersIndexEncapsulator;






#ifdef DEBUG

//void inline print_map_description( std::map< Tag, Tag > &description_to_print ) 
void inline print_map_description( std::set<Tag> &description_to_print ) 
{
	//std::map< Tag, Tag >::iterator map_itr = description_to_print.begin();
	Description::iterator description_itr = description_to_print.begin();

	while ( description_itr != description_to_print.end() )
	{
		printf(" %u ", (*description_itr) );
		description_itr++;
	}
	printf("\n");
};
#endif

#ifdef DEBUG

void inline print_map_known_routers( std::map< unsigned int, RouterNode *> &known_routers_to_print ) 
{
	std::map< unsigned int, RouterNode *>::iterator map_itr = known_routers_to_print.begin();
	while ( map_itr != known_routers_to_print.end() )
	{
		printf(" %u ", (*map_itr).first );
		map_itr++;
	}
	printf("\n");
};
#endif

/*
#ifdef DEBUG
template <typename specific_map>
void inline print_map(specific_map &map_to_print) 
{
	typename specific_map::iterator map_itr = map_to_print.begin();
	while ( map_itr != map_to_print.end() )
	{
		printf(" %u ", (*map_itr).first );
		map_itr++;
	}
	printf("\n");
}
#endif
*/
#endif  // #ifndef FORWARDDECLARATION_HPP


