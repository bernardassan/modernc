const std = @import("std");
const Build = std.Build;
const CSourceFile = Build.Module.CSourceFile;
const LazyPath = Build.LazyPath;

const debug_mode = [_][]const u8{ "-g", "-fdebug-macro" };

// TODO: load flags from compile_flags
const cflags = [_][]const u8{
    "-std=c2x", "-Weverything", "-Werror", "-Wall", "-Wextra", //
    "-Wpedantic", "-Wno-declaration-after-statement", //
    "-pedantic-errors", "-Wno-pre-c2x-compat", //
};

const deps = struct {
    const snow = LazyPath{ .path = "./deps/snow/snow.h" };

    const eh = CSourceFile{
        .file = .{ .path = "./deps/eh/eh.c" },
        .flags = &cflags,
    };
};

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
    exe.root_module.addCSourceFiles(.{ .files = c_sources, .flags = &cflags });

    const eh = b.addStaticLibrary(.{ .name = "eh", .target = target, .optimize = optimize });
    eh.root_module.addCMacro("_POSIX_C_SOURCE", "200809L");
    eh.root_module.addCSourceFile(deps.eh);
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
    exe_tests.addIncludePath(deps.snow);
    exe_tests.linkLibrary(eh);
    const c_test_sources = try findCfiles(b.allocator, "test/");
    exe_tests.addCSourceFiles(.{ .files = c_test_sources, .flags = &.{} });

    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&exe_tests.step);
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
