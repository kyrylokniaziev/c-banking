#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define MAX_ACCOUNTS 100

//enum for account types
enum Type {
    CREDIT,
    DEBIT
};

//union for account details
union Details {
    struct {
        float balance;
        float limit;
    } creditDetails;

    float debitBalance;
};

// Account structure
typedef struct Account {
    char name[20];
    char number[11];
    int password;
    enum Type type;
    union Details details;
} Account;

extern Account accounts[MAX_ACCOUNTS];
extern int accountCount;
extern int currentID;
extern int transferPersonID;

void writeToDatabase(Account account);
void readFromDatabase();
bool checkIfAccountExists(char *number);
void createAccount(char *name, int password, enum Type type);
bool validateName(const char *name);
bool validatePassword(int password);
bool validateType(int* type);
bool searchByNameAndPassword(char *name, int password);
bool searchByNumber(char *number);
void updateDatabase();
void printArray(Account *array, int counter);

bool makeTransfer(float sum);
Account* searchByType(enum Type type, int *counter);
Account* searchByLimit(Account *array, float limit, int counter1, int* counter2);
Account* searchByBalance(Account *array, float balance, int counter1, int* counter2);
Account* searchByRangeOfBalance(Account *array, float balance1, float balance2, int counter1, int* counter2);