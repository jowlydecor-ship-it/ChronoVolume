#include "chrono_core.h"
#include <cstdio>
#include <cstdlib>
using namespace chrono;

int main() {
    const int W = 320, H = 180, N = 24;
    std::vector<std::vector<float>> data(N, std::vector<float>(W * H * 4));
    for (int f = 0; f < N; f++) {
        float cx = 40 + f * 10.f, cy = 90 + 40 * std::sin(f * 0.35f);
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++) {
                float* p = &data[f][((size_t)y * W + x) * 4];
                float g = 0.15f + 0.1f * x / W;
                float d = std::hypot(x - cx, y - cy);
                float disc = d < 18 ? 1.f : 0.f;
                p[0] = g + disc * (0.9f - g); p[1] = g + disc * (0.6f - g) ; p[2] = g + disc * (0.2f - g); p[3] = 1.f;
            }
    }
    std::vector<FrameView> views(N);
    for (int f = 0; f < N; f++) {
        // newest first (j=0 front)
        FrameView& v = views[N - 1 - f];
        v.base = (const char*)data[f].data(); v.rowBytes = W * 16; v.w = W; v.h = H;
    }
    struct Case { const char* name; Params p; };
    std::vector<Case> cases;
    { Params p; p.motionOnly = true; p.opacitySrc = 0; p.density = 10; p.glowAmt = 0.4f; cases.push_back({"ghost", p}); }
    { Params p; p.mode = 2; p.opacitySrc = 0; p.before = 23; cases.push_back({"slices", p}); }
    { Params p; p.mode = 3; p.crossSlopeX = 1; cases.push_back({"cross", p}); }
    { Params p; p.mode = 0; p.opacitySrc = 0; p.yaw = 35; p.pitch = 20; cases.push_back({"solid", p}); }
    { Params p; p.bend = 1.f; p.pitch = 55; p.yaw = 0; p.depth = 2.5f; p.opacitySrc = 0; p.density = 4; p.trailFade = 0; cases.push_back({"ring", p}); }
    { Params p; p.style = 1; p.motionOnly = true; p.opacitySrc = 0; p.density = 12; p.caAmt = 4; p.grain = 0.05f; cases.push_back({"dust", p}); }
    for (auto& c : cases) {
        std::vector<float> out;
        renderFrame(c.p, views, 480, 270, 1.f, 1, out);
        char fn[64]; snprintf(fn, 64, "out_%s.ppm", c.name);
        FILE* f = fopen(fn, "wb"); fprintf(f, "P6\n480 270\n255\n");
        for (int y = 269; y >= 0; y--) for (int x = 0; x < 480; x++) {
            const float* p = &out[((size_t)y * 480 + x) * 4];
            // composite over checker-ish dark grey to see alpha
            for (int k = 0; k < 3; k++) { float v = p[k] + (1 - p[3]) * 0.08f; fputc((int)(255 * std::min(1.f, std::max(0.f, v))), f); }
        }
        fclose(f);
        printf("rendered %s\n", c.name);
    }
}
