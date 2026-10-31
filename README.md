# Personal Expense Management System

A menu-driven personal finance application built in **C++**. It helps users record income, manage expenses, set a monthly budget, and review basic financial summaries.

## Features

- Record income with amount, source/category, date, and description.
- Add, view, edit, and delete expense records.
- Categorize expenses and view category-wise totals.
- Set and save a monthly budget.
- View total income, expenses, remaining balance, savings, and budget usage.
- Save records locally using file handling, so data remains available after restarting the program.

## Requirements

- A C++ compiler that supports **C++11** or later (such as g++, MinGW, or clang++).
- Terminal or command prompt.

## How to compile and run

### Windows (MinGW)

```bash
g++ -std=c++11 main.cpp -o expense-manager.exe
expense-manager.exe
```

### Linux / macOS

```bash
g++ -std=c++11 main.cpp -o expense-manager
./expense-manager
```

## Data files

The program creates these files in the same directory where it is run:

- `transactions.txt` — stores income and expense records.
- `budget.txt` — stores the monthly budget.

These files are generated automatically when you use the application. They are excluded from Git by `.gitignore` so personal financial data is not accidentally uploaded.

## Project modules

1. **Record Income** — add income records.
2. **Manage Expenses** — add, view, edit, and delete expenses.
3. **Categories and Budget** — organize expenses and set a monthly budget.
4. **Financial Reports** — view overall totals and category-wise expense summaries.

## Notes and limitations

- Dates are stored as text in `DD-MM-YYYY` format.
- The current summary totals all saved transactions; it does not yet filter records by calendar month.
- The budget is a single overall budget, not separate budgets for each category.
- Descriptions and categories can contain spaces.

## Repository structure

```text
Personal-Expense-Management-System/
├── main.cpp
├── README.md
└── .gitignore
```

## License

This project is provided for educational use. Add a license file if you want to specify reuse and distribution terms.
