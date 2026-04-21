#include <meta_rm/meta_rm.hpp>
#include <type_traits>

namespace m = meta_rm;

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
using InheritResult = InheritNode::type<Extra>;
using InheritExpected = m::entity<subject_meta, Base, Extra>;
static_assert(std::is_same_v<InheritResult, InheritExpected>);

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

static_assert(std::is_same_v<IfTrue::type<>, Then>);
static_assert(std::is_same_v<IfFalse::type<>, Else>);
static_assert(m::is_rm_v<InheritNode>);
static_assert(!m::is_rm_v<Subject>);

using Meta = typename m::node_meta<InheritNode>::type;
static_assert(Meta::name.value[0] == 'r');

using Children = typename m::node_children<InheritNode>::type;
static_assert(std::is_same_v<Children, m::type_list<Subject, m::inherit_r, Base>>);

int main() {
    return 0;
}
