#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <limits>
#include <map>
#include <algorithm>

using namespace std;

struct Transaction {
    string type;        // Income or Expense
    double amount;
    string category;
    string date;        // DD-MM-YYYY
    string description;
};

class ExpenseManager {
private:
    vector<Transaction> transactions;
    double monthlyBudget = 0.0;

    const string DATA_FILE = "transactions.txt";
    const string BUDGET_FILE = "budget.txt";

    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    double readPositiveAmount(const string& prompt) {
        double amount;
        while (true) {
            cout << prompt;
            if (cin >> amount && amount > 0) {
                clearInput();
                return amount;
            }
            cout << "Please enter a valid amount greater than zero.\n";
            clearInput();
        }
    }

    string readText(const string& prompt) {
        string value;
        do {
            cout << prompt;
            getline(cin, value);
            if (value.empty())
                cout << "This field cannot be empty.\n";
        } while (value.empty());
        return value;
    }

    void loadData() {
        ifstream file(DATA_FILE);
        Transaction t;

        // Each text field is stored on its own line.
        while (getline(file, t.type)) {
            string amountLine;
            if (!getline(file, amountLine) ||
                !getline(file, t.category) ||
                !getline(file, t.date) ||
                !getline(file, t.description))
                break;

            try {
                t.amount = stod(amountLine);
                transactions.push_back(t);
            } catch (...) {
                // Ignore an invalid/incomplete record.
            }
        }

        ifstream budgetFile(BUDGET_FILE);
        if (budgetFile)
            budgetFile >> monthlyBudget;
    }

    void saveData() const {
        ofstream file(DATA_FILE);
        for (const auto& t : transactions) {
            file << t.type << '\n'
                 << t.amount << '\n'
                 << t.category << '\n'
                 << t.date << '\n'
                 << t.description << '\n';
        }

        ofstream budgetFile(BUDGET_FILE);
        budgetFile << monthlyBudget;
    }

    void displayTransactions() const {
        if (transactions.empty()) {
            cout << "\nNo transactions found.\n";
            return;
        }

        cout << "\n"
             << left << setw(5) << "No."
             << setw(10) << "Type"
             << setw(12) << "Amount"
             << setw(18) << "Category"
             << setw(14) << "Date"
             << "Description\n";
        cout << string(80, '-') << '\n';

        for (size_t i = 0; i < transactions.size(); ++i) {
            const auto& t = transactions[i];
            cout << left << setw(5) << i + 1
                 << setw(10) << t.type
                 << setw(12) << fixed << setprecision(2) << t.amount
                 << setw(18) << t.category
                 << setw(14) << t.date
                 << t.description << '\n';
        }
    }

    void addTransaction(const string& type) {
        Transaction t;
        t.type = type;
        t.amount = readPositiveAmount("Enter amount: Rs. ");
        t.category = readText("Enter category/source: ");
        t.date = readText("Enter date (DD-MM-YYYY): ");
        t.description = readText("Enter description: ");

        transactions.push_back(t);
        saveData();
        cout << type << " record added successfully.\n";
    }

public:
    ExpenseManager() {
        loadData();
    }

    void recordIncome() {
        addTransaction("Income");
    }

    void addExpense() {
        addTransaction("Expense");
    }

    void viewTransactions() const {
        displayTransactions();
    }

    void editExpense() {
        displayTransactions();
        if (transactions.empty())
            return;

        size_t number;
        cout << "\nEnter the record number to edit: ";
        if (!(cin >> number) || number == 0 ||
            number > transactions.size()) {
            cout << "Invalid record number.\n";
            clearInput();
            return;
        }
        clearInput();

        Transaction& t = transactions[number - 1];
        if (t.type != "Expense") {
            cout << "Only expense records can be edited here.\n";
            return;
        }

        Transaction updated = t;
        updated.amount = readPositiveAmount("Enter new amount: Rs. ");
        updated.category = readText("Enter new category: ");
        updated.date = readText("Enter new date (DD-MM-YYYY): ");
        updated.description = readText("Enter new description: ");

        t = updated;
        saveData();
        cout << "Expense updated successfully.\n";
    }

    void deleteExpense() {
        displayTransactions();
        if (transactions.empty())
            return;

        size_t number;
        cout << "\nEnter the record number to delete: ";
        if (!(cin >> number) || number == 0 ||
            number > transactions.size()) {
            cout << "Invalid record number.\n";
            clearInput();
            return;
        }
        clearInput();

        if (transactions[number - 1].type != "Expense") {
            cout << "Only expense records can be deleted here.\n";
            return;
        }

        transactions.erase(transactions.begin() + number - 1);
        saveData();
        cout << "Expense deleted successfully.\n";
    }

    void setBudget() {
        double value;
        cout << "Enter monthly budget: Rs. ";
        if (!(cin >> value) || value < 0) {
            cout << "Please enter a valid non-negative budget.\n";
            clearInput();
            return;
        }
        clearInput();

        monthlyBudget = value;
        saveData();
        cout << "Monthly budget saved successfully.\n";
    }

    void categoryReport() const {
        map<string, double> totals;
        for (const auto& t : transactions) {
            if (t.type == "Expense")
                totals[t.category] += t.amount;
        }

        cout << "\n--- CATEGORY-WISE EXPENSE REPORT ---\n";
        if (totals.empty()) {
            cout << "No expenses recorded.\n";
            return;
        }

        for (const auto& item : totals)
            cout << left << setw(22) << item.first
                 << "Rs. " << fixed << setprecision(2)
                 << item.second << '\n';
    }

    void financialSummary() const {
        double income = 0.0, expenses = 0.0;

        for (const auto& t : transactions) {
            if (t.type == "Income")
                income += t.amount;
            else if (t.type == "Expense")
                expenses += t.amount;
        }

        cout << fixed << setprecision(2);
        cout << "\n--- FINANCIAL SUMMARY ---\n"
             << "Total Income:       Rs. " << income << '\n'
             << "Total Expenses:     Rs. " << expenses << '\n'
             << "Remaining Balance:  Rs. " << income - expenses << '\n'
             << "Savings:            Rs. " << income - expenses << '\n'
             << "Monthly Budget:     Rs. " << monthlyBudget << '\n';

        if (monthlyBudget > 0) {
            double usage = (expenses / monthlyBudget) * 100.0;
            cout << "Budget Used:        " << usage << "%\n";
            if (expenses > monthlyBudget)
                cout << "Warning: Your recorded expenses exceed the budget.\n";
            else
                cout << "Remaining Budget:   Rs. "
                     << monthlyBudget - expenses << '\n';
        }
    }

    void run() {
        int choice;

        do {
            cout << "\n====================================\n"
                 << "   PERSONAL EXPENSE MANAGEMENT\n"
                 << "====================================\n"
                 << "1. Record Income\n"
                 << "2. Add Expense\n"
                 << "3. View All Transactions\n"
                 << "4. Edit Expense\n"
                 << "5. Delete Expense\n"
                 << "6. Set Monthly Budget\n"
                 << "7. Financial Summary\n"
                 << "8. Category-wise Expense Report\n"
                 << "9. Exit\n"
                 << "Enter your choice: ";

            if (!(cin >> choice)) {
                cout << "Please enter a number from 1 to 9.\n";
                clearInput();
                continue;
            }
            clearInput();

            switch (choice) {
                case 1: recordIncome(); break;
                case 2: addExpense(); break;
                case 3: viewTransactions(); break;
                case 4: editExpense(); break;
                case 5: deleteExpense(); break;
                case 6: setBudget(); break;
                case 7: financialSummary(); break;
                case 8: categoryReport(); break;
                case 9: cout << "Thank you for using the system!\n"; break;
                default: cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 9);
    }
};

int main() {
    ExpenseManager manager;
    manager.run();
    return 0;
}
