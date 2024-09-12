#include <stdio.h>
#include <string.h>
#include <ctype.h>

void print_string (unsigned char a[], int len);


int len; 
unsigned char cipher[1000] = "Tfewzuvekzrczkp zj ivjvimzex rlkyfizqvu ivjkiztkzfej fe zewfidrkzfe rttvjj reu uzjtcfjliv, zetcluzex dvrej wfi gifkvtkzex gvijferc gizmrtp reu gifgizvkrip zewfidrkzfe. Zekvxizkp zj xlriuzex rxrzejk zdgifgvi zewfidrkzfe dfuzwztrkzfe fi uvjkiltkzfe, reu zetcluvj vejlizex zewfidrkzfe efe-ivgluzrkzfe reu rlkyvekztzkp. Rmrzcrszczkp zj vejlizex kzdvcp reu ivczrscv rttvjj kf reu ljv fw zewfidrkzfe. ";
unsigned char dechipher_text [1000];

int main() {
 	
    char commonWords[MAX_WORDS][MAX_WORD_LENGHT];
    len = strlen(cipher);
	print_string(cipher,len);

	int aukerak[5];
    int kont_aukerak;
    int passwrd;
    char* fileName; //ToDO poner este nombre adecuadamente y crear el fitxero: https://github.com/first20hours/google-10000-english/blob/master/google-10000-english-no-swears.txt

    printf("Hitzen fitxategia irakurtzen (&s)", fileName)
    printf("Desplazamenduak aztertzen:\n\n");

    while (passwrd < 25)
    {
        decipher(passwrd);
        kalk_decipher();
        passwrd++;
    }

   	return 0;
}

void hitzakIrakurri(char *fileName)
{
    FILE *artx = fopen(fileName, "r");
    if(artx == NULL)
    {
        printf("Errorea fitxategia irekitzerakoan");
        return;
    }

    int i = 0;
    while(fscan(artx, "%s", commonWords[i]) != EOF  && i < MAX_WORDS)
    {
        i++;
    } 

    fclose(artx);
}

void decipher(int passwrd)
{
    char a;
    for(int j = 0; j <= len; j++)
    {
        if(isalpha(cipher[j]))
        {
            if(isupper(cipher [j]))
            {
                a = 'A';
            }else{
                a = 'a';
            }

            dechipher_text[j] = (cipher[j] - a + passwrd) % 26 +a; 
        }else{
            dechipher_text [j] = cipher [j];
        }
    }
}

void kalk_decipher()
{
    int probab = 0;
    char *hitza = strtok(dechipher_text, " ");

    for(int i = 0; i <= len; i++)
    {
        for(int j = 0; j < MAX_WORDS; j++)
        {
            if(strcm(hitza, commonWords[j]) == 0)
            {
                probab++;
            }
        }
    }

    if(probab/len >= 0.5)
    {
        print_string(dechipher_text, len);
        printf("Desplazamendua: %d; Segurtasun Portzentaia %d\n", passwrd, probab/len);
    }
}

void print_string (unsigned char a[], int len)
{
	for (int i=0; i<len; i++)
	{
		printf("%c",a[i]);
	}
	printf("\n");
}