/*
 * File: file_exit.h
 * Author: Pratyasa Roy
 * Purpose: Declares file input/output, cleanup and application exit
 *          functions for the Amusement Park Ticket Generator.
 */
 /**
  * @file file_exit.h
  * @brief File management and application cleanup functions.
  *
  * This module handles loading ride information, saving ticket
  * records, maintaining booking history, and cleaning up
  * dynamically allocated resources.
  */
#ifndef EXIT_FILE_MODULE_H
#define EXIT_FILE_MODULE_H

#include <stdio.h>

#include "delete_billing.h"
#include "eligibility_selection.h"
#include "user_ride_display.h"

#define FILE_MODULE_MAX_RIDES 100

#ifdef __cplusplus
extern "C" {
#endif

    /*=========================================================
                        FILE MODULE FUNCTIONS
    =========================================================*/

    /*
     * Author: Pratyasa Roy
     * Inputs: Input filename, Ride array and ride-count pointer.
     * Outputs: Populates the Ride array and count. Returns 1 when at least one
     *          valid ride is loaded; otherwise, returns 0.
     * Purpose: Reads comma-separated ride records from an ASCII text file.
     */
     /**
  * @brief Loads ride information from a ride data file.
  *
  * Reads ride information from the specified file and stores
  * the available rides in the provided array.
  *
  * @param filename Name of the ride data file.
  * @param rides Array where the loaded rides will be stored.
  * @param rideCount Pointer to the number of rides loaded.
  *
  * @return 1 if the ride data was loaded successfully.
  * @return 0 if the file could not be opened or the data is invalid.
  */
    int loadRideData(
        const char* filename,
        Ride rides[],
        int* rideCount);

    /*
     * Author: Pratyasa Roy
     * Inputs: Output filename, User, final ride cart, ride count and Ticket.
     * Outputs: Writes the complete ticket record and returns 1 when successful;
     *          otherwise, returns 0.
     * Purpose: Saves visitor, ride, subtotal, discount and final-total
     *          information.
     */
    int saveTicketRecord(
        const char* filename,
        const User* user,
        const Ride* cart,
        int rideCount,
        const Ticket* ticket);

    /*
     * Author: Pratyasa Roy
     * Inputs: Output filename, User, cart, ride count, ticket ID and total amount.
     * Outputs: Writes a basic ticket record and returns 1 when successful;
     *          otherwise, returns 0.
     * Purpose: Preserves the function interface from the detailed design.
     *          New code should use saveTicketRecord() to include discount
     *          information.
     */
    int saveTicketDetails(
        const char* filename,
        const User* user,
        const Ride* cart,
        int rideCount,
        int ticketId,
        float totalAmount);

    /*=========================================================
                        EXIT MODULE FUNCTIONS
    =========================================================*/

    /*
     * Author: Pratyasa Roy
     * Inputs: Address of the dynamically allocated Ride cart pointer and an
     *         optional pointer to an open file.
     * Outputs: Frees the cart, sets the cart pointer to NULL and closes the file.
     * Purpose: Releases resources before the application terminates.
     */
     /**
  * @brief Cleans up system resources before application termination.
  *
  * Frees dynamically allocated memory for the ride cart and closes any open files.
  *
  * @param cart Pointer to the user's ride cart.
  * @param file Pointer to the open file (if applicable).
  */
    void cleanupSystem(
        Ride** cart,
        FILE* file);

    /*
     * Author: Pratyasa Roy
     * Inputs: None.
     * Outputs: Displays a successful application-exit message.
     * Purpose: Provides the normal-exit routine that can be registered by using
     *          atexit(exitProgram).
     */
     /**
  * @brief Performs application exit cleanup.
  *
  * Releases resources and performs required cleanup operations
  * when the application terminates.
  */
    void exitProgram(void);

#ifdef __cplusplus
}
#endif

#endif /* EXIT_FILE_MODULE_H */