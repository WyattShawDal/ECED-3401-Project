# Performance optimization report

This document highlights the main performance risks in the current codebase and recommends optimizations, with emphasis on the large-grid slowdown described in the issue.

## Observed bottlenecks

1. **Full-frame grid rendering**
   - `UpdateDrawFrame` calls `UpdateMap` every frame. `UpdateMap` iterates `MAX_COLS * MAX_ROWS` tiles and calls `DrawMap` for each, even when the camera is zoomed into a small region.
   - For an input of `xLength` by `yLength` buildings, the grid becomes `(4 * xLength + 1) * (4 * yLength + 1)` tiles. A 200 x 200 building map produces ~640k tiles, triggering that many draw calls and multiple `TextFormat` allocations per frame.
   - Tile rendering also issues per-tile labels (row/column counters and NESW markers) and gridlines, multiplying CPU and GPU work and stressing VRAM bandwidth.

2. **Pathfinding allocations and search cost**
   - `pathCalculation` (BFS) allocates a new `TileNode` for every explored tile and a new `InstructionNode` chain per path. `clearBFS` frees everything after each call, so every route recalculates and reallocates.
   - `searchQueue` linearly scans the visited queue, turning visited checks into O(n) over an already O(n) BFS, which hurts on large grids and with many simultaneous AEDVs.
   - Because paths are recomputed even for common routes (e.g., stable to frequent buildings), allocator churn and cache misses grow quickly with grid size.

3. **Memory layout of the grid**
   - `AllocDynamicMap` creates `MAX_COLS` separate heap allocations (pointer-of-pointer). Traversals in `UpdateMap` and BFS touch memory non-contiguously, increasing cache misses. For large grids, allocator overhead and TLB pressure also rise.

4. **Per-frame string formatting**
   - `DrawMap` repeatedly calls `TextFormat` for NESW labels and counters every frame. These transient allocations add avoidable overhead, especially when multiplied by large tile counts.

## Recommended improvements

### High-impact, low-risk (quick wins)
- **View frustum culling:** Compute the visible tile bounds from `camera.target`, `camera.zoom`, and the window size, and only iterate/draw tiles in view. Keep row/column labels in sync with the visible range.
- **Static map rendering:** Render the static road/building layer once to a `RenderTexture2D` after `InitTiles`, then draw that texture each frame. Only dynamic overlays (vehicles, highlights) need per-frame drawing.
- **Cache labels:** Precompute row/column strings and NESW labels per tile (or per modulo pattern) and reuse them instead of calling `TextFormat` inside the render loop.
- **Reduce draw calls:** Batch gridlines or omit them when zoomed out. Consider drawing block outlines instead of per-tile lines at low zoom levels.
- **Visited lookup speedup:** Replace `searchQueue`’s linear scan with a boolean visited bitmap or an `int` array keyed by tile index to make BFS membership checks O(1).

### Medium-term algorithmic gains
- **Pathfinding reuse:** Cache common routes (e.g., stable ↔ frequent buildings) and reuse instruction lists when destinations repeat. Alternatively, keep a small LRU of recent paths keyed by start/destination coordinates.
- **A* instead of plain BFS:** Use Manhattan distance on grid coordinates to prioritize exploration and shrink the search space, especially on large, sparse maps.
- **Memory pooling:** Preallocate `TileNode` and `InstructionNode` pools sized to the grid to avoid frequent malloc/free churn inside `pathCalculation`.
- **Contiguous grid storage:** Allocate `dynamicMap` as a single contiguous `Tile` array and index with `dynamicMap[y * MAX_COLS + x]` to improve cache locality for both rendering and navigation.
- **Decouple tick and render rates:** Keep simulation at `frameRate` ticks, but lower the render rate when the camera is stationary or when the map is static to buy headroom for large grids.

### Instrumentation and validation
- Add timing around `UpdateMap`, `pathCalculation`, and vehicle drawing using `GetFrameTime()` or `clock_gettime` and log via `TraceLog(LOG_INFO, ...)`.
- Track heap allocations during pathfinding (counts and bytes) to size pools appropriately.
- When culling is implemented, validate that tile counts drawn scale with zoomed viewport rather than total map size.

## Expected impact on large grids

Applying view culling and static map rendering should reduce per-frame work from O(grid area) to approximately O(visible area + active vehicles), restoring responsiveness even when `MAX_COLS`/`MAX_ROWS` are large. Pathfinding pooling and faster visited checks will cut allocator overhead and CPU time for routing, particularly when multiple AEDVs compute paths concurrently.
