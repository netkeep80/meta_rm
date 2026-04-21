#include <catch2/catch_test_macros.hpp>

#include <meta_rm/meta_rm.hpp>

#include <type_traits>

namespace m = meta_rm;

namespace {

using sym_meta = m::make_meta<"Sym", "Symbol primitive">;
using lit_meta = m::make_meta<"Lit", "Literal primitive">;
using ref_meta = m::make_meta<"Ref", "Reference primitive">;
using target_meta = m::make_meta<"Target", "Reference target">;

using Target = m::make_entity<target_meta>;

} // namespace

TEST_CASE("symbol exposes its meta and no children", "[primitives][symbol]") {
    using Sym = m::symbol<sym_meta>;
    STATIC_REQUIRE(std::is_same_v<typename Sym::meta, sym_meta>);
    STATIC_REQUIRE(std::is_same_v<typename Sym::children, m::type_list<>>);
    STATIC_REQUIRE_FALSE(m::is_rm_v<Sym>);
}

TEST_CASE("literal carries its compile-time value and meta", "[primitives][literal]") {
    using Lit = m::literal<42, lit_meta>;
    STATIC_REQUIRE(Lit::value == 42);
    STATIC_REQUIRE(std::is_same_v<typename Lit::meta, lit_meta>);
    STATIC_REQUIRE(std::is_same_v<typename Lit::children, m::type_list<>>);
}

TEST_CASE("ref records its target in children", "[primitives][ref]") {
    using Ref = m::ref<Target, ref_meta>;
    STATIC_REQUIRE(std::is_same_v<typename Ref::target, Target>);
    STATIC_REQUIRE(std::is_same_v<typename Ref::meta, ref_meta>);
    STATIC_REQUIRE(std::is_same_v<typename Ref::children, m::type_list<Target>>);
}

TEST_CASE("fixed_string preserves characters and size", "[primitives][fixed_string]") {
    constexpr m::fixed_string fs{"abc"};
    STATIC_REQUIRE(fs.value[0] == 'a');
    STATIC_REQUIRE(fs.value[1] == 'b');
    STATIC_REQUIRE(fs.value[2] == 'c');
    STATIC_REQUIRE(fs.size() == 4); // includes trailing NUL
}
