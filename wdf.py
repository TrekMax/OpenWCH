#!/usr/bin/env python3
"""
WCH build helper.
Usage (from any example directory):
    python /path/to/wdf.py build [--config Debug|Release] [-S .]
Requires toolchain env (source export_env.sh).
"""
from __future__ import annotations

import argparse
import os
import subprocess
import sys
from pathlib import Path


def run(cmd: list[str], cwd: Path) -> None:
    print(f"[wdf] {' '.join(cmd)} (cwd={cwd})")
    subprocess.run(cmd, cwd=cwd, check=True)


def copy_compile_commands(build_dir: Path, sdk_root: Path) -> None:
    src = build_dir / "compile_commands.json"
    if not src.exists():
        return
    dst_dir = sdk_root / "build"
    dst_dir.mkdir(parents=True, exist_ok=True)
    dst = dst_dir / "compile_commands.json"
    dst.write_bytes(src.read_bytes())


def generate_kconfig(sdk_root: Path, build_dir: Path, prj_conf: Path | None) -> Path:
    try:
        from kconfiglib import Kconfig  # type: ignore
    except Exception as exc:
        raise SystemExit(
            "kconfiglib not available. Install with: pip install kconfiglib"
        ) from exc

    kconfig = sdk_root / "Kconfig"
    if not kconfig.exists():
        raise SystemExit(f"Kconfig not found: {kconfig}")

    os.environ.setdefault("srctree", str(sdk_root))
    kconf = Kconfig(str(kconfig))

    # Load defaults first, then overlay prj.conf if present.
    kconf.load_config()
    if prj_conf and prj_conf.exists():
        kconf.load_config(str(prj_conf), replace=False)

    build_dir.mkdir(parents=True, exist_ok=True)
    out_config = build_dir / ".config"
    os.environ["KCONFIG_CONFIG"] = str(out_config)
    kconf.write_config(str(out_config))
    return out_config


def do_build(args: argparse.Namespace) -> None:
    sdk_root = Path(__file__).resolve().parent
    example_dir = Path(args.example).resolve()
    if not (example_dir / "CMakeLists.txt").exists():
        raise SystemExit(f"example path '{example_dir}' missing CMakeLists.txt")

    cwd = Path.cwd().resolve()
    if example_dir != cwd:
        build_dir = cwd / "build"
    else:
        build_dir = example_dir / "build"
    build_dir.mkdir(parents=True, exist_ok=True)

    toolchain = sdk_root / "cmake" / "toolchain-riscv-none-elf.cmake"
    if not toolchain.exists():
        raise SystemExit(f"toolchain file not found: {toolchain}")

    if args.config:
        prj_conf = Path(args.config).expanduser().resolve()
    else:
        prj_conf = example_dir / "prj.conf"
    out_config = generate_kconfig(sdk_root, build_dir, prj_conf if prj_conf.exists() else None)

    configure_cmd = [
        args.cmake,
        "-S",
        str(example_dir),
        "-B",
        str(build_dir),
        f"-DOPENWCH_SDK_DIR={sdk_root}",
        f"-DCMAKE_BUILD_TYPE={args.type}",
        f"-DCMAKE_TOOLCHAIN_FILE={toolchain}",
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
    ]
    if out_config is not None:
        configure_cmd.append(f"-DOPENWCH_CONFIG_FILE={out_config}")
    build_cmd = [args.cmake, "--build", str(build_dir)]

    run(configure_cmd, cwd=example_dir)
    run(build_cmd, cwd=example_dir)
    copy_compile_commands(build_dir, sdk_root)
    
def do_flash(args: argparse.Namespace) -> None:
    target_bin = Path(args.binary).resolve()
    if not target_bin.exists():
        raise SystemExit(f"built binary not found: {target_bin}")
    flash_cmd = [
        "wlink",
        "flash",
        str(target_bin),
    ]
    run(flash_cmd, cwd=target_bin.parent)


def do_menuconfig(args: argparse.Namespace) -> None:
    try:
        from kconfiglib import Kconfig  # type: ignore
    except Exception as exc:
        raise SystemExit(
            "kconfiglib not available. Install with: pip install kconfiglib"
        ) from exc

    try:
        import menuconfig as kconfig_menu  # type: ignore
    except Exception:
        try:
            from kconfiglib import menuconfig as kconfig_menu  # type: ignore
        except Exception as exc:
            raise SystemExit(
                "menuconfig not available. Install with: pip install kconfiglib"
            ) from exc

    sdk_root = Path(__file__).resolve().parent
    example_dir = Path(args.example).resolve()
    if not (example_dir / "CMakeLists.txt").exists():
        raise SystemExit(f"example path '{example_dir}' missing CMakeLists.txt")

    if args.config:
        config_path = Path(args.config).expanduser().resolve()
        build_dir = config_path.parent
    else:
        build_dir = (Path.cwd().resolve() / "build") if example_dir != Path.cwd().resolve() else (example_dir / "build")
        config_path = build_dir / ".config"

    kconfig = sdk_root / "Kconfig"
    if not kconfig.exists():
        raise SystemExit(f"Kconfig not found: {kconfig}")

    os.environ.setdefault("srctree", str(sdk_root))
    os.environ["KCONFIG_CONFIG"] = str(config_path)
    kconf = Kconfig(str(kconfig))

    if config_path.exists():
        kconf.load_config(str(config_path))
    else:
        kconf.load_config()

    import tempfile

    with tempfile.NamedTemporaryFile("w+", delete=False, encoding="utf-8") as tmp:
        before_path = Path(tmp.name)
    kconf.write_config(str(before_path))
    before_text = before_path.read_text(encoding="utf-8", errors="ignore")
    before_path.unlink(missing_ok=True)

    try:
        build_dir.mkdir(parents=True, exist_ok=True)
        old_cwd = Path.cwd()
        os.chdir(build_dir)
        try:
            if hasattr(kconfig_menu, "menuconfig"):
                kconfig_menu.menuconfig(kconf)
            elif hasattr(kconfig_menu, "main"):
                kconfig_menu.main()
            else:
                raise SystemExit("menuconfig entrypoint not found in kconfiglib")
        finally:
            os.chdir(old_cwd)
    except KeyboardInterrupt:
        print("[wdf] menuconfig interrupted; not writing prj.conf")
        return

    config_path.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.NamedTemporaryFile("w+", delete=False, encoding="utf-8") as tmp:
        tmp_path = Path(tmp.name)
    kconf.write_config(str(tmp_path))
    new_text = tmp_path.read_text(encoding="utf-8", errors="ignore")
    tmp_path.unlink(missing_ok=True)

    if new_text == before_text:
        print("[wdf] Config unchanged; not writing prj.conf")
        return

    config_path.write_text(new_text, encoding="utf-8")
    print(f"[wdf] Saved config: {config_path}")


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(prog="wdf.py")
    sub = parser.add_subparsers(dest="command", required=True)

    p_build = sub.add_parser("build", help="configure and build an example")
    p_build.add_argument("-S", "--example", default=".", help="example path (default: cwd)")
    p_build.add_argument("--type", default="Debug", choices=["Debug", "Release"], help="CMAKE_BUILD_TYPE")
    p_build.add_argument("--config", help="path to prj.conf (default: example/prj.conf)")
    p_build.add_argument("--cmake", default="cmake", help="cmake executable")

    p_flash = sub.add_parser("flash", help="flash built example to device")
    p_flash.add_argument("binary", help="path to .bin or .hex file to flash")

    p_menuconfig = sub.add_parser("menuconfig", help="open Kconfig menuconfig UI")
    p_menuconfig.add_argument("-S", "--example", default=".", help="example path (default: cwd)")
    p_menuconfig.add_argument("--config", help="path to .config (default: <cwd>/build/.config)")

    args = parser.parse_args(argv)

    if args.command == "build":
        do_build(args)
    elif args.command == "flash":
        do_flash(args)
    elif args.command == "menuconfig":
        do_menuconfig(args)
    else:
        parser.error("unknown command")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
