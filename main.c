#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MAX_VERTICES 100000

#define ROTATE_XY 0
#define ROTATE_XZ 1
#define ROTATE_YZ 2

struct RGB {
    int r;
    int g;
    int b;
};

struct vec3 {
    long double x;
    long double y;
    long double z;
};

struct vec2 {
    long double x;
    long double y;
};

struct Rotation { // In Radians
    long double xy;
    long double xz;
    long double yz;
};

struct Object {
    struct vec3 *vs;
    int vertices;
    struct Rotation rotation;
    struct vec3 displacement;
};

struct Object sphere(long double radius, int parts);

struct vec2 translate(const struct vec3 a);

void rotate(struct vec3 *verticies, long double angle, int rotate);

int main(int argc, char **argv) {

    struct Object sh = sphere(0.5, 300);

    sh.displacement = (struct vec3) {0, 0, 1};

    int w = 600;
    int h = 600;

    long double angleSpeed = 0.05;

    for (int frame = 0; frame <= 100; frame++) {\

        printf("FRAME: %d\n", frame);

        char *s = (char*)malloc(32 * sizeof(char));
        sprintf(s, "image%03d.ppm", frame);
        FILE *fp = fopen(s, "wb");
        fprintf(fp, "P6\n");
        fprintf(fp, "%d %d\n", w, h);
        fprintf(fp, "255\n");

        rotate(sh.vs, angleSpeed, ROTATE_XZ);

        struct vec2 tr[MAX_VERTICES];
        struct vec2 m_points[MAX_VERTICES];

        struct RGB r[h][w];

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                r[x][y] = (struct RGB) {0x00, 0x00, 0x00};
            }
        }

        for (int i = 0; i < sh.vertices; i++) {
            struct vec3 u = (struct vec3) {sh.vs[i].x + sh.displacement.x, sh.vs[i].y + sh.displacement.y, sh.vs[i].z + sh.displacement.z};
            tr[i] = translate(u);
            m_points[i] = (struct vec2) {(tr[i].x + 1.0) / 2 * w, (tr[i].y + 1.0) / 2 * h};
            if ((int) m_points[i].x >= 0 && (int) m_points[i].x < w && (int) m_points[i].y >= 0 && (int) m_points[i].y < h) {
                r[(int) m_points[i].x][(int) m_points[i].y] = (struct RGB) {0xFF, 0x00, 0x00};
            }
        }

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                putc(r[x][y].r, fp);
                putc(r[x][y].g, fp);
                putc(r[x][y].b, fp);
            }
        }
        fclose(fp);
    }
}

struct Object parallelepiped(long double w, long double u, long double h) {

}

struct Object sphere(long double radius, int parts) {
    long double delta = 2 * M_PI / (long double) parts;
    struct vec3 *points = (struct vec3*)malloc(MAX_VERTICES * sizeof(struct vec3));
    struct vec3 *b = points;
    for (int i = 0; i < parts; i++) {
        long double phi = i * delta;
        for (int j = 0; j < parts; j++) {
            long double teta = j * delta;
            long double x, y, z;
            x = radius * cos(teta) * cos(phi);
            y = radius * cos(teta) * sin(phi);
            z = radius * sin(teta);
            *points = (struct vec3) {x, y, z};
            points++;
        }
    }

    return (struct Object) {b, points - b, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
}

struct vec2 translate(const struct vec3 a) {
    return (struct vec2) {a.x / a.z, a.y / a.z};
}

void rotate(struct vec3 *verticies, long double angle, int rotate) {
    struct vec3 * out = (struct vec3*)malloc(MAX_VERTICES * sizeof(struct vec3));
    if (rotate == ROTATE_XY) {

    } else if (rotate == ROTATE_XZ) {
        for (int i = 0; i < MAX_VERTICES; i++) {
            long double x = verticies[i].x, z = verticies[i].z;
            verticies[i].x = x * cos(angle) - z * sin(angle);
            verticies[i].z = x * sin(angle) + z * cos(angle);
        }
    } else if (rotate == ROTATE_YZ) {

    }
}