#include "pch.h"
#include "CppUnitTest.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

extern "C"
{
#include "../Amusement_Park_Ticket_System/file_exit.h"

    /*
     * Include the implementation directly so the test project can
     * access the File and Exit module functions.
     *
     * Do not add file_exit.c separately to the UnitTest4 project.
     */
#include "../Amusement_Park_Ticket_System/file_exit.c"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest4
{
    TEST_CLASS(FileExitModuleTests)
    {
    private:

        /*
         * Creates a temporary text file for a test.
         */
        static void writeTextFile(
            const char* filename,
            const char* content)
        {
            FILE* file = nullptr;

            errno_t result = fopen_s(
                &file,
                filename,
                "w");

            Assert::AreEqual(
                0,
                static_cast<int>(result));

            Assert::IsNotNull(file);

            if (file != nullptr)
            {
                fputs(content, file);
                fclose(file);
            }
        }

        /*
         * Reads an entire text file into a C++ string.
         */
        static std::string readTextFile(
            const char* filename)
        {
            std::ifstream inputFile(
                filename,
                std::ios::in);

            std::ostringstream content;

            content << inputFile.rdbuf();

            return content.str();
        }

        /*
         * Creates valid test User information.
         */
        static User createUser(void)
        {
            User user = {};

            snprintf(
                user.name,
                sizeof(user.name),
                "%s",
                "Pratyasa Roy");

            user.age = 21;
            user.height = 165;
            user.weight = 60;

            return user;
        }

        /*
         * Creates a valid Ride structure.
         */
        static Ride createRide(
            int id,
            const char* name,
            float price,
            int minimumAge,
            int minimumHeight,
            int maximumWeight)
        {
            Ride ride = {};

            ride.id = id;

            snprintf(
                ride.name,
                sizeof(ride.name),
                "%s",
                name);

            ride.price = price;
            ride.minAge = minimumAge;
            ride.minHeight = minimumHeight;
            ride.maxWeight = maximumWeight;

            return ride;
        }

        /*
         * Creates valid test Ticket information.
         */
        static Ticket createTicket(void)
        {
            Ticket ticket = {};

            ticket.ticketId = 123456;
            ticket.rideCount = 2;
            ticket.subtotal = 35.00f;
            ticket.discountPercentage = 0.00f;
            ticket.discountAmount = 0.00f;
            ticket.finalTotal = 35.00f;

            return ticket;
        }

    public:

        /*
         * Test 1:
         * A valid CSV record with an R-prefixed ID should load.
         */
        TEST_METHOD(
            Test_LoadRideData_RPrefixedId_LoadsSuccessfully)
        {
            const char* filename =
                "UnitTest4_RPrefixedRide.csv";

            writeTextFile(
                filename,
                "R101, Thunder Coaster, 25.00, 12, 140, 120\n");

            Ride rides[FILE_MODULE_MAX_RIDES] = {};
            int rideCount = 0;

            int result = loadRideData(
                filename,
                rides,
                &rideCount);

            Assert::AreEqual(1, result);
            Assert::AreEqual(1, rideCount);
            Assert::AreEqual(101, rides[0].id);

            Assert::AreEqual(
                "Thunder Coaster",
                rides[0].name);

            Assert::AreEqual(
                25.00,
                static_cast<double>(rides[0].price),
                0.001);

            Assert::AreEqual(12, rides[0].minAge);
            Assert::AreEqual(140, rides[0].minHeight);
            Assert::AreEqual(120, rides[0].maxWeight);

            remove(filename);
        }

        /*
         * Test 2:
         * A valid CSV record without the R prefix should load.
         */
        TEST_METHOD(
            Test_LoadRideData_NumericId_LoadsSuccessfully)
        {
            const char* filename =
                "UnitTest4_NumericRide.csv";

            writeTextFile(
                filename,
                "102, Sky Ferris Wheel, 10.00, 0, 0, 0\n");

            Ride rides[FILE_MODULE_MAX_RIDES] = {};
            int rideCount = 0;

            int result = loadRideData(
                filename,
                rides,
                &rideCount);

            Assert::AreEqual(1, result);
            Assert::AreEqual(1, rideCount);
            Assert::AreEqual(102, rides[0].id);

            Assert::AreEqual(
                "Sky Ferris Wheel",
                rides[0].name);

            Assert::AreEqual(
                10.00,
                static_cast<double>(rides[0].price),
                0.001);

            remove(filename);
        }

        /*
         * Test 3:
         * Headers, comments and blank lines should be ignored.
         */
        TEST_METHOD(
            Test_LoadRideData_HeaderCommentsAndBlankLines_AreIgnored)
        {
            const char* filename =
                "UnitTest4_MultipleRides.csv";

            writeTextFile(
                filename,
                "RideID,Name,Price,MinAge,MinHeight,MaxWeight\n"
                "# Approved ride records\n"
                "\n"
                "R101, Thunder Coaster, 25.00, 12, 140, 120\n"
                "102, Sky Ferris Wheel, 10.00, 0, 0, 0\n");

            Ride rides[FILE_MODULE_MAX_RIDES] = {};
            int rideCount = 0;

            int result = loadRideData(
                filename,
                rides,
                &rideCount);

            Assert::AreEqual(1, result);
            Assert::AreEqual(2, rideCount);
            Assert::AreEqual(101, rides[0].id);
            Assert::AreEqual(102, rides[1].id);

            Assert::AreEqual(
                "Thunder Coaster",
                rides[0].name);

            Assert::AreEqual(
                "Sky Ferris Wheel",
                rides[1].name);

            remove(filename);
        }

        /*
         * Test 4:
         * An invalid CSV record should cause loading to fail.
         */
        TEST_METHOD(
            Test_LoadRideData_InvalidRecord_ReturnsZero)
        {
            const char* filename =
                "UnitTest4_InvalidRide.csv";

            writeTextFile(
                filename,
                "This is not a valid ride record\n");

            Ride rides[FILE_MODULE_MAX_RIDES] = {};
            int rideCount = 99;

            int result = loadRideData(
                filename,
                rides,
                &rideCount);

            Assert::AreEqual(0, result);
            Assert::AreEqual(0, rideCount);

            remove(filename);
        }

        /*
         * Test 5:
         * Loading a file that does not exist should fail.
         */
        TEST_METHOD(
            Test_LoadRideData_FileDoesNotExist_ReturnsZero)
        {
            const char* filename =
                "UnitTest4_FileDoesNotExist.csv";

            remove(filename);

            Ride rides[FILE_MODULE_MAX_RIDES] = {};
            int rideCount = 99;

            int result = loadRideData(
                filename,
                rides,
                &rideCount);

            Assert::AreEqual(0, result);
            Assert::AreEqual(0, rideCount);
        }

        /*
         * Test 6:
         * A NULL ride array should cause loading to fail.
         */
        TEST_METHOD(
            Test_LoadRideData_NullRideArray_ReturnsZero)
        {
            int rideCount = 99;

            int result = loadRideData(
                "UnitTest4_Rides.csv",
                NULL,
                &rideCount);

            Assert::AreEqual(0, result);
            Assert::AreEqual(0, rideCount);
        }

        /*
         * Test 7:
         * A complete valid ticket should be saved successfully.
         */
        TEST_METHOD(
            Test_SaveTicketRecord_ValidInformation_SavesFile)
        {
            const char* filename =
                "UnitTest4_CompleteTicket.txt";

            remove(filename);

            User user = createUser();

            Ride cart[2] =
            {
                createRide(
                    101,
                    "Thunder Coaster",
                    25.00f,
                    12,
                    140,
                    120),

                createRide(
                    102,
                    "Sky Ferris Wheel",
                    10.00f,
                    0,
                    0,
                    0)
            };

            Ticket ticket = createTicket();

            int result = saveTicketRecord(
                filename,
                &user,
                cart,
                2,
                &ticket);

            Assert::AreEqual(1, result);

            std::string content =
                readTextFile(filename);

            Assert::IsTrue(
                content.find("AMUSEMENT PARK TICKET") !=
                std::string::npos);

            Assert::IsTrue(
                content.find("Ticket ID: 123456") !=
                std::string::npos);

            Assert::IsTrue(
                content.find("Visitor Name: Pratyasa Roy") !=
                std::string::npos);

            Assert::IsTrue(
                content.find("Age: 21") !=
                std::string::npos);

            Assert::IsTrue(
                content.find(
                    "R101,Thunder Coaster,$25.00") !=
                std::string::npos);

            Assert::IsTrue(
                content.find("Subtotal: $35.00") !=
                std::string::npos);

            Assert::IsTrue(
                content.find("Final Total: $35.00") !=
                std::string::npos);

            remove(filename);
        }

        /*
         * Test 8:
         * A NULL User pointer should prevent ticket saving.
         */
        TEST_METHOD(
            Test_SaveTicketRecord_NullUser_ReturnsZero)
        {
            const char* filename =
                "UnitTest4_NullUserTicket.txt";

            remove(filename);

            Ride cart[1] =
            {
                createRide(
                    101,
                    "Thunder Coaster",
                    25.00f,
                    12,
                    140,
                    120)
            };

            Ticket ticket = createTicket();

            int result = saveTicketRecord(
                filename,
                NULL,
                cart,
                1,
                &ticket);

            Assert::AreEqual(0, result);

            remove(filename);
        }

        /*
         * Test 9:
         * A ticket ID of zero should prevent ticket saving.
         */
        TEST_METHOD(
            Test_SaveTicketRecord_InvalidTicketId_ReturnsZero)
        {
            const char* filename =
                "UnitTest4_InvalidTicketId.txt";

            remove(filename);

            User user = createUser();

            Ride cart[1] =
            {
                createRide(
                    101,
                    "Thunder Coaster",
                    25.00f,
                    12,
                    140,
                    120)
            };

            Ticket ticket = createTicket();
            ticket.ticketId = 0;

            int result = saveTicketRecord(
                filename,
                &user,
                cart,
                1,
                &ticket);

            Assert::AreEqual(0, result);

            remove(filename);
        }

        /*
         * Test 10:
         * saveTicketDetails should create a basic ticket file.
         */
        TEST_METHOD(
            Test_SaveTicketDetails_ValidInformation_SavesFile)
        {
            const char* filename =
                "UnitTest4_BasicTicket.txt";

            remove(filename);

            User user = createUser();

            Ride cart[1] =
            {
                createRide(
                    103,
                    "Bumper Cars",
                    15.00f,
                    8,
                    110,
                    130)
            };

            int result = saveTicketDetails(
                filename,
                &user,
                cart,
                1,
                654321,
                15.00f);

            Assert::AreEqual(1, result);

            std::string content =
                readTextFile(filename);

            Assert::IsTrue(
                content.find("Ticket ID: 654321") !=
                std::string::npos);

            Assert::IsTrue(
                content.find("Visitor Name: Pratyasa Roy") !=
                std::string::npos);

            Assert::IsTrue(
                content.find("R103,Bumper Cars,$15.00") !=
                std::string::npos);

            Assert::IsTrue(
                content.find("Subtotal: $15.00") !=
                std::string::npos);

            Assert::IsTrue(
                content.find("Final Total: $15.00") !=
                std::string::npos);

            remove(filename);
        }

        /*
         * Test 11:
         * An empty cart should prevent basic ticket saving.
         */
        TEST_METHOD(
            Test_SaveTicketDetails_ZeroRideCount_ReturnsZero)
        {
            const char* filename =
                "UnitTest4_EmptyCartTicket.txt";

            remove(filename);

            User user = createUser();

            Ride cart[1] =
            {
                createRide(
                    101,
                    "Thunder Coaster",
                    25.00f,
                    12,
                    140,
                    120)
            };

            int result = saveTicketDetails(
                filename,
                &user,
                cart,
                0,
                123456,
                0.00f);

            Assert::AreEqual(0, result);

            remove(filename);
        }

        /*
         * Test 12:
         * cleanupSystem should free an allocated cart and set
         * its pointer to NULL.
         */
        TEST_METHOD(
            Test_CleanupSystem_AllocatedCart_SetsPointerToNull)
        {
            Ride* cart = static_cast<Ride*>(
                malloc(2 * sizeof(Ride)));

            Assert::IsNotNull(cart);

            cleanupSystem(
                &cart,
                NULL);

            Assert::IsNull(cart);
        }

        /*
         * Test 13:
         * cleanupSystem should close an open file.
         */
        TEST_METHOD(
            Test_CleanupSystem_OpenFile_ClosesFile)
        {
            const char* filename =
                "UnitTest4_CleanupFile.txt";

            remove(filename);

            FILE* file = nullptr;

            errno_t openResult = fopen_s(
                &file,
                filename,
                "w");

            Assert::AreEqual(
                0,
                static_cast<int>(openResult));

            Assert::IsNotNull(file);

            cleanupSystem(
                NULL,
                file);

            /*
             * Windows normally prevents deleting an open file.
             * A successful deletion indicates the file was closed.
             */
            int removeResult = remove(filename);

            Assert::AreEqual(0, removeResult);
        }

        /*
         * Test 14:
         * cleanupSystem should safely accept NULL arguments.
         */
        TEST_METHOD(
            Test_CleanupSystem_NullArguments_DoesNotThrow)
        {
            try
            {
                cleanupSystem(
                    NULL,
                    NULL);

                Assert::IsTrue(true);
            }
            catch (...)
            {
                Assert::Fail(
                    L"cleanupSystem threw an unexpected exception.");
            }
        }

        /*
         * Test 15:
         * exitProgram should complete without throwing an exception.
         */
        TEST_METHOD(
            Test_ExitProgram_DoesNotThrow)
        {
            try
            {
                exitProgram();

                Assert::IsTrue(true);
            }
            catch (...)
            {
                Assert::Fail(
                    L"exitProgram threw an unexpected exception.");
            }
        }
    };
}