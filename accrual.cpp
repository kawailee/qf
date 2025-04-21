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
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/time/daycounters/all.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace QuantLib;

void splitString(std::string& input, char delimiter, std::string arr[], int& index)
{
	// Creating an input string stream from the input string
	std::istringstream stream(input);

	// Temporary string to store each token
	std::string token;

	// Read tokens from the string stream separated by the
	// delimiter
	while (getline(stream, token, delimiter)) {
		// Add the token to the array
		arr[index++] = token;
	}
}

//	AccruedInterestFileCouponRecord to 
class AccruedInterestFileCouponRecord
{
	private:
	int index;
	std::string array_of_string[100];
	std::string date_string[100];
	Date startDate;
	Date endDate;
	Date paymentDate;
	Date valueDate;
	Real nominal;

	// Define the fixed rate parameters
	Rate fixedRate;
	DayCounter dayCounter;

	Date convFromYMD(std::string y, std::string m, std::string d) {
		int y_num = std::stoi(y);
		int m_num = std::stoi(m);
		int d_num = std::stoi(d);
		switch (m_num) {
			case 1:
				return(Date(d_num, January, y_num));
			case 2:
				return(Date(d_num, February, y_num));
			case 3:
				return(Date(d_num, March, y_num));
			case 4:
				return(Date(d_num, April, y_num));
			case 5:
				return(Date(d_num, May, y_num));
			case 6:
				return(Date(d_num, June, y_num));
			case 7:
				return(Date(d_num, July, y_num));
			case 8:
				return(Date(d_num, August, y_num));
			case 9:
				return(Date(d_num, September, y_num));
			case 10:
				return(Date(d_num, October, y_num));
			case 11:
				return(Date(d_num, November, y_num));
			case 12:
				return(Date(d_num, December, y_num));
			default:
				break;
		}
		// Should Fail here.
		throw std::runtime_error("Invalid Date");
		return(Date(1, February, 1999));
	}

	public:
	AccruedInterestFileCouponRecord(std::string line) {
		int date_index;
		index = 0;
		nominal = 1;

		splitString(line, '|', array_of_string, index);

		//	coupon start date
		date_index = 0;
		splitString(array_of_string[1], '/', date_string, date_index);
		startDate = convFromYMD(date_string[2],date_string[0],date_string[1]);
	
		//	value date
		date_index = 0;
		splitString(array_of_string[2], '/', date_string, date_index);
		valueDate = convFromYMD(date_string[2],date_string[0],date_string[1]);

		//	coupon end date
		date_index = 0;
		splitString(array_of_string[3], '/', date_string, date_index);
		endDate = convFromYMD(date_string[2],date_string[0],date_string[1]);
		paymentDate = endDate;

		//	interest rate daycount code
		//array_of_string[5]

		int dc_num = std::stoi(array_of_string[5]);

		switch(dc_num) {
			case 1:
				//	1 - 30/360
				dayCounter = Thirty360(Thirty360::BondBasis);
				break;
			case 2:
				//	2 - ACT/360
				dayCounter = Actual360();
				break;
			case 3:
				//	3 - ACT/ACT
				dayCounter = ActualActual(ActualActual::Convention::AFB);
				break;
			case 4:
				//	4 - ACT/365 FIXED
				dayCounter = Actual365Fixed();
				break;
			case 5:
				//	5 - 30E/360 ISDA
				dayCounter = Thirty360(Thirty360::ISDA);
				break;
			case 6:
				//	6 - 1/1
			case 7:
				//	7 - ACT/1
			case 8:
				//	8 - 30/1
				throw std::runtime_error("Unsupported Daycount");
				break;
			case 9:
				//	9 - ACT/ACT PEDT
				dayCounter = ActualActual(ActualActual::Convention::AFB);
				break;
			case 10:
				//	10 - ACT/ACT QUUAL
				dayCounter = ActualActual(ActualActual::Convention::AFB);
				break;
			case 11:
				//	11 - 30B/360
				dayCounter = Thirty360(Thirty360::BondBasis);
				break;
			case 12:
				//	12 - 30E/360
				dayCounter = Thirty360(Thirty360::EurobondBasis);
				break;
			case 13:
				//	13 - FULL
			case 14:
				//	14 - 360/360
			case 15:
				//	15 - BUS/252
				throw std::runtime_error("Unsupported Daycount");
				break;
			case 16:
				//	16 - ACT/ACT (ICMA)
				dayCounter = ActualActual(ActualActual::Convention::ISMA);
				break;
			
		default:
			// Should Fail here.
			throw std::runtime_error("Invalid Daycount");
		}

		//	interest rate percentage
		fixedRate = std::stod(array_of_string[6]);

	}

	double calcInterestAccrued() {
		auto coupon = FixedRateCoupon(paymentDate, nominal, fixedRate, dayCounter, startDate, endDate);
		return(coupon.accruedAmount(valueDate));
	}
};

int main(int argc, char**argv)
{

	if (argc<2) {
		std::cout << "Missing accrued interest file name" << std::endl;
		return 2;
	}

	// value date
	auto valueDate = Date(10, March, 2025);
	Settings::instance().evaluationDate() = valueDate;

	std::string line;
	std::string accraul_rate_filename = argv[1];
	std::ifstream inputFile;

	inputFile.open(accraul_rate_filename);
	if (!inputFile.is_open()) {
		std::cerr << "Error: Unable to open file " << accraul_rate_filename << std::endl;
		return 1;
	}

	while(std::getline(inputFile, line)) {
		AccruedInterestFileCouponRecord aifcr(line);
		std::cout << line << "|" << std::setprecision(14) << aifcr.calcInterestAccrued() << std::endl;
	}
	//	rm -rf ./* && cmake .. -DCMAKE_CXX_COMPILER=/opt/rocm/llvm/bin/clang++ && make && ./accrual ../accrual_rate.txt

	inputFile.close();

	return 0;
}