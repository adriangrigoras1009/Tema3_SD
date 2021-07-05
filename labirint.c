#include<stdio.h>

#include<stdlib.h>

typedef struct vzt {
	int matrice[100][100];
} vzt;
void construire(FILE *graf, int linii, int coloane, int matrice[][coloane],
	int *directii, int nr_directii, int *x_coord, int *y_coord) {
	int i, j;
	int z = 0;
	for (i = 0; i < linii; i++) {
		for (j = 0; j < coloane; j++) {
			fscanf(graf, "%d ", & matrice[i][j]);
			if (matrice[i][j] == 2) {
				*x_coord = i;
				*y_coord = j;
			}
		}
		fscanf(graf, "\n");
	}
	for (i = 0; i < nr_directii; i++)
		fscanf(graf, "%d ", & directii[i]);
}
int rezolvare(int coloane, int linii, int matrice[][coloane], int nr_directii,
	int *directii, int x_coord, int y_coord, int go, int *numar_opriri) {
	static int contor;
	static int frunze[100][100] = {0};
	static vzt vazut[1000] = {0};
	if (go < nr_directii - 1) {
		if (directii[go] == 0) {
			while (x_coord - 1 >= 0) {
				x_coord--;
				if (matrice[x_coord][y_coord] == 1)
					break;
				else if ((directii[go + 1] == 1 &&
				x_coord + 1 <= linii - 1 &&
				matrice[x_coord + 1][y_coord] != 1 &&
				x_coord + 1 <= linii - 1) ||
				(directii[go + 1] == 2 && y_coord - 1 >= 0 &&
				matrice[x_coord][y_coord - 1] != 1 &&
				y_coord - 1 >= 0) ||
				(directii[go + 1] == 3 &&
				y_coord + 1 <= coloane - 1 &&
				matrice[x_coord][y_coord + 1] != 1 &&
				y_coord + 1 <= coloane - 1))
					if (vazut[go + 1].matrice[x_coord][y_coord] == 0) {
						rezolvare(coloane, linii, matrice, nr_directii,
							directii, x_coord, y_coord, go + 1, numar_opriri);
						vazut[go + 1].matrice[x_coord][y_coord] = 1;
					}
			}
		} else if (directii[go] == 1) {
			while (x_coord + 1 <= linii - 1) {
				x_coord++;
				if (matrice[x_coord][y_coord] == 1)
					break;
				else if ((directii[go + 1] == 0 &&
				x_coord - 1 >= 0 &&
				matrice[x_coord - 1][y_coord] != 1 &&
				x_coord - 1 >= 0) ||
				(directii[go + 1] == 2 &&
				y_coord - 1 >= 0 &&
				matrice[x_coord][y_coord - 1] != 1 &&
				y_coord - 1 >= 0) ||
				(directii[go + 1] == 3 &&
				y_coord + 1 <= coloane - 1 &&
				matrice[x_coord][y_coord + 1] != 1 &&
				y_coord + 1 <= coloane - 1))
					if (vazut[go + 1].matrice[x_coord][y_coord] == 0) {
						rezolvare(coloane, linii, matrice, nr_directii,
							directii, x_coord, y_coord, go + 1, numar_opriri);
						vazut[go + 1].matrice[x_coord][y_coord] = 1;
					}
			}
		} else if (directii[go] == 2) {
			while (y_coord - 1 >= 0) {
				y_coord--;
				if (matrice[x_coord][y_coord] == 1)
					break;
				else if ((directii[go + 1] == 1 &&
				x_coord + 1 < linii &&
				matrice[x_coord + 1][y_coord] != 1 &&
				x_coord + 1 <= linii - 1) ||
				(directii[go + 1] == 0 &&
				x_coord - 1 >= 0 &&
				matrice[x_coord - 1][y_coord] != 1 &&
				x_coord - 1 >= 0) ||
				(directii[go + 1] == 3 &&
				y_coord + 1 <= coloane - 1 &&
				matrice[x_coord][y_coord + 1] != 1 &&
				y_coord + 1 <= coloane - 1))
					if (vazut[go + 1].matrice[x_coord][y_coord] == 0) {
						rezolvare(coloane, linii, matrice, nr_directii,
							directii, x_coord, y_coord, go + 1, numar_opriri);
						vazut[go + 1].matrice[x_coord][y_coord] = 1;
					}
			}
		} else if (directii[go] == 3) {
			while (y_coord + 1 <= coloane - 1) {
				y_coord++;
				if (matrice[x_coord][y_coord] == 1)
					break;
				else if ((directii[go + 1] == 1 &&
				x_coord + 1 < linii &&
				matrice[x_coord + 1][y_coord] != 1 &&
				x_coord + 1 <= linii - 1) ||
				(directii[go + 1] == 2 &&
				y_coord - 1 >= 0 &&
				matrice[x_coord][y_coord - 1] != 1 &&
				y_coord - 1 >= 0) ||
				(directii[go + 1] == 0 &&
				x_coord - 1 >= 0 &&
				matrice[x_coord - 1][y_coord] != 1 &&
				x_coord - 1 >= 0))
					if (vazut[go + 1].matrice[x_coord][y_coord] == 0) {
						rezolvare(coloane, linii, matrice, nr_directii,
							directii, x_coord, y_coord, go + 1, numar_opriri);
						vazut[go + 1].matrice[x_coord][y_coord] = 1;
					}
			}
		}
	} else if (go == nr_directii - 1) {
		if (directii[go] == 0) {
			while (x_coord - 1 >= 0) {
				x_coord--;
				if (matrice[x_coord][y_coord] == 1)
					break;
				if (frunze[x_coord][y_coord] != 1) {
					contor++;
					frunze[x_coord][y_coord] = 1;
				} else {
					break;
				}
			}
		} else if (directii[go] == 1) {
			while (x_coord + 1 <= linii - 1) {
				x_coord++;
				if (matrice[x_coord][y_coord] == 1)
					break;
				if (frunze[x_coord][y_coord] != 1) {
					contor++;
					frunze[x_coord][y_coord] = 1;
				} else {
					break;
				}
			}
		} else if (directii[go] == 2) {
			while (y_coord - 1 >= 0) {
				y_coord--;
				if (matrice[x_coord][y_coord] == 1)
					break;
				if (frunze[x_coord][y_coord] != 1) {
					contor++;
					frunze[x_coord][y_coord] = 1;
				} else {
					break;
				}
			}
		} else if (directii[go] == 3) {
			while (y_coord + 1 <= coloane - 1) {
				y_coord++;
				if (matrice[x_coord][y_coord] == 1)
					break;
				if (frunze[x_coord][y_coord] != 1) {
					contor++;
					frunze[x_coord][y_coord] = 1;
				} else {
					break;
				}
			}
		}
	}
	*numar_opriri = contor;
}
int main() {
	int linii, coloane, nr_directii, i, x_coord, y_coord;
	int numar_opriri = 0, j;
	int *directii;
	FILE *graf;
	graf = fopen("labirint.in", "r");
	fscanf(graf, "%d ", & linii);
	fscanf(graf, "%d ", & coloane);
	fscanf(graf, "%d\n", & nr_directii);
	int matrice[linii][coloane];
	directii = malloc(nr_directii *sizeof(int));
	construire(graf, linii, coloane, matrice, directii,
		nr_directii, & x_coord, & y_coord);
	fclose(graf);
	int go = 0;
	rezolvare(coloane, linii, matrice, nr_directii, directii,
		x_coord, y_coord, go, & numar_opriri);
	FILE *out;
	out = fopen("labirint.out", "w+");
	fprintf(out, "%d\n", numar_opriri);
	fclose(out);
	free(directii);
	return 0;
}
