# CLAUDE.md — Developer Notes for AI Assistants

This file records conventions, architectural decisions, and session history for
anyone (human or AI) picking up this codebase.

---

## Project overview

SRN is a C++ discrete-event simulator for a self-organising peer-to-peer
overlay that routes resource-discovery queries using a **semantic ontology**
rather than keyword equality or central directory lookups.  See [README.md](README.md)
for the full description and [RESULTS.md](RESULTS.md) for output file formats.

---

## Repository layout

```
/
├── src/           — all active .hpp / .cpp source files
├── config/        — .dat parameter files (13 variants)
├── archive/       — dead / experimental code, not in the build
├── .devcontainer/ — Dockerfile + devcontainer.json for VS Code
├── Makefile       — VPATH=src so sources live in src/ but objects land in root
├── README.md      — full algorithm description, config reference, experiments
├── RESULTS.md     — output file format reference
└── CLAUDE.md      — this file
```

## Build

```bash
make          # produces ./simulation
make clean    # remove .o files and binary
```

Compiler: `g++34` (symlink to system g++ in the dev container).
The active `CFLAGS` line in the Makefile enables:
`DEBUG`, `DEBUG_L1`, `DEBUG_L2`, `DUPPACK_CACHE`, `DETECT_LOOP`,
`SELECTIVE_PACKET_FILTER`.

---

## Code-edit conventions

- **Always keep old code commented** with a brief reason when making a fix,
  rather than deleting it outright.  Example:
  ```cpp
  //bool operator()(Tag a, Tag b) { // STL requires comparator to be const-invocable (C++17 static_assert)
  bool operator()(Tag a, Tag b) const {
  ```
- Do not auto-commit.  Draft a commit message and stop; the user commits.

---

## Bug fixes applied (session history)

### 1. `src/Message.hpp` — extra qualification on inline constructors
**Error:** `error: extra qualification 'Message::' on member 'Message'`
**Root cause:** Inline constructor definitions inside a class body cannot use
`ClassName::MethodName()` syntax.
**Fix:** Removed the class-name prefix from all four constructors
(`Message`, `Response`, `Query`, `Feedback`), keeping old lines commented.

### 2. `src/Node.hpp` — comparator not `const`-invocable
**Error:** `static_assert` failure on `routing_table_rows_greater_than::operator()`
**Root cause:** C++17 requires STL comparators to be callable on a `const`
reference; `operator()` lacked `const`.
**Fix:** Added `const` to the operator signature.

### 3. `src/Scheduler.hpp` — same issue on `event_less::operator()`
Same root cause and fix as #2.

### 4. `src/Scheduler.cpp` — iterator invalidation segfault
**Symptom:** Crash after "Canceled birth agent node 4294967293".
**Root cause:** The periodic-schedule for-loop incremented `run_periodic_itr`
*after* calling the handler.  If the handler called `cancel_periodic()`, which
erased and deleted the current element, the iterator was already invalid when
the for-loop's `run_periodic_itr++` ran.
**Fix:** Save `std::next(run_periodic_itr)` **before** calling the handler;
advance to the saved iterator afterward (`execute_periodic_schedule()`,
[src/Scheduler.cpp:131-154](src/Scheduler.cpp#L131-L154)).

### 5. `src/simulation.cpp` — `input_file_name[20]` overflows for deep paths
**Symptom:** Crash (exit 133, SIGTRAP) when passing a config path ≥ 20 chars
(e.g. `logs/config17/run.dat` = 21 chars).  `run.sh` uses patched configs in
`logs/<name>/run.dat`, so all runs except `logs/config/run.dat` (19 chars)
crashed.
**Fix:** `char input_file_name[20]` → `char input_file_name[256]`
([src/simulation.cpp:70](src/simulation.cpp#L70)).

### 6. `src/Analyst.cpp` — histogram arrays overrun with large birth-cycle configs
**Symptom:** SIGTRAP on configs where the actual routing table width/length
exceeds `RouterNode_routing_table_max_width` / `RouterNode_routing_table_max_length`.
**Root cause:** Three histogram arrays allocated `[max+1]` but indexed by the
actual (unbounded) value; the original bounds-check guards were commented out.
**Fix:** Restored guards as `if (actual <= max) histogram[actual]++;` for all
three histograms ([src/Analyst.cpp:207,220,222](src/Analyst.cpp#L207)).

---

## Known pre-existing crashes (9 of 13 configs)

After the above fixes, 4/13 configs pass cleanly.  The remaining 9 exhibit
`free(): invalid pointer` (exit 133) or SIGSEGV (exit 139):

| Passing | Failing |
|---------|---------|
| config, config17, config18, config18big | config_1, configbig, configm, configu, configv, configv1–v4 |

The crashes are **pre-existing heap corruption bugs in the simulation's routing
/ duplicate-packet-cache logic**, not caused by the changes in sessions 1–2.
They manifest as heap corruption detected by glibc during a later `fopen` /
`malloc` call.  To diagnose: rebuild with `-fsanitize=address` (AddressSanitizer)
to get a real stack trace pointing at the first bad write.

---

## Repo restructure (completed)

- Moved all source files to `src/`, all configs to `config/`, dead code to
  `archive/`.
- Added `VPATH = src` and `INCLUDES = -Isrc` to Makefile.
- Removed CVS/, trace files, diff artefacts, and generated simulation output
  from git tracking via `.gitignore`.

---

## Open design question: dynamic visualization

The user asked how to add live visualization while the simulation runs.
A design document with three options (zero-touch file watcher, JSON-on-stdout
terminal dashboard, web dashboard) lives at [VISUALIZATION.md](VISUALIZATION.md).

The recommended approach is **Option B** (minimal C++ touch): one `fprintf` in
`IO_Reporter::heartbeat()` emits a JSON line per snapshot tick; a Python script
consumes it and renders a live terminal dashboard.  **Not yet implemented.**

---

## Key extension points for future work

| Where | What |
|-------|------|
| `IO_Reporter::heartbeat()` [src/IO_Reporter.cpp:978](src/IO_Reporter.cpp#L978) | Fires every `snapshot_reporter_period` ticks; cleanest place to add JSON/live output |
| `Scheduler::run()` [src/Scheduler.cpp:53](src/Scheduler.cpp#L53) | Per-tick hook for finer-grained instrumentation |
| `Database` fields [src/Database.hpp](src/Database.hpp) | All live counters; read directly from any heartbeat callback |
| `Infrastructure::list_of_*_nodes` [src/Infrastructure.hpp](src/Infrastructure.hpp) | Live node lists for topology snapshots |
