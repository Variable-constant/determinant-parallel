#include <iostream>
#include <vector>
#include <fstream>
#include <omp.h>
using namespace std;
int th;
float determinant(vector <vector <float>> a);
float omp_determinant(vector <vector <float>> a);
vector <vector <float>> read_matrix(string file);
vector <vector <float>> create(int n);
int main(int argc, char* argv[]) {
    float res;
    int max = atoi(argv[1]);
    th = 0;
    vector <vector <float>> a = read_matrix(argv[2]);
    double start = omp_get_wtime();
    res = determinant(a);
    double time = omp_get_wtime() - start;
    if (argc > 3) {
        ofstream out;
        out.open(argv[3]);
        out << "Determinant: ";
        out << res;
        out << endl;
        out.close();
    }
    else {
        printf("Determinant: % g\n", res);
    }
    printf("\nWithout OpenMP % f ms\n", time * 1000);
    for (int i = 0; i <= max; i++) {
        th = i;
        start = omp_get_wtime();
        omp_determinant(a);
        time = omp_get_wtime() - start;
        printf("\nTime(%i thread(s)) : % f ms\n", i, time * 1000);
    }
    
    return 0;
}

float determinant(vector<vector<float>> a) {
    int n = a.size();
    float res = 1;
    for (int i = 0; i < n; i++) {
        int cur = -1;
        for (int j = i; j < n; j++) {
            if (a[i][j] != 0) {
                cur = j;
                break;
            }
        }
        if (cur == -1) {
            return 0;
        }
        if (cur != i) {
            swap(a[i], a[cur]);
            res = -res;
        }
        for (int j = i + 1; j < n; j++) {
            float proportion = a[j][i] / a[i][i];
            for (int k = i + 1; k < n; k++) {
                a[j][k] -= a[i][k] * proportion;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        res *= a[i][i];
    }
    return res;
}

float omp_determinant(vector<vector<float>> a) {
    int n = a.size();
    float res = 1;
    for (int i = 0; i < n; i++) {
        int cur = -1;
        for (int j = i; j < n; j++) {
            if (a[i][j] != 0) {
                cur = j;
                break;
            }
        }
        if (cur == -1) {
            return 0;
        }
        if (cur != i) {
            swap(a[i], a[cur]);
            res = -res;
        }
#pragma omp parallel for num_threads(th)
        for (int j = i + 1; j < n; j++) {
            float proportion = a[j][i] / a[i][i];
            for (int k = i + 1; k < n; k++) {
                a[j][k] -= a[i][k] * proportion;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        res *= a[i][i];
    }
    return res;
}

vector <vector <float>> create(int n) {
    vector<vector<float>> a(n, vector <float>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = float(rand()) / float(RAND_MAX) * 100;
        }
    }
    return a;
}

vector <vector <float>> read_matrix(string file) {
    ifstream in;
    in.open(file);
    if (!in.is_open()) {
        cout << "Opening file error";
        exit(0);
    }
    int n;
    in >> n;
    vector <vector <float>> a(n, vector <float>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            in >> a[i][j];
        }
    }
    in.close();
    return a;
}
