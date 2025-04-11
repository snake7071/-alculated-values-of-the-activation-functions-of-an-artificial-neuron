#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

double binarna(double x) {
    return x >= 0 ? 1.0 : 0.0;
}

double reLu(double x) {
    return x > 0 ? x : 0.0;
}

double squareplus(double x) {
    return 0.5 * (x + sqrt(x * x + 2));
}

double gauss(double x) {
    return exp(-x * x);
}

double hyperbol(double x) {
    double ex = exp(x);
    double e_minus_x = exp(-x);
    return (ex - e_minus_x) / (ex + e_minus_x);
}

double swish(double x) {
    return x / (1 + exp(-0.5 * x));
}

void auto_mode(double x_start, double x_xtop, double step, int precision, int width) {
    printf("%*s", width, "x");
    for (int i = 1; i <= 6; i++) 
        printf("%*d", width, i);
    printf("\n");

    for (double x = x_start; x <= x_xtop + 1e-9; x += step) {
        printf("%*.*lf", width, precision, x);
        printf("%*.*lf", width, precision, binarna(x));
        printf("%*.*lf", width, precision, reLu(x));
        printf("%*.*lf", width, precision, squareplus(x));
        printf("%*.*lf", width, precision, gauss(x));
        printf("%*.*lf", width, precision, hyperbol(x));
        printf("%*.*lf\n", width, precision, swish(x));
    }
}

void manual_mode(double x_start, double x_xtop, double step, int precision, int width, int *funcs, int count) {
    int x_count = 0;
    for (double x = x_start; x <= x_xtop + 1e-9; x += step)
        x_count++;
    
    double *x_values = malloc(x_count * sizeof(double));
    for (int i = 0; i < x_count; i++)
        x_values[i] = x_start + i * step;

    printf("%*s", width, "x");
    for (int i = 0; i < x_count; i++)
        printf("%*.*lf", width, precision, x_values[i]);
    printf("\n");

    for (int i = 0; i < count; i++) {
        printf("%*d", width, funcs[i]);
        
        for (int j = 0; j < x_count; j++) {
            double val;
            switch(funcs[i]) {
                case 1: val = binarna(x_values[j]); break;
                case 2: val = reLu(x_values[j]); break;
                case 3: val = squareplus(x_values[j]); break;
                case 4: val = gauss(x_values[j]); break;
                case 5: val = hyperbol(x_values[j]); break;
                case 6: val = swish(x_values[j]); break;
                default: val = 0.0;
            }
            printf("%*.*lf", width, precision, val);
        }
        printf("\n");
    }
    
    free(x_values);
}

int main() {
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("E1\n");
        return 0;
    }

    double x_start, x_xtop, step;
    int precision, width;
    char *ptr = buffer;
    
    int parsed = sscanf(ptr, "%lf %lf %lf %d %d", &x_start, &x_xtop, &step, &precision, &width);
    if (parsed != 5 || x_start >= x_xtop) {
        printf(parsed != 5 ? "E1\n" : "E2\n");
        return 0;
    }

    int funcs[100];
    int func_count = 0;
    
    while (*ptr && !isspace(*ptr)) ptr++; 
    for (int i = 0; i < 4; i++) {
        ptr += strspn(ptr, " \t\n");
        ptr += strcspn(ptr, " \t\n");
    }
    ptr += strspn(ptr, " \t\n");

    while (*ptr) {
        char *end;
        long num = strtol(ptr, &end, 10);
        if (end == ptr) break;
        
        if (num >= 1 && num <= 6 && func_count < 100)
            funcs[func_count++] = num;
        
        ptr = end;
    }

    if (func_count > 0)
        manual_mode(x_start, x_xtop, step, precision, width, funcs, func_count);
    else
        auto_mode(x_start, x_xtop, step, precision, width);

    return 0;
}