const std = @import("std");
const testing = std.testing;
const c = @import("c");
/// __numberline__:
/// interpret `str` as a sequence of numbers represented with `base`
/// # Returns:
/// a slice of usize's found in `str`
/// # Parameters:
/// `str` is supposed to be a string
/// `base` value from 0 to 36, with the same interpretation as for strtoul
/// `numbs_buf` is a buffer used as the backing storage of the returned slice
fn numberline(str: [:0]const u8, base_: u8, numbs_buf: []usize) []const usize {
    var count: usize = 0;
    var next: *u8 = undefined;
    var str_ = str;
    while (true) : ({
        if (&str_[0] == next) break;
        count += 1;
        str_.ptr = @ptrCast(next);
    }) {
        numbs_buf[count] = c.strtoull(str_, @ptrCast(&next), base_);
    }
    return numbs_buf[0..count];
}

test numberline {
    var buf: [4]usize = undefined;
    for ([_]u8{ 8, 10, 16, 0 }) |base_| {
        switch (base_) {
            // for base 0 the effective base is deduced from the
            // ﬁrst (or ﬁrst two) characters in the string
            8, 0 => {
                const octal = numberline("0789a", base_, &buf);
                try testing.expect(octal[0] == 7);
            },
            10 => {
                const decimal = numberline("0789a", base_, &buf);
                try testing.expectEqual(decimal[0], @as(usize, 789));
            },
            16 => {
                const hex = numberline("0789a", base_, &buf);
                try testing.expectEqual(hex[0], @as(usize, 30874));
            },
            else => unreachable,
        }
    }
    for ([_]u8{ 10, 16 }) |base_| {
        switch (base_) {
            10 => {
                const dec = numberline("07 89a", base_, &buf);
                try testing.expect(dec[0] == 7);
                try testing.expect(dec[1] == 89);
            },
            16 => {
                const dec = numberline("07 89a", base_, &buf);
                try testing.expect(dec[0] == 7);
                try testing.expect(dec[1] == 0x89a);
            },
            else => unreachable,
        }
    }
}

fn fgetline(comptime size: usize, buf: [size]u8, io: u8) bool {
    _ = buf;
    _ = io;
}
fn fprintnumbers(io: u8, comptime fmt: []const u8, num: usize) usize {
    _ = io;
    _ = fmt;
    _ = num;
}
fn ioMain() void {
    const buf: [256]u8 = undefined;
    while (true) {
        if (fgetline(@sizeOf(buf), buf, c.stdin)) {
            var n: isize = undefined;
            const nums = numberline(c.strlen(buf) + 1, buf, &n, 0);
            defer c.free(nums);

            const ret = fprintnumbers(c.stdout, "%#zX", ",\t", n, nums);
            if (ret < 0) return c.EXIT_FAILURE;
        } else {
            if (buf[0]) { // a partial line has been read
                while (true) {
                    const char = c.getc(c.stdin);
                    if (char == c.EOF) return c.EXIT_FAILURE;
                    if (char == '\n') {
                        c.fprintf(c.stderr, "line too long: %s\n", buf);
                        break;
                    }
                }
            } else break; //regular end of input
        }
    }
}
