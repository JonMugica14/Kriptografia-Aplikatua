#include <stdio.h>
#include <string.h>

void bihurtu(unsigned char mezua[], int pad, int len)
{

    unsigned char deszifratua[1000];

    for (int i = 0; i < len; i++)
    {
        if (mezua[i] != 32 && mezua[i] != 46 && mezua[i] != 44)
        {
            if (mezua[i] < 123 && mezua[i] > 96)
            {
                if ((mezua[i] + pad) > 96 && (mezua[i] + pad) < 123)
                    deszifratua[i] = mezua[i] + pad;
                else if (mezua[i] + pad < 97)
                    deszifratua[i] = mezua[i] + pad + 26;
                else if (mezua[i] + pad > 122)
                    deszifratua[i] = mezua[i] + pad - 26;
            }
            else
            {
                if (mezua[i] + pad < 91 && mezua[i] + pad > 64)
                    deszifratua[i] = mezua[i] + pad;
                else if (mezua[i] + pad < 65)
                    deszifratua[i] = mezua[i] + pad + 26;
                else if (mezua[i] + pad > 90)
                    deszifratua[i] = mezua[i] + pad - 26;
            }
        }
        else
        {
            deszifratua[i] = mezua[i];
        }
    }

    printf("\n");

    for (int i = 0; i < len; i++)
    {
        printf("%c", deszifratua[i]);
    }
    printf("\n");
}

void maiztAurkitu(unsigned char mezua[], int len)
{

    int maizt[26];
    int pad;

    for (int i = 0; i < 26; i++)
        maizt[i] = 0;

    for (int i = 0; i < len - 1; i++)
    {
        int letra = mezua[i];
        if (letra < 91 && letra > 64)
        {
            maizt[letra - 65]++;
        }
        else if (letra < 123 && letra > 96)
        {
            maizt[letra - 97]++;
        }
    }

    int erabiliena1 = 0;

    for (int i = 0; i < 26; i++)
    {
        if (maizt[i] > erabiliena1)
            erabiliena1 = i;
    }

    printf("\n");
    printf("Maiztasuna aurkitu ondoren, ingelesean gehien erabiltzen diren bokalengatik aldatuko ditugu, a, e eta i:");
    printf("\nE hizkia gaitik aldaturik: \n");
    pad = erabiliena1;

    bihurtu(mezua, pad, len);
    printf("\n");
    printf("\nA hizkia gaitik aldaturik: \n");
    pad = 97 - (erabiliena1 + 97);

    bihurtu(mezua, pad, len);
    printf("\n");
    printf("\nI hizkia gaitik aldaturik: \n");
    pad = 105 - (erabiliena1 + 97);
    bihurtu(mezua, pad, len);
    printf("\n");
}

int main()
{

    unsigned char mezua[1000] = "Jypwavnyhwof pz aol kpzjpwspul aoha ltivkplz aol wypujpwslz, tlhuz, huk tlaovkz mvy aol ayhuzmvythapvu vm khah pu vykly av opkl aolpy zlthuapj jvualua, wylclua aolpy buhbaovypglk bzl, vy wylclua aolpy buklaljalk tvkpmpjhapvu.";

    int len = strlen(mezua);

    maiztAurkitu(mezua, len);

    return 0;
}