const std = @import("std");

extern "c" fn oldMain(argc: usize, argv: [*][:0]u8) usize;

pub fn main() anyerror!void {
    const allocator = std.heap.c_allocator;
    const argv = try std.process.argsAlloc(allocator);
    defer std.process.argsFree(allocator, argv);

    _ = oldMain(argv.len, argv.ptr);
}

test "basic test" {
    try std.testing.expectEqual(10, 3 + 7);
}
