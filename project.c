/*******************************************************************************************************
*CSC1003
*GRP P1-1 Final Project
*Naive Bayers
*Main program file
********************************************************************************************************/

/*******************************************************************************************************
*Header file
********************************************************************************************************/
#include "project.h"

/*******************************************************************************************************
*Main function
********************************************************************************************************/
int main(int argc, char **argv){

    //initialise params, which are key program variables
    setup params = {0,0,0,0,0,{0},NULL,NULL};
    init_data(&params);
    
    //run tests, if first argument is 't'
    if (argc > 1) {
        if (argv[1][0] == 't') {
            run_tests(&params);
            exit(0);
        }
    }

    //get user inputs
    int choice = 0;

    //only accepts choice 0 or 1
    do {
        printf("\nSelect mode...\n0: Predict outcome\n1: Evaluate program\n");
        scanf("%d", &choice);
    } while (choice < 0 || choice > 1);

    //program runs based off userchoice
    if (choice == 0) {
        predict(&params, choice);
    } else {
        evaluate(&params, choice);
    }

    //free file pointers
    fclose(params.plot_train);
    fclose(params.plot_test);

    //plots on gnuplot, script is in gnuscript.txt
    if (choice == 1) {
        system("gnuplot -persistent gnuscript.txt");
    }

    //exits successfully
    return 0;
}

/*******************************************************************************************************
*Function predict is for userchoice == 0, prediction mode
********************************************************************************************************/
void predict(setup *params, int choice) {

    //init features data
    data features[FEATURESIZE] = {
            {4, {-1, -0.33, 0.33, 1}, {0}, {0}},
            {2, {0}, {0}, {0}},
            {2, {0, 1}, {0}, {0}},
            {2, {0, 1}, {0}, {0}},
            {2, {0, 1}, {0}, {0}},
            {3, {-1, 0, 1}, {0}, {0}},
            {5, {0.2, 0.4, 0.6, 0.8, 1}, {0}, {0}},
            {3, {-1, 0, 1}, {0}, {0}},
            {2, {0}, {0}, {0}},
        };
    //set training to full datacount
    set_training(params, params->datacount);
    //train data
    train(params, features);
    //get user input and store in array 0 of input
    for (int i=0;i<FEATURESIZE;i++) {
        printf("\nEnter Feature %d data: ", i+1);
        scanf("%lf", &params->input[0][i]);
    }
    //test data from array 0
    test(params, features, 0, 1, 1, choice);
}

/*******************************************************************************************************
*Function evaluate if for userchoice == 1, evaluate program
********************************************************************************************************/
void evaluate(setup *params, int choice) {
        //start clock to time program
        double time_elapsed = 0.0;
        clock_t start = clock();
        //loop from 50 to 90, for training and testing
        for (int i = 50; i < 100; i+=10) {
            //init features array
            data features[FEATURESIZE] = {
                {4, {-1, -0.33, 0.33, 1}, {0}, {0}},
                {2, {0}, {0}, {0}},
                {2, {0, 1}, {0}, {0}},
                {2, {0, 1}, {0}, {0}},
                {2, {0, 1}, {0}, {0}},
                {3, {-1, 0, 1}, {0}, {0}},
                {5, {0.2, 0.4, 0.6, 0.8, 1}, {0}, {0}},
                {3, {-1, 0, 1}, {0}, {0}},
                {2, {0}, {0}, {0}},
            };
            //set training params
            set_training(params, i);
            //train data
            train(params, features);
            //test data on training dataset
            test(params, features, 0, params->training, 1, choice);
            //test data on testing dataset
            test(params, features, params->training, params->datacount, 0, choice);
        }
        //stop clock
        clock_t end = clock();
        //get time elapsed
        time_elapsed += (double)(end - start) / CLOCKS_PER_SEC;
        //prints time taken for program
        printf("Time taken for program: %fs\n", time_elapsed);
}

/*******************************************************************************************************
*Function init_data initialises the variables that will be used
multiple times in this program, such as placing the data from file
to memory, as well as creating the pointers to writing output
********************************************************************************************************/
void init_data(setup *params) {
    //set file pointer to file
    FILE *file_ptr;
    file_ptr = fopen("fertility_Diagnosis_Data_Group1_4.txt", "r");
    //error check if file exists
    if (file_ptr == NULL) {
        printf("File could not opened\n");
        exit(1);
    }
    //place data from file into memory
    for (int i=0;;i++) {
        //scans line by line into params->input
        int last = fscanf(file_ptr, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &params->input[i][0], &params->input[i][1], &params->input[i][2], &params->input[i][3], &params->input[i][4], &params->input[i][5], &params->input[i][6], &params->input[i][7], &params->input[i][8], &params->input[i][9]);
        //checks if last line is reached
        if (last != FEATURESIZE + 1) {
            break;
        }
        //adds 1 to count each successful read
        params->datacount++;
        //if data more than MAXDATA, warns and exits program
        if (params->datacount > MAXDATA) {
            fclose(file_ptr);
            printf("Exceeded MAXDATA\n");
            exit(1);
        }
    }
    //close file_ptr
    fclose(file_ptr);

    //open file stream for writing to testing.txt and training.txt for output
    params->plot_train = fopen("training.txt", "w");
    params->plot_test = fopen("testing.txt", "w");

}

/*******************************************************************************************************
*Function set_training allows to dynamically set the number of
training and testing sets
********************************************************************************************************/
void set_training(setup *params, int percent) {
    params->training = params->datacount / 100 * percent;
    params->testing = params->datacount - params->training;
}

/*******************************************************************************************************
*Function train will allow the program to train the data based on the 
params inputed, it will calculate and store the counts for 
non-gaussian features, and the mean and variance for gaussian features
********************************************************************************************************/
void train(setup *params, data features[FEATURESIZE]){

    //reset params data for each training
    params->normal = 0;
    params->altered = 0;

    //loops over from 0 till training size
    for (int i=0;i<params->training;i++) {
        //adds to total normal count if output is 0
        if (params->input[i][FEATURESIZE] == 0) {
            params->normal++;
            //else adds to total altered count
        } else {
            params->altered++;
        }
        //loops over from 0 to number of features
        for (int j=0;j<FEATURESIZE;j++){
            //if feature is gaussian
            if (j == 1 || j == 8) {
                //adds to total normal value of the feature (to calculate mean normal) if output is 0
                if (params->input[i][FEATURESIZE] == 0) {
                    features[j].normal[0] += params->input[i][j];
                //else add to the total altered value of the feature (to calculate mean altered) if output is 1
                } else {
                    features[j].altered[0] += params->input[i][j];
                }
                //if feature is not gaussian
            } else {
                //loops over from 0 till number of attributes within a feature
                for (int k=0;k<features[j].data_rep_size;k++){
                    //if data == feature attribute
                    if (params->input[i][j] == features[j].data_rep[k]) {
                        //increment count of feature attribute normal count if output is 0
                        if (params->input[i][FEATURESIZE] == 0) {
                            features[j].normal[k]++;
                            //else increment count of feature attribute altered count if output is 1
                        } else {
                            features[j].altered[k]++;
                        }
                    }
                }
            }
        }
    }

    //calculate mean for gaussian features where mean = totalvalue / number of data
    features[1].normal[0] /= params->normal; 
    features[1].altered[0] /= params->altered;
    features[8].normal[0] /= params->normal; 
    features[8].altered[0] /= params->altered; 

    //loop over from 0 to size of training
    for (int i=0;i<params->training;i++){
        //loop over from 0 to featuresize
        for (int j=0;j<FEATURESIZE;j++){
            //if feature is gaussian
            if (j == 1 || j == 8) {
                //calculate total sum of gaussian values of individual features minus mean for both altered and normal
                if (params->input[i][FEATURESIZE] == 0) {
                    features[j].normal[1] += pow(params->input[i][j] - features[j].normal[0], 2);
                } else {
                    features[j].altered[1] += pow(params->input[i][j] - features[j].altered[0], 2);
                }
            }
        }
    }

    //calculate variance for gaussian features using sample formula (not population)
    features[1].normal[1] /= params->normal - 1;
    features[1].altered[1] /= params->altered - 1;
    features[8].normal[1] /= params->normal - 1;
    features[8].altered[1] /= params->altered - 1;

}

/*******************************************************************************************************
*Function test will allow the program to test the data according to
the train data and write the output to the filepointers
********************************************************************************************************/
void test(setup *params, data features[FEATURESIZE], int start, int end, int mode, int choice) {
    //arrays to store conditional probability for each feature for both normal and altered
    double condp_normal[FEATURESIZE] = {0};
    double condp_altered[FEATURESIZE] = {0};

    //variables for err counting, and confusion matrix
    int err_count = 0, true_positive = 0, true_negative = 0, false_positive = 0, false_negative = 0;

    //loop from start to end
    for (int i=start;i<end;i++) {
        //loop from 0 till featuresize
        for (int j=0;j<FEATURESIZE;j++) {
            //if feature is gaussian
            if (j == 1 || j == 8) {

                //standard gaussian formula to calculate conditional probability, where variance = 1, for normal outcome
                condp_normal[j] = gaussian_condp(params->input[i][j], features[j].normal[0], features[j].normal[1]);

                condp_altered[j] = gaussian_condp(params->input[i][j], features[j].altered[0], features[j].altered[1]);

            //if feature is non-gaussian
            } else {
                //loop over from 0 to number of attributes for the feature
                for (int k=0;k<features[j].data_rep_size;k++){
                    //if data == feature attribute
                    if (params->input[i][j] == features[j].data_rep[k]) {

                        //calculate conditional probabilty with laplace smoothing, for normal outcome
                        condp_normal[j] = (features[j].normal[k] + ALPHA) / (params->normal + (FEATURESIZE * ALPHA));

                        //calculate conditional probabilty with laplace smoothing, for altered outcome
                        condp_altered[j] = (features[j].altered[k] + ALPHA) / (params->altered + (FEATURESIZE * ALPHA));
                    }
                }
            }
        }

        //get posterior probability
        double postp_normal = get_postp(condp_normal, (double)params->normal, (double)params->training);
        double postp_altered = get_postp(condp_altered, (double)params->altered, (double)params->training);

        //predicts outcome and exits program if on prediction mode
        if (choice == 0) {
            if (postp_normal >= postp_altered) {
                printf("\nPredicted outcome is: Normal");
            } else {
                printf("\nPredicted outcome is: Altered");
            }
            return;
        }

        //checks if posterior probability of normal is greater or equal to posterior probability of altered
        if (postp_normal >= postp_altered) {
            //if outcome is same as output (normal)
            if (params->input[i][FEATURESIZE] == 0) {
                //increments true negative
                true_negative++;
            //else increments false negative, and err count
            } else {
                false_negative++;
                err_count ++;
            }
            //if otherwise
        } else {
            //if outcome is same as output (altered)
            if (params->input[i][FEATURESIZE] == 1) {
                //increments true positive
                true_positive++;
            //else increments false positive, and err count
            } else {
                false_positive++;
                err_count ++;
            }
        }
    }
    
    //computes err percentage
    double err_per = (double)err_count / (end - start) * 100;

    //prints the confusion matrix and error percentage
    printf("\nConfusion Matrix and Error %c:", 37);
    if (start == 0) {
        printf("\n== Training(%d) | Err: %g%c ====", params->training, err_per, 37);
        printf("\n|\tTP: %d\t|\tFP: %d\t|", true_positive, false_positive);
        printf("\n---------------------------------");
        printf("\n|\tFN: %d\t|\tTN: %d\t|", false_negative, true_negative);
        printf("\n=================================\n");
    } else {
        printf("\n=== Testing(%d) | Err: %g%c ======", params->testing, err_per, 37);
        printf("\n|\tTP: %d\t|\tFP: %d\t|", true_positive, false_positive);
        printf("\n---------------------------------");
        printf("\n|\tFN: %d\t|\tTN: %d\t|", false_negative, true_negative);
        printf("\n=================================\n");
    }

    //switch for mode, 0 for testing dataset, 1 for training dataset, and prints outputs to file
    switch (mode) {
        //plot for testing
        case 0:
        fprintf(params->plot_test, "%d,%lf\n", params->training, err_per);
        break;
        //plot for training dataset
        case 1:
        fprintf(params->plot_train, "%d,%lf\n", params->training, err_per);
        break;
    }
}

/*******************************************************************************************************
*Function gaussian condp calculate the conditional probability using the gaussian formula
********************************************************************************************************/
double gaussian_condp(double x, double xmean, double xvar) {
    //calculate z value
    double z = (x - xmean)/ sqrt(xvar);
    //calculate gaussian prob using normalized gaussian probability formula
    double condp = (1/sqrt(2*PI)) * exp(-0.5*pow(z,2));
    return condp;
}

/*******************************************************************************************************
*Function get_postp calculates the posterior probability using log to prevent underflow
********************************************************************************************************/
double get_postp(double condp[FEATURESIZE], double num, double total) {
    //calculate prior probability
    double postp = log10(num/total);
    //calculate posterior probability using log to improve accuracy
    for (int i=0;i<FEATURESIZE;i++) {
        postp += log10(condp[i]);
    }
    postp = pow(10, postp);
    return postp;
}

/*******************************************************************************************************
*END
********************************************************************************************************/