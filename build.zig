const std = @import("std");
const Build = std.Build;
const LazyPath = Build.LazyPath;
const Query = std.Target.Query;

const CompileFlags = std.ArrayListUnmanaged([]const u8);

const max_compile_flags = 24;

//TODO: Implement using gcc and or clang when they are available since they offer sanitizer options and static analysis
pub fn build(b: *std.Build) !void {
    //FIX: error: 'stdbit.h' file not found
    // Musl doesn't implement C23 stdbit.h and gnu 2.39 isn't currently part of zig
    //NOTE: https://github.com/ziglang/zig/issues/19590
    // const target = b.standardTargetOptions(.{
    //     .default_target = .{
    //         .cpu_arch = .x86_64,
    //         .cpu_model = .native,
    //         .os_tag = .linux,
    //         .abi = .musl,
    //     },
    // });
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const c_translate = b.addTranslateC(.{
        .root_source_file = b.path("src/c.h"),
        .optimize = optimize,
        .target = target,
        .use_clang = true,
    });

    var compile_txt_buf: [max_compile_flags][]const u8 = undefined;
    var array = CompileFlags.initBuffer(&compile_txt_buf);

    const cflags = loadCompileFlags("compile_flags.txt", &array);

    const src = "src/";
    const c_sources = try findCfiles(b, src);

    for (c_sources) |source| {
        const name = std.fs.path.stem(source);
        const mod = b.createModule(.{
            .root_source_file = b.path(b.fmt("{s}/{s}.zig", .{ src, name })),
            .link_libc = true,
            .sanitize_c = .full,
            .stack_check = true,
            .target = target,
            .optimize = optimize,
        });
        switch (optimize) {
            // TODO: set release options
            .ReleaseFast => {
                mod.addCMacro("_FORTIFY_SOURCE", "3");
            },
            // TODO: enable setting debug options for easy debugin
            else => {},
        }
        mod.addImport("c", c_translate.createModule());
        mod.addCSourceFile(.{ .file = b.path(source), .flags = cflags });

        const exe = b.addExecutable(.{
            .name = name,
            .root_module = mod,
            .use_lld = false,
            .use_llvm = false,
        });
        const run_cmd = b.addRunArtifact(exe);
        const run_step = b.step(name, b.fmt("Run the {s}", .{name}));
        run_step.dependOn(&run_cmd.step);
    }
}

// Search for all C files in `src` and add them
fn findCfiles(b: *Build, src: []const u8) ![]const []const u8 {
    var dir = try std.fs.cwd().openDir(src, .{ .iterate = true });

    var walker = try dir.walk(b.allocator);
    defer walker.deinit();

    var sources = std.ArrayList([]const u8).init(b.allocator);

    const allowed_exts = [_][]const u8{".c"};
    while (try walker.next()) |entry| {
        const ext = std.fs.path.extension(entry.basename);
        const include_file = for (allowed_exts) |e| {
            if (std.mem.eql(u8, ext, e))
                break true;
        } else false;
        if (include_file) {
            // we have to clone the path as walker.next() or walker.deinit() will override/kill it
            try sources.append(b.fmt("{[src]s}/{[path]s}", .{ .src = src, .path = entry.path }));
        }
    }

    std.mem.sortUnstable([]const u8, sources.items, {}, struct {
        pub fn lessThanFn(context: void, lhs: []const u8, rhs: []const u8) bool {
            _ = context;
            return std.mem.lessThan(u8, lhs, rhs);
        }
    }.lessThanFn);

    return sources.items;
}

fn loadCompileFlags(comptime path: []const u8, array: *CompileFlags) []const []const u8 {
    //use -Werror for compilation only
    array.appendAssumeCapacity("-Werror");

    const compile_flags = @embedFile(path);
    var itr = std.mem.splitScalar(u8, compile_flags, '\n');
    while (itr.next()) |line| {
        if (line.len == 0) break; // End of Stream
        if (line[0] == '#') continue; // A comment
        array.appendAssumeCapacity(line);
    }
    return array.items;
}
