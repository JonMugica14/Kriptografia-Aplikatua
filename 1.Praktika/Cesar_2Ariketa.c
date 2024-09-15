
#include <stdio.h>
#include <string.h>
#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000
unsigned char ema[1000];
unsigned char deszifratua[1000];

void print_string(unsigned char a[], int len);

void fitxategiaAtzitu(char *izena, char hitzak[MAX_WORDS][MAX_WORD_LENGTH])
{
    FILE *artx = fopen(izena, "r");
    if (artx == NULL)
    {
        printf("Ezin izan da ireki fitxategia %s\n", izena);
        return;
    }

    int i = 0;
    while (fscanf(artx, "%s", hitzak[i]) != EOF && i < MAX_WORDS)
    {
        i++;
    }

    fclose(artx);
}

void bihurtu(unsigned char mezua[], int pad, int len)
{

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
    deszifratua[len] = '\0'; 

    for (int i = 0; i < len; i++)
    {
        ema[i] = deszifratua[i];
    }
    char hitzak[MAX_WORDS][MAX_WORD_LENGTH];

    fitxategiaAtzitu("1000-most-common-words.txt", hitzak);
    char *word = strtok(deszifratua, " ");

    int count = 0;
    while (word != NULL && count < 6)
    {
        int dago = 0;
        for (int i = 0; i < MAX_WORDS; i++)
        {
            if (strcmp(word, hitzak[i]) == 0)
            {
                dago = 1;
            }
        }

        if (dago)
        {

            count++;
        }

        word = strtok(NULL, " ");
    }

    if (count == 6)
    {
        printf("Hau da deszifratutako mezua %d-ko desplazamenduarekin: \n", pad);

        print_string(ema, len);

        printf("\n");
    }
}

void maiztAurkitu(unsigned char mezua[], int len)
{
    
    int maizt[26] = {0};
    int pad;

    for (int i = 0; i < len; i++)
    {
        int hitz = mezua[i];
        if (hitz >= 'A' && hitz <= 'Z')
        {
            maizt[hitz - 'A']++;
        }
        else if (hitz >= 'a' && hitz <= 'z')
        {
            maizt[hitz - 'a']++;
        }
    }

    int maxFreq = 0;
    int maxIndex = 0;
    for (int i = 0; i < 26; i++)
    {
        if (maizt[i] > maxFreq)
        {
            maxFreq = maizt[i];
            maxIndex = i;
        }
    }

    for (int i = 0; i < 26; i++)
    {
        pad = i - maxIndex;
        if (pad < 0)
            pad += 26;

        bihurtu(mezua, pad, len);

    }
}
void print_string(unsigned char a[], int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%c", a[i]);
    }
    printf("\n");
}

int main()
{

    unsigned char mezua[1000] = "Tfewzuvekzrczkp zj ivjvimzex rlkyfizqvu ivjkiztkzfej fe zewfidrkzfe rttvjj reu uzjtcfjliv, zetcluzex dvrej wfi gifkvtkzex gvijferc gizmrtp reu gifgizvkrip zewfidrkzfe. Zekvxizkp zj xlriuzex rxrzejk zdgifgvi zewfidrkzfe dfuzwztrkzfe fi uvjkiltkzfe, reu zetcluvj vejlizex zewfidrkzfe efe-ivgluzrkzfe reu rlkyvekztzkp. Rmrzcrszczkp zj vejlizex kzdvcp reu ivczrscv rttvjj kf reu ljv fw zewfidrkzfe.";

    int len = strlen(mezua);

    maiztAurkitu(mezua, len);

    return 0;
}