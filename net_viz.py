#!/usr/bin/env python3
"""
net_viz.py — Render Pajek .net files from SRN simulation output to PNG images.

Usage:
    python net_viz.py <input_dir> [output_dir]

    <input_dir>   directory containing *.net files (e.g. output/ or logs/config/)
    <output_dir>  where to write PNGs (default: <input_dir>/png/)

Examples (Docker):
    # Visualize one run
    docker run --rm -v $(pwd):/workspace -w /workspace srn \\
        python net_viz.py output/ output/png/

    # Visualize all runs produced by run.sh
    for d in logs/*/; do
        docker run --rm -v $(pwd):/workspace -w /workspace srn \\
            python net_viz.py "$d" "${d}png/"
    done

Each .net file produces one PNG named <stem>.png.
Node size scales with degree. Nodes are positioned with a spring layout.
"""

import sys
import math
from pathlib import Path

try:
    import networkx as nx
except ImportError:
    sys.exit("networkx not installed — rebuild the Docker image (Dockerfile now includes it).")

try:
    import matplotlib
    matplotlib.use("Agg")           # non-interactive; no display required
    import matplotlib.pyplot as plt
except ImportError:
    sys.exit("matplotlib not installed — rebuild the Docker image.")


# ── Pajek parser ──────────────────────────────────────────────────────────────

def parse_pajek(path: Path) -> nx.DiGraph:
    """Read a Pajek *Vertices / *Arcs / *Edges file into a DiGraph."""
    G = nx.DiGraph()
    section = None

    with open(path) as fh:
        for line in fh:
            line = line.strip()
            if not line:
                continue
            low = line.lower()
            if low.startswith("*vertices"):
                section = "vertices"
            elif low.startswith("*arcs"):
                section = "arcs"
            elif low.startswith("*edges"):
                section = "edges"
            elif section == "vertices":
                parts = line.split()
                G.add_node(int(parts[0]))
            elif section == "arcs":
                parts = line.split()
                G.add_edge(int(parts[0]), int(parts[1]),
                           weight=float(parts[2]) if len(parts) > 2 else 1.0)
            elif section == "edges":
                # undirected — add both directions
                parts = line.split()
                u, v = int(parts[0]), int(parts[1])
                w = float(parts[2]) if len(parts) > 2 else 1.0
                G.add_edge(u, v, weight=w)
                G.add_edge(v, u, weight=w)

    return G


# ── renderer ──────────────────────────────────────────────────────────────────

def render(net_path: Path, out_path: Path) -> None:
    G = parse_pajek(net_path)
    n = G.number_of_nodes()
    e = G.number_of_edges()

    fig_size = max(8, math.ceil(math.sqrt(n) * 1.5))
    fig, ax = plt.subplots(figsize=(fig_size, fig_size))

    # spring layout — deterministic seed for reproducibility
    k = 2 / math.sqrt(n) if n > 1 else 1.0
    pos = nx.spring_layout(G, seed=42, k=k)

    degrees = dict(G.degree())
    node_sizes = [80 + 15 * degrees.get(nd, 0) for nd in G.nodes()]

    nx.draw_networkx(
        G, pos, ax=ax,
        node_size=node_sizes,
        node_color="steelblue",
        edge_color="#aaaaaa",
        font_size=7,
        font_color="white",
        arrows=True,
        arrowsize=8,
        width=0.5,
    )

    ax.set_title(f"{net_path.name}  ({n} nodes, {e} arcs)", fontsize=10)
    ax.axis("off")
    plt.tight_layout()
    plt.savefig(out_path, dpi=150, bbox_inches="tight")
    plt.close(fig)

    print(f"  {net_path.name:<30}  {n:>3} nodes  {e:>4} arcs  →  {out_path}")


# ── main ──────────────────────────────────────────────────────────────────────

def main() -> None:
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    in_dir  = Path(sys.argv[1])
    out_dir = Path(sys.argv[2]) if len(sys.argv) > 2 else in_dir / "png"

    if not in_dir.is_dir():
        sys.exit(f"Not a directory: {in_dir}")

    net_files = sorted(in_dir.glob("*.net"))
    if not net_files:
        sys.exit(f"No .net files found in {in_dir}")

    out_dir.mkdir(parents=True, exist_ok=True)
    print(f"Rendering {len(net_files)} .net file(s) from {in_dir}/ → {out_dir}/\n")

    for f in net_files:
        render(f, out_dir / (f.stem + ".png"))

    print(f"\nDone — {len(net_files)} PNG(s) written to {out_dir}/")


if __name__ == "__main__":
    main()
