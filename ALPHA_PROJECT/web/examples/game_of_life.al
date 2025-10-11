W = 47;             /* width  */
H = 62;             /* height */
DELAY = 200000;     /* delay between generations */
gen = 0;

function newGrid(w, h) {
    g = [];
    for (y = 0; y < h; ++y) {
        row = [];
        for (x = 0; x < w; ++x) row[x] = 0;
        g[y] = row;
    }
    return g;
}

function wrap(i, max) { 
    return (i + max) % max; 
}

function neighbors(g, x, y) {
    cnt = 0;
    for (dy = -1; dy <= 1; ++dy)
        for (dx = -1; dx <= 1; ++dx) {
            if (dx == 0 and dy == 0) continue;
            nx = wrap(x + dx, W);
            ny = wrap(y + dy, H);
            if (g[ny][nx] != 0) cnt = cnt + 1;
        }
    return cnt;
}

function step(curr, next) {
    for (y = 0; y < H; ++y) {
        for (x = 0; x < W; ++x) {
            n = neighbors(curr, x, y);
            a = curr[y][x];
            if (a != 0) {
                if (n == 2 or n == 3) next[y][x] = 1;
                else next[y][x] = 0;
            } else {
                if (n == 3) next[y][x] = 1;
                else next[y][x] = 0;
            }
        }
    }
}

function draw(g) {
    print("\x1b[H");
    for (y = 0; y < H; ++y) {
        for (x = 0; x < W; ++x)
            if (g[y][x] != 0) print("# "); else print("  ");
        print("\n");
    }
    print("Generation: ", gen, "\n");
}

/* ----- init ----- */
DENSITY = 25;     // % random alive at start (0..100)
DRIZZLE_EVERY = 35;
DRIZZLE_CELLS = 30;

board = newGrid(W, H);
next  = newGrid(W, H);

/* pseudo-randomly fill board */
for (y = 0; y < H; ++y)
  for (x = 0; x < W; ++x)
    if ((x*y + x + 17*y + 5) % 100 < DENSITY) 
      board[y][x] = 1; else board[y][x] = 0;

/* ----- run ----- */
print("\x1b[2J\x1b[3J\x1b[H");  // clear screen

for (;true;) {
    draw(board);
    step(board, next);

    tmp = board; board = next; next = tmp;
    gen = gen + 1;

    if (gen % DRIZZLE_EVERY == 0) {
        /* flip a few cells near center to sustain motion */
        cx = W / 2; cy = H / 2;
        for (k = 0; k < DRIZZLE_CELLS; ++k) {
            rx = (cx + (k*7 + 3) % (W - 2)) % W;
            ry = (cy + (k*11 + 5) % (H - 2)) % H;
            board[ry][rx] = 1 - board[ry][rx];
        }
    }

    for (d = 0; d < DELAY; ++d) { }
}
