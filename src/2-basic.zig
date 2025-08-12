const c = @import("c");
extern "c" fn cMain() void;

pub fn main() !void {
    _ = c.printf("Title: ✨ Chapter 5: Basic Value and Data\n");
    cMain();
}
