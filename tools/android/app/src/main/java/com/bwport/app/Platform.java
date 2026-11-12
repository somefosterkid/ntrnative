package com.bwport.app;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.os.Build;
import android.os.PowerManager;
import android.system.Os;
import android.util.Log;
import android.view.Choreographer;
import android.view.Display;
import android.view.Surface;

import org.libsdl.app.SDLActivity;

/**
 * What the game's activity does on iOS in port/src/apple_thermal.m and ios_input.m's CADisplayLink, as Android has them
 * (docs/android-parity.md): the heat state and Power Saver in the log (a shared log shows when the phone got hot), the app build line
 * for the log's head, and the panel's refresh rate: iOS asks 120 Hz only while Smooth motion is on and the game runs, 60 otherwise
 * (30-60 with the settings screen up); Android's default for a GL surface is the panel's top rate, which a 60 fps game does not need.
 * BwportActivity calls {@link #early} before SDL starts the game and {@link #init} after; everything here is the app's own, no library.
 */
final class Platform {
    private static final String TAG = "bwport";

    private Platform() {}

    // native side: port/src/android_log.c
    private static native void nativeThermal(int status);
    private static native void nativeLowPower(int on);
    private static native void nativeRate(float hz, int mode);

    /** Before SDL loads the game: the build line the log starts with (versionName and versionCode). */
    static void early(Activity a) {
        try {
            PackageInfo pi = a.getPackageManager().getPackageInfo(a.getPackageName(), 0);
            Os.setenv("BWPORT_APP_BUILD", pi.versionName + " (" + pi.versionCode + ")", true);
        } catch (Exception e) {
            Log.w(TAG, "BWPORT_APP_BUILD: " + e);
        }
    }

    /** After SDL has loaded libmain.so (the natives exist). */
    static void init(final Activity a) {
        try {
            PowerManager pm = (PowerManager) a.getSystemService(Context.POWER_SERVICE);
            if (pm != null) {
                if (Build.VERSION.SDK_INT >= 29) {
                    nativeThermal(pm.getCurrentThermalStatus());
                    pm.addThermalStatusListener(a.getMainExecutor(), s -> nativeThermal(s));
                }
                nativeLowPower(pm.isPowerSaveMode() ? 1 : 0);
                a.registerReceiver(new BroadcastReceiver() {
                    @Override
                    public void onReceive(Context c, Intent i) {
                        PowerManager p = (PowerManager) c.getSystemService(Context.POWER_SERVICE);
                        nativeLowPower(p != null && p.isPowerSaveMode() ? 1 : 0);
                    }
                }, new IntentFilter(PowerManager.ACTION_POWER_SAVE_MODE_CHANGED));
            }
        } catch (Throwable t) {
            Log.w(TAG, "thermal listener: " + t);
        }
        if (Build.VERSION.SDK_INT >= 30 && !"0".equals(System.getenv("BWPORT_RATE"))) {
            rateStart(a);
        }
    }

    // ---- the refresh rate ---------------------------------------------------------------------------------------------------------

    private static float maxHz(Activity a) {
        float best = 60f;
        try {
            Display d = a.getDisplay();
            Display.Mode cur = d.getMode();
            for (Display.Mode m : d.getSupportedModes()) {   // (the current resolution only: another one is a visible mode switch)
                if (m.getPhysicalWidth() == cur.getPhysicalWidth() && m.getPhysicalHeight() == cur.getPhysicalHeight()
                    && m.getRefreshRate() > best) {
                    best = m.getRefreshRate();
                }
            }
        } catch (Throwable t) {
            Log.w(TAG, "maxHz: " + t);
        }
        return best;
    }

    private static int sMode = -1;

    /**
     * Each frame (Choreographer, the UI thread): mode 2 = the panel's top rate (only when it is above 60 and Smooth motion is on),
     * 1 = 60, 0 = the settings screen is up and the game stands still (anything from 30). The vote is Surface.setFrameRate on the game's
     * surface; it is repeated when the surface changes (a rotation or a return from the background makes a new one).
     */
    private static void rateStart(final Activity a) {
        final float top = maxHz(a);
        final Choreographer.FrameCallback cb = new Choreographer.FrameCallback() {
            private Surface last;

            @Override
            public void doFrame(long t) {
                try {
                    int mode = SettingsScreen.isOpen() ? 0 : top > 61f && SettingsScreen.nativeGet("smooth") != 0 ? 2 : 1;
                    Surface s = SDLActivity.getNativeSurface();
                    if (s != null && s.isValid() && (mode != sMode || s != last)) {
                        float hz = mode == 2 ? top : mode == 1 ? 60f : 30f;
                        if (Build.VERSION.SDK_INT >= 31) {
                            s.setFrameRate(hz, Surface.FRAME_RATE_COMPATIBILITY_DEFAULT, Surface.CHANGE_FRAME_RATE_ALWAYS);
                        } else {
                            s.setFrameRate(hz, Surface.FRAME_RATE_COMPATIBILITY_DEFAULT);
                        }
                        if (mode != sMode) {
                            nativeRate(hz, mode);
                        }
                        sMode = mode;
                        last = s;
                    }
                } catch (UnsatisfiedLinkError e) {
                    return;    // (the game's library is not up yet, or is gone)
                } catch (Throwable t2) {
                    Log.w(TAG, "frame rate vote: " + t2);
                }
                Choreographer.getInstance().postFrameCallback(this);
            }
        };
        Choreographer.getInstance().postFrameCallback(cb);
    }
}
