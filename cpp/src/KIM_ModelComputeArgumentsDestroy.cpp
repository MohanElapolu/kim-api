//
// CDDL HEADER START
//
// The contents of this file are subject to the terms of the Common Development
// and Distribution License Version 1.0 (the "License").
//
// You can obtain a copy of the license at
// http://www.opensource.org/licenses/CDDL-1.0.  See the License for the
// specific language governing permissions and limitations under the License.
//
// When distributing Covered Code, include this CDDL HEADER in each file and
// include the License file in a prominent location with the name LICENSE.CDDL.
// If applicable, add the following below this CDDL HEADER, with the fields
// enclosed by brackets "[]" replaced with your own identifying information:
//
// Portions Copyright (c) [yyyy] [name of copyright owner]. All rights reserved.
//
// CDDL HEADER END
//

//
// Copyright (c) 2016--2020, Regents of the University of Minnesota.
// All rights reserved.
//
// Contributors:
//    Ryan S. Elliott
//

//
// Release: This file is part of the kim-api-2.2.0 package.
//


#include <cstddef>

#ifndef KIM_LOG_VERBOSITY_HPP_
#include "KIM_LogVerbosity.hpp"
#endif

#ifndef KIM_MODEL_COMPUTE_ARGUMENTS_DESTROY_HPP_
#include "KIM_ModelComputeArgumentsDestroy.hpp"
#endif

#ifndef KIM_COMPUTE_ARGUMENTS_IMPLEMENTATION_HPP_
#include "KIM_ComputeArgumentsImplementation.hpp"
#endif

#define CONVERT_POINTER                  \
  ComputeArgumentsImplementation * pImpl \
      = reinterpret_cast<ComputeArgumentsImplementation *>(pimpl)


namespace KIM
{
void ModelComputeArgumentsDestroy::GetModelBufferPointer(
    void ** const ptr) const
{
  CONVERT_POINTER;

  pImpl->GetModelBufferPointer(ptr);
}

void ModelComputeArgumentsDestroy::LogEntry(LogVerbosity const logVerbosity,
                                            std::string const & message,
                                            int const lineNumber,
                                            std::string const & fileName) const
{
  CONVERT_POINTER;

  pImpl->LogEntry(logVerbosity, message, lineNumber, fileName);
}

void ModelComputeArgumentsDestroy::LogEntry(LogVerbosity const logVerbosity,
                                            std::stringstream const & message,
                                            int const lineNumber,
                                            std::string const & fileName) const
{
  CONVERT_POINTER;

  pImpl->LogEntry(logVerbosity, message, lineNumber, fileName);
}

std::string const & ModelComputeArgumentsDestroy::ToString() const
{
  CONVERT_POINTER;

  return pImpl->ToString();
}

ModelComputeArgumentsDestroy::ModelComputeArgumentsDestroy() : pimpl(NULL) {}

ModelComputeArgumentsDestroy::~ModelComputeArgumentsDestroy() {}

}  // namespace KIM
