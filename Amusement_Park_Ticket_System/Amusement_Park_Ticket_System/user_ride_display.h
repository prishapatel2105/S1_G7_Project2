/*
 * File: user_ride_display.h
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
		int minAge;
		int minHeight; /* in centimeters */
		int maxWeight; /* in kilograms; 0 means no maximum */
		double price;
	} Ride;

	/*=========================================================
					  USER MODULE FUNCTIONS
	=========================================================*/

	int getUserDetails(User* user);

	/*=========================================================
				  RIDE DISPLAY MODULE FUNCTIONS
	=========================================================*/

	int initializeRides(Ride rides[], int maximumRides);

	void displayRides(const Ride rides[], int rideCount);

	const Ride* getRideById(
		const Ride rides[],
		int rideCount,
		int rideId);

	const char* getRideCategoryById(int rideId);

#ifdef __cplusplus
}
#endif

#endif /* USER_RIDE_DISPLAY_H */
