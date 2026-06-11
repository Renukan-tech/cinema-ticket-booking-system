#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store booking details
typedef struct {
    char seatLabel[5];       // Seat like A1, B3 etc.
    char customerName[50];   // Name of the customer
    int ticketID;            // Auto‑generated ticket number
} Booking;

// Function to clear leftover input from buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);   // Flush input buffer
}

// Display the complete seat layout in a formatted structure
void displaySeats(int rows, int cols, int **seats) {
    printf("\n================ CINEMA HALL ================\n");
    printf("                     SCREEN\n");
    printf("             --------------------\n\n");

    // Print column numbers
    printf("      ");
    for (int j = 0; j < cols; j++)
        printf("%3d", j + 1);
    printf("\n");

    printf("      ");
    for (int j = 0; j < cols; j++)
        printf(" ---");
    printf("\n");

    // Print each row with seat availability (O/X)
    for (int i = 0; i < rows; i++) {
        printf("Row %c |", 'A' + i);

        for (int j = 0; j < cols; j++) {
            if (seats[i][j] == 0)
                printf("  O");   // Available seat
            else
                printf("  X");   // Booked seat
        }
        printf(" |\n");
    }

    printf("      ");
    for (int j = 0; j < cols; j++)
        printf(" ---");
    printf("\n");

    printf("\nO = Available   X = Booked\n");
    printf("=============================================\n\n");
}

// Convert row letter to index (A = 0, B = 1 ...)
int rowCharToIndex(char c) {
    return c - 'A';
}

// Function to book multiple seats under ONE name
void bookSeats(int rows, int cols, int **seats, Booking *bookings, int *count) {
    int n;
    char customerName[50];

    // Ask name only once
    printf("Enter customer name: ");
    fgets(customerName, sizeof(customerName), stdin);
    customerName[strcspn(customerName, "\n")] = '\0';   // Remove newline

    // Number of seats to book
    printf("How many seats do you want to book? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    // Loop for each seat
    for (int k = 0; k < n; k++) {

        char input[10], rowChar;
        int col;

        while (1) {
            printf("Enter seat %d (e.g., B3): ", k + 1);

            if (fgets(input, sizeof(input), stdin) == NULL) 
                continue;

            // Extract row letter and column number
            if (sscanf(input, " %c%d", &rowChar, &col) != 2) {
                printf("Invalid format! Try again.\n");
                continue;
            }

            int i = rowCharToIndex(rowChar);
            int j = col - 1;

            // Check seat validity
            if (i < 0 || i >= rows || j < 0 || j >= cols) {
                printf("Seat out of range! Try again.\n");
                continue;
            }

            // Check if already booked
            if (seats[i][j] == 1) {
                printf("Seat already booked! Choose another.\n");
                continue;
            }

            // Mark seat as booked
            seats[i][j] = 1;

            // Store booking details
            strcpy(bookings[*count].customerName, customerName);
            sprintf(bookings[*count].seatLabel, "%c%d", rowChar, col);
            bookings[*count].ticketID = 1000 + *count;  // Auto Ticket ID

            (*count)++;

            printf("Seat %c%d booked successfully for %s!\n", rowChar, col, customerName);
            break;
        }
    }
}

// Function to cancel a booking using seat number + customer name verification
void cancelBooking(int **seats, int rows, int cols, Booking *bookings, int *count) {
    char seat[5], name[50];

    printf("Enter seat to cancel (e.g., B3): ");
    scanf("%s", seat);
    clearInputBuffer();

    printf("Enter customer name for verification: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    // Search through all bookings
    for (int i = 0; i < *count; i++) {
        if (strcmp(bookings[i].seatLabel, seat) == 0) {

            // Verify name matches
            if (strcmp(bookings[i].customerName, name) == 0) {

                // Convert seat label back into row & column
                int row = rowCharToIndex(seat[0]);
                int col = atoi(seat + 1) - 1;

                seats[row][col] = 0;  // Mark seat as available

                printf("Booking cancelled for %s (Ticket ID: %d)\n",
                       bookings[i].customerName, bookings[i].ticketID);

                // Shift bookings to remove entry
                for (int j = i; j < *count - 1; j++)
                    bookings[j] = bookings[j + 1];

                (*count)--;
                return;
            } else {
                printf("Name does NOT match! Cannot cancel.\n");
                return;
            }
        }
    }

    printf("No booking found for that seat.\n");
}

int main() {
    int rows = 5, cols = 6;
    int **seats;
    Booking bookings[100];
    int count = 0;

    // Allocate 2D array dynamically
    seats = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        seats[i] = (int *)malloc(cols * sizeof(int));

    // Initialize all seats to 0 (available)
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            seats[i][j] = 0;

    int choice;

    // Menu loop
    do {
        printf("\n=== Cinema Ticket Booking System ===\n");
        printf("1. Display Seats\n");
        printf("2. Book Seats\n");
        printf("3. Cancel Booking\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        // Validate input
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = 0;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: displaySeats(rows, cols, seats); break;
            case 2: bookSeats(rows, cols, seats, bookings, &count); break;
            case 3: cancelBooking(seats, rows, cols, bookings, &count); break;
            case 4: printf("Exiting...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }

    } while (choice != 4);

    // Free dynamic memory
    for (int i = 0; i < rows; i++)
        free(seats[i]);
    free(seats);

    return 0;
}