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
// Copyright (c) 2016--2019, Regents of the University of Minnesota.
// All rights reserved.
//
// Contributors:
//    Ryan S. Elliott
//

//
// Release: This file is part of the kim-api.git repository.
//


#ifndef KIM_MODEL_HPP_
#define KIM_MODEL_HPP_

#include <string>

namespace KIM
{
// Forward declarations
class LogVerbosity;
class DataType;
class ModelRoutineName;
class SpeciesName;
class Numbering;
class LengthUnit;
class EnergyUnit;
class ChargeUnit;
class TemperatureUnit;
class TimeUnit;
class ComputeArguments;
class ModelImplementation;

/// This class provides the primary interface to a %KIM API Model object and is
/// meant to be used by Simulators.
///
/// \since 2.0
class Model
{
 public:
  /// Create a new %KIM API Model object.
  ///
  /// Allocates a new %KIM API Model object for use by a Simulator and calls
  /// the Model's KIM::MODEL_ROUTINE_NAME::Create routine.
  ///
  /// \param[in]  numbering The KIM::Numbering value used by the Simulator.
  /// \param[in]  requestedLengthUnit The KIM::LengthUnit requested by the
  ///             Simulator.
  /// \param[in]  requestedEnergyUnit The KIM::EnergyUnit requested by the
  ///             Simulator.
  /// \param[in]  requestedChargeUnit The KIM::ChargeUnit requested by the
  ///             Simulator.
  /// \param[in]  requestedTemperatureUnit The KIM::TemperatureUnit requested
  ///             by the Simulator.
  /// \param[in]  requestedTimeUnit The KIM::TimeUnit requested by the
  ///             Simulator.
  /// \param[in]  modelName The name of the Model to be created.
  /// \param[out] requestedUnitsAccepted An integer that is set to \c true if
  ///             the Model accepts the Simulator's requested, \c false if the
  ///             Model will use units other than those requested by the
  ///             Simulator.
  /// \param[out] model Pointer to the newly created Model object.
  ///
  /// \return \c true if the %KIM API is unable to allocate a new log object.
  /// \return \c true if \c numbering or any of the units are invalid.
  /// \return \c true if the requested model's library cannot be found,
  ///         opened, is of the wrong type, or has some other problem.
  /// \return \c true if the Model's KIM::MODEL_ROUTINE_NAME::Create routine
  ///         returns \c true.
  /// \return \c true if the Model's KIM::MODEL_ROUTINE_NAME::Create routine
  ///         does not set the Model's (1) numbering, (2) units, (3)
  ///         influence distance, (4) numberOfNeighborLists, (5) cutoff values,
  ///         (6) modelWillNotRequesNeighborsOfNoncontributingParticles, (7)
  ///         required KIM::ModelRoutine pointers, (8) supported sepecies codes.
  /// \return \c true if max(cutoffs) > influenceDistance
  /// \return \c true if parameters are registered but not a
  ///         KIM::MODEL_ROUTINE_NAME::Refresh pointer, or vise-versa.
  /// \return \c true if a KIM::MODEL_ROUTINE_NAME::WriteParameterizedModel
  ///         pointer is provided but no parameters are registered.
  /// \return \c true if the Model's KIM::MODEL_ROUTINE_NAME::Create routine
  ///         does not set the Model's KIM::Numbering.
  /// \return \c false otherwise.
  ///
  /// \post \c requestedUnitsAccepted is unchanged and `model == NULL` if an
  ///       error occurs.
  ///
  /// \since 2.0
  static int Create(Numbering const numbering,
                    LengthUnit const requestedLengthUnit,
                    EnergyUnit const requestedEnergyUnit,
                    ChargeUnit const requestedChargeUnit,
                    TemperatureUnit const requestedTemperatureUnit,
                    TimeUnit const requestedTimeUnit,
                    std::string const & modelName,
                    int * const requestedUnitsAccepted,
                    Model ** const model);

  /// Destroy a previously KIM::Model::Create'd object.
  ///
  /// Call the Model's KIM::MODEL_ROUTINE_NAME::Destroy routine and deallocate
  /// the Model object.
  ///
  /// \param[in/out] model Pointer to the Model obejct.
  ///
  /// \pre \c model points to a previously created %KIM API Model object.
  ///
  /// \post `model == NULL`.
  ///
  /// \since 2.0
  static void Destroy(Model ** const model);

  /// Determine presence and required status of the given KIM::ModelRoutineName.
  ///
  /// \param[in]  modelRoutineName The KIM::ModelRoutineName of interest.
  /// \param[out] present \c true if the Model provides the routine, \c false
  ///             otherwise.
  /// \param[out] required \c true if the Model requires the use of the routine,
  ///             \c false otherwise.
  ///
  /// \pre \c present or \c required may be \c NULL if the corresponding value
  ///      is not needed.
  ///
  /// \post \c present and \c required are unchanged if an error occurs.
  ///
  /// \since 2.0
  int IsRoutinePresent(ModelRoutineName const modelRoutineName,
                       int * const present,
                       int * const required) const;

  /// Get the Model's influence distance.
  ///
  /// \param[out] influenceDistance
  ///
  /// \since 2.0
  void GetInfluenceDistance(double * const influenceDistance) const;

  /// Get the Model's neighbor list information
  ///
  /// Each neighbor list has a cutoff value and a flag indicating if the Model
  /// will request the neighbors of noncontributing particles.
  ///
  /// \param[out] numberOfNeighborLists The number of neighbor lists required
  ///             by the Model.
  /// \param[out] cutoffs The cutoff distance for each neighbor list.
  /// \param[out] modelWillNotRequestNeighborsOfNoncontributingParticles
  ///             \c true if such neighbor lists will not be requested,
  ///             \c false otherwise.
  ///
  /// \pre \c numberOfNeighborLists, \c cutoffs, or
  ///      \c modelWillNotRequestNeighborsOfNoncontributingParticles may be
  ///      \c NULL if the corresponding value is not needed.
  ///
  /// \since 2.0
  void GetNeighborListPointers(
      int * const numberOfNeighborLists,
      double const ** const cutoffs,
      int const ** const modelWillNotRequestNeighborsOfNoncontributingParticles)
      const;

  /// Get the Model's unit values.
  ///
  /// \param[out] lengthUnit The Model's KIM::LengthUnit.
  /// \param[out] energyUnit The Model's KIM::EnergyUnit.
  /// \param[out] chargeUnit The Model's KIM::ChargeUnit.
  /// \param[out] temperatureUnit The Model's KIM::TemperatureUnit.
  /// \param[out] timeUnit The Model's KIM::TimeUnit.
  ///
  /// \pre \c lengthUnit, \c energyUnit, \c chargeUnit, \c temperatureUnit, or
  ///      \c timeUnit may be \c NULL if the corresponding value is not needed.
  ///
  /// \since 2.0
  void GetUnits(LengthUnit * const lengthUnit,
                EnergyUnit * const energyUnit,
                ChargeUnit * const chargeUnit,
                TemperatureUnit * const temperatureUnit,
                TimeUnit * const timeUnit) const;

  /// Create a new KIM::ComputeArguments object for the Model object.
  ///
  /// Allocates a new KIM::ComputeArguments object for use by a Simulator and
  /// calls the Model's KIM::MODEL_ROUTINE_NAME::ComputeArgumentsCreate routine.
  ///
  /// \param[inout] computeArguments Pointer to the newly created
  ///                KIM::ComputeArguments object.
  ///
  /// \return \c true if the %KIM API is unable to allocate a new
  ///         KIM::ComputeArguments object.
  /// \return \c true if the Model's
  ///         KIM::MODEL_ROUTINE_NAME::ComputeArgumentsCreate routine
  ///         returns \c true.
  /// \return \c false otherwise.
  ///
  /// \post `computeArguments == NULL` if an error occurs.
  ///
  /// \since 2.0
  int ComputeArgumentsCreate(ComputeArguments ** const computeArguments) const;

  /// Destroy a previously KIM::Model::ComputeArgumentsCreate'd object.
  ///
  /// Call the Model's KIM::MODEL_ROUTINE_NAME::ComputeArgumentsDestroy routine
  /// and deallocate the KIM::ComputeArguments object.
  ///
  /// \param[inout] computeArguments Pointer to the KIM::ComputeArguments
  ///               object.
  ///
  /// \return \c true if the KIM::ComputeArguments object was created by a
  ///         different Model (as identified by its name string).
  /// \return \c true if the Model's
  ///         KIM::MODEL_ROUTINE_NAME::ComputeArgumentsDestroy routine
  ///         returns \c true.
  /// \return \c false otherwise.
  ///
  /// \post \c computeArguments is unchanged if an error occurs, otherwise
  ///       `computeArguments == NULL`.
  ///
  /// \since 2.0
  int ComputeArgumentsDestroy(ComputeArguments ** const computeArguments) const;

  /// Call the Model's KIM::MODEL_ROUTINE_NAME::Compute routine.
  ///
  /// \param[in] computeArguments A KIM::ComputeArguments object.
  ///
  /// \return \c true if \c computeArguments was created by a different Model
  ///         (as identified by its name string).
  /// \return \c true if
  ///        `KIM::ComputeArguments::AreAllRequiredArgumentsAndCallbacksPresent`
  ///         returns \c false for \c computeArguments.
  /// \return \c true if the Model's KIM::MODEL_ROUTINE_NAME::Compute routine
  ///         returns \c true.
  /// \return \c false otherwise.
  ///
  /// \since 2.0
  int Compute(ComputeArguments const * const computeArguments) const;

  /// Call the Model's KIM::MODEL_ROUTINE_NAME::Extension routine.
  ///
  /// \param[in]    extensionID A string uniquely identifying the extension to
  ///               be executed.
  /// \param[inout] extensionStructure Pointer to a data structure of the type
  ///               defined by the extension to be executed.
  ///
  /// \return \c true if the Model does not provide a
  ///         KIM::MODEL_ROUTINE_NAME::Extension routine.
  /// \return \c true if the Model's KIM::MODEL_ROUTINE_NAME::Extension routine
  ///         returns \c true.
  /// \return \c false otherwise.
  ///
  /// \since 2.0
  int Extension(std::string const & extensionID,
                void * const extensionStructure);

  /// Clear influence distance and neighbor list pointers and refresh Model
  /// after parameter changes.
  ///
  /// Nullify the Model's influence distance, neighbor list cutoff, and \c
  /// modelWillNotRequestNeighborsOfNoncontributingParticles pointers.  Then
  /// call the Model's KIM::MODEL_ROUTINE_NAME::Refresh routine.
  ///
  /// \return \c true if the Model does not register any parameters.
  /// \return \c true if the Model's KIM::MODEL_ROUTINE_NAME::Refresh routine
  ///         returns \c ture.
  /// \return \c true if the Model's KIM::MODEL_ROUTINE_NAME::Refresh routine
  ///         does not set the influence distance, the number of neighbor lists,
  ///         the neighbor list cutoffs, or the \c
  ///         modelWillNotRequestNeighborsOfNoncontributingParticles pointer.
  /// \return \c false otherwise.
  ///
  /// \since 2.0
  int ClearThenRefresh();

  /// Call the Model's KIM::MODEL_ROUTINE_NAME::WriteParameterizedModel routine.
  ///
  /// \param[in] path Path string to directory within which the new
  ///            parameterized model files should be written.
  /// \param[in] modelName Name of the parameterized model to be created.  Must
  ///            be a valid C identifier.
  ///
  /// \return \c true if the Model object is not a parameterized model.
  /// \return \c true if \c modelName is not a valid C identifier.
  /// \return \c true if the Model's
  ///         KIM::MODEL_ROUTINE_NAME::WriteParameterizedModel routine returns
  ///         \c true.
  /// \return \c true if the %KIM API is unable to write the \c CMakeLists.txt
  ///         file.
  /// \return \c false otherwise.
  ///
  /// \since 2.0
  int WriteParameterizedModel(std::string const & path,
                              std::string const & modelName) const;

  /// Get the Model's support and code for the requested KIM::SpeciesName.
  ///
  /// \param[in]  speciesName The KIM::SpeciesName of interest.
  /// \param[out] speciesIsSupported \c true the Model supports the species of
  ///             interest, \c false otherwise.
  /// \param[out] code Value used by the Model to refer to the species of
  ///             interest.
  ///
  /// \return \c true if \c speciesName is invalid.
  /// \return \c false otherwise.
  ///
  /// \pre \c code may be \c NULL if the value is not needed.
  ///
  /// \post \c speciesIsSupported and \c code are unchanged if an error occurs.
  ///       \c code is unchanged if `speciesIsSupported == false`.
  ///
  /// \since 2.0
  int GetSpeciesSupportAndCode(SpeciesName const speciesName,
                               int * const speciesIsSupported,
                               int * const code) const;

  /// Get the number of parameter arrays provided by the Model.
  ///
  /// \param[out] numberOfParameters The number of parameter arrays provided
  ///             by the Model.
  ///
  /// \since 2.0
  void GetNumberOfParameters(int * const numberOfParameters) const;

  /// Get the metadata associated with one of the Model's parameter arrays.
  ///
  /// \param[in]  parameterIndex Zero-based index for the parameter array.
  /// \param[out] dataType The KIM::DataType value for the parameter array.
  /// \param[out] extent The number of parameters in the array.
  /// \param[out] name A string identifying the parameter array (will be a valid
  ///             C identifier).
  /// \param[out] description A free-from string description of the parameter
  ///             array's content.
  ///
  /// \return \c true if \c parameterIndex is invalid
  /// \return \c false otherwise.
  ///
  /// \pre \c dataType, \c extent, \c name, or \description may be \c NULL if
  ///      the corresponding value is not needed.
  ///
  /// \since 2.0
  int GetParameterMetadata(int const parameterIndex,
                           DataType * const dataType,
                           int * const extent,
                           std::string const ** const name,
                           std::string const ** const description) const;

  /// Get a parameter value from the Model.
  ///
  /// \param[in]  parameterIndex Zero-based index for the parameter array of
  ///             interest.
  /// \param[in]  arrayIndex Zero-based index within the array for the parameter
  ///             of interest.
  /// \param[out] parameterValue The value of the parameter of interest.
  ///
  /// \return \c true if \c parameterIndex is invalid.
  /// \return \c true if \c arrayIndex is invalid.
  /// \return \c false otherwise.
  ///
  /// \since 2.0
  int GetParameter(int const parameterIndex,
                   int const arrayIndex,
                   int * const parameterValue) const;

  /// \overload
  int GetParameter(int const parameterIndex,
                   int const arrayIndex,
                   double * const parameterValue) const;

  /// Set a parameter value for the Model.
  ///
  /// \param[in] parameterIndex Zero-based index for the parameter array of
  ///            interest.
  /// \param[in] arrayIndex Zero-based index within the array for the parameter
  ///            of interest.
  /// \param[in] parameterValue The new value for the parameter of interest.
  ///
  /// \return \c true if \c parameterIndex is invalid.
  /// \return \c true if \c arrayIndex is invalid.
  /// \return \c false otherwise.
  ///
  /// \since 2.0
  int SetParameter(int const parameterIndex,
                   int const arrayIndex,
                   int const parameterValue);

  /// \overload
  int SetParameter(int const parameterIndex,
                   int const arrayIndex,
                   double const parameterValue);

  /// Set the Simulator's buffer pointer within the Model.
  ///
  /// The simulator buffer pointer may be used by the Simulator to associate
  /// a memory buffer with an instance of the KIM::Model object.
  ///
  /// \param[in] ptr The simulator buffer pointer.
  ///
  /// \since 2.0
  void SetSimulatorBufferPointer(void * const ptr);

  /// Get the Simulator's buffer pointer from the Model.
  ///
  /// \param[out] ptr The simulator buffer pointer.
  ///
  /// \note `ptr == NULL` if the Simulator has not previously called
  ///       KIM::Model::SetSimulatorBufferPointer.
  ///
  /// \since 2.0
  void GetSimulatorBufferPointer(void ** const ptr) const;

  /// Get a string representing the internal state of the Model object.
  ///
  /// This string is primarily meant for use as a debugging tool.  The string
  /// may be quite long.  It begins and ends with lines consisting only of \c
  /// ='s.
  ///
  /// \since 2.0
  std::string const & String() const;

  /// Set the identity of the log object associated with the Model.
  ///
  /// \param[in] logID String identifying the Model's log object.
  ///
  /// \since 2.0
  void SetLogID(std::string const & logID);

  /// Push a new KIM::LogVerbosity onto the Model's log object verbosity stack.
  ///
  /// \param[in] logVerbosity A KIM::LogVerbosity value.
  ///
  /// \since 2.0
  void PushLogVerbosity(LogVerbosity const logVerbosity);

  /// Pop a KIM::LogVerbosity from the Model's log object verbosity stack.
  ///
  /// \since 2.0
  void PopLogVerbosity();

 private:
  // do not allow copy constructor or operator=
  Model(Model const &);
  void operator=(Model const &);

  Model();
  ~Model();

  ModelImplementation * pimpl;
};  // class Model
}  // namespace KIM

#endif  // KIM_MODEL_HPP_
