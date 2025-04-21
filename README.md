# qf
Learning QuantLib is fun!

We can try the code from [QuantLib Guide](https://www.quantlibguide.com/A%20taste%20of%20QuantLib.html)


## To Build

``` bash
cd qf
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/your/directory/
make -j 4
```

## Display Coupon Schedule

This demonstrates to create Schedule using MakeSchedule().

``` bash
./coupon_date
August 26th, 2020
May 26th, 2021
November 26th, 2021
May 26th, 2022
November 28th, 2022
May 26th, 2023
November 27th, 2023
May 27th, 2024
November 26th, 2024
May 26th, 2025
November 26th, 2025
May 26th, 2026
November 26th, 2026
May 26th, 2027
November 26th, 2027
May 26th, 2028
November 27th, 2028
May 28th, 2029
November 26th, 2029
May 27th, 2030
November 26th, 2030
May 26th, 2031
```

## Calculate coupon interest accrued

This sample reads a file which has each line representing a coupon.

1. Coupon file name accrual_rate.txt is provided in the command line
2. Coupon file has 7 columns
   - Column 1: Identifier of this coupon (no use)
   - Column 2: Coupon start date in MM/DD/YYYY format
   - Column 3: Value Date in MM/DD/YYYY format
   - Column 4: Coupon end date in MM/DD/YYYY format
   - Column 5: Coupon leg number (no use)
   - Column 6: Daycount code (e.g. 1 means 30/360; 2 means ACT/360, ...)
   - Column 7: Fixed Rate
3. The output adds Column 8 which is coupon interest accrued rate.
   - If coupon is $100 and accrued rate is 0.47382475, interest accrued is calculated by multiplying $100 to the rate.

./accrual ../accrual_rate.txt
```
1|01/07/2025|03/10/2025|07/07/2025|2|1|2.70757|0.47382475
2|03/03/2025|03/10/2025|09/03/2025|2|2|2.982|0.057983333333333
3|08/12/2024|03/10/2025|08/11/2025|2|3|4.345|2.4998630136986
4|02/04/2025|03/10/2025|05/06/2025|2|4|9.607999|0.89499168767123
5|08/09/2024|03/10/2025|08/09/2025|2|5|9.5|5.5680555555556
6|06/12/2024|03/10/2025|06/12/2025|2|9|5.043|3.7442547945205
```

---
End