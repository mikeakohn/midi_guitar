/**
 *  midi_guitar
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2015 by Michael Kohn
 *
 */


#ifndef _FLOAT_H
#define _FLOAT_H

#define INT_PRECISION 14
//#define USE_DOUBLE

#ifdef USE_DOUBLE
#define FLOAT double
#define COS cos
#define SQRT sqrt
#else
#define COS_LOOKUP
#define FLOAT float
#define COS cosf
#define SQRT sqrtf
#endif

#endif

