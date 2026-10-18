#pragma once
#include <string>
#include "Money.h"

// Клас Account -- банківський рахунок.
// Поле balance -- об'єкт незалежного класу Money (композиція).
class Account
{
private:
    std::string surname;         // прізвище власника
    std::string accountNumber;   // номер рахунку (цифри)
    double interestRate;         // відсоток нарахування, %
    Money balance;               // сума на рахунку

public:
    Account();
    Account(const std::string& surname, const std::string& accountNumber,
        double interestRate, const Money& balance);

    // --- геттери ---
    std::string getSurname() const { return surname; }
    std::string getAccountNumber() const { return accountNumber; }
    double getInterestRate() const { return interestRate; }
    Money getBalance() const { return balance; }

    // --- сеттери з перевіркою ---
    bool setSurname(const std::string& value);
    bool setAccountNumber(const std::string& value);
    bool setInterestRate(double value);
    void setBalance(const Money& value);

    // --- Init / Read / Display / toString ---
    bool Init(const std::string& surname, const std::string& accountNumber,
        double interestRate, const Money& balance);   // відкриття рахунку
    void Read();
    void Display() const;
    std::string toString() const;

    // --- операції зі списку завдання ---
    bool changeOwner(const std::string& newSurname);   // змінити власника
    bool withdraw(const Money& amount);                // зняти гроші
    bool deposit(const Money& amount);                 // покласти гроші
    Money accrueInterest();                            // нарахувати відсотки
    Money toUSD(double rate) const;                    // у долари (rate -- грн за 1 USD)
    Money toEUR(double rate) const;                    // у євро (rate -- грн за 1 EUR)
    std::string amountInWords() const;                 // сума прописом
};
