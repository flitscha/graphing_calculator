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
    manager->triangles = (triangle_t**)malloc(sizeof(triangle_t*) * INITIAL_SIZE);

    manager->line_vertices = (float*)malloc(sizeof(float) * INITIAL_SIZE * 12);
    manager->num_visible_lines = 0;
    manager->num_deleted_lines = 0;
    manager->allocated_lines = INITIAL_SIZE;
    manager->lines = (line_t**)malloc(sizeof(line_t*) * INITIAL_SIZE);

    manager->VAO_triangles = 0;
    manager->VBO_triangles = 0;
    manager->VAO_lines = 0;
    manager->VBO_lines = 0;

    // check malloc error
    if (!manager->triangle_vertices || !manager->triangles || !manager->line_vertices || !manager->lines) {
        fprintf(stderr, "error in init_object_manager: malloc\n");
        free(manager->triangle_vertices);
        free(manager->triangles);
        free(manager->line_vertices);
        free(manager->lines);
    }
}


// this function doubles the storage of the triangle_vertices and triangle array.
// all deleted triangles get removed.
// this fields of the manager get changed: allocated_triangles, num_deleted_triangles, triangle_vertices, triangles
void extend_triangle_storage(object_manager_t* manager) {
    size_t old_size = manager->allocated_triangles;
    size_t new_size = old_size * 2;

    float* new_vertices = (float*)malloc(sizeof(float) * 18 * new_size);
    triangle_t** new_triangles = (triangle_t**)malloc(sizeof(triangle_t*) * new_size);
    if (!new_vertices || !new_triangles) {
        fprintf(stderr, "error in extend_triangle_storage: malloc\n");
        free(new_vertices);
        free(new_triangles);
        return;
    }

    // copy the old vertices (and remove the deleted triangles at the same time)
    size_t new_index = 0;
    size_t old_index = 0;
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

        // copy the triangle
        if (old_index % 18 == 0) {
            new_triangles[new_index / 18] = manager->triangles[old_index / 18];
            new_triangles[new_index / 18]->index_in_vertex_array = new_index / 18;
        }

        old_index++;
        new_index++;
    }
    manager->allocated_triangles = new_size;
    free(manager->triangle_vertices);
    free(manager->triangles);
    manager->triangle_vertices = new_vertices;
    manager->triangles = new_triangles;
}


void add_triangle(object_manager_t* manager, triangle_t* triangle) {
    if (manager == NULL || triangle == NULL) {
        fprintf(stderr, "error in add_triangle: arguments must not be NULL\n");
        return;
    }

    size_t total_triangles = manager->num_deleted_triangles + manager->num_visible_triangles;
    if (manager->allocated_triangles < total_triangles + 1) {
        // not enough space: extend the triangle_vertices array (we double it)
        extend_triangle_storage(manager);
    }

    // now we have space: add the triangle data into the array
    total_triangles = manager->num_deleted_triangles + manager->num_visible_triangles;
    triangle->index_in_vertex_array = total_triangles;

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


void collect_deleted_triangles(object_manager_t* manager) {
    size_t total_triangles = manager->num_deleted_triangles + manager->num_visible_triangles;

    size_t old_index = 0;
    size_t new_index = 0;

    while (old_index < total_triangles * 18) {
        // search the first triangle, that is not deleted
        if (manager->triangle_vertices[old_index] <= -50) {
            if (old_index % 18 == 0) {
                old_index += 18;
                manager->num_deleted_triangles--;
                continue;
            }
        }

        // copy the triangle
        manager->triangle_vertices[new_index] = manager->triangle_vertices[old_index];
        
        if (new_index % 18 == 0) {
            manager->triangles[new_index / 18] = manager->triangles[old_index / 18];
            manager->triangles[new_index / 18]->index_in_vertex_array = new_index / 18;
        }

        new_index++;
        old_index++;
    }

    // check if everything worked
    if (manager->num_deleted_triangles != 0 || new_index / 18 != manager->num_visible_triangles) {
        fprintf(stderr, "something went wrong in collect_deleted_triangles\n");
    }
}


// this function halves the memory for the lines
void cut_triangle_storage(object_manager_t* manager) {
    size_t old_size = manager->allocated_triangles;
    size_t new_size = old_size / 2;

    if (new_size < manager->num_visible_triangles) {
        fprintf(stderr, "error in cut_triangle_storage: new_size < num_visible_triangles\n");
        return;
    }

    float* new_vertices = (float*)malloc(sizeof(float) * new_size * 18);
    triangle_t** new_triangles = (triangle_t**)malloc(sizeof(triangle_t*) * new_size);

    if (!new_vertices || !new_triangles) {
        fprintf(stderr, "error in cut_triangles_storage: malloc\n");
        free(new_vertices);
        free(new_triangles);
        return;
    }

    // copy the vertices
    for (size_t i = 0; i < manager->num_visible_triangles * 18; i++) {
        new_vertices[i] = manager->triangle_vertices[i];
    }

    // copy the lines
    for (size_t i = 0; i < manager->num_visible_triangles; i++) {
        new_triangles[i] = manager->triangles[i];
    }

    free(manager->triangles);
    free(manager->triangle_vertices);
    manager->triangles = new_triangles;
    manager->triangle_vertices = new_vertices;
    manager->allocated_triangles = new_size;
}


void remove_triangle(object_manager_t* manager, triangle_t* triangle) {
    int index = triangle->index_in_vertex_array;

    // set the position of all 3 points to (-100, -100, -100)
    for (size_t i = 0; i < 3; i++) {
        manager->triangle_vertices[index + i * 6] = -100.0f;
        manager->triangle_vertices[index + i * 6 + 1] = -100.0f;
        manager->triangle_vertices[index + i * 6 + 2] = -100.0f;
    }

    // update the counters
    manager->num_deleted_triangles++;
    manager->num_visible_triangles--;

    // if there are a lot of deleted triangles, remove them from the array
    if (manager->num_visible_triangles > 0 && manager->num_deleted_triangles >= manager->num_visible_triangles / 4 &&
        manager->allocated_triangles >= INITIAL_SIZE * 2) {

        collect_deleted_triangles(manager);
    }
}




// -----------------------------------------
//      the same functions for lines
// -----------------------------------------

void extend_line_storage(object_manager_t* manager) {
    size_t old_size = manager->allocated_lines;
    size_t new_size = old_size * 2;

    float* new_vertices = (float*)malloc(sizeof(float) * 12 * new_size);
    line_t** new_lines = (line_t**)malloc(sizeof(line_t*) * new_size);
    if (!new_vertices || !new_lines) {
        fprintf(stderr, "error in add_line: malloc\n");
        free(new_vertices);
        free(new_lines);
        return;
    }

    // copy the old vertices (and remove the deleted lines at the same time)
    size_t new_index = 0;
    size_t old_index = 0;
    while (old_index < old_size * 12) {
        if (manager->line_vertices[old_index] <= -50) { // big negative number means deleted line
            if (old_index % 12 == 0) { // check if this is the first vertex of the line
                old_index += 12;
                manager->num_deleted_lines--;
                continue;
            }
        }

        // copy the vertex
        new_vertices[new_index] = manager->line_vertices[old_index];

        // copy the line
        if (old_index % 12 == 0) {
            new_lines[new_index / 12] = manager->lines[old_index / 12];
            new_lines[new_index / 12]->index_in_vertex_array = new_index / 12;
        }

        old_index++;
        new_index++;
    }
    manager->allocated_lines = new_size;
    free(manager->line_vertices);
    free(manager->lines);
    manager->line_vertices = new_vertices;
    manager->lines = new_lines;
}


void add_line(object_manager_t* manager, line_t* line) {
    if (manager == NULL || line == NULL) {
        fprintf(stderr, "error in add_line: arguments must not be NULL\n");
        return;
    }

    size_t total_lines = manager->num_deleted_lines + manager->num_visible_lines;
    if (manager->allocated_lines < total_lines + 1) {
        // not enough space: extend the line_vertices array (we double it)
        extend_line_storage(manager);
    }

    // now we have space: add the lines data into the array
    total_lines = manager->num_deleted_lines + manager->num_visible_lines;
    line->index_in_vertex_array = total_lines;

    manager->line_vertices[total_lines*12] = line->v1.x;
    manager->line_vertices[total_lines*12+1] = line->v1.y;
    manager->line_vertices[total_lines*12+2] = line->v1.z;
    manager->line_vertices[total_lines*12+3] = line->color.r;
    manager->line_vertices[total_lines*12+4] = line->color.g;
    manager->line_vertices[total_lines*12+5] = line->color.b;

    manager->line_vertices[total_lines*12+6] = line->v2.x;
    manager->line_vertices[total_lines*12+7] = line->v2.y;
    manager->line_vertices[total_lines*12+8] = line->v2.z;
    manager->line_vertices[total_lines*12+9] = line->color.r;
    manager->line_vertices[total_lines*12+10] = line->color.g;
    manager->line_vertices[total_lines*12+11] = line->color.b;

    // add 1 to the counter
    manager->num_visible_lines++;
}


void collect_deleted_lines(object_manager_t* manager) {
    size_t total_lines = manager->num_deleted_lines + manager->num_visible_lines;

    size_t old_index = 0;
    size_t new_index = 0;

    while (old_index < total_lines * 12) {
        // search the first line, that is not deleted
        if (manager->line_vertices[old_index] <= -50) {
            if (old_index % 12 == 0) {
                old_index += 12;
                manager->num_deleted_lines--;
                continue;
            }
        }

        // copy the line
        manager->line_vertices[new_index] = manager->line_vertices[old_index];
        
        if (new_index % 12 == 0) {
            manager->lines[new_index / 12] = manager->lines[old_index / 12];
            manager->lines[new_index / 12]->index_in_vertex_array = new_index / 12;
        }

        new_index++;
        old_index++;
    }

    // check if everything worked
    if (manager->num_deleted_lines != 0 || new_index / 12 != manager->num_visible_lines) {
        fprintf(stderr, "something went wrong in collect_deleted_lines\n");
    }
}


// this function halves the memory for the lines
void cut_line_storage(object_manager_t* manager) {
    size_t old_size = manager->allocated_lines;
    size_t new_size = old_size / 2;

    if (new_size < manager->num_visible_lines) {
        fprintf(stderr, "error in cut_line_storage: new_size < num_visible_lines\n");
        return;
    }

    float* new_vertices = (float*)malloc(sizeof(float) * new_size * 12);
    line_t** new_lines = (line_t**)malloc(sizeof(line_t*) * new_size);

    if (!new_vertices || !new_lines) {
        fprintf(stderr, "error in cut_line_storage: malloc\n");
        free(new_vertices);
        free(new_lines);
        return;
    }

    // copy the vertices
    for (size_t i = 0; i < manager->num_visible_lines * 12; i++) {
        new_vertices[i] = manager->line_vertices[i];
    }

    // copy the lines
    for (size_t i = 0; i < manager->num_visible_lines; i++) {
        new_lines[i] = manager->lines[i];
    }

    free(manager->lines);
    free(manager->line_vertices);
    manager->lines = new_lines;
    manager->line_vertices = new_vertices;
    manager->allocated_lines = new_size;
}

void remove_line(object_manager_t* manager, line_t* line) {
    int index = line->index_in_vertex_array;

    // set the position of the 2 points to (-100, -100, -100)
    for (size_t i = 0; i < 2; i++) {
        manager->line_vertices[index + i * 6] = -100.0f;
        manager->line_vertices[index + i * 6 + 1] = -100.0f;
        manager->line_vertices[index + i * 6 + 2] = -100.0f;
    }

    // update the counters
    manager->num_deleted_lines++;
    manager->num_visible_lines--;

    // if there are a lot of deleted lines, remove them from the array
    if (manager->num_visible_lines > 0 && manager->num_deleted_lines >= manager->num_visible_lines / 4) {
        collect_deleted_lines(manager);

        // if there are not a lot of lines left, but the storage is big, reduce the storage
        if (manager->num_visible_lines > 0 && manager->num_deleted_lines >= manager->num_visible_lines / 4 &&
            manager->allocated_lines >= INITIAL_SIZE * 2) {
                
            cut_line_storage(manager);
        }
    }
}