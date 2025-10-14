/*
    Alpha Donut
    Made with the help of https://www.a1k0n.net/2011/07/20/donut-math.html
*/

/* ---------- config ---------- */
W = 77; 
H = 61;

PI = 3.141592653589793;
theta_spacing = 0.07;   /* radians */
phi_spacing   = 0.02;   /* radians */

R1 = 1; R2 = 2; K2 = 5;
K1 = W * K2 * 1.25 / (8 * (R1 + R2));

LCHARS = [".", ",", "-", "~", ":", ";", "=", "!", "*", "#", "$", "@"];

/* ----- helpers ----- */
function zeros(h, w){ g=[]; for(y=0;y<h;++y){ r=[]; for(x=0;x<w;++x) r[x]=0; g[y]=r; } return g; }
function fillChar(h, w, ch){ g=[]; for(y=0;y<h;++y){ r=[]; for(x=0;x<w;++x) r[x]=ch; g[y]=r; } return g; }
function iclamp(v, lo, hi){ if(v<lo) return lo; if(v>hi) return hi; return v; }

/* radians → degrees trig wrappers */
RAD2DEG = 180.0 / PI;
function s(x){ return sin(x * RAD2DEG); }
function c(x){ return cos(x * RAD2DEG); }

/* ---------- renderer ---------- */
function render_frame(A, B) {
    cosA = c(A); sinA = s(A);
    cosB = c(B); sinB = s(B);

    out  = fillChar(H, W, " ");
    zbuf = zeros(H, W);

    for (theta = 0.0; theta < 2*PI; theta = theta + theta_spacing) {
        costh = c(theta); sinth = s(theta);

        for (phi = 0.0; phi < 2*PI; phi = phi + phi_spacing) {
            cosph = c(phi); sinph = s(phi);

            circlex = R2 + R1 * costh;
            circley = R1 * sinth;

            x = circlex*(cosB*cosph + sinA*sinB*sinph) - circley*cosA*sinB;
            y = circlex*(sinB*cosph - sinA*cosB*sinph) + circley*cosA*cosB;
            z = K2 + cosA*circlex*sinph + circley*sinA;

            if (z <= 0.0) continue;
            ooz = 1.0 / z;

            xp = trunc(W/2 + K1 * ooz * x);
            yp = trunc(H/2 - K1 * ooz * y);
            if (xp < 0 or xp >= W or yp < 0 or yp >= H) continue;

            L = cosph*costh*sinB - cosA*costh*sinph - sinA*sinth
                + cosB*(cosA*sinth - costh*sinA*sinph);
            if (L <= 0.0) continue;

            if (ooz > zbuf[yp][xp]) {
                zbuf[yp][xp] = ooz;
                idx = trunc(L * 8);
                if (idx < 0) idx = 0;
                if (idx >= objecttotalmembers(LCHARS)) idx = objecttotalmembers(LCHARS)-1;
                out[yp][xp] = LCHARS[idx];
            }
        }
    }

    print("\x1b[H");
    for (j = 0; j < H; ++j) {
        line = "";
        for (i = 0; i < W; ++i) line = line + out[j][i];
        print(line, "\n");
    }
}

/* main loop */
A = 0.0; B = 0.0;
print("\x1b[2J\x1b[3J\x1b[H");
while (true) {
    render_frame(A, B);
    A = A + 0.2;
    B = B + 0.1;
}
