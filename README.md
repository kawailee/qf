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

## Run

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

96.3256
0.0833333
96.6641
110.34
101.913
0.0171229
```