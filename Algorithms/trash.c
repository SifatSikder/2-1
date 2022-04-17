#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void rabin_Karp(char *Text, char *Pattern, int d, int q)
{
    int n = strlen(Text);
    int m = strlen(Pattern);

    // finding h=(pow(d,m-1))mod q;
    long long int h = 1;
    for (int i = 1; i <= m - 1; i++)
    h *= d; // pow(d,m-1)
    h = h % q;

    long long int p = 0;
    long long int t[n - m + 1];
    t[0] = 0;
    for (int i = 0; i < m; i++)
    {
        p = (d * p + Pattern[i]-48) % q;
        t[0] = (d * t[0] + Text[i]-48) % q;
    }

    for (int s = 0; s < n - m; s++)
    {
        if (p == t[s]) // mod value matched now check its valid match or a spurious hit
        {
            int flag = 1;
            for (int i = 0; i < m; i++)
            {
                if (Pattern[i] != Text[s + i])
                {
                    flag = 0;
                    break;
                }
            }
            if (flag)
                printf("Pattern occurs with shift %d\n", s);
        }

        if (s < n - m - 1) // mod value mismatched now time to shift the text using horner rule
        {
            if ((((t[s] - (Text[s]) * h) * d + (Text[s + m])) % q)>=0)
            {
                t[s + 1] = (((t[s] - (Text[s]) * h) * d + (Text[s + m])) % q);
            }
            else
            {
                t[s + 1] =q + (((t[s] - (Text[s]) * h) * d + (Text[s + m])) % q);
            }
            
            
        }
    }
}
int main(void)
{

    // char Text[] = "GEEKS FOR GEEKS";
    // char Pattern[] = "GEEK";

    char Text[] = "2314153141526739921";
    char Pattern[] = "31415";
    rabin_Karp(Text, Pattern, 256, 13);

    return 0;
}