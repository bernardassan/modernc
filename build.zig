const std = @import("std");
const Build = std.Build;
const CSourceFile = Build.Module.CSourceFile;
const LazyPath = Build.LazyPath;
const Array = std.BoundedArray([]const u8, 128);

pub fn build(b: *std.Build) !void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard release options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall.
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{ .name = "relearn", .root_source_file = .{ .path = "src/main.zig" }, .target = target, .optimize = optimize });
    exe.root_module.addCMacro("_POSIX_C_SOURCE", "200809L");
    const c_sources = try findCfiles(b.allocator, "src/");

    var array = Array.init(0) catch unreachable;
    const cflags = loadCompileFlags("compile_flags.txt", &array);

    exe.root_module.addCSourceFiles(.{ .files = c_sources, .flags = cflags });

    const eh = b.addStaticLibrary(.{ .name = "eh", .target = target, .optimize = optimize });
    eh.root_module.addCMacro("_POSIX_C_SOURCE", "200809L");
    const eh_source = CSourceFile{
        .file = .{ .path = "./deps/eh/eh.c" },
        .flags = cflags,
    };
    eh.root_module.addCSourceFile(eh_source);
    eh.root_module.addIncludePath(.{ .path = "./deps/eh" });
    eh.linkLibC();
    exe.linkLibrary(eh);
    exe.linkLibC();
    b.installArtifact(exe);

    switch (optimize) {
        .ReleaseFast => {
            eh.root_module.addCMacro("_FORTIFY_SOURCE", "3");
            exe.root_module.addCMacro("_FORTIFY_SOURCE", "3");
        },
        else => {},
    }

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    const exe_tests = b.addTest(.{ .root_source_file = .{ .path = "test/test.zig" }, .target = target });
    const snow_source = LazyPath{ .path = "./deps/snow/snow" };
    exe_tests.root_module.addIncludePath(snow_source);
    exe_tests.root_module.addCMacro("_POSIX_C_SOURCE", "200809L");
    exe_tests.root_module.addCMacro("SNOW_ENABLED", "1");
    exe_tests.linkLibrary(eh);
    const c_test_sources = try findCfiles(b.allocator, "test/");
    exe_tests.root_module.addCSourceFiles(.{ .files = c_test_sources, .flags = &.{"-std=c2x"} });

    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&exe_tests.step);
}

fn loadCompileFlags(comptime path: []const u8, array: *Array) []const []const u8 {
    const compile_flags = @embedFile(path);
    var itr = std.mem.splitScalar(u8, compile_flags, '\n');
    while (itr.next()) |line| {
        if (line.len == 0) break;
        array.appendAssumeCapacity(line);
    }
    //use -Werror for compilation only
    array.appendAssumeCapacity("-Werror");
    return array.constSlice();
}

// Search for all C/C++ files in `src` and add them
fn findCfiles(allocator: std.mem.Allocator, comptime parent_directory: []const u8) ![]const []const u8 {
    var dir = try std.fs.cwd().openDir(parent_directory, .{ .iterate = true });

    var walker = try dir.walk(allocator);
    defer walker.deinit();

    var sources = std.ArrayList([]const u8).init(allocator);

    const allowed_exts = [_][]const u8{ ".c", ".cpp", ".cxx", ".c++", ".cc" };
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
