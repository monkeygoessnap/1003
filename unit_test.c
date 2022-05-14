/*******************************************************************************************************
*CSC1003
*GRP P1-1 Final Project
*Naive Bayers
*Test file
********************************************************************************************************/

/*******************************************************************************************************
*Header file
********************************************************************************************************/
#include "project.h"

/*******************************************************************************************************
*Function prototypes
********************************************************************************************************/
void test_init_data(setup *params);
void test_set_training(setup *params);
void test_train(setup *params, data features[FEATURESIZE]);
void test_gaussian_condp(void);

/*******************************************************************************************************
*Function run_tests initializes dependencies and runs the tests
********************************************************************************************************/
void run_tests(setup *params) {

    //dependency
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

    //run tests
    test_init_data(params);
    test_set_training(params);
    test_train(params, features);
    test_gaussian_condp();
}

/*******************************************************************************************************
*Function tests the Function init_data in project.c
********************************************************************************************************/
void test_init_data(setup *params) {

    //test cases, boundary testing
    bool test_cases[5] = {
        params->datacount == 100,
        params->input[0][0] == -0.33,
        params->input[0][9] == 0,
        params->input[99][0] == -1,
        params->input[99][9] == 0,
    };

    //loop through test cases
    for (int i=0;i<5;i++) {
        if (test_cases[i] == true) {
            printf("Function init_data | T.Case %d - PASSED\n", i+1);
        } else {
            printf("Function init_data | T.Case %d - FAILED\n", i+1);
        }
    }
    printf("\n");
}

/*******************************************************************************************************
*Function tests the Function set_training in project.c
********************************************************************************************************/
void test_set_training(setup *params) {

    //set dependency
    set_training(params, 90);
    //set test cases
    bool test_cases[2] = {
        params->training == 90,
        params->testing == 10,
    };
    //loop through test cases
    for (int i=0;i<2;i++) {
        if (test_cases[i] == true) {
            printf("Function set_training | T.Case %d - PASSED\n", i+1);
        } else {
            printf("Function set_training | T.Case %d - FAILED\n", i+1);
        }
    }
    printf("\n");
}

/*******************************************************************************************************
*Function tests the Function train in project.c
********************************************************************************************************/
void test_train(setup *params, data features[FEATURESIZE]) {
    //set dependency
    train(params, features);
    //test cases
    bool test_cases[8] = {
        params->altered == 11,
        params->normal == 79,
        features[0].altered[0] == 1,
        features[0].normal[0] == 22,
        features[7].altered[2] == 3,
        features[7].normal[2] == 16,
        features[1].altered[0] == (7.73/11),
        features[8].normal[0] == (32.65/79),
    };
    //loop through test cases
    for (int i=0;i<8;i++) {
        if (test_cases[i] == true) {
            printf("Function train | T.Case %d - PASSED\n", i+1);
        } else {
            printf("Function train | T.Case %d - FAILED\n", i+1);
        }
    }
    printf("\n");
}

/*******************************************************************************************************
*Function tests the Function gaussian_condp in project.c
********************************************************************************************************/
void test_gaussian_condp(void) {
    //set dependencies
    double x = 18;
    double mean = 10;
    double var = 4;
    double result = gaussian_condp(x, mean, var);
    //test cases
    bool test_cases[1] = {
        result == (1/sqrt(2*PI) * exp(-8)),
    };
    //loop through test cases
    for (int i=0;i<1;i++) {
        if (test_cases[i] == true) {
            printf("Function gaussian_condp | T.Case %d - PASSED\n", i+1);
        } else {
            printf("Function gaussian condp | T.Case %d - FAILED\n", i+1);
        }
    }
    printf("\n");
}