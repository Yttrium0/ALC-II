#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265
#define PI_180  0.017453292
#define A180_PI 57.29577951

double eqT_Decl(double fA, int eqt_decl);
double fracAnnee(int a, int m, int j, int h, int mn, int s);
int jourAnnee(int a, int m, int j);
int conv_Mn_HMS(double mn, int hr_min);

int lmvSoleil(int a, int m, int j, int tZ, int tcr, double la, double lon, int alt, int lSo_cSo, int hr_min) {
	// "a" est l'année; "m" est le mois; "j" est le jour
  // "tz" est la zone horaire 
	// "tcr" est le type de cr�puscule retenu :
	//   -  0 : cr�puscule sur l'horizon avec correction de la r�fraction athmosph�rique  : r = 0.833� |!!!!! A CHOISIR !!!!!|
	//   -  6 : cr�puscule civil 
	//   -  12 : cr�puscule nautique
	//   -  18 : cr�puscule astronomique
	// "lSo" est l'heure de lev�e du soleil, "mSo" est celle du z�nit et "cSo" celle du coucher. Ces valeurs doivent �tre fournies par adresse
	// "la" est la latitude et "lon" est la longitude
  // alt est l'altitude du lieu en m�tres
  // retour est l'information retournée :
  //   - 0 : horaire levé soleil
  //   - 1 : horaire couché soleil
  int lSoH;
  int lSoM;
  int cSoH;
  int cSoM;
  double eqT = eqT_Decl(fracAnnee(a, m, j, 12, 0, 0), 0);
  double decl = eqT_Decl(fracAnnee(a, m, j, 12, 0, 0), 1);
  la = la * PI_180; // radians
  double acr; // cosinus de l'angle cr�pusculaire
  switch (tcr) {
    case 0: // cr�puscule sur l'horizon
      acr = cos(PI_180 * (90.833 + 0.015 * sqrt(alt)));
      break;
    case 6: // cr�puscule civil
      acr = cos(PI_180 * 96);
      break;
    case 12 : // cr�puscule nautique
      acr = cos(PI_180 * 102);
      break;
    case 18 : // cr�puscule astronomique
      acr = cos(PI_180 * 108);
      break;
  }
  double aS = acos(acr / (cos(la) * cos(decl)) - tan(la) * tan(decl)) * A180_PI;
  lSoH = conv_Mn_HMS(720 + 4 * (-lon - aS) - eqT + 60.0 * tZ, 0);
  lSoM = conv_Mn_HMS(720 + 4 * (-lon - aS) - eqT + 60.0 * tZ, 1);
  cSoH = conv_Mn_HMS(720 + 4 * (-lon + aS) - eqT + 60.0 * tZ, 0);
  cSoM = conv_Mn_HMS(720 + 4 * (-lon + aS) - eqT + 60.0 * tZ, 1);

  switch (lSo_cSo) {
      case 0:
        if(hr_min == 0)
          return lSoH;
        else if(hr_min == 1)
          return lSoM;
        break;
      case 1:
        if(hr_min == 0)
          return cSoH;
        else if(hr_min == 1)
          return cSoM;
        break;
  }
}

double eqT_Decl(double fA, int eqt_decl) { // retourne l'�quation du temps en minutes et l'angle de d�clinaison du soleil en radians
  double eqT; // minutes
  double decl; // radians
  eqT = 229.18 * (0.000075 + 0.001868 * cos(fA) - 0.032077 * sin(fA) - 0.014615 * cos(2 * fA) - 0.040849 * sin(2 * fA)); // minutes
  decl = 0.006918 - 0.399912 * cos(fA) + 0.070257 * sin(fA) - 0.006758 * cos(2 * fA) + 0.000907 * sin(2 * fA) - 0.002697 * cos(3 * fA) + 0.00148 * sin(3 * fA); // radians
  switch (eqt_decl)
  {
    case 0:
        return eqT;
        break;
    case 1:
        return decl;
        break; 
  }
}

double fracAnnee(int a, int m, int j, int h, int mn, int s) { // fraction de l'ann�e en radians le jour de l'ann�e "a", au mois "m", au jour "j", � l'heure "h", la minute "mn" et la seconde "s"
  int n = 365;
  if (a % 4 == 0) // Cas de l'ann�e bissextile
    n = 366;
  return 2 * PI * (jourAnnee(a, m, j) - 1 + (h + mn / 60 + s / 3600 - 12) / 24) / n; // radians
}

int jourAnnee(int a, int m, int j) { // retourne le jour de l'ann�e "a" au mois "m" et au jour "j"
  int nbJmois[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int nbJ = 0;
  for (int i = 0; i < m; i++)
    nbJ += nbJmois[i];
  if (a % 4 == 0 && m > 2) // Cas d'une ann�e bissextile
    nbJ++;
  return nbJ + j;
}

int conv_Mn_HMS(double mn, int hr_min) { // retourne l'heure, les minutes et les secondes sous forme d'une cha�ne au format : HH:MM:
  float hr = mn / 60;
  float min = (hr - (int)hr) *60;
  
  switch (hr_min)
  {
    case 0:
      return (int)hr;
      break;
    case 1:
      return (int)min;
      break;
  }
}