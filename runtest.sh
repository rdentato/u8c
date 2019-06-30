${CC:-gcc} -Wall -std=c99 -O2 -c u8c.c
cd test
chmod +x ./runtest.sh
./runtest.sh
