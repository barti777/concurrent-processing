#ifndef _search_max

#define _search_max

#ifdef __cplusplus
extern "C"
{
#endif

	double search_max(
		double *A,
		int p,
		int k);

	double search_max_openmp_simple(
		double *A,
		int p,
		int k);

	double search_max_openmp_task(
		double *A,
		int p,
		int k);

	double bin_search_max(
		double *A,
		int p,
		int k);

	double bin_search_max_openmp(
		double *A,
		int p,
		int k);

#ifdef __cplusplus
}
#endif

#endif
