#!/usr/bin/env python3
"""
SRN live terminal dashboard — Option B visualization.

Usage (pick one):

  # Stream stderr directly through the script
  ./simulation config/config.dat 2>&1 1>/dev/null | python3 viz.py

  # Or capture to a file and tail it (lets you see C++ stdout too)
  ./simulation config/config.dat 2>viz_stream.jsonl &
  tail -f viz_stream.jsonl | python3 viz.py

  # Replay a saved stream at full speed
  python3 viz.py < viz_stream.jsonl

Requires:  pip3 install plotext
"""

import sys
import json

try:
    import plotext as plt
    HAVE_PLOTEXT = True
except ImportError:
    HAVE_PLOTEXT = False
    print("plotext not found — install with:  pip3 install plotext")
    print("Falling back to plain-text output.\n")


# ── rolling history ──────────────────────────────────────────────────────────
MAX_POINTS = 60          # keep at most this many ticks on the chart

ticks    = []
recall   = []
overhead = []
nodes_res = []
nodes_rtr = []
nodes_usr = []


def parse_line(raw):
    """Return a dict if the line is a valid JSON snapshot, else None."""
    raw = raw.strip()
    if not raw.startswith("{"):
        return None
    try:
        return json.loads(raw)
    except json.JSONDecodeError:
        return None


def trim(lst):
    """Keep only the last MAX_POINTS entries."""
    if len(lst) > MAX_POINTS:
        del lst[:-MAX_POINTS]


def render_plotext(d):
    """Redraw the plotext terminal chart."""
    plt.clf()
    plt.theme("dark")
    plt.plotsize(plt.terminal_width(), plt.terminal_height() - 4)

    plt.subplots(2, 2)

    # ── top-left: recall % ────────────────────────────────────────────────
    plt.subplot(1, 1)
    plt.title("User query recall  (%)")
    plt.ylim(0, 100)
    plt.plot(ticks, recall, color="green", label="recall %")

    # ── top-right: message overhead ──────────────────────────────────────
    plt.subplot(1, 2)
    plt.title("Message overhead  (total msg / query)")
    plt.plot(ticks, overhead, color="orange", label="overhead")

    # ── bottom-left: node counts ──────────────────────────────────────────
    plt.subplot(2, 1)
    plt.title("Node counts")
    plt.plot(ticks, nodes_res, color="cyan",   label="resource")
    plt.plot(ticks, nodes_rtr, color="blue",   label="router")
    plt.plot(ticks, nodes_usr, color="yellow", label="user")

    # ── bottom-right: text summary of latest tick ─────────────────────────
    plt.subplot(2, 2)
    plt.title("Latest snapshot")
    summary = [
        f"Tick          : {d['t']}",
        f"Recall        : {d['recall_pct']:.1f}%",
        f"Overhead      : {d['overhead']:.2f}x",
        f"Diameter      : {d['diameter']}",
        f"Queries sent  : {d['queries']}",
        f"Good resp     : {d['good_resp']}",
        f"Dupes dropped : {d['dupes_dropped']}",
        f"Loop dropped  : {d['loop_dropped']}",
        f"TTL dropped   : {d['ttl_dropped']}",
        f"Res nodes     : {d['nodes']['res']}",
        f"Rtr nodes     : {d['nodes']['rtr']}",
        f"Usr nodes     : {d['nodes']['usr']}",
    ]
    # plotext text() places a label at a coordinate; use scatter with no dots
    # as a workaround to display text in the subplot area.
    # We place each line as a separate y=index point (invisible) then label it.
    for i, line in enumerate(summary):
        plt.text(line, x=0, y=len(summary) - i)

    plt.show()


def render_plain(d):
    """Minimal text output when plotext is not available."""
    print(
        f"t={d['t']:>4}  "
        f"res={d['nodes']['res']:>3} rtr={d['nodes']['rtr']:>3} usr={d['nodes']['usr']:>3}  "
        f"recall={d['recall_pct']:>5.1f}%  "
        f"overhead={d['overhead']:>5.2f}x  "
        f"diam={d['diameter']:>3}  "
        f"queries={d['queries']:>6}  good={d['good_resp']:>6}"
    )


def main():
    print("SRN live dashboard — waiting for simulation output …")
    if HAVE_PLOTEXT:
        print("(Ctrl-C to exit)\n")

    for raw in sys.stdin:
        d = parse_line(raw)
        if d is None:
            continue

        # accumulate history
        ticks.append(d["t"])
        recall.append(d["recall_pct"])
        overhead.append(d["overhead"])
        nodes_res.append(d["nodes"]["res"])
        nodes_rtr.append(d["nodes"]["rtr"])
        nodes_usr.append(d["nodes"]["usr"])

        for lst in (ticks, recall, overhead, nodes_res, nodes_rtr, nodes_usr):
            trim(lst)

        if HAVE_PLOTEXT:
            render_plotext(d)
        else:
            render_plain(d)

    # ── end of stream: final summary ──────────────────────────────────────
    if ticks:
        print(f"\nSimulation finished at tick {ticks[-1]}.")
        print(f"Final recall : {recall[-1]:.1f}%")
        print(f"Final overhead : {overhead[-1]:.2f}x")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nDashboard closed.")
