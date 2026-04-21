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

TEST_CASE("root rm<> is self-closed in its three ternary slots", "[root][self-closure]") {
    STATIC_REQUIRE(std::is_same_v<typename m::rm<>::S, m::rm<>>);
    STATIC_REQUIRE(std::is_same_v<typename m::rm<>::R, m::rm<>>);
    STATIC_REQUIRE(std::is_same_v<typename m::rm<>::O, m::rm<>>);
}

TEST_CASE("rm_root alias names the canonical root", "[root][alias]") {
    STATIC_REQUIRE(std::is_same_v<m::rm_root, m::rm<>>);
    STATIC_REQUIRE(std::is_same_v<typename m::rm_root::S, m::rm_root>);
    STATIC_REQUIRE(std::is_same_v<typename m::rm_root::R, m::rm_root>);
    STATIC_REQUIRE(std::is_same_v<typename m::rm_root::O, m::rm_root>);
}

TEST_CASE("root is recognized as an rm node", "[root][is_rm]") {
    STATIC_REQUIRE(m::is_rm_v<m::rm<>>);
    STATIC_REQUIRE(m::is_rm_v<m::rm_root>);
}

TEST_CASE("root introspection exposes rm_node_meta and three self-children", "[root][introspection]") {
    using Meta = typename m::node_meta<m::rm<>>::type;
    STATIC_REQUIRE(std::is_same_v<Meta, m::rm_node_meta>);

    using Children = typename m::node_children<m::rm<>>::type;
    STATIC_REQUIRE(std::is_same_v<Children, m::type_list<m::rm<>, m::rm<>, m::rm<>>>);
}

TEST_CASE("root resolves to itself through ::type<>", "[root][resolve]") {
    STATIC_REQUIRE(std::is_same_v<typename m::rm<>::type<>, m::rm<>>);
    STATIC_REQUIRE(std::is_same_v<m::resolve_t<m::rm<>>, m::rm<>>);
}

TEST_CASE("nested rm<...> constructions still compile alongside the root", "[root][coexistence]") {
    using Node = m::rm<Subject, m::inherit_r, Mix>;
    using Result = Node::type<>;
    using Expected = m::entity<subject_meta, Mix>;
    STATIC_REQUIRE(std::is_same_v<Result, Expected>);

    // The root is not the same type as any non-root ternary node.
    STATIC_REQUIRE_FALSE(std::is_same_v<m::rm<>, Node>);
}

TEST_CASE("root self-closure survives arbitrary nesting", "[root][identity]") {
    // The conceptual identity rm == rm<rm, rm, rm> is realized through
    // self-closure: walking into any ternary slot of the root yields the root
    // again, so the ternary-node view of the origin is rm == rm<rm, rm, rm>.
    using Root = m::rm<>;
    STATIC_REQUIRE(std::is_same_v<typename Root::S::S, Root>);
    STATIC_REQUIRE(std::is_same_v<typename Root::R::R::R, Root>);
    STATIC_REQUIRE(std::is_same_v<typename Root::S::R::O::S, Root>);
}
