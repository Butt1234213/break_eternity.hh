#include "break_eternity.hh"
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

//the only difference between the static and nonstatic functions is that one of the arguments is a *this, and it has to be the dereferenced pointer due to this being the pointer of the Decimal object

const double EXP_LIMIT = 9e15;
//log10 9e15
const double LAYER_LIMIT = 15.95424250943;
const int MAX_SIGNIFICANT_DIGITS = 17;
const double tolEq_TOLERANCE = 1e-7;

const std::vector<double> CRITICAL_HEADERS = {2.0, std::exp(1.0), 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
//the website is down, so I just copied these magic numbers from break_eternity.js, but once it comes back up,
//you can find these numbers from http://myweb.astate.edu/wpaulsen/tetcalc/tetcalc.html
const std::vector<std::vector<double long>> CRITICAL_TET_VALUES = {
	//base 2
	{1.0, 1.0891180521811202527, 1.1789767925673958433, 1.2701455431742086633, 1.3632090180450091941, 1.4587818160364217007, 1.5575237916251418333, 1.6601571006859253673, 1.7674858188369780435, 1.8804192098842727359, 2.0},
	//base e
	{1.0, 1.1121114330934078681, 1.2310389249316089299, 1.3583836963111376089, 1.4960519303993531879, 1.6463542337511945810, 1.8121385357018724464, 1.9969713246183068478, 2.2053895545527544330, 2.4432574483385252544, exp(1.0)},
	//base 3
	{1.0, 1.1187738849693603, 1.2464963939368214, 1.38527004705667, 1.5376664685821402, 1.7068895236551784, 1.897001227148399, 2.1132403089001035, 2.362480153784171, 2.6539010333870774, 3.0},
	//base 4 
	{1.0, 1.1367350847096405, 1.2889510672956703, 1.4606478703324786, 1.6570295196661111, 1.8850062585672889, 2.1539465047453485, 2.476829779693097, 2.872061932789197, 3.3664204535587183, 4.0},
	//base 5
	{1.0, 1.1494592900767588, 1.319708228183931, 1.5166291280087583, 1.748171114438024, 2.0253263297298045, 2.3636668498288547, 2.7858359149579424, 3.3257226212448145, 4.035730287722532, 5.0},
	//base 6
	{1.0, 1.159225940787673, 1.343712473580932, 1.5611293155111927, 1.8221199554561318, 2.14183924486326, 2.542468319282638, 3.0574682501653316, 3.7390572020926873, 4.6719550537360774, 6.0},
	//base 7
	{1.0, 1.1670905356972596, 1.3632807444991446, 1.5979222279405536, 1.8842640123816674, 2.2416069644878687, 2.69893426559423, 3.3012632110403577, 4.121250340630164, 5.281493033448316, 7.0},
	//base 8
	{1.0, 1.1736630594087796, 1.379783782386201, 1.6292821855668218, 1.9378971836180754, 2.3289975651071977, 2.8384347394720835, 3.5232708454565906, 4.478242031114584, 5.868592169644505, 8.0},
	//base 9
	{1.0, 1.1793017514670474, 1.394054150657457, 1.65664127441059, 1.985170999970283, 2.4069682290577457, 2.9647310119960752, 3.7278665320924946, 4.814462547283592, 6.436522247411611, 9.0},
	//base 10
	{1.0, 1.1840100246247336579, 1.4061375836156954169, 1.6802272208863963918, 2.026757028388618927, 2.4770056063449647580, 3.0805252717554819987, 3.9191964192627283911, 5.1351528408331864230, 6.9899611795347148455, 10.0},
};
const std::vector<std::vector<double long>> CRITICAL_SLOG_VALUES = {
	//base 2
	{-1.0, -0.9194161097107025, -0.8335625019330468, -0.7425599821143978, -0.6466611521029437, -0.5462617907227869, -0.4419033816638769, -0.3342645487554494, -0.224140440909962, -0.11241087890006762, 0.0},
	//base e
	{-1.0, -0.90603157029014, -0.80786507256596, -0.7064666939634, -0.60294836853664, -0.49849837513117, -0.39430303318768, -0.29147201034755, -0.19097820800866, -0.09361896280296, 0.0},
	//base 3
	{-1.0, -0.9021579584316141, -0.8005762598234203, -0.6964780623319391, -0.5911906810998454, -0.486050182576545, -0.3823089430815083, -0.28106046722897615, -0.1831906535795894, -0.08935809204418144, 0.0},
	//base 4 
	{-1.0, -0.8917227442365535, -0.781258746326964, -0.6705130326902455, -0.5612813129406509, -0.4551067709033134, -0.35319256652135966, -0.2563741554088552, -0.1651412821106526, -0.0796919581982668, 0.0},
	//base 5
	{-1.0, -0.8843387974366064, -0.7678744063886243, -0.6529563724510552, -0.5415870994657841, -0.4352842206588936, -0.33504449124791424, -0.24138853420685147, -0.15445285440944467, -0.07409659641336663, 0.0},
	//base 6
	{-1.0, -0.8786709358426346, -0.7577735191184886, -0.6399546189952064, -0.527284921869926, -0.4211627631006314, -0.3223479611761232, -0.23107655627789858, -0.1472057700818259, -0.07035171210706326, 0.0},
	//base 7
	{-1.0, -0.8740862815291583, -0.7497032990976209, -0.6297119746181752, -0.5161838335958787, -0.41036238255751956, -0.31277212146489963, -0.2233976621705518, -0.1418697367979619, -0.06762117662323441, 0.0},
	//base 8
	{-1.0, -0.8702632331800649, -0.7430366914122081, -0.6213373075161548, -0.5072025698095242, -0.40171437727184167, -0.30517930701410456, -0.21736343968190863, -0.137710238299109, -0.06550774483471955, 0.0},
	//base 9
	{-1.0, -0.8670016295947213, -0.7373984232432306, -0.6143173985094293, -0.49973884395492807, -0.394584953527678, -0.2989649949848695, -0.21245647317021688, -0.13434688362382652, -0.0638072667348083, 0.0},
	//base 10
	{-1.0, -0.8641642839543857, -0.732534623168535, -0.6083127477059322, -0.4934049257184696, -0.3885773075899922, -0.29376029055315767, -0.2083678561173622, -0.13155653399373268, -0.062401588652553186, 0.0},
};

//only use this shorthand if you are creating 0
static Decimal N() {
	return Decimal();
}
//only use this shorthand when you are sure a is and will not be a massive number
//to make a Decimal of a mag that requires division to notate, ex: 1/3 or 5/12, make sure to notate both numbers as doubles -> 1.0/3.0 or 5.0/12.0
static Decimal I(Decimal::Mag a) {
	return Decimal(a);
}
//use this shorthand if you are returning an existing Decimal object
static Decimal D(Decimal a) {
	Decimal::Sign sign = a.sign;
	Decimal::Layer layer = a.layer;
	Decimal::Mag mag = a.mag;
	return Decimal(sign, layer, mag);
}
//use this shorthand if you are creating a new Decimal object
static Decimal FC(Decimal::Sign sign, Decimal::Layer layer, Decimal::Mag mag) {
	return Decimal(sign, layer, mag);
}

const double long MAGIC_CONSTANT = 1.44466786100976613366;
const double OTHER_MAGIC_CONSTANT = 1.444667861009099;

const Decimal INF = Decimal(1, std::numeric_limits<long long>::max(), std::numeric_limits<double>::infinity());
const Decimal negINF = Decimal(-1, std::numeric_limits<long long>::max(), std::numeric_limits<double>::infinity());

template<typename T>
bool Decimal::isZero(T a) {
	if (a == 0) {
		return true;
	} else {
		return false;
	}
}

template <typename T>
Decimal::Sign Decimal::toSign(T a) {
	if (Decimal::isZero(a)) {
		return 0;
	}
	if (std::signbit(a)) {
		return -1;
	} else {
		return 1;
	}
}

Decimal::Sign Decimal::signFromStr(std::string a) {
	char front = a.front();
	std::string zeroPoint = a.substr(0, 2);
	if (front == '-') {
		return -1;
	}
	if (front == '0' && zeroPoint != "0.") {
		return 0;
	}
	return 1;
}
Decimal::Layer Decimal::layerFromStr(std::string a) {
	std::string::difference_type e = std::count(a.begin(), a.end(), 'e');
	return static_cast<Layer>(e);
}
Decimal::Mag Decimal::magFromStr(std::string a) {
	//if the layer is 0
	char exp = 'e';
	if (a.find(exp) == std::string::npos) {
		return std::stod(a);
	}

	//splits up the string by the e's
	std::stringstream ss(a);
	std::string str;
	std::vector<std::string> strs;
	while (std::getline(ss, str, exp)) {
		strs.push_back(str);
	}
	
	//I am only gonna support a layer of 1 right now, but I think this code works for layers higher than 1
	//xey => new Decimal(1, 1, (y + std::log10(x)));
	//I don't know the math for xeyez, it might be new Decimal(1, 1, (z + std::log10(y) + std::log10(std::log10(x))));
	std::deque<double> values;
	for (int i = 0; i < strs.size(); i++) {
		std::string temp = strs[i];
		if (temp.length() > 0) {
			if (i == 0) {
				if (temp.front() == '-') {
					temp.erase(0, 1);
				}
			}
			if (!temp.empty()) {
				values.push_front(std::stod(temp));
			}
		}
	}
	int queSize = static_cast<int>(values.size());
	for (int i = queSize; i > 0; i--) {
		double val = values[i - 1];
		for (int j = 0; j < (i - 1); j++) {
			if (j < 2) {
				val = std::log10(val);
			} else {
				val = 0;
			}
		}
		values[i - 1] = val;
	}
	Mag totalMag = 0;
	for (int i = 0; i < queSize; i++) {
		totalMag = totalMag + values[i];
	}
	return totalMag;
}


template<typename T>
Decimal::Layer Decimal::layerChecker(T a, Layer b) {
	if (isZero(std::abs(static_cast<double>(a)))) {
		return 0;
	}
	double x = std::abs(static_cast<double>(a));
	double y = std::log10(x);

	if (std::abs(y) > LAYER_LIMIT) {
		return b + 1;
	} else {
		return b;
	}
}

template<typename T>
Decimal::Mag Decimal::setMagBasedOnLayer(T a, Layer b, Sign c) {
	if (b > 0) {
		if (c == 0) {
			return std::nan("");
		}
		return std::log10(a);
	} else {
		return a;
	}
}

Decimal* Decimal::toPtr(Decimal a) {
	return &a;
}
//this in c++ returns a pointer to the object
Decimal* Decimal::toPtr() {
	return this;
}
Decimal Decimal::fromPtr(Decimal* a) {
	return *a;
}
Decimal Decimal::fromPtr() {
	return *this;
}

int Decimal::toInt(Decimal a) {
	constexpr int lowerLimit = std::numeric_limits<int>::lowest();
	constexpr int negInf = -std::numeric_limits<int>::infinity();
	constexpr int upperLimit = std::numeric_limits<int>::max();
	constexpr int inf = std::numeric_limits<int>::infinity();
	double val;

	if (a.layer == 0) {
		val = a.sign * a.mag;
	}
	//this probably won't be triggered much but this is here if for some reason a Decimal is instantantiated with Decimal(1, 1, 3) or something 
	else if (a.layer == 1) {
		val = a.sign * std::pow(10, a.mag);
	} else {
		return a.mag > 0 ? a.sign > 0 ? inf : negInf : 0;
	}
	if (val > upperLimit) {
		return inf;
	} else if (val < lowerLimit) {
		return negInf;
	} else {
		return static_cast<long long>(val);
	}
}
int Decimal::toInt() {
	return toInt(*this);
}
long long Decimal::toLongLong(Decimal a) {
	constexpr long long lowerLimit = std::numeric_limits<long long>::lowest();
	constexpr long long negInf = -std::numeric_limits<long long>::infinity();
	constexpr long long upperLimit = std::numeric_limits<long long>::max();
	constexpr long long inf = std::numeric_limits<long long>::infinity();
	double val;

	if (a.layer == 0) {
		val = a.sign * a.mag;
	}
	//this probably won't be triggered much but this is here if for some reason a Decimal is instantantiated with Decimal(1, 1, 3) or something 
	else if (a.layer == 1) {
		val = a.sign * std::pow(10, a.mag);
	} else {
		return a.mag > 0 ? a.sign > 0 ? inf : negInf : 0;
	}
	if (val > upperLimit) {
		return inf;
	} else if (val < lowerLimit) {
		return negInf;
	} else {
		return static_cast<long long>(val);
	}
}
long long Decimal::toLongLong() {
	return toLongLong(*this);
}
double Decimal::toDouble(Decimal a) {
	constexpr double lowerLimit = std::numeric_limits<double>::lowest();
	constexpr double lowerMin = -std::numeric_limits<double>::min();
	constexpr double negInf = -std::numeric_limits<double>::infinity();
	constexpr double upperLimit = std::numeric_limits<double>::max();
	constexpr double upperMin = std::numeric_limits<double>::min();
	constexpr double inf = -std::numeric_limits<double>::infinity();
	//I really don't like using long double due to it taking up a massive amount of memory but luckily the compiler cleans it up automatically so yay
	long double val;
	if (a.layer == 0) {
		val = a.sign * a.mag;
	} else if (a.layer == 1) {
		val = a.sign * std::pow(10, a.mag);
	} else {
		return a.mag > 0 ? a.sign > 0 ? inf : negInf : 0;
	}
	if (val > upperLimit) {
		return inf;
	} else if (val < lowerLimit) {
		return negInf;
	} else if (val < upperMin && val > 0) {
		return 0;
	} else if (val > lowerMin && val < 0) {
		return 0;
	} else {
		return static_cast<double>(val);
	}
}
double Decimal::toDouble() {
	return toDouble(*this);
}
float Decimal::toFloat(Decimal a) {
	constexpr float lowerLimit = std::numeric_limits<float>::lowest();
	constexpr float lowerMin = -std::numeric_limits<float>::min();
	constexpr float negInf = -std::numeric_limits<float>::infinity();
	constexpr float upperLimit = std::numeric_limits<float>::max();
	constexpr float upperMin = std::numeric_limits<float>::min();
	constexpr float inf = -std::numeric_limits<float>::infinity();
	//I really don't like using long float due to it taking up a massive amount of memory but luckily the compiler cleans it up automatically so yay
	long double val;
	if (a.layer == 0) {
		val = a.sign * a.mag;
	}
	else if (a.layer == 1) {
		val = a.sign * std::pow(10, a.mag);
	}
	else {
		return a.mag > 0 ? a.sign > 0 ? inf : negInf : 0;
	}
	if (val > upperLimit) {
		return inf;
	}
	else if (val < lowerLimit) {
		return negInf;
	}
	else if (val < upperMin && val > 0) {
		return 0;
	}
	else if (val > lowerMin && val < 0) {
		return 0;
	}
	else {
		return static_cast<float>(val);
	}
}
float Decimal::toFloat() {
	return toFloat(*this);
}

double Decimal::d_maglog10(double a) {
	return std::log10(std::abs(a));
}

Decimal Decimal::neg(Decimal a) {
	return FC(-a.sign, a.layer, a.mag);
}
Decimal Decimal::neg() {
	return neg(*this);
}

Decimal Decimal::abs(Decimal a) {
	return a.sign < 0 ? neg(a) : a;
}
Decimal Decimal::abs() {
	return abs(*this);
}

int Decimal::cmp(Decimal a, Decimal b) {
	if (a.sign > b.sign) {
		return 1;
	} else if (b.sign > a.sign) {
		return -1;
	}
	return a.sign * cmpabs(a, b);
}
int Decimal::cmp(Decimal a) {
	return cmp(*this, a);
}
int Decimal::compare(Decimal a, Decimal b) {
	return cmp(a, b);
}
int Decimal::compare(Decimal a) {
	return cmp(*this, a);
}
int Decimal::cmpabs(Decimal a, Decimal b) {
	double layera = a.mag > 0 ? a.layer : -static_cast<long>(a.layer);
	double layerb = b.mag > 0 ? b.layer : -static_cast<long>(a.layer);

	if (layera > layerb) {
		return 1;
	}
	if (layerb > layera) {
		return -1;
	}
	if (a.mag > b.mag) {
		return 1;
	}
	if (b.mag > a.mag) {
		return -1;
	}
	return 1;
}
int Decimal::cmpabs(Decimal a) {
	return cmpabs(*this, a);
}
int Decimal::compareAbs(Decimal a, Decimal b) {
	return cmpabs(a, b);
}
int Decimal::compareAbs(Decimal a) {
	return cmpabs(*this, a);
}

bool Decimal::gt(Decimal a, Decimal b) {
	return cmp(a, b) == 1;
}
bool Decimal::gt(Decimal a) {
	return gt(*this, a);
}
bool Decimal::greaterThan(Decimal a, Decimal b) {
	return gt(a, b);
}
bool Decimal::greaterThan(Decimal a) {
	return gt(*this, a);
}
bool Decimal::gte(Decimal a, Decimal b) {
	return !lt(a, b);
}
bool Decimal::gte(Decimal a) {
	return gte(*this, a);
}
bool Decimal::greaterThanOrEqualTo(Decimal a, Decimal b) {
	return gte(a, b);
}
bool Decimal::greaterThanOrEqualTo(Decimal a) {
	return gte(*this, a);
}

bool Decimal::lt(Decimal a, Decimal b) {
	return cmp(a, b) == -1;
}
bool Decimal::lt(Decimal a) {
	return lt(*this, a);
}
bool Decimal::lessThan(Decimal a, Decimal b) {
	return lt(a, b);
}
bool Decimal::lessThan(Decimal a) {
	return lt(*this, a);
}
bool Decimal::lte(Decimal a, Decimal b) {
	return !gt(a, b);
}
bool Decimal::lte(Decimal a) {
	return lte(*this, a);
}
bool Decimal::lessThanOrEqualTo(Decimal a, Decimal b) {
	return lte(a, b);
}
bool Decimal::lessThanOrEqualTo(Decimal a) {
	return lte(*this, a);
}

Decimal Decimal::max(Decimal a, Decimal b) {
	return gt(a, b) ? a : b;
}
Decimal Decimal::max(Decimal a) {
	return max(*this, a);
}
Decimal Decimal::clampMin(Decimal a, Decimal b) {
	return max(a, b);
}
Decimal Decimal::clampMin(Decimal a) {
	return max(*this, a);
}

Decimal Decimal::maxabs(Decimal a, Decimal b) {
	return cmpabs(a, b) > 0 ? D(b) : D(a);
}
Decimal Decimal::maxabs(Decimal a) {
	return maxabs(*this, a);
}

Decimal Decimal::min(Decimal a, Decimal b) {
	return lt(a, b) ? a : b;
}
Decimal Decimal::min(Decimal a) {
	return min(*this, a);
}
Decimal Decimal::clampMax(Decimal a, Decimal b) {
	return min(a, b);
}
Decimal Decimal::clampMax(Decimal a) {
	return min(*this, a);
}

Decimal Decimal::minabs(Decimal a, Decimal b) {
	return cmpabs(a, b) < 0 ? D(b) : D(a);
}
Decimal Decimal::minabs(Decimal a) {
	return minabs(*this, a);
}

Decimal Decimal::clamp(Decimal a, Decimal b, Decimal c) {
	return min(max(a, b), c);
}
Decimal Decimal::clamp(Decimal a, Decimal b) {
	return clamp(*this, a, b);
}

Decimal Decimal::floor(Decimal a) {
	if (a.mag < 0) {
		return a.sign == -1 ? FC(-1, 0, 1) : N();
	}
	if (a.sign == -1) return neg(ceil(neg(a)));
	else if (a.layer == 0) return FC(a.sign, 0, std::floor(a.mag));
	else if (a.layer == 1) {
		double _mag = std::pow(10.0, a.mag);
		return FC(a.sign, 1, std::log10(std::floor(_mag)));
	} else {
		return a;
	}
}
Decimal Decimal::floor() {
	return floor(*this);
}

Decimal Decimal::ceil(Decimal a) {
	if (a.mag < 0) {
		return a.sign == -1 ? FC(1, 0, 1) : N();
	}
	if (a.sign == 1) return neg(floor(neg(a)));
	else if (a.layer == 0) return FC(a.sign, 0, std::ceil(a.mag));
	else if (a.layer == 1) {
		double _mag = std::pow(10.0, a.mag);
		return FC(a.sign, 1, std::log10(std::ceil(_mag)));
	} else {
		return a;
	}
}
Decimal Decimal::ceil() {
	return ceil(*this);
}


bool Decimal::eq(Decimal a, Decimal b) {
	return a.sign == b.sign && a.layer == b.layer && a.mag == b.mag;
}
bool Decimal::eq(Decimal a) {
	return eq(*this, a);
}
bool Decimal::equal(Decimal a, Decimal b) {
	return eq(a, b);
}
bool Decimal::equal(Decimal a) {
	return eq(*this, a);
}
bool Decimal::equals(Decimal a, Decimal b) {
	return eq(a, b);
}
bool Decimal::equals(Decimal a) {
	return eq(*this, a);
}
bool Decimal::equality(Decimal a, Decimal b) {
	return eq(a, b);
}
bool Decimal::equality(Decimal a) {
	return eq(*this, a);
}

bool Decimal::tolEq(Decimal a, Decimal b, double c) {
	if (a.sign != b.sign || std::abs(static_cast <double>(a.layer - b.layer)) > 0) {
		return false;
	}
	double magA = a.mag;
	double magB = b.mag;
	if (a.layer > b.layer) {
		magB = toSign(magB) * d_maglog10(magB);
	} else if (a.layer < b.layer) {
		magA = toSign(magA) * d_maglog10(magA);
	}
	return std::abs(magA - magB) <= c * std::max(std::abs(magA), std::abs(magB));
}
bool Decimal::tolEq(Decimal a, double b) {
	return tolEq(*this, a, b);
}
bool Decimal::tolEq(Decimal a, Decimal b) {
	return tolEq(a, b, tolEq_TOLERANCE);
}
bool Decimal::tolEq(Decimal a) {
	return tolEq(*this, a);
}
bool Decimal::tolerantEqual(Decimal a, Decimal b, double c) {
	return tolEq(a, b, c);
}
bool Decimal::tolerantEqual(Decimal a, double b) {
	return tolEq(*this, a, b);
}
bool Decimal::tolerantEqual(Decimal a, Decimal b) {
	return tolEq(a, b);
}
bool Decimal::tolerantEqual(Decimal a) {
	return tolEq(*this, a);
}
bool Decimal::tolerantEquals(Decimal a, Decimal b, double c) {
	return tolEq(a, b, c);
}
bool Decimal::tolerantEquals(Decimal a, double b) {
	return tolEq(*this, a, b);
}
bool Decimal::tolerantEquals(Decimal a, Decimal b) {
	return tolEq(a, b);
}
bool Decimal::tolerantEquals(Decimal a) {
	return tolEq(*this, a);
}
bool Decimal::tolerantEquality(Decimal a, Decimal b, double c) {
	return tolEq(a, b, c);
}
bool Decimal::tolerantEquality(Decimal a, double b) {
	return tolEq(*this, a, b);
}
bool Decimal::tolerantEquality(Decimal a, Decimal b) {
	return tolEq(a, b);
}
bool Decimal::tolerantEquality(Decimal a) {
	return tolEq(*this, a);
}

bool Decimal::tolNeq(Decimal a, Decimal b, double c) {
	return !tolEq(a, b, c);
}
bool Decimal::tolNeq(Decimal b, double c) {
	return !tolEq(*this, b, c);
}
bool Decimal::tolNeq(Decimal a, Decimal b) {
	return !tolEq(a, b, 1e-7);
}
bool Decimal::tolNeq(Decimal b) {
	return !tolEq(*this, b, 1e-7);
}


Decimal Decimal::recip(Decimal a) {
	try {
		if (a.sign == 0) {
			throw std::domain_error("Division by 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "\033[31m" << "Domain error: " << e.what() << "\033[0m" << std::endl;
	}
	if (a.layer == 0) {
		return FC(a.sign, 0, 1 / a.mag);
	} else {
		return FC(a.sign, a.layer, -a.mag);
	}
}
Decimal Decimal::recip() {
	return recip(*this);
}
Decimal Decimal::reciprocal(Decimal a) {
	return recip(a);
}
Decimal Decimal::reciprocal() {
	return recip(*this);
}
Decimal Decimal::reciprocate(Decimal a) {
	return recip(a);
}
Decimal Decimal::reciprocate() {
	return recip(*this);
}
Decimal Decimal::inv(Decimal a) {
	return recip(a);
}
Decimal Decimal::inv() {
	return recip(*this);
}
Decimal Decimal::inverse(Decimal a) {
	return recip(a);
}
Decimal Decimal::inverse() {
	return recip(*this);
}
Decimal Decimal::invert(Decimal a) {
	return recip(a);
}
Decimal Decimal::invert() {
	return recip(*this);
}

Decimal Decimal::add(Decimal a, Decimal b) {
	Mag x = 0.0;
	if (a.sign == 0) {
		return D(b);
	}
	if (b.sign == 0) {
		return D(a);
	}
	if ((a.sign == -b.sign) && (a.layer == b.layer) && (a.mag == b.mag)) {
		return N();
	}

	if (a.layer >= 2 || b.layer >= 2) {
		return maxabs(a, b);
	}

	//this is a cool way to save time on writing code
	Decimal A;
	Decimal B;
	if (cmpabs(a, b) > 0) {
		A = a;
		B = b;
	} else {
		B = a;
		A = b;
	}

	//typical case of addition
	if (A.layer == 0 && B.layer == 0) {
		x = (A.mag * A.sign) + (B.mag * B.sign);
		return I(x);
	}
	int layera = A.layer * toSign(A.mag);
	int layerb = B.layer * toSign(B.mag);

	if (layera - layerb >= 2) {
		return D(A);
	}

	if (layera == 0 && layerb == -1) {
		x = B.mag - std::log10(A.mag);
		if (std::abs(x) > MAX_SIGNIFICANT_DIGITS) {
			return D(A);
		} else {
			double _magdiff = std::pow(10, -x);
			double _mantissa = B.sign + (A.sign * _magdiff);
			return FC(toSign(_mantissa), 1, B.mag + d_maglog10(_mantissa));
		}
	}

	if (layera == 1 && layerb == 0) {
		x = A.mag - std::log10(B.mag);
		if (std::abs(x) > MAX_SIGNIFICANT_DIGITS) {
			return D(A);
		} else {
			double _magdiff = std::pow(10, x);
			double _mantissa = B.sign + (A.sign * _magdiff);
			return FC(toSign(_mantissa), 1, std::log10(B.mag) + d_maglog10(_mantissa));
		}
	}

	x = A.mag - B.mag;
	if (std::abs(x) > MAX_SIGNIFICANT_DIGITS) {
		return D(A);
	} else {
		double _magdiff = std::pow(10, x);
		double _mantissa = B.sign + (A.sign * _magdiff);
		return FC(toSign(_mantissa), 1, B.mag + d_maglog10(_mantissa));
	}

	try {
		throw std::invalid_argument("Invalid arguments for addition");
	}
	catch (const std::invalid_argument e) {
		std::cerr << "\033[31m" << "Invalid argument error: " << e.what() << "\033[0m" << std::endl;
	}
}
Decimal Decimal::add(Decimal a) {
	return add(*this, a);
}
Decimal Decimal::plus(Decimal a, Decimal b) {
	return add(a, b);
}
Decimal Decimal::plus(Decimal a) {
	return add(*this, a);
}
Decimal Decimal::sub(Decimal a, Decimal b) {
	return add(a, neg(b));
}
Decimal Decimal::sub(Decimal a) {
	return sub(*this, a);
}
Decimal Decimal::subtract(Decimal a, Decimal b) {
	return sub(a, b);
}
Decimal Decimal::subtract(Decimal a) {
	return sub(*this, a);
}
Decimal Decimal::minus(Decimal a, Decimal b) {
	return sub(a, b);
}
Decimal Decimal::minus(Decimal a) {
	return sub(*this, a);
}

Decimal Decimal::mul(Decimal a, Decimal b) {
	if (a.sign == 0 || b.sign == 0) {
		return N();
	}
	if (a.layer == b.layer && a.mag == -b.mag) {
		return FC(a.sign * b.sign, 0, 1);
	}

	Decimal A;
	Decimal B;
	if (a.layer > b.layer || (a.layer == b.layer && std::abs(a.mag) > std::abs(b.mag))) {
        A = a;
        B = b;
    } else {
        A = b;
        B = a;
    }
    
	//typical case of multiplication
    if (A.layer == 0 && B.layer == 0) {
        return I(A.sign * B.sign * A.mag * B.mag);
    }
	if (A.layer >= 3 || A.layer - B.layer == 2) {
		return FC(A.sign * B.sign, A.layer, A.mag);
	}
	if (A.layer == 1 && B.layer == 0) {
		return FC(A.sign * B.sign, 1, A.mag + std::log10(B.mag));
	}
	if (A.layer == 1 && B.layer == 1) {
		return FC(A.sign * B.sign, 1, A.mag + B.mag);
	}
	if ((A.layer == 2 && B.layer == 1) || (A.layer == 2 && B.layer == 2)) {
		Decimal _newDecimal = add(FC(toSign(A.mag), A.layer - 1, std::abs(A.mag)), (FC(toSign(B.mag), B.layer - 1, std::abs(B.mag))));
		return FC(A.sign * B.sign, _newDecimal.layer + 1, _newDecimal.sign * _newDecimal.mag);
	}
	try {
		throw std::invalid_argument("Invalid arguments for multiplication");
	} catch(const std::invalid_argument e) {
		std::cerr << "\033[31m" << "Invalid argument error: " << e.what() << "\033[0m" << std::endl;
		return N();
	}
}
Decimal Decimal::mul(Decimal a) {
	return mul(*this, a);
}
Decimal Decimal::mult(Decimal a, Decimal b) {
	return mul(a, b);
}
Decimal Decimal::mult(Decimal a) {
	return mul(*this, a);
}
Decimal Decimal::times(Decimal a, Decimal b) {
	return mul(a, b);
}
Decimal Decimal::times(Decimal a) {
	return mul(*this, a);
}
Decimal Decimal::multiply(Decimal a, Decimal b) {
	return mul(a, b);
}
Decimal Decimal::multiply(Decimal a) {
	return mul(*this, a);
}

Decimal Decimal::div(Decimal a, Decimal b) {
	return mul(a, recip(b));
}
Decimal Decimal::div(Decimal a) {
	return div(*this, a);
}
Decimal Decimal::divide(Decimal a, Decimal b) {
	return div(a, b);
}
Decimal Decimal::divide(Decimal a) {
	return div(*this, a);
}
Decimal Decimal::divideBy(Decimal a, Decimal b) {
	return div(a, b);
}
Decimal Decimal::divideBy(Decimal a) {
	return div(*this, a);
}
Decimal Decimal::dividedBy(Decimal a, Decimal b) {
	return div(a, b);
}
Decimal Decimal::dividedBy(Decimal a) {
	return div(*this, a);
}
Decimal Decimal::sr(Decimal a, Decimal b) {
	constexpr long long lowerLimit = std::numeric_limits<long long>::lowest() / 2;
	constexpr long long upperLimit = std::numeric_limits<long long>::max() / 2;
	double temp{};
	bool _outOfRange = false;
	long long _value{};

	if (a.layer == 0) {
		temp = a.sign * a.mag;
	}
	//this probably won't be triggered much but this is here if for some reason a Decimal is instantantiated with Decimal(1, 1, 3) or something 
	else if (a.layer == 1) {
		temp = a.sign * std::pow(10, a.mag);
	}
	else {
		if (a.sign == 0 && a.mag == 0) {
			_value = 0;
		}
		else {
			_outOfRange = true;
		}
	}
	if (temp > upperLimit) {
		_outOfRange = true;
	}
	else if (temp < lowerLimit) {
		_outOfRange = true;
	}
	else {
		_value = static_cast<long long>(temp);
	}

	int _ops = toInt(b);
	if (_ops >= std::log2(_value)) {
		_outOfRange = true;
	}

	if (!_outOfRange) {
		return FC(a.sign, 0, static_cast<double>(_value >> _ops));
	}
	return div(a, pow2(b));
}
Decimal Decimal::sr(Decimal a) {
	return sr(*this, a);
}
Decimal Decimal::rs(Decimal a, Decimal b) {
	return sr(a, b);
}
Decimal Decimal::rs(Decimal a) {
	return sr(*this, a);
}

Decimal Decimal::log(Decimal a, Decimal b) {
	try {
		if (b.sign == 0) {
			throw std::domain_error("log base 0");
		} else if (b.sign == 1 && b.layer == 0 && b.mag == 1) {
			throw std::domain_error("log base 1");
		} else if (a.sign == 0) {
			throw std::domain_error("log 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "\033[31m" << "Domain error: " << e.what() << "\033[0m" << std::endl;
	}
	//change of base formula :)
	return div(log10(a), log10(b));
}
Decimal Decimal::log(Decimal a) {
	return log(*this, a);
}
Decimal Decimal::absLog10(Decimal a) {
	try {
		if (a.sign == 0) {
			throw std::domain_error("log 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "\033[31m" << "Domain error: " << e.what() << "\033[0m" << std::endl;
	}
	if (a.layer > 0) {
		return FC(toSign(a.mag), a.layer - 1, std::abs(a.mag));
	} else {
		return FC(1, 0, std::log10(a.mag));
	}
}
Decimal Decimal::absLog10() {
	return absLog10(*this);
}
Decimal Decimal::log10(Decimal a) {
	try {
		if (a.sign == 0) {
			throw std::domain_error("log 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "\033[31m" << "Domain error: " << e.what() << "\033[0m" << std::endl;
	}
	if (a.layer > 0) {
		return FC(toSign(a.mag), a.layer - 1, std::abs(a.mag));
	}
	else {
		return FC(a.sign, 0, std::log10(a.mag));
	}
}
Decimal Decimal::log10() {
	return log10(*this);
}
Decimal Decimal::lg(Decimal a) {
	try {
		if (a.sign == 0) {
			throw std::domain_error("lg 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "\033[31m" << "Domain error: " << e.what() << "\033[0m" << std::endl;
	}
	if (a.layer == 0) {
		return FC(a.sign, 0, std::log2(a.mag));
	} else if (a.layer == 1) {
		double _lg10 = 3.321928094887362;
		return FC(toSign(a.mag), 0, std::abs(a.mag) + _lg10);
	} else if (a.layer == 2) {
		double _minusloglog2 = 0.5213902276543247;
		return FC(toSign(a.mag), 1, std::abs(a.mag) + _minusloglog2);
	} else {
		return FC(toSign(a.mag), a.layer - 1, std::abs(a.mag));
	}
}
Decimal Decimal::lg() {
	return lg(*this);
}
Decimal Decimal::log2(Decimal a) {
	return lg(a);
}
Decimal Decimal::log2() {
	return lg(*this);
}
Decimal Decimal::ln(Decimal a) {
	try {
		if (a.sign == 0) {
			throw std::domain_error("ln 0");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "\033[31m" << "Domain error: " << e.what() << "\033[0m" << std::endl;
	}
	if (a.layer == 0) {
		return FC(a.sign, 0, std::log(a.mag));
	} else if (a.layer == 1) {
		double _ln10 = 2.302585092994046;
		return FC(toSign(a.mag), 0, std::abs(a.mag) * _ln10);
	} else if (a.layer == 2) {
		double _logloge = 0.36221568869946325;
		return FC(toSign(a.mag), 1, std::abs(a.mag) + _logloge);
	} else {
		return FC(toSign(a.mag), a.layer - 1, std::abs(a.mag));
	}
}
Decimal Decimal::ln() {
	return ln(*this);
}

Decimal Decimal::pow(Decimal a, Decimal b) {
	if (a.sign == 0) {
		return b.sign == 0 ? FC(1, 0, 1) : N();
	}
	if (b.sign == 0) {
		return FC(1, 0, 1);
	}
	if (a.sign == 1 && a.layer == 0 && a.mag == 1) {
		return a;
	}
	if (b.sign == 1 && b.layer == 0 && b.mag == 1) {
		return a;
	}

	//to be honest, I have no clue what patashu is doing here, but I will add it
	Decimal _result = pow10(mul(absLog10(a), b));
	if (a.sign < 0) {
		long long _modulo = std::abs(toLongLong(b) % 2) % 2;
		if (_modulo == 1) {
			return neg(_result);
		} else if (_modulo == 0) {
			return _result;
		} else {
			return N();
		}
	}
	return _result;
}
Decimal Decimal::pow(Decimal a) {
	return pow(*this, a);
}
Decimal Decimal::power(Decimal a, Decimal b) {
	return pow(a, b);
}
Decimal Decimal::power(Decimal a) {
	return pow(*this, a);
}
Decimal Decimal::raise(Decimal a, Decimal b) {
	return pow(a, b);
}
Decimal Decimal::raise(Decimal a) {
	return pow(*this, a);
}
Decimal Decimal::pow10(Decimal a) {
	Decimal b = a, gtTen1024;
	if (a.layer == 0) {
		double _newmag{};
		if (a.mag >= 308) {
			return FC(a.sign, 1, a.mag);
		} else _newmag = std::pow(10, a.sign * a.mag);
		if (std::abs(_newmag) >= 0.1) {
			return FC(1, 0, _newmag);
		} else {
			if (a.sign == 0) {
				return FC(1, 0, 1);
			} else {
				b = FC(a.sign, a.layer + 1, std::log10(a.mag));
			}
		}
	}
	
	if (b.sign > 0 && b.mag >= 0) {
		return FC(b.sign, b.layer + 1, b.mag);
	} else if (b.sign < 0 && b.mag >= 0) {
		return FC(-b.sign, b.layer + 1, -b.mag);
	}
	return FC(1, 0, 1);
};
Decimal Decimal::pow10() {
	return pow10(*this);
}
Decimal Decimal::power10(Decimal a) {
	return pow10(a);
}
Decimal Decimal::power10() {
	return pow10(*this);
}
Decimal Decimal::raise10(Decimal a) {
	return pow10(a);
}
Decimal Decimal::raise10() {
	return pow10(*this);
}
Decimal Decimal::exp10(Decimal a) {
	return pow10(a);
}
Decimal Decimal::exp10() {
	return pow10(*this);
}
Decimal Decimal::pow2(Decimal a) {
	if (a.sign == 0) {
		return FC(1, 0, 1);
	}
	if (a.sign == 1 && a.layer == 0 && a.mag == 1) {
		return FC(1, 0, 2);
	}
	//this would be really easy if the layers worked in base 2, but they work in base 10 so it's like 20x more annoying
	double _log2 = 0.30102999566;
	if (a.layer == 0) {
		return FC(1, 1, a.sign * _log2 * a.mag);
	} else if (a.layer == 1) {
		return pow10(mul(a.sign, std::log10(_log2) + a.mag));
	} else {
		return FC(1, a.layer + 1, a.sign * a.mag);
	}
};
Decimal Decimal::pow2() {
	return pow2(*this);
}
Decimal Decimal::power2(Decimal a) {
	return pow2(a);
}
Decimal Decimal::power2() {
	return pow2(*this);
}
Decimal Decimal::raise2(Decimal a) {
	return pow2(a);
}
Decimal Decimal::raise2() {
	return pow2(*this);
}
Decimal Decimal::exp2(Decimal a) {
	return pow2(a);
}
Decimal Decimal::exp2() {
	return pow2(*this);
}
Decimal Decimal::sl(Decimal a, Decimal b) {
	constexpr long long lowerLimit = std::numeric_limits<long long>::lowest() / 2; 
	constexpr long long upperLimit = std::numeric_limits<long long>::max() / 2;
	double temp{};
	bool _outOfRange = false;
	long long _value{};

	if (a.layer == 0) {
		temp = a.sign * a.mag;
	}
	//this probably won't be triggered much but this is here if for some reason a Decimal is instantantiated with Decimal(1, 1, 3) or something 
	else if (a.layer == 1) {
		temp = a.sign * std::pow(10, a.mag);
	} else {
		if (a.sign == 0 && a.mag == 0) {
			_value = 0;
		} else {
			_outOfRange = true;
		}
	} 
	if (temp > upperLimit) {
		_outOfRange = true;
	} else if (temp < lowerLimit) {
		_outOfRange = true;
	} else {
		_value = static_cast<long long>(temp);
	}

	int _ops = toInt(b);
	if (_ops >= (63 - std::log2(_value))) {
		_outOfRange = true;
	}
	
	if (!_outOfRange) {
		return FC(a.sign, 0, static_cast<double>(_value << _ops));
	}
	return mul(a, pow2(b));
}
Decimal Decimal::sl(Decimal a) {
	return sl(*this, a);
}
Decimal Decimal::ls(Decimal a, Decimal b) {
	return sl(a, b);
}
Decimal Decimal::ls(Decimal a) {
	return sl(*this, a);
}
Decimal Decimal::exp(Decimal a) {
	if (a.sign == 0) {
		return FC(1, 0, 1);
	}
	if (a.sign == 1 && a.layer == 0 && a.mag == 1) {
		return FC(1, 0, std::exp(1));
	}
	//this would be really easy if the layers worked in base e, but they work in base 10 so it's like (10e)x more annoying
	double _logE = 0.4342944819;
	if (a.layer == 0) {
		return FC(1, 1, a.sign * _logE * a.mag);
	} else if (a.layer == 1) {
		return pow10(mul(a.sign, std::log10(_logE) + a.mag));
	} else {
		return FC(1, a.layer + 1, a.sign * a.mag);
	}
}
Decimal Decimal::exp() {
	return exp(*this);
}
Decimal Decimal::powE(Decimal a) {
	return exp(a);
}
Decimal Decimal::powE() {
	return exp(*this);
}
Decimal Decimal::powerE(Decimal a) {
	return exp(a);
}
Decimal Decimal::powerE() {
	return exp(*this);
}
Decimal Decimal::raiseE(Decimal a) {
	return exp(a);
}
Decimal Decimal::raiseE() {
	return exp(*this);
}

Decimal Decimal::sqrt(Decimal a) {
	return pow(a, I(0.5));
}
Decimal Decimal::sqrt() {
	return sqrt(*this);
}
Decimal Decimal::root(Decimal a) {
	return sqrt(a);
}
Decimal Decimal::root() {
	return sqrt(*this);
}

Decimal Decimal::cbrt(Decimal a) {
	return pow(a, I(1.0 / 3.0));
}
Decimal Decimal::cbrt() {
	return cbrt(*this);
}

//W(1, 0)
double _omega = 0.567143290409783;
//-1/e
double _expm1 = 0.367879441171442;

Decimal Decimal::lambertw(Decimal a, bool b) {
	try {
		if (lt(a, I(_expm1))) {
			throw std::domain_error("lambert W function of something less than -1/e");
		}
		//W_0 branch
		else if (b) {
			if (lt(abs(a), FC(1, 1, -300))) {
				return a;
			} else if (a.mag < 0) {
				return I(d_lambertw(toDouble(a)));
			} else if (a.layer == 0) {
				return I(d_lambertw(a.sign * a.mag));
			} else if (lt(a, FC(1, 3, 15))) {
				return d_lambertw(a);
			} else {
				return ln(a);
			}
		} 
		//W_-1 branch
		else {
			if (a.sign == 1) {
				throw std::domain_error("lambert W_-1 function of something > 0");
			} else if (a.layer == 0) {
				return I(d_lambertw(a.sign * a.mag, false));
			} else if (a.layer == 1) {
				return d_lambertw(a, false);
			} else {
				return neg(lambertw(recip(neg(a))));
			}
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "\033[31m" << "Domain error: " << e.what() << "\033[0m" << std::endl;
	}
}
Decimal Decimal::lambertw(bool b) {
	return lambertw(*this, b);
}
Decimal Decimal::lambertw(Decimal a) {
	return lambertw(a, true);
}
Decimal Decimal::lambertw() {
	return lambertw(*this, true);
}

double Decimal::d_lambertw(double a, double b, bool c) {
	if (a == std::numeric_limits<double>::infinity() || a == -std::numeric_limits<double>::infinity()) {
		return a;
	}
	double w, wn;
	if (c) {
		if (a == 0) {
			return a;
		} else if (a == 1) {
			return _omega;
		}
		if (a < 10) {
			w = 0;
		} else {
			w = std::log(a) - std::log(std::log(a));
		}
	} else {
		if (a == 0) {
			return -std::numeric_limits<double>::infinity();
		}
		if (a <= 0.1) {
			w = -2;
		} else {
			w = std::log(-a) - std::log(std::log(-a));
		}
	}
	for (int i = 0; i < 100; ++i) {
		wn = (a * std::exp(-w) + w * w) / (w + 1);
		if (std::abs(wn - w) < b * std::abs(wn)) {
			return wn;
		} else {
			w = wn;
		}
	}
}
double Decimal::d_lambertw(double a, double b) {
	return d_lambertw(a, b, true);
}
double Decimal::d_lambertw(double a, bool c) {
	return d_lambertw(a, 1e-10, c);
}
double Decimal::d_lambertw(double a) {
	return d_lambertw(a, 1e-10, true);
}
Decimal Decimal::d_lambertw(Decimal a, double b, bool c) {
	Decimal w, wn, wewz, ew;
	if (c) {
		if (tolEq(a, N(), b)) {
			return a;
		}
		else if (tolEq(a, FC(1, 0, 1), b)) {
			return I(_omega);
		}
		w = ln(a);
	}
	else {
		try {
			if (tolEq(a, N(), b)) {
				throw std::domain_error("lambert W_-1 function of something > 0");
			}
		}
		catch (const std::domain_error e) {
			std::cerr << "\033[31m" << "Domain error: " << e.what() << "\033[0m" << std::endl;
		}
		w = ln(neg(a));
	}
	for (int i = 0; i < 100; ++i) {
		ew = exp(neg(w));
		wewz = sub(w, mul(a, ew));
		wn = sub(w, div(wewz, sub(add(w, 1), div(mul(wewz, add(w, 2)), add(mul(w, 2), 2)))));

		if (lt(abs(sub(wn, w)), mul(abs(wn), b))) {
			return wn;
		} else {
			w = wn;
		}
	}
}
Decimal Decimal::d_lambertw(Decimal a, double b) {
	return d_lambertw(a, b, true);
}
Decimal Decimal::d_lambertw(Decimal a, bool c) {
	return d_lambertw(a, 1e-10, c);
}
Decimal Decimal::d_lambertw(bool c) {
	return d_lambertw(*this, 1e-10, c);
}
Decimal Decimal::d_lambertw(Decimal a) {
	return d_lambertw(a, 1e-10, true);
}
Decimal Decimal::d_lambertw() {
	return d_lambertw(*this, 1e-10, true);
}

double Decimal::c_tet(double a, double b, std::vector<std::vector<double long>> c) {
	double _base = a;
	double _height = b * 10;
	//c_tet has a range of 0 <= x <= 10, x is int only
	if (_base < 0) {
		_base = 0;
	}
	else if (_base > 10) {
		_base = 10;
	}
	if (_height < 0) {
		_height = 0;
	}
	else if (_height > 10) {
		_height = 10;
	}

	//for bases that are not integers, we kind of interpolate between values of them 
	long double _lower{}, _upper{};
	for (int i = 0; i < CRITICAL_HEADERS.size(); ++i) {
		if (CRITICAL_HEADERS[i] == _base) {
			_lower = c[i][std::floor(_height)];
			_upper = c[i][std::ceil(_height)];
			break;
		}
		else if (CRITICAL_HEADERS[i] < _base && CRITICAL_HEADERS[i + 1] > _base) {
			double _fracBase = (_base - CRITICAL_HEADERS[i]) / (CRITICAL_HEADERS[i + 1] - CRITICAL_HEADERS[i]);
			_lower = c[i][std::floor(_height)] * (1 - _fracBase) + c[i + 1][std::floor(_height)] * _fracBase;
			_upper = c[i][std::ceil(_height)] * (1 - _fracBase) + c[i + 1][std::ceil(_height)] * _fracBase;
			break;
		}
	}
	double _frac = _height - std::floor(_height);
	if (_lower <= 0 || _upper <= 0) {
		return _lower * (1 - _frac) + _upper * _frac;
	}
	else {
		return std::pow(_base, std::log(_lower) / std::log(_base) * (1 - _frac) + std::log(_upper) / std::log(_base) * _frac);
	}
}


//I use eq() here due to every decimal place mattering for tetration
Decimal Decimal::tet(Decimal a, Decimal b, Decimal c, bool d) {
	if (b.sign == 1 && b.layer == 0 && b.mag == 1) {
		return pow(a, c);
	} else if (b.sign == 0) {
		return c;
	} else if (eq(a, FC(1, 0, 1))) {
		return FC(1, 0, 1);
	} else if (eq(a, FC(-1, 0, 1))) {
		return pow(a, c);
	}

	double _height = b.toDouble();
	//we define 0^0 as 1, so 0^^x flip flops from 0 to 1 over and over again
	if (a.sign == 0) {
		double _result = std::abs(std::fmod(_height + 1, 2));
		if (_result > 1) {
			_result = 2 - _result;
		}
		return I(_result);
	}
	if (_height < 0) {
		return itLog(c, a, neg(b), d);
	}
	double _a = a.toDouble();
	double _truncHeight = std::trunc(_height);
	double _fracHeight = _height - _truncHeight;
	Decimal _payload = c;

	//time for computation hell
	if (_a > 0 && (_a < 1 || _a < MAGIC_CONSTANT) && lte(c, div(lambertw(neg(ln(a)), false), neg(ln(a)))) && (_height > 10000 || !d)) {
		int _limitHeight = std::min(10000, static_cast<int>(_height));
		if (eq(_payload, N())) {
			_payload = pow(a, _fracHeight);
		} else if (lt(a, FC(1, 0, 1))) {
			_payload = mul(pow(_payload, I(1.0 - _fracHeight)), pow(a, pow(_payload, _fracHeight)));
		}
		for (int i = 0; i < _limitHeight; ++i) {
			Decimal _oldPayload = _payload;
			_payload = pow(a, _payload);
			//case if it converges early
			if (eq(_oldPayload, _payload)) {
				return _payload;
			}
		}
		return _payload;
	}
	if (_fracHeight != 0) {
		if (eq(_payload, FC(1, 0, 1))) {
			if (gt(a, FC(1, 0, 10)) || d) {
				_payload = pow(a, I(_fracHeight));
			} else {
				_payload = I(c_tet(toDouble(a), _fracHeight, CRITICAL_TET_VALUES));
				if (lt(a, FC(1, 0, 2))) {
					_payload = plus(mul(sub(_payload, FC(1, 0, 1)), sub(a, FC(1, 0, 1))), FC(1, 0, 1));
				}
			}
		} else {
			if (eq(a, FC(1, 0, 10))) _payload = ladd10(_payload, I(_fracHeight));
			else if (lt(a, FC(1, 0, 1))) _payload = mul(pow(_payload, I(1 - _fracHeight)), pow(pow(a, _payload), _fracHeight));
			else _payload = ladd(_payload, _fracHeight, a, d);
		}
	}

	//massive loop here
	for (int i = 0; i < _truncHeight; ++i) {
		_payload = pow(a, _payload);
		if (gte(_payload, INF)) return INF;
		if (_payload.layer - a.layer > 3) return FC(_payload.sign, _payload.layer + (_truncHeight - i - 1), _payload.mag);
		//escape if the loop is gonna go forever
		if (i > 10000) return _payload;
	}
	return _payload;
}

//{layer offset, resulting mag}
std::vector<double, std::allocator<double>> Decimal::d_ladd(double a, double b) {
	if (a + b > 9e15) {
		return { 1.0, std::log10(a + b - EXP_LIMIT) };
	} else if (a + b < -EXP_LIMIT) {
		return { -1.0, -std::log10(std::abs(a + b + EXP_LIMIT)) };
	} else if (a + b == EXP_LIMIT) {
		return { 1.0, 0.0 };
	} else if (a + b == -EXP_LIMIT) {
		return { -1.0, 0.0 };
	} else {
		return { 0.0, a + b };
	}
}

//my awesome custom function, I hope this is faster than BE's layeradd10
Decimal Decimal::ladd10(Decimal a, Decimal b) {
	double _fracHeight = toDouble(sub(abs(b), floor(abs(b))));
	if (eq(_fracHeight, N())) {
		return FC(a.mag, a.layer + toLongLong(b), a.mag);
	}
	long long _baseHeight = toLongLong(floor(abs(b)));
	//I knew LAYER_LIMIT wouldn't be useless :)
	Decimal _payload = FC(b.sign, _baseHeight, toSign(b.mag) * (std::pow(10, LAYER_LIMIT * _fracHeight) - 1.0));
	std::vector<double, std::allocator<double>> _resultMag = d_ladd(a.mag, _payload.mag);

	//primary logic
	if (_payload.sign == -a.sign && (_payload.sign != 0 || a.sign != 0)) {
		if (gt(abs(_payload), abs(a))) {
			//since std::abs(x) does not support long long, I have to emulate it
			//_payload.layer - a.layer will never be < 0 due to _payload > a
			if (_payload.layer - a.layer > 0) {
				return FC(_payload.sign * a.sign, (_payload.layer - a.layer + _resultMag[0] > -1) ? _payload.layer - a.layer + _resultMag[0] : 0, _resultMag[1]);
			} else {
				return FC(_payload.sign * a.sign, _resultMag[0], _resultMag[1]);
			}
		} else if (eq(abs(_payload), abs(a))) {
			return N();
		} else {
			return FC(_payload.sign * a.sign, (_payload.layer - a.layer + _resultMag[0] > -1) ? _payload.layer - a.layer + _resultMag[0] : 0, _resultMag[1]);
		}
	} else if (_payload.sign == a.sign && (_payload.sign != 0 || a.sign != 0)) {
		return FC(a.sign, (_payload.layer + a.layer + _resultMag[0] > -1) ? _payload.layer + a.layer + _resultMag[0] : 0, _resultMag[1]);
	} else if (_payload.sign == 0) {
		return a;
	} else {
		return _payload;
	}
}
Decimal Decimal::ladd10(Decimal b) {
	return ladd10(*this, b);
};
Decimal Decimal::lsub10(Decimal a, Decimal b) {
	return ladd10(a, neg(b));
};
Decimal Decimal::lsub10(Decimal b) {
	return ladd10(*this, neg(b));
};


Decimal Decimal::ladd(Decimal a, Decimal b, Decimal c, bool d) {
	if (gt(c, FC(1, 0, 1)) && lte(c, FC(1, 0, MAGIC_CONSTANT))) {
		//horrible std::variant stuff
		std::vector<std::variant<Decimal, int>> _init = e_slog(a, c, d);
		int _range = std::get<int>(_init[1]);
		Decimal _slogDest = add(std::get<Decimal>(_init[0]), b);
		Decimal _negln = neg(ln(c)), _lower = div(lambertw(_negln), _negln), _upper = div(lambertw(_negln, false), _negln); 

		Decimal _slogZero = FC(1, 0, 1), _slogOne = pow(c, _slogZero);
		//geometric mean
		if (_range == 1) _slogZero = sqrt(mul(_lower, _upper));
		else if (_range == 0) _slogZero = mul(_upper, FC(1, 0, 2));

		Decimal _baseHeight = floor(_slogDest), _fracHeight = sub(_slogDest, _baseHeight);
		Decimal _towerTop = mul(pow(_slogZero, sub(FC(1, 0, 1), _fracHeight)), pow(_slogOne, _fracHeight));
		return tet(c, _baseHeight, _towerTop, d);
	}

	Decimal _slogDest = add(slog(a, c, 100, d), b);
	if (gte(_slogDest, N())) return tet(c, _slogDest, FC(1, 0, 1), d);
	else if (gte(_slogDest, FC(-1, 0, 1))) return log(tet(c, add(_slogDest, FC(1, 0, 1)), FC(1, 0, 1), d), c);
	Decimal step1 = tet(c, add(_slogDest, FC(1, 0, 2)), FC(1, 0, 1), d);
	return log(log(step1, c), c);
}

Decimal Decimal::slog(Decimal a, Decimal b, int c, bool d) {
	double _stepSize = 0.001, _result = toDouble(d_slog(a, b, 100, d));
	Decimal _payload = tet(a, b, FC(1, 0, 1), d);
	bool _changedDirection = false, _previouslyRose = false;

	for (int i = 1; i < c; ++i) {
		_payload = tet(b, I(_result), FC(1, 0, 1), d);
		bool _currentlyRose = gt(_payload, a);
		if (i > 1) if (_previouslyRose != _currentlyRose) _changedDirection = true;
		_previouslyRose = _currentlyRose;

		if (_changedDirection) _stepSize /= 2;
		else _stepSize *= 2;
		_stepSize = std::abs(_stepSize) * (_currentlyRose ? -1 : 1);
		_result += _stepSize;

		if (_stepSize == 0) break;
	}
	return I(_result);
}
Decimal Decimal::d_slog(Decimal a, Decimal b, int c, bool d) {
	try {
		if (lte(b, N())) {
			throw std::domain_error("slog base 0 or base < 0");
		} else if (eq(b, FC(1, 0, 1))) {
			throw std::domain_error("slog base 1");
		} else if (lt(b, FC(1, 0, 1))) {
			if (eq(a, FC(1, 0, 1))) {
				return N();
			} else if (eq(a, N())) {
				return FC(-1, 0, 1);
			} else {
				throw std::domain_error("slog with a noninteger base of with values 0 < x < 1 are pretty much all imaginary");
			}
		} else if (a.mag < 0 || eq(a, N())) {
			return FC(-1, 0, 1);
		} else if (lt(b, FC(1, 0, MAGIC_CONSTANT))) {
			if (eq(a, div(lambertw(neg(ln(b))), neg(ln(b))))) throw std::domain_error("lambert W function of -1/e");
			if (gt(a, div(lambertw(neg(ln(b))), neg(ln(b))))) throw std::domain_error("lambert W function of something less than -1/e");
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "\033[31m" << "Domain error: " << e.what() << "\033[0m" << std::endl;
	}

	unsigned long long _result = 0;
	Decimal _payload = a;
	if (_payload.layer - b.layer > 3) {
		_result += _payload.layer - b.layer - 3;
		_payload.layer -= _payload.layer - b.layer - 3;
	}
	for (int i = 0; i < c; ++i) {
		if (lt(_payload, N())) {
			_payload = pow(b, _payload);
			_result -= 1;
		} else if (lte(_payload, FC(1, 0, 1))) {
			if (d || gt(b, FC(1, 0, 10))) return I(_result + toDouble(_payload) - 1);
			else return I(_result + c_tet(toDouble(b), toDouble(_payload), CRITICAL_SLOG_VALUES));
		} else {
			_result += 1;
			_payload = log(_payload, b);
		}
	}
	return I(_result);
}

//this is horrible, I have no clue what it does, so it stays
//god no I hate std::variant it sucks
std::vector<std::variant<Decimal, int>> Decimal::e_slog(Decimal a, Decimal b, bool c) {
	try {
		if (eq(b, FC(1, 0, 1)) || lte(b, N())) throw std::domain_error("slog base 1 or base <= 0");
		if (gt(b, I(MAGIC_CONSTANT))) return {slog(a, b, 100, c), 0};
		Decimal _upper = INF;
		Decimal _negln = neg(ln(b));
		Decimal _lower = div(lambertw(_negln), _negln);
		if (gt(b, FC(1, 0, 1))) _upper = div(lambertw(_negln, false), _negln);
		if (gt(b, FC(1, 0, OTHER_MAGIC_CONSTANT))) _lower = _upper = FC(1, 0, std::exp(1.0));

		if (lt(a, _lower)) return {slog(a, b, 100, c), 0};
		else if (eq(a, _lower)) return {INF, 0};
		else if (eq(a, _upper)) return {negINF, 2};

		Decimal _slogZero, _slogOne, _payload;
		long long _estimate{};
		if (gt(a, _upper)) {
			_slogZero = mul(_upper, FC(1, 0, 2));
			_slogOne = pow(b, _slogZero);
			_estimate = 0;

			if (gte(a, _slogZero) && lt(a, _slogOne)) _estimate = 0;
			else if (gte(a, _slogOne)) {
				_payload = _slogOne;
				_estimate = 1;
				while (lt(_payload, a)) {
					_payload = pow(b, _payload);
					_estimate += 1;
					if (_payload.layer > 3) {
						long long _layersLeft = std::floor(a.layer - _payload.layer + 1);
						_payload = tet(b, _layersLeft, _payload, c);
						_estimate += _layersLeft;
					}
				}
				if (gt(_payload, a)) _estimate -= 1;
			} else if (lt(a, _slogZero)) {
				_payload = _slogZero;
				_estimate = 0;
				while (gt(_payload, a)) {
					_payload = log(_payload, b);
					_estimate -= 1;
				}
			}

			double _fracHeight = 0.0, _tested = 0.0, _stepSize = 0.5;
			Decimal _towerTop = _slogZero, _guess = N();
			while (_stepSize > 1e-16) {
				_tested = _fracHeight + _stepSize;
				//geometric average kinda
				_towerTop = mul(pow(_slogZero, I(1 - _tested)), pow(_slogOne, I(_tested)));
				_guess = tet(b, I(_estimate), _towerTop, false);
				if (eq(_guess, a)) return {I(_estimate + _tested), 2};
				else if (lt(_guess, a)) _fracHeight += _stepSize;
				_stepSize /= 2;
			}
			if (tolNeq(_guess, a)) throw std::domain_error("no value could be estimated");
			return {I(_estimate + _fracHeight), 2};
		} 
		//second branch
		else if (lt(a, _upper) && gt(a, _lower)) {
			//actual geometric mean
			_slogZero = sqrt(mul(_lower, _upper));
			_slogOne = pow(b, _slogZero);
			_estimate = 0;

			if (lte(a, _slogZero) && gt(a, _slogOne)) _estimate = 0;
			else if (lte(a, _slogOne)) {
				_payload = _slogOne;
				_estimate = 1;
				while (gt(_payload, a)) {
					_payload = pow(b, _payload);
					_estimate += 1;
				}
				if (lt(_payload, a)) {
					_payload = log(_payload, b);
					_estimate -= 1;
				}
			} else if (gt(a, _slogZero)) {
				_payload = _slogZero;
				_estimate = 0;
				while (lt(_payload, a)) {
					_payload = log(_payload, b);
					_estimate -= 1;
				}
			}
			double _fracHeight = 0.0, _tested = 0.0, _stepSize = 0.5;
			Decimal _towerTop = _slogZero, _guess = N();
			while (_stepSize > 1e-16) {
				_tested = _fracHeight + _stepSize;
				//geometric average kinda
				_towerTop = mul(pow(_slogZero, I(1 - _tested)), pow(_slogOne, I(_tested)));
				_guess = tet(b, I(_estimate), _towerTop, false);
				if (eq(_guess, a)) return {I(_estimate + _tested), 1};
				else if (gt(_guess, a)) _fracHeight += _stepSize;
				_stepSize /= 2;
			}
			if (tolNeq(_guess, a)) throw std::domain_error("no value could be estimated");
			return {I(_estimate + _fracHeight), 1};
		}
	}
	catch (const std::domain_error e) {
		std::cerr << "\033[31m" << "Domain error: " << e.what() << "\033[0m" << std::endl;
	}
	std::cerr << "\033[31m" << "You somehow managed to jump over all of the conditions and ran into this unhandled crap" << "\033[31m" << std::endl;
	return {N(), 0};
}

Decimal Decimal::itLog(Decimal a, Decimal b, Decimal c, bool d) {
	try {
		if (lte(b, N()) || eq(b, FC(1, 0, 1))) throw std::domain_error("itLog's base can only output imaginary values");
	}
	catch (const std::domain_error e) {
		std::cerr << "\033[31m" << "Domain error: " << e.what() << "\033[0m" << std::endl;
	}
	if (lt(c, N())) return tet(b, neg(c), a, d);
	Decimal _payload = a;
	long long _truncC = std::trunc(toLongLong(c));
	double _fracC = toDouble(c) - _truncC;

	if (_payload.layer - b.layer > 3) {
		long long _layerLoss = std::min(_truncC, static_cast<long long>(_payload.layer - b.layer - 3));
		_truncC -= _layerLoss;
		_payload.layer -= _layerLoss;
	}

	for (int i = 0; i < _truncC; ++i) {
		_payload = log(_payload, b);
		if (gte(_payload, INF)) return INF;
		//escape if the loop is gonna go forever
		if (i > 10000) return _payload;
	}
	
	//deals with the fractional component
	if (_fracC > 0 && _fracC < 1) {
		if (eq(b, FC(1, 0, 10))) _payload = ladd10(_payload, c);
		else _payload = ladd(_payload, b, I(_fracC), d);
	}
	return _payload;
}
Decimal Decimal::logN(Decimal a, Decimal b, Decimal c, bool d) {
	return itLog(a, b, c, d);
}

Decimal Decimal::repLog10(Decimal a, Decimal b) {
	if (eq(b, N())) return a;
	//I will add a tet10 function at some point, I am lazy right now
	else if (lt(b, N())) return tet(FC(1, 0, 10), neg(b), a, false);
	Decimal _payload = a;
	long long _truncB = toLongLong(b), _layerDiff = a.layer - _truncB;
	double _fracB = toDouble(b) - _truncB;
	if (_layerDiff < 0) return N();

	if (gt(_fracB, N()), lt(_fracB, FC(1, 0, 1))) {
		if (_layerDiff == 0) return FC(1, 0, a.layer + _fracB);

		double _newMag = std::pow(10, LAYER_LIMIT * _fracB) - 1;
		//checks if a.mag + _newMag are outside of the layer range
		std::vector<double, std::allocator<double>> _layerCheck = d_ladd(a.mag, _newMag);
		return FC(a.sign, _layerDiff + _layerCheck[0], _layerCheck[1]);
	} else {
		if (_layerDiff == 0) return FC(1, 0, a.layer);
		return FC(a.sign, _layerDiff, a.mag);
	}
}