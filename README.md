# SRN — Semantic Routing Network Simulator

A C++ discrete-event simulator for peer-to-peer networks that use semantic ontologies to route resource-discovery queries. The project models how nodes in a distributed network can find resources by leveraging meaning rather than exact addresses.

---

## Overview

The simulator builds a small-world semantic ontology over a vocabulary of words, then populates a P2P network with three node types that communicate through a message-passing event loop:

| Node Type | Role |
|-----------|------|
| **ResourceNode** | Advertises resources tagged with semantic descriptions |
| **RouterNode** | Maintains routing tables and forwards queries toward semantically relevant destinations |
| **UserNode** | Generates queries for resources and collects responses |

At the end (and periodically during) the simulation, the framework writes network topology, routing tables, and performance metrics to report files.

---

## Architecture

```
simulation.cpp          Main driver — reads config, wires components, runs event loop
├── Infrastructure      Global container (scheduler, database, analyst, reporter, node lists)
├── Scheduler           Discrete-event engine backed by std::multiset
├── SmallWorldGenerator Boost Graph Library — generates the semantic ontology
├── OntologyRepository  Distance matrix (Bellman-Ford) over the ontology graph
├── Node / ProxyAgent   Node hierarchy + template-based birth agents
│   ├── ResourceNode
│   ├── RouterNode
│   └── UserNode
├── Message             Query / Response / Feedback with TTL and loop-detection
├── Database            Counters & histograms for queries, responses, drops
├── Analyst             Extracts structural metrics from live routing tables
└── IO_Reporter         Writes Pajek, text-structure, and performance reports
```

Key design patterns: event-driven scheduling, template proxy (BirthAgent<T>), observer (Listener), and compile-time feature flags.

---

## Technologies

- **Language**: C++ (C++98/03 compatible)
- **Compiler**: g++34 (GCC 3.4)
- **Build**: GNU Make
- **Libraries**: Boost Graph Library (small-world generation, Bellman-Ford), STL

---

## Building

```bash
make        # produces the 'simulation' executable
make clean  # remove object files and binary
```

Relevant compile-time flags set in the Makefile:

| Flag | Effect |
|------|--------|
| `DEBUG`, `DEBUG_L1`, `DEBUG_L2` | Verbose logging levels |
| `DUPPACK_CACHE` | Enable duplicate-packet caching |
| `DETECT_LOOP` | Enable in-message loop detection |
| `SELECTIVE_PACKET_FILTER` | Filter redundant forwarded packets |

---

## Running

```bash
# Pass the config file as a positional argument
./simulation config.dat

# Or run interactively and type the config filename when prompted
./simulation
```

The simulator prints progress to stdout and writes all output files to the current directory.

---

## Configuration

Configuration is read from an ASCII `.dat` file (see `config.dat` for the canonical example). Key parameter groups:

### Simulation control
| Parameter | Description |
|-----------|-------------|
| `run_for_ticks` | Total simulation duration |
| `snapshot_reporter_period` | How often to write snapshot reports |
| `bootstrap_threashold` | Min nodes before network activation |

### Message management
| Parameter | Description |
|-----------|-------------|
| `message_time_to_live` | TTL for internal routing messages |
| `query_time_to_live` | TTL for user-originated queries |

### Node behaviour (ResourceNode / RouterNode / UserNode)
Parameters control vitality, extroversion, delays, response timeouts, routing-table capacity, trimming thresholds, and exploration frequency. See `Input.hpp` for the full list (~100 parameters).

### Ontology generation
| Parameter | Description |
|-----------|-------------|
| `num_words` | Vocabulary size (semantic graph nodes) |
| `degree_of_associativity_of_words` | Average word-to-word connections |
| `sw_model_probability` | Small-world rewiring probability |

### Birth agents
Control when each node type starts joining the network (`Birth_*Node_start_time`), how frequently (`Birth_*Node_period`), and the total count (`Birth_*Node_total_num_cycles`).

Alternative config variants are included for different scenarios: `configbig.dat`, `config18.dat`, `configv*.dat`, `configm.dat`, `configu.dat`.

---

## Output Files

Snapshot files are prefixed with the snapshot number (e.g., `1_`, `51_`):

| File | Content |
|------|---------|
| `N_netout.net` | Pajek-format network topology |
| `N_netroutingtable` | Routing table contents per RouterNode |
| `N_netstrucrep` | Network structure analysis (distances, histogram) |
| `N_perfrep` | Performance metrics (recall, drops, hop counts) |
| `ontoout.net` | Pajek-format semantic ontology |
| `ontorep` | Ontology analysis (diameter, distances) |

### Performance report metrics
- Queries issued and forwarded per node type
- **Recall rate** — good responses / (queries × nodes present)
- Message drops broken down by cause (TTL, loop, duplicate)
- Per-tag resource-discovery analysis
- Histograms for hop counts and message longevity

---

## Key Algorithms

- **Small-world generation** — Watts-Strogatz model via Boost Graph Library
- **Semantic distance** — Bellman-Ford shortest paths on the ontology graph
- **Query routing** — Relevance-sorted routing tables; top-k forwarding
- **Loop detection** — Visited-node bitfield carried in each message header
- **Table trimming** — Configurable LRU-like eviction when tables exceed capacity

---

## Known Limitations

- Large configurations (`configbig.dat`) can cause memory pressure and potential segfaults due to iterator overflow at scale.
- Network-structure extraction is memory-intensive and is disabled by default in the Makefile.

---

## Project Structure

```
srn/
├── simulation.cpp          Entry point
├── Makefile
├── config*.dat             Configuration variants
├── *.hpp / *.cpp           Source headers and implementations
│   ├── Infrastructure.hpp
│   ├── Scheduler.hpp/cpp
│   ├── Node.hpp/cpp        (ResourceNode, RouterNode, UserNode)
│   ├── ProxyAgent.hpp
│   ├── Message.hpp
│   ├── Listener.hpp
│   ├── SmallWorldGenerator.hpp
│   ├── OntologyRepository.hpp/cpp
│   ├── Analyst.hpp/cpp
│   ├── Database.hpp/cpp
│   ├── IO_Reporter.hpp/cpp
│   ├── Input.hpp
│   ├── ForwardDeclaration.hpp
│   ├── Event.hpp
│   ├── Functor.hpp
│   └── ...
└── *.net / *rep / *table   Simulation output (generated at runtime)
```
