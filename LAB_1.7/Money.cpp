#include "Money.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <climits>
using namespace std;

// ===== допоміжне: безпечне введення =====
static long readMoneyLong(const char* text)
{
    long v;
    cout << text;
    while (!(cin >> v)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Введіть ціле число: ";
    }
    return v;
}

// ===== конструктори =====
Money::Money() : hryvnias(0), kopiyky(0) {}

Money::Money(long h, unsigned char k) : hryvnias(0), kopiyky(0)
{
    Init(h, k);
}

// ===== приватні допоміжні методи =====
long long Money::toKopiyky() const
{
    return (long long)hryvnias * 100 + kopiyky;
}

Money Money::fromKopiyky(long long total)
{
    Money m;
    if (total < 0 || total / 100 > 999999999999LL || total / 100 > LONG_MAX) {
        cerr << "[Помилка]: результат від'ємний або завеликий" << endl;
        return m;
    }
    m.hryvnias = (long)(total / 100);
    m.kopiyky = (unsigned char)(total % 100);
    return m;
}

// ===== сеттери =====
bool Money::setHryvnias(long value)
{
    if (value < 0 || value > 999999999999LL) {
        cerr << "[Помилка]: гривні мають бути від 0 до 999999999999" << endl;
        return false;
    }
    hryvnias = value;
    return true;
}

bool Money::setKopiyky(unsigned char value)
{
    if (value > 99) {
        cerr << "[Помилка]: копійки мають бути від 0 до 99" << endl;
        return false;
    }
    kopiyky = value;
    return true;
}

// ===== Init / Read / Display / toString =====
bool Money::Init(long h, unsigned char k)
{
    bool okH = setHryvnias(h);
    bool okK = setKopiyky(k);
    if (!okH || !okK) {
        hryvnias = 0;
        kopiyky = 0;
        return false;
    }
    return true;
}

void Money::Read()
{
    long h = readMoneyLong("Гривні: ");
    while (!setHryvnias(h)) h = readMoneyLong("Гривні: ");

    long k = readMoneyLong("Копійки (0-99): ");
    while (k < 0 || k > 99) {
        cerr << "[Помилка]: копійки мають бути від 0 до 99" << endl;
        k = readMoneyLong("Копійки (0-99): ");
    }
    setKopiyky((unsigned char)k);
}

void Money::Display() const
{
    cout << toString() << " грн." << endl;
}

string Money::toString() const
{
    stringstream ss;
    ss << hryvnias << ',' << setfill('0') << setw(2) << (int)kopiyky;   // кома між гривнями й копійками
    return ss.str();
}

double Money::toDouble() const
{
    return hryvnias + kopiyky / 100.0;
}

// ===== арифметика =====
Money Money::operator+(const Money& other) const
{
    return fromKopiyky(toKopiyky() + other.toKopiyky());
}

Money Money::operator-(const Money& other) const
{
    return fromKopiyky(toKopiyky() - other.toKopiyky());   // від'ємний результат -> помилка
}

double Money::operator/(const Money& other) const
{
    if (other.toKopiyky() == 0) {
        cerr << "[Помилка]: ділення на нульову суму" << endl;
        return 0.0;
    }
    return (double)toKopiyky() / (double)other.toKopiyky();
}

Money Money::operator/(double divisor) const
{
    if (!(divisor > 0)) {
        cerr << "[Помилка]: дільник має бути додатним" << endl;
        return Money();
    }
    double r = toKopiyky() / divisor;
    if (r > 9.0e18) return fromKopiyky(-1);   // завелике значення
    return fromKopiyky(llround(r));
}

Money Money::operator*(double factor) const
{
    double r = toKopiyky() * factor;
    if (!(r >= 0.0) || r > 9.0e18) return fromKopiyky(-1);   // від'ємне або завелике
    return fromKopiyky(llround(r));
}

// ===== порівняння (за сумою в копійках) =====
bool Money::operator==(const Money& other) const { return toKopiyky() == other.toKopiyky(); }
bool Money::operator!=(const Money& other) const { return toKopiyky() != other.toKopiyky(); }
bool Money::operator<(const Money& other) const { return toKopiyky() < other.toKopiyky(); }
bool Money::operator>(const Money& other) const { return toKopiyky() > other.toKopiyky(); }
bool Money::operator<=(const Money& other) const { return toKopiyky() <= other.toKopiyky(); }
bool Money::operator>=(const Money& other) const { return toKopiyky() >= other.toKopiyky(); }
