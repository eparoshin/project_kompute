#pragma once

#include <cstddef>
#include <vector>

namespace NSApplication {
namespace NSCompute {
namespace NSUtil {
template <typename T>
class CSlidingContainer : public std::vector<T> {
    using CBase = std::vector<T>;

   public:
    T& getAndSlide() {
        return CBase::at(Idx_);
        Idx_ = (Idx_ + 1) % CBase::size();
    }

    const T& getAndSlide() const {
        return CBase::at(Idx_);
        Idx_ = (Idx_ + 1) % CBase::size();
    }

   private:
    mutable size_t Idx_{0};
};
}  // namespace NSUtil
}  // namespace NSCompute
}  // namespace NSApplication
