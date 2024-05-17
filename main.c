#include "sysfun.h"

#define ADMIN_NAME "David"
#define ADMIN_PASSWORD 11111111

extern Account accounts[MAX_ACCOUNTS];
extern int accountCount;
extern int currentID;


int main() {
    int temp, temp2, counterADM = 0, counterADM2 = 0;
    char name[20], tempNumber[11];
    int type;
    int password, tempPassword;
    float sum = 0;
    float lim = 0, bal1, bal2;
    accountCount = 0;
    Account *founded;
    readFromDatabase();

    currentID = -1;

    do {
        printf("Menu:\n 1 - Create account\n 2 - Log in account\n");
        scanf("%d", &temp);
    } while (temp != 1 && temp != 2);
    if(temp == 1){
        printf("\nEnter your name (only using A-Za-z): ");
        scanf("%s", name);
        if (!validateName(name)) {
            printf("Invalid name. Please use only A-Za-z.\n");
            return 1;
        }

        printf("\nEnter your password (8 digits, only using 0-9): ");
        scanf("%d", &password);
        if (!validatePassword(password)) {
            printf("Invalid password. Please enter an 8-digit number.\n");
            return 1;
        }

        printf("\nEnter account type: 1 - credit, 2 - debit: \n");
        scanf("%d", &type);
        if (!validateType(&type)) {
            printf("Invalid type. Please enter 1 for credit or 2 for debit.\n");
            return 1;
        }

        createAccount(name, password, type);

    } else if(temp == 2){
        printf("\nEnter your name (only using A-Za-z): ");
        scanf("%s", name);
        if (!validateName(name)) {
            printf("Invalid name. Please use only A-Za-z.\n");
            return 1;
        }

        printf("\nEnter your password (8 digits, only using 0-9): ");
        scanf("%d", &password);
        if (!validatePassword(password)) {
            printf("Invalid password. Please enter an 8-digit number.\n");
            return 1;
        }
        if((strcmp(name, ADMIN_NAME) != 0) || password != ADMIN_PASSWORD){
            if(searchByNameAndPassword(name, password)){
                printf("Hello, %s !\n", name);
                printf("Choose the option:\n 1. Make a transfer.\n 2. Check the balance\n 3. Exit from the app\n");
                scanf(" %d", &temp2);
                switch (temp2) {
                    case 1:
                        printf("\nEnter your password: ");
                        scanf("%d", &tempPassword);
                        printf("\nEnter the number of account where you want to send money: ");
                        scanf("%s", tempNumber);
                        if(tempPassword == password && searchByNumber(tempNumber)){
                            printf("\nEnter the sum: ");
                            scanf("%f", &sum);
                            if(sum > 0){
                                if(makeTransfer(sum)){
                                    printf("Transaction made successfully\n");
                                }
                            } else {
                                printf("Wrong sum!");
                                return 1;
                            }
                        }
                        break;
                    case 2:
                        if(accounts[currentID].type == CREDIT){
                            printf("Current balance: %.2f\n", accounts[currentID].details.creditDetails.balance);
                        } else if(accounts[currentID].type == DEBIT){
                            printf("Current balance: %.2f\n", accounts[currentID].details.debitBalance);
                        } else {
                            printf("Unknown account type\n");
                            return 1;
                        }
                        break;
                    case 3:
                        printf("\nThank you for using our app!");
                        break;
                    default:
                        printf("Wrong option!");
                        return 1;
                }
            } else{
                printf("Account does not exist.");
                return 1;
            }
        } else {
            printf("Choose the option:\n 1. Find all by account type.\n 2. Find by enum and union.\n 3. Find all by balance.\n");
            scanf("%d", &temp2);
            if(temp2 == 1){
                printf("Choose account type: \n 1. Credit\n 2. Debit\n");
                scanf("%d", &temp2);
                if(temp2 == 1){
                    founded = searchByType(CREDIT, &counterADM);
                    if (founded != NULL) {
                        printArray(founded, counterADM);
                        printf("%d", counterADM);
                        printf("%s", founded[0].name);
                    } else {
                        printf("No accounts found of type CREDIT\n");
                    }
                } else if(temp2 == 2){
                    founded = searchByType(DEBIT, &counterADM);
                    if(founded != NULL){
                        printArray(founded, counterADM);
                    } else {
                        printf("No accounts found of type DEBIT\n");
                    }
                } else {
                    printf("Invalid option!");
                    return 1;
                }
            } else if(temp2 == 2){
                printf("Choose account type: \n 1. Credit\n 2. Debit\n");
                scanf(" %d", &temp2);
                if(temp2 == 1){
                    printf("Enter limit size:\n");
                    scanf(" %f", &lim);
                    if(lim >= 0){
                        founded = searchByType(CREDIT, &counterADM);
                        founded = searchByLimit(founded, lim, counterADM, &counterADM2);
                        printArray(founded, counterADM2);
                    }
                } else if(temp2 == 2){
                    printf("Enter balance size:\n");
                    scanf(" %f", &lim);
                    if(lim >= 0){
                        founded = searchByType(DEBIT, &counterADM);
                        founded = searchByBalance(founded, lim, counterADM, &counterADM2);
                        printArray(founded, counterADM2);
                    }
                } else {
                    printf("Invalid option!");
                    return 1;
                }
            } else if(temp2 == 3){
                printf("\nEnter minimum value of the balance: ");
                scanf("%f", &bal1);
                printf("\nEnter maximum value of the balance: ");
                scanf("%f", &bal2);
                if(bal1 <= bal2){
                    founded = searchByRangeOfBalance(accounts, bal1, bal2, accountCount, &counterADM);
                    printArray(founded, counterADM);
                } else {
                    printf("Invalid values!");
                    return 1;
                }
            }
        }

    }
    return 0;
}

/*Menu:
 1 - Create account
 2 - Log in account
2

Enter your name (only using A-Za-z):Klark

Enter your password (8 digits, only using 0-9):86736486
 Hello, Klark !
Choose the option:
 1. Make a transfer.
 2. Check the balance
 3. Exit from the app
1

Enter your password:86736486

Enter the number of account where you want to send money:7945391407

Enter the sum:10
 Transaction made successfully*/