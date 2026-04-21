#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace meta_rm {

// -----------------------------------------------------------------------------
// fixed_string
// -----------------------------------------------------------------------------

template <std::size_t N>
struct fixed_string {
    char value[N]{};

    constexpr fixed_string(const char (&s)[N]) {
        for (std::size_t i = 0; i < N; ++i) {
            value[i] = s[i];
        }
    }

    [[nodiscard]] constexpr const char* c_str() const noexcept { return value; }
    [[nodiscard]] constexpr std::size_t size() const noexcept { return N; }
};

template <std::size_t N>
fixed_string(const char (&)[N]) -> fixed_string<N>;

// -----------------------------------------------------------------------------
// metadata
// -----------------------------------------------------------------------------

template <fixed_string Name_, fixed_string Description_>
struct meta_info {
    static constexpr auto name = Name_;
    static constexpr auto description = Description_;
};

using rm_node_meta = meta_info<
    "rm",
    "Relation model ternary node: Subject-Relation-Object"
>;

// -----------------------------------------------------------------------------
// primitive type utilities
// -----------------------------------------------------------------------------

template <typename... Ts>
struct type_list {};

template <typename T>
struct identity {
    using type = T;
};

// -----------------------------------------------------------------------------
// primitive entities
// -----------------------------------------------------------------------------

template <typename Meta>
struct symbol {
    using meta = Meta;
    using children = type_list<>;
};

template <auto V, typename Meta>
struct literal {
    static constexpr auto value = V;
    using meta = Meta;
    using children = type_list<>;
};

template <typename Target, typename Meta>
struct ref {
    using target = Target;
    using meta = Meta;
    using children = type_list<Target>;
};

// A minimal variadic entity that can accumulate heirs/bases/payload types.
template <typename Meta, typename... Heirs>
struct entity {
    using meta = Meta;
    using heirs = type_list<Heirs...>;
    using children = type_list<Heirs...>;

    template <typename... Extra>
    using rebind = entity<Meta, Heirs..., Extra...>;
};

// -----------------------------------------------------------------------------
// relation base
// -----------------------------------------------------------------------------

template <typename Meta>
struct relation {
    using meta = Meta;
    using children = type_list<>;

    template <typename Subject, typename Object, typename... Extra>
    struct apply;
};

// -----------------------------------------------------------------------------
// forward declarations for relation model
// -----------------------------------------------------------------------------

template <typename S_, typename R_, typename O_>
struct rm;

template <typename T>
struct is_rm : std::false_type {};

template <typename S, typename R, typename O>
struct is_rm<rm<S, R, O>> : std::true_type {};

template <typename T>
inline constexpr bool is_rm_v = is_rm<T>::value;

// Resolve nested rm-nodes to their computed type; leave ordinary types unchanged.
template <typename T, typename... Extra>
struct resolve {
    using type = T;
};

template <typename S, typename R, typename O, typename... Extra>
struct resolve<rm<S, R, O>, Extra...> {
    using type = typename rm<S, R, O>::template type<Extra...>;
};

template <typename T, typename... Extra>
using resolve_t = typename resolve<T, Extra...>::type;

// -----------------------------------------------------------------------------
// relation model node
// -----------------------------------------------------------------------------

template <typename S_, typename R_, typename O_>
struct rm {
    using S = S_;
    using R = R_;
    using O = O_;

    using meta = rm_node_meta;
    using children = type_list<S, R, O>;

    // The result of every rm-node is intentionally exposed as a variadic template.
    // What Extra... means is relation-specific and can evolve with the language.
    template <typename... Extra>
    using type = typename R::template apply<resolve_t<S>, resolve_t<O>, Extra...>::type;
};

// -----------------------------------------------------------------------------
// introspection
// -----------------------------------------------------------------------------

template <typename T>
struct node_meta {
    using type = typename T::meta;
};

template <typename T>
struct node_children {
    using type = typename T::children;
};

// -----------------------------------------------------------------------------
// example relation: inherit
// -----------------------------------------------------------------------------

using inherit_meta = meta_info<
    "inherit",
    "Attach Object to Subject and return the resulting variadic subject type"
>;

struct inherit_r : relation<inherit_meta> {
    template <typename Subject, typename Object, typename... Extra>
    struct apply;
};

template <typename SubjectMeta, typename... Existing, typename Object, typename... Extra>
struct inherit_r::apply<entity<SubjectMeta, Existing...>, Object, Extra...> {
    using type = entity<SubjectMeta, Existing..., Object, Extra...>;
};

// -----------------------------------------------------------------------------
// simple branching support for future if-relations
// -----------------------------------------------------------------------------

using bool_true_meta = meta_info<"true", "Compile-time true value">;
using bool_false_meta = meta_info<"false", "Compile-time false value">;

using true_c = literal<true, bool_true_meta>;
using false_c = literal<false, bool_false_meta>;

template <typename Cond, typename Then, typename Else, typename Meta>
struct branch {
    using condition = Cond;
    using then_type = Then;
    using else_type = Else;
    using meta = Meta;
    using children = type_list<Cond, Then, Else>;
};

using if_meta = meta_info<
    "if",
    "Choose between branch::then_type and branch::else_type"
>;

struct if_r : relation<if_meta> {
    template <typename Subject, typename Object, typename... Extra>
    struct apply;
};

template <typename Subject, typename Then, typename Else, typename BranchMeta, typename... Extra>
struct if_r::apply<Subject, branch<true_c, Then, Else, BranchMeta>, Extra...> {
    using type = resolve_t<Then, Extra...>;
};

template <typename Subject, typename Then, typename Else, typename BranchMeta, typename... Extra>
struct if_r::apply<Subject, branch<false_c, Then, Else, BranchMeta>, Extra...> {
    using type = resolve_t<Else, Extra...>;
};

// -----------------------------------------------------------------------------
// convenience aliases for examples and future layers
// -----------------------------------------------------------------------------

template <fixed_string Name_, fixed_string Description_>
using make_meta = meta_info<Name_, Description_>;

template <typename Meta>
using make_symbol = symbol<Meta>;

template <typename Meta, typename... Heirs>
using make_entity = entity<Meta, Heirs...>;

} // namespace meta_rm
