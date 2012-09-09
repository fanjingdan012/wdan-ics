/* Testing Code */

#include <limits.h>
int test_abs(int x) {
  return (x < 0) ? -x : x;
}
int test_bitAnd(int x, int y)
{
  return x&y;
}
int test_bitMask(int highbit, int lowbit)
{
  int result = 0;
  int i;
  for (i = lowbit; i <= highbit; i++)
    result |= 1 << i;
  return result;
}
int test_bitXor(int x, int y)
{
  return x^y;
}
int test_conditional(int x, int y, int z)
{
  return x?y:z;
}
int test_evenBits(void) {
  int result = 0;
  int i;
  for (i = 0; i < 32; i+=2)
    result |= 1<<i;
  return result;
}
int test_isEqual(int x, int y)
{
  return x == y;
}
int test_isLess(int x, int y)
{
  return x < y;
}
int test_isNegative(int x) {
  return x < 0;
}
int test_isNonZero(int x)
{
  return x!=0;
}
int test_leastBitPos(int x) {
  int mask = 1;

  if (x == 0)
    return 0;
  while (!(mask & x)) {
    mask = mask << 1;
  }
  return mask;
}
int test_logicalNeg(int x)
{
  return !x;
}
int test_reverseBytes(int x)
{
  union U {
    int result;
    char byte[4];
  };
  union U u;
  int temp;
  u.result = x;
  temp = u.byte[0];
  u.byte[0] = u.byte[3];
  u.byte[3] = temp;
  temp = u.byte[1];
  u.byte[1] = u.byte[2];
  u.byte[2] = temp;
  return u.result;
}
int test_sm2tc(int x) {
  int sign = x < 0;
  int mag = x & LONG_MAX;
  return sign ? -mag : mag;
}
int test_sum3(int x, int y, int z)
{
  return x+y+z;
}
