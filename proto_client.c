/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "proto.h"

liste_bloc lecturefichier();

void
proto_gestion_fichier_1(char *host, char *type_commande, char *fichier_distant, bool_t ecraser, char *fichier_source)
{
	CLIENT *clnt;
	ls_res  *result_1;
	type_nom  ls_1_arg=fichier_distant;
	read_res  *result_2;
	type_nom  read_1_arg=fichier_distant;
	int  *result_3;
	write_parm  write_1_arg;

	#ifndef	DEBUG
		clnt = clnt_create (host, PROTO_GESTION_FICHIER, PROTO_GESTION_FICHIER_V1, "udp");
		if (clnt == NULL) {
			clnt_pcreateerror (host);
			exit (1);
		}
	#endif	/* DEBUG */

	if(strncmp(type_commande,"ls",2)==0)
	{
		printf("-----------------Debut du programme ls-----------------\n\n");
		result_1 = ls_1(&ls_1_arg, clnt);
		if (result_1 == (ls_res *) NULL) 
		{
			clnt_perror (clnt, "call failed");
		}

		int erreur = result_1->erreur;
		if (erreur == 0 )
		{
			printf("Contenu du repertoire \"%s\".\n\n", fichier_distant);
			cell_nom* celluleResultat = result_1->ls_res_u.liste;
			while(celluleResultat!=NULL)
			{
				printf("%s\n", celluleResultat->nom);
				celluleResultat = celluleResultat->suivant;
			}
			printf("\nLe programme s'est terminé sans erreur.\n", erreur);
		}
		else
		{
			printf("Erreur %d lors de l'execution sur la machine distante, vérifiez les paramètres\n", erreur);
		}
		printf("\n------------------Fin du programme ls------------------\n\n\n");
	}
	else 
	{
		if(strncmp(type_commande,"read",4)==0)
		{
			printf("----------------Debut du programme read----------------\n\n");
			result_2 = read_1(&read_1_arg, clnt);
			if (result_2 == (read_res *) NULL) 
			{
				clnt_perror (clnt, "call failed");
			}

			int erreur = result_2->erreur;
			if (erreur == 0 )
			{
				printf("Contenu du fichier \"%s\".\n\n", fichier_distant);
				cell_bloc* celluleResultat = result_2->read_res_u.fichier;
				while(celluleResultat!=NULL)
				{
					printf("%s", celluleResultat->bloc);
					celluleResultat = celluleResultat->suivant;
				}
				printf("\nLe programme s'est terminé sans erreur.\n", erreur);
			}
			else
			{
				printf("Erreur %d lors de l'execution, vérifiez les paramètres\n", erreur);
			}
			printf("\n-----------------Fin du programme read-----------------\n\n\n");
		}
		else
		{
			if(strncmp(type_commande,"write",5)==0)
			{
				printf("----------------Debut du programme write---------------\n\n");
				write_1_arg.nom=fichier_distant;
				write_1_arg.ecraser=ecraser;
				write_1_arg.donnees=lecturefichier(fichier_source);
				result_3 = write_1(&write_1_arg, clnt);
				if (result_3 ==  NULL) 
				{
					clnt_perror (clnt, "call failed");
				}

				int erreur = *result_3;
				if(erreur==0)
				{
					printf("Contenu du fichier local \"%s\" ecrit sur le fichier distant \"%s\".\n\n", fichier_source,fichier_distant);
					printf("Le programme s'est terminé sans erreur.\n", erreur);
				}
				else
				{
					printf("Erreur %d lors de l'execution, vérifiez les paramètres\n", erreur);
				}
				printf("\n----------------Fin du programme write-----------------\n\n\n");
			}
			else
			{
				perror("Commande inconnue");
			}
		}
	}
	#ifndef	DEBUG
		clnt_destroy (clnt);
	#endif	 /* DEBUG */	
}


int
main (int argc, char *argv[])
{
	char *host;
	char *type_commande;
	char *fichier_distant;
	bool_t ecraser;
	char *fichier_source;

	if (argc == 4)
	{
		host = argv[1];
		type_commande=argv[2];
		fichier_distant=argv[3];
		proto_gestion_fichier_1 (host, type_commande, fichier_distant, 0, "");
	}
	else
	{
		if(argc == 6)
		{
			host = argv[1];
			type_commande=argv[2];
			fichier_source=argv[3];	
			ecraser= atoi(argv[4]);
			fichier_distant=argv[5];
			proto_gestion_fichier_1 (host, type_commande, fichier_distant, ecraser, fichier_source);
		}	
		else
		{	
			printf ("!!! Il manque des arguments !!!\nUsage 1 : IP read fichier_distant \nUsage 2 : IP ls repertoire_distant\nUsage 3 : IP write fichier_source ecraser fichier_distant,\necraser : \n0  pour NON \n1 pour OUI\n\n");
			exit (1);
		}
	}
	exit (0);
}

liste_bloc lecturefichier(char *fichier)
{
	FILE *file;
	struct dirent *infosReadDir;

	liste_bloc resultatTmp;
	cell_bloc *celluleCourante;
	cell_bloc *celluleSuivante;
	char buffer[MAXBLOC];

	//Ouverture du fichier
	file = fopen(fichier,"r");
	if(file==NULL)
	{
		perror("Erreur d'ouverture du fichier ");
		printf("Erreur d'ouverture du fichier : %s \n",fichier);
		exit(1);
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
		resultatTmp=celluleCourante;

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
	}

	return resultatTmp;
}