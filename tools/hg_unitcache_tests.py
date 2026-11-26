#!/usr/bin/env python3
"""tools/hg_unitcache_tests.py: the unit cache's keying (a header edit misses exactly the units that include it, a moved
checkout hits), its pruning, the casts compile through it, and hg_declscan's block items merging to the old decl list."""
import os
import shutil
import sys
import tempfile
import time
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import hg_declscan as ds  # noqa: E402
import hg_unitcache as UC  # noqa: E402

HAVE_CLANG = bool(shutil.which('clang'))


def tree(root, macro='1'):
    os.makedirs(os.path.join(root, 'inc'), exist_ok=True)
    w = lambda p, s: open(os.path.join(root, p), 'w').write(s)
    w('inc/h.h', '#define M %s\nint f(int);\n' % macro)
    w('inc/other.h', 'int g(int);\n')
    w('a.c', '#include "h.h"\nint a(void){ return M + (int)(void *)&a; }\n')
    w('b.c', '#include "other.h"\nint b(void){ return g(1); }\n')


@unittest.skipUnless(HAVE_CLANG, 'no clang')
class Keys(unittest.TestCase):
    def setUp(self):
        self.d = tempfile.mkdtemp()
        os.environ['BWPORT_UNITCACHE'] = os.path.join(self.d, 'cache')
        self.r1 = os.path.join(self.d, 'one')
        tree(self.r1)

    def tearDown(self):
        os.environ.pop('BWPORT_UNITCACHE', None)
        shutil.rmtree(self.d, ignore_errors=True)

    def key(self, root, src):
        UC._FILEH.clear()
        return UC.unit_key('t', 'v1', ['-I' + os.path.join(root, 'inc'), '-Wno-everything'], os.path.join(root, src), root, root)

    def test_changed_macro_in_a_header_misses_only_its_includers(self):
        ka, kb = self.key(self.r1, 'a.c'), self.key(self.r1, 'b.c')
        open(os.path.join(self.r1, 'inc/h.h'), 'w').write('#define M 2\nint f(int);\n')
        self.assertNotEqual(ka, self.key(self.r1, 'a.c'))
        self.assertEqual(kb, self.key(self.r1, 'b.c'))

    def test_whitespace_or_comment_edit_misses(self):
        k = self.key(self.r1, 'a.c')
        open(os.path.join(self.r1, 'inc/h.h'), 'a').write('/* x */\n')
        self.assertNotEqual(k, self.key(self.r1, 'a.c'))

    def test_a_copy_of_the_checkout_hits(self):
        r2 = os.path.join(self.d, 'two')
        shutil.copytree(self.r1, r2)
        self.assertEqual(self.key(self.r1, 'a.c'), self.key(r2, 'a.c'))

    def test_version_and_flags_miss(self):
        a = ['-I' + os.path.join(self.r1, 'inc')]
        s = os.path.join(self.r1, 'a.c')
        k = UC.unit_key('t', 'v1', a, s, self.r1, self.r1)
        self.assertNotEqual(k, UC.unit_key('t', 'v2', a, s, self.r1, self.r1))
        self.assertNotEqual(k, UC.unit_key('t', 'v1', a + ['-DX=1'], s, self.r1, self.r1))

    def test_a_failing_preprocess_is_not_cached(self):
        open(os.path.join(self.r1, 'a.c'), 'w').write('#include "nope.h"\n')
        self.assertIsNone(self.key(self.r1, 'a.c'))

    def test_off_disables(self):
        os.environ['BWPORT_UNITCACHE'] = 'off'
        self.assertIsNone(self.key(self.r1, 'a.c'))

    def test_reach_rule(self):
        self.assertTrue(UC.cache_wanted(10, 100, {}))                # a few units reached: the cache
        self.assertTrue(UC.cache_wanted(40, 100, {}))
        self.assertFalse(UC.cache_wanted(41, 100, {}))               # more than 40%: the old path, no keys
        self.assertFalse(UC.cache_wanted(10212, 10212, {}))          # rp12/rp14: everything reached
        self.assertFalse(UC.cache_wanted(0, 0, {}))                  # unknown
        self.assertTrue(UC.cache_wanted(80, 100, {'HGSS_REPIN_CACHE_REACH': '0.9'}))

    def test_skipped_unit_gets_no_key(self):
        sk = os.path.join(self.d, 'skip.txt')
        open(sk, 'w').write('a.c\n')
        UC._SKIP.clear()
        os.environ['BWPORT_UNITCACHE_SKIP'] = sk
        try:
            self.assertIsNone(self.key(self.r1, 'a.c'))
            self.assertIsNotNone(self.key(self.r1, 'b.c') if os.path.exists(os.path.join(self.r1, 'b.c')) else 'x')
            r2 = os.path.join(self.d, 'two')
            shutil.copytree(self.r1, r2)
            self.assertIsNone(self.key(r2, 'a.c'))                    # by tree-relative path, whatever the checkout
        finally:
            os.environ.pop('BWPORT_UNITCACHE_SKIP', None)
            UC._SKIP.clear()
        self.assertIsNotNone(self.key(self.r1, 'a.c'))

    def test_casts_unit_hit_equals_miss(self):
        args = ['-I' + os.path.join(self.r1, 'inc'), '-Wpointer-to-int-cast', '-Wshorten-64-to-32']
        out1, out2 = os.path.join(self.d, 'o1'), os.path.join(self.d, 'o2')
        src = os.path.join(self.r1, 'a.c')
        m = UC.casts_unit(self.r1, args, out1, self.r1, src)
        h = UC.casts_unit(self.r1, args, out2, self.r1, src)
        self.assertEqual((m[3], h[3]), (False, True))
        self.assertEqual(m[:3], h[:3])
        self.assertEqual(open(os.path.join(out1, 'log/a.c.log')).read(), open(os.path.join(out2, 'log/a.c.log')).read())
        r2 = os.path.join(self.d, 'two')
        shutil.copytree(self.r1, r2)
        w = UC.casts_unit(r2, ['-I' + os.path.join(r2, 'inc')] + args[1:], os.path.join(self.d, 'o3'), r2, os.path.join(r2, 'a.c'))
        self.assertTrue(w[3])
        self.assertIn(r2, w[2])
        self.assertNotIn(self.r1, w[2])


class Parse(unittest.TestCase):
    def test_make_rule(self):
        text = 'a.o: a.c ' + chr(92) + chr(10) + '  /x/h.h /y/with' + chr(92) + ' space.h' + chr(10)
        self.assertEqual(UC.parse_make_deps(text), ['a.c', '/x/h.h', '/y/with space.h'])


class Prune(unittest.TestCase):
    def test_ttl_and_cap(self):
        d = tempfile.mkdtemp()
        os.environ['BWPORT_UNITCACHE'] = d
        try:
            now = time.time()
            for i, age in enumerate((10, 9, 2, 1, 0)):
                UC.put('k', '%02dabc' % i, {'x': 'y' * 1000})
                os.utime(UC._path('k', '%02dabc' % i), (now - age * 86400,) * 2)
            self.assertEqual(UC.prune(ttl_days=7, now=now), (3, 2))
            self.assertIsNone(UC.get('k', '00abc'))
            self.assertIsNotNone(UC.get('k', '04abc'))
            kept, dropped = UC.prune(max_mb=0, now=now)         # over the cap: all of them, oldest first
            self.assertEqual((kept, dropped), (0, 3))
        finally:
            os.environ.pop('BWPORT_UNITCACHE', None)
            shutil.rmtree(d, ignore_errors=True)


class Items(unittest.TestCase):
    def d(self, name, f, l):
        return (name, 'i32', True, [], False, False, f, l, 'int', [])

    def test_blocks_merge_to_the_first_of_each_key(self):
        u1 = [self.d('a', 'h1.h', 1), self.d('b', 'h1.h', 2), self.d('m1', 'u1.c', 5), self.d('c', 'h2.h', 1)]
        u2 = [self.d('a', 'h1.h', 1), self.d('b', 'h1.h', 2), self.d('c', 'h2.h', 1), self.d('d', 'h2.h', 2), self.d('m2', 'u2.c', 7)]
        naive, seen = [], set()
        for u in (u1, u2):
            for x in u:
                if (x[0], x[6], x[7]) not in seen:
                    seen.add((x[0], x[6], x[7]))
                    naive.append(x)
        decls, seen, merged, blocks = [], set(), set(), {}
        for u, main in ((u1, 'u1.c'), (u2, 'u2.c')):
            items, bl = ds.group_items(u, main)
            blocks.update(bl)
            ds.merge_items(items, blocks, merged, seen, decls)
        self.assertEqual(decls, [tuple(x) for x in naive])
        a, _ = ds.group_items(u1, 'u1.c')
        b, _ = ds.group_items(u2, 'u2.c')
        self.assertEqual(a[0], b[0])          # the same header run: the same block


if __name__ == '__main__':
    unittest.main()
