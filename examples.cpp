#include "break_eternity.hh"
#include "break_eternity.cpp"
#include <iostream>
#include <iomanip>

int main() {
    double test1 = 2.0;
    double test2 = 3.5;
    int test3 = -1;
    long test4 = 0;
    double test5 = 1e22;
    double test6 = 1e-30;
    int test7 = 1;
    double test8 = 1.79e308;
    double test9 = 5e22;

    Decimal decimal1(test1);
    Decimal decimal2(test2);
    Decimal decimal3(test3);
    Decimal decimal4(test4);
    Decimal decimal5(test5);
    Decimal decimal6(test6);
    Decimal decimal7(test7, test7, test8);
    Decimal decimal8(test9);
    Decimal decimal9("2.2e37");
    Decimal decimal10("-1.92e402");
    Decimal decimal11("-e-20");
    Decimal decimal12("8");
    Decimal decimal13("e2.2e9");
        
    std::cout << std::fixed << std::setprecision(3);

    std::cout << "these are the results of the Decimal constructor" << std::endl;
    std::cout << "sign: " << decimal1.sign << " layer: " << decimal1.layer << " mag: " << decimal1.mag << std::endl;
    std::cout << "sign: " << decimal2.sign << " layer: " << decimal2.layer << " mag: " << decimal2.mag << std::endl;
    std::cout << "sign: " << decimal3.sign << " layer: " << decimal3.layer << " mag: " << decimal3.mag << std::endl;
    std::cout << "sign: " << decimal4.sign << " layer: " << decimal4.layer << " mag: " << decimal4.mag << std::endl;
    std::cout << "sign: " << decimal5.sign << " layer: " << decimal5.layer << " mag: " << decimal5.mag << std::endl;
    std::cout << "sign: " << decimal6.sign << " layer: " << decimal6.layer << " mag: " << decimal6.mag << std::endl;
    std::cout << "sign: " << decimal7.sign << " layer: " << decimal7.layer << " mag: " << decimal7.mag << std::endl;
    std::cout << "sign: " << decimal8.sign << " layer: " << decimal8.layer << " mag: " << decimal8.mag << std::endl;
    std::cout << "sign: " << decimal9.sign << " layer: " << decimal9.layer << " mag: " << decimal9.mag << std::endl;
    std::cout << "sign: " << decimal10.sign << " layer: " << decimal10.layer << " mag: " << decimal10.mag << std::endl;
    std::cout << "sign: " << decimal11.sign << " layer: " << decimal11.layer << " mag: " << decimal11.mag << std::endl;
    std::cout << "sign: " << decimal12.sign << " layer: " << decimal12.layer << " mag: " << decimal12.mag << std::endl;
    std::cout << "sign: " << decimal13.sign << " layer: " << decimal13.layer << " mag: " << decimal13.mag << std::endl;
    std::cout << "does test1 = 2: " << std::to_string(test1 == 2) << std::endl;
    std::cout << std::endl;

    Decimal added1 = Decimal::add(decimal1, decimal2);
    Decimal added2 = Decimal::add(decimal5, decimal8);
    Decimal added3 = decimal5.sub(decimal8);
    std::cout << "these are the results of add operations" << std::endl;
    std::cout << "Decimal(2) + Decimal(3.5)" << std::endl;
    std::cout << "sign: " << added1.sign << " layer: " << added1.layer << " mag: " << added1.mag << std::endl;
    std::cout << "Decimal(1e22) + Decimal(5e22)" << std::endl;
    std::cout << "sign: " << added2.sign << " layer: " << added2.layer << " mag: " << added2.mag << std::endl;
    std::cout << "Decimal(1e22) - Decimal(5e22)" << std::endl;
    std::cout << "sign: " << added3.sign << " layer: " << added3.layer << " mag: " << added3.mag << std::endl;
    std::cout << std::endl;

    Decimal mul1 = Decimal::mul(decimal1, decimal2);
    Decimal mul2 = Decimal::mul(decimal5, decimal8);
    Decimal mul3 = Decimal::div(decimal5, decimal8);
    std::cout << "these are the results of multiply operations" << std::endl;
    std::cout << "Decimal(2) * Decimal(3.5)" << std::endl;
    std::cout << "sign: " << mul1.sign << " layer: " << mul1.layer << " mag: " << mul1.mag << std::endl;
    std::cout << "Decimal(1e22) * Decimal(5e22)" << std::endl;
    std::cout << "sign: " << mul2.sign << " layer: " << mul2.layer << " mag: " << mul2.mag << std::endl;
    std::cout << "Decimal(1e22) / Decimal(5e22)" << std::endl;
    std::cout << "sign: " << mul3.sign << " layer: " << mul3.layer << " mag: " << mul3.mag << std::endl;
    std::cout << std::endl;

    Decimal pow1 = Decimal::pow(decimal1, decimal2);
    Decimal pow2 = Decimal::pow(decimal5, decimal8);
    Decimal pow3 = Decimal::pow(decimal5, Decimal::recip(decimal8));
    Decimal pow4 = Decimal::exp(decimal2);
    Decimal pow5 = Decimal::cbrt(decimal12);
    Decimal pow6 = Decimal::sl(decimal12, decimal1);
    std::cout << "these are the results of power operations" << std::endl;
    std::cout << "Decimal(2) ^ Decimal(3.5)" << std::endl;
    std::cout << "sign: " << pow1.sign << " layer: " << pow1.layer << " mag: " << pow1.mag << std::endl;
    std::cout << "Decimal(1e22) ^ Decimal(5e22)" << std::endl;
    std::cout << "sign: " << pow2.sign << " layer: " << pow2.layer << " mag: " << pow2.mag << std::endl;
    std::cout << "Decimal(1e22) ^ (1 / Decimal(5e22))" << std::endl;
    std::cout << "sign: " << pow3.sign << " layer: " << pow3.layer << " mag: " << pow3.mag << std::endl;
    std::cout << "e ^ Decimal(3.5)" << std::endl;
    std::cout << "sign: " << pow4.sign << " layer: " << pow4.layer << " mag: " << pow4.mag << std::endl;
    std::cout << "cbrt(Decimal(8))" << std::endl;
    std::cout << "sign: " << pow5.sign << " layer: " << pow5.layer << " mag: " << pow5.mag << std::endl;
    std::cout << "Decimal(8) << Decimal(2)" << std::endl;
    std::cout << "sign: " << pow6.sign << " layer: " << pow6.layer << " mag: " << pow6.mag << std::endl;
    std::cout << std::endl;

    Decimal log1 = Decimal::log(decimal1, decimal2);
    Decimal log2 = Decimal::log2(decimal8);
    Decimal log3 = Decimal::log10(decimal8);
    Decimal log4 = Decimal::ln(decimal5);
    std::cout << "these are the results of log operations" << std::endl;
    std::cout << "log base Decimal(2)(Decimal(3.5))" << std::endl;
    std::cout << "sign: " << log1.sign << " layer: " << log1.layer << " mag: " << log1.mag << std::endl;
    std::cout << "lg(Decimal(5e22))" << std::endl;
    std::cout << "sign: " << log2.sign << " layer: " << log2.layer << " mag: " << log2.mag << std::endl;
    std::cout << "log(Decimal(5e22))" << std::endl;
    std::cout << "sign: " << log3.sign << " layer: " << log3.layer << " mag: " << log3.mag << std::endl;
    std::cout << "ln(Decimal(1e22))" << std::endl;
    std::cout << "sign: " << log4.sign << " layer: " << log4.layer << " mag: " << log4.mag << std::endl;
    std::cout << std::endl;

    Decimal ladd1 = Decimal::ladd10(decimal5, decimal2);
    Decimal ladd2 = Decimal::ladd(decimal1, decimal2, decimal12, false);
    std::cout << "these are the results of layer add operations" << std::endl;
    std::cout << "Decimal(1e22) ladd10(Decimal(3.5))" << std::endl;
    std::cout << "sign: " << ladd1.sign << " layer: " << ladd1.layer << " mag: " << ladd1.mag << std::endl;
    std::cout << "Decimal(2) ladd base Decimal(8)(Decimal(3.5))" << std::endl;
    std::cout << "sign: " << ladd2.sign << " layer: " << ladd2.layer << " mag: " << ladd2.mag << std::endl;
    std::cout << std::endl;

    Decimal tet1 = Decimal::tet(decimal1, decimal1, decimal1, false);
    Decimal tet2 = Decimal::tet(decimal1, decimal2, FC(1, 0, 1), false);
    Decimal tet3 = Decimal::tet(decimal12, FC(1, 0, 20), FC(1, 0, 1.01), false);
    Decimal tet4 = Decimal::slog(FC(1, 8, 28), FC(1, 0, 8), 100, false);
    Decimal tet5 = Decimal::slog(decimal1, decimal2, 100, false);
    std::cout << "these are the results of tetration operations" << std::endl;
    std::cout << "Decimal(2) ^^ Decimal(2) ^ Decimal(2)" << std::endl;
    std::cout << "sign: " << tet1.sign << " layer: " << tet1.layer << " mag: " << tet1.mag << std::endl;
    std::cout << "Decimal(2) ^^ Decimal(3.5) ^ Decimal(1)" << std::endl;
    std::cout << "sign: " << tet2.sign << " layer: " << tet2.layer << " mag: " << tet2.mag << std::endl;
    std::cout << "Decimal(8) ^^ Decimal(20) ^ Decimal(1.01)" << std::endl;
    std::cout << "sign: " << tet3.sign << " layer: " << tet3.layer << " mag: " << tet3.mag << std::endl;
    std::cout << "slog base 8(E28#8)" << std::endl;
    std::cout << "sign: " << tet4.sign << " layer: " << tet4.layer << " mag: " << tet4.mag << std::endl;
    std::cout << "slog base 3.5(2)" << std::endl;
    std::cout << "sign: " << tet5.sign << " layer: " << tet5.layer << " mag: " << tet5.mag << std::endl;
    std::cout << std::endl;
    return 0;
}