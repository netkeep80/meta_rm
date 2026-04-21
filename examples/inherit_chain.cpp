#include <meta_rm/meta_rm.hpp>
#include <type_traits>

namespace m = meta_rm;

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

using Result = Tree::type<>;
using Expected = m::entity<subject_meta, Mix1, Mix2, Mix3>;

static_assert(std::is_same_v<Result, Expected>);
static_assert(std::is_same_v<typename Tree::S, m::rm<m::rm<Subject, m::inherit_r, Mix1>, m::inherit_r, Mix2>>);
static_assert(std::is_same_v<typename Tree::R, m::inherit_r>);
static_assert(std::is_same_v<typename Tree::O, Mix3>);

int main() {
    return 0;
}
