#pragma once

namespace dip
{
	class Utils
	{
	public:
		Utils();
		~Utils();

		float quicksort(float *a, int p, int q);
		int partition(float *a, int p, int q);
	};
}; // namespace dip
