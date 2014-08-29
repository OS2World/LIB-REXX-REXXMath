/**********************************************************************
*   REXXMATH.c - (c) Zhitao Zeng Footprint Software 1994              *
*   This program is released to the public domain, feel free to use   *
*   it for any purposes.                                              *
*   If you have any suggestions, please email to Zhitao Zeng at:      *
*      CIS: 72000,2440                                                *
*      Internet: zzeng@footprint.com                                  *
**********************************************************************/

#define  INCL_ERRORS
#define  INCL_REXXSAA
#define  _DLL
#define  _MT
#include <os2.h>
#include <rexxsaa.h>
#include <float.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define  ENABLE_CACHE

RexxFunctionHandler MathLoadFuncs;
RexxFunctionHandler MathDropFuncs;
RexxFunctionHandler MathFunc;

typedef enum _MathFuncID {
	fn_acos,
	fn_asin, 
	fn_atan, 
	fn_atan2,
	fn_ceil, 
	fn_cos,  
	fn_cosh, 
	fn_exp,  
	fn_fabs, 
	fn_floor,
	fn_fmod, 
	fn_frexp,
	fn_ldexp,
	fn_log,  
	fn_log10,
	fn_modf, 
	fn_pow,  
	fn_sin,  
	fn_sinh, 
	fn_sqrt, 
	fn_tan, 
	fn_tanh,
	fn_erf,
	fn_erfc,  
	fn_gamma,
	fn_hypot, 
	fn_j0,    
	fn_j1,    
	fn_jn,    
	fn_y0,    
	fn_y1,    
	fn_yn,
	fn_pi
} MathFuncID;

typedef struct _RxMathFunc
{
	PSZ        name;
	MathFuncID id;
	ULONG      numargs;
} RxMathFunc, *PRxMathFunc;

static RxMathFunc RxMathFncTable[] =
{
	"acos",	  fn_acos,    1,
	"asin", 	  fn_asin, 	  1,
	"atan", 	  fn_atan, 	  1,
	"atan2",	  fn_atan2,	  2,
	"ceil", 	  fn_ceil, 	  1,
	"cos",  	  fn_cos,  	  1,
	"cosh", 	  fn_cosh, 	  1,
	"exp",  	  fn_exp,  	  1,
	"fabs", 	  fn_fabs, 	  1,
	"floor",	  fn_floor,	  1,
	"fmod", 	  fn_fmod, 	  2,
	"frexp",	  fn_frexp,	  1,
	"ldexp",	  fn_ldexp,	  2,
	"log",  	  fn_log,  	  1,
	"log10",	  fn_log10,	  1,
	"modf", 	  fn_modf, 	  1,
	"pow",  	  fn_pow,  	  2,
	"sin",  	  fn_sin,  	  1,
	"sinh", 	  fn_sinh, 	  1,
	"sqrt", 	  fn_sqrt, 	  1,
	"tan", 	  fn_tan, 	  1,
	"tanh",	  fn_tanh,	  1,
   "erf",	  fn_erf,	  1,
   "erfc",    fn_erfc,    1,
   "gamma",   fn_gamma,   1,
   "hypot",   fn_hypot,   2,
   "j0",      fn_j0,      1,
   "j1",      fn_j1,      1,
   "jn",      fn_jn,      2,
   "y0",      fn_y0,      1,
   "y1",      fn_y1,      1,
   "yn",      fn_yn,      2,
   "pi",      fn_pi,      0,
};

#ifdef ENABLE_CACHE
static RxMathFunc RxMathFncCache =
{
	"",	  -1,    0,
};
#endif								  

#define  INVALID_ROUTINE 40            /* Error                      */
#define  VALID_ROUTINE    0            /* Successful completion      */

/*************************************************************************
* Function:  MathLoadFuncs                                               *
* Syntax:    call MathLoadFuncs [option]                                 *
* Return:    null string                                                 *
*************************************************************************/

ULONG MathLoadFuncs(CHAR *name, ULONG numargs, RXSTRING args[],
                           CHAR *queuename, RXSTRING *retstr)
{
  INT    entries;                      /* Num of entries             */
  INT    j;                            /* Counter                    */

  retstr->strlength = 0;               /* set return value           */

  entries = sizeof(RxMathFncTable)/sizeof(RxMathFunc);

  RexxRegisterFunctionDll("MathLoadFuncs", "REXXMATH", "MathLoadFuncs");
  RexxRegisterFunctionDll("MathDropFuncs", "REXXMATH", "MathDropFuncs");

  for (j = 0; j < entries; j++) {
    RexxRegisterFunctionDll(RxMathFncTable[j].name,
          "REXXMATH", "MathFunc");
  }
  return VALID_ROUTINE;
}


/*************************************************************************
* Function:  MathDropFuncs                                               *
* Syntax:    call MathDropFuncs                                          *
* Return:    NO_UTIL_ERROR - Successful.                                 *
*************************************************************************/

ULONG MathDropFuncs(CHAR *name, ULONG numargs, RXSTRING args[],
                          CHAR *queuename, RXSTRING *retstr)
{
  INT     entries;                     /* Num of entries             */
  INT     j;                           /* Counter                    */

  retstr->strlength = 0;               /* return a null string result*/

  RexxDeregisterFunction("MathLoadFuncs");
  RexxDeregisterFunction("MathDropFuncs");

  entries = sizeof(RxMathFncTable)/sizeof(RxMathFunc);

  for (j = 0; j < entries; j++)
    RexxDeregisterFunction(RxMathFncTable[j].name);

  return VALID_ROUTINE;                /* no error on call           */
}

int resolve_func(CHAR *name, ULONG numargs)
{
	int entries = sizeof(RxMathFncTable)/sizeof(RxMathFunc);
	register int i;
	int fn_id = -1;

	for (i=0; i < entries; i++)
		if (stricmp(name, RxMathFncTable[i].name) == 0)
		{
   		if (numargs == RxMathFncTable[i].numargs)
         {
				fn_id = RxMathFncTable[i].id;
#ifdef ENABLE_CACHE
				RxMathFncCache.id = fn_id;
				RxMathFncCache.name = RxMathFncTable[i].name;
				RxMathFncCache.numargs = RxMathFncTable[i].numargs;
#endif
         }
			break;
		}

	return fn_id;
}

#ifdef ENABLE_CACHE
int cache_func(CHAR *name, ULONG numargs)
{
	int fn_id = -1;

	if (stricmp(name, RxMathFncCache.name) == 0)
		if (numargs == RxMathFncCache.numargs)
			fn_id = RxMathFncCache.id;

	return fn_id;
}
#endif

/*************************************************************************
* Function:  MathFunc                                                    *
*************************************************************************/

ULONG MathFunc(CHAR *name, ULONG numargs, RXSTRING args[],
                        CHAR *queuename, RXSTRING *retstr)
{
	double x, y, result;
	int i, fn_id = -1;

#ifdef ENABLE_CACHE
   if (RxMathFncCache.id >= 0)
      fn_id = cache_func(name, numargs);

   if (fn_id < 0)
   {
   	fn_id = resolve_func(name, numargs);
	   if (fn_id < 0)           
		   return INVALID_ROUTINE;
   }
#else
  	fn_id = resolve_func(name, numargs);
   if (fn_id < 0)           
	   return INVALID_ROUTINE;
#endif

	x = atof(args[0].strptr);
	if (numargs > 1L)
	{
		y = atof(args[1].strptr);
		i = atoi(args[1].strptr);
	}

	switch (fn_id)
	{
	case fn_acos:
		result = acos(x);
		break;
	case fn_asin: 
		result = asin(x);
		break;
	case fn_atan: 
		result = atan(x);
		break;
	case fn_atan2:
		result = atan2(x, y);
		break;
	case fn_ceil: 
		result = ceil(x);
		break;
	case fn_cos:  
		result = cos(x);
		break;
	case fn_cosh: 
		result = cosh(x);
		break;
	case fn_exp:  
		result = exp(x);
		break;
	case fn_fabs: 
		result = fabs(x);
		break;
	case fn_floor:
		result = floor(x);
		break;
	case fn_fmod: 
		result = fmod(x, y);
		break;
	case fn_frexp:
		result = frexp(x, &i);
		break;
	case fn_ldexp:
		result = ldexp(x, i);
		break;
	case fn_log:  
		result = log(x);
		break;
	case fn_log10:
		result = log10(x);
		break;
	case fn_modf: 
		result = modf(x, &y);
		break;
	case fn_pow:  
		result = pow(x, y);
		break;
	case fn_sin:  
		result = sin(x);
		break;
	case fn_sinh: 
		result = sinh(x);
		break;
	case fn_sqrt: 
		result = sqrt(x);
		break;
	case fn_tan: 
		result = tan(x);
		break;
	case fn_tanh:
		result = tanh(x);
		break;
#ifdef __IBMC__
	case fn_erf:
      result = erf( x );
		break;
	case fn_erfc:  
      result = erfc( x );
		break;
	case fn_gamma: 
      result = gamma( x );
		break;
#endif
	case fn_hypot: 
      result = hypot( x, y );
		break;
	case fn_j0:    
      result = j0( x );
		break;
	case fn_j1:    
      result = j1( x );
		break;
	case fn_jn:    
      result = jn( i, x );
		break;
	case fn_y0:    
      result = y0( x );
		break;
	case fn_y1:    
      result = y1( x );
		break;
	case fn_yn:    
      result = yn( i, x );
		break;
	case fn_pi:    
      result = 3.1415926575;
		break;
   default:
	   return INVALID_ROUTINE;
	}

	switch (fn_id)
	{
	case fn_frexp:
		sprintf(retstr->strptr, "%lf %i", result, i);
		break;
	case fn_modf: 
		sprintf(retstr->strptr, "%lf %lf", result, y);
		break;
	default:
		sprintf(retstr->strptr, "%lf", result);
		break;
	}

	retstr->strlength = strlen(retstr->strptr);
	return VALID_ROUTINE;
}
