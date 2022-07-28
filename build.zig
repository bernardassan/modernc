const std = @import("std");

pub fn build(b: *std.build.Builder) !void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard release options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall.
    const mode = b.standardReleaseOptions();

    const cflags = [_][]const u8{
        "-std=c2x",                                 "-Wall",                            "-Wextra",
        "-Wpedantic",                               "-Walloca",                         "-Warray-bounds-pointer-arithmetic",
        "-Wassign-enum",                            "-Watomic-implicit-seq-cst",        "-Watomic-properties",
        "-Wbad-function-cast",                      "-Wcast-align",                     "-Wcast-function-type",
        "-Wcast-qual",                              "-Wcomma",                          "-Wconditional-uninitialized",
        "-Wconversion",                             "-Wduplicate-enum",                 "-Wfloat-equal",
        "-Wgnu",                                    "-Wnullable-to-nonnull-conversion", "-Wloop-analysis",
        "-Wimplicit-fallthrough",                   "-Wmethod-signatures",              "-Wmicrosoft",
        "-Wmissing-noreturn",                       "-Wmissing-prototypes",             "-Wmissing-variable-declarations",
        "-Wover-aligned",                           "-Wpadded",                         "-Wpacked",
        "-Wpointer-arith",                          "-Wreserved-identifier",            "-Wshift-sign-overflow",
        "-Wsigned-enum-bitfield",                   "-Wswitch-enum",                    "-Wtautological-compare",
        "-Wtautological-constant-in-range-compare", "-Wthread-safety",                  "-Wthread-safety-beta",
        "-Wthread-safety-negative",                 "-Wthread-safety-verbose",          "-Wunaligned-access",
        "-Wundef",                                  "-Wunreachable-code-aggressive",    "-Wunused-macros",
        "-Wused-but-marked-unused",                 "-Wvla",                            "-Wzero-as-null-pointer-constant",
        "-Wshadow-all",                             "-Werror",
    };

    const exe = b.addExecutable("relearn", "src/main.zig");
    var sources = std.ArrayList([]const u8).init(b.allocator);

    // Search for all C/C++ files in `src` and add them
    {
        var dir = try std.fs.cwd().openIterableDir("src", .{});

        var walker = try dir.walk(b.allocator);
        defer walker.deinit();

        const allowed_exts = [_][]const u8{ ".c", ".cpp", ".cxx", ".c++", ".cc" };
        while (try walker.next()) |entry| {
            const ext = std.fs.path.extension(entry.basename);
            const include_file = for (allowed_exts) |e| {
                if (std.mem.eql(u8, ext, e))
                    break true;
            } else false;
            if (include_file) {
                // we have to clone the path as walker.next() or walker.deinit() will override/kill it
                try sources.append(try std.fmt.allocPrint(b.allocator, "src/{s}", .{entry.path}));
            }
        }
    }

    exe.addCSourceFiles(sources.items, &cflags);
    exe.setTarget(target);
    exe.setBuildMode(mode);
    exe.linkLibC();
    exe.install();

    const run_cmd = exe.run();
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    const exe_tests = b.addTest("src/main.zig");
    exe_tests.setTarget(target);
    exe_tests.setBuildMode(mode);

    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&exe_tests.step);
}
