This document records the root causes of simulation slowness and the fixes
applied, along with guidance on getting the most speed out of the simulator.

---

## Bug fixes applied (parser correctness)

These bugs were silently corrupting config values, making the simulation run
with wrong parameters — which in turn caused runaway event counts and apparent
"hangs".

### 1. Config parser: `while(!eof)` overwrites the last-parsed parameter

**Root cause:** The classic C++ "read-past-EOF" bug.  After the last valid
line is consumed, `while(!file.eof())` executes one extra iteration.  In that
iteration `getline` returns an empty string, `>>temp` fails and leaves `temp`
with its previous stack value (the name of the last-parsed parameter), and
`>>value` fails and yields `0`.  The `strcmp` chain then matches the stale
`temp` and writes `0` into the parameter, silently overwriting the value that
was just correctly parsed.

**Symptom:** The last parameter in the config file always reads back as `0`
regardless of its actual value.

**Fix:** `src/IO_Reporter.cpp` — skip the entire `if/else` chain if `>>temp`
fails (empty or EOF line):
```cpp
// Old: (*ss_ptr)>>temp; // then unconditionally fell into if/else chain
if (!((*ss_ptr)>>temp)) { delete ss_ptr; continue; } // skip blank / unreadable lines
```

### 2. Config parser: CRLF line endings garble all values

**Root cause:** All config files were saved with Windows CRLF (`\r\n`) line
endings.  On Linux, `getline` strips `\n` but leaves `\r` in the string.
When `>>temp` reads a token from `"param_name value\r"`, the `\r` is treated
as whitespace and absorbed into the stream — but subsequent `>>value` reads
fail in inconsistent ways depending on where the `\r` fell relative to the
value token.  The result was that most parameters silently fell through to
their hard-coded defaults.

**Symptom:** `network_struct_report_required` always read as `1` (the struct
analysis default) even when the config said `0`; `Birth_UserNode_period`
always read as `1` (not the config value of `20`), causing one new user node
per tick instead of one per 20 ticks, which made the event queue explode.

**Fix (parser):** `src/IO_Reporter.cpp` — strip `\r` and truncate at `#`
before handing the line to `istringstream`:
```cpp
// Strip Windows-style \r
if (!smline.empty() && smline[smline.size()-1] == '\r')
    smline.erase(smline.size()-1);
// Truncate at '#' so inline comments don't interfere with value parsing
std::string::size_type hash_pos = smline.find('#');
if (hash_pos != std::string::npos)
    smline.erase(hash_pos);
```

**Fix (files):** All 13 `config/*.dat` files converted from CRLF to LF with a
one-time Python script.  They are now stored as LF in the repository.

### Net effect of both fixes

Before fixes: simulation appeared to hang; after 120 s it had not advanced
past tick 1.

After fixes: a full 202-tick run completes in **under 30 seconds** with all
config values reading correctly.

---

## Remaining bottleneck: `network_struct_report_required`

When set to `1` (formerly the accidental default due to bug #2 above),
`IO_Reporter::heartbeat()` calls `Analyst::generate_distance_matrix()` every
`snapshot_reporter_period` ticks.  This runs an all-pairs shortest-path
(Bellman-Ford via Boost) on the full router+resource node graph — O(V · E)
per call.  With 8 nodes and a dense routing graph this still dominates runtime
on large runs.

**Fix:** set `network_struct_report_required 0` in the config (already the
default in `config/config.dat`).  The structural report (distance matrix,
diameter, parent table) will not be written, but the performance report,
routing table dump, and JSON visualization stream are unaffected.

---