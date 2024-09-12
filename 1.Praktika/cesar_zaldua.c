#include <stdio.h>
#include <string.h>

void bihurtu(unsigned char mezua[], int pad, int len){

    unsigned char deszifratua[1000];

    for (int i = 0; i < len-1; i++) deszifratua[i] = mezua[i] + pad;

    printf("\n");

    for (int i = 0; i < len; i++)
    {
        printf("%c", deszifratua[i]);
    }
    printf(".");
    printf("\n");
}

void maiztAurkitu(unsigned char mezua[], int len){
    
    int maizt[26];
    int pad;

    for (int i = 0; i < 26; i++) maizt[i] = 0;

    for (int i = 0; i < len-1; i++){
        int letra = mezua[i];
        if (letra < 91 && letra > 64){
            maizt[letra-65]++;
        } else if(letra < 123 && letra > 96){
            maizt[letra-97]++;
        }    
    }

    int erabiliena1 = 0;
    int erabiliena2 = 0;
    int erabiliena3 = 0;

    for (int i = 0; i < 26; i++){
        if (maizt[i] > erabiliena1) erabiliena1 = i;
        else if(maizt[i] > erabiliena2) erabiliena2 = i;
        else if(maizt[i] > erabiliena3) erabiliena3 = i;
    }

    for (int i = 0; i < 26; i++){
        printf("\n");
        printf("\n%c erabilita\n", i+97);
        pad = (i+97) - (erabiliena1+97);
        bihurtu(mezua, pad, len);
        printf("\n");
    }      
}

int main(){

    unsigned char mezua[]="Jypwavnyhwof pz aol kpzjpwspul aoha ltivkplz aol wypujpwslz, tlhuz, huk tlaovkz mvy aol ayhuzmvythapvu vm khah pu vykly av opkl aolpy zlthuapj jvualua, wylclua aolpy buhbaovypglk bzl, vy wylclua aolpy buklaljalk tvkpmpjhapvu.";
    
    int len = strlen(mezua);

    maiztAurkitu(mezua, len);

    return 0;
}