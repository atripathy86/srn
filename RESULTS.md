# Simulation Output Files — Description

The simulation models a self-organising overlay network for content-based resource
discovery.  Three node types participate: **ResourceNodes** (hold resources described
by tags), **RouterNodes** (route queries through the overlay), and **UserNodes**
(submit queries and receive responses).

At each snapshot tick `T` the simulator writes four timestamped files
`T_<name>` plus two once-only ontology files written at startup.

---

## Ontology files (written once at startup)

### `0_ontoout.net` — Pajek network file of the semantic ontology
The word/tag universe is represented as a small-world graph.
Pajek format: a `*Vertices` section listing node ids, followed by an `*Arcs` section
listing directed edges.  Each vertex is a semantic tag (integer id 1..`num_words`).
Edges encode associativity between tags (controlled by
`degree_of_associativity_of_words` and `sw_model_probability` in `config.dat`).

### `0_ontorep` — Structural analysis of the ontology
```
num_vertices = <N>
Network Diameter is <D>
***Distance matrix**   — shortest-path distance (hops) between every pair of tags
***Parents**           — for each (source, dest) pair, the next-hop tag on the shortest path
```
Used to compute semantic distance between any two tag sets — the core of the
`relevance()` comparator that drives routing table ordering.

---

## Per-snapshot files (one set per reported tick T)

### `T_netout.net` — Pajek network snapshot
Current overlay topology.  `*Vertices` lists active node ids; `*Arcs` lists
directed edges (node A knows node B as a router).  Can be loaded directly into
Pajek, Gephi, or NetworkX for visualisation.

### `T_netstrucrep` — Structural report of the generated network

| Section | Meaning |
|---------|---------|
| `Routing table length histogram` | Distribution of how many unique tag-rows each router node's routing table has.  Longer tables → broader semantic coverage. |
| `Routing table width histogram` | Distribution of how many resource/router entries exist per row (averaged per router). |
| `Routing table max width histogram` | Distribution of the widest row across each router's table. |
| `Distance distribution/histogram` | Pairwise hop-count distribution between all router nodes.  Low mean distance = well-connected overlay. |
| `num_vertices` | Number of active nodes at tick T. |
| `Network Diameter` | Maximum shortest-path distance between any two active nodes. |
| `Distance matrix` | Full N×N shortest-path matrix (`Nd/Tg` = Node/Tag id). |
| `Parents matrix` | Next-hop node on the shortest path between every ordered pair (used for routing analysis). |

### `T_netroutingtable` — Routing table contents

Lists each router node's complete routing table at tick T.

```
Node :<router_id>: Row <i> : Tag <tag> id <dest_id> - type <node_type> , ...
```

- **router_id** — the router whose table is shown.
- **Row i** — rows are ordered by semantic relevance (most relevant tag first).
- **Tag** — the content tag this row covers.
- **dest_id / node_type** — a neighbour node known to be relevant to this tag.
  - `type 1` = ResourceNode
  - `type 2` = RouterNode

A row with multiple entries means the router knows several nodes relevant to
that tag; entries within a row are ordered by relevance (strict descending).

### `T_perfrep` — Performance / database report

Counters accumulated from time 0 up to tick T.

| Counter | Meaning |
|---------|---------|
| `Queries_sent_from_ResourceNode/RouterNode` | Query packets originated by each node type for internal network operations (bootstrapping, advertising). |
| `Queries_forwarded_by_RouterNode_*` | Forwarded (relayed) query packets, broken down by origin type. |
| `Responses_sent_from_ResourceNode/RouterNode` | Response packets generated. |
| `Num_of_good_responses` | Responses that successfully reached a requesting node (internal ops). |
| `Num_duplicate_message_dropped` | Packets suppressed by the duplicate-packet cache (`DUPPACK_CACHE`). |
| `Num_looping_message_dropped` | Packets suppressed by loop detection (`DETECT_LOOP`). |
| `Num_queries_dropped_for_TTL` | Queries that expired before reaching a destination. |
| `Num_reponses_dropped_when_no_active_listeners` | Responses that arrived but no listener was waiting. |
| `Num_total_message_generated` | Total packets created for internal network operations. |
| `Queries_sent_from_UserNode` | Query packets sent by end users. |
| `Num_responses_sent_for_UserNode` | Response packets sent back towards user queries. |
| `Num_of_good_responses_for_UserNode` | User queries that received at least one useful response. |
| `Num_total_message_generated_for_UserNode` | Total packets attributable to user-initiated queries. |
| `Num_total_queries_generated` | Grand total of all query packets (internal + user). |
| `Num_total_responses_generated` | Grand total of all response packets. |

The per-tag breakdown at the bottom:
```
Tag : res: rtr: queries: good responses from routers
  <tag>: <#resource_nodes_with_tag>: <#router_nodes_with_tag>: <#queries_for_tag>: <#good_responses>
```
Lets you see which tags are well-covered by the network and how effectively
queries for each tag are resolved.

---

## Key metrics to watch across time

| What to plot | Files | Interpretation |
|---|---|---|
| Network growth | `T_netstrucrep` → `num_vertices` | How fast nodes join. |
| Connectivity | `T_netstrucrep` → `Network Diameter` | Lower = better-connected overlay. |
| Query success rate | `T_perfrep` → `Num_of_good_responses_for_UserNode / Queries_sent_from_UserNode` | Fraction of user queries answered; improves as routing tables fill in. |
| Overhead ratio | `T_perfrep` → `Num_total_message_generated / Queries_sent_from_UserNode` | Packets per user query; lower = more efficient routing. |
| Duplicate/loop drops | `T_perfrep` → duplicate + loop counters | High values indicate redundant flooding; cache/TTL parameters may need tuning. |
| Routing table coverage | `T_netstrucrep` → length & width histograms | Wider, longer tables = richer semantic routing. |
