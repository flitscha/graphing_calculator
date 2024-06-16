#pragma once

#include <stdlib.h>

typedef struct {
    float x;
    float y;
    float z;
} vector_t;

typedef struct {
    float r;
    float g;
    float b;
} color_t;

typedef struct {
    vector_t v1;
    vector_t v2;
    vector_t v3;
    color_t color;
    size_t index_in_vertex_array; // to know where the triangle is located in the vertex array
} triangle_t;

typedef struct {
    vector_t v1;
    vector_t v2;
    color_t color;
    size_t index_in_vertex_array;
} line_t;

typedef struct {
    float* triangle_vertices; // position + color data
    size_t num_visible_triangles;
    // if a triangle gets deleted, it does get invisible. After a lot of triangles are deleted, the data gets reorganised
    size_t num_deleted_triangles;
    size_t allocated_triangles; // memory allocated for this many triangles
    triangle_t** triangles; // array, containing all triangles

    float* line_vertices;
    size_t num_visible_lines;
    size_t num_deleted_lines;
    size_t allocated_lines;
    line_t** lines;

    unsigned int VAO_triangles, VBO_triangles;
    unsigned int VAO_lines, VBO_lines;
} object_manager_t;


void init_object_manager(object_manager_t* manager);
void add_triangle(object_manager_t* manager, triangle_t* triangle);
void remove_triangle(object_manager_t* manager, triangle_t* triangle);

void add_line(object_manager_t* manager, line_t* line);
void remove_line(object_manager_t* manager, line_t* line);