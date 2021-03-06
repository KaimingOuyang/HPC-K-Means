/**
 * @file random_basis.hpp
 * @author Ryan Curtin
 *
 * Generate a random d-dimensional basis.
 *
 * This file is part of mlpack 2.0.1.
 *
 * mlpack is free software; you may redstribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef __MLPACK_CORE_MATH_RANDOM_BASIS_HPP
#define __MLPACK_CORE_MATH_RANDOM_BASIS_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {
namespace math {

/**
 * Create a random d-dimensional orthogonal basis, storing it in the given
 * matrix.
 *
 * @param basis Matrix to store basis in.
 * @param d Desired number of dimensions in the basis.
 */
void RandomBasis(arma::mat& basis, const size_t d);

} // namespace math
} // namespace mlpack

#endif
