#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double G = 1.0;

typedef struct {
    double mass;
    double pos[3];
    double acc[3];
} Particle;

double get_rand() {
    return (double)(rand() % 10000) / 10000.0;
}

int main(int argc, char *argv[]) {
    int nb_particles = atoi(argv[1]);

    Particle* p_arr = (Particle*)malloc(sizeof(Particle) * nb_particles);

    if (!p_arr) {
        printf("p_arr is not allocated\n");
        exit(1);
    }

    // initializing
    for (int i = 0; i < nb_particles; i++) {
        p_arr[i].mass = get_rand();
#ifdef DEBUG
        printf("[%d] mass = %f\n", i, p_arr[i].mass);
#endif

        for (int j = 0; j < 3; j++) {
            p_arr[i].pos[j] = get_rand();
        }
        for (int j = 0; j < 3; j++) {
            p_arr[i].acc[j] = 0.0;
        }
    }

    printf("\n");

    // calculating accel
    for (int i = 0; i < nb_particles; i++) {
        for (int j = 0; j < nb_particles; j++) {
            if (j == i) continue;

            double dist_sqr = 0.0;
            for (int k = 0; k < 3; k++)
                dist_sqr += (p_arr[j].pos[k] - p_arr[i].pos[k]) * (p_arr[j].pos[k] - p_arr[i].pos[k]);

            double gravity = G * p_arr[i].mass * p_arr[j].mass / dist_sqr;
            for (int k = 0; k < 3; k++)
                p_arr[i].acc[k] += (gravity / p_arr[i].mass) * ((p_arr[j].pos[k] - p_arr[i].pos[k]) / sqrt(dist_sqr));
        }

#ifdef DEBUG
        printf("[%d]", i);
        for (int k = 0; k < 3; k++)
            printf(" %f", p_arr[i].acc[k]);
        printf("\n");
#endif
    }

    free(p_arr);

    return 0;
}

