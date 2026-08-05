#pragma once

/*
 * File: user_ride_display.h
 * Author: Prisha Bhaveshkumar Patel
 * Purpose: Defines the User and Ride structures and declares the
 *          User and Ride Display module functions.
 */

#ifdef __cplusplus
extern "C" {
#endif

    /*=========================================================
                            CONSTANTS
    =========================================================*/

#define USER_NAME_LENGTH 50
#define RIDE_NAME_LENGTH 50
#define RIDE_CATALOG_SIZE 15

    /*=========================================================
                          USER STRUCTURE
    =========================================================*/

    typedef struct User
    {
        char name[USER_NAME_LENGTH];
        int age;
        int height;
        int weight;
    } User;

    /*=========================================================
                          RIDE STRUCTURE
    =========================================================*/

    typedef struct Ride
    {
        int id;
        char name[RIDE_NAME_LENGTH];
        float price;
        int minAge;
        int minHeight;
        int maxWeight;
    } Ride;

    /*=========================================================
                      USER MODULE FUNCTIONS
    =========================================================*/

    int getUserDetails(User* user);

    /*=========================================================
                  RIDE DISPLAY MODULE FUNCTIONS
    =========================================================*/

    int initializeRides(
        Ride rides[],
        int maximumRides);

    void displayRides(
        const Ride rides[],
        int rideCount);

    const Ride* getRideById(
        const Ride rides[],
        int rideCount,
        int rideId);

    const char* getRideCategoryById(
        int rideId);

#ifdef __cplusplus
}
#endif