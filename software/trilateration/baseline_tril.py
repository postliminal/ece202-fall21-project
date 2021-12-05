import numpy as np
# https://www.alanzucconi.com/2017/03/13/positioning-and-trilateration/
# distances d_x are measured by an RSSI signal
# https://arc.aiaa.org/doi/pdf/10.2514/3.20169 algorithm implemented
# reference https://stackoverflow.com/questions/56058383/i-want-to-implement-trilateration-in-python-i-cant-t-find-what-is-wrong-with-m

def localize(loc_one, loc_two, loc_three, d_one, d_two, d_three):
  p1 = np.array([0, 0, 0])
  p2 = np.array([loc_two[0] - loc_one[0], loc_two[1] - loc_one[1], loc_two[2] - loc_one[2]])
  p3 = np.array([loc_three[0] - loc_one[0], loc_three[1] - loc_one[1], loc_three[2] - loc_one[2]])
  b1 = p2 - p1 # b1 and b2 are baseline vectors
  b2 = p3 - p1
  xn = (b1)/np.linalg.norm(b1)
  var = np.cross(b1, b2)
  zn = (var)/np.linalg.norm(var)

  yn = np.cross(xn, zn)
  i = np.dot(xn, b2)
  d = np.dot(xn, b1)
  j = np.dot(yn, b2)

  X = ((d_one**2)-(d_two**2)+(d**2))/(2*d)
  Y = (((d_one**2)-(d_three**2)+(i**2)+(j**2))/(2*j))-((i/j)*(X))
  z1 = np.sqrt(max(0, d_one**2-X**2-Y**2))
  z2 = -z1

  pred_one = loc_one + X * xn + Y * yn + z1 * zn
  pred_two = loc_one + X * xn + Y * yn + z2 * zn
  return pred_one,pred_two
