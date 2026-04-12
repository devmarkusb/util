namespace {
class C {};

// new C;
[[maybe_unused]] C* c = nullptr; // NOLINT
} // namespace
