#include <iostream>

namespace boost {
  namespace algorithm {
    template<typename OutputIteratorT, typename RangeT> 
      OutputIteratorT 
      to_lower_copy(OutputIteratorT, const RangeT &, 
                    const std::locale & = std::locale());
    template<typename SequenceT> 
      SequenceT to_lower_copy(const SequenceT &, 
                              const std::locale & = std::locale());
    template<typename WritableRangeT> 
      std::string to_lower(WritableRangeT &, const std::locale & = std::locale());
    template<typename OutputIteratorT, typename RangeT> 
      OutputIteratorT 
      to_upper_copy(OutputIteratorT, const RangeT &, 
                    const std::locale & = std::locale());
    template<typename SequenceT> 
      SequenceT to_upper_copy(const SequenceT &, 
                              const std::locale & = std::locale());
    template<typename WritableRangeT> 
      void to_upper(WritableRangeT &, const std::locale & = std::locale());
  }
}