/* REXXMath demo */
call RxFuncAdd 'MathLoadFuncs', 'REXXMATH', 'MathLoadFuncs'
call MathLoadFuncs

x = 10.00
do i = 1 to 10
   say "pow( " x "," i ") =" pow( x, i )
end

x = 2.00
do i = 1 to 10
   say "pow( " x "," i ") =" pow( x, i )
end

x = 1+.1/365
y = 365
say "pow( " x "," y ") =" pow( x, y )

x = 1+.1/12
y = 12
say "pow( " x "," y ") =" pow( x, y )

x = pi()
say "cos( " x  ") =" cos( x )
say "sin( " x  ") =" sin( x )

x = pi()/2
say "cos( " x  ") =" cos( x )
say "sin( " x  ") =" sin( x )

x = pi()/4
say "cos( " x  ") =" cos( x )
say "sin( " x  ") =" sin( x )

x = 1.00
say "j0( " x  ") =" j0( x )
say "j1( " x  ") =" j1( x )
say "y0( " x  ") =" y0( x )
say "y1( " x  ") =" y1( x )

x = 10.00
say "frexp( " x  ") =" frexp( x )
say "modf( " x  ") =" modf( x )
x = 10.50
say "frexp( " x  ") =" frexp( x )
say "modf( " x  ") =" modf( x )

call MathDropFuncs
