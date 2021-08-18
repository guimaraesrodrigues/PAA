#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct {            //Point struct with x and y coordinates
    int x,y;
}Point;

enum orientation{                           //This was a little bit unnecessary and I could do the same thing with int values while checking Clockwise condition
    counterWise, clockWise,coolinear        //However I wanted to learn how to implement and use typedef in such cases.
};
typedef enum orientation orientation;

void printArray(Point points[], int size);          //Explanations are at below.
Point leftpoint(Point points[], int size);
orientation isCounter(Point A,Point B,Point C);
int wrap(Point points[],Point *wrapped,int size,Point left);
int findIndex(Point points[], int size,Point p);
void gravaFecho(int n_pontos, Point vertices[n_pontos]);
void createPointsList(FILE *file, Point* pontos);


int main(int argc, char *argv[])
{
    FILE * file = NULL;

    /* Checa argumentos da linha de comando */
    if (argc != 2) {
        printf ("ERROR: you must specify file name!\n");
        return 1;
    }

    file = fopen(argv[1], "r");                  //Opens file to read

    if(!file){
        printf("Could not read the file %s",argv[1]);   //Checks to read if not error message occurs
        exit(0);
    }

    /* Definicao de variaveis*/
    char first_line[20];
    int n_pontos;

    // Le a primeira linha do arquivo para saber quantos pares foram gerados
    fscanf(file, "%s", first_line); 

    //converte string para int
    sscanf(first_line, "%d", &n_pontos);

    // Defini um lista de pontos
    Point pontos[n_pontos];

    // Cria lista de pares com base no arquivo input.txt
	createPointsList(file, pontos);

    Point left;                                                 //Left Most point

    Point *wrapped = (Point*)malloc(n_pontos*sizeof(Point));       //Is used to store Points that will be returned

    left = leftpoint(pontos, n_pontos);

    int hull_size = wrap(pontos, wrapped, n_pontos, left);

    gravaFecho(hull_size, wrapped);

    return 0;
}
////////////////////////////////////////////////
//This function basically prints the Point array
////////////////////////////////////////////////
void printArray(Point points[], int size){

    for(int i=0;i<size;i++){

        printf("%d %d\n",points[i].x,points[i].y);

    }
}
/////////////////////////////////////////////////////////////////
//This function finds the left most point of the given point list
////////////////////////////////////////////////////////////////
Point leftpoint(Point points[],int size){

    Point left = {points[0].x,points[0].y};
    int min = points[0].x;                       //It searches for the smallest x value and sets it to min

    for(int i=1;i<size;i++){

        if(points[i].x<min){
           min = points[i].x;
           left.x = min;
           left.y = points[i].y;                    //At the end it returns left Point which has the smallest x value
        }
        else{
            continue;
        }
    }
    return left;
}

////////////////////////////////////////////////////////////
//isCounter Function returns whether given 3 points
//are CounterClock wise,Clock wise or coolinear.
///////////////////////////////////////////////////////////
orientation isCounter(Point A,Point B,Point C){

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
//It finds convexHull and returns the Points.
/////////////////////////////////////////////////////////////
int wrap(Point points[],Point *wrapped,int size,Point left){

    if(size>2){

        int index = findIndex(points,size,left);        //Finds the index of Leftmost point
        int p = index, q;                                //p is the first and q is the second point
        int k = 0;                                      //k is the int value which will be increased while we are adding new points
        do{

            wrapped[k] = points[p] ;                    //First it adds left most to the array, that in every loop it assigns new p point
            
            q = (p+1)%size;                             //Assigning q value while it is not more than size

            for(int i=0;i<size-1;i++){

                if(isCounter(points[p],points[i],points[q])==counterWise){      //This loop finds a point which is clockwise to all other points

                    q = i;                                                      //And that assigns that point to q
                }
            }

           k++;
           p = q;                                         //At the end of every loop we set p as q so that we can add q to the list and continue finding other points
        }while(p != index);     
        return k;                          //This loop continues until it reaches start point
    }
    else{
        printf("There must be at least 3 points");
        return -1;
    }

}

///////////////////////////////////////////////////////////////////
//This function finds the index of given point in given point array
//Where I used it at wrap function
///////////////////////////////////////////////////////////////////
int findIndex(Point points[], int size,Point p){

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
void gravaFecho(int n_pontos, Point vertices[n_pontos]) {
    FILE *fp = fopen("fecho.txt", "w");
	if (fp == NULL)
	{
		fprintf(stderr, "Falha ao criar fecho.txt.\n");
		return;
	}
    int n = 0;
    while (n < n_pontos)
	{
        fprintf(fp, "%d %d\n", vertices[n].x, vertices[n].y);
        n++;
	}
    fclose(fp);
}

// Cria uma lista pontos P baseado no arquivo passado como parâmetro
void createPointsList(FILE *file, Point* pontos) {
    Point p;

	int ret = 0;
	int n_lines = 0;
	char line_buffer[100]; //buffer a ser utilizado na leitura de cada linha

	// Percorre a stream, identifica as duas coordenadas na linha
	// e insere o valor no array pontos
	while(fgets(line_buffer, sizeof(line_buffer), file)) {
		ret = sscanf(line_buffer, "%d %d", &p.x, &p.y);
		if(ret < 1)
			continue;
		pontos[n_lines].x = p.x;
		pontos[n_lines].y = p.y;
		n_lines++;
	}
}
