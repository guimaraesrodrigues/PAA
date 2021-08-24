/**
 * Author: https://github.com/AkdenizKutayOcal
 * Repo: https://github.com/AkdenizKutayOcal/Convex-Hull-Calculation-with-Gift-Wrapping-Algorithm
 **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structs.h"

enum orientation {                           //This was a little bit unnecessary and I could do the same thing with int values while checking Clockwise condition
    counterWise, clockWise,coolinear        //However I wanted to learn how to implement and use typedef in such cases.
};
typedef enum orientation orientation;

Ponto leftpoint(Ponto points[], int size);
orientation isCounter(Ponto A,Ponto B,Ponto C);
int wrap(Ponto points[],Ponto *wrapped,int size,Ponto left);
int findIndex(Ponto points[], int size,Ponto p);
void gravaFecho(int n_pontos, Ponto vertices[n_pontos]);

int convexHull(int n_pontos, Ponto pontos[], Ponto *hull) {

    //Left Most point
    Ponto left;

    left = leftpoint(pontos, n_pontos);
    
    int hull_size = wrap(pontos, hull, n_pontos, left);

    gravaFecho(hull_size, hull);

    return hull_size;
}

/////////////////////////////////////////////////////////////////
//This function finds the left most point of the given point list
////////////////////////////////////////////////////////////////
Ponto leftpoint(Ponto points[], int size) {

    Ponto left = {points[0].x, points[0].y};
    float min = points[0].x;                       //It searches for the smallest x value and sets it to min


    for(int i = 1;i < size; i++){

        if(points[i].x < min) {
           min = points[i].x;
           left.x = min;
           left.y = points[i].y;                    //At the end it returns left Ponto which has the smallest x value
        }
    }

    return left;
}

////////////////////////////////////////////////////////////
//isCounter Function returns whether given 3 points
//are CounterClock wise,Clock wise or coolinear.
///////////////////////////////////////////////////////////
orientation isCounter(Ponto A,Ponto B,Ponto C){

    orientation result;

    int a = ((B.y-A.y)*(C.x-B.x))- ((B.x-A.x)*(C.y-B.y));    //Calculates the orientation

    if(a<0){
        result = counterWise;
    }                                                           //And assigns the value according to that value
    else if((int)a==0){
        result = coolinear;
    }
    else if(a>0){
        result = clockWise;
    }
    return result;
}

/////////////////////////////////////////////////////////////
//wrap Function finds ConvexHull. Takes points array,
// wrapped array that will return final points, size,
//and Left point as parameters.If there are more than 3 points
//It finds convexHull and returns the Pontos.
/////////////////////////////////////////////////////////////
int wrap(Ponto points[],Ponto *wrapped,int size, Ponto left){

    if(size > 2){

        int index = findIndex(points, size, left);        //Finds the index of Leftmost point
        int p = index, q;                                //p is the first and q is the second point
        int k = 0;                                      //k is the int value which will be increased while we are adding new points
        do {

            //First it adds left most to the array, that in every loop it assigns new p point
            wrapped[k] = points[p];
            
            //Assigning q value while it is not more than size
            q = (p+1) % size;

            for(int i = 0; i < size-1; i++){

                //This loop finds a point which is clockwise to all other points
                if(isCounter(points[p],points[i],points[q]) == counterWise) {
                    //And that assigns that point to q
                    q = i;                                                      
                }
            }

            printf("vsf %f, %f \n", wrapped[k].x, wrapped[k].y);

           k++;
           //At the end of every loop we set p as q so that we can add q to the list and continue finding other points
           p = q;
        } while(p != index);//This loop continues until it reaches start point

        wrapped[k] = wrapped[0];
        return k + 1; //returns the size of wrapped list
    }
    else{
        printf("There must be at least 3 points");
        return 0;
    }

}

///////////////////////////////////////////////////////////////////
//This function finds the index of given point in given point array
//Where I used it at wrap function
///////////////////////////////////////////////////////////////////
int findIndex(Ponto points[], int size, Ponto p){

    int index = 0;
    for(int i=0;i<size;i++){

        if(p.x == points[i].x && p.y==points[i].y){
            index = i;
            break;
        }
        else{
            continue;
        }
    }
    return index;
}


/**
 * Grava ciclo no arquivo cycle.txt
 * 
 * */
void gravaFecho(int n_pontos, Ponto vertices[n_pontos]) {
    FILE *fp = fopen("fecho.txt", "w");
	if (fp == NULL)
	{
		fprintf(stderr, "Falha ao criar fecho.txt.\n");
		return;
	}
    int i = n_pontos - 1;
    
    while (i >= 0)
	{
        fprintf(fp, "%f %f\n", vertices[i].x, vertices[i].y);
        i--;
	}
    fclose(fp);
}

// Cria uma lista pontos P baseado no arquivo passado como parâmetro
void createPointsList(FILE *file, Ponto* pontos) {
    Ponto p;

	int ret = 0;
	int n_lines = 0;
	char line_buffer[100]; //buffer a ser utilizado na leitura de cada linha

	// Percorre a stream, identifica as duas coordenadas na linha
	// e insere o valor no array pontos
	while(fgets(line_buffer, sizeof(line_buffer), file)) {
		ret = sscanf(line_buffer, "%f %f", &p.x, &p.y);
		if(ret < 1)
			continue;
		pontos[n_lines].x = p.x;
		pontos[n_lines].y = p.y;
		n_lines++;
	}
}
