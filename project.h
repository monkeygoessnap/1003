/*******************************************************************************************************
*CSC1003
*GRP P1-1 Final Project
*Naive Bayers
*Headerfile
********************************************************************************************************/
#ifndef PROJECT_H
#define PROJECT_H

/*******************************************************************************************************
*Header Files
********************************************************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*******************************************************************************************************
*Preprocessor Constants
********************************************************************************************************/
#define MAXDATA 100 //cater for increases in data
#define MAX_ATTRIBUTES 5
#define FEATURESIZE 9
#define PI 3.142
#define ALPHA 1

/*******************************************************************************************************
*Structure definitions
********************************************************************************************************/
/*
Struct setup used to pass in common variables into functions
to prevent the use of global variables
*/
typedef struct {
    int datacount; //total data count
    int training; //total training count
    int testing; //total testing count
    int altered; //total altered training count
    int normal; //total normal training count
    double input[MAXDATA][FEATURESIZE+1]; //data of input
    FILE *plot_train; //filepointer for training.txt for output
    FILE *plot_test; //filepointer for testing.txt for output
}setup;

/*
Struct data is used to record altered and normal data for both 
non-gaussian and gaussian features
Non-gaussian:
Records altered and normal counts
Gaussian:
Records altered and normal mean and variance in array [0] and [1] respectively
*/
typedef struct {
    int data_rep_size; //number of attributes
    double data_rep[MAX_ATTRIBUTES]; //values of attributes
    double altered[MAX_ATTRIBUTES]; //values of altered data
    double normal[MAX_ATTRIBUTES]; //values of normal data
}data;

/*******************************************************************************************************
*Function prototypes
********************************************************************************************************/
void init_data(setup *params);
void set_training(setup *params, int percent);
void train(setup *params, data features[FEATURESIZE]);
void test(setup *params, data features[FEATURESIZE], int start, int end, int mode, int choice);
void predict(setup *params, int choice);
void evaluate(setup *params, int choice);
double gaussian_condp(double x, double xmean, double xvar);
double get_postp(double condp[FEATURESIZE], double num, double total);

/*******************************************************************************************************
*Function prototypes for testing
********************************************************************************************************/
void run_tests(setup *params);

#endif