#include "pch.h"
#include "CppUnitTest.h"

extern "C"
{
#include "../Amusement_Park_Ticket_System/user_ride_display.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(UserRideDisplayTests)
    {
    public:

        // Test 1
        TEST_METHOD(GetUserDetails_ReturnValue_ShouldBeSuccess)
        {
            User user{};
            int result = getUserDetails(&user);

            Assert::AreEqual(1, result);
        }

        // Test 2
        TEST_METHOD(GetUserDetails_NullPointer_ShouldReturnFailure)
        {
            int result = getUserDetails(NULL);

            Assert::AreEqual(0, result);
        }

        // Test 3
        TEST_METHOD(GetUserDetails_Age_ShouldNotBeNegative)
        {
            User user{};
            getUserDetails(&user);

            Assert::IsTrue(user.age >= 0);
        }

        // Test 4
        TEST_METHOD(GetUserDetails_Height_ShouldNotBeNegative)
        {
            User user{};
            getUserDetails(&user);

            Assert::IsTrue(user.height >= 0);
        }

        // Test 5
        TEST_METHOD(GetUserDetails_Weight_ShouldNotBeNegative)
        {
            User user{};
            getUserDetails(&user);

            Assert::IsTrue(user.weight >= 0);
        }

        // Test 6
        TEST_METHOD(InitializeRides_ReturnsCatalogSize)
        {
            Ride rides[RIDE_CATALOG_SIZE];

            int result = initializeRides(rides, RIDE_CATALOG_SIZE);

            Assert::AreEqual(RIDE_CATALOG_SIZE, result);
        }

        // Test 7
        TEST_METHOD(InitializeRides_NullArray_ReturnsZero)
        {
            int result = initializeRides(NULL, RIDE_CATALOG_SIZE);

            Assert::AreEqual(0, result);
        }

        // Test 8
        TEST_METHOD(InitializeRides_ArrayTooSmall_ReturnsZero)
        {
            Ride rides[5];

            int result = initializeRides(rides, 5);

            Assert::AreEqual(0, result);
        }

        // Test 9
        TEST_METHOD(InitializeRides_FirstRideID_ShouldBePositive)
        {
            Ride rides[RIDE_CATALOG_SIZE];

            initializeRides(rides, RIDE_CATALOG_SIZE);

            Assert::IsTrue(rides[0].id > 0);
        }

        // Test 10
        TEST_METHOD(InitializeRides_FirstRidePrice_ShouldBePositive)
        {
            Ride rides[RIDE_CATALOG_SIZE];

            initializeRides(rides, RIDE_CATALOG_SIZE);

            Assert::IsTrue(rides[0].price > 0.0f);
        }

        // Test 11
        TEST_METHOD(GetRideCategoryById_ValidID_ShouldReturnCorrectCategory)
        {
            const char* category = getRideCategoryById(1);

            Assert::IsNotNull(category);

            Assert::AreEqual(
                "Roller Coaster",
                category);
        }

        // Test 12
        TEST_METHOD(GetRideCategoryById_InvalidID_ShouldReturnNull)
        {
            const char* category = getRideCategoryById(999);

            Assert::IsNull(category);
        }

        // Test 13
        TEST_METHOD(DisplayRides_ShouldNotModifyRideID)
        {
            Ride rides[1] = { {1, "Roller Coaster", 15.0f, 12, 140, 120} };

            int originalID = rides[0].id;

            displayRides(rides, 1);

            Assert::AreEqual(originalID, rides[0].id);
        }

        // Test 14
        TEST_METHOD(DisplayRides_ShouldNotModifyRidePrice)
        {
            Ride rides[1] =
            {
                {1, "Roller Coaster", 12, 140, 120, 15.0f}
            };

            float originalPrice = rides[0].price;

            displayRides(rides, 1);

            Assert::AreEqual(
                originalPrice,
                rides[0].price,
                0.001f);
        }

        // Test 15
        TEST_METHOD(DisplayRides_ShouldNotModifyRideName)
        {
            Ride rides[1] = {};

            rides[0].id = 1;

            strcpy_s(
                rides[0].name,
                sizeof(rides[0].name),
                "Roller Coaster");

            rides[0].minAge = 12;
            rides[0].minHeight = 140;
            rides[0].maxWeight = 120;
            rides[0].price = 15.00;

            char originalName[RIDE_NAME_LENGTH];

            strcpy_s(
                originalName,
                sizeof(originalName),
                rides[0].name);

            displayRides(rides, 1);

            Assert::AreEqual(
                0,
                strcmp(originalName, rides[0].name));
        }

        // Test 16
        TEST_METHOD(GetRideById_ExistingRide_ShouldReturnActualArrayElement)
        {
            Ride rides[1] =
            {
                {
                    5,
                    "Ferris Wheel",
                    8.50f,
                    5,
                    100,
                    150
                }
            };

            const Ride* result = getRideById(rides, 1, 5);

            Assert::IsTrue(result == &rides[0]);
        }

        // Test 17
        TEST_METHOD(GetRideById_ExistingRide_ShouldMatchID)
        {
            Ride rides[1] = { {5, "Ferris Wheel", 8.5f, 5, 100, 150} };

            const Ride* ride = getRideById(rides, 1, 5);

            Assert::AreEqual(5, ride->id);
        }

        // Test 18
        TEST_METHOD(GetRideById_InvalidRide_ShouldReturnNull)
        {
            Ride rides[1] = { {5, "Ferris Wheel", 8.5f, 5, 100, 150} };

            const Ride* ride = getRideById(rides, 1, 999);

            Assert::IsNull(ride);
        }

        // Test 19
        TEST_METHOD(InitializeRides_FirstRideName_ShouldBeThunderCoaster)
        {
            Ride rides[RIDE_CATALOG_SIZE] = {};

            initializeRides(
                rides,
                RIDE_CATALOG_SIZE);

            Assert::AreEqual(
                "Thunder Coaster",
                rides[0].name);
        }
    };
}