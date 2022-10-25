import math


def positive_fmod(x, y):
    if (x>0):
      while (x>=y):
        x -= y
    else:
      while (x<0):
        x += y

    return x


def find_constants(x1, y1, x2, y2, r):

  m = (x1-x2)/(y2-y1)
  cl = (((y2*y2)-(y1*y1) + (x2*x2)-(x1*x1))/(2*(y2-y1)))
  A = 1+(m*m)
  B = (2*m*cl)-(2*m*y1)-(2*x1)
  C = (x1*x1)+(y1*y1)+(cl*cl)-(2*cl*y1)-(r*r)

  print(f'm: {m}\ncl: {cl}\nA: {A}\nB: {B}\nC: {C}\n')
  print(f'root of the function: {(B*B) - (4*A*C)}')

  xc1 = (-B + math.sqrt((B*B) - (4*A*C)))/(2*A)
  xc2 = (-B - math.sqrt((B*B) - (4*A*C)))/(2*A)

  yc1 = m*xc1 + cl
  yc2 = m*xc2 + cl

  print(f'm: {m}\ncl: {cl}\nA: {A}\nB: {B}\nC: {C}\n(xc1, yc1): ({xc1, yc1})\n(xc2, yc2): ({xc2, yc2})')


find_constants(-3.923386, 49.055489, -5.110847, 49.055695, 0.838200)

# print(positive_fmod(0.789488, 2*math.pi))
# logfile line number 2406 (ss2.log)