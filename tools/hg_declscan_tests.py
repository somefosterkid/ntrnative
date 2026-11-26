#!/usr/bin/env python3
"""tools/hg_declscan_tests.py: the -j default of hg_declscan.py follows the slot's CPU share."""
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import hg_declscan as ds  # noqa: E402


class Jobs(unittest.TestCase):
    def test_default_is_the_slot_share(self):
        self.assertEqual(ds.default_jobs({'NTR_SLOT_JOBS': '3'}), 3)
        self.assertEqual(ds.default_jobs({'NTR_SLOT_JOBS': '16'}), 16)

    def test_fallback_is_8(self):
        for env in ({}, {'NTR_SLOT_JOBS': ''}, {'NTR_SLOT_JOBS': 'x'}, {'NTR_SLOT_JOBS': '0'}):
            self.assertEqual(ds.default_jobs(env), 8, env)

    def test_cli_default_uses_the_env(self):
        import subprocess
        r = subprocess.run([sys.executable, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'hg_declscan.py'), '--help'],
                           capture_output=True, text=True, env=dict(os.environ, NTR_SLOT_JOBS='5'))
        self.assertIn('NTR_SLOT_JOBS', r.stdout)


if __name__ == '__main__':
    unittest.main()
