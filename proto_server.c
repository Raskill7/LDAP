/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "proto.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


ls_res *
ls_1_svc(type_nom *argp, struct svc_req *rqstp)
{
	printf("-----------------Debut du programme ls---------------\n\n");
	static ls_res  result;
	

	DIR *directory;
	struct dirent *infosReadDir;
	char* dossier = *argp;

	ls_res resultatTmp;
	cell_nom *celluleCourante;
	cell_nom *celluleSuivante;
	resultatTmp.erreur=1;

	//Ouverture du fichier
	// directory = opendir((char *)argp);
	directory = opendir(dossier);
	if(directory==NULL)
	{
		perror("Erreur d'ouverture du dossier ");
		printf("Erreur d'ouverture du dossier : %s \n",dossier);
		resultatTmp.erreur=2;
	}
	
	//Si le fichier est ouvert sans erreur
	else
	{
		//initialisation de cellule courante
		celluleCourante=malloc(10*sizeof(char));
		//initialisation de la cellule suivante
		celluleSuivante=celluleCourante;

		//initialisation de resultatTmp
		resultatTmp.ls_res_u.liste=celluleCourante;
	

		while ((infosReadDir = readdir(directory)) != NULL)
		{	
			celluleCourante->suivant=celluleSuivante;
			celluleCourante=celluleSuivante;

			//On prepare la cellule a recevoir les valeurs
			celluleCourante->nom=calloc(MAXNOM,sizeof(char));
			celluleCourante->suivant=NULL;		

			//On ecrit les valeurs
			strcpy(celluleCourante->nom, infosReadDir->d_name);

			//initialisation de la cellule suivante
			celluleSuivante=malloc(10*sizeof(char));

		}
		closedir(directory);	
		resultatTmp.erreur=0;
	}

	result = resultatTmp;
	printf("Erreur retournée (0 = OK) : %d\n", result.erreur);
	printf("\n------------------Fin du programme ls------------------\n\n");
	return &result;	
}

read_res *
read_1_svc(type_nom *argp, struct svc_req *rqstp)
{
	printf("----------------Debut du programme read----------------\n\n");
	static read_res  result;
	
	DIR *directory;
	FILE *file;
	struct dirent *infosReadDir;
	char* fichier = *argp;

	read_res resultatTmp;
	cell_bloc *celluleCourante;
	cell_bloc *celluleSuivante;
	resultatTmp.erreur=1;
	char buffer[MAXBLOC];

	directory = opendir(fichier);
	if(directory!=NULL){
		closedir(directory);
		resultatTmp.erreur=2;
	}
	else
	{
		//Ouverture du fichier
		file = fopen(fichier,"r");
		if(file==NULL)
		{
			perror("Erreur d'ouverture du fichier ");
			resultatTmp.erreur=3;
		}
		//Si le fichier est ouvert sans erreur
		else
		{
			//initialisation de cellule courante
			celluleCourante=calloc(MAXBLOC,sizeof(char));
			celluleCourante->bloc=calloc(MAXBLOC,sizeof(char));
			celluleCourante->suivant=NULL;

			//initialisation de la cellule suivante
			celluleSuivante=celluleCourante;

			//initialisation de resultatTmp
			resultatTmp.read_res_u.fichier=celluleCourante;

			while (!feof(file))
			{
				celluleCourante->suivant=celluleSuivante;

				celluleCourante=celluleSuivante;
				celluleCourante->bloc= calloc(MAXBLOC,sizeof(char));
				celluleCourante->suivant=NULL;

				fgets(buffer, MAXBLOC, file);
				strcpy(celluleCourante->bloc, buffer);

				celluleSuivante=calloc(MAXBLOC,sizeof(char));
			}
			fclose(file);	
			resultatTmp.erreur=0;
		}
	}
	result = resultatTmp;
	printf("Erreur retournée (0 = OK) : %d\n", result.erreur);
	printf("\n-----------------Fin du programme read-----------------\n\n");
	return &result;
}

int *
write_1_svc(write_parm *argp, struct svc_req *rqstp)
{
	printf("----------------Debut du programme write---------------\n\n");
	static int  result;
	int erreur=1;
	FILE *file;
	char* fichier=argp->nom;
	cell_bloc *celluleCourante = argp->donnees;

	if(argp->ecraser)
	{
		file = fopen(fichier,"w");
	}
	else
	{
		file = fopen(fichier,"a");
	}
	
	if(file==NULL)
	{
		perror("Erreur d'ouverture du fichier ");
		printf("Erreur d'ouverture du fichier : %s \n",fichier);
		erreur=2;
	}
	else
	{
		while(celluleCourante!=NULL)
		{	
			fputs(celluleCourante->bloc,file);
			printf("%s\n", celluleCourante->bloc);
			celluleCourante=celluleCourante->suivant;
		}
		erreur=0;
	}
	fputs("\n",file);
	fclose(file);
	result=erreur;
	printf("Erreur retournée (0 = OK) : %d\n", result);
	printf("\n----------------Fin du programme write-----------------\n\n");
	return &result;
}

