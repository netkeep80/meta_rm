#include <catch2/catch_test_macros.hpp>

#include <meta_rm/meta_rm.hpp>

#include <type_traits>

namespace m = meta_rm;

namespace {

using subject_meta = m::make_meta<"Subject", "Root subject type">;
using mix_meta = m::make_meta<"Mix", "Mixin for root-adjacency tests">;

using Subject = m::make_entity<subject_meta>;
using Mix = m::make_entity<mix_meta>;

} // namespace

TEST_CASE("rm_void is self-closed in its three ternary slots", "[root][self-closure]") {
    STATIC_REQUIRE(std::is_same_v<typename m::rm_void::S, m::rm_void>);
    STATIC_REQUIRE(std::is_same_v<typename m::rm_void::R, m::rm_void>);
    STATIC_REQUIRE(std::is_same_v<typename m::rm_void::O, m::rm_void>);
}

TEST_CASE("rm_void is recognized as an rm node", "[root][is_rm]") {
    STATIC_REQUIRE(m::is_rm_v<m::rm_void>);
}

TEST_CASE("rm_void introspection exposes rm_node_meta and three self-children", "[root][introspection]") {
    using Meta = typename m::node_meta<m::rm_void>::type;
    STATIC_REQUIRE(std::is_same_v<Meta, m::rm_node_meta>);

    using Children = typename m::node_children<m::rm_void>::type;
    STATIC_REQUIRE(std::is_same_v<Children, m::type_list<m::rm_void, m::rm_void, m::rm_void>>);
}

TEST_CASE("rm_void resolves to itself through ::type<>", "[root][resolve]") {
    STATIC_REQUIRE(std::is_same_v<typename m::rm_void::type<>, m::rm_void>);
    STATIC_REQUIRE(std::is_same_v<m::resolve_t<m::rm_void>, m::rm_void>);
}

TEST_CASE("nested rm<...> constructions still compile alongside rm_void", "[root][coexistence]") {
    using Node = m::rm<Subject, m::inherit_r, Mix>;
    using Result = Node::type<>;
    using Expected = m::entity<subject_meta, Mix>;
    STATIC_REQUIRE(std::is_same_v<Result, Expected>);

    // rm_void is not the same type as any non-empty ternary node.
    STATIC_REQUIRE_FALSE(std::is_same_v<m::rm_void, Node>);
}

TEST_CASE("rm_void self-closure survives arbitrary nesting", "[root][identity]") {
    // The conceptual identity rm_void == rm<rm_void, rm_void, rm_void> is
    // realized through self-closure: walking into any ternary slot of rm_void
    // yields rm_void again, so the ternary-node view of the origin is
    // rm_void == rm<rm_void, rm_void, rm_void>.
    using Void = m::rm_void;
    STATIC_REQUIRE(std::is_same_v<typename Void::S::S, Void>);
    STATIC_REQUIRE(std::is_same_v<typename Void::R::R::R, Void>);
    STATIC_REQUIRE(std::is_same_v<typename Void::S::R::O::S, Void>);
}

TEST_CASE("partial-empty triads use rm_void in empty slots", "[root][partial]") {
    // Partial-empty forms are legitimate parts of the model. They always use
    // rm_void explicitly in the empty slots, never raw `void`, so every slot
    // remains type-closed under ::S / ::R / ::O.
    using LeftEmpty = m::rm<m::rm_void, m::inherit_r, Mix>;
    STATIC_REQUIRE(std::is_same_v<typename LeftEmpty::S, m::rm_void>);
    STATIC_REQUIRE(std::is_same_v<typename LeftEmpty::R, m::inherit_r>);
    STATIC_REQUIRE(std::is_same_v<typename LeftEmpty::O, Mix>);

    using RightEmpty = m::rm<Subject, m::inherit_r, m::rm_void>;
    STATIC_REQUIRE(std::is_same_v<typename RightEmpty::S, Subject>);
    STATIC_REQUIRE(std::is_same_v<typename RightEmpty::R, m::inherit_r>);
    STATIC_REQUIRE(std::is_same_v<typename RightEmpty::O, m::rm_void>);

    // Partial-empty slots are themselves type-closed: ::S / ::R / ::O of the
    // empty slot keep returning rm_void, so introspection never lands on raw
    // `void`.
    STATIC_REQUIRE(std::is_same_v<typename LeftEmpty::S::R::O, m::rm_void>);
    STATIC_REQUIRE(std::is_same_v<typename RightEmpty::O::S::R, m::rm_void>);
}
