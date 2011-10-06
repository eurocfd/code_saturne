#ifndef __BFT_TIMER_H__
#define __BFT_TIMER_H__

/*============================================================================
 * Program timing information
 *============================================================================*/

/*
  This file is part of Code_Saturne, a general-purpose CFD tool.

  Copyright (C) 1998-2011 EDF S.A.

  This program is free software; you can redistribute it and/or modify it under
  the terms of the GNU General Public License as published by the Free Software
  Foundation; either version 2 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
  details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
  Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

/*----------------------------------------------------------------------------*/

/* Standard C library headers */

/* BFT library headers */

/*-----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#if 0
} /* Fake brace to force Emacs auto-indentation back to column 0 */
#endif
#endif /* __cplusplus */

/*============================================================================
 * Public types
 *============================================================================*/

/*============================================================================
 * Public function prototypes
 *============================================================================*/

/*
 * Return Wall clock time
 *
 * returns:
 *   elapsed time from first call of a function of the bft_timer_...()
 *   series, or -1 if unable to compute.
 */

double
bft_timer_wtime(void);

/*
 * Return CPU time.
 *
 * Note that in the rare case that only the minimal C library clock()
 * method is available (see bft_timer_cpu_time_method()), at least one of
 * the bft_timer_...() functions (possibly this one) must be called
 * upon program start for this function to be used. In addition,
 * in this case, time may "loop" back to 0 every multiple of
 * 2^size_t / CLOCKS_PER_SEC seconds.
 *
 * returns:
 *   current CPU time usage, or -1 if unable to compute.
 */

double
bft_timer_cpu_time(void);

/*
 * Return separate user and system CPU times.
 *
 * parameters:
 *   user_time   --> current user CPU usage.
 *   system_time --> current system CPU usage.
 */

void
bft_timer_cpu_times(double *user_time,
                    double *system_time);

/*
 * Return method used to return wall clock time.
 *
 * Note that in the rare case that only the minimal C library clock()
 * method is available, this function will return -1 values.
 *
 * returns:
 *   short description of method used to return wall clock time.
 */

const char *
bft_timer_wtime_method(void);

/*
 * Return method used to return CPU time.
 *
 * returns:
 *   short description of method used to return CPU time.
 */

const char *
bft_timer_cpu_time_method(void);

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BFT_TIMER_H__ */
