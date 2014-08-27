/******************************************************************************
 *
 *  Copyright (C) 2014 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <gtest/gtest.h>

extern "C" {
#include "allocation_tracker.h"

void allocation_tracker_uninit(void);
}

TEST(AllocationTrackerTest, test_uninit_no_bad_effects) {
  void *dummy_allocation = malloc(4);

  // Ensure uninitialized state (previous tests may have called init)
  allocation_tracker_uninit();

  allocation_tracker_notify_alloc(dummy_allocation, 4);
  EXPECT_EQ(0U, allocation_tracker_expect_no_allocations()); // should not have registered an allocation
  allocation_tracker_notify_free(dummy_allocation);
  EXPECT_EQ(0U, allocation_tracker_expect_no_allocations());

  free(dummy_allocation);
}

TEST(AllocationTrackerTest, test_inited_basic) {
  allocation_tracker_init();

  void *dummy_allocation = malloc(4);

  allocation_tracker_notify_alloc(dummy_allocation, 4);
  EXPECT_EQ(4U, allocation_tracker_expect_no_allocations()); // should have registered the allocation
  allocation_tracker_notify_free(dummy_allocation);
  EXPECT_EQ(0U, allocation_tracker_expect_no_allocations());

  free(dummy_allocation);
}
