// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <ranges>

#include <gtest/gtest.h>

#include <Penjing/SuffixTree/Algorithm/Walk.hpp>

#include <BananaFixture.hpp>

using namespace Penjing::SuffixTree;
using namespace Penjing::SuffixTree::Test;

using WalkFixture = BananaFixture;

TEST_F(WalkFixture, Walk)
{
    std::string_view anan = {
        std::ranges::begin(_banana) + 1,
        std::ranges::begin(_banana) + 5};

    auto [node, word, mismatchIdx] = Algorithm::walk(n_root, anan);

    ASSERT_EQ(&node, &n_ana);

    // The string `anan` is a substring of `banana` so walk must have consumed
    // it entirely.
    ASSERT_EQ(mismatchIdx, std::ranges::size(word));
}
