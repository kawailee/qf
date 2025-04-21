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
}