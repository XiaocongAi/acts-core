// This file is part of the Acts project.
//
// Copyright (C) 2017-2018 Acts project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// Boost include(s)
#define BOOST_TEST_MODULE Propagator Tests

#include <boost/test/included/unit_test.hpp>
// leave blank
#include <boost/test/data/test_case.hpp>

#include <cmath>
#include <iostream>

#include <boost/test/data/test_case.hpp>

#include "Acts/ActsVersion.hpp"
#include "Acts/EventData/TrackParameters.hpp"
#include "Acts/MagneticField/ConstantBField.hpp"
#include "Acts/Propagator/AtlasStepper.hpp"
#include "Acts/Propagator/EigenStepper.hpp"
#include "Acts/Propagator/Propagator.hpp"
#include "Acts/Propagator/detail/DebugOutputActor.hpp"
#include "Acts/Surfaces/CylinderSurface.hpp"
#include "Acts/Surfaces/DiscSurface.hpp"
#include "Acts/Surfaces/PlaneSurface.hpp"
#include "Acts/Surfaces/StrawSurface.hpp"
#include "Acts/Utilities/Definitions.hpp"
#include "Acts/Utilities/Units.hpp"
#include "PropagationTestHelper.hpp"

namespace bdata = boost::unit_test::data;
namespace tt    = boost::test_tools;

namespace Acts {

namespace IntegrationTest {

  using BFieldType          = ConstantBField;
  using EigenStepperType    = EigenStepper<BFieldType>;
  using AtlasStepperType    = AtlasStepper<BFieldType>;
  using EigenPropagatorType = Propagator<EigenStepperType>;
  using AtlasPropagatorType = Propagator<AtlasStepperType>;

  // number of tests
  const int  ntests = 100;
  const int  skip   = 0;
  const bool covtpr = true;
  const bool debug  = false;

  // setup propagator with constant B-field
  const double        Bz = 2. * units::_T;
  BFieldType          bField(0, 0, Bz);
  EigenStepperType    estepper(bField);
  EigenPropagatorType epropagator(std::move(estepper));
  AtlasStepperType    astepper(bField);
  AtlasPropagatorType apropagator(std::move(astepper));

  // The constant field test
  /// test forward propagation in constant magnetic field
  BOOST_DATA_TEST_CASE(
      constant_bfieldforward_propagation_,
      bdata::random((bdata::seed = 0,
                     bdata::distribution
                     = std::uniform_real_distribution<>(0.4 * units::_GeV,
                                                        10. * units::_GeV)))
          ^ bdata::random((bdata::seed = 1,
                           bdata::distribution
                           = std::uniform_real_distribution<>(-M_PI, M_PI)))
          ^ bdata::random((bdata::seed = 2,
                           bdata::distribution
                           = std::uniform_real_distribution<>(0.1, M_PI - 0.1)))
          ^ bdata::random((bdata::seed = 3,
                           bdata::distribution
                           = std::uniform_int_distribution<>(0, 1)))
          ^ bdata::xrange(ntests),
      pT,
      phi,
      theta,
      charge,
      index)
  {
    if (index < skip) {
      return;
    }

    double dcharge = -1 + 2 * charge;
    // constant field propagation atlas stepper
    auto aposition = constant_field_propagation(
        apropagator, pT, phi, theta, dcharge, index, Bz);
    // constant field propagation eigen stepper
    auto eposition = constant_field_propagation(
        epropagator, pT, phi, theta, dcharge, index, Bz);
    // check consistency
    BOOST_CHECK(eposition.isApprox(aposition));
  }

// The actual test - needs to be included to avoid
// template inside template definition through boost
#include "PropagationTestBase.hpp"

}  // namespace Test

}  // namespace Acts
