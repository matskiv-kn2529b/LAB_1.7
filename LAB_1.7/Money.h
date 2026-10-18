#pragma once
#pragma once
#include <string>

// Клас Money -- грошова сума: гривні (long) + копійки (unsigned char).
// Незалежний клас, використовується як поле в Account.
class Money
{
private:
    long hryvnias;            // гривні (>= 0)
    unsigned char kopiyky;    // копійки (0..99)

    long long toKopiyky() const;                  // уся сума в копійках
    static Money fromKopiyky(long long total);    // сума з копійок (з перевіркою)

public:
    Money();
    Money(long hryvnias, unsigned char kopiyky);

    // --- геттери ---
    long getHryvnias() const { return hryvnias; }
    unsigned char getKopiyky() const { return kopiyky; }

    // --- сеттери з перевіркою ---
    bool setHryvnias(long value);
    bool setKopiyky(unsigned char value);

    // --- Init / Read / Display / toString ---
    bool Init(long hryvnias, unsigned char kopiyky);
    void Read();
    void Display() const;
    std::string toString() const;      // формат "1992,28"
    double toDouble() const;

    // --- арифметика ---
    Money operator+(const Money& other) const;     // додавання
    Money operator-(const Money& other) const;     // віднімання
    double operator/(const Money& other) const;    // ділення сум (результат -- число)
    Money operator/(double divisor) const;         // ділення на дробове число
    Money operator*(double factor) const;          // множення на дробове число

    // --- порівняння ---
    bool operator==(const Money& other) const;
    bool operator!=(const Money& other) const;
    bool operator<(const Money& other) const;
    bool operator>(const Money& other) const;
    bool operator<=(const Money& other) const;
    bool operator>=(const Money& other) const;
};
