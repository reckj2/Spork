/**************************************************************************************************/

/*
* File: sporkprofile.c
* Author: Joshua Reck
* NetID: reckj2
* Date:
*
* Description: Function definitions for reading, processing, and writing Spork profiles.
*
*/

/**************************************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#define MAX_SPORK_PROFILES 5000
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "sporkprofile.h"

/**************************************************************************************************/

/* Reads up to maxProfiles Spork profiles from an input file specified by fileName. Functions reads
* the input file line-by-line using the format:
*
*    BusinessName X.XX Y.YY R.RR A
*
* BusinessName is the name of the restauarant/business. The buseiness name will not include any
* whitespace characters
*
* X.XX represents the X location in miles using a Cartesian coodinate system
*
* Y.YY represents the Y location in miles using a Cartesian coodinate system
*
* R.RR represents the average rating for the business
*
* A is the advertising level, which should 0, 1, or 2
*
* Alpha submission: ReadSporkDataFromFile() function should initialize isNearby and isGood
*                   to true and initialize distMiles to 0.0 for all profile entries.
*                   Note: This requirement is different for the final project submission.
*
* Project submission: ReadSporkDataFromFile() function should initialize isNearby and isGood
*                     to false for all profile entries.
*                     Note: This requirement is different for the alpha project submission.
*
* Returns: The number of Spork profiles read from the file. If the input file could not be opened,
*          the function returns -1.
*
*/

int ReadSporkDataFromFile(SporkProfile sporkProfiles[], int maxProfiles, char *fileName) {
	FILE *inp;
	inp = fopen(fileName, "r");
	char BName[500];
	char temp[500];
	char temp2;
	double x, y, rate;
	int ad, i;

	strcpy(BName, "empty");
	if (inp == NULL) {

		printf("This file, %s,  doesn't exist. Try again.\n", fileName);

		return -1;

	}

	i = 0;
	//fscanf(inp, "%s %lf %lf %lf %d", BName , &x, &y, &rate, &ad) != 
	while (!feof(inp)) {
		temp2 = '\n';
		fgets(temp, MAX_SPORK_PROFILES, inp);
		sscanf(temp, "%s %lf %lf %lf %d %c", BName, &x, &y, &rate, &ad, &temp2);
		if (strcmp(BName, "empty") == 0){
			break;
		}
		if (temp2 == '\n') {
			strcpy(sporkProfiles[i].businessName, BName);
			sporkProfiles[i].locX = x;
			sporkProfiles[i].locY = y;
			sporkProfiles[i].avgRating = rate;
			sporkProfiles[i].adLevel = ad;
			sporkProfiles[i].isNearby = true;
			sporkProfiles[i].isGood = true;
			sporkProfiles[i].distMiles = 0.0;
			i++;
		}
		

	}
	
	return i;
}

/**************************************************************************************************/

/*
* Determines if each business is nearby the user,  sets the Spork profile's isNearby flag to
* true (is nearby) or false (not nearby), and stores the distance in miles in the Spork profile.
*
* userLocX, userLocY: Indicates the x and y coordiante of the user's location
* maxDist: Indicates the maxmimum distance between the user and a nearby business
*
*/
void FindNearbyBusinesses(SporkProfile sporkProfiles[], int numProfiles, double userLocX, double userLocY, double maxDist) {
	double ActDist = 0;
	int i = 0;

	for (i = 0; i < numProfiles; i++) {
		ActDist = sqrt((userLocX - sporkProfiles[i].locX)*(userLocX - sporkProfiles[i].locX) + (userLocY - sporkProfiles[i].locY)*(userLocY - sporkProfiles[i].locY));
		sporkProfiles[i].distMiles = ActDist;
		if (ActDist <= maxDist) {
			sporkProfiles[i].isNearby = true;
		}
		else
		{
			sporkProfiles[i].isNearby = false;
		}
	}
}


/**************************************************************************************************/

/*
* Determines if each business is good based on the user's minimum required average rating.
* Sets the Spork profile's isGood flag to true if the business' avgRating is greater than or
* equal to minRating, and false otherwise.
*
*/
void FindGoodBusinesses(SporkProfile sporkProfiles[], int numProfiles, double minRating) {
	for (int i = 0; i < numProfiles; i++) {
		if (sporkProfiles[i].avgRating >= minRating) {
			sporkProfiles[i].isGood = true;
		}
		else {
			sporkProfiles[i].isGood = false;
		}
	}
}

/**************************************************************************************************/

/*
* Returns the index of the Spork profile that is neary, good, and has the highest adLevel. If
* there is a tie, the index of the first entry found with the highest ad level should be returned.
* If no businesses are nearby and good, the function will return -1.
*
*/

int GetIndexMaxSponsor(SporkProfile sporkProfiles[], int numProfiles) {
	int i = 0;
	int temp = 0;
	int tempadlevel = 0;
	for (i = 0; i < numProfiles; i++) {
		if (sporkProfiles[i].isGood && sporkProfiles[i].isNearby) {
			if (sporkProfiles[i].adLevel > 0 && sporkProfiles[i].adLevel <= 2 && sporkProfiles[i].adLevel > tempadlevel) {
				tempadlevel = sporkProfiles[i].adLevel;
				temp = i;
			}
		}
	}
	if (temp == 0) {
		return -1;
	}
	else {
		return temp;
	}
}

/**************************************************************************************************/

/*
* Writes all good and nearby business to an output file specified by fileName using the format:
*
*    BusinessName R.RR D.DD
*
* R.RR is the average rating with exactly two decimal digits of precision.
* D.DD is the distance in miles with exactly two decimal digits of precision.
*
* If maxSponsorIndex is not -1, the cooresponding profile entry will be output first. All other
* nearby and good profiles will be output in the order they are stored in the sporkProfiles array.
*
* Each entry should be separated by a single tab character (\t), and each line should end
* with a single newline (\n).
*
* Returns: -1 if the output file could not be opened, and 0 otherwise.
*
*/
int WriteSporkResultsToFile(SporkProfile sporkProfiles[], int numProfiles, int maxSponsorIndex, char *fileName) {
	FILE *outp;
	outp = fopen(fileName, "w");
	int i = 0;
	if (numProfiles == 0) {
		return 0;
	}
	if (maxSponsorIndex != -1) {
		fprintf(outp, "%s\t%.2lf\t%.2lf\n", sporkProfiles[maxSponsorIndex].businessName, sporkProfiles[maxSponsorIndex].avgRating, sporkProfiles[maxSponsorIndex].distMiles);
	}
	while (i < numProfiles) {
		
		/*while (sporkProfiles[i].adLevel < 0 || sporkProfiles[i].adLevel>2) {
			i++;
		}
		while (!sporkProfiles[i].isNearby) {
			i++;
		}

		while (!sporkProfiles[i].isGood) {
			i++;
		} */
		if ((sporkProfiles[i].isNearby) && (sporkProfiles[i].isGood) && (sporkProfiles[i].adLevel>=0) && (sporkProfiles[i].adLevel<=2)){
			if (i != maxSponsorIndex) {
				fprintf(outp, "%s\t%.2lf\t%.2lf\n", sporkProfiles[i].businessName, sporkProfiles[i].avgRating, sporkProfiles[i].distMiles);
			}
		}
		i++;

	}
	
	

	return 0;

}
/**************************************************************************************************/
