float fitness(float *values)
{
  int i;
  float total=0.0;
  for(i=0;i<10;i++)
  {
    total+=(
      (values[i]*values[i])
      -(10*cos(2*M_PI*values[i]))
      +10);  //
  }
  return total;
}