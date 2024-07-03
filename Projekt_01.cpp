#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#pragma warning(disable:4996)

double f_scisle(double x, double a); //funckja do liczenia wartosci scislej cos(x+a)
double f_szereg(double x, double a, double e, long long M); //=||= wartosci szeregowej cos(x+a)

long long j = 1; //liczba elementow szeregu, inkrementowana
int out_con = 1; //"bool" do sprawdzenia warunku wyjscia

int main(int argc, char* argv[])
{
    setlocale(LC_CTYPE, "polish"); //polskie znaki
    double a; //stała a z wzoru cos(x+a)
    double min, max; //przedział <a,b>
    long long n; //liczba punktow podziału
    double e; //stała epsilon - dokładność
    long long M; //max ilość wyrazów szeregu

    printf("Program obliczania wartości funkcji f(x) = cos(x+a).\n");
    printf("Podaj stałą a, przedział [p,q], liczbę punktów podziału, dokładność i maksymalną liczbę wyrazów szeregu : \n");
    if (scanf("%lf%lf%lf%lld%lf%lld", &a, &min, &max, &n, &e, &M) != 6)
    {
        printf("Podano błędny typ danych. Kończę pracę programu.\n");
        exit(1);
    }
    if (min > max)
    {
        printf("Początek przedziału nie może być mniejszy od jego końca. Zamieniam w przedział : \n");
        printf("[%lf, %lf] -> [%lf, %lf]\n", min, max, max, min);
        int pom = max;
        max = min;
        min = pom;
    }
    if (n <= 0 || M < 0 || e < 0)
    {
        printf("Liczba punktów podziału nie może być niedodatnia. Dokładność oraz liczba wyrazów szeregu nie może być ujemna. Kończę pracę programu.\n");
        exit(1);
    }
    printf("UWAGA: Jeżeli podałeś niecałkowite wartości n lub M, zostały one zaokrąglone do najbliższej mniejszej liczby całkowitej.\n\n");
    //obliczanie kroku dx
    double dx;
    if (max != min)
        dx = (max - min) / n;
    else
        dx = n;

    FILE* plik_wynikowy = fopen("wyniki.txt", "w");
    if (plik_wynikowy == NULL) {
        printf("Nie udało się otworzyć pliku wynikowego.\n");
        exit(1);
    }

    printf("%-15s | %-13s | %-13s | %s | %s\n", "x", "f_szereg(x)", "f_ścisłe(x)", "liczba wyrazów szeregu", "warunek stopu (e lub M)");
    fprintf(plik_wynikowy, "%-16s | %-13s | %-13s | %s | %s\n", "x", "f_szereg(x)", "f_ścisłe(x)", "liczba wyrazów szeregu", "warunek stopu (e lub M)");

    for (double i = min; i <= max+0.5*dx; i += dx)
    {
        double szereg_val = f_szereg(i, a, e, M);
        double scisle_val = f_scisle(i, a);
        int ile_wyrazow = M;
        if (out_con) {
            ile_wyrazow = j;
        }
        else {
            ile_wyrazow = M;
        }
        printf("%-15.8lf | %-13.8lf | %-13.8lf | %-21d  | %s\n", i, szereg_val, scisle_val, ile_wyrazow, (out_con) ? "e" : "M");
        fprintf(plik_wynikowy, "%-15.8lf  | %-13.8lf | %-13.8lf | %-21d  | %s\n", i, szereg_val, scisle_val, ile_wyrazow, (out_con) ? "e" : "M");
    }

    fclose(plik_wynikowy);
}

double f_scisle(double x, double a)
{
    return cos(x + a);
}

double f_szereg(double x, double a, double e, long long M)
{
    double ans = cos(a);
    j = 1;
    int odd_even = 1; //sin czy cos
    int s = 1, c = 1; //+ czy -
    double x_power = 1.0;
    double factorial = 1.0;
    double sum = 0, pom = -2;
    out_con = 1;
    do
    {
        x_power *= x; //liczymy x^n jako x*x*x*x...
        factorial *= j; // liczymy n! jako j*(j+1)*(j+2)...
        if (odd_even % 2 != 0) 
        {
            if (isnan(x_power * sin(a)) || isnan(factorial))
                return ans;
            sum = (pow(-1, s) * x_power * sin(a)) / factorial;
            if (fabs(sum - pom) < e)
            {
                ans += sum;
                return ans;
            }
            ans += sum;
            pom = sum;
            s++;
        }
        else
        {
            if (isnan(x_power * cos(a)) || isnan(factorial))
                return ans;
            sum = (pow(-1, c) * x_power * cos(a)) / factorial;
            if (fabs(sum - pom) < e)
            {
                ans += sum;
                return ans;
            }
            ans += sum;
            pom = sum;
            c++;
        }
        j++;
        odd_even++;
    } while (j < M);
    out_con = 0;
    return ans;
}
