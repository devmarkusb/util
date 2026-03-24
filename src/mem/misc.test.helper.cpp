namespace misc_test_helper {
class C {};

namespace {
// new C;
[[maybe_unused]] C* c = nullptr; // NOLINT
} // namespace
} // namespace misc_test_helper
