#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Função para inicializar a fonte s
void initializeSource(float *s, float f, float dt, int nt) {
    float t;
    float pi = 3.14;

    for (int i = 0; i < nt; i++)
    {
        t = i * dt;
        s[i] = (1 - 2 * pow(pi, 2) * pow(f, 2) * pow(t, 2)) * exp(pow(-pi, 2) * pow(f, 2) * pow(t, 2));
        printf("%f\n", s[i]);
    }
    
}

// Função para propagar a onda acústica usando diferenças finitas
void propagateWave(float *s, float c, int dx, int dy, int dz, float dt,
                    int nx, int ny, int nz, int nt, float u[nx][ny][nz], int xs, int ys, int zs) {
    
    float dEx, dEy, dEz;
    float ***uAnterior = malloc(nx * sizeof(float **));
    float ***uProximo = malloc(nx * sizeof(float **));

    for (int i = 0; i < nx; i++) {
        uAnterior[i] = malloc(ny * sizeof(float *));
        uProximo[i] = malloc(ny * sizeof(float *));
        for (int j = 0; j < ny; j++) {
            uAnterior[i][j] = malloc(nz * sizeof(float));
            uProximo[i][j] = malloc(nz * sizeof(float));
        }
    }

    uAnterior = NULL;
    uProximo = NULL;

    memset(u, 0, nx * ny * nz * sizeof(float));
    memset(uAnterior, 0, nx * ny * nz * sizeof(float));
    memset(uProximo, 0, nx * ny * nz * sizeof(float));

    for (int t = 0; t < nt; t++)
    {
        u[xs][ys][zs] = s[t];
        for (int x = 2; x < nx - 2; x++)
        {
            for (int y = 2; y < ny - 2; y++)
            {
                for (int z = 2; z < nz - 2; z++)
                {
                    //int index = t * (nx * ny * nz) + x * (ny * nz) + y * nz + z;
                    
                    dEx = ((-1.0/12.0)*u[x - 2*dx][y][z] + (4.0/3.0)*u[x - dx][y][z] - (5.0/2.0)*u[x][y][z] + (4.0/3.0)*u[x + dx][y][z] - (1.0/12.0)*u[x + 2*dx][y][z])/pow(dx, 2);
                    dEy = ((-1.0/12.0)*u[x][y - 2*dy][z] + (4.0/3.0)*u[x][y - dy][z] - (5.0/2.0)*u[x][y][z] + (4.0/3.0)*u[x][y + dy][z] - (1.0/12.0)*u[x][y + 2*dy][z])/pow(dy, 2);
                    dEz = ((-1.0/12.0)*u[x][y][z - 2*dz] + (4.0/3.0)*u[x][y][z - dz] - (5.0/2.0)*u[x][y][z] + (4.0/3.0)*u[x][y][z + dz] - (1.0/12.0)*u[x][y][z + 2*dz])/pow(dz, 2);

                    uAnterior[x][y][z] = u[x][y][z];
                    uProximo[x][y][z] = pow(c, 2) * pow(dt, 2) * (dEx + dEy + dEz) - uAnterior[x][y][z] + 2 * u[x][y][z];
                    u[x][y][z] = uProximo[x][y][z];
                    
                }
            } 
        }

        uProximo[xs][ys][zs] += pow(c, 2) * pow(dt, 2) * -s[t];

    }

    free(u);
    free(uAnterior);
    free(uProximo);
    free(s);
    
}

int main() {
    // Parâmetros de entrada
    int xs, ys, zs;  // Posição da fonte
    int dx, dy, dz;  // Resolução espacial
    float dt = 0.01;         // Passo de tempo
    int nx, ny, nz;   // Dimensões da malha tridimensional
    int nt = 20;           // Número de passos de tempo
    float f = 10;  // Frequência de pico da fonte
    int c = 1500; //Velocidade de propagação da onda no meio

    // Aloque memória para os arrays u (campo de pressão) e s (fonte)
    float ***u = malloc(nx * sizeof(float **));

    for (int i = 0; i < nx; i++) {
        u[i] = malloc(ny * sizeof(float *));
        for (int j = 0; j < ny; j++) {
            u[i][j] = malloc(nz * sizeof(float));
        }
    }

    float *s = (float *)malloc(nt * sizeof(float));

    // Inicialize a fonte s
    initializeSource(s, f, dt, nt);

    // Propague a onda acústica
    //propagateWave(s, c, dx, dy, dz, dt, nx, ny, nz, nt, u, xs, ys, zs);

    // Libere a memória alocada
    free(u);
    free(s);

    return 0;
}
