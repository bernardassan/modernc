const std = @import("std");
const Build = std.Build;
const CSourceFile = Build.Module.CSourceFile;
const LazyPath = Build.LazyPath;
const Query = std.Target.Query;
const Array = std.ArrayListUnmanaged([]const u8);

const Translator = @import("translate_c").Translator;

var compile_txt_buf: [128][]const u8 = undefined;
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
    const translate_c = b.dependency("translate_c", .{
        .target = target,
        .optimize = optimize,
    });

    const c_translate: Translator = .init(translate_c, .{
        .c_source_file = b.path("src/c.h"),
        .target = target,
        .optimize = optimize,
    });
    const mod = b.createModule(.{
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    mod.addImport("c", c_translate.mod);
    mod.addCMacro("_XOPEN_SOURCE", "700");
    const c_sources = try findCfiles(b.allocator, "src/");

    var array = Array.initBuffer(&compile_txt_buf);
    const cflags = loadCompileFlags("compile_flags.txt", &array);

    mod.addCSourceFiles(.{ .files = c_sources, .flags = cflags });

    const exe = b.addExecutable(.{
        .name = "relearn",
        .root_module = mod,
        .use_lld = false,
        .use_llvm = false,
    });
    b.installArtifact(exe);

    // NOTE: https://man7.org/linux/man-pages/man7/feature_test_macros.7.html
    // https://stackoverflow.com/questions/5378778/what-does-d-xopen-source-do-mean
    // https://pubs.opengroup.org/onlinepubs/9699919799/
    switch (optimize) {
        // TODO: set release options
        .ReleaseFast => {
            exe.root_module.addCMacro("_FORTIFY_SOURCE", "3");
        },
        // TODO: enable setting debug options for easy debugin
        else => {},
    }

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}

fn loadCompileFlags(comptime path: []const u8, array: *Array) []const []const u8 {
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

// Search for all C files in `src` and add them
fn findCfiles(allocator: std.mem.Allocator, comptime parent_directory: []const u8) ![]const []const u8 {
    var dir = try std.fs.cwd().openDir(parent_directory, .{ .iterate = true });

    var walker = try dir.walk(allocator);
    defer walker.deinit();

    var sources = std.ArrayList([]const u8).init(allocator);

    const allowed_exts = [_][]const u8{".c"};
    while (try walker.next()) |entry| {
        const ext = std.fs.path.extension(entry.basename);
        const include_file = for (allowed_exts) |e| {
            if (std.mem.eql(u8, ext, e))
                break true;
        } else false;
        if (include_file) {
            // we have to clone the path as walker.next() or walker.deinit() will override/kill it
            try sources.append(try std.fmt.allocPrint(allocator, parent_directory ++ "/{s}", .{entry.path}));
        }
    }
    return sources.items;
}
