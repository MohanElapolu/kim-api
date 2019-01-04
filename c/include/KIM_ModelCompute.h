/*                                                                            */
/* CDDL HEADER START                                                          */
/*                                                                            */
/* The contents of this file are subject to the terms of the Common           */
/* Development and Distribution License Version 1.0 (the "License").          */
/*                                                                            */
/* You can obtain a copy of the license at                                    */
/* http://www.opensource.org/licenses/CDDL-1.0.  See the License for the      */
/* specific language governing permissions and limitations under the License. */
/*                                                                            */
/* When distributing Covered Code, include this CDDL HEADER in each file and  */
/* include the License file in a prominent location with the name             */
/* LICENSE.CDDL.                                                              */
/* If applicable, add the following below this CDDL HEADER, with the fields   */
/* enclosed by brackets "[]" replaced with your own identifying information:  */
/*                                                                            */
/* Portions Copyright (c) [yyyy] [name of copyright owner].                   */
/* All rights reserved.                                                       */
/*                                                                            */
/* CDDL HEADER END                                                            */
/*                                                                            */

/*                                                                            */
/* Copyright (c) 2016--2019, Regents of the University of Minnesota.          */
/* All rights reserved.                                                       */
/*                                                                            */
/* Contributors:                                                              */
/*    Ryan S. Elliott                                                         */
/*                                                                            */

/*                                                                            */
/* Release: This file is part of the kim-api.git repository.                  */
/*                                                                            */


#ifndef KIM_MODEL_COMPUTE_H_
#define KIM_MODEL_COMPUTE_H_

/* Forward declarations */
#ifndef KIM_LOG_VERBOSITY_DEFINED_
#define KIM_LOG_VERBOSITY_DEFINED_
typedef struct KIM_LogVerbosity KIM_LogVerbosity;
#endif


struct KIM_ModelCompute;

#ifndef KIM_MODEL_COMPUTE_DEFINED_
#define KIM_MODEL_COMPUTE_DEFINED_
typedef struct KIM_ModelCompute KIM_ModelCompute;
#endif

void KIM_ModelCompute_GetModelBufferPointer(
    KIM_ModelCompute const * const modelCompute, void ** const ptr);

void KIM_ModelCompute_LogEntry(KIM_ModelCompute const * const modelCompute,
                               KIM_LogVerbosity const logVerbosity,
                               char const * const message,
                               int const lineNumber,
                               char const * const fileName);

char const *
KIM_ModelCompute_ToString(KIM_ModelCompute const * const modelCompute);

#endif /* KIM_MODEL_COMPUTE_H_ */
