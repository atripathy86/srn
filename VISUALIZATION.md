# Dynamic Visualization — Design Options

This document describes three ways to add live visualization to the SRN
simulator as it runs.  The simulator is a single-threaded C++ discrete-event
loop; all options work without restructuring that loop.

---

## Hook points in the existing code

| Location | When it fires | What it can see |
|----------|---------------|-----------------|
| `IO_Reporter::heartbeat()` [src/IO_Reporter.cpp:978](src/IO_Reporter.cpp#L978) | Every `snapshot_reporter_period` ticks (default: 40) | All `Database` counters, node lists, edge list, diameter |
| `Scheduler::run()` [src/Scheduler.cpp:53](src/Scheduler.cpp#L53) | Every tick | Tick number only (other data via `If_ptr`) |
| `Database` fields [src/Database.hpp](src/Database.hpp) | Always up to date | `Queries_sent_from_UserNode`, `Num_of_good_responses_for_UserNode`, duplicate/loop/TTL drops, per-tag breakdowns, … |

`IO_Reporter::heartbeat()` is the natural extension point: it already
aggregates everything and runs at the reporting cadence.

---

## Option A — Zero C++ changes: file watcher + Python

**How it works**

The simulation already writes `T_perfrep` on each heartbeat.  A Python script
watches the working directory for new files matching `*_perfrep`, parses the
counters with regex, and renders a live terminal dashboard.

```bash
# Terminal 1
./simulation config/config.dat

# Terminal 2
python viz_watch.py          # watches cwd for new *_perfrep files
```

**What to parse from `T_perfrep`**

```
Queries_sent_from_UserNode  (query packets) : <N>
Num_of_good_responses_for_UserNode : <N>
Num_total_message_generated_for_UserNode : <N>
Num_duplicate_message_dropped : <N>
```

**Python dependencies**

```
pip install watchdog rich
```

**Pros**
- Zero changes to C++.
- Works with any existing run.

**Cons**
- Parsing fragile human-readable text.
- Updates only at snapshot granularity (every 40 ticks by default).
- Two terminals required.

---

## Option B — Minimal C++ touch: JSON line on stderr (recommended)

**How it works**

Add one `fprintf(stderr, ...)` at the end of `IO_Reporter::heartbeat()` that
emits a single compact JSON line per snapshot tick.  A Python script reads it
from a pipe and renders a live terminal dashboard.

```bash
./simulation config/config.dat 2>&1 >/dev/null | python viz.py
```

Or, keeping stdout (the existing verbose DEBUG log) separate from the JSON
stream:

```bash
./simulation config/config.dat 2>viz_stream.jsonl &
tail -f viz_stream.jsonl | python viz.py
```

**JSON schema (one line per heartbeat)**

```json
{
  "t": 40,
  "nodes": { "res": 20, "rtr": 10, "usr": 5 },
  "queries": 150,
  "good_resp": 87,
  "total_msg": 463,
  "dupes_dropped": 12,
  "loop_dropped": 3,
  "ttl_dropped": 8,
  "diameter": 4,
  "recall_pct": 58.0,
  "overhead": 3.1
}
```

**C++ change (one block added to `IO_Reporter::heartbeat()`)**

```cpp
// At the end of IO_Reporter::heartbeat(), after writing all files:
{
    unsigned int q  = If_ptr->database_ptr->Queries_sent_from_UserNode;
    unsigned int gr = If_ptr->database_ptr->Num_of_good_responses_for_UserNode;
    unsigned int tm = If_ptr->database_ptr->Num_total_message_generated_for_UserNode;
    float recall  = q ? 100.0f * gr / q : 0.0f;
    float overhead = q ? (float)tm / q   : 0.0f;
    fprintf(stderr,
        "{\"t\":%u,\"nodes\":{\"res\":%zu,\"rtr\":%zu,\"usr\":%zu},"
        "\"queries\":%u,\"good_resp\":%u,\"total_msg\":%u,"
        "\"dupes_dropped\":%u,\"loop_dropped\":%u,\"ttl_dropped\":%u,"
        "\"diameter\":%d,\"recall_pct\":%.1f,\"overhead\":%.2f}\n",
        If_ptr->scheduler_ptr->now(),
        If_ptr->list_of_resource_nodes.size(),
        If_ptr->list_of_router_nodes.size(),
        If_ptr->list_of_user_nodes.size(),
        q, gr, tm,
        If_ptr->database_ptr->Num_duplicate_message_dropped,
        If_ptr->database_ptr->Num_looping_message_dropped,
        If_ptr->database_ptr->Num_queries_dropped_for_TTL,
        If_ptr->analyst.diameter,
        recall, overhead);
}
```

**Python consumer sketch**

```python
# viz.py  —  pip install plotext
import sys, json, plotext as plt

ticks, recall, overhead = [], [], []

plt.title("SRN live metrics")
for line in sys.stdin:
    line = line.strip()
    if not line.startswith("{"):
        continue
    d = json.loads(line)
    ticks.append(d["t"])
    recall.append(d["recall_pct"])
    overhead.append(d["overhead"])

    plt.clf()
    plt.subplots(1, 2)
    plt.subplot(1, 1)
    plt.plot(ticks, recall, label="Recall %")
    plt.ylim(0, 100)
    plt.subplot(1, 2)
    plt.plot(ticks, overhead, label="Overhead (msg/query)")
    plt.show()
    print(f"  t={d['t']}  nodes={d['nodes']}  recall={d['recall_pct']:.1f}%  overhead={d['overhead']:.2f}x  diameter={d['diameter']}")
```

**Pros**
- Structured, machine-readable data — trivial to parse.
- Exactly one snapshot per heartbeat, no polling.
- Single small edit to `IO_Reporter.cpp`.
- `plotext` is pure Python; no system dependencies.

**Cons**
- Requires one `fprintf` addition to the C++ source.

---

## Option C — Web dashboard: live JSON file + browser

**How it works**

`IO_Reporter::heartbeat()` overwrites a `live_metrics.json` file in the working
directory on each tick.  A Python HTTP server serves the directory.  A
single-page HTML dashboard fetches the file every second and updates Chart.js
charts.

```bash
./simulation config/config.dat &
python -m http.server 8080 &
open http://localhost:8080/dashboard.html
```

**C++ change**

Same as Option B but writing to a file (`fopen("live_metrics.json","w")`) instead
of `stderr`.  The file is overwritten (not appended) each heartbeat — only the
latest snapshot is needed.

**Browser dashboard sketch**

```html
<!-- dashboard.html -->
<canvas id="recall"></canvas>
<canvas id="overhead"></canvas>
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<script>
const recallChart = new Chart(document.getElementById('recall'), { type: 'line', data: { labels: [], datasets: [{ label: 'Recall %', data: [] }] } });
const overheadChart = new Chart(document.getElementById('overhead'), { type: 'line', data: { labels: [], datasets: [{ label: 'Overhead', data: [] }] } });

setInterval(async () => {
  const d = await fetch('live_metrics.json?_=' + Date.now()).then(r => r.json());
  recallChart.data.labels.push(d.t);
  recallChart.data.datasets[0].data.push(d.recall_pct);
  recallChart.update();
  overheadChart.data.labels.push(d.t);
  overheadChart.data.datasets[0].data.push(d.overhead);
  overheadChart.update();
}, 1000);
</script>
```

**Pros**
- Rich, interactive charts.
- Shareable via browser — works over VS Code port forwarding in the dev container.
- Easily extended (add node-count chart, per-tag heatmap, etc.).

**Cons**
- More moving parts (HTTP server, HTML file, browser).
- Requires port forwarding if running inside the dev container.

---

## Comparison

| | Option A | Option B (recommended) | Option C |
|--|---------|------------------------|----------|
| C++ changes | None | ~15 lines in `IO_Reporter.cpp` | ~15 lines in `IO_Reporter.cpp` |
| Data format | Fragile text parse | Clean JSON | Clean JSON |
| Rendering | Terminal (`rich`) | Terminal (`plotext`) | Browser (Chart.js) |
| Dependencies | `watchdog`, `rich` | `plotext` | None (browser) |
| Interactivity | Low | Low | High |
| Dev-container friendly | Yes | Yes | Yes (port-forward 8080) |

---

## Next steps

To implement Option B:
1. Add the `fprintf(stderr, ...)` block to `IO_Reporter::heartbeat()` in [src/IO_Reporter.cpp](src/IO_Reporter.cpp).
2. Add `viz.py` at the repo root.
3. `pip install plotext` in the dev container (`pip3 install plotext`).
4. Run: `./simulation config/config.dat 2>&1 1>/dev/null | python3 viz.py`

To implement Option C additionally:
5. Add `dashboard.html` at the repo root.
6. Adjust the C++ block to write `live_metrics.json` instead of (or in addition to) `stderr`.
