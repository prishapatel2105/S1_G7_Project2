#pragma once
/**
 * @file user_ride_display.h
 * @brief User management and ride display functions.
 *
 * This module provides functionality for registering users,
 * displaying available rides, displaying the user's selected
 * rides, and searching for rides by ID.
 */
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
    /**
 * @struct User
 * @brief Stores information about a visitor.
 *
 * Contains the visitor's name, age, height, and weight.
 */
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
    /**
     * @struct User
     * @brief Stores information about a visitor.
     *
	 * Contains the visitor's id, name, price, minimum age, minimum height, and maximum weight.
     */
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
    /**
     * @brief Displays all available rides.
     *
     * @param rides Array containing the available rides.
     * @param rideCount Number of rides in the array.
     */
    void displayRides(
        const Ride rides[],
        int rideCount);
    /**
 * @brief Finds a ride using its ride ID.
 *
 * @param rides Array containing available rides.
 * @param rideCount Number of rides in the array.
 * @param rideId ID of the ride to search for.
 *
 * @return Pointer to the matching ride.
 * @return NULL if the ride is not found.
 */
    const Ride* getRideById(
        const Ride rides[],
        int rideCount,
        int rideId);

    const char* getRideCategoryById(
        int rideId);

#ifdef __cplusplus
}
#endif