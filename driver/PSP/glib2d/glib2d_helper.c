#include "glib2d_helper.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <pspgu.h>

static struct {
    g2dImage* current_tex;
    bool is_batching;
} _batch = {
    .current_tex = NULL,
    .is_batching = false
};

static void _flush_batch(void) {
    if (_batch.is_batching) {
        g2dEnd();
        _batch.current_tex = NULL;
        _batch.is_batching = false;
    }
}

static void _check_batch(g2dImage* tex) {
    if (_batch.is_batching && _batch.current_tex != tex) {
        _flush_batch();
    }

    if (!_batch.is_batching) {
        g2dBeginRects(tex);
        _batch.current_tex = tex;
        _batch.is_batching = true;
    }
}

void g2dHelperClear(g2dColor color) {
    _flush_batch();
    g2dClear(color);
}

void g2dHelperFlip(void) {
    _flush_batch();
    g2dFlip(G2D_VSYNC);
}

void g2dHelperFlushBatch(void) {
    if (_batch.is_batching) {
        g2dEnd();
        _batch.current_tex = NULL;
        _batch.is_batching = false;
    }
}

void g2dHelperFillRect(int x, int y, int w, int h, g2dColor color) {
    _check_batch(NULL);
    g2dReset();
    g2dSetColor(color);
    g2dSetCoordXY(x, y);
    g2dSetScaleWH(w, h);
    g2dAdd();
}

void g2dHelperDrawRect(int x, int y, int w, int h, g2dColor color) {
    g2dHelperFillRect(x, y, w, 1, color);
    g2dHelperFillRect(x, y + h - 1, w, 1, color);
    g2dHelperFillRect(x, y + 1, 1, h - 2, color);
    g2dHelperFillRect(x + w - 1, y + 1, 1, h - 2, color);
}

void g2dHelperDrawImage(g2dImage* tex, int x, int y, int w, int h, g2dColor color, int srcx, int srcy, int srcw, int srch) {
    _check_batch(tex);
    g2dReset();
    g2dSetColor(color);
    g2dSetCoordXY(x, y);
    g2dSetCropXY(srcx, srcy);
    g2dSetCropWH(srcw, srch);
    g2dSetScaleWH(w, h);
    g2dAdd();
}

void g2dHelperDrawTriangles(const float* xy, int count, g2dColor color) {
    if (!xy || count < 3 || (count % 3) != 0) return;

    _flush_batch();

    struct Vertex {
        g2dColor color;
        short x, y;
        short z;
    };

    struct Vertex* v = (struct Vertex*) sceGuGetMemory(count * sizeof(struct Vertex));
    if (!v) return;

    for (int i = 0; i < count; i++) {
        v[i].color = color;
        v[i].x = (short) (xy[2 * i]);
        v[i].y = (short) (xy[2 * i + 1]);
        v[i].z = 0.0f;
    }

    sceGuDisable(GU_TEXTURE_2D);
    sceGuEnable(GU_BLEND);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

    sceGuDrawArray(GU_TRIANGLES, GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D, count, NULL, v);
}

#ifdef __cplusplus
}
#endif