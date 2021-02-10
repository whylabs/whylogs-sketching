/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef _AUXHASHMAP_HPP_
#define _AUXHASHMAP_HPP_

#include <iostream>
#include <memory>
#include <functional>

#include "coupon_iterator.hpp"

namespace datasketches {

template<typename A>
class AuxHashMap final {
  public:
    AuxHashMap(int lgAuxArrInts, int lgConfigK, const A& allocator);
    static AuxHashMap* newAuxHashMap(int lgAuxArrInts, int lgConfigK, const A& allocator);
    static AuxHashMap* newAuxHashMap(const AuxHashMap<A>& that);

    static AuxHashMap* deserialize(const void* bytes, size_t len,
                                   int lgConfigK,
                                   int auxCount, int lgAuxArrInts,
                                   bool srcCompact, const A& allocator);
    static AuxHashMap* deserialize(std::istream& is, int lgConfigK,
                                   int auxCount, int lgAuxArrInts,
                                   bool srcCompact, const A& allocator);
    virtual ~AuxHashMap() = default;
    static std::function<void(AuxHashMap<A>*)> make_deleter();
    
    AuxHashMap* copy() const;
    int getUpdatableSizeBytes() const;
    int getCompactSizeBytes() const;

    int getAuxCount() const;
    int* getAuxIntArr();
    int getLgAuxArrInts() const;

    coupon_iterator<A> begin(bool all = false) const;
    coupon_iterator<A> end() const;

    void mustAdd(int slotNo, int value);
    int mustFindValueFor(int slotNo) const;
    void mustReplace(int slotNo, int value);

  private:
    typedef typename std::allocator_traits<A>::template rebind_alloc<AuxHashMap<A>> ahmAlloc;

    using vector_int = std::vector<int, typename std::allocator_traits<A>::template rebind_alloc<int>>;

    // static so it can be used when resizing
    static int find(const int* auxArr, int lgAuxArrInts, int lgConfigK, int slotNo);

    void checkGrow();
    void growAuxSpace();

    const int lgConfigK;
    int lgAuxArrInts;
    int auxCount;
    vector_int entries;
};

}

#include "AuxHashMap-internal.hpp"

#endif /* _AUXHASHMAP_HPP_ */
