const std = @import("std");
extern "c" fn oldMain(argc: usize, argv: [*][:0]u8) usize;

pub fn main() anyerror!void {
    const allocator = std.heap.smp_allocator;
    const argv = try std.process.argsAlloc(allocator);
    defer std.process.argsFree(allocator, argv);

    _ = oldMain(argv.len, argv.ptr);
}
