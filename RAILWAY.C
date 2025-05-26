#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h> // For system() function
#include<time.h>   // For generating random transaction IDs

//train structure
struct Train {
    int trainNumber;
    char trainName[20];
    char source[30];
    char destination[30];
    int totalSeats;
    int availableSeats;
    int pricePerticket;
};

//structure for booking ticket
struct Booking {
    int trainNumber;
    int bookedSeats;
    int totalprice;
    char paymentStatus[20]; // "Pending", "Completed", "Failed"
    char transactionID[20];  // random trans id store
};

struct Train trains[20];
int trainCount = 0;  // to store how many trains

struct Booking bookings[100];
int bookingCount = 0;// to store how many bookings

// function prototypes  for admin
void adminmanu();
void addTrain();
void viewTrains();
void deleteTrain();

// function prototypes for user
void usermanu();
void bookTrain();
void viewBookings();
void cancelBooking();
void processPayment(int bookingIndex);

// Payment gateway functions
void showPaymentOptions(int amount);
int processCardPayment(int amount);
int processMobileBanking(int amount);
void generateTransactionID(char* id);  // use pointer  for id

//------------------------------login page---------------------------------------------------------------
int main() {
    int choice;
    while(1) {
        printf("\n");
        printf("==================================================\n");
        printf("|                                                |\n");
        printf("|   WELCOME TO TRAIN TICKET MANAGEMENT SYSTEM    |\n");
        printf("|                                                |\n");
        printf("==================================================\n");
        printf("\n");
        printf(" Enter any choice you as your action needed:\n");
        printf("1.admin login\n");
        printf("2.user login\n");
        printf("3.exit\n");
        printf("enter your choice:");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                adminmanu();
                break;
            case 2:
                usermanu();
                break;
            case 3:
                return 0;  // Exit program
            default:
                printf("Invalid choice. Please select 1-3.\n");
        }
    }
}

//------------------------------ admin page ----------------------
void adminmanu() {
    int adminchoice;
    while(1) {
        printf("\n WELCOME TO ADMIN MENU \n");
        printf("\n");
        printf("1.add train\n");
        printf("2.view all train\n");
        printf("3.delete train\n");
        printf("4.back to main menu\n");
        printf("enter your choice:");
        scanf("%d", &adminchoice);

        switch(adminchoice) {
            case 1:
                addTrain();
                break;
            case 2:
                viewTrains();
                break;
            case 3:
                deleteTrain();
                break;
            case 4:
                printf("returning to main menu:\n");
                return;
            default:
                printf("\nInvalid choice. Please select 1-4.\n");
        }
    }
}

// function for admin section--------------------------------------------
void addTrain() {
    printf("\n ------add new train:------\n");

    if(trainCount >= 20) {
        printf("maximum train capacity reached!\n");
        return;
    }

    printf("Enter Train Number: ");
    scanf("%d", &trains[trainCount].trainNumber);

    printf("Enter Train Name: ");
    scanf("%s", trains[trainCount].trainName);

    printf("Enter Source Station: ");
    scanf("%s", trains[trainCount].source);

    printf("Enter Destination Station: ");
    scanf("%s", trains[trainCount].destination);

    printf("Enter Total Seats: ");
    scanf("%d", &trains[trainCount].totalSeats);

    printf("Enter Price per ticket: ");
    scanf("%d", &trains[trainCount].pricePerticket);

    trains[trainCount].availableSeats = trains[trainCount].totalSeats;
    trainCount++;

    printf("\n added train successfully\n ");
}

void viewTrains() {
    printf("\n ----view all trains-----------\n");
    if(trainCount == 0) {
        printf("no train available right now!!\n");
        return;
    }

    printf("-------------------------------------------------------------------------------------\n");
    printf("train no----train name-----source--------destination----------seats---------price---- \n");
    printf("-------------------------------------------------------------------------------------\n");

    for(int i = 0; i < trainCount; i++) {
        printf("%-10d  %-15s  %-15s  %-15s  %3d/%-3d  %-10d\n",
            trains[i].trainNumber,
            trains[i].trainName,
            trains[i].source,
            trains[i].destination,
            trains[i].availableSeats,
            trains[i].totalSeats,
            trains[i].pricePerticket);
    }
}

void deleteTrain() {
    if(trainCount == 0) {
        printf("there are not available train to delete\n");
        return;
    }

    int trainNo;
    printf("\nEnter Train Number to delete: ");
    scanf("%d", &trainNo);

    for(int i = 0; i < trainCount; i++) {
        if(trains[i].trainNumber == trainNo) {
            for(int j = i; j < trainCount - 1; j++) {
                trains[j] = trains[j + 1];
            }
            trainCount--;
            printf("\nTrain %d deleted successfully!\n", trainNo);
            return;
        }
    }
    printf("\nTrain not found with number: %d\n", trainNo);
}

//---------------------------------------user page----------------------------------------------------------------------
void usermanu() {
    int userchoice;
    while(1) {
        printf("\n user menu\n");
        printf("1.view available train\n");
        printf("2.book train\n");
        printf("3.View my bookings\n");
        printf("4.cancel booking\n");
        printf("5.back to main menu\n");
        printf("enter a choice:");
        scanf("%d", &userchoice);

        switch(userchoice) {
            case 1:
                viewTrains();
                break;
            case 2:
                bookTrain();
                break;
            case 3:
                viewBookings();
                break;
            case 4:
                cancelBooking();
                break;
            case 5:
                return;
            default:
                printf("\n 404 page not found...Please select 1-5.\n");
        }
    }
}

//-----------------------functions for user-------------------------------------
void bookTrain() {
    if(trainCount == 0) {
        printf("\nNo trains available for booking!\n");
        return;
    }

    printf("\n--- Available Trains ---\n");
    viewTrains();

    int trainNo, seats;
    printf("\nEnter Train Number: ");
    scanf("%d", &trainNo);

    int found = -1;
    for(int i = 0; i < trainCount; i++) {
        if(trains[i].trainNumber == trainNo) {
            found = i;
            break;
        }
    }

    if(found == -1) {
        printf("Train not found!\n");
        return;
    }

    if(trains[found].availableSeats <= 0) {
        printf("Train is fully booked!\n");
        return;
    }

    printf("Available seats: %d\n", trains[found].availableSeats);
    printf("Enter seats to book: ");
    scanf("%d", &seats);

    if(seats <= 0 || seats > trains[found].availableSeats) {
        printf("Invalid seat count!\n");
        return;
    }

    int totalPrice = seats * trains[found].pricePerticket; // calculating total price = pricecc per ticket * seats

    //--------------------------------- Create a temporary booking--------------------------------------------------------
    struct Booking tempBooking;
    tempBooking.trainNumber = trainNo;
    tempBooking.bookedSeats = seats;
    tempBooking.totalprice = totalPrice;
    strcpy(tempBooking.paymentStatus, "Pending");
    generateTransactionID(tempBooking.transactionID);

    printf("\nBOOKING SUMMARY:\n");
    printf("Train: %d (%s)\n", trains[found].trainNumber, trains[found].trainName);
    printf("Seats: %d\n", seats);
    printf("Price per ticket: tk%d\n", trains[found].pricePerticket);
    printf("Total Price: tk%d\n", totalPrice);
    printf("From: %s to %s\n", trains[found].source, trains[found].destination);
    printf("\nTransaction ID: %s\n", tempBooking.transactionID);


    // -----------------------------------------Process payment----------------------------------------------------------------
    printf("\nProceeding to payment...\n");
    showPaymentOptions(totalPrice);

    // Add to bookings only if payment is successful
    printf("\nProcessing payment for booking...\n");
    if(bookingCount < 100) {
        bookings[bookingCount] = tempBooking;
        processPayment(bookingCount);
        bookingCount++;
    } else {
        printf("Maximum bookings reached!\n");
    }
}
    //-------------------------------------------------------------------------------------------------------------------------

    void processPayment(int bookingIndex) {
    int paymentSuccess = 0;
    int choice;

    printf("\n--- Payment Gateway ---\n");
    printf("Amount to pay: tk%d\n", bookings[bookingIndex].totalprice);
    printf("Transaction ID: %s\n", bookings[bookingIndex].transactionID);

    printf("\nSelect payment method:\n");
    printf("1. Credit/Debit Card\n");
    printf("2. Mobile Banking\n");
    printf("3. Cancel Booking\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1: // just call respected function to perform
            paymentSuccess = processCardPayment(bookings[bookingIndex].totalprice);
            break;
        case 2:
            paymentSuccess = processMobileBanking(bookings[bookingIndex].totalprice);
            break;
        case 3:
            printf("Booking cancelled by user.\n");
            strcpy(bookings[bookingIndex].paymentStatus, "Cancelled");
            return;
        default:
            printf("Invalid choice. Payment failed.\n");
    }

    if(paymentSuccess) {
        strcpy(bookings[bookingIndex].paymentStatus, "Completed");
        printf("\nPayment successful! Booking confirmed.\n");

        // Update available seats
        for(int i = 0; i < trainCount; i++) {
            if(trains[i].trainNumber == bookings[bookingIndex].trainNumber) {
                trains[i].availableSeats -= bookings[bookingIndex].bookedSeats;
                break;
            }
        }
    } else {
        strcpy(bookings[bookingIndex].paymentStatus, "Failed");
        printf("\nPayment failed! Booking not confirmed.\n");
    }
}






void showPaymentOptions(int amount) {
    printf("\n--- Payment Options ---\n");
    printf("Total Amount: tk%d\n", amount);
    printf("1. Credit/Debit Card\n");
    printf("2. Mobile Banking (bKash/Nagad/Rocket)\n");
    printf("3. Cancel\n");
    printf("Enter your choice: ");
}

int processCardPayment(int amount) {
    char cardNumber[20];
    char expiryDate[10];
    char cvv[5];

    printf("\n--- Card Payment ---\n");
    printf("Amount: tk%d\n", amount);
    printf("Enter card number (16 digits): ");
    scanf("%s", cardNumber);

    if(strlen(cardNumber) != 16) {
        printf("Invalid card number!\n");
        return 0;
    }

    printf("Enter expiry date (MM/YY): ");
    scanf("%s", expiryDate);

    printf("Enter CVV: ");
    scanf("%s", cvv);

    // Simulate payment processing
    printf("\nProcessing payment...\n");
    printf("Connecting to bank...\n");


    //pausing work for 2 sec
        sleep(2);


    // Simulate network delay

    // Random success/failure for simulation
    srand(time(0));
    int success = rand() % 2;

    if(success) {
        printf("Payment approved!\n");
        printf("Transaction completed for tk%d\n", amount);
        return 1;
    } else {
        printf("Payment declined by bank!\n");
        return 0;
    }
}

int processMobileBanking(int amount) {
    char mobileNumber[12];
    char pin[5];
    int provider;

    printf("\n--- Mobile Banking ---\n");
    printf("Amount: tk%d\n", amount);
    printf("Select provider:\n");
    printf("1. bKash\n");
    printf("2. Nagad\n");
    printf("3. Rocket\n");
    printf("Enter choice: ");
    scanf("%d", &provider);

    printf("Enter mobile number: ");
    scanf("%s", mobileNumber);

    printf("Enter PIN: ");
    scanf("%s", pin);

    // Simulate payment processing
    printf("\nProcessing payment...\n");
    printf("Sending OTP...\n");
    sleep(2); // Simulate network delay

    char otp[5];
    printf("Enter OTP received on your phone: ");
    scanf("%s", otp);

    // Random success/failure for simulation
    srand(time(0));
    int success = rand() % 2;

    if(success) {
        printf("Payment successful!\n");
        printf("Transaction completed for tk%d\n", amount);
        return 1;
    } else {
        printf("Payment failed! Invalid OTP or insufficient balance.\n");
        return 0;
    }
}
//--------------------------------------------trans id-------------------------------------------------
void generateTransactionID(char* id) {
    srand(time(0));
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for(int i = 0; i < 10; i++) {
        if(i == 3 || i == 7) {
            id[i] = '-';
        } else {
            int index = rand() % (sizeof(charset) - 1);
            id[i] = charset[index];
        }
    }
    id[10] = '\0';
}
//------------------------------------------------------------------------------
void cancelBooking() {
    if (bookingCount == 0) {
        printf("\nNo bookings to cancel!\n");
        return;
    }

    printf("\n--- Your Bookings ---\n");
    for (int i = 0; i < bookingCount; i++) {
        printf("%d. Train No: %d, Seats: %d, Status: %s\n",
               i + 1,
               bookings[i].trainNumber,
               bookings[i].bookedSeats,
               bookings[i].paymentStatus);
    }

    int choice;
    printf("\nEnter booking number to cancel (0 to cancel): ");
    scanf("%d", &choice);

    if (choice == 0) {
        return;
    }

    if (choice < 1 || choice > bookingCount) {
        printf("Invalid booking number!\n");
        return;
    }

    //---------------------------------Check if payment was completed
    if(strcmp(bookings[choice - 1].paymentStatus, "Completed") == 0) {
        printf("\nThis booking was paid for. Initiating refund process...\n");
        printf("Refund of tk%d will be processed to your original payment method.\n",
               bookings[choice - 1].totalprice);
    }

    int trainNo = bookings[choice - 1].trainNumber;
    int seats = bookings[choice - 1].bookedSeats;

    for (int i = 0; i < trainCount; i++) {
        if (trains[i].trainNumber == trainNo) {
            trains[i].availableSeats += seats;
            break;
        }
    }

    for (int i = choice - 1; i < bookingCount - 1; i++) {
        bookings[i] = bookings[i + 1];
    }
    bookingCount--;

    printf("\nBooking cancelled successfully!\n");
}

void viewBookings() {
    if (bookingCount == 0) {
        printf("\nNo bookings found!\n");
        return;
    }

    printf("\n--- All Bookings ---\n");
    printf("No. | Train No. | Seats Booked | Total Price | Payment Status | Transaction ID\n");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < bookingCount; i++) {
        printf("%-3d | %-9d | %-12d | %-11d | %-14s | %s\n",
               i + 1,
               bookings[i].trainNumber,
               bookings[i].bookedSeats,
               bookings[i].totalprice,
               bookings[i].paymentStatus,
               bookings[i].transactionID);
    }
}
