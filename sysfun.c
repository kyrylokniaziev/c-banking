#include "sysfun.h"

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;
int currentID = 0;
int transferPersonID = 0;

void writeToDatabase(Account account) {
    FILE *file = fopen("database.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%s/%s/%d/", account.name, account.number, account.password);

    if (account.type == CREDIT) {
        fprintf(file, "CREDIT/%.2f/%.2f+\n", account.details.creditDetails.balance, account.details.creditDetails.limit);
    } else {
        fprintf(file, "DEBIT/%.2f+\n", account.details.debitBalance);
    }
    fflush(file);
    fclose(file);
}

void readFromDatabase() {
    FILE *file = fopen("database.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Account newAccount;
        char type[10];
        char limit[15];
        char balance[15];

        sscanf(line, "%[^/]/%[^/]/%d/%[^/]/%[^/]/%[^+]",
               newAccount.name,
               newAccount.number,
               &newAccount.password,
               type,
               balance,
               limit);

        if (strcmp(type, "CREDIT") == 0) {
            newAccount.type = CREDIT;
            sscanf(balance, "%f", &newAccount.details.creditDetails.balance);
            sscanf(limit, "%f", &newAccount.details.creditDetails.limit);
        } else {
            newAccount.type = DEBIT;
            sscanf(balance, "%f", &newAccount.details.debitBalance);
        }

        accounts[accountCount++] = newAccount;
    }

    fclose(file);
}

bool checkIfAccountExists(char *number) {
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].number, number) == 0) {
            return true;
        }
    }
    return false;
}

void createAccount(char *name, int password, enum Type type) {
    srand(time(NULL));
    Account newAccount;
    strcpy(newAccount.name, name);
    newAccount.password = password;
    newAccount.type = type;

    if (type == CREDIT) {
        newAccount.details.creditDetails.balance = 0;
        newAccount.details.creditDetails.limit = 30000;
    } else {
        newAccount.details.debitBalance = 0;
    }

    do {
        for (int i = 0; i < 10; i++) {
            newAccount.number[i] = '0' + rand() % 10;
        }
        newAccount.number[10] = '\0';
    } while (checkIfAccountExists(newAccount.number));

    accounts[accountCount++] = newAccount;
    writeToDatabase(newAccount);
}

// Функция проверки имени пользователя
bool validateName(const char *name) {
    // Проверяем, что все символы - латинские буквы
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i])) {
            return false;
        }
    }
    return true;
}

// Функция проверки пароля
bool validatePassword(int password) {
    // Проверяем, что пароль состоит из 8 цифр
    int count = 0;
    while (password > 0) {
        count++;
        password /= 10;
    }
    return count == 8;
}


// Функция проверки типа аккаунта
bool validateType(int* type) {
    // Проверяем, что тип аккаунта - 1 или 2
    if(*type == 1){
        *type = CREDIT;
        return true;
    } else if(*type == 2){
        *type = DEBIT;
        return true;
    } else {
        return false;
    }
}

bool searchByNameAndPassword(char *name, int password){
    for (int i = 0; i < accountCount; i++){
        if (strcmp(accounts[i].name, name) == 0) {
            if(accounts[i].password == password){
                currentID = i;
                return true;
            }
        }
    }
    return false;
}

bool searchByNumber(char *number){
    for (int i = 0; i < accountCount; i++){
        if (strcmp(accounts[i].number, number) == 0) {
            transferPersonID = i;
            return true;
        }
    }
    return false;
}

void updateDatabase(){
    remove("database.txt");
    for(int i = 0; i < accountCount; i++){
        writeToDatabase(accounts[i]);
    }
}

void printArray(Account *array, const int counter){
    int i = 0;
    while (i < counter){
        printf("\n%s", array[i].name);
        printf(" %u ", array[i].type);
        printf(" %s ", array[i].number);
        printf(" %d ", array[i].password);
        if(array[i].type == CREDIT) {
            printf(" %.2f ", array[i].details.creditDetails.balance);
            printf(" %.2f ", array[i].details.creditDetails.limit);
        } else {
            printf(" %.2f ", array[i].details.debitBalance);
        }
        i++;
    }
}

bool makeTransfer(float sum){
    if(accounts[currentID].type == CREDIT && accounts[transferPersonID].type == CREDIT){
        if((accounts[currentID].details.creditDetails.balance +
            accounts[currentID].details.creditDetails.limit - sum) >= 0){
            accounts[currentID].details.creditDetails.balance -= sum;
            accounts[transferPersonID].details.creditDetails.balance += sum;
            updateDatabase();
            return true;
        }
    } else if(accounts[currentID].type == CREDIT && accounts[transferPersonID].type == DEBIT){
        if((accounts[currentID].details.creditDetails.balance +
            accounts[currentID].details.creditDetails.limit - sum) >= 0){
            accounts[currentID].details.creditDetails.balance -= sum;
            accounts[transferPersonID].details.debitBalance += sum;
            updateDatabase();
            return true;
        }
    } else if(accounts[currentID].type == DEBIT && accounts[transferPersonID].type == CREDIT){
        if((accounts[currentID].details.debitBalance - sum) >= 0){
            accounts[currentID].details.debitBalance -= sum;
            accounts[transferPersonID].details.creditDetails.balance += sum;
            updateDatabase();
            return true;
        }
    } else if(accounts[currentID].type == DEBIT && accounts[transferPersonID].type == DEBIT){
        if((accounts[currentID].details.debitBalance - sum) >= 0){
            accounts[currentID].details.debitBalance -= sum;
            accounts[transferPersonID].details.debitBalance += sum;
            updateDatabase();
            return true;
        }
    }

    return false;
}

Account* searchByType(enum Type type, int *counter) {
    Account *foundAccounts = (Account*) calloc(accountCount,sizeof(Account));
    *counter = 0;

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].type == type) {
            foundAccounts[*counter] = accounts[i];
            (*counter)++;
        }
    }
    foundAccounts = (Account*)realloc(foundAccounts, (*counter) * sizeof(Account));
    return foundAccounts;
}

Account* searchByLimit(Account *array, float limit, const int counter1, int* counter2){
    Account *foundAccounts = (Account*) malloc(sizeof(Account) * counter1);
    *(counter2) = 0;
    for(int i = 0; i < counter1; i++){
        if(array[i].details.creditDetails.limit == limit){
            foundAccounts[*counter2] = array[i];
            (*counter2)++;
        }
    }
    foundAccounts = realloc(foundAccounts, (*counter2) * sizeof(Account));
    return foundAccounts;
}

Account* searchByBalance(Account *array, float balance, const int counter1, int* counter2){
    Account *foundAccounts = (Account*) malloc(sizeof(Account) * counter1);
    *(counter2) = 0;
    for(int i = 0; i < counter1; i++){
        if(array[i].details.debitBalance == balance){
            foundAccounts[*counter2] = array[i];
            (*counter2)++;
        }
    }
    foundAccounts = realloc(foundAccounts, (*counter2) * sizeof(Account));
    return foundAccounts;
}

Account* searchByRangeOfBalance(Account *array, float balance1, float balance2, const int counter1, int* counter2){
    Account *foundAccounts = (Account*) malloc(sizeof(Account) * counter1);
    *(counter2) = 0;
    for(int i = 0; i < counter1; i++){
        if(array[i].type == CREDIT){
            if((array[i].details.creditDetails.balance >= balance1) && (array[i].details.creditDetails.balance <= balance2)){
                foundAccounts[*counter2] = array[i];
                (*counter2)++;
            }
        } else if(array[i].type == DEBIT){
            if((array[i].details.debitBalance >= balance1) && (array[i].details.debitBalance <= balance2)){
                foundAccounts[*counter2] = array[i];
                (*counter2)++;
            }
        }
    }
    foundAccounts = realloc(foundAccounts, (*counter2) * sizeof(Account));
    return foundAccounts;
}
