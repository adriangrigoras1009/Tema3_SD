#include <stdio.h>

#include <stdlib.h>

typedef struct graph {
    int nr_nodes;
    int nr_muchii;
    struct node **neighbours;
} graph;
typedef struct node {
    int cost;
    int val;
    struct node *next;
} node;
typedef struct query {
    int nod1;
    int nod2;
} query;
typedef struct Heap {
    int n;
    node vector[10000];
} Heap;
void sort_list(node **t) {
    node *aux1;
    node *aux2;
    for (aux1 = *t; aux1 != NULL; aux1 = aux1->next)
        for (aux2 = aux1->next; aux2 != NULL; aux2 = aux2->next)
            if (aux2->cost < aux1->cost) {
                int a = aux1->val;
                aux1->val = aux2->val;
                aux2->val = a;
                int b = aux1->cost;
                aux1->cost = aux2->cost;
                aux2->cost = b;
            }
}
void heapify_stergere(node vector[], int n, int i, int poz[]) {
    int contor = 0;
    node t = vector[i];
    while (1) {
        int mic = i;
        int l = 2 *i + 1;
        int r = l + 1;
        if (l < n && vector[l].cost < t.cost)
            mic = l;
        if (mic == l) {
            if (r < n && vector[r].cost < vector[mic].cost)
                mic = r;
        } else {
            if (r < n && vector[r].cost < t.cost)
                mic = r;
        }
        if (mic != i) {
            contor++;
            poz[vector[mic].val] = i + 1;
            vector[i] = vector[mic];
            i = mic;
        } else {
            break;
        }
    }
    if (contor >= 1) {
        vector[i] = t;
        poz[t.val] = i + 1;
    }
}
void heapify(node vector[], int i, int poz[]) {
    int contor = 0;
    node t;
    t.val = vector[i].val;
    t.cost = vector[i].cost;
    int parent1;
    while (1) {
        parent1 = (i - 1) / 2;
        if (i > 0 && t.cost < vector[parent1].cost) {
            contor++;
            poz[vector[parent1].val] = i + 1;
            vector[i] = vector[parent1];
            i = parent1;
        } else {
            break;
        }
    }
    if (contor >= 1) {
        vector[i] = t;
        poz[t.val] = i + 1;
    }
}
void add_heap(Heap *h, int poz[], int c, int d) {
    h->n = h->n + 1;
    h->vector[h->n - 1].val = c;
    h->vector[h->n - 1].cost = d;
    heapify(h->vector, h->n - 1, poz);
}
node deleteRoot(Heap *a, int poz[]) {
    node z;
    z.val = a->vector[0].val;
    z.cost = a->vector[0].cost;
    a->vector[0] = a->vector[a->n - 1];
    a->n = a->n - 1;
    poz[a->vector[0].val] = 1;
    poz[z.val] = -1;
    heapify_stergere(a->vector, a->n, 0, poz);
    return z;
}
int is_empty(Heap *a) {
    if (a->n == 0)
        return 0;
    else
        return 1;
}
void update_heap(Heap *a, int poz[], int c, int d) {
    a->vector[poz[c] - 1].val = c;
    a->vector[poz[c] - 1].cost = d;
    heapify(a->vector, poz[c] - 1, poz);
}
void construire(graph **a, int muchii, FILE *graf,
	query *queryuri, int nr_query) {
    int i;
    for (i = 0; i < muchii; i++) {
        int x, x1, cost;
        fscanf(graf, "%d ", & x);
        fscanf(graf, "%d ", & x1);
        fscanf(graf, "%d\n", & cost);
        node *aux, *aux2;
        if ((*a)->neighbours[x] == NULL) {
            (*a)->neighbours[x] = malloc(sizeof(node));
            (*a)->neighbours[x]->val = x1;
            (*a)->neighbours[x]->cost = cost;
            (*a)->neighbours[x]->next = NULL;
        } else {
            node *t = (*a)->neighbours[x];
            while (t->next != NULL)
                t = t->next;
            aux = malloc(sizeof(node));
            aux->val = x1;
            aux->cost = cost;
            aux->next = NULL;
            t->next = aux;
        }
        if ((*a)->neighbours[x1] == NULL) {
            (*a)->neighbours[x1] = malloc(sizeof(node));
            (*a)->neighbours[x1]->val = x;
            (*a)->neighbours[x1]->cost = cost;
            (*a)->neighbours[x1]->next = NULL;
        } else {
            node *t1 = (*a)->neighbours[x1];
            while (t1->next != NULL)
                t1 = t1->next;
            aux2 = malloc(sizeof(node));
            aux2->val = x;
            aux2->cost = cost;
            aux2->next = NULL;
            t1->next = aux2;
        }
    }
    for (i = 0; i < nr_query; i++) {
        fscanf(graf, "%d %d\n", & queryuri[i].nod1, & queryuri[i].nod2);
    }
}
void citire(graph **a, FILE *graf, query **queryuri, int *nr_query) {
    int muchii, i;
    (*a) = malloc(sizeof(graph));
    fscanf(graf, "%d ", & (*a)->nr_nodes);
    fscanf(graf, "%d ", & muchii);
    fscanf(graf, "%d\n", & (*nr_query));
    (*a)->nr_muchii = muchii;
    (*a)->neighbours = malloc(((*a)->nr_nodes + 1) *sizeof(node *));
    for (i = 0; i <= (*a)->nr_nodes; i++)
        (*a)->neighbours[i] = NULL;
    *queryuri = malloc((*nr_query) *sizeof(query));
    construire(a, muchii, graf, *queryuri, *nr_query);
}
void prim(graph *a, graph **ama, int q) {
    int i;
    Heap *h;
    int p[10001];
    int poz[10001];
    int dist[10001];
    h = malloc(sizeof(Heap));
    h->n = 0;
    for (i = 1; i <= a->nr_nodes; i++) {
        dist[i] = 999999999;
        poz[i] = -1;
        p[i] = -1;
    }
    dist[q] = 0;
    poz[q] = h->n + 1;
    add_heap(h, poz, q, 0);
    while (is_empty(h) != 0) {
        node nod = deleteRoot(h, poz);
        int t = nod.val;
        if (t != q) {
            if ((*ama)->neighbours[t] == NULL) {
                (*ama)->neighbours[t] = malloc(sizeof(node));
                (*ama)->neighbours[t]->val = p[t];
                (*ama)->neighbours[t]->cost = nod.cost;
                (*ama)->neighbours[t]->next = NULL;
            } else {
                node *t1 = (*ama)->neighbours[t];
                while (t1->next != NULL)
                    t1 = t1->next;
                node *aux = malloc(sizeof(node));
                aux->val = p[t];
                aux->cost = nod.cost;
                aux->next = NULL;
                t1->next = aux;
                sort_list(& (*ama)->neighbours[t]);
            }
            if ((*ama)->neighbours[p[t]] == NULL) {
                (*ama)->neighbours[p[t]] = malloc(sizeof(node));
                (*ama)->neighbours[p[t]]->val = t;
                (*ama)->neighbours[p[t]]->cost = nod.cost;
                (*ama)->neighbours[p[t]]->next = NULL;
            } else {
                node *t2 = (*ama)->neighbours[p[t]];
                while (t2->next != NULL)
                    t2 = t2->next;
                node *aux2 = malloc(sizeof(node));
                aux2->val = t;
                aux2->cost = nod.cost;
                aux2->next = NULL;
                t2->next = aux2;
                sort_list(& (*ama)->neighbours[p[t]]);
            }
        }
        node *x = a->neighbours[t];
        while (x != NULL) {
            if (dist[x->val] > x->cost) {
                dist[x->val] = x->cost;
                p[x->val] = t;
                if (poz[x->val] == -1) {
                    poz[x->val] = h->n + 1;
                    add_heap(h, poz, x->val, dist[x->val]);
                } else {
                    update_heap(h, poz, x->val, dist[x->val]);
                }
            }
            if (x->next == NULL)
                break;
            x = x->next;
        }
    }
    free(h);
}
void dfs2(graph *ama, int vazut[], int sursa, int destinatie, int *min) {
    static int max;
    vazut[sursa] = 1;
    if (sursa != destinatie) {
        node *t = ama->neighbours[sursa];
        while (t != NULL) {
            if (vazut[t->val] == 0 && ama->neighbours[t->val]->next != NULL &&
            	t->val != destinatie) {
                int y = max;
                if (t->cost > max)
                    max = t->cost;
                vazut[t->val] = 1;
                dfs2(ama, vazut, t->val, destinatie, min);
                max = y;
            } else if (vazut[t->val] == 0 && t->val == destinatie) {
                int y = max;
                if (t->cost > max)
                    max = t->cost;
                vazut[t->val] = 1;
                dfs2(ama, vazut, t->val, destinatie, min);
                max = y;
            }
            if (t->next == NULL)
                break;
            t = t->next;
        }
    } else {
        *min = max;
        return;
    }
}
void dfs(graph *ama, int sursa, int destinatie, FILE *out) {
    int i;
    int vazut[10002] = {0};
    int min = 0;
    dfs2(ama, vazut, sursa, destinatie, & min);
    fprintf(out, "%d\n", min);
}
void free_vecini(node *head) {
    node *aux;
    while (head != NULL) {
        aux = head;
        head = head->next;
        free(aux);
    }
}
int main() {
    int i, j;
    graph *a;
    FILE *graf;
    graf = fopen("bonus.in", "r");
    int nr_query;
    query *queryuri;
    citire(& a, graf, & queryuri, & nr_query);
    fclose(graf);
    graph *ama;
    ama = malloc(sizeof(graph));
    ama->neighbours = malloc((a->nr_nodes + 1) *sizeof(node *));
    ama->nr_nodes = a->nr_nodes;
    for (i = 0; i <= ama->nr_nodes; i++)
        ama->neighbours[i] = NULL;
    prim(a, & ama, 1);
    FILE *out;
    out = fopen("bonus.out", "w");
    for (i = 0; i < nr_query; i++) {
        dfs(ama, queryuri[i].nod1, queryuri[i].nod2, out);
    }
    fclose(out);
    free(queryuri);
    for (i = 0; i <= a->nr_nodes; i++) {
        if (ama->neighbours[i] != NULL)
            free_vecini(ama->neighbours[i]);
        if (a->neighbours[i] != NULL)
            free_vecini(a->neighbours[i]);
    }
    free(ama->neighbours);
    free(ama);
    free(a->neighbours);
    free(a);
}
