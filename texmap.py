from sympy import *



P0x, P0y, P1x, P1y, P2x, P2y, Px, Py = symbols("P0x P0y P1x P1y P2x P2y Px Py")

P0 = Matrix([P0x, P0y])
P1 = Matrix([P1x, P1y])
P2 = Matrix([P2x, P2y])

P = Matrix([Px, Py])

print (P-P0)

