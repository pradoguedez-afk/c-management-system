# 🧾 Management System in C

## 📌 Overview
This project is a modular management system developed in C.  
It allows handling clients, products, and sales through a structured and scalable architecture.

The system was designed focusing on **dynamic memory management**, **modular design**, and **data consistency across modules**.

---

## 🚀 Features

- 👤 Client Management (CRUD)
- 📦 Product Management (CRUD)
- 💰 Sales Registration System
- 🔎 Data Validation
- 📊 Stock Management (integrated with sales)
- 💾 Binary Persistence for Clients and Products (.dat)
- 📄 CSV Export for Sales (reporting)

---

## 🧠 Key Concepts

### Dynamic Memory Management
The system uses dynamic arrays (`malloc`, `realloc`, `free`) to manage data efficiently and allow scalability.

### Modular Architecture
The project is divided into independent modules:
- `clients` → client management
- `products` → product and inventory
- `sales` → business logic and transactions
- `menu` → user interface (CLI)

This separation improves maintainability and readability.

### Data Consistency
The sales module ensures:
- Product stock is validated before a sale
- Stock is updated after each transaction
- Rollback mechanisms prevent inconsistent states

---

## 📁 Project Structure
