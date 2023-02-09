namespace misc_test_helper
{
class C
{
};

namespace
{
[[maybe_unused]] C* c = nullptr; // new C;
} // namespace
} // namespace misc_test_helper
