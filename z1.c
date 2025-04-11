#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// все активационные функции 
double binarna(double x) {
    if (x < 0) {
        return 0;
    }
    else if (x >= 0)
    {
        return 1;
    }
}

double reLu(double x) {
    if (x <= 0) {
        return 0;
    }
    else if (x > 0) {
        return x;
    }
}

double squareplus(double x) {
    double func = 0.5 * (x + sqrt((x*x) + 2));
    return func;
}

double gauss(double x) {
    double res = pow(exp(1), -(x*x));
    return res;
}

double hyperbol(double x) {
    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}

double swish(double x) {
    return x/(1 + (exp(-0.5*x)));
}

// авторежим
int auto_mode(double x_start, double x_xtop, double step, int precision, int width) {
        // вывод чисел в заголовке
        char x = 'x';
        char fst = '1';
        char sec = '2';
        char third = '3';
        char forth = '4';
        char five = '5';
        char six = '6';
        printf("%*.c%*.c%*.c%*.c%*.c%*.c%*.c\n", width, x, width, fst, width, sec, width, third, width, forth, width, five, width, six);
    
        for (double x = x_start; x <= x_xtop; x += step) {
            // вывожу значения с 1 по 6
            printf("%*.*lf", width, precision, x);
            printf("%*.*lf", width, precision, binarna(x));
            printf("%*.*lf", width, precision, reLu(x));
            printf("%*.*lf", width, precision, squareplus(x));
            printf("%*.*lf", width, precision, gauss(x));
            printf("%*.*lf", width, precision, hyperbol(x));
            printf("%*.*lf\n", width, precision, swish(x));
        }
        return 0;
}

int manual_mode(double x_start, double x_xtop, double step, int precision, int width, int *funcs, int count) {
    int x_count = 0;
    for (double x = x_start; x <= x_xtop; x += step)
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
    return 0;
}


int main() {
    // проверка на ошибки E1 и E2
    double x_start, x_xtop, step;
    int precision, width;

    char buff[100];
    char *ptr = buff;
            
    if (fgets(buff, sizeof(buff), stdin) == NULL) {
        printf("E1");
        return 0;
    }
    int chars = sscanf(buff, "%lf %lf %lf %d %d", &x_start, &x_xtop, &step, &precision, &width);
    if (chars < 5) {
        printf("E1");
        return 0;
    }

    if (x_start >= x_xtop) {
        printf("E2");
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
        
        if ((num >= 1) && (num <= 6) && func_count < 100)
            funcs[func_count++] = num;
        ptr = end;
    }

    if (func_count > 0)
        manual_mode(x_start, x_xtop, step, precision, width, funcs, func_count);
    else
        auto_mode(x_start, x_xtop, step, precision, width);

    return 0;
}