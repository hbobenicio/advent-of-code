#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

enum node_kind {
    NODE_KIND_DIRECTORY,
    NODE_KIND_FILE,
};

struct node_file;
struct node_dir;
struct node;

struct node_list {
    struct node** items;
    size_t len;
    size_t capacity;
};

void node_list_append(struct node_list* list, struct node* item)
{
    if (list->len >= list->capacity) {
        // grow
        if (list->capacity == 0) {
            static const size_t initial_capacity = 4;
            list->capacity = initial_capacity;
        } else {
            list->capacity *= 2;
        }

        list->items = realloc(list->items, list->capacity * sizeof(item));
    }
    list->items[list->len++] = item;
}

struct node_file {
    size_t size;
};

struct node_dir {
    size_t size;

    struct node** children;
    size_t children_len;
    size_t children_capacity;
    #define NODE_DIR_INITIAL_CAPACITY 4
};

struct node {
    char name[64];
    struct node* parent;

    enum node_kind kind;
    union node_variant {
        struct node_file file;
        struct node_dir dir;
    } as;
};

struct node* node_new_file(const char* name, size_t name_len, size_t size)
{
    struct node* node = calloc(1, sizeof(struct node));
    assert(node != NULL);

    node->parent = NULL;
    assert(name_len < sizeof(node->name));
    strncpy(node->name, name, sizeof(node->name));

    node->kind = NODE_KIND_FILE;
    node->as.file = (struct node_file) {
        .size = size,
    };

    return node;
}

struct node* node_new_dir(const char* name, size_t name_len)
{
    struct node* node = calloc(1, sizeof(struct node));
    assert(node != NULL);

    node->parent = NULL;
    assert(name_len < sizeof(node->name));
    strncpy(node->name, name, sizeof(node->name));

    node->kind = NODE_KIND_DIRECTORY;
    node->as.dir = (struct node_dir) {
        .size = 0,
        .children = NULL,
        .children_len = 0,
        .children_capacity = 0,
    };

    return node;
}

void node_free(struct node* node)
{
    assert(node != NULL);
    switch (node->kind)
    {
        case NODE_KIND_FILE:
            // noop
            break;

        case NODE_KIND_DIRECTORY: {
            for (size_t i = 0; i < node->as.dir.children_len; i++) {
                struct node* child = node->as.dir.children[i];
                node_free(child);
            }
            free(node->as.dir.children);
        } break;

        default:
            assert(false && "unknown node kind");
    }
    free(node);
}

size_t node_calc_size(struct node* node)
{
    assert(node != NULL);
    switch (node->kind)
    {
        case NODE_KIND_FILE:
            return node->as.file.size;

        case NODE_KIND_DIRECTORY: {
            size_t children_total_size = 0;
            for (size_t i = 0; i < node->as.dir.children_len; i++) {
                struct node* child = node->as.dir.children[i];
                children_total_size += node_calc_size(child);
            }
            // fprintf(stderr, "debug: setting dir %s size=%zu\n", node->name, children_total_size);
            node->as.dir.size = children_total_size;
            return children_total_size;
        } break;
    
        default:
            assert(false && "unknown node kind");
    }
    assert(false && "unreachable");
    return 0;
}

void node_dir_children_append(struct node* node_dir, struct node* child)
{
    assert(node_dir->kind == NODE_KIND_DIRECTORY);
    struct node_dir* dir = &node_dir->as.dir;

    // needs to grow
    if (dir->children_len >= dir->children_capacity) {
        // grow
        if (dir->children_capacity == 0) {
            dir->children_capacity = NODE_DIR_INITIAL_CAPACITY;
        } else {
            dir->children_capacity *= 2;
        }

        dir->children = realloc(dir->children, dir->children_capacity * sizeof(struct node*));
    }
    dir->children[dir->children_len++] = child;
    child->parent = node_dir;
}

struct node* node_dir_children_find_by_name(struct node* node, const char* name)
{
    assert(node->kind == NODE_KIND_DIRECTORY);
    for (size_t i = 0; i < node->as.dir.children_len; i++) {
        struct node* child = node->as.dir.children[i];
        if (strcmp(child->name, name) == 0) {
            return child;
        }
    }
    return NULL;
}

struct filesystem {
    struct node* root;
    struct node* cwd;
    struct node_list directories;
};

struct filesystem filesystem_new(void)
{
    struct node* root = node_new_dir("/", 1);

    struct filesystem fs = {
        .root = root,
        .cwd = root,
        .directories = (struct node_list) {
            .items = NULL,
            .len = 0,
            .capacity = 0,
        },
    };

    node_list_append(&fs.directories, root);

    return fs;
}

void filesystem_free(struct filesystem* fs)
{
    node_free(fs->root);
    free(fs->directories.items);
}

void filesystem_mkdir(struct filesystem* fs, const char* dir_name)
{
    struct node* dir = node_new_dir(dir_name, strlen(dir_name));
    node_dir_children_append(fs->cwd, dir);
    node_list_append(&fs->directories, dir);
}

void filesystem_cd_up(struct filesystem* fs)
{
    if (fs->cwd->parent != NULL) {
        fs->cwd = fs->cwd->parent;
    }
}

void filesystem_cd_root(struct filesystem* fs)
{
    fs->cwd = fs->root;
}

void filesystem_cd(struct filesystem* fs, const char* dir_name)
{
    struct node* child = node_dir_children_find_by_name(fs->cwd, dir_name);
    assert(child != NULL);
    assert(child->kind == NODE_KIND_DIRECTORY);

    fs->cwd = child;
}

void filesystem_create_file_with_size(struct filesystem* fs, const char* file_name, size_t file_size)
{
    struct node* file = node_new_file(file_name, strlen(file_name), file_size);
    node_dir_children_append(fs->cwd, file);
}

size_t filesystem_calc_size(struct filesystem* fs)
{
    return node_calc_size(fs->root);
}

int main()
{
    struct filesystem fs = filesystem_new();

    char line[128] = {0};
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t line_len = strlen(line);
        assert(line_len > 0);
        line[--line_len] = '\0';

        if (line_len == 0) continue;
        
        // shell command
        if (line[0] == '$') {
            assert(line_len >= 4);

            if (line[2] == 'c' && line[3] == 'd') {
                // $ cd x
                // $ cd ..
                // $ cd /
                assert(line_len >= 6);

                const char* dir_name = line + 5;

                // $ cd /
                if (strcmp(dir_name, "/") == 0) {
                    assert(line_len == 6);
                    filesystem_cd_root(&fs);
                    continue;
                }

                // $ cd ..
                if (strcmp(dir_name, "..") == 0) {
                    assert(line_len == 7);
                    filesystem_cd_up(&fs);
                    continue;
                }

                // $ cd x
                filesystem_cd(&fs, dir_name);

                continue;
            }
            
            if (line[2] == 'l' && line[3] == 's') {
                // $ ls
                assert(line_len == 4);

                continue;
            }

            assert(false && "invalid command");
        }

        // ls entry: dir
        if (line[0] == 'd' && line[1] == 'i' && line[2] == 'r') {
            // dir x
            assert(line_len >= 5);

            const char* dir_name = line + 4;

            filesystem_mkdir(&fs, dir_name);
            continue;
        }

        // ls entry: file
        size_t file_size;
        char file_name[64];
        assert(sscanf(line, "%zu %s", &file_size, file_name) == 2);

        filesystem_create_file_with_size(&fs, file_name, file_size);
    }

    // Solution A
    printf("Solution A:\n");
    size_t fs_total_size = filesystem_calc_size(&fs);

    size_t total_size = 0, threshold = 100000;
    for (size_t i = 0; i < fs.directories.len; i++) {
        struct node* dir = fs.directories.items[i];
        assert(dir->kind == NODE_KIND_DIRECTORY);

        // fprintf(stderr, "debug: dir %s has size=%zu\n", dir->name, dir->as.dir.size);
        if (dir->as.dir.size <= threshold) {
            total_size += dir->as.dir.size;
        }
    }
    printf("  Total size of all dirs with at most 100,000) = %zu\n", total_size);

    // Solution B:
    // 1. For each directory, get its size
    // 2. if size(dir) >
    printf("Solution B:\n");
    const size_t fs_max_size = 70000000;
    const size_t free_space = fs_max_size - fs_total_size;
    const size_t desired_unused_space = 30000000;
    assert(free_space < desired_unused_space);
    const size_t required_space_to_free = desired_unused_space - free_space;

    size_t min_size = SIZE_MAX;
    for (size_t i = 0; i < fs.directories.len; i++) {
        struct node* node = fs.directories.items[i];
        assert(node->kind == NODE_KIND_DIRECTORY);
        
        struct node_dir* dir = &node->as.dir;
        if (dir->size < required_space_to_free) {
            continue;
        }

        if (dir->size < min_size) {
            min_size = dir->size;
        }
    }
    assert(min_size != SIZE_MAX);
    printf("  size of dir with minimum size that frees up enough space: %zu\n", min_size);

    filesystem_free(&fs);
}
