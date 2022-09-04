#ifndef PROTO_RETROMODE_H
#define PROTO_RETROMODE_H

/*
**	$Id$
**
**	Prototype/inline/pragma header file combo
**
**	Copyright (c) 2010 Hyperion Entertainment CVBA.
**	All Rights Reserved.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef LIBRARIES_RETROMODE_H
#include <libraries/retromode.h>
#endif

/****************************************************************************/

#ifndef __NOLIBBASE__
 #if defined(__cplusplus) && defined(__USE_AMIGAOS_NAMESPACE__)
  extern struct AmigaOS::Library * RetroModeBase;
 #else
  extern struct Library * RetroModeBase;
 #endif
#endif /* __NOLIBBASE__ */

/****************************************************************************/

#ifdef __amigaos4__
 #include <interfaces/retromode.h>
 #ifdef __USE_INLINE__
  #include <inline4/retromode.h>
 #endif /* __USE_INLINE__ */
 #ifndef CLIB_RETROMODE_PROTOS_H
  #define CLIB_RETROMODE_PROTOS_H 1
 #endif /* CLIB_RETROMODE_PROTOS_H */
 #ifndef __NOGLOBALIFACE__
  #if defined(__cplusplus) && defined(__USE_AMIGAOS_NAMESPACE__)
   extern struct AmigaOS::RetroModeIFace * IRetroMode;
  #else
   extern struct RetroModeIFace * IRetroMode;
  #endif
 #endif /* __NOGLOBALIFACE__ */
#else /* __amigaos4__ */
 #ifndef CLIB_RETROMODE_PROTOS_H
  #include <clib/retromode_protos.h>
 #endif /* CLIB_RETROMODE_PROTOS_H */
 #if defined(__GNUC__)
  #ifndef __PPC__
   #include <inline/retromode.h>
  #else /* __PPC__ */
   #include <ppcinline/retromode.h>
  #endif /* __PPC__ */
 #elif defined(__VBCC__)
  #ifndef __PPC__
   #include <inline/retromode_protos.h>
  #endif /* __PPC__ */
 #else /* __GNUC__ */
  #include <pragmas/retromode_pragmas.h>
 #endif /* __GNUC__ */
#endif /* __amigaos4__ */

/****************************************************************************/

#endif /* PROTO_RETROMODE_H */
