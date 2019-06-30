rm -f ut_*

cp dbg.h dbgstat.c
${CC:-gcc} -DDBGSTAT -std=c99 -O2 -Wall -o dbgstat dbgstat.c
rm dbgstat.c

for f in test*.c; do
  echo "compiling '$f'"
  ${CC:-gcc} -DDEBUG -std=c99 -O2 -Wall -I.. -c $f
  ${CC:-gcc} -o ut_${f%.c} ${f%.c}.o ../u8c.o 
done

rm -f runtest.log
for f in ut_*; do
  echo "running '$f' 2> runtest.log"
  ./$f 2>> runtest.log
done 

./dbgstat < runtest.log
