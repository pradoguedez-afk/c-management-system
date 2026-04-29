# 🧾 C Management System with Data Analysis (C + Python)

## 📌 Overview

A modular management system developed in C, designed to handle clients, products, and sales with a structured and scalable architecture.

This project goes beyond a typical CRUD system by integrating **low-level systems programming (C)** with **high-level data analysis (Python)**, demonstrating the ability to work across different layers of software development.

---

## 🚀 Features

* 👤 Client Management (CRUD operations)
* 📦 Product Management (CRUD + inventory control)
* 💰 Sales Registration System
* 🔎 Data Validation across modules
* 📊 Stock Management integrated with sales
* 💾 Binary Persistence for clients and products (`.dat`)
* 📄 CSV Export for sales data (`sales.csv`)
* 📊 Data Analysis Module using Python (visual insights)

---

## 🧠 Key Concepts

### Dynamic Memory Management

The system uses dynamic memory (`malloc`, `realloc`, `free`) to manage collections of clients, products, and sales efficiently, allowing scalability and flexibility.

### Modular Architecture

The project is structured into independent modules:

* `clients` → client management
* `products` → product and inventory management
* `sales` → business logic and transactions
* `menu` → command-line interface (CLI)

This separation improves maintainability, readability, and scalability.

### Data Consistency & Integrity

The sales module ensures:

* Validation of clients and products before transactions
* Stock verification before registering a sale
* Automatic stock updates after each sale
* Rollback mechanisms to prevent inconsistent states

---

## 📊 Data Analysis Module (Python)

This project includes a complementary Python script (`analysis.py`) used for analyzing and visualizing sales data.

The script processes the generated `sales.csv` file and produces graphical insights such as:

* Revenue per product
* Quantity of products sold
* Sales distribution

This demonstrates integration between:

* **Low-level programming (C)** → data generation
* **High-level analysis (Python)** → data interpretation


---

## 📁 Project Structure

```
/src            → C source files
/include        → Header files
/data           → Binary persistence files (.dat)
/docs/images    → Generated charts (Python output)
analysis.py     → Data analysis and visualization script
sales.csv       → Generated sales data
README.md       → Project documentation
.gitignore      → Git configuration
```

---

## ⚙️ Compilation & Execution

### Compile

```bash
gcc src\*.c -I include -o sistema_gestion.exe
```

### Run

```bash
.\sistema_gestion.exe
```

---

## 📌 Data Persistence

* Clients & Products → stored in binary files (`.dat`) for efficient storage
* Sales → exported to CSV (`sales.csv`) for easy analysis and reporting

---

## 🧠 What I Learned

* Dynamic memory management in C
* Modular system design
* File persistence using binary and CSV formats
* Integration between C and Python workflows
* Data visualization and basic analysis with Python
* Structuring a complete system with multiple interacting modules

---

## 🔮 Future Improvements

* Full persistence for sales in binary format
* Unified input validation across all modules
* Enhanced CLI or graphical interface (GUI)
* Advanced reporting and analytics
* Database integration (SQL or NoSQL)

---

## 👩‍💻 Author

Software Engineering Student
Focused on building structured systems, adaptable solutions, and integrating multiple technologies (C + Python + data analysis).

---
