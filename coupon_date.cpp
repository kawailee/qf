#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/math/solvers1d/brent.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/quotes/simplequote.hpp>
#include <ql/settings.hpp>
#include <ql/termstructures/yield/zerocurve.hpp>
#include <ql/termstructures/yield/zerospreadedtermstructure.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/thirty360.hpp>
#include <ql/time/schedule.hpp>
#include <iostream>

int main() {

	using namespace QuantLib;

	auto valueDate = Date(8, December, 2022);
	Settings::instance().evaluationDate() = valueDate;

	// Dates and calendars and conventions, oh my

	Schedule schedule =
		MakeSchedule()
			.from(Date(26, August, 2020))
			.to(Date(26, May, 2031))
			.withFirstDate(Date(26, May, 2021))
			.withFrequency(Semiannual)
			.withCalendar(TARGET())
			.withConvention(Following)
			.backwards();

	for (auto d : schedule) {
		std::cout << d << "\n";
	}
	std::cout << std::endl;

	// Some simple bond calculations

	unsigned settlementDays = 3;
	double faceAmount = 1000000.0;
	std::vector<Rate> coupons = {0.02};
	auto dayCounter = Thirty360(Thirty360::BondBasis);

	auto bond =
	FixedRateBond(settlementDays, faceAmount, schedule,
					coupons, dayCounter);

	Rate yield = 0.025;
	double price = bond.cleanPrice(yield, dayCounter,
									Compounded, Annual);
	double accrual = bond.accruedAmount();

	std::cout << price << "\n"
				<< accrual << std::endl;

	// Discount curves

	std::vector<Date> curveNodes = {valueDate,
									valueDate + 1 * Years,
									valueDate + 2 * Years,
									valueDate + 5 * Years,
									valueDate + 10 * Years,
									valueDate + 15 * Years};
	std::vector<Rate> curveRates = {0.015, 0.015, 0.018,
									0.022, 0.025, 0.028};
	auto issuerCurve =
		ext::make_shared<InterpolatedZeroCurve<Linear>>(
			curveNodes, curveRates, Actual360());

	bond.setPricingEngine(
		ext::make_shared<DiscountingBondEngine>(
			Handle<YieldTermStructure>(issuerCurve)));
	std::cout << bond.cleanPrice() << std::endl;

	// Pricing at a spread over a curve

	std::vector<Rate> curveRates2 = {-0.005, -0.005, 0.001,
										0.004,  0.009,  0.012};
	auto goviesCurve =
		ext::make_shared<InterpolatedZeroCurve<Linear>>(
			curveNodes, curveRates2, Actual360());

	auto spread = ext::make_shared<SimpleQuote>(0.0);
	auto discountCurve =
		ext::make_shared<ZeroSpreadedTermStructure>(
			Handle<YieldTermStructure>(goviesCurve),
			Handle<Quote>(spread));

	bond.setPricingEngine(
		ext::make_shared<DiscountingBondEngine>(
			Handle<YieldTermStructure>(discountCurve)));
	std::cout << bond.cleanPrice() << std::endl;

	spread->setValue(0.01);
	std::cout << bond.cleanPrice() << std::endl;

	// Finding the z-spread given a price

	auto discrepancy = [&spread, &bond, price](Spread s) {
		spread->setValue(s);
		return bond.cleanPrice() - price;
	};

	auto solver = Brent();
	double accuracy = 1e-5, guess = 0.015, step = 1e-4;
	Spread spreadOverGovies =
		solver.solve(discrepancy, accuracy, guess, step);

	std::cout << spreadOverGovies << std::endl;
}