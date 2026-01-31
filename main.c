#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MAX_VERTICES 1000

#define ROTATE_XY 0
#define ROTATE_XZ 1
#define ROTATE_YZ 2

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

    struct Object sh = sphere(0.5, 30);

    int w = 9 * 60;
    int h = 9 * 60;

    long double angleSpeed = 0.005;

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

        for (int i = 0; i < sh.vertices; i++) {
            tr[i] = translate(sh.vs[i]);
        }

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                bool t = false;
                for (int i = 0; i < sh.vertices; i++) {
                    int sh_x = (tr[i].x + 1.0) / 2 * w;
                    int sh_y = (tr[i].y + 1.0) / 2 * h;
                    if (sh_x == x && sh_y == y) {
                        t = true;
                        break;
                    }
                }
                if (t) {
                    putc(0xFF, fp);
                    putc(0x00, fp);
                    putc(0x00, fp);
                } else {
                    putc(0x00, fp);
                    putc(0x00, fp);
                    putc(0x00, fp);
                }
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
    return (struct vec2) {a.x / (1.0 + a.z), a.y / (1.0 + a.z)};
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