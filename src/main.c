#include <math.h>
#include <stdio.h>

#include "raylib.h"

#define RENDER_W 600
#define RENDER_H 600

Vector3 vAdd(Vector3 A, Vector3 B) {
  return (Vector3){A.x + B.x, A.y + B.y, A.z + B.z};
}
Vector3 vSub(Vector3 A, Vector3 B) {
  return (Vector3){A.x - B.x, A.y - B.y, A.z - B.z};
}

float vDot(Vector3 A, Vector3 B) { return A.x * B.x + A.y * B.y + A.z * B.z; }
Vector3 vCross(Vector3 A, Vector3 B) {
  return (Vector3){A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z,
                   A.x * B.y - A.y * B.x};
}
Vector3 vMult(Vector3 A, float l) {
  return (Vector3){A.x * l, A.y * l, A.z * l};
}
float vMagnitude(Vector3 A) { return sqrt(vDot(A, A)); }
Vector3 vNormalize(Vector3 A) {
  return (Vector3){
      A.x / vMagnitude(A),
      A.y / vMagnitude(A),
      A.z / vMagnitude(A),
  };
}

typedef Vector3 Point;

typedef struct {
  Point A;
  Point B;
} Line;

typedef struct {
  Vector3 normal;
  Point P;
} Plane;

typedef struct {
  Point points[8];
  Line lines[12];
} Cube;

typedef struct {
  Point points[4];
  Line lines[6];
} Tetrahedron;

Line connectPoints(Point A, Point B) {
  Line line;
  line.A = A;
  line.B = B;
  return line;
}

Point createPoint(float x, float y, float z) {
  Point point;
  point.x = x;
  point.y = y;
  point.z = z;
  return point;
}

Vector2 toScreenOnPlane(Point p, Plane plane) {
  Vector3 n = vNormalize(plane.normal);

  Vector3 up = (fabsf(n.y) < 0.99f) ? (Vector3){0.0f, 1.0f, 0.0f}
                                    : (Vector3){1.0f, 0.0f, 0.0f};

  Vector3 u = vNormalize(vCross(up, n));
  Vector3 v = vCross(n, u);

  Vector3 rel = vSub(p, plane.P);
  float x2D = vDot(rel, u);
  float y2D = vDot(rel, v);

  float scale = 100.0f;
  return (Vector2){RENDER_W / 2 + x2D * scale, y2D * scale - RENDER_H / 2};
}

// linear transformation 10/10 wowwoowow
Point projectToPlane(Plane plane, Point point) {
  Vector3 vec1;
  vec1.x = point.x - plane.P.x;
  vec1.y = point.y - plane.P.y;
  vec1.z = point.z - plane.P.z;

  Vector3 ProjectionVectorFromBaricenterToPlane = vMult(
      vNormalize(plane.normal), -1 * vDot(vec1, vNormalize(plane.normal)));
  return vAdd(point, ProjectionVectorFromBaricenterToPlane);
};

Vector2 toScreen(Point p) {
  float scale = 80.0f;
  return (Vector2){RENDER_W / 2 + p.x * scale, RENDER_H / 2 + p.y * scale};
}

void rotatePlane(Plane* P, float t) {
  P->normal.x = sinf(t);
  // P->normal.y = cosf(10*t);
  P->normal.z = cosf(t);
}

void projectCubeMesh(Cube cube, Plane p, Vector2 (*projected_mesh)[2]) {
  size_t line_count = 12;
  for (size_t i = 0; i < line_count; i++) {
    projected_mesh[i][0] =
        toScreenOnPlane(projectToPlane(p, cube.lines[i].A), p);
    projected_mesh[i][1] =
        toScreenOnPlane(projectToPlane(p, cube.lines[i].B), p);
  }
}

void projectTetraMesh(Tetrahedron tetra, Plane p,
                      Vector2 (*projected_mesh)[2]) {
  size_t line_count = 6;
  for (size_t i = 0; i < line_count; i++) {
    projected_mesh[i][0] =
        toScreenOnPlane(projectToPlane(p, tetra.lines[i].A), p);
    projected_mesh[i][1] =
        toScreenOnPlane(projectToPlane(p, tetra.lines[i].B), p);
  }
}

Cube createCube(Vector3 pos, float side_length) {
  int connections[12][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6},
                            {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};
  Cube cube = {0};
  cube.points[0] =
      createPoint(0 - (pos.x + side_length / 2), 0 - (pos.y + side_length / 2),
                  0 - (pos.z + side_length / 2));
  cube.points[1] =
      createPoint(side_length - (pos.x + side_length / 2),
                  0 - (pos.y + side_length / 2), 0 - (pos.z + side_length / 2));
  cube.points[2] = createPoint(side_length - (pos.x + side_length / 2),
                               0 - (pos.y + side_length / 2),
                               side_length - (pos.z + side_length / 2));
  cube.points[3] =
      createPoint(0 - (pos.x + side_length / 2), 0 - (pos.y + side_length / 2),
                  side_length - (pos.z + side_length / 2));
  cube.points[4] = createPoint(0 - (pos.x + side_length / 2),
                               side_length - (pos.y + side_length / 2),
                               0 - (pos.z + side_length / 2));
  cube.points[5] = createPoint(side_length - (pos.x + side_length / 2),
                               side_length - (pos.y + side_length / 2),
                               0 - (pos.z + side_length / 2));
  cube.points[6] = createPoint(side_length - (pos.x + side_length / 2),
                               side_length - (pos.y + side_length / 2),
                               side_length - (pos.z + side_length / 2));
  cube.points[7] = createPoint(0 - (pos.x + side_length / 2),
                               side_length - (pos.y + side_length / 2),
                               side_length - (pos.z + side_length / 2));

  for (int i = 0; i < 12; i++)
    cube.lines[i] = connectPoints(cube.points[connections[i][0]],
                                  cube.points[connections[i][1]]);
  return cube;
}

#include <math.h>

Tetrahedron createTetrahedron(Vector3 pos, float side_length) {
  int connections[6][2] = {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {2, 3}, {3, 1}};

  Tetrahedron tetra = {0};

  float height = side_length * sqrtf(2.0f / 3.0f);
  float y_top = 0.75f * height;
  float y_base = -0.25f * height;
  float r = side_length / sqrtf(3.0f);
  float half_s = 0.5f * side_length;

  tetra.points[0] = createPoint(pos.x, -pos.y - y_top, pos.z);
  tetra.points[1] = createPoint(pos.x + r, -pos.y - y_base, pos.z);
  tetra.points[2] =
      createPoint(pos.x - 0.5f * r, -pos.y - y_base, pos.z + half_s);
  tetra.points[3] =
      createPoint(pos.x - 0.5f * r, -pos.y - y_base, pos.z - half_s);

  for (int i = 0; i < 6; i++) {
    tetra.lines[i] = connectPoints(tetra.points[connections[i][0]],
                                   tetra.points[connections[i][1]]);
  }

  return tetra;
}

int main() {
  Cube cube = createCube((Vector3){0.0f, -3.0f, 0.0f}, 2.0f);
  Cube cube2 = createCube((Vector3){0.0f, -3.0f, 0.0f}, 1.0f);
  Cube cube3 = createCube((Vector3){0.0f, -3.0f, 0.0f}, 0.5f);
  Cube cube4 = createCube((Vector3){0.0f, -3.0f, 0.0f}, 0.25f);
  Cube cube5 = createCube((Vector3){0.0f, -3.0f, 0.0f}, 0.125f);
  Tetrahedron tetrahedron =
      createTetrahedron((Vector3){0.0f, -1.55f, 0.0f}, 2.0f);

  Plane p;
  p.normal = (Vector3){1.0f, 0.5f, 1.0f};
  p.P = (Point){0.0f, -4.5f, 0.0f};

  Vector2 projected_cube[12][2];
  Vector2 projected_cube2[12][2];
  Vector2 projected_cube3[12][2];
  Vector2 projected_cube4[12][2];
  Vector2 projected_cube5[12][2];
  Vector2 projected_tetra[6][2];

  SetTraceLogLevel(LOG_NONE);
  InitWindow(RENDER_W, RENDER_H, "ola mundo");

  double t = GetTime();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    projectCubeMesh(cube, p, projected_cube);
    projectCubeMesh(cube2, p, projected_cube2);
    projectCubeMesh(cube3, p, projected_cube3);
    projectCubeMesh(cube4, p, projected_cube4);
    projectCubeMesh(cube5, p, projected_cube5);
    projectTetraMesh(tetrahedron, p, projected_tetra);
    for (int i = 0; i < 12; i++) {
      DrawLineV(projected_cube2[i][0], projected_cube[i][1], BLUE);
            DrawLineV(projected_cube[i][0], projected_cube[i][1], WHITE);
            // DrawLineV(projected_cube3[i][0], projected_cube3[i][1], PURPLE);
            // DrawLineV(projected_cube4[i][0], projected_cube4[i][1], RED);
            // DrawLineV(projected_cube5[i][0], projected_cube5[i][1], WHITE);
    }
    for (int i = 0; i < 6; i++)
      DrawLineV(projected_tetra[i][0], projected_tetra[i][1], RED);

    // for (int i = 0; i < 8; i++) {
    //   printf("%f %f\n", s[i].x, s[i].y);
    // }
    rotatePlane(&p, t);

    EndDrawing();
    t = GetTime();
  }

  CloseWindow();
  printf("ola");
}