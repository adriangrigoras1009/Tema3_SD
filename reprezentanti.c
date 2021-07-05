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
	struct node *next;
} node;
typedef struct min_repr {
	int repr;
	int min;
} min_repr;
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
void construire(graph **a, int muchii, FILE *graf,
	int *reprezentanti, int nr_repr) {
	int i;
	for (i = 0; i < muchii; i++) {
		int x, x1, cost;
		fscanf(graf, "%d ", & x);
		fscanf(graf, "%d ", & x1);
		fscanf(graf, "%d\n", & cost);
		node *t = (*a)->neighbours[x];
		node *aux, *aux2;

		if (t == NULL) {
			(*a)->neighbours[x] = malloc(sizeof(node));
			(*a)->neighbours[x]->val = x1;
			(*a)->neighbours[x]->cost = cost;
			(*a)->neighbours[x]->next = NULL;
		} else {
			aux = malloc(sizeof(node));
			aux->val = x1;
			aux->cost = cost;
			aux->next = NULL;
			while (t->next != NULL)
				t = t->next;
			t->next = aux;
		}
		node *t1 = (*a)->neighbours[x1];
		if (t1 == NULL) {
			(*a)->neighbours[x1] = malloc(sizeof(node));
			(*a)->neighbours[x1]->val = x;
			(*a)->neighbours[x1]->cost = cost;
			(*a)->neighbours[x1]->next = NULL;
		} else {
			aux2 = malloc(sizeof(node));
			aux2->val = x;
			aux2->cost = cost;
			aux2->next = NULL;
			while (t1->next != NULL)
				t1 = t1->next;
			t1->next = aux2;
		}
	}
	for (i = 0; i < nr_repr; i++) {
		fscanf(graf, "%d ", & reprezentanti[i]);
	}
}
void citire(graph **a, FILE *graf, int **reprezentanti, int *nr_repr) {
	int muchii, i;
	(*a) = malloc(sizeof(graph));
	fscanf(graf, "%d ", & (*a)->nr_nodes);
	fscanf(graf, "%d\n", & muchii);
	fscanf(graf, "%d\n", & (*nr_repr));
	(*a)->nr_muchii = muchii;
	(*a)->neighbours = malloc(((*a)->nr_nodes + 1) *sizeof(node *));
	for (i = 0; i <= (*a)->nr_nodes; i++)
		(*a)->neighbours[i] = NULL;
	*reprezentanti = malloc((*nr_repr) *sizeof(int));
	construire(a, muchii, graf, *reprezentanti, *nr_repr);
}
void dijkstra(graph *a, int sursa, int dist[],
	int adevar[], min_repr drumuri[]) {
	Heap *h;
	int poz[] = { [0 ... 10000] = -1 };
	h = malloc(sizeof(Heap));
	h->n = 0;
	dist[sursa] = 0;
	poz[sursa] = h->n + 1;
	add_heap(h, poz, sursa, 0);
	while (is_empty(h) != 0) {
		int nod = deleteRoot(h, poz);
		node *x = a->neighbours[nod];
		if (dist[nod] < drumuri[nod - 1].min ||
		(dist[nod] == drumuri[nod - 1].min && sursa < drumuri[nod - 1].repr)) {
			drumuri[nod - 1].min = dist[nod];
			drumuri[nod - 1].repr = sursa;
		}
		while (x != NULL) {
			if (dist[x->val] > dist[nod] + x->cost && adevar[x->val] == 0 &&
			drumuri[x->val - 1].min >= x->cost + dist[nod]) {
				dist[x->val] = dist[nod] + x->cost;
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
void free_vecini(node *head) {
	node *aux;
	while (head != NULL) {
		aux = head;
		head = head->next;
		free(aux);
	}
}
int main(int argc, char **argv) {
	int i, j;
	graph *a;
	FILE *graf;
	graf = fopen("reprezentanti.in", "r");
	int d = 0;
	int nr_repr;
	int *reprezentanti;
	citire(& a, graf, & reprezentanti, & nr_repr);
	fclose(graf);
	int contor2;
	min_repr drumuri[10001];
	int adevar[10001] = {0};
	FILE *out;
	out = fopen("reprezentanti.out", "w+");
	for (i = 1; i <= a->nr_nodes; i++) {
		drumuri[i - 1].min = 99999999;
		drumuri[i - 1].repr = 99999999;
		if (i <= nr_repr)
			adevar[reprezentanti[i - 1]] = 1;
	}
	for (i = 0; i < nr_repr; i++) {
		int dist[] = { [0 ... 10000] = 99999999 };
		dijkstra(a, reprezentanti[i], dist, adevar, drumuri);
	}
	for (i = 0; i < a->nr_nodes; i++) {
		if (drumuri[i].min == 99999999)
			fprintf(out, "-1\n");
		else
			fprintf(out, "%d %d\n", drumuri[i].repr, drumuri[i].min);
	}
	fclose(out);
	for (i = 0; i <= a->nr_nodes; i++) {
		if (a->neighbours[i] != NULL)
			free_vecini(a->neighbours[i]);
	}
	free(reprezentanti);
	free(a->neighbours);
	free(a);
	return 0;
}
