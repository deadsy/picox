#!/bin/env python3

# ------------------------------------------------------------------------------
"""

Pico Build Tool

Setup an SDK based build environment for RP2040 and RP2350 based boards.

Format: black -l120 pbt

"""
# ------------------------------------------------------------------------------

import sys
import datetime
import os
import subprocess
import shutil

# ------------------------------------------------------------------------------

debug = False

# ------------------------------------------------------------------------------

# supported machine set (match the machine names in the pico sdk)
machines = {
    "pico": "Raspberry Pi Pico (rp2040)",
    "pico2": "Raspberry Pi Pico2 (rp2350)",
    "pimoroni_pico_plus2_rp2350": "Pimoroni Pico+2 (rp2350)",
}


def check_machine(m):
    """check if this machine is supported"""
    if m in machines:
        return m
    return None


# ------------------------------------------------------------------------------
# print messages


def pr_error(msg, usage=None):
    """log an error message and exit 1"""
    ts = datetime.datetime.utcnow()
    print(f"{ts} error: {msg}")
    if usage:
        usage(sys.argv)
    sys.exit(1)


def pr_info(msg):
    """log an informational message"""
    ts = datetime.datetime.utcnow()
    print(f"{ts} info: {msg}")


def pr_debug(msg):
    """log a debug message"""
    if debug:
        ts = datetime.datetime.utcnow()
        print(f"{ts} debug: {msg}")


# ------------------------------------------------------------------------------


def check_dir(name):
    """check for the existence of a directory"""
    return os.path.isdir(name)


def check_file(name):
    """check for the existence of a file"""
    return os.path.isfile(name)


def remove_dir(name):
    """remove a directory and all files within"""
    if not os.path.isdir(name):
        return
    pr_info(f"removing {name}")
    shutil.rmtree(name)


# ------------------------------------------------------------------------------


def exec_cmd(cmd):
    """execute a command, return the output and return code"""
    pr_debug(cmd)
    output = b""
    rc = 0
    try:
        output = subprocess.check_output(cmd, shell=True)
    except subprocess.CalledProcessError as e:
        rc = e.returncode
    pr_debug(f"returned {rc}")
    return output.decode("utf-8"), rc


# ------------------------------------------------------------------------------


class package:
    """tarball (tgz) package handler"""

    def __init__(self, dst, tar, csum, url):
        top = os.getcwd()
        self.dst = os.path.join(top, dst)  # destination directory of extracted package
        self.tar = os.path.join(top, "data", "downloads", tar)  # path for package tarball
        self.csum = csum  # sha256 checksum of package tarball
        self.url = url  # source url of package

    def exists(self):
        """return true if the package destination directory exists"""
        return os.path.isdir(self.dst)

    def get(self):
        """get the package tarball"""
        # create the download directory if it does not exist
        dl_dir = os.path.dirname(self.tar)
        if not check_dir(dl_dir):
            os.makedirs(dl_dir)
        pr_info(f"downloading {self.url} to {self.tar}")
        cmd = f'wget -qP {dl_dir} -O {self.tar} "{self.url}"'
        _, rc = exec_cmd(cmd)
        if rc != 0:
            return f"can't download {self.url}"
        return None

    def extract(self):
        """extract the package tarball"""
        if not self.exists():
            os.makedirs(self.dst)
        cmd = f"tar -C {self.dst} -xf {self.tar} --strip-components 1"
        _, rc = exec_cmd(cmd)
        if rc != 0:
            return f"can't extract {self.tar}"
        pr_info(f"extracted {self.tar} to {self.dst}")
        return None

    def check(self):
        """validate the package checksum"""
        if not check_file(self.tar):
            return f"{self.tar} does not exist"
        cmd = f"echo {self.csum} {self.tar} | sha256sum --check --strict"
        _, rc = exec_cmd(cmd)
        if rc != 0:
            return f"bad checksum for {self.tar}"
        return None

    def clean(self):
        """remove the extracted package files"""
        remove_dir(self.dst)

    def init(self):
        """initialize a package"""
        # do we already have the extracted package files
        if check_dir(self.dst):
            # consider the package files as present and correct
            return None
        # do we have the tarball?
        if not check_file(self.tar):
            err = self.get()
            if err is not None:
                return err
        # we have the tarball, check it
        err = self.check()
        if err is not None:
            return err
        # extract the tarball
        err = self.extract()
        if err is not None:
            return err
        return None


# ------------------------------------------------------------------------------

packages = {}

# pico sdk

sdk_ver = "2.0.0"
packages["sdk"] = package(
    "sdk",
    "pico-sdk-" + sdk_ver + ".tar.gz",
    "626db87779fa37f7f9c7cfe3e152f7e828fe19c486730af2e7c80836b6b57e1d",
    "https://github.com/raspberrypi/pico-sdk/releases/download/" + sdk_ver + "/pico-sdk-" + sdk_ver + ".tar.gz",
)


# ------------------------------------------------------------------------------


def cmd_clean_usage(argv):
    """usage for clean command"""
    print(f"usage: {argv[0]} clean")


def cmd_clean(argv):
    """clean the build environment"""

    if len(argv) != 2:
        pr_error("bad argument", cmd_clean_usage)
        sys.exit(1)

    # remove extracted packages
    for _, pkg in packages.items():
        pkg.clean()

    # remove the build directory
    top = os.getcwd()
    build_dir = os.path.join(top, "build")
    remove_dir(build_dir)


# ------------------------------------------------------------------------------


def cmd_init_usage(argv):
    """usage for init command"""
    print(f"usage: {argv[0]} init <machine>")
    print("Machines:")
    for k, v in machines.items():
        print(f"{k:30}{v}")


def cmd_init(argv):
    """initialize a build environment"""

    if len(argv) != 3:
        pr_error("bad argument", cmd_init_usage)
        sys.exit(1)

    if argv[2] == "help":
        cmd_init_usage(argv)
        sys.exit(0)

    machine = check_machine(argv[2])
    if machine is None:
        pr_error(f'unrecognized machine "{argv[2]}"', cmd_init_usage)

    # get the sdk files
    packages["sdk"].init()

    # build directory
    top = os.getcwd()
    build_dir = os.path.join(top, "build", machine)
    if not check_dir(build_dir):
        pr_info(f"mkdir {build_dir}")
        os.makedirs(build_dir)

    # <build_dir>/Makefile
    makefile = os.path.join(build_dir, "Makefile")
    bld_path = os.path.join(build_dir, "build")
    src_path = os.path.join(top, "src")
    with open(makefile, "w", encoding="utf-8") as f:
        f.write(f"SDK_PATH = {sdk_path}\n")
        f.write(f"BLD_PATH = {bld_path}\n")
        f.write(f"SRC_PATH = {src_path}\n\n")
        f.write(".PHONY: all\n")
        f.write("all:\n")
        f.write(f"\tPICO_SDK_PATH=$(SDK_PATH) cmake -GNinja -S $(SRC_PATH) -B $(BLD_PATH) -DPICO_BOARD={machine}\n")
        f.write("\tninja -C $(BLD_PATH)\n\n")
        f.write(".PHONY: clean\n")
        f.write("clean:\n")
        f.write("\t-rm -rf $(BLD_PATH)\n")
    pr_info(f"created {makefile}")


# ------------------------------------------------------------------------------


def cmd_help(argv):
    """display command help"""
    pr_usage(argv)


# ------------------------------------------------------------------------------


commands = {
    "clean": cmd_clean,
    "init": cmd_init,
    "help": cmd_help,
}


def pr_usage(args):
    """print a usage message"""
    print(f"usage: {args[0]} [command] <args>")
    print("Commands:")
    for k, v in commands.items():
        print(f"{k:20}{v.__doc__}")


def main():
    """Pico Build Tool"""

    # check arguments
    if len(sys.argv) <= 1:
        pr_error("missing command", pr_usage)

    # run the command
    cmd = sys.argv[1]
    if cmd in commands:
        commands[cmd](sys.argv)
    else:
        pr_error(f"unrecognized command {cmd}", pr_usage)

    sys.exit(0)


main()


# ------------------------------------------------------------------------------
