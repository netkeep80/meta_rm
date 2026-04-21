#include <catch2/catch_test_macros.hpp>

#include <meta_rm/meta_rm.hpp>

#include <type_traits>

namespace m = meta_rm;

namespace {

using subject_meta = m::make_meta<"Subject", "Root subject type">;
using mix1_meta = m::make_meta<"Mix1", "First mixin">;
using mix2_meta = m::make_meta<"Mix2", "Second mixin">;
using mix3_meta = m::make_meta<"Mix3", "Third mixin">;

using Subject = m::make_entity<subject_meta>;
using Mix1 = m::make_entity<mix1_meta>;
using Mix2 = m::make_entity<mix2_meta>;
using Mix3 = m::make_entity<mix3_meta>;

using Tree = m::rm<
    m::rm<
        m::rm<Subject, m::inherit_r, Mix1>,
        m::inherit_r,
        Mix2
    >,
    m::inherit_r,
    Mix3
>;

} // namespace

TEST_CASE("rm<Subject, inherit_r, Mix> chain resolves to expanded entity", "[inherit][chain]") {
    using Result = Tree::type<>;
    using Expected = m::entity<subject_meta, Mix1, Mix2, Mix3>;
    STATIC_REQUIRE(std::is_same_v<Result, Expected>);
}

TEST_CASE("rm ternary structure is fully introspectable", "[inherit][introspection]") {
    using InnerInner = m::rm<Subject, m::inherit_r, Mix1>;
    using Inner = m::rm<InnerInner, m::inherit_r, Mix2>;

    STATIC_REQUIRE(std::is_same_v<typename Tree::S, Inner>);
    STATIC_REQUIRE(std::is_same_v<typename Tree::R, m::inherit_r>);
    STATIC_REQUIRE(std::is_same_v<typename Tree::O, Mix3>);
}

TEST_CASE("single inherit_r node attaches a mixin", "[inherit][single]") {
    using Node = m::rm<Subject, m::inherit_r, Mix1>;
    using Result = Node::type<>;
    using Expected = m::entity<subject_meta, Mix1>;
    STATIC_REQUIRE(std::is_same_v<Result, Expected>);
}
