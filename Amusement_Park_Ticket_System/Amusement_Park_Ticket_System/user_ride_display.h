#pragma once

/*
 * File: user_ride_display.h
 * Author: Prisha Bhaveshkumar Patel
 * Purpose: Defines the User and Ride structures and declares the
 *          User and Ride Display module functions.
 */

#ifndef USER_RIDE_DISPLAY_H
#define USER_RIDE_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

    /*=========================================================
                            CONSTANTS
    =========================================================*/

#define USER_NAME_LENGTH 64
#define RIDE_NAME_LENGTH 64
#define RIDE_CATALOG_SIZE 15

    /*=========================================================
                          USER STRUCTURE
    =========================================================*/

    typedef struct User
    {
		int id;
        char name[USER_NAME_LENGTH];
        int age;
		int height; /* in centimeters */
		int weight; /* in kilograms */
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
		int minHeight; /* in centimeters */
		int maxWeight; /* in kilograms; 0 means no maximum */
		double price;
    } Ride;

    /*=========================================================
                      USER MODULE FUNCTIONS
    =========================================================*/

    /*
     * Author: Prisha Bhaveshkumar Patel
     * Inputs: Pointer to a User structure and visitor input from the keyboard.
     * Outputs: Populates the User structure and returns 1 when registration is
     *          completed; returns 0 if the pointer is invalid or input ends.
     * Purpose: Collects and validates the visitor's name, age, height and weight.
     */
    int getUserDetails(User* user);

    /*=========================================================
                  RIDE DISPLAY MODULE FUNCTIONS
    =========================================================*/

    /*
     * Author: Prisha Bhaveshkumar Patel
     * Inputs: Ride array and its maximum available capacity.
     * Outputs: Populates the array and returns the number of initialized rides;
     *          returns 0 when the array is invalid or too small.
     * Purpose: Loads the approved amusement park rides into Ride structures.
     */
    int initializeRides(Ride rides[], int maximumRides);

    /*
     * Author: Prisha Bhaveshkumar Patel
     * Inputs: Ride array and number of rides in the array.
     * Outputs: Displays the complete ride catalogue on the console.
     * Purpose: Shows ride IDs, names, categories, restrictions and prices.
     */
    void displayRides(const Ride rides[], int rideCount);

    /*
     * Author: Prisha Bhaveshkumar Patel
     * Inputs: Ride array, ride count and numeric ride ID.
     * Outputs: Returns a pointer to the matching Ride or NULL when not found.
     * Purpose: Allows other modules to retrieve a selected ride by its ID.
     */
    const Ride* getRideById(
        const Ride rides[],
        int rideCount,
        int rideId);

    /*
     * Author: Prisha Bhaveshkumar Patel
     * Inputs: Numeric ride ID.
     * Outputs: Returns the category text or NULL when the ID is not in the
     *          catalogue.
     * Purpose: Provides the category stored in the module's 2D character array.
     */
    const char* getRideCategoryById(int rideId);

#ifdef __cplusplus
}
#endif

#endif /* USER_RIDE_DISPLAY_H */
