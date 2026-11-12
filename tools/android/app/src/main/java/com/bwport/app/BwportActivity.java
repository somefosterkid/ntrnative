package com.bwport.app;

import android.content.Intent;
import android.graphics.Insets;
import android.os.Build;
import android.os.Bundle;
import android.system.Os;
import android.util.Log;
import android.view.Choreographer;
import android.view.DisplayCutout;
import android.view.View;
import android.view.WindowInsets;

import org.libsdl.app.SDLActivity;

/**
 * The game's activity (the maintainers' notes, "Android"): SDL's, which loads libSDL2.so and libmain.so (the game, built by meson:
 * port/machine/android-*.ini) and runs its SDL_main (port/src/android_main.c) on SDL's thread.
 * Test runs (tools/android_run.sh): the start intent's string extras named BWPORT_* or SDL_* become environment variables of the
 * process before the game starts (the iOS Simulator's SIMCTL_CHILD_ variables), e.g.
 * {@code adb shell am start -n com.bwport.app/.BwportActivity --es BWPORT_KEYS start@f600}.
 */
public class BwportActivity extends SDLActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if (getIntent() != null && TEST_LOOP.equals(getIntent().getAction())) {
            testLoop();
        }
        Bundle extras = getIntent() != null ? getIntent().getExtras() : null;
        if (extras != null) {
            for (String k : extras.keySet()) {
                Object v = extras.get(k);
                if (v instanceof String && (k.startsWith("BWPORT_") || k.startsWith("SDL_"))) {
                    try {
                        Os.setenv(k, (String) v, true);
                        Log.i("bwport", "env " + k + "=" + v);
                    } catch (Exception e) {
                        Log.w("bwport", "setenv " + k + ": " + e);
                    }
                }
            }
        }
        try {
            // the touch controls are laid out in dp (port/src/android_main.c, sim_touch.cpp): the density before the game starts
            Os.setenv("BWPORT_DENSITY", Float.toString(getResources().getDisplayMetrics().density), true);
        } catch (Exception e) {
            Log.w("bwport", "setenv BWPORT_DENSITY: " + e);
        }
        Platform.early(this);    // (Platform.java: the log's build line)
        super.onCreate(savedInstanceState);
        Platform.init(this);     // (the heat state in the log, the panel rate)
        // (SDL loaded the game's library: the insets can be reported)
        final View decor = getWindow().getDecorView();
        decor.setOnApplyWindowInsetsListener(new View.OnApplyWindowInsetsListener() {
            @Override
            public WindowInsets onApplyWindowInsets(View v, WindowInsets insets) {
                reportInsets(insets);
                return v.onApplyWindowInsets(insets);
            }
        });
        decor.requestApplyInsets();
    }

    /** Firebase Test Lab's game loop intent (tools/android_testlab.sh) */
    private static final String TEST_LOOP = "com.google.intent.action.TEST_LOOP";

    /**
     * A Firebase Test Lab game loop (tools/android_testlab.sh, docs/gpu-compat.md): the TEST_LOOP intent carries no extras, so the
     * run's variables come from testlab.env in the app's external files (pushed there with the ROM by the test's --other-files):
     * lines KEY=VALUE, the BWPORT_* and SDL_* ones become environment variables as an intent's extras do, and
     * BWPORT_TESTLAB_SECONDS (default 120) is when the activity ends itself (a game loop is over when the app finishes), after
     * copying log.txt to files/testlab/ (the results: that log, logcat and Test Lab's video).
     */
    private void testLoop() {
        int seconds = 120;
        java.io.File f = new java.io.File(getExternalFilesDir(null), "testlab.env");
        try (java.io.BufferedReader r = new java.io.BufferedReader(new java.io.FileReader(f))) {
            for (String line; (line = r.readLine()) != null;) {
                int eq = line.indexOf('=');
                if (eq <= 0 || line.startsWith("#")) {
                    continue;
                }
                String k = line.substring(0, eq).trim(), v = line.substring(eq + 1).trim();
                if (k.equals("BWPORT_TESTLAB_SECONDS")) {
                    try {
                        seconds = Integer.parseInt(v);
                    } catch (NumberFormatException e) {
                        Log.w("bwport", "testlab: " + line);
                    }
                } else if (k.startsWith("BWPORT_") || k.startsWith("SDL_")) {
                    try {
                        Os.setenv(k, v, true);
                        Log.i("bwport", "testlab env " + k + "=" + v);
                    } catch (Exception e) {
                        Log.w("bwport", "setenv " + k + ": " + e);
                    }
                }
            }
        } catch (java.io.IOException e) {
            Log.w("bwport", "testlab: no " + f + " (" + e + "): the game's defaults");
        }
        Log.i("bwport", "testlab: game loop, ending after " + seconds + " s");
        new android.os.Handler(android.os.Looper.getMainLooper()).postDelayed(new Runnable() {
            @Override
            public void run() {
                Log.i("bwport", "testlab: done");
                // the results Test Lab pulls (--directories-to-pull .../files/testlab): the log only, not the ROM or its files
                java.io.File out = getExternalFilesDir("testlab");
                try (java.io.InputStream in = new java.io.FileInputStream(new java.io.File(getExternalFilesDir(null), "log.txt"));
                     java.io.OutputStream o = new java.io.FileOutputStream(new java.io.File(out, "log.txt"))) {
                    byte[] b = new byte[65536];
                    for (int n; (n = in.read(b)) > 0;) {
                        o.write(b, 0, n);
                    }
                } catch (java.io.IOException e) {
                    Log.w("bwport", "testlab: log copy: " + e);
                }
                finishAndRemoveTask();
                android.os.Process.killProcess(android.os.Process.myPid());
            }
        }, seconds * 1000L);
    }

    /**
     * The panel's refresh, the equivalent of iOS's CADisplayLink (port/src/ios_input.m): Choreographer calls this on the UI thread at each
     * display refresh (60, 90 or 120 Hz, or what the system holds the app to); the frame time (CLOCK_MONOTONIC ns: the game's SpNowNs
     * clock) goes to the game, which keeps its presents on the real panel (libntr-android-touch.patch's SpLink). Runs only while the
     * activity is resumed.
     */
    private static native void nativeVsync(long frameTimeNanos);

    private boolean vsyncOn;
    private final Choreographer.FrameCallback vsyncCallback = new Choreographer.FrameCallback() {
        @Override
        public void doFrame(long frameTimeNanos) {
            if (!vsyncOn) {
                return;
            }
            try {
                nativeVsync(frameTimeNanos);
            } catch (UnsatisfiedLinkError e) {
                vsyncOn = false;
                return;
            }
            Choreographer.getInstance().postFrameCallback(this);
        }
    };

    private void vsyncStart() {
        if (!vsyncOn) {
            vsyncOn = true;
            Choreographer.getInstance().postFrameCallback(vsyncCallback);
        }
    }

    private void vsyncStop() {
        vsyncOn = false;
        Choreographer.getInstance().removeFrameCallback(vsyncCallback);
    }

    @Override
    protected void onResume() {
        super.onResume();
        vsyncStart();
    }

    @Override
    protected void onPause() {
        vsyncStop();
        super.onPause();
    }

    /**
     * Leaves the game for the launcher (docs/launcher.md, "Android launcher"; called by JNI from port/src/android_main.c's
     * bwport_android_leave, on the game's thread): the launcher activity is started (Games: the library; Restart to title: with
     * "bwport_play", it starts the game again once this process is gone), then this process ends. Until the in-process restart reaches
     * Android a new process is the simplest correct restart: the game's memory starts clean and the launcher's process stays up.
     * The save is on disk already (the game writes it with one fopen/fclose per backup call).
     */
    public void leaveGame(final String playCode) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Intent i = new Intent(BwportActivity.this, LauncherActivity.class);
                i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
                if (playCode != null && !playCode.isEmpty()) {
                    i.putExtra("bwport_play", playCode);
                } else {
                    i.putExtra("bwport_games", true);
                }
                Bundle ex = getIntent() != null ? getIntent().getExtras() : null;
                if (ex != null) {
                    for (String k : ex.keySet()) {
                        Object v = ex.get(k);
                        if (v instanceof String && (k.startsWith("BWPORT_") || k.startsWith("SDL_")) && !k.equals("BWPORT_GAME")
                                && !k.equals("BWPORT_IMPORT_TEST")) {
                            i.putExtra(k, (String) v);
                        }
                    }
                }
                Log.i("bwport", "leaving the game: " + (playCode != null && !playCode.isEmpty() ? "restart " + playCode : "the library"));
                startActivity(i);
                android.os.Process.killProcess(android.os.Process.myPid());
            }
        });
    }

    /** Native (port/src/android_main.c): the insets in pixels and the density, for the touch layout. */
    private static native void nativeInsets(int l, int t, int r, int b, float density);

    /**
     * The area the controls keep clear, as iOS's safe area: the display cutout (the notch, the camera hole, rounded corners) on every side,
     * and the navigation bar's size (stable, also while SDL's immersive mode hides it: its gesture area is there) at the sides and the bottom.
     * The status bar is not reserved (it is hidden; only a cutout takes the top).
     */
    @SuppressWarnings("deprecation")
    private void reportInsets(WindowInsets w) {
        int l = 0, t = 0, r = 0, b = 0;
        try {
            if (Build.VERSION.SDK_INT >= 28) {
                DisplayCutout c = w.getDisplayCutout();
                if (c != null) {
                    l = c.getSafeInsetLeft();
                    t = c.getSafeInsetTop();
                    r = c.getSafeInsetRight();
                    b = c.getSafeInsetBottom();
                }
            }
            if (Build.VERSION.SDK_INT >= 30) {
                Insets n = w.getInsetsIgnoringVisibility(WindowInsets.Type.navigationBars());
                l = Math.max(l, n.left);
                r = Math.max(r, n.right);
                b = Math.max(b, n.bottom);
            } else {
                l = Math.max(l, w.getStableInsetLeft());
                r = Math.max(r, w.getStableInsetRight());
                b = Math.max(b, w.getStableInsetBottom());
            }
            nativeInsets(l, t, r, b, getResources().getDisplayMetrics().density);
            Log.i("bwport", "insets " + l + " " + t + " " + r + " " + b + " density " + getResources().getDisplayMetrics().density);
        } catch (UnsatisfiedLinkError e) {
            Log.w("bwport", "nativeInsets: " + e);
        }
    }
}
