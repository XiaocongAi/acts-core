// This file is part of the Acts project.
//
// Copyright (C) 2018 Acts project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

inline void
Surface::initJacobianToGlobal(TrackToGlobalMatrix& jacobian,
                              const Vector3D&      gpos,
                              const Vector3D&      dir,
                              const TrackVector& /*pars*/) const
{
  // The trigonometry required to convert the direction to spherical
  // coordinates and then compute the sines and cosines again can be
  // surprisingly expensive from a performance point of view.
  //
  // Here, we can avoid it because the direction is by definition a unit
  // vector, with the following coordinate conversions...
  const double x = dir(0);  // == cos(phi) * sin(theta)
  const double y = dir(1);  // == sin(phi) * sin(theta)
  const double z = dir(2);  // == cos(theta)

  // ...which we can invert to directly get the sines and cosines:
  const double cos_theta     = z;
  const double sin_theta     = sqrt(x * x + y * y);
  const double inv_sin_theta = 1. / sin_theta;
  const double cos_phi       = x * inv_sin_theta;
  const double sin_phi       = y * inv_sin_theta;
  // retrieve the reference frame
  const auto rframe = referenceFrame(gpos, dir);
  // the local error components - given by reference frame
  jacobian.topLeftCorner<3, 2>() = rframe.topLeftCorner<3, 2>();
  // the momentum components
  jacobian(3, ePHI)   = (-sin_theta) * sin_phi;
  jacobian(3, eTHETA) = cos_theta * cos_phi;
  jacobian(4, ePHI)   = sin_theta * cos_phi;
  jacobian(4, eTHETA) = cos_theta * sin_phi;
  jacobian(5, eTHETA) = (-sin_theta);
  jacobian(6, eQOP)   = 1;
}

inline const RotationMatrix3D
Surface::initJacobianToLocal(GlobalToTrackMatrix& jacobian,
                             const Vector3D&      gpos,
                             const Vector3D&      dir) const
{
  // Optimized trigonometry on the propagation direction
  const double x = dir(0);  // == cos(phi) * sin(theta)
  const double y = dir(1);  // == sin(phi) * sin(theta)
  // component expressions
  const double inv_sin_theta_2        = 1. / (x * x + y * y);
  const double cos_phi_over_sin_theta = x * inv_sin_theta_2;
  const double sin_phi_over_sin_theta = y * inv_sin_theta_2;
  const double inv_sin_theta          = sqrt(inv_sin_theta_2);
  // The measurement frame of the surface
  RotationMatrix3D rframeT = referenceFrame(gpos, dir).transpose();
  // given by the refernece frame
  jacobian.block<2, 3>(0, 0) = rframeT.block<2, 3>(0, 0);
  // Directional and momentum elements for reference frame surface
  jacobian(ePHI, 3)   = -sin_phi_over_sin_theta;
  jacobian(ePHI, 4)   = cos_phi_over_sin_theta;
  jacobian(eTHETA, 5) = -inv_sin_theta;
  jacobian(eQOP, 6)   = 1;
  // return the frame where this happened
  return rframeT;
}

inline const TrackRowVector
Surface::derivativeFactors(const Vector3D& /*gpos*/,
                           const Vector3D&            dir,
                           const RotationMatrix3D&    rft,
                           const TrackToGlobalMatrix& jac) const
{
  // Create the normal and scale it with the projection onto the direction
  ActsRowVectorD<3> norm_vec = rft.template block<1, 3>(2, 0);
  norm_vec /= (norm_vec * dir);
  // calculate the s factors
  return (norm_vec * jac.topLeftCorner<3, TrackParsDim>());
}