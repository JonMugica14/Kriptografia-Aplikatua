#include <stdio.h>
#include <string.h>
#include <ctype.h>
void print_string (unsigned char a[], int len);


int len; 
unsigned char cipher[1000] = "Tfewzuvekzrczkp zj ivjvimzex rlkyfizqvu ivjkiztkzfej fe zewfidrkzfe rttvjj reu uzjtcfjliv, zetcluzex dvrej wfi gifkvtkzex gvijferc gizmrtp reu gifgizvkrip zewfidrkzfe. Zekvxizkp zj xlriuzex rxrzejk zdgifgvi zewfidrkzfe dfuzwztrkzfe fi uvjkiltkzfe, reu zetcluvj vejlizex zewfidrkzfe efe-ivgluzrkzfe reu rlkyvekztzkp. Rmrzcrszczkp zj vejlizex kzdvcp reu ivczrscv rttvjj kf reu ljv fw zewfidrkzfe. ";
unsigned char dechipher_text [1000];
int main() {
 	
    len = strlen(cipher);
	print_string(cipher,len);

	int aukerak[5];
    int kont_aukerak;
    int passwrd;
    //La idea es probra con cada posibilidad y al final analizar la cantidad de veces que aparece una letra o varias, y si aparecen ciertas palabras cortas comunes tipo "and, you, me, I, etc."

    int kont = 0;
    while (kont < 5 && passwrd < 25)
    {
        decipher(passwrd);
        kalk_decipher();
    }


   	return 0;
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
    int kont_e = 0;
    for(int j = 0; j <= len; j++)
    {
        if(dechipher_text[j] == 'e' || dechipher_text[j] == 'E')
        {
            kont_e ++;
        }
        
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