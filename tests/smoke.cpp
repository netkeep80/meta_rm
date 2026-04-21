#include <catch2/catch_test_macros.hpp>

#include <meta_rm/meta_rm.hpp>

#include <type_traits>

namespace m = meta_rm;

namespace {

using subject_meta = m::make_meta<"Subject", "Root subject type">;
using base_meta = m::make_meta<"Base", "Base mixin type">;
using extra_meta = m::make_meta<"Extra", "Extra variadic payload">;
using then_meta = m::make_meta<"Then", "Then branch type">;
using else_meta = m::make_meta<"Else", "Else branch type">;
using branch_meta = m::make_meta<"Branch", "Simple branch object">;

using Subject = m::make_entity<subject_meta>;
using Base = m::make_entity<base_meta>;
using Extra = m::make_entity<extra_meta>;
using Then = m::make_entity<then_meta>;
using Else = m::make_entity<else_meta>;

using InheritNode = m::rm<Subject, m::inherit_r, Base>;

} // namespace

TEST_CASE("core header compiles and exposes primitives", "[smoke]") {
    STATIC_REQUIRE(m::is_rm_v<InheritNode>);
    STATIC_REQUIRE_FALSE(m::is_rm_v<Subject>);
}

TEST_CASE("inherit_r attaches object and accepts extra payload", "[smoke][inherit]") {
    using InheritResult = InheritNode::type<Extra>;
    using InheritExpected = m::entity<subject_meta, Base, Extra>;
    STATIC_REQUIRE(std::is_same_v<InheritResult, InheritExpected>);
}

TEST_CASE("if_r picks the correct branch", "[smoke][if]") {
    using IfTrue = m::rm<
        Subject,
        m::if_r,
        m::branch<m::true_c, Then, Else, branch_meta>
    >;
    using IfFalse = m::rm<
        Subject,
        m::if_r,
        m::branch<m::false_c, Then, Else, branch_meta>
    >;

    STATIC_REQUIRE(std::is_same_v<IfTrue::type<>, Then>);
    STATIC_REQUIRE(std::is_same_v<IfFalse::type<>, Else>);
}

TEST_CASE("introspection exposes meta and children", "[smoke][introspection]") {
    using Meta = typename m::node_meta<InheritNode>::type;
    STATIC_REQUIRE(Meta::name.value[0] == 'r');

    using Children = typename m::node_children<InheritNode>::type;
    STATIC_REQUIRE(std::is_same_v<Children, m::type_list<Subject, m::inherit_r, Base>>);
}
