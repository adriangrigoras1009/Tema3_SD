#include<stdio.h>

#include<stdlib.h>

typedef struct graph {
	int nr_nodes;
	int nr_muchii;
	struct node **neighbours;
} graph;
typedef struct node {
	int cost;
	int val;
	int capat;
	struct node *next;
	int parent;
} node;
typedef struct Heap {
	int n;
	node vector[7000];
} Heap;
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
	node t = vector[i];
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
int deleteRoot(Heap *a, int poz[]) {
	int z = a->vector[0].val;
	a->vector[0] = a->vector[a->n - 1];
	a->n = a->n - 1;
	poz[a->vector[0].val] = 1;
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
void construire(graph **a, int muchii, FILE *graf, int *contor2, FILE *out) {
	int i;
	*contor2 = 0;
	for (i = 0; i < muchii; i++) {
		int x, x1, cost;
		fscanf(graf, "%d ", &x);
		fscanf(graf, "%d ", &x1);
		fscanf(graf, "%d\n", &cost);
		node *t = (*a)->neighbours[x];
		node *aux, *aux2;
		if (t == NULL) {
			(*a)->neighbours[x] = malloc(sizeof(node));
			(*a)->neighbours[x]->capat = 2;
			(*a)->neighbours[x]->val = x1;
			(*a)->neighbours[x]->cost = 0;
			(*a)->neighbours[x]->parent = x;
			(*a)->neighbours[x]->next = NULL;
		} else {
			aux = malloc(sizeof(node));
			aux->capat = 2;
			aux->val = x1;
			aux->cost = 0;
			aux->parent = x;
			aux->next = NULL;
			while (t->next != NULL) {
				if (t->val == x1) {
					(*contor2) ++;
					break;
				}
				t = t->next;
			}
			if (*contor2 != 0) {
				fprintf(out, "0\n");
				break;
			}
			t->next = aux;
		}
		node *t1 = (*a)->neighbours[x1];
		if (t1 == NULL) {
			(*a)->neighbours[x1] = malloc(sizeof(node));
			(*a)->neighbours[x1]->val = x;
			(*a)->neighbours[x1]->cost = cost;
			(*a)->neighbours[x1]->capat = 1;
			(*a)->neighbours[x1]->parent = x1;
			(*a)->neighbours[x1]->next = NULL;
		} else {
			aux2 = malloc(sizeof(node));
			aux2->val = x;
			aux2->cost = cost;
			aux2->capat = 1;
			aux2->parent = x1;
			aux2->next = NULL;
			while (t1->next != NULL) {
				t1 = t1->next;
			}
			t1->next = aux2;
		}
	}
}
void citire(graph **a, FILE *graf, int *c, FILE *out) {
	int muchii, i;
	(*a) = malloc(sizeof(graph));
	fscanf(graf, "%d ", &(*a)->nr_nodes);
	fscanf(graf, "%d\n", &muchii);
	(*a)->nr_muchii = muchii;
	(*a)->neighbours = malloc(((*a)->nr_nodes + 1) *sizeof(node *));
	for (i = 0; i <= (*a)->nr_nodes; i++)
		(*a)->neighbours[i] = NULL;
	construire(a, muchii, graf, c, out);
}
void dijkstra(graph *a, int sursa, int dist[], int sursa_reala) {
	int i;
	Heap *h;
	int poz[1001];
	h = malloc(sizeof(Heap));
	h->n = 0;
	for (i = 1; i <= a->nr_nodes; i++) {
		dist[i] = 9999999;
		poz[i] = -1;
	}
	dist[sursa] = 0;
	int nod = 0;
	int aux = 0;
	poz[sursa] = h->n + 1;
	add_heap(h, poz, sursa, dist[sursa]);
	int asd = 0;
	while (is_empty(h) != 0) {
		asd++;
		nod = deleteRoot(h, poz);
		node *x = a->neighbours[nod];
		while (x != NULL) {
			if (asd == 1) {
				if (x->val != sursa_reala) {
					if (dist[x->val] > dist[nod] + x->cost) {
						dist[x->val] = dist[nod] + x->cost;
						if (poz[x->val] == -1) {
							poz[x->val] = h->n + 1;
							add_heap(h, poz, x->val, dist[x->val]);
						} else {
							update_heap(h, poz, x->val, dist[x->val]);
						}
					}
				}
			} else {
				if (dist[x->val] > dist[nod] + x->cost) {
					dist[x->val] = dist[nod] + x->cost;
					if (poz[x->val] == -1) {
						poz[x->val] = h->n + 1;
						add_heap(h, poz, x->val, dist[x->val]);
					} else {
						update_heap(h, poz, x->val, dist[x->val]);
					}
				}
			}
			if (x->next == NULL)
				break;
			x = x->next;
		}
	}
	free(h);
}
void schimbare(graph *a, FILE *out) {
	int i;
	int min[1001];
	for (i = 1; i <= a->nr_nodes; i++) {
		int contor = 0;
		min[i] = 99999;
		node *t = a->neighbours[i];
		while (t != NULL) {
			if (t->capat == 2) {
				int dist[1001];
				dijkstra(a, t->val, dist, i);
				if (contor == 0) {
					min[i] = dist[i];
				} else {
					if (dist[i] < min[i]) {
						min[i] = dist[i];
					}
				}
				contor++;
			}
			if (t->next == NULL)
				break;
			t = t->next;
		}
	}
	int min2 = min[1];
	for (i = 2; i <= a->nr_nodes; i++)
		if (min[i] < min2) {
			min2 = min[i];
		}
	if (min2 != 99999)
		fprintf(out, "%d\n", min2);
	else
		fprintf(out, "-1\n");
}
void free_vecini(node *head) {
	node *aux;
	while (head != NULL) {
		aux = head;
		head = head->next;
		free(aux);
	}
}
int main(int argc, char **argv) {
	int i;
	graph *a;
	FILE *graf;
	graf = fopen("schimbare.in", "r");
	int d = 0;
	FILE *out;
	out = fopen("schimbare.out", "w");
	citire(&a, graf, &d, out);
	fclose(graf);
	if (d == 0) {
	schimbare(a, out);
	}
	fclose(out);
	for (i = 0; i <= a->nr_nodes; i++) {
	if (a->neighbours[i] != NULL)
		free_vecini(a->neighbours[i]);
	}
	free(a->neighbours);
	free(a);
}
