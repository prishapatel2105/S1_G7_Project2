/*
 * File: file_exit.c
 * Author: Pratyasa Roy
 * Purpose: Temporary TDD Red-phase implementation.
 *
 * IMPORTANT:
 * This file deliberately contains incorrect function behavior
 * so all UnitTest4 test cases fail.
 *
 * Replace this file with the correct implementation immediately
 * after recording the Red-phase evidence.
 */

#define _CRT_SECURE_NO_WARNINGS

#include "file_exit.h"

#include <stdio.h>

 /*=========================================================
                   TEMPORARY RED-PHASE CODE
 =========================================================*/

 /*
  * Deliberately fails successful loading tests.
  *
  * For invalid-input tests, it deliberately sets rideCount
  * incorrectly or returns the incorrect result.
  */
int loadRideData(
    const char* filename,
    Ride rides[],
    int* rideCount)
{
    /*
     * The tests expect rideCount to become zero when loading
     * fails. Setting it to -1 deliberately violates that
     * requirement.
     */
    if (rideCount != NULL)
    {
        *rideCount = -1;
    }

    /*
     * Invalid arguments should return zero, but this temporary
     * Red-phase implementation returns one.
     */
    if (filename == NULL ||
        filename[0] == '\0' ||
        rides == NULL ||
        rideCount == NULL)
    {
        return 1;
    }

    /*
     * Valid ride files should return one, but this temporary
     * implementation returns zero.
     */
    return 0;
}

/*
 * Deliberately returns the opposite result expected by the tests.
 */
int saveTicketRecord(
    const char* filename,
    const User* user,
    const Ride* cart,
    int rideCount,
    const Ticket* ticket)
{
    /*
     * Invalid ticket information should return zero.
     * Return one so invalid-input tests fail.
     */
    if (filename == NULL ||
        filename[0] == '\0' ||
        user == NULL ||
        cart == NULL ||
        rideCount <= 0 ||
        ticket == NULL ||
        ticket->ticketId <= 0)
    {
        return 1;
    }

    /*
     * Valid ticket information should return one.
     * Return zero so the valid-saving test fails.
     */
    return 0;
}

/*
 * Deliberately returns the opposite result expected by the tests.
 */
int saveTicketDetails(
    const char* filename,
    const User* user,
    const Ride* cart,
    int rideCount,
    int ticketId,
    float totalAmount)
{
    /*
     * Invalid information should return zero.
     * Return one so the invalid-input test fails.
     */
    if (filename == NULL ||
        filename[0] == '\0' ||
        user == NULL ||
        cart == NULL ||
        rideCount <= 0 ||
        ticketId <= 0 ||
        totalAmount < 0.0f)
    {
        return 1;
    }

    /*
     * Valid information should return one.
     * Return zero so the valid-saving test fails.
     */
    return 0;
}

/*
 * Deliberately does not free memory or close the file.
 *
 * UnitTest4 includes this .c file in a C++ test file, so
 * __cplusplus is defined there. Throwing causes all cleanup
 * tests to fail.
 *
 * When this file is compiled as normal C, the throw statement
 * is excluded.
 */
void cleanupSystem(
    Ride** cart,
    FILE* file)
{
    (void)cart;
    (void)file;

#ifdef __cplusplus
    throw 1;
#endif
}

/*
 * Deliberately throws during the UnitTest4 C++ build so the
 * exit test fails.
 */
void exitProgram(void)
{
#ifdef __cplusplus
    throw 1;
#else
    printf(
        "Temporary TDD Red-phase exit function.\n");
#endif
}