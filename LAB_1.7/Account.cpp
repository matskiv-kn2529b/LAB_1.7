#include "Account.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

// ===== допоміжне: безпечне введення =====
static double readAccDouble(const char* text)
{
    double v;
    cout << text;
    while (!(cin >> v)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Введіть число: ";
    }
    return v;
}

// ===== допоміжне: сума прописом =====
namespace {
    const char* const HUNDREDS[10] = { "", "сто", "двісті", "триста", "чотириста",
        "п'ятсот", "шістсот", "сімсот", "вісімсот", "дев'ятсот" };
    const char* const TENS[10] = { "", "", "двадцять", "тридцять", "сорок",
        "п'ятдесят", "шістдесят", "сімдесят", "вісімдесят", "дев'яносто" };
    const char* const TEENS[10] = { "десять", "одинадцять", "дванадцять", "тринадцять",
        "чотирнадцять", "п'ятнадцять", "шістнадцять", "сімнадцять", "вісімнадцять", "дев'ятнадцять" };
    const char* const UNITS_M[10] = { "", "один", "два", "три", "чотири",
        "п'ять", "шість", "сім", "вісім", "дев'ять" };
    const char* const UNITS_F[10] = { "", "одна", "дві", "три", "чотири",
        "п'ять", "шість", "сім", "вісім", "дев'ять" };

    // число 1..999 словами (feminine -- жіночий рід: одна, дві)
    string tripleToWords(int n, bool feminine)
    {
        string s;
        auto add = [&s](const char* w) {
            if (w[0] == '\0') return;
            if (!s.empty()) s += " ";
            s += w;
            };
        add(HUNDREDS[n / 100]);
        int rest = n % 100;
        if (rest >= 10 && rest <= 19) {
            add(TEENS[rest - 10]);
        }
        else {
            add(TENS[rest / 10]);
            add(feminine ? UNITS_F[rest % 10] : UNITS_M[rest % 10]);
        }
        return s;
    }

    // правильна форма слова: 1 тисяча, 2 тисячі, 5 тисяч
    const char* pluralForm(int n, const char* one, const char* few, const char* many)
    {
        int n100 = n % 100, n10 = n % 10;
        if (n100 >= 11 && n100 <= 14) return many;
        if (n10 == 1) return one;
        if (n10 >= 2 && n10 <= 4) return few;
        return many;
    }
}

// =====================================================================
// Account
// =====================================================================
Account::Account()
{
    Init("Unknown", "000000", 0, Money());
}

Account::Account(const string& surname, const string& accountNumber,
    double interestRate, const Money& balance)
{
    Init(surname, accountNumber, interestRate, balance);
}

// ===== сеттери =====
bool Account::setSurname(const string& value)
{
    bool ok = !value.empty() && value.size() <= 50;
    for (size_t i = 0; ok && i < value.size(); ++i) {
        if (value[i] >= '0' && value[i] <= '9') ok = false;   // цифр у прізвищі бути не може
    }
    if (!ok) {
        cerr << "[Помилка]: прізвище не порожнє, до 50 символів, без цифр" << endl;
        return false;
    }
    surname = value;
    return true;
}

bool Account::setAccountNumber(const string& value)
{
    bool ok = value.size() >= 6 && value.size() <= 20;
    for (size_t i = 0; ok && i < value.size(); ++i) {
        if (value[i] < '0' || value[i] > '9') ok = false;   // тільки цифри
    }
    if (!ok) {
        cerr << "[Помилка]: номер рахунку -- від 6 до 20 цифр" << endl;
        return false;
    }
    accountNumber = value;
    return true;
}

bool Account::setInterestRate(double value)
{
    if (!(value >= 0 && value <= 100)) {
        cerr << "[Помилка]: відсоток має бути від 0 до 100" << endl;
        return false;
    }
    interestRate = value;
    return true;
}

void Account::setBalance(const Money& value)
{
    balance = value;   // Money вже гарантує коректність
}

// ===== Init / Read / Display / toString =====
bool Account::Init(const string& surname, const string& accountNumber,
    double interestRate, const Money& balance)
{
    bool ok = true;
    if (!setSurname(surname)) { this->surname = "Unknown"; ok = false; }
    if (!setAccountNumber(accountNumber)) { this->accountNumber = "000000"; ok = false; }
    if (!setInterestRate(interestRate)) { this->interestRate = 0; ok = false; }
    this->balance = balance;
    return ok;
}

void Account::Read()
{
    string s;
    cout << "Прізвище власника: ";
    getline(cin >> ws, s);
    while (!setSurname(s)) {
        cout << "Прізвище власника: ";
        getline(cin >> ws, s);
    }

    cout << "Номер рахунку: ";
    cin >> s;
    while (!setAccountNumber(s)) {
        cout << "Номер рахунку: ";
        cin >> s;
    }

    double r = readAccDouble("Відсоток нарахування: ");
    while (!setInterestRate(r)) r = readAccDouble("Відсоток нарахування: ");

    cout << "Сума на рахунку:" << endl;
    balance.Read();   // делегування класу Money
}

void Account::Display() const
{
    cout << toString() << endl;
}

string Account::toString() const
{
    stringstream ss;
    ss << "Рахунок " << accountNumber << " | власник: " << surname
        << " | відсоток: " << interestRate << "% | сума: " << balance.toString() << " грн.";
    return ss.str();
}

// ===== операції =====
bool Account::changeOwner(const string& newSurname)
{
    return setSurname(newSurname);
}

bool Account::withdraw(const Money& amount)
{
    if (!(amount > Money())) {
        cerr << "[Помилка]: сума зняття має бути додатною" << endl;
        return false;
    }
    if (amount > balance) {
        cerr << "[Помилка]: недостатньо коштів на рахунку" << endl;
        return false;
    }
    balance = balance - amount;
    return true;
}

bool Account::deposit(const Money& amount)
{
    if (!(amount > Money())) {
        cerr << "[Помилка]: сума поповнення має бути додатною" << endl;
        return false;
    }
    Money result = balance + amount;
    if (result < balance) {   // переповнення
        cerr << "[Помилка]: сума на рахунку стала б завеликою" << endl;
        return false;
    }
    balance = result;
    return true;
}

Money Account::accrueInterest()
{
    Money interest = balance * (interestRate / 100.0);
    Money result = balance + interest;
    if (result < balance) {
        cerr << "[Помилка]: не вдалося нарахувати відсотки (переповнення)" << endl;
        return Money();
    }
    balance = result;
    return interest;   // повертаємо нараховану суму
}

Money Account::toUSD(double rate) const
{
    if (!(rate > 0)) {
        cerr << "[Помилка]: курс має бути додатним" << endl;
        return Money();
    }
    return balance / rate;
}

Money Account::toEUR(double rate) const
{
    if (!(rate > 0)) {
        cerr << "[Помилка]: курс має бути додатним" << endl;
        return Money();
    }
    return balance / rate;
}

string Account::amountInWords() const
{
    long h = balance.getHryvnias();
    string result;

    if (h == 0) {
        result = "нуль";
    }
    else {
        // розбиваємо на групи по три цифри: одиниці, тисячі, мільйони, мільярди
        int g[4];
        long t = h;
        for (int i = 0; i < 4; ++i) {
            g[i] = (int)(t % 1000);
            t /= 1000;
        }
        if (g[3]) result += tripleToWords(g[3], false) + " " + pluralForm(g[3], "мільярд", "мільярди", "мільярдів") + " ";
        if (g[2]) result += tripleToWords(g[2], false) + " " + pluralForm(g[2], "мільйон", "мільйони", "мільйонів") + " ";
        if (g[1]) result += tripleToWords(g[1], true) + " " + pluralForm(g[1], "тисяча", "тисячі", "тисяч") + " ";
        if (g[0]) result += tripleToWords(g[0], true) + " ";
        if (!result.empty() && result[result.size() - 1] == ' ') result.erase(result.size() - 1);
    }

    stringstream ss;
    ss << result << " грн. " << setfill('0') << setw(2) << (int)balance.getKopiyky() << " коп.";
    return ss.str();
}
