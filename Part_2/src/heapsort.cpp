#include "../include/heapsort.h"

#include <utility>  // for std::swap
#include <vector>

namespace mapra {

// Sift‐down the node at index `root` in a heap of size `count`.
static void Heapify(std::vector<unsigned int>& a, std::size_t root,
                    std::size_t count) {
  while (true) {
    std::size_t left = 2 * root + 1;
    if (left >= count) break;
    std::size_t right = left + 1;
    std::size_t child = (right < count && a[right] > a[left]) ? right : left;
    if (a[child] > a[root]) {
      std::swap(a[root], a[child]);
      root = child;
    } else {
      break;
    }
  }
}

void Heapsort(std::vector<unsigned int>& array) {
  const std::size_t n = array.size();
  // Build max‐heap
  for (std::size_t start = (n / 2); start-- > 0;) {
    Heapify(array, start, n);
  }
  // Extract elements one by one
  for (std::size_t end = n; end > 1; --end) {
    std::swap(array[0], array[end - 1]);
    Heapify(array, 0, end - 1);
  }
}

}  // namespace mapra
