#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NAME "accounts.txt"
#define ACC 100 

struct account {
    char accNo[6];
    char PIN[5];
    int year;
    int month;
    int day;
    float initBal;
    float totalDeposit;
    float totalWithdraw;
};

int i = 0;

void createAccount() {
    struct account newAcc;
    
    printf("\n\tEnter 5-digit account number: ");
    scanf("%5s", newAcc.accNo);

    printf("\tEnter 4-digit PIN: ");
    scanf("%4s", newAcc.PIN);

    printf("\tEnter initial balance: ");
    scanf("%f", &newAcc.initBal);

    newAcc.totalDeposit = 0.0;
    newAcc.totalWithdraw = 0.0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("\nAccount created on: %d-%02d-%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    FILE *fp;
    fp = fopen(FILE_NAME, "a"); // Open the file in "append" mode instead of "write" mode

    if (fp == NULL) {
        printf("\n\t\tError opening file for writing.\n");
        return;
    }

    fprintf(fp, "%s\t%s\t%d-%02d-%02d\t%.2f\t%.2f\t%.2f\n", newAcc.accNo, newAcc.PIN,
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, newAcc.totalDeposit, newAcc.totalWithdraw, newAcc.initBal);

    fclose(fp);

    printf("\nAccount created and saved successfully!\n");
}

int authenticateUser(const char *accountNo, const char *pin) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("\t\tError opening file for reading.\n");
        return 0;
    }

    char buffer[100]; // Serves as the temporary reader of the txt file
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char savedAccNo[6];
        char savedPIN[5];
        float bal, totalDeposit, totalWithdraw;
        int year, month, day;
        sscanf(buffer, "%5s %4s %d-%d-%d %f %f %f", savedAccNo, savedPIN,
               &year, &month, &day, &totalDeposit, &totalWithdraw, &bal);

        if (strcmp(accountNo, savedAccNo) == 0 && 
            strcmp(pin, savedPIN) == 0) {
            fclose(fp);
            return 1; // Successful
        }
    }

    fclose(fp);
    return 0; // Unsuccessful
}

float getBal(const char *accountNo) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("\t\tError opening file for reading.\n");
        return -1.0;
    }

    char buffer[100]; // Serves as the temporary reader of the txt file
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char savedAccNo[6];
        char savedPIN[5];
        float bal, totalDeposit, totalWithdraw;
        int year, month, day;
        sscanf(buffer, "%5s %4s %d-%d-%d %f %f %f", savedAccNo, savedPIN,
               &year, &month, &day, &totalDeposit, &totalWithdraw, &bal);

        if (strcmp(accountNo, savedAccNo) == 0) {
            fclose(fp);
            return bal; // Successful
        }
    }

    fclose(fp);
    return -1.0; // Account not found
}

void updateBal(const char *accountNo, const char *pin, float newBal, float depositAmt, float withdrawalAmt) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("\t\tError opening file for reading.\n");
        return;
    }

    struct account newAcc[ACC];
    int ind = 0;
    char buffer[100]; // Serves as the temporary reader of the txt file
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int year, month, day;
        sscanf(buffer, "%5s %4s %d-%d-%d %f %f %f", newAcc[ind].accNo, newAcc[ind].PIN,
               &year, &month, &day, &newAcc[ind].totalDeposit, &newAcc[ind].totalWithdraw, &newAcc[ind].initBal);

        if (strcmp(accountNo, newAcc[ind].accNo) == 0 &&
            strcmp(pin, newAcc[ind].PIN) == 0) {
            newAcc[ind].initBal = newBal;
            newAcc[ind].totalDeposit += depositAmt;
            newAcc[ind].totalWithdraw += withdrawalAmt;
        }
        ind++;
    }

    fclose(fp);

    fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("\t\tError opening file for writing\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    for (int i = 0; i < ind; i++) {
        fprintf(fp, "%s\t%s\t%d-%02d-%02d\t%.2f\t%.2f\t%.2f\n", newAcc[i].accNo, newAcc[i].PIN,
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, newAcc[i].totalDeposit, newAcc[i].totalWithdraw, newAcc[i].initBal);
    }

    fclose(fp);
}

void deleteAccount(const char *accountNo, const char *pin) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("\t\tError opening file for reading.\n");
        return;
    }

    struct account newAcc[ACC];
    int ind = 0;
    char buffer[100]; // Serves as the temporary reader of the txt file
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int year, month, day;
        sscanf(buffer, "%5s %4s %d-%d-%d %f %f %f", newAcc[ind].accNo, newAcc[ind].PIN,
               &year, &month, &day, &newAcc[ind].totalDeposit, &newAcc[ind].totalWithdraw, &newAcc[ind].initBal);

        if (strcmp(accountNo, newAcc[ind].accNo) == 0 &&
            strcmp(pin, newAcc[ind].PIN) == 0) {
            // If account found, skip saving it to effectively delete it from the file
        } else {
            ind++;
        }
    }

    fclose(fp);

    fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("\t\tError opening file for writing\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    for (int i = 0; i < ind; i++) {
        fprintf(fp, "%s\t%s\t%d-%02d-%02d\t%.2f\t%.2f\t%.2f\n", newAcc[i].accNo, newAcc[i].PIN,
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, newAcc[i].totalDeposit, newAcc[i].totalWithdraw, newAcc[i].initBal);
    }

    fclose(fp);
}

int isFileEmpty() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("\t\tError opening file for reading.\n");
        return 1; // Treat the file as empty if there's an error opening it
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        fclose(fp);
        return 0; // File is not empty
    }

    fclose(fp);
    return 1; // File is empty
}

int again() {
    char repeat;

    while (!(repeat == 'N' || repeat == 'n' || repeat == 'Y' || repeat == 'y')) {
        printf("\nWould you like to do another transaction:\n\t[Y]es\t\t[N]o\n\t\t= ");
        scanf(" %c", &repeat);

        if (repeat == 'N' || repeat == 'n') {
            printf("\n\tThank you for using PUP OnLine Banking Systems!\n\n");
            return 0;
        } else if (repeat == 'Y' || repeat == 'y') {
            return 1;
        } else {
            printf("\n\t\tError: Invalid choice. Please try again\n");
        }
    }
}

int exitMenu() {
    char repeat;

    while (!(repeat == 'N' || repeat == 'n' || repeat == 'Y' || repeat == 'y')) {
        printf("\nDo you confirm the exit:\n[Y]es\t\t[N]o\n\t= ");
        scanf(" %c", &repeat);

        if (repeat == 'Y' || repeat == 'y') {
            printf("\n\tThank you for using PUP OnLine Banking Systems!\n\n");
            return 0;
        } else if (repeat == 'N' || repeat == 'n') {
            return 1;
        } else {
            printf("\n\t\tError: Invalid choice. Please try again\n");
        }
    }
}

int main(int argc, char *argv[]) {

    int choice;
    char accountNo[6];
    char pin[5];
    int repeat = 1;

    if (isFileEmpty()) {
        printf("\n\t\tNo accounts found. Creating a new account...\n");
        do {
            createAccount();
            printf("\nDo you want to create another account? (Y/N): ");
            scanf(" %c", &repeat);
            i++;
        } while (repeat == 'Y' || repeat == 'y');

        printf("\n\t\tRun the program again to use the ATM.\n\n");
        return 0;
    }

    printf("\nEnter account number (5-digit): ");
    scanf("%5s", accountNo);

    printf("Enter PIN (4-digit): ");
    scanf("%4s", pin);

while (repeat == 1) {

    char accountNoConfirm[6]; 
    char pinConfirm[5]; 
    float balance;
    float depositAmt;
    float withdrawalAmt;

    if (authenticateUser(accountNo, pin)) {

        printf("\n\n===\tWelcome to PUP OnLine Banking Systems\t===\n");
        printf("\n");
        printf("1. Create bank accounts\n2. Read an existing account\n3. Update an exisiting account\n4. Delete an exisiting account\n5. Exit\n");
        printf("\n\tChoose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                do {
                    createAccount();
                    printf("\nDo you want to create another account? (Y/N): ");
                    scanf(" %c", &repeat);
                    i++;
                } while (repeat == 'Y' || repeat == 'y');

                printf("\n\t\tRun the program again to use the ATM.\n\n");
                break;

            case 2:
                do {
                    printf("\n---\tRead an existing account\t---\n");
                    printf("\n1. Balance Inquiry\n");
                    printf("\t\nChoose an option: ");
                    scanf("%d", &choice);

                    if (choice == 1) {
                        balance = getBal(accountNo);
                            if (balance >= 0) {
                                printf("\nYour balance: %.2f\n", balance);
                                repeat = again();
                            } else {
                                printf("\n\t\tError: Account not found\n");
                            }
                        } else {
                            printf("\n\t\tError: Invalid account number or PIN.\n\n");
                            repeat = again();
                        }
                        break;
                } while (!(choice == 1));
            break;

            case 3:
                do {
                    printf("\n---\tUpdate an existing account\t---\n");
                    printf("\n1. Deposit\n2. Withdraw\n");
                    printf("\t\nChoose an option: ");
                    scanf("%d", &choice);

                    if (choice == 1 || choice == 2) {
                        switch (choice) {
                        case 1:
                            printf("\nEnter deposit amount: ");
                            scanf("%f", &depositAmt);
                            balance = getBal(accountNo);

                            if (balance >= 0) {
                                balance += depositAmt;
                                updateBal(accountNo, pin, balance, depositAmt, 0.0); 
                                printf("\nDeposit successful. Your new balance is P%.2f\n", balance);
                                repeat = again();
                            } else {
                                printf("\n\t\tError: Account not found\n");
                            }
                            break;
                        case 2:
                            printf("\nEnter withdrawal amount: ");
                            scanf("%f", &withdrawalAmt);
                            balance = getBal(accountNo);

                            if (balance >= 0) {
                                if (withdrawalAmt <= balance) {
                                    balance -= withdrawalAmt;
                                    updateBal(accountNo, pin, balance, 0.0, withdrawalAmt);
                                    printf("\nWithdrawal successful. Your new balance is P%.2f\n", balance);
                                    repeat = again();
                                } else {
                                    printf("\n\t\tError: Insufficient balance.\n");
                                }
                            } else {
                                printf("\n\t\tError: Account not found\n");
                            }
                            break;
                        
                        default:
                            printf("\n\tInvalid option.\n");
                        }
                    }
                            
                    else {
                        printf("\n\t\tInvalid input!\n");
                        repeat = again();
                    }
                } while (!(choice == 1 || choice == 2));
                break;

            case 4:
                do {
                    printf("\n---\tDelete an existing account\t---\n");

                    printf("\nEnter account number (5-digit) to confirm deletion: ");
                    scanf("%5s", accountNoConfirm);

                    if (strcmp(accountNoConfirm, accountNo) != 0) {
                        printf("\n\t\tError: Account number confirmation does not match the logged-in account.\n");
                        repeat = again();
                    } 
                    else {
                        printf("Enter PIN (4-digit) to confirm deletion: ");
                        scanf("%4s", pinConfirm);

                        if (strcmp(pinConfirm, pin) != 0) {
                            printf("\n\t\tError: PIN confirmation does not match the logged-in account.\n");
                            repeat = again();
                        } else {
                            deleteAccount(accountNo, pin);
                            printf("\nAccount deleted successfully!\n\n\t\tExiting...\n\n");
                            repeat = 0; // Exit the loop and end the program
                        }
                    }
                
                } while (!(strcmp(accountNoConfirm, accountNo) == 0 && strcmp(pinConfirm, pin) == 0));

            case 5:
                repeat = exitMenu();
                break;
            
            default:
                printf("\n\tInvalid option.\n");
        }
    }
    else {
        printf("\n\t\tInvalid input!\n");
        repeat = again();

        if (repeat == 1) {
            printf("\nEnter account number (5-digit): ");
            scanf("%5s", accountNo);

            printf("Enter PIN (4-digit): ");
            scanf("%4s", pin);
        }
    }
        
    }
    return 0;
}