#include "object_manager.h"

#include <stdio.h>

#define INITIAL_SIZE 50

void init_object_manager(object_manager_t* manager) {
    if (manager == NULL) {
        fprintf(stderr, "error in init_object_manager: argument must not be NULL\n");
    }
    // allocate memory for the vertices. Each triangle has 9 position vertices and 9 color vertices
    manager->triangle_vertices = (float*)malloc(sizeof(float) * INITIAL_SIZE * 18);
    manager->num_visible_triangles = 0;
    manager->num_deleted_triangles = 0;
    manager->allocated_triangles = INITIAL_SIZE;

    manager->line_vertices = (float*)malloc(sizeof(float) * INITIAL_SIZE * 12);
    manager->num_visible_lines = 0;
    manager->num_deleted_lines = 0;
    manager->allocated_lines = INITIAL_SIZE;

    manager->VAO_triangles = 0;
    manager->VBO_triangles = 0;
    manager->VAO_lines = 0;
    manager->VBO_lines = 0;
}


// TODO: this function changes the position of triangles in the array.
// this has to be updated in each triangle-struct
// maybe make a second array, containing all triangles...?


// this function returns a new vertex-array with the double size, containing all the visible triangles
// and two fields of the manager gets changed: num_deleted_triangles, allocated_triangles
float* get_extended_triangle_vertices(object_manager_t* manager) {
    int old_size = manager->allocated_triangles;
    float* new_vertices = (float*)malloc(sizeof(float) * 18 * old_size * 2);
    if (!new_vertices) {
        return NULL;
    }

    // copy the old vertices (and remove the deleted triangles at the same time)
    int new_index = 0;
    int old_index = 0;
    while (old_index < old_size * 18) {
        if (manager->triangle_vertices[old_index] <= -50) { // big negative number means deleted triangle
            if (old_index % 18 == 0) { // check if this is the first vertex of the triangle
                old_index += 18;
                manager->num_deleted_triangles--;
                continue;
            }
        }

        // copy the vertex
        new_vertices[new_index] = manager->triangle_vertices[old_index];
        old_index++;
        new_index++;
    }
    manager->allocated_triangles *= 2;
    return new_vertices;
}


void add_triangle(object_manager_t* manager, triangle_t* triangle) {
    if (manager == NULL || triangle == NULL) {
        fprintf(stderr, "error in add_triangle: arguments must not be NULL\n");
        return;
    }

    int total_triangles = manager->num_deleted_triangles + manager->num_visible_triangles;
    if (manager->allocated_triangles < total_triangles + 1) {
        // not enough space: extend the triangle_vertices array (we double it)
        float* extended_vertices = get_extended_triangle_vertices(manager);
        if (!extended_vertices) {
            fprintf(stderr, "error in add_triangle: malloc\n");
            return;
        }
        free(manager->triangle_vertices);
        manager->triangle_vertices = extended_vertices;
    }

    // now we have space: add the triangle data into the array
    total_triangles = manager->num_deleted_lines + manager->num_visible_triangles;
    triangle->index_in_vertex_array = total_triangles * 18;

    manager->triangle_vertices[total_triangles*18]   = triangle->v1.x;
    manager->triangle_vertices[total_triangles*18+1] = triangle->v1.y;
    manager->triangle_vertices[total_triangles*18+2] = triangle->v1.z;
    manager->triangle_vertices[total_triangles*18+3] = triangle->color.r;
    manager->triangle_vertices[total_triangles*18+4] = triangle->color.g;
    manager->triangle_vertices[total_triangles*18+5] = triangle->color.b;
    
    manager->triangle_vertices[total_triangles*18+6] = triangle->v2.x;
    manager->triangle_vertices[total_triangles*18+7] = triangle->v2.y;
    manager->triangle_vertices[total_triangles*18+8] = triangle->v2.z;
    manager->triangle_vertices[total_triangles*18+9] = triangle->color.r;
    manager->triangle_vertices[total_triangles*18+10] = triangle->color.g;
    manager->triangle_vertices[total_triangles*18+11] = triangle->color.b;

    manager->triangle_vertices[total_triangles*18+12] = triangle->v3.x;
    manager->triangle_vertices[total_triangles*18+13] = triangle->v3.y;
    manager->triangle_vertices[total_triangles*18+14] = triangle->v3.z;
    manager->triangle_vertices[total_triangles*18+15] = triangle->color.r;
    manager->triangle_vertices[total_triangles*18+16] = triangle->color.g;
    manager->triangle_vertices[total_triangles*18+17] = triangle->color.b;

    // add 1 to the counter
    manager->num_visible_triangles++;
}


void remove_triangle(object_manager_t* manager, triangle_t* triangle) {
    int index = triangle->index_in_vertex_array;

    manager->triangle_vertices[index] = -100.0f;
    manager->triangle_vertices[index+1] = -100.0f;
    manager->triangle_vertices[index+2] = -100.0f;

    manager->triangle_vertices[index+6] = -100.0f;
    manager->triangle_vertices[index+7] = -100.0f;
    manager->triangle_vertices[index+8] = -100.0f;

    manager->triangle_vertices[index+12] = -100.0f;
    manager->triangle_vertices[index+13] = -100.0f;
    manager->triangle_vertices[index+14] = -100.0f;

    // update the counters
    manager->num_deleted_triangles++;
    manager->num_visible_triangles--;

    // if there are a lot of deleted triangles, remove them from the array
    if (manager->num_deleted_triangles / manager->num_visible_triangles >= 0.25) {
        // TODO
    }
}