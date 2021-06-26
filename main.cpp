#include <iostream>
#include <vector>
#include <random>
#include <fstream>

//GLOBALS:
const char CMD_FILL = '+';
const char CMD_GET = '-';
const int RATES[] = {100, 200, 500, 1000, 2000, 5000};
const int MAX_BILLS_QUANTITY = 1000;
const std::string BANK_PATH = "..\\bank.txt";
//=======================================================

/*
 * Fills the bills vector with random bills.
 */
void fillTheBank(std::vector<int>& bills)
{
    for (int i = 0; i < MAX_BILLS_QUANTITY; i++)
    {
        int bill = RATES[random() % 6];
        bills.push_back(bill);
    }
    std::cout << "Bills are generated.\n";
}

/*
 * Bubble sort of bills vector. Need for appropriate choose of bills combination for extracting.
 */
void sortTheBank(std::vector<int>& v)
{
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v.size() - i - 1; j++)
        {
            if (v[j] < v[j + 1])
            {
                int temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
}

/*
 * Give money to user. Checks if there enough appropriate bills in the bank vector.
 * Extracts chosen bills from bills vector:
 * - printing their rates and setting them to '0' in bills vector.
 */
void extractMoney(int amount, std::vector<int>& bills)
{
    int amountToCheck = amount;
    for (int bill : bills)
    {
        if (bill <= amountToCheck) {
            amountToCheck -= bill;
        }
    }
    if (amountToCheck != 0)
    {
        std::cout << "Not enough appropriate bills in the bank.\n";
        return;
    }

    std::cout << "Your cash:\n";
    for (int i = 0; i < bills.size() && amount > 0; i++)
    {
        if (bills[i] <= amount && bills[i] > 0)
        {
            std::cout << "\t " << bills[i] << "\n";
            amount -= bills[i];
            bills[i] = 0;
        }
    }
    std::cout << "Done. Check sum is OK.\n";
}

/*
 * Read bank file to bills vector.
 */
void readBank(std::vector<int>& bills)
{
    bills.clear();
    std::ifstream inputStream(BANK_PATH, std::ios::binary);
    if (inputStream.is_open())
    {
        while (!inputStream.eof())
        {
            int bill = 0;
            inputStream.read((char*) &bill, sizeof(bill));
            bills.push_back(bill);
        }
        inputStream.close();
    }
    else
    {
        std::cerr << "File read error\n";
    }
    std::cout << "Bills are successfully read from bank file.\n";
}

/*
 * Write bills vector to bank file.
 */
void saveBank(std::vector<int>& bills)
{
    std::ofstream outputStream (BANK_PATH, std::ios::binary);
    if (outputStream.is_open())
    {
        for (int bill : bills)
        {
            outputStream.write((char*)&bill, sizeof(bill));
        }
        outputStream.close();
    }
    else
    {
        std::cerr << "File read error!\n";
    }
    std::cout << "Bills are successfully saved to bank file.\n";
}

/*
 * Get <int> amount from std::cin with input check.
 */
int getAmountFromKeyboard()
{
    int amount;
    do
    {
        std::cout << "Please enter the amount you'd like to get (accuracy is 100):\n";
        std::cin >> amount;
        if (amount % 100)
        {
            std::cerr << "Bad amount. Please enter the amount multiple of 100.\n";
        }
        else
        {
            return amount;
        }
    } while (true);
}

/*
 * Get <char> command from std::cin with input check.
 */
char getCommandFromKeyboard()
{
    do {
        std::cout << "Please enter the command:\n'+' \t- to fill the bank\n'-' \t- to get money\n";
        char cmd;
        std::cin >> cmd;
        if (cmd != CMD_GET && cmd != CMD_FILL)
            std::cerr << "Bad command. Try again.\n";
        else
            return cmd;
    }
    while (true);
}

/*
 * Print the bills vector to console (for check).
 */
void printBills(std::vector<int>& bills)
{
    for (int bill : bills)
    {
        std::cout << bill << " ";
    }
    std::cout << "\n";
}

int main() {
    std::srand(time(nullptr));
    std::vector<int> bills;

    char cmd = getCommandFromKeyboard();
    if (cmd == CMD_FILL)
    {
        fillTheBank(bills);
        sortTheBank(bills);
        printBills(bills); // Check
        saveBank(bills);
    }
    else if (cmd == CMD_GET)
    {
        int amount = getAmountFromKeyboard();
        readBank(bills);
        printBills(bills); // Check the state before extracting money.
        extractMoney(amount, bills);
        printBills(bills); // Check the state after extracting money.
        saveBank(bills);
    }
    return 0;
}