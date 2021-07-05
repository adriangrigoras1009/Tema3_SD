#include<stdio.h>

#include<stdlib.h>

typedef struct graph {
	int nr_nodes;
	int nr_muchii;
	struct node **neighbours;
} graph;
typedef struct node {
	int val;
	struct node *next;
} node;
typedef struct query {
	int n1;
	int n2;
} query;
typedef struct parinti {
	struct nod_list *head;
	int len;
} parinti;
typedef struct nod_list {
	int val;
	int level;
	struct nod_list *next;
} nod_list;
typedef struct Queue {
	struct nodeq *head, *tail;
	int len;
} Queue;
typedef struct nodeq {
	int val;
	struct nodeq *next;
} nodeq;
typedef struct aparitii {
	struct node *head;
} aparitii;
void sort_list(node **t) {
	node *aux1;
	node *aux2;
	for (aux1 = *t; aux1 != NULL; aux1 = aux1->next)
		for (aux2 = aux1->next; aux2 != NULL; aux2 = aux2->next)
			if (aux2->val < aux1->val) {
				int a = aux1->val;
				aux1->val = aux2->val;
				aux2->val = a;
			}
}
void initq(Queue **q) {
	(*q) = (Queue *) malloc(sizeof(Queue));
	(*q)->head = NULL;
	(*q)->tail = NULL;
	(*q)->len = 0;
}
void freeq(Queue *q) {
	free(q);
}
void add_queue(Queue *q, int val) {
	nodeq *t;
	t = malloc(sizeof(nodeq));
	if (q->head == NULL) {
		t->val = val;
		t->next = NULL;
		q->head = t;
		q->tail = t;
	} else {
		t->val = val;
		t->next = NULL;
		q->tail->next = t;
		q->tail = t;
	}
}
int pop_queue(Queue *q) {
	nodeq *t;
	int elem;
	t = q->head;
	elem = t->val;
	q->head = t->next;
	free(t);
	return elem;
}
int is_emptyq(Queue *q) {
	if (q->head == NULL)
		return 0;
	else
		return 1;
}
void construire(graph **a, int muchii, FILE *graf,
	query noduri[], int nr_query) {
	int i;
	for (i = 0; i < muchii; i++) {
		int x, x1;
		fscanf(graf, "%d ", & x);
		fscanf(graf, "%d\n", & x1);
		node *aux, *aux2;
		node *t = (*a)->neighbours[x];
		node *t1 = (*a)->neighbours[x1];
		if (t == NULL) {
			(*a)->neighbours[x] = malloc(sizeof(node));
			(*a)->neighbours[x]->val = x1;
			(*a)->neighbours[x]->next = NULL;
		} else {
			aux = malloc(sizeof(node));
			aux->val = x1;
			aux->next = NULL;
			while (t->next != NULL)
				t = t->next;
			t->next = aux;
		}
		if (t1 == NULL) {
			(*a)->neighbours[x1] = malloc(sizeof(node));
			(*a)->neighbours[x1]->val = x;
			(*a)->neighbours[x1]->next = NULL;
		} else {
			aux2 = malloc(sizeof(node));
			aux2->val = x;
			aux2->next = NULL;
			while (t1->next != NULL)
				t1 = t1->next;
			t1->next = aux2;
		}
	}
	for (i = 0; i < nr_query; i++) {
		fscanf(graf, "%d ", & noduri[i].n1);
		fscanf(graf, "%d\n", & noduri[i].n2);
	}
}
void citire(graph **a, FILE *graf, query **noduri, int *nr_query) {
	int muchii, i;
	(*a) = malloc(sizeof(graph));
	fscanf(graf, "%d ", & (*a)->nr_nodes);
	fscanf(graf, "%d ", & muchii);
	fscanf(graf, "%d\n", & (*nr_query));
	(*a)->nr_muchii = muchii;
	(*a)->neighbours = malloc(((*a)->nr_nodes + 1) *sizeof(node *));
	for (i = 0; i <= (*a)->nr_nodes; i++)
		(*a)->neighbours[i] = NULL;
	*noduri = malloc((*nr_query) *sizeof(query));
	construire(a, muchii, graf, *noduri, *nr_query);
}
void free_vecini(node *head) {
	node *aux;
	while (head != NULL) {
		aux = head;
		head = head->next;
		free(aux);
	}
}
void free_parents(nod_list *head) {
	nod_list *aux;
	while (head != NULL) {
		aux = head;
		head = head->next;
		free(aux);
	}
}
void bfs(graph *a, int sursa, int vazut[], int dest, FILE *out) {
	parinti parinte[10001];
	int i;
	for (i = 0; i <= a->nr_nodes; i++) {
		vazut[i] = 0;
		parinte[i].head = NULL;
	}
	Queue *q;
	int a_ajuns = 0;
	initq(& q);
	vazut[sursa] = 1;
	add_queue(q, sursa);
	int level = 0;
	int contor = 1;
	while (is_emptyq(q) != 0) {
		level++;
		int z;
		int contor_aux = contor;
		contor = 0;
		while (contor_aux != 0) {
			if (is_emptyq(q) == 0)
				break;
			z = pop_queue(q);
			if (dest == z) {
				a_ajuns = 1;
				break;
			}
			node *x = a->neighbours[z];
			while (x != NULL) {
				if (vazut[x->val] == 0) {
					vazut[x->val] = 1;
					add_queue(q, x->val);
					contor++;
					if (parinte[x->val].head == NULL) {
						parinte[x->val].head = malloc(sizeof(nod_list));
						parinte[x->val].head->val = z;
						parinte[x->val].head->level = level;
						parinte[x->val].head->next = NULL;
					} else {
						nod_list *t = parinte[x->val].head;
						while (t->next != NULL)
							t = t->next;
						nod_list *aux = malloc(sizeof(nod_list));
						aux->val = z;
						aux->level = level;
						aux->next = NULL;
						t->next = aux;
					}
				} else if (parinte[x->val].head != NULL &&
					parinte[x->val].head->level == level) {
					nod_list *t = parinte[x->val].head;
					while (t->next != NULL)
						t = t->next;
					nod_list *aux = malloc(sizeof(nod_list));
					aux->val = z;
					aux->level = level;
					aux->next = NULL;
					t->next = aux;
				}
				if (x->next == NULL)
					break;
				x = x->next;
			}
			contor_aux--;
		}
		if (dest == z) {
			a_ajuns = 1;
			while (is_emptyq(q) != 0) {
				int degeaba = pop_queue(q);
			}
			break;
		}
	}
	freeq(q);
	if (a_ajuns == 1) {
		Queue *q1;
		int noduri_comune = 1;
		aparitii *apare;
		apare = malloc(sizeof(aparitii));
		apare->head = NULL;
		int contor_aux;
		for (i = 0; i <= a->nr_nodes; i++) {
			vazut[i] = 0;
		}
		initq(& q1);
		int de_adaugat = 0;
		add_queue(q1, dest);
		vazut[dest] = 1;
		contor = 1;
		while (is_emptyq(q1) != 0) {
			contor_aux = contor;
			contor = 0;
			while (contor_aux != 0) {
				int z = pop_queue(q1);
				nod_list *x = parinte[z].head;
				while (x != NULL) {
					if (vazut[x->val] == 0) {
						vazut[x->val] = 1;
						add_queue(q1, x->val);
						contor++;
						de_adaugat = x->val;
					}
					if (x->next == NULL)
						break;
					x = x->next;
				}
				contor_aux--;
			}
			if (contor == 1) {
				noduri_comune++;
				if (apare->head == NULL) {
					apare->head = malloc(sizeof(node));
					apare->head->val = de_adaugat;
					apare->head->next = NULL;
				} else {
					node *final = apare->head;
					while (final->next != NULL)
						final = final->next;
					node *adaug = malloc(sizeof(node));
					adaug->val = de_adaugat;
					adaug->next = NULL;
					final->next = adaug;
				}
			}
		}
		if (apare->head == NULL) {
			apare->head = malloc(sizeof(node));
			apare->head->val = dest;
			apare->head->next = NULL;
		} else {
			node *final = apare->head;
			while (final->next != NULL)
				final = final->next;
			node *adaug = malloc(sizeof(node));
			adaug->val = dest;
			adaug->next = NULL;
			final->next = adaug;
		}
		sort_list(& apare->head);
		fprintf(out, "%d ", noduri_comune);
		node *afis = apare->head;
		while (afis != NULL) {
			fprintf(out, "%d ", afis->val);
			if (afis->next == NULL)
				break;
			afis = afis->next;
		}
		free_vecini(apare->head);
		free(apare);
		freeq(q1);
	} else {
		fprintf(out, "0");
	}
	for (i = 0; i <= a->nr_nodes; i++)
		if (parinte[i].head != NULL)
			free_parents(parinte[i].head);
}
int main() {
	int i, j;
	graph *a;
	FILE *graf;
	graf = fopen("drumuri.in", "r");
	int d = 0;
	int nr_query;
	query *noduri;
	citire(& a, graf, & noduri, & nr_query);
	fclose(graf);
	int vazut[10001];
	FILE *out;
	out = fopen("drumuri.out", "w");
	for (i = 0; i < nr_query; i++) {
		bfs(a, noduri[i].n1, vazut, noduri[i].n2, out);
		fprintf(out, "\n");
	}
	fclose(out);
	for (i = 0; i <= a->nr_nodes; i++) {
		if (a->neighbours[i] != NULL)
			free_vecini(a->neighbours[i]);
	}
	free(noduri);
	free(a->neighbours);
	free(a);
}
