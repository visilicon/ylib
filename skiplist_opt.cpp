#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

using namespace std;

typedef int Elem;

#define MAXLEVEL 32

struct NodeValue {
    int myLevel;
    Elem value;
};

class SkipNode {
public:
    NodeValue value;
    SkipNode **forward;

    SkipNode() {}

    SkipNode(Elem r, int level) {
        value.myLevel = level;
        value.value = r;
        forward = new SkipNode *[level + 1];
        for (int i = 0; i <= level; i++)
            forward[i] = NULL;
    }

    ~SkipNode() {
        delete[] forward;
    }
};

class SkipList {
    SkipNode *head;
    int max_level;
    int item_count;

    void adjust_level(int &level) {
        item_count = MAXLEVEL;
    }

public:
    SkipList() {
        head = new SkipNode(-1, MAXLEVEL);
        max_level = MAXLEVEL;
        item_count = 1;
    }

    Elem search(Elem);

    void insert(Elem);

    int remove(Elem);

    bool dump_file(char *file) {
        FILE *f = fopen(file, "wb");
        if (f == NULL) {
            return false;
        }
        fwrite(&item_count, sizeof(int), 1, f);
        int item_size = sizeof(NodeValue);
        fwrite(&item_size, sizeof(int), 1, f);
        SkipNode *h = head;
        for (; h != NULL; h = h->forward[0]) {
            fwrite((NodeValue *) &h->value, sizeof(NodeValue), 1, f);
        }
        fclose(f);
        return true;
    }

    bool load_file(char *file) {
        FILE *f = fopen(file, "rb");
        if (f == NULL) {
            return false;
        }
        int item_size = 0;
        fread(&item_count, sizeof(item_count), 1, f);
        fread(&item_size, sizeof(item_size), 1, f);
        NodeValue *node = new NodeValue[item_count];
        fread(node, item_size * item_count, 1, f);
        SkipNode *next = head;
        int maxlevel = 0;
        SkipNode *levels[MAXLEVEL + 1] = {0x0};
        levels[head->value.myLevel] = head;
        int pre_level = MAXLEVEL + 1;
        for (int i = 1; i < item_count; i++) {
            SkipNode *n = new SkipNode(node[i].value, node[i].myLevel);
            int j = 0;
            for (; j <= node[i].myLevel && j <= pre_level; j++) {
                next->forward[j] = n;
            }
            if (node[i].myLevel > pre_level) {
                int k = pre_level + 1;
                while (k <= node[i].myLevel) {
                    int z = k;
                    while (z <= MAXLEVEL + 1 && k <= node[i].myLevel) {
                        if (levels[z] == NULL) {
                            z++;
                            continue;
                        }
                        SkipNode *l = levels[z];
                        if (l->value.myLevel >= k) {
                            int j = k;
                            for (; j <= l->value.myLevel && j <= node[i].myLevel; j++) {
                                levels[z]->forward[j] = n;
                            }
                            k = j;
                        }
                        z++;
                    }
                }
            }
            levels[node[i].myLevel] = n;
            for (int k = 0; k < node[i].myLevel; k++) {
                levels[k] = NULL;

            }

            pre_level = node[i].myLevel;
            next = n;
            if (node[i].myLevel > max_level) {
                max_level = node[i].myLevel;
            }
        }
        max_level = maxlevel;
        return true;
    }

    void print() {
        SkipNode *node = head;
        for (; node != NULL; node = node->forward[0]) {
            printf("nodevalue:%d, level:%d\n", node->value.value, node->value.myLevel);
            printf("    forward ");
            for (int i = 1; i <= node->value.myLevel; i++) {
                if (node->forward[i] != NULL) {
                    printf("    fval:%d//flevel:%d ", node->forward[i]->value.value, node->forward[i]->value.myLevel);
                }
            }
            printf("\n");
        }
    }
};

uint64_t timeus() {
    timeval time;
    ::gettimeofday(&time, 0);
    return time.tv_sec * 1000 * 1000 + time.tv_usec;
}


int randomLevel(void) {
    int level = 1;
    while ((random() & 0xFFFF) < (0.25 * 0xFFFF))
        level += 1;
    return (level < MAXLEVEL) ? level : MAXLEVEL;
}

Elem SkipList::search(int searchKey) {
    SkipNode *x = head;
    for (int i = max_level; i >= 0; i--)
        while ((x->forward[i] != NULL) &&
               (x->forward[i]->value.value < searchKey))
            x = x->forward[i];
    x = x->forward[0];
    if ((x != NULL) && (x->value.value == searchKey)) return x->value.value;
    else return -1;
}

void SkipList::insert(Elem newValue) {
    SkipNode *x = head;
    int i;
    int newLevel = randomLevel();

    if (newLevel > max_level) {
        adjust_level(newLevel);
        max_level = newLevel;
    }
    SkipNode *update[max_level + 1];
    for (i = max_level; i >= 0; i--) {
        while ((x->forward[i] != NULL) && (x->forward[i]->value.value < newValue))
            x = x->forward[i];
        update[i] = x;
    }
    x = new SkipNode(newValue, newLevel);
    for (i = 0; i <= newLevel; i++) {
        x->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = x;
    }
    item_count++;
}

int SkipList::remove(Elem newValue) {
    SkipNode *x = head;
    int i;
    SkipNode *update[max_level + 1];
    for (i = max_level; i >= 0; i--) {
        while ((x->forward[i] != NULL) && (x->forward[i]->value.value < newValue)) {
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[0];
    if (x != NULL && x->value.value != newValue) {
        return -1;
    }
    for (i = 0; i <= x->value.myLevel; i++) {
        if (update[i]->forward[i] != x) {
            break;
        }
        update[i]->forward[i] = x->forward[i];
    }
    delete x;
    item_count--;
    return 0;
}


void DumpTo(char *dump_file, int count) {
    SkipList S;
    for (int i = 0; i < count; i++) {
        S.insert(i);
    }
    uint64_t start = timeus();
    S.dump_file(dump_file);
    printf("  dump:%lld\n", timeus() - start);
    //S.print();
}

void LoadFrom(char *dump_file) {
    SkipList U;
    uint64_t start = timeus();
    U.load_file(dump_file);
    printf("  load:%lld\n", timeus() - start);
    //U.print();
}

void Dump(int count) {
    char dump_file[] = "dump";
    printf("point count:%d\n", count);
    DumpTo(dump_file, count);
    LoadFrom(dump_file);
}


int main() {
    srand((unsigned) time(NULL));
    int round[] = {1e5, 1e6, 1e7, 5e7, 1e8};
    int count = sizeof(round) / sizeof(round[0]);
    for (int i = 0; i < count; i++) {
        Dump(round[i]);
    }
    return 0;
}