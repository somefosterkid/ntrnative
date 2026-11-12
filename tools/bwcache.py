"""Small on-disk caches in build/cache, for tools that read the same big, rarely changing inputs on every run (clones.py's
per-object function tables, port_decls.py's parsed headers). A cache entry is a pickle written atomically (a whole new
file beside it, then renamed) holding the key, a token and the value; it is only used when the token is the same now:
the inputs' stamps (stamp(): size and mtime of a file) and the hash of the code that made the value (code_hash()), so a
change of either makes the entry stale and the next run writes it again. A missing, torn or foreign entry is a miss.

    tok = (stamp(path), code_hash(__file__))
    value = bwcache.get("clones", path, tok)       # None on a miss
    bwcache.put("clones", path, tok, value)
"""
import hashlib
import os
import pickle
from pathlib import Path

DIR = Path(__file__).resolve().parents[1] / "build" / "cache"
_HASH = {}


def stamp(path):
    """(size, mtime in ns) of a file; None when it isn't there."""
    try:
        st = os.stat(path)
    except OSError:
        return None
    return st.st_size, st.st_mtime_ns


def code_hash(*files):
    """A hash of the files' contents (once a run): the version of the code that makes a cached value."""
    key = tuple(files)
    if key not in _HASH:
        h = hashlib.sha1()
        for f in files:
            h.update(Path(f).read_bytes())
        _HASH[key] = h.hexdigest()
    return _HASH[key]


def _file(kind, key):
    return DIR / kind / (hashlib.sha1(str(key).encode()).hexdigest()[:24] + ".pkl")


def get(kind, key, token):
    """The value cached under (kind, key) if it was made with this token; None otherwise."""
    try:
        k, t, value = pickle.loads(_file(kind, key).read_bytes())
    except Exception:  # absent, torn by a crash, from another version of the code
        return None
    return value if k == key and t == token else None


def put(kind, key, token, value):
    path = _file(kind, key)
    tmp = path.with_suffix(f".{os.getpid()}.tmp")
    try:
        path.parent.mkdir(parents=True, exist_ok=True)
        tmp.write_bytes(pickle.dumps((key, token, value), protocol=5))
        os.replace(tmp, path)
    except OSError:  # a cache that can't be written is only a slower run
        tmp.unlink(missing_ok=True)


def files_token(paths, *code):
    """The token for a value read from these files by code in these files: their stamps and the code's hash."""
    return tuple((str(p), stamp(p)) for p in paths), code_hash(*code)
