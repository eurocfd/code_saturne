#ifndef __CS_GUI_BOUNDARY_CONDITION_H__
#define __CS_GUI_BOUNDARY_CONDITION_H__

/*============================================================================
 * Management of the GUI parameters file: boundary conditions
 *============================================================================*/

/*
  This file is part of Code_Saturne, a general-purpose CFD tool.

  Copyright (C) 1998-2012 EDF S.A.

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

/*----------------------------------------------------------------------------
 * MEI library headers
 *----------------------------------------------------------------------------*/

#include "mei_evaluate.h"

/*----------------------------------------------------------------------------
 * Local headers
 *----------------------------------------------------------------------------*/

#include "cs_base.h"

/*----------------------------------------------------------------------------*/

BEGIN_C_DECLS

/*============================================================================
 * Type definitions
 *============================================================================*/

/*----------------------------------------------------------------------------
 * Structures associated to boundary conditions definition
 *----------------------------------------------------------------------------*/

typedef struct {
  double val1;             /* fortran array RCODCL(.,.,1) mapping             */
  double val2;             /* fortran array RCODCL(.,.,2) mapping             */
  double val3;             /* fortran array RCODCL(.,.,3) mapping             */
} cs_val_t;

typedef struct {
  int        read_data;    /* 1 if profile is calculated from data            */
  int        automatic;    /* 1 if nature of the boundary is automatic        */
} cs_meteo_t;

typedef struct {
  char      **label;       /* label for each boundary zone                    */
  char      **nature;      /* nature for each boundary zone                   */
  int        *iqimp;       /* 1 if a flow rate is applied                     */
  int        *ientat;      /* 1 if inlet for oxydant (coal combustion)        */
  int        *ientcp;      /* 1 if inlet for oxydant+coal (coal combustion)   */
  int        *icalke;      /* automatic boundaries for turbulent variables    */
  double     *qimp;        /* oxydant flow rate (coal combustion)             */
  double     *inmoxy;      /* oxydant number (coal combustion)                */
  double     *timpat;      /* inlet temperature of oxydant (coal combustion)  */
  double    **qimpcp;      /* inlet coal flow rate (coal combustion)          */
  double    **timpcp;      /* inlet coal temperature (coal combustion)        */
  int        *itype;       /* type of inlet/outlet (compressible model)       */
  double     *prein;       /* inlet pressure (compressible model)             */
  double     *rhoin;       /* inlet density  (compressible model)             */
  double     *tempin;      /* inlet temperature (compressible model)          */
  double     *entin;       /* inlet total energy (compressible model)         */
  double     *denin;       /* inlet density for subsonic (compressible model) */
  double     *preout;      /* outlet pressure for subsonic(compressible model)*/
  double     *dh;          /* inlet hydraulic diameter                        */
  double     *xintur;      /* inlet turbulent intensity                       */
  int       **type_code;   /* type of boundary for each variables             */
  cs_val_t  **values;      /* fortran array RCODCL mapping                    */
  double   ***distch;      /* ratio for each coal                             */
  double     *rough;       /* roughness size                                  */
  double     *norm;        /* norm of velocity vector                         */
  double     *dirx;        /* directions x inlet velocity                     */
  double     *diry;        /* directions y inlet velocity                     */
  double     *dirz;        /* directions z inlet velocity                     */
  mei_tree_t **velocity;   /* formula for norm or mass flow rate of velocity  */
  mei_tree_t **direction;  /* formula for direction of velocity               */
  cs_meteo_t  *meteo;      /* inlet or outlet info for atmospheric flow       */
  mei_tree_t ***scalar;    /* formula for scalar (neumann, dirichlet or
                              exchange coefficient)*/
} cs_boundary_t;


/*----------------------------------------------------------------------------
 * Enum for boundary conditions
 *----------------------------------------------------------------------------*/

typedef enum {
  DIRICHLET,
  FLOW1,
  HYDRAULIC_DIAMETER,
  TURBULENT_INTENSITY,
  NEUMANN,
  COEF_ECHANGE,
  COALFLOW,
  WALL_FUNCTION,
  DIRICHLET_FORMULA,
  NEUMANN_FORMULA,
  COEF_ECHANGE_FORMULA
} cs_boundary_value_t;

/*============================================================================
 * Static global variables
 *============================================================================*/

/* Pointer on the main boundaries structure */

extern cs_boundary_t *boundaries;

/*============================================================================
 * Public function prototypes for Fortran API
 *============================================================================*/

/*----------------------------------------------------------------------------
 * Boundary conditions treatment
 *
 * Fortran Interface:
 *
 * SUBROUTINE UICLIM
 * *****************
 *
 * INTEGER          NTCABS  --> current iteration number
 * INTEGER          NFABOR  --> number of boundary faces
 * INTEGER          NOZPPM  --> max number of boundary conditions zone
 * INTEGER          NCHARM  --> maximal number of coals
 * INTEGER          NCHARB  --> number of simulated coals
 * INTEGER          NCLPCH  --> number of simulated class per coals
 * INTEGER          IINDEF  --> type of boundary: not defined
 * INTEGER          IENTRE  --> type of boundary: inlet
 * INTEGER          IESICF  --> type of boundary: imposed inlet (compressible)
 * INTEGER          ISSPCF  --> type of boundary: supersonic outlet (compressible)
 * INTEGER          IERUCF  --> type of boundary: subsonic inlet (compressible)
 * INTEGER          ISOPCF  --> type of boundary: subsonic outlet (compressible)
 * INTEGER          IPAROI  --> type of boundary: smooth wall
 * INTEGER          IPARUG  --> type of boundary: rough wall
 * INTEGER          ISYMET  --> type of boundary: symetry
 * INTEGER          ISOLIB  --> type of boundary: outlet
 * INTEGER          ISCA    <-- indirection array for scalar number
 * INTEGER          IPR     <-- rtp index for pressure
 * INTEGER          IRHO    <-- rtp index for density
 * INTEGER          ITEMPK  <-- rtp index for temperature (in K)
 * INTEGER          IENERG  <-- rtp index for energy total
 * INTEGER          IQIMP   --> 1 if flow rate is applied
 * INTEGER          ICALKE  --> 1 for automatic turbulent boundary conditions
 * INTEGER          IENTAT  --> 1 for air temperature boundary conditions (coal)
 * INTEGER          IENTCP  --> 1 for coal temperature boundary conditions (coal)
 * INTEGER          inmoxy  --> oxydant number (coal)
 * integer          iprofm  --> atmospheric flows: on/off for profile from data
 * INTEGER          ITYPFB  --> type of boundary for each face
 * INTEGER          IZFPPP  --> zone number for each boundary face
 * INTEGER          ICODCL  --> boundary conditions array type
 * DOUBLE PRECISION DTREF   --> time step
 * DOUBLE PRECISION TTCABS  --> current time
 * DOUBLE PRECISION SURFBO  --> boundary faces surface
 * DOUBLE PRECISION CGDFBO  --> boundary faces center of gravity
 * DOUBLE PRECISION QIMP    --> inlet flow rate
 * DOUBLE PRECISION QIMPAT  --> inlet air flow rate (coal)
 * DOUBLE PRECISION QIMPCP  --> inlet coal flow rate (coal)
 * DOUBLE PRECISION DH      --> hydraulic diameter
 * DOUBLE PRECISION XINTUR  --> turbulent intensity
 * DOUBLE PRECISION TIMPAT  --> air temperature boundary conditions (coal)
 * DOUBLE PRECISION TIMPCP  --> inlet coal temperature (coal)
 * DOUBLE PRECISION DISTCH  --> ratio for each coal
 * DOUBLE PRECISION RCODCL  --> boundary conditions array value
 *----------------------------------------------------------------------------*/

void CS_PROCF (uiclim, UICLIM)(const    int *const ntcabs,
                               const    int *const nfabor,
                               const    int *const nozppm,
                               const    int *const ncharm,
                               const    int *const ncharb,
                               const    int *const nclpch,
                               const    int *const iindef,
                               const    int *const ientre,
                               const    int *const iesicf,
                               const    int *const isspcf,
                               const    int *const ierucf,
                               const    int *const isopcf,
                               const    int *const iparoi,
                               const    int *const iparug,
                               const    int *const isymet,
                               const    int *const isolib,
                               const    int       *isca,
                               const    int *const ipr,
                               const    int *const irho,
                               const    int *const itempk,
                               const    int *const ienerg,
                                        int *const iqimp,
                                        int *const icalke,
                                        int *const ientat,
                                        int *const ientcp,
                                        int *const inmoxy,
                                        int *const iprofm,
                                        int *const itypfb,
                                        int *const izfppp,
                                        int *const icodcl,
                                     double *const dtref,
                                     double *const ttcabs,
                                     double *const surfbo,
                                     double *const cdgfbo,
                                     double *const qimp,
                                     double *const qimpat,
                                     double *const qimpcp,
                                     double *const dh,
                                     double *const xintur,
                                     double *const timpat,
                                     double *const timpcp,
                                     double *const distch,
                                     double *const rcodcl);

/*----------------------------------------------------------------------------
 * Boundary conditions input verification
 *
 * Fortran Interface:
 *
 * SUBROUTINE UICLVE
 * *****************
 *
 * INTEGER          NFABOR  --> number of boundary faces
 * INTEGER          NOZPPM  --> max number of boundary conditions zone
 * INTEGER          IINDEF  --> type of boundary: not defined
 * INTEGER          IENTRE  --> type of boundary: inlet
 * INTEGER          IESICF  --> type of boundary: imposed inlet (compressible)
 * INTEGER          ISSPCF  --> type of boundary: supersonic outlet (compressible)
 * INTEGER          IERUCF  --> type of boundary: subsonic inlet (compressible)
 * INTEGER          ISOPCF  --> type of boundary: subsonic outlet (compressible)
 * INTEGER          IPAROI  --> type of boundary: wall
 * INTEGER          IPARUG  --> type of boundary: wall with rugosity
 * INTEGER          ISYMET  --> type of boundary: symmetry
 * INTEGER          ISOLIB  --> type of boundary: outlet
 * INTEGER          ITYPFB  --> type of boundary for each face
 * INTEGER          IZFPPP  --> zone number
 *----------------------------------------------------------------------------*/

void CS_PROCF (uiclve, UICLVE) (const int *const nfabor,
                                const int *const nozppm,
                                const int *const iindef,
                                const int *const ientre,
                                const int *const iesicf,
                                const int *const ierucf,
                                const int *const isspcf,
                                const int *const isopcf,
                                const int *const iparoi,
                                const int *const iparug,
                                const int *const isymet,
                                const int *const isolib,
                                      int *const itypfb,
                                      int *const izfppp);

/*=============================================================================
 * Public function prototypes
 *============================================================================*/

/*-----------------------------------------------------------------------------
 * Return number of boundary regions definition
 *----------------------------------------------------------------------------*/

int
cs_gui_boundary_zones_number(void);

/*-----------------------------------------------------------------------------
 * Return the nature of boundary condition for the given zone
 *----------------------------------------------------------------------------*/

char *
cs_gui_boundary_zone_nature(int  ith_zone);

/*-----------------------------------------------------------------------------
 * Return the label of boundary condition for the given zone
 *----------------------------------------------------------------------------*/

char *
cs_gui_boundary_zone_label(int  ith_zone);

/*-----------------------------------------------------------------------------
 * Return the zone number of boundary condition for the given zone
 *----------------------------------------------------------------------------*/

int
cs_gui_boundary_zone_number(int  ith_zone);

/*-----------------------------------------------------------------------------
 * Return the description of a boundary zone
 *
 * parameters:
 *   label <--  label of boundary zone
 *----------------------------------------------------------------------------*/

char *
cs_gui_boundary_zone_localization(const char *label);

/*-----------------------------------------------------------------------------
 * Helper to get the face list for the izone
 *
 * parameters:
 *   izone   <--  zone index
 *   label   <--  boundary label
 *   nfabor  <--  number of boundary faces
 *   nozppm  <--  max number of boundary zone for preefined physics
 *   faces   -->  number of faces
 *----------------------------------------------------------------------------*/

int*
cs_gui_get_faces_list(int          izone,
                      const char  *label,
                      int          nfabor,
                      int          nozppm,
                      int         *faces );

/*----------------------------------------------------------------------------
 * Free boundary conditions structures
 *
 * parameters:
 *   ncharb  <-- number of coals
 *----------------------------------------------------------------------------*/

void
cs_gui_boundary_conditions_free_memory(const int  *ncharb);

/*----------------------------------------------------------------------------*/

END_C_DECLS

#endif /* __CS_GUI_BOUNDARY_CONDITION_H__ */
