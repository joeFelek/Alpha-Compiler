/*
    Conway’s Game of Life in Alpha 
    https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
*/

/* ---------- config ---------- */
W = 43;             /* width  */
H = 35;             /* height */
DELAY = 1000/60;    /* ~60 FPS in ms */
DENSITY = 25;       /* % random alive at start (0..100) */
FLIP_EVERY = 30;    /* gens between flips */ 
FLIP_CELLS = 30;    /* cells to flip each time */
gen = 0;

/* ---------- utils ---------- */
function newGrid(w, h) {
    g = [];
    for (y = 0; y < h; ++y) {
        row = [];
        for (x = 0; x < w; ++x) row[x] = 0;
        g[y] = row;
    }
    return g;
}
function wrap(i, max) { return (i + max) % max; }

/* ---------- global buffers ---------- */
board = newGrid(W, H);      /* 0/1 Life grid */
next  = newGrid(W, H);      /* next generation */
h3    = newGrid(W, H);      /* horizontal 3-wide sums */
h5    = newGrid(W, H);      /* horizontal 5-wide sums */
heat  = newGrid(W, H);      /* 5x5 density for color (0..24) */

/* ---------- colors (simple 5-band ramp) ---------- */
PURPLE = "\x1b[38;5;55m";
BLUE   = "\x1b[38;5;25m";
TEAL   = "\x1b[38;5;37m";
GREEN  = "\x1b[38;5;77m";
YELLOW = "\x1b[38;5;226m";
RESET  = "\x1b[0m";

function bandColor(s) {           /* s in [0,1] */
    if (s < 0.20) return PURPLE;
    if (s < 0.40) return BLUE;
    if (s < 0.60) return TEAL;
    if (s < 0.80) return GREEN;
    return YELLOW;
}

/* ---------- fast Life step (sliding windows) ---------- */
function step(curr, next) {
    /* pass 1: horizontal rolling sums (3 and 5) */
    for (y = 0; y < H; ++y) {
        s3 = curr[y][W-1] + curr[y][0] + curr[y][1 % W];
        s5 = curr[y][(W-2+W)%W] + curr[y][(W-1+W)%W] + curr[y][0] + curr[y][1 % W] + curr[y][2 % W];

        for (x = 0; x < W; ++x) {
            h3[y][x] = s3;  /* centered at x */
            h5[y][x] = s5;

            left3 = curr[y][(x-1+W)%W];
            add3  = curr[y][(x+2)%W];
            s3 = s3 - left3 + add3;

            left5 = curr[y][(x-2+W)%W];
            add5  = curr[y][(x+3)%W];
            s5 = s5 - left5 + add5;
        }
    }

    /* pass 2: vertical rolling sums → 3x3 neighbors, 5x5 density */
    for (x = 0; x < W; ++x) {
        v3 = h3[H-1][x] + h3[0][x] + h3[1 % H][x];
        v5 = h5[(H-2+H)%H][x] + h5[(H-1+H)%H][x] + h5[0][x] + h5[1 % H][x] + h5[2 % H][x];

        for (y = 0; y < H; ++y) {
            sum3 = v3;                  /* includes center */
            n    = sum3 - board[y][x];  /* neighbors 0..8 */

            a = board[y][x];
            if (a == 1) {
                if (n == 2 or n == 3) next[y][x] = 1; else next[y][x] = 0;
            } else {
                if (n == 3) next[y][x] = 1; else next[y][x] = 0;
            }

            heat[y][x] = v5 - board[y][x];  /* 0..24 */

            v3 = v3 - h3[(y-1+H)%H][x] + h3[(y+2)%H][x];
            v5 = v5 - h5[(y-2+H)%H][x] + h5[(y+3)%H][x];
        }
    }
}

/* ---------- draw ---------- */
function draw(g) {
    print("\x1b[H");
    for (y = 0; y < H; ++y) {
        line = "";
        for (x = 0; x < W; ++x) {
            if (g[y][x] == 1) {
                s = heat[y][x] / 24.0;
                line = line + bandColor(s) + "ϙ " + RESET;
            } else {
                line = line + "  ";
            }
        }
        print(line, "\n");
    }
    print("Generation: ", gen, "\n");
}

/* ---------- drizzle to keep action going ---------- */
function flip(b) {
    if (gen % FLIP_EVERY == 0) {
        for (k = 0; k < FLIP_CELLS; ++k) {
            rx = trunc((W/2 + random(-(W-2)/2, (W-2)/2)) % W);
            ry = trunc((H/2 + random(-(H-2)/2, (H-2)/2)) % H);
            b[ry][rx] = 1 - b[ry][rx];
        }
    }
}

/* ---------- init ---------- */
for (y = 0; y < H; ++y)
  for (x = 0; x < W; ++x)
    if (random(0, 100) < DENSITY)
        board[y][x] = 1;
    else 
        board[y][x] = 0;

print("\x1b[2J\x1b[3J\x1b[H");  /* clear + home */

/* ---------- main loop ---------- */
while (true) {
    draw(board);
    flip(board);
    step(board, next);

    tmp = board; board = next; next = tmp;
    gen = gen + 1;

    sleep(DELAY);
}
