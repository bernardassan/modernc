const c = @import("c");
extern "c" fn cMain(argc: u32, argv: ?[*]const [:0]const u8) u32;

pub fn main() !void {
    _ = c.printf("Title: ✨ Chapter 1: Getting Started\n");
    _ = cMain(0, null);
}
