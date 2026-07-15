#ifndef BREAK_ETERNITY_HH
#define BREAK_ETERNITY_HH

#include <concepts>
#include <cmath>
#include <iostream>
#include <type_traits>
#include <algorithm>
#include <string>
#include <deque>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <variant>

extern const double EXP_LIMIT;
extern const double LAYER_LIMIT;
extern const int MAX_SIGNIFICANT_DIGITS;
extern const double tolEq_TOLERANCE;

extern const std::vector<std::vector<double long>> CRITICAL_TET_VALUES;
extern const std::vector<std::vector<double long>> CRITICAL_SLOG_VALUES;
extern const double long MAGIC_CONSTANT;
extern const double OTHER_MAGIC_CONSTANT;

class Decimal {
public:
	//uses int operations to set value to 0
	typedef int Sign;
	typedef unsigned long long Layer;
	typedef double Mag;
	Sign sign;
	Layer layer;
	Mag mag;

	template <typename T>
	static Sign toSign(T a);
	template <typename T>
	static bool isZero(T a);

	template <typename T>
	Layer layerChecker(T a, Layer b);
	template <typename T>
	Mag setMagBasedOnLayer(T a, Layer b, Sign c);

	Sign signFromStr(std::string a);
	Layer layerFromStr(std::string a);
	Mag magFromStr(std::string a);

	//constructors
	//Decimal.normalize() constructor
	Decimal() : sign(0), layer(0), mag(0) {}
	//primary constructor, Decimal test(x), similar to BE's test = new Decimal(x)
	template <typename T>
		requires (std::integral<T> || std::floating_point<T>)
	Decimal(const T _value) : sign(toSign(_value)), layer(layerChecker(_value, 0)), mag(setMagBasedOnLayer(_value* sign, layer, sign)) {}
	//secondary constructor, Decimal test(sign, layer, mag), similar to BE's test = new Decimal.fromComponents(sign, layer, mag)
	Decimal(const Sign _sign, const Layer _layer, const Mag _mag) : sign(_sign), layer(_layer), mag(_mag) {}
	//tertiary constructor, Decimal test("x"), similar to BE's test = new Decimal("x")
	Decimal(const std::string _string) : sign(signFromStr(_string)), layer(layerFromStr(_string)), mag(magFromStr(_string)) {}

	//utilities
	//pointer manipulation
	static Decimal fromPtr(Decimal* a);
	Decimal fromPtr();
	static Decimal* toPtr(Decimal a);
	Decimal* toPtr();

	//export as type
	static int toInt(Decimal a);
	int toInt();
	static long long toLongLong(Decimal a);
	long long toLongLong();
	static double toDouble(Decimal a);
	double toDouble();
	static float toFloat(Decimal a);
	float toFloat();

	//spooky secret operation from HyperCalc.js
	static double d_maglog10(double a);

	//comparisons
	//-a
	static Decimal neg(Decimal a);
	Decimal neg();
	//|a|
	static Decimal abs(Decimal a);
	Decimal abs();
	//some comparison crap, I know what it does, but it's so simple to the point where I don't know how to word it
	static int cmp(Decimal a, Decimal b);
	int cmp(Decimal a);
	static int compare(Decimal a, Decimal b);
	int compare(Decimal a);
	//for some reason this is a different function that just calling cmp(|a|, |b|) lol but that's essentially what it does
	static int cmpabs(Decimal a, Decimal b);
	int cmpabs(Decimal a);
	static int compareAbs(Decimal a, Decimal b);
	int compareAbs(Decimal a);
	//a > b
	static bool gt(Decimal a, Decimal b);
	bool gt(Decimal a);
	static bool greaterThan(Decimal a, Decimal b);
	bool greaterThan(Decimal a);
	//a ≥ b
	static bool gte(Decimal a, Decimal b);
	bool gte(Decimal a);
	static bool greaterThanOrEqualTo(Decimal a, Decimal b);
	bool greaterThanOrEqualTo(Decimal a);
	//a < b
	static bool lt(Decimal a, Decimal b);
	bool lt(Decimal a);
	static bool lessThan(Decimal a, Decimal b);
	bool lessThan(Decimal a);
	//a ≤ b
	static bool lte(Decimal a, Decimal b);
	bool lte(Decimal a);
	static bool lessThanOrEqualTo(Decimal a, Decimal b);
	bool lessThanOrEqualTo(Decimal a);

	//returns which Decimal is bigger
	static Decimal max(Decimal a, Decimal b);
	Decimal max(Decimal a);
	static Decimal clampMin(Decimal a, Decimal b);
	Decimal clampMin(Decimal a);
	//max of a and b's magnitudes
	static Decimal maxabs(Decimal a, Decimal b);
	Decimal maxabs(Decimal a);
	//returns which Decimal is smaller
	static Decimal min(Decimal a, Decimal b);
	Decimal min(Decimal a);
	static Decimal clampMax(Decimal a, Decimal b);
	Decimal clampMax(Decimal a);
	//min of a and b's magnitudes
	static Decimal minabs(Decimal a, Decimal b);
	Decimal minabs(Decimal a);
	//if a < b || a > c, return a Decimal in that range
	static Decimal clamp(Decimal a, Decimal b, Decimal c);
	Decimal clamp(Decimal a, Decimal b);

	//floor(a)
	static Decimal floor(Decimal a);
	Decimal floor();
	//ceil(a)
	static Decimal ceil(Decimal a);
	Decimal ceil();

	//a = b
	static bool eq(Decimal a, Decimal b);
	bool eq(Decimal a);
	static bool equal(Decimal a, Decimal b);
	bool equal(Decimal a);
	static bool equals(Decimal a, Decimal b);
	bool equals(Decimal a);
	static bool equality(Decimal a, Decimal b);
	bool equality(Decimal a);
	//I recommend using tolEq over eq due to Decimal.mag being a double
	//a = (b - c ≤ x ≤ b + c), if c is not given, the tolerance is 1e-7
	static bool tolEq(Decimal a, Decimal b, double c);
	bool tolEq(Decimal a, double b);
	static bool tolEq(Decimal a, Decimal b);
	bool tolEq(Decimal a);
	static bool tolerantEqual(Decimal a, Decimal b, double c);
	bool tolerantEqual(Decimal a, double b);
	static bool tolerantEqual(Decimal a, Decimal b);
	bool tolerantEqual(Decimal a);
	static bool tolerantEquals(Decimal a, Decimal b, double c);
	bool tolerantEquals(Decimal a, double b);
	static bool tolerantEquals(Decimal a, Decimal b);
	bool tolerantEquals(Decimal a);
	static bool tolerantEquality(Decimal a, Decimal b, double c);
	bool tolerantEquality(Decimal a, double b);
	static bool tolerantEquality(Decimal a, Decimal b);
	bool tolerantEquality(Decimal a);
	//a != (b - c ≤ x ≤ b + c), if c is not given, the tolerance is 1e-7
	static bool tolNeq(Decimal a, Decimal b, double c);
	bool tolNeq(Decimal b, double c);
	static bool tolNeq(Decimal a, Decimal b);
	bool tolNeq(Decimal b);

	//1/a
	static Decimal recip(Decimal a);
	Decimal recip();
	static Decimal reciprocal(Decimal a);
	Decimal reciprocal();
	static Decimal reciprocate(Decimal a);
	Decimal reciprocate();
	static Decimal inv(Decimal a);
	Decimal inv();
	static Decimal inverse(Decimal a);
	Decimal inverse();
	static Decimal invert(Decimal a);
	Decimal invert();

	//operations
	//a + b
	static Decimal add(Decimal a, Decimal b);
	Decimal add(Decimal a);
	static Decimal plus(Decimal a, Decimal b);
	Decimal plus(Decimal a);
	//a - b
	static Decimal sub(Decimal a, Decimal b);
	Decimal sub(Decimal a);
	static Decimal subtract(Decimal a, Decimal b);
	Decimal subtract(Decimal a);
	static Decimal minus(Decimal a, Decimal b);
	Decimal minus(Decimal a);

	//a * b
	static Decimal mul(Decimal a, Decimal b);
	Decimal mul(Decimal a);
	static Decimal mult(Decimal a, Decimal b);
	Decimal mult(Decimal a);
	static Decimal times(Decimal a, Decimal b);
	Decimal times(Decimal a);
	static Decimal multiply(Decimal a, Decimal b);
	Decimal multiply(Decimal a);

	//a / b
	static Decimal div(Decimal a, Decimal b);
	Decimal div(Decimal a);
	static Decimal divide(Decimal a, Decimal b);
	Decimal divide(Decimal a);
	static Decimal divideBy(Decimal a, Decimal b);
	Decimal divideBy(Decimal a);
	static Decimal dividedBy(Decimal a, Decimal b);
	Decimal dividedBy(Decimal a);
	//WARNING, only bitshifts when values are lower than the 64 bit integer limit, values over the 64 bit integer limit will try to emulate this behavior
	//this is (probably) faster than the div functions
	//roughly a / 2^(b) or a >> b
	static Decimal sr(Decimal a, Decimal b);
	Decimal sr(Decimal a);
	static Decimal rs(Decimal a, Decimal b);
	Decimal rs(Decimal a);

	//loga(b)
	static Decimal log(Decimal a, Decimal b);
	Decimal log(Decimal a);
	//log10(|a|)
	static Decimal absLog10(Decimal a);
	Decimal absLog10();
	//log10(a)
	static Decimal log10(Decimal a);
	Decimal log10();
	//log2(a)
	static Decimal lg(Decimal a);
	Decimal lg();
	static Decimal log2(Decimal a);
	Decimal log2();
	//ln(a)
	static Decimal ln(Decimal a);
	Decimal ln();

	//a^b
	static Decimal pow(Decimal a, Decimal b);
	Decimal pow(Decimal a);
	static Decimal power(Decimal a, Decimal b);
	Decimal power(Decimal a);
	static Decimal raise(Decimal a, Decimal b);
	Decimal raise(Decimal a);
	//10^a
	static Decimal pow10(Decimal a);
	Decimal pow10();
	static Decimal power10(Decimal a);
	Decimal power10();
	static Decimal raise10(Decimal a);
	Decimal raise10();
	static Decimal exp10(Decimal a);
	Decimal exp10();
	//2^a
	static Decimal pow2(Decimal a);
	Decimal pow2();
	static Decimal power2(Decimal a);
	Decimal power2();
	static Decimal raise2(Decimal a);
	Decimal raise2();
	static Decimal exp2(Decimal a);
	Decimal exp2();
	//WARNING, only bitshifts when values are lower than the 64 bit integer limit, values over the 64 bit integer limit will try to emulate this behavior
	//this is (probably) faster than the pow2 functions
	//a * 2^(|b|) or a << b
	static Decimal sl(Decimal a, Decimal b);
	Decimal sl(Decimal a);
	static Decimal ls(Decimal a, Decimal b);
	Decimal ls(Decimal a);
	//e^a
	static Decimal exp(Decimal a);
	Decimal exp();
	static Decimal powE(Decimal a);
	Decimal powE();
	static Decimal powerE(Decimal a);
	Decimal powerE();
	static Decimal raiseE(Decimal a);
	Decimal raiseE();

	//sqrt(a)
	static Decimal sqrt(Decimal a);
	Decimal sqrt();
	static Decimal root(Decimal a);
	Decimal root();
	//cbrt(a)
	static Decimal cbrt(Decimal a);
	Decimal cbrt();

	//tetration is spooky
	//lambert W function(a), b ? W_0 : W_-1, if b is not given, assume W_0 https://en.wikipedia.org/wiki/Lambert_W_function
	// W_0 works for all numbers >= -1/e, while W_-1 only works in the range of -1/e =< x =< 0
	static Decimal lambertw(Decimal a, bool b);
	Decimal lambertw(bool b);
	static Decimal lambertw(Decimal a);
	Decimal lambertw();
	//helper function (that does like 99% of the computations) for the lambert W function, tolerance b, c ? W_0 : W-1
	//if b is not given, tolerance = 1e-10, if c is not given, assume W_0
	static double d_lambertw(double a, double b, bool c);
	static double d_lambertw(double a, double b);
	static double d_lambertw(double a, bool c);
	static double d_lambertw(double a);
	static Decimal d_lambertw(Decimal a, double b, bool c);
	static Decimal d_lambertw(Decimal a, double b);
	static Decimal d_lambertw(Decimal a, bool c);
	Decimal d_lambertw(bool c);
	static Decimal d_lambertw(Decimal a);
	Decimal d_lambertw();

	//tetration helper function, a^^b, c is the table of values to pull from
	static double c_tet(double a, double b, std::vector<std::vector<double long>> c);
	//(a^^b)^c, if c is not given, c = I(1), d is if we are using a linear approximation or not, if d is not given, assume no linear approx.
	static Decimal tet(Decimal a, Decimal b, Decimal c, bool d);

	//helper function for ladd10, itLog10, tet10, checks if a + b would increase the layer of the result
	static std::vector<double, std::allocator<double>> d_ladd(double a, double b);
	//a.layer = a.layer + b, b works with noninteger values, but is a lot laggier, for now, the only approximation is linear because I'm not a college student :(
	//a ladd base 10(b), just a simple addition of layers
	static Decimal ladd10(Decimal a, Decimal b);
	Decimal ladd10(Decimal b);
	//a lsub base 10(b)
	static Decimal lsub10(Decimal a, Decimal b);
	Decimal lsub10(Decimal b);
	//a ladd base c(b), 2436473262x more complex than ladd10, d for linear approx.
	static Decimal ladd(Decimal a, Decimal b, Decimal c, bool d);

	//slog base b(a), c is the number of iterations to estimate, d is linear approx.
	static Decimal slog(Decimal a, Decimal b, int c, bool d);
	static Decimal d_slog(Decimal a, Decimal b, int c, bool d);
	//magic function that I have no goddamn clue what it does, it is used one time in the tet(x) code and that is it
	static std::vector<std::variant<Decimal, int>> e_slog(Decimal a, Decimal b, bool c);
	
	//log base b^c(a), d is if we are using a linear approximation or not, if d is not given, assume no linear approx.
	static Decimal itLog(Decimal a, Decimal b, Decimal c, bool d);
	static Decimal logN(Decimal a, Decimal b, Decimal c, bool d);

	//log10^b(a)
	static Decimal repLog10(Decimal a, Decimal b);
	static Decimal rLog10(Decimal a, Decimal b);
};

extern const Decimal INF;
extern const Decimal negINF;

Decimal I(Decimal::Mag a);
Decimal D(Decimal a);
Decimal FC(Decimal::Sign sign, Decimal::Layer layer, Decimal::Mag mag);

#endif