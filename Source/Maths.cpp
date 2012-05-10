/**> HEADER FILES <**/
#include "Maths.h"

double fast_sqrt (register double arg)
{	
#ifdef OS9 
	// Can replace with slower return std::sqrt(arg);
	register double result;

	if (arg == 0.0) return 0.0;
	
	asm {
		frsqrte		result,arg			// Calculate Square root
	}	
	
	// Newton Rhapson iterations.
	result = result + 0.5 * result * (1.0 - arg * result * result);
	result = result + 0.5 * result * (1.0 - arg * result * result);
	
	return result * arg;
#else
	return sqrt(arg);
#endif
}
