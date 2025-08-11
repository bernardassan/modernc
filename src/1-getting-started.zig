const c = @import("c");
extern "c" fn getting_started(argc: u32, argv: ?[*]const [:0]const u8) u32;

pub fn main() !void {
    _ = c.printf("Title: ✨ 1.1 Getting Started\n");
    _ = getting_started(0, null);
}
