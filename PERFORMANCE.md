# Performance Notes

## Why the simulator uses only one CPU core

The simulator is a **single-threaded discrete-event loop**:

```
Scheduler::run()
  └─ while (ticks < max)
       ├─ execute_event_schedule()   // fire all events at current tick
       ├─ execute_periodic_schedule() // fire all periodic handlers
       └─ ticks++
```

Events are processed **one at a time** from a sorted `std::multiset`.
Parallelising this loop would require:

1. **Partitioning shared state** — `Database` counters, node routing tables,
   and node lists are read and written by many events.  Concurrent access
   needs locks or lock-free structures throughout.
2. **Synchronising the logical clock** — `ticks` is a global counter; every
   event handler calls `scheduler_ptr->now()`.
3. **Handling causal dependencies** — event B often reads state written by
   event A at the same tick; out-of-order execution produces wrong results.

True parallel DES (PDES) protocols such as **Time Warp** (optimistic) or
**Chandy-Misra-Bryant** (conservative) solve this but require a major
redesign.  This is a research-level undertaking and out of scope for this
codebase.

---

## Practical speedup options (no code changes needed)

| Option | How | Expected gain |
|--------|-----|---------------|
| `network_struct_report_required 0` | Set in config (already default) | 5–50× on larger networks |
| Release build (no DEBUG flags) | Active CFLAGS in Makefile already does this | 10–100× vs debug build |
| Run a parameter sweep in parallel | `for f in config/*.dat; do ./simulation "$f" 2>>stream.jsonl & done` | Linear with CPU count |
| Reduce `run_for_ticks` | Stop earlier once routing tables converge (use viz to judge) | Proportional |
| Increase `snapshot_reporter_period` | Fewer heartbeat file-writes per run | Minor |

### Parallel sweep with a single dashboard

All config variants emit to a shared JSONL stream and `viz.py` shows all runs
together:

```bash
rm -f sweep.jsonl
for f in config/*.dat; do
    ./simulation "$f" 2>>sweep.jsonl >/dev/null &
done
wait
python3 viz.py < sweep.jsonl
```

Or live (as runs proceed):

```bash
rm -f sweep.jsonl
for f in config/*.dat; do
    ./simulation "$f" 2>>sweep.jsonl >/dev/null &
done
tail -f sweep.jsonl | python3 viz.py
```
