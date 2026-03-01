#!/usr/bin/env bash
# run.sh — build (if needed) and run the simulation for every config/*.dat
# Each run gets its own subdirectory under logs/<config-name>/
#   logs/<name>/stdout.txt  — console output
#   logs/<name>/*_perfrep   — performance snapshots
#   logs/<name>/*_strucrep  — structural reports
#   etc.

set -uo pipefail   # note: no -e; individual run failures are logged, not fatal

IMAGE=srn
LOGDIR=logs
FAILED=()

# ── 1. Always do a clean build inside Docker ──────────────────────────────────
# Make has no header-dependency tracking, so a stale binary from before a
# struct change (e.g. Input::output_dir) will silently heap-corrupt.
echo "Building simulation (clean)..."
docker run --rm \
    -v "$(pwd)":/workspace \
    -w /workspace \
    "$IMAGE" \
    bash -c "make clean && make"

mkdir -p "$LOGDIR"

# ── 2. Run each config ────────────────────────────────────────────────────────
total=$(ls config/*.dat | wc -l)
idx=0

for cfg in config/*.dat; do
    name=$(basename "$cfg" .dat)
    outdir="$LOGDIR/$name"
    idx=$((idx + 1))

    mkdir -p "$outdir"

    # Write a patched config that redirects output_dir into logs/<name>/
    sed "s|^output_dir .*|output_dir $outdir|" "$cfg" > "$outdir/run.dat"

    printf "[%s] (%d/%d) %-20s → %s/\n" \
        "$(date +%T)" "$idx" "$total" "$name" "$outdir"

    docker run --rm \
        -v "$(pwd)":/workspace \
        -w /workspace \
        "$IMAGE" \
        ./output/simulation "$outdir/run.dat" \
        > "$outdir/stdout.txt" 2>&1

    rc=$?
    if [ $rc -ne 0 ]; then
        echo "             FAILED (exit $rc) — see $outdir/stdout.txt"
        FAILED+=("$name")
    else
        echo "             done"
    fi
done

# ── 3. Summary ────────────────────────────────────────────────────────────────
echo ""
if [ ${#FAILED[@]} -eq 0 ]; then
    echo "All $total runs complete. Results in $LOGDIR/"
else
    echo "${#FAILED[@]}/$total runs FAILED: ${FAILED[*]}"
    echo "Successful results in $LOGDIR/"
    exit 1
fi
