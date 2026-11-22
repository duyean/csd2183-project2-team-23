# Conversation: `make bench` fails with read-only /tmp on WSL

## User

```
make bench
mkdir -p build
g++ -std=c++23 -O2 -DNDEBUG -Iinclude -Ibench/nanobench -c bench/nanobench/nanobench.cpp -o build/nanobench.o
Cannot create temporary file in /tmp/: Read-only file system
make: *** [Makefile:79: build/nanobench.o] Aborted (core dumped)
```

i got this error when running make bench what could be the problem? i am guessing it has to do with access i am running this command on WSL venv

## Claude

The error comes from g++ itself, not from your Makefile. The compiler writes intermediate files (preprocessed output, assembly, etc.) to a temp directory, and it's finding `/tmp/` mounted read-only. Your guess about WSL is on the right track, though a venv is unlikely to be the cause, since venvs don't touch `/tmp`.

### Quick fix: point the compiler at a writable temp dir

```bash
mkdir -p ~/tmp
export TMPDIR=$HOME/tmp
make bench
```

GCC respects `TMPDIR`, so this should get you past the error immediately. To make it permanent, add `export TMPDIR=$HOME/tmp` to your `~/.bashrc`.

### Check what's going on with /tmp

```bash
mount | grep -E ' /tmp | / '
touch /tmp/testfile && echo "writable" || echo "read-only"
df -h /tmp
```

Common causes on WSL:

1. **WSL filesystem went read-only.** This happens after a disk error, a crashed/forced shutdown, or running out of disk space. Run `wsl --shutdown` from PowerShell (not inside WSL), then reopen your distro. That often clears it.
2. **Disk full.** Check `df -h`. A full virtual disk can cause the kernel to remount read-only.
3. **Sandboxed shell.** If you're running inside an IDE terminal, container, or restricted environment (some AI coding tools, Docker with `--read-only`, or a hardened systemd service), `/tmp` can be intentionally read-only. In that case the `TMPDIR` workaround above is the right fix.
4. **Custom /tmp mount.** Check `/etc/fstab` and `/etc/wsl.conf` for a `/tmp` entry with the `ro` option.

### One more thing to check

The error says "Aborted (core dumped)", which means g++ crashed rather than failing gracefully. That's consistent with the temp file problem, but if `TMPDIR` doesn't fix it, try `g++ -pipe` by adding `-pipe` to your CXXFLAGS, which makes the compiler use pipes instead of temp files:

```bash
make bench CXXFLAGS="-pipe"
```

(Adjust depending on how your Makefile defines flags.)

If `touch /tmp/testfile` fails even after `wsl --shutdown`, paste the output of the `mount` and `df -h` commands and I can help narrow it down.

## User

create a md of this conversation
