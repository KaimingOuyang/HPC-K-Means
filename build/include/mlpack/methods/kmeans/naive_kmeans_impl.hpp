/**
 * @file naive_kmeans_impl.hpp
 * @author Ryan Curtin
 *
 * An implementation of a naively-implemented step of the Lloyd algorithm for
 * k-means clustering.  This may still be the best choice for small datasets or
 * datasets with very high dimensionality.
 *
 * This file is part of mlpack 2.0.1.
 *
 * mlpack is free software; you may redstribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef __MLPACK_METHODS_KMEANS_NAIVE_KMEANS_IMPL_HPP
#define __MLPACK_METHODS_KMEANS_NAIVE_KMEANS_IMPL_HPP

// In case it hasn't been included yet.
#include "naive_kmeans.hpp"
//#include "acml.h"
//#include "papi.h"
namespace mlpack {
namespace kmeans {

template<typename MetricType, typename MatType>
NaiveKMeans<MetricType, MatType>::NaiveKMeans(const MatType& dataset,
		MetricType& metric) :
		dataset(dataset), metric(metric), distanceCalculations(0) {

	ddt.zeros(dataset.n_cols, 1);
	dataset_t = dataset.t();
	double sum;
	for (size_t i = 0; i < dataset.n_cols; i++) {
		sum = 0;
		for (size_t j = 0; j < dataset.n_rows; j++)
			sum += dataset(j, i) * dataset(j, i);
		ddt(i, 0) = sum;
	}
	/* Nothing to do. */}

// Run a single iteration.
template<typename MetricType, typename MatType>
double NaiveKMeans<MetricType, MatType>::Iterate(arma::mat& centroids,
		arma::mat& newCentroids, arma::Col<size_t>& counts) {
	newCentroids.zeros(centroids.n_rows, centroids.n_cols);
	counts.zeros(centroids.n_cols);

	/* this is our optimized way of calculating distance */

	/* initialize dist_matrix first */
	arma::mat dist_matrix(dataset.n_cols, centroids.n_cols);
	// float gemm_real_time = 0.0;
	// float gemm_proc_time = 0.0;
	// long long gemm_flpins = 0.0;
	// float gemm_mflops = 0.0;

	//if (PAPI_flops(&gemm_real_time, &gemm_proc_time, &gemm_flpins, &gemm_mflops) < PAPI_OK) {
	//    std::cout << "PAPI ERROR" << std::endl;
	//}

	dist_matrix = dataset_t * centroids;

	// double * data_ptr = dataset_t.memptr();
	// double * cent_ptr = centroids.memptr();
	// double * dist_ptr = dist_matrix.memptr();

	// dgemm('N', 'N',
	//     dataset_t.n_rows,
	//     centroids.n_cols,
	//     dataset_t.n_cols,
	//     1.0,
	//     data_ptr, dataset_t.n_rows,
	//     cent_ptr, centroids.n_rows,
	//     0.0,
	//     dist_ptr, dist_matrix.n_rows);

	//if (PAPI_flops(&gemm_real_time, &gemm_proc_time, &gemm_flpins, &gemm_mflops) < PAPI_OK) {
	//    std::cout << "PAPI ERROR" << std::endl;
	//}
	//std::cout << "gemm time:" << gemm_real_time <<"---flpins:"<<gemm_flpins<< "---mflops:" << gemm_mflops << std::endl;
	//PAPI_shutdown();
	//float gemv_real_time = 0.0;
	//float gemv_proc_time = 0.0;
	//long long gemv_flpins = 0.0;
	//float gemv_mflops = 0.0;

	//if (PAPI_flops(&gemv_real_time, &gemv_proc_time, &gemv_flpins, &gemv_mflops) < PAPI_OK) {
	//    std::cout << "PAPI ERROR" << std::endl;
	//}

	arma::mat cct(centroids.n_cols, 1);
	//arma::mat centroids_t = centroids.t();
	double sum;
	for (size_t i = 0; i < centroids.n_cols; i++) {
		sum = 0;
		for (size_t j = 0; j < centroids.n_rows; j++)
			sum += centroids(j, i) * centroids(j, i);
		cct(i, 0) = sum;
	}

	// d * c^T -> -2 * d * c^T
	dist_matrix = -2 * dist_matrix;

	// //d * d^T - 2 * d * c^T

//	for (size_t i = 0; i < centroids.n_cols; i++) {
//		dist_matrix.col(i) += ddt;
//	}

	dist_matrix.each_col() += ddt;
	//d * d^T + c * c^T - 2 * d * c^T
	arma::mat dist_matrix_t = dist_matrix.t();

	//for (size_t i = 0; i < dataset.n_cols; i++) {
//		dist_matrix_t.col(i) += cct; // for_each
//	}

	dist_matrix_t.each_col() += cct;

	//if (PAPI_flops(&gemv_real_time, &gemv_proc_time, &gemv_flpins, &gemv_mflops) < PAPI_OK) {
	//  std::cout << "PAPI ERROR" << std::endl;
	//}
	//std::cout << "gemv_time:" << gemv_real_time <<"---flpins:"<<gemv_flpins<< "---mflops:" << gemv_mflops << std::endl;
	//PAPI_shutdown();

	// for (size_t i = 0; i < dataset.n_cols; i++)
	// {
	//   for (size_t j = 0; j < centroids.n_cols; j++)
	//   {
	//     dist_matrix(i ,j) = metric.Evaluate(dataset.col(i), centroids.col(j));
	//   }
	// }
	//std::cout << "total time for dist calc: " << gemm_real_time + gemv_real_time << std::endl;
	//float other_real_time = 0.0;
	//float other_proc_time = 0.0;
	//long long other_flpins = 0.0;
	//float other_mflops = 0.0;
	//timing start (this has to be done for both optimized and naive way)
	//if (PAPI_flops(&other_real_time, &other_proc_time, &other_flpins, &other_mflops) < PAPI_OK) {
	//  std::cout << "PAPI ERROR" << std::endl;
	//}
	// Find the closest centroid to each point and update the new centroids.
	for (size_t i = 0; i < dataset.n_cols; i++) {
		// Find the closest centroid to this point.
		//double minDistance = std::numeric_limits<double>::infinity();
		arma::uword closestCluster; // Invalid value.
		dist_matrix_t.col(i).min(closestCluster);

//		 for (size_t j = 0; j < centroids.n_cols; j++)
//		 {
//		 const double distance = dist_matrix_t(j, i);
//		 const double distance = pow(dist_matrix(i, j), (1.0 / 2.0));
//		 if (distance < minDistance)
//		 {
//		 minDistance = distance;
//		 closestCluster = j;
//		 }
//		 }

		Log::Assert(closestCluster != centroids.n_cols);

		// We now have the minimum distance centroid index.  Update that centroid.
		newCentroids.col(closestCluster) += dataset.col(i);
		++counts(closestCluster);
	}

	// Now normalize the centroid.
	for (size_t i = 0; i < centroids.n_cols; ++i)
		if (counts(i) != 0)
			newCentroids.col(i) /= counts(i);
		else
			newCentroids.col(i).fill(DBL_MAX); // Invalid value.

	distanceCalculations += centroids.n_cols * dataset.n_cols;

	// Calculate cluster distortion for this iteration.
	double cNorm;
	for (size_t i = 0; i < centroids.n_cols; ++i) {
		cNorm = std::pow(metric.Evaluate(centroids.col(i), newCentroids.col(i)),
				2.0);
	}
	distanceCalculations += centroids.n_cols;

	//timing end
	//if (PAPI_flops(&other_real_time, &other_proc_time, &other_flpins, &other_mflops) < PAPI_OK) {
	//    std::cout << "PAPI ERROR" << std::endl;
	//return -1;
	//}
	//std::cout << "other time:" << other_real_time <<"---flpins:"<<other_flpins<< "---mflops:" << other_mflops << std::endl;
	//PAPI_shutdown();

	return std::sqrt(cNorm);
}

} // namespace kmeans
} // namespace mlpack

#endif
