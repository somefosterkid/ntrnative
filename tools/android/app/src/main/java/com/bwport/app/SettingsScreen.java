package com.bwport.app;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.os.Build;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.style.ForegroundColorSpan;
import android.text.style.RelativeSizeSpan;
import android.util.Log;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;


/**
 * The settings screen (the maintainers' notes, "Android"): the Android twin of port/src/ios_settings.m, with the same sections, rows, keys,
 * names and choices in the same order. libntr's touch menu button asks for it (libntr-android-settings.patch, port/src/android_settings.c:
 * {@link #show}); the game and its sound stand still while it is up. A full-screen Dialog over SDL's activity (it needs no lifecycle
 * round trip: the game's surface stays), views built here in code (no resources), 48 dp targets, portrait and landscape. Every choice goes
 * through {@link #nativeSet}, which is SIM_UISettingSet (libntr's sim_gui.cpp: the helpers the desktop menus use, then sim_config.ini is
 * written); the rows read their values back with {@link #nativeGet} when the screen is built and after each change.
 * The log share (ab86's LogShare.share(Activity), step 4 of the platform layer) is called by reflection: a build without it says so.
 */
public final class SettingsScreen {
    private static final String TAG = "bwport";

    static native int nativeGet(String key);
    static native void nativeSet(String key, int v);
    static native String nativeStatus();
    static native String nativeFast3DWhyNot();
    static native void nativeShown();
    static native void nativeClosed();
    static native void nativePerfTest();
    static native void nativeRestart();
    static native void nativeGames();

    // ---- the rows (ios_settings.m's BWRow) ----------------------------------------------------------------------------------------

    private static final int MENU = 0, SWITCH = 1, SEGMENT = 2, SLIDER = 3, INFO = 4, ACTION = 5;

    private static final class Row {
        int kind;
        String key, title, detail;
        String[] options, subtitles;
        int[] values;
    }

    private static final class Section {
        String title;
        Row[] rows;
    }

    private static Row row(int kind, String key, String title) {
        Row r = new Row();
        r.kind = kind;
        r.key = key;
        r.title = title;
        return r;
    }

    private static Row menu(String key, String title, String[] opts, int[] vals, String[] subs) {
        Row r = row(MENU, key, title);
        r.options = opts;
        r.values = vals;
        r.subtitles = subs;
        return r;
    }

    private static Row segment(String key, String title, String[] opts) {
        Row r = row(SEGMENT, key, title);
        r.options = opts;
        r.values = new int[] {0, 1};
        return r;
    }

    private static Row info(String title, String detail) {
        Row r = row(INFO, null, title);
        r.detail = detail;
        return r;
    }

    private static Section section(String title, Row... rows) {
        Section s = new Section();
        s.title = title;
        s.rows = rows;
        return s;
    }

    private static String versionName(Context c) {
        try {
            PackageInfo pi = c.getPackageManager().getPackageInfo(c.getPackageName(), 0);
            return pi.versionName + " (" + pi.versionCode + ")";
        } catch (Exception e) {
            return "?";
        }
    }

    private Section[] buildSections() {
        String[] res = new String[8], ress = new String[8];
        int[] resv = new int[8];
        for (int n = 1; n <= 8; n++) {
            res[n - 1] = n + "x";
            resv[n - 1] = n;
            ress[n - 1] = (256 * n) + " x " + (384 * n);
        }
        return new Section[] {
            section("Display",
                menu("renderer", "Renderer", new String[] {"Auto", "CPU (vcore)", "GPU"}, new int[] {0, 1, 2}, null),
                menu("resolution", "Resolution", res, resv, ress),
                menu("antialias", "Antialiasing", new String[] {"Off", "4 samples", "SMAA"}, new int[] {0, 1, 2}, null),
                menu("renderer3d", "3D renderer", new String[] {"Exact", "Fast"}, new int[] {0, 1},
                     new String[] {"The DS's 3D bit for bit", "The GPU's own rasterizer: much cheaper, slightly off at polygon edges"}),
                row(SWITCH, "smooth", "Smooth motion")),
            section("Look at 2x and above",
                menu("filter", "Texture filter", new String[] {"Nearest", "Bilinear"}, new int[] {0, 1}, null),
                menu("line", "Lines", new String[] {"Wide", "Thin"}, new int[] {0, 1}, null),
                menu("edge", "Edge marking", new String[] {"DS width", "Fine"}, new int[] {0, 1}, null)),
            section("Speed",
                menu("ffspeed", "Fast-forward speed", new String[] {"2x", "3x", "4x", "Max"}, new int[] {2, 3, 4, 5}, null),
                menu("battlespeed", "Battle speed", new String[] {"Off", "2x", "3x", "4x"}, new int[] {1, 2, 3, 4}, null),
                menu("textspeed", "Text speed", new String[] {"Game", "Faster", "Fastest", "Instant"}, new int[] {0, 1, 2, 3}, null),
                menu("fadespeed", "Transitions", new String[] {"Normal", "2x", "4x", "Instant"}, new int[] {0, 1, 2, 3}, null),
                menu("walkspeed", "Walk speed", new String[] {"Normal", "2x", "4x"}, new int[] {1, 2, 4}, null),
                menu("runspeed", "Run speed", new String[] {"Normal", "2x", "4x"}, new int[] {1, 2, 4}, null),
                menu("surfspeed", "Surf speed", new String[] {"Normal", "2x", "4x"}, new int[] {1, 2, 4}, null),
                menu("savespeed", "Save speed", new String[] {"Off", "2x", "4x"}, new int[] {1, 2, 4}, null),
                row(SWITCH, "skipslow", "Skip frames when slow"),
                row(SWITCH, "musicnormal", "Music at normal speed")),
            section("Sound",
                row(SLIDER, "volume", "Volume"),
                menu("soundquality", "Sound quality", new String[] {"DS (exact)", "Enhanced"}, new int[] {0, 1},
                     new String[] {"Exactly the DS's output", "Smoother and cleaner"})),
            section("Controller",
                row(SWITCH, "hidetouch", "Hide touch controls when a controller is connected")),
            section("Performance overlay",
                row(SWITCH, "overlay", "Show overlay"),
                segment("corner", "Corner", new String[] {"Top left", "Top right"}),
                segment("detail", "Detail", new String[] {"Compact", "Detailed"})),
            section("About",
                info("Build", versionName(mAct)),
                info("Device", Build.MANUFACTURER + " " + Build.MODEL),
                info("Android", Build.VERSION.RELEASE + " (API " + Build.VERSION.SDK_INT + ")"),
                row(ACTION, "perftest", "Run performance test"),
                row(ACTION, "sharelog", "Share latest log"),
                row(ACTION, "games", "Games"),
                row(ACTION, "restart", "Restart to title")),
        };
    }

    // settings that grey out others (ios_settings.m's enabled:)
    private static boolean enabled(String key) {
        if (key == null) return true;
        switch (key) {
            case "antialias":
            case "renderer3d": {
                int r = nativeGet("renderer");
                return r == 2 || (r == 0 && nativeGet("active") == 2);
            }
            case "filter":
            case "line":
            case "edge":
                return nativeGet("resolution") > 1;
            case "corner":
            case "detail":
                return nativeGet("overlay") != 0;
            default:
                return true;
        }
    }

    private static String footer(String section) {
        switch (section) {
            case "Display": {
                String st = nativeStatus();
                String note = enabled("antialias") ? "" : "\nAntialiasing and the 3D renderer need the GPU renderer.";
                String why = nativeGet("renderer3d") == 1 ? nativeFast3DWhyNot() : null;
                if (why != null) note += "\n3D renderer Fast: not on this GPU (" + why + "); the exact renderer draws.";
                return "Drawing now: " + st + note + "\nSmooth motion draws the game's 30 fps 3D scenes at the screen's rate (up to 120) and backs off by itself "
                    + "when the game falls behind. The picture rate is automatic: 60 with it off.";
            }
            case "Sound":
                return "DS (exact) is the sound as the DS makes it: each sample held until the next and a 10-bit output, a little crunchy with "
                    + "its aliasing and noise floor. Enhanced interpolates the samples and keeps the mix at 16 bits: smoother, cleaner highs. "
                    + "The game itself plays the same.";
            case "Look at 2x and above":
                return nativeGet("resolution") > 1 ? null : "These act at Resolution 2x and above.";
            case "Speed":
                return "Play is at normal speed. The fast-forward button next to the controls switches to this speed; Max runs as fast as the "
                    + "game can, showing at most the screen's rate. Skipping drops pictures, never game time. Sound effects follow the speed; "
                    + "the music keeps its tempo while Music at normal speed is on.";
            default:
                return null;
        }
    }

    // ---- the dialog ----------------------------------------------------------------------------------------------------------------

    private static final int BG = 0xFF000000, CARD = 0xFF1C1C1E, LINE = 0xFF38383A, TEXT = 0xFFFFFFFF, DIM = 0xFF8E8E93, OFF = 0xFF55555A,
        BLUE = 0xFF0A84FF;

    private static SettingsScreen sCurrent;

    private final Activity mAct;
    private final float mDp;
    private Dialog mDialog;
    private Context mCtx;
    private ScrollView mScroll;
    private LinearLayout mList;
    private Section[] mSections;

    private SettingsScreen(Activity a) {
        mAct = a;
        mDp = a.getResources().getDisplayMetrics().density;
    }

    /** Called from the game's render thread (port/src/android_settings.c) when the touch menu button was pressed. */
    public static void show(final Activity act) {
        act.runOnUiThread(() -> {
            try {
                if (sCurrent != null) {
                    return;
                }
                SettingsScreen s = new SettingsScreen(act);
                sCurrent = s;
                s.open();
                nativeShown();
            } catch (Throwable t) {
                Log.e(TAG, "settings: " + t, t);
                sCurrent = null;
                nativeClosed();
            }
        });
    }

    /** The screen is up (Platform.java: the panel rate follows it). */
    static boolean isOpen() {
        return sCurrent != null;
    }

    private int dp(float v) {
        return Math.round(v * mDp);
    }

    private void open() {
        mSections = buildSections();
        Dialog d = new Dialog(mAct, android.R.style.Theme_DeviceDefault_NoActionBar);
        mDialog = d;
        mCtx = d.getContext();   // (the views take the dialog's Material theme, not the game activity's old black one)
        d.requestWindowFeature(Window.FEATURE_NO_TITLE);

        LinearLayout root = new LinearLayout(mCtx);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setBackgroundColor(BG);

        LinearLayout bar = new LinearLayout(mCtx);
        bar.setOrientation(LinearLayout.HORIZONTAL);
        bar.setGravity(Gravity.CENTER_VERTICAL);
        bar.setPadding(dp(20), dp(4), dp(8), dp(4));
        TextView title = text("Settings", 28, TEXT);
        title.setTypeface(Typeface.DEFAULT_BOLD);
        bar.addView(title, new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1));
        TextView done = text("Done", 18, BLUE);
        done.setTypeface(Typeface.DEFAULT_BOLD);
        done.setGravity(Gravity.CENTER);
        done.setMinWidth(dp(80));
        done.setMinHeight(dp(48));
        done.setContentDescription("Done");
        done.setOnClickListener(v -> d.dismiss());
        bar.addView(done, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, dp(48)));
        root.addView(bar, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));

        mScroll = new ScrollView(mCtx);
        mScroll.setFillViewport(false);
        mList = new LinearLayout(mCtx);
        mList.setOrientation(LinearLayout.VERTICAL);
        mList.setPadding(dp(16), 0, dp(16), dp(32));
        mScroll.addView(mList, new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));
        root.addView(mScroll, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 0, 1));

        d.setContentView(root);
        d.setCanceledOnTouchOutside(false);
        d.setOnDismissListener(x -> {
            sCurrent = null;
            nativeClosed();
        });
        render();
        d.show();
        Window w = d.getWindow();
        if (w != null) {
            w.setLayout(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            w.setBackgroundDrawable(new android.graphics.drawable.ColorDrawable(BG));
        }
    }

    private TextView text(String s, float sp, int color) {
        TextView t = new TextView(mCtx);
        t.setText(s);
        t.setTextSize(TypedValue.COMPLEX_UNIT_SP, sp);
        t.setTextColor(color);
        return t;
    }

    /** Builds every section again from the native values (the rows that grey out others, the footers, the choices shown). */
    private void render() {
        final int keep = mScroll.getScrollY();
        mList.removeAllViews();
        for (Section s : mSections) {
            TextView h = text(s.title.toUpperCase(), 13, DIM);
            h.setPadding(dp(16), dp(24), dp(16), dp(6));
            mList.addView(h);

            LinearLayout card = new LinearLayout(mCtx);
            card.setOrientation(LinearLayout.VERTICAL);
            GradientDrawable bg = new GradientDrawable();
            bg.setColor(CARD);
            bg.setCornerRadius(dp(12));
            card.setBackground(bg);
            for (int i = 0; i < s.rows.length; i++) {
                if (i > 0) {
                    View div = new View(mCtx);
                    div.setBackgroundColor(LINE);
                    LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 1);
                    lp.leftMargin = dp(16);
                    card.addView(div, lp);
                }
                card.addView(rowView(s.rows[i]));
            }
            mList.addView(card, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));

            String f = footer(s.title);
            if (f != null) {
                TextView ft = text(f, 13, DIM);
                ft.setPadding(dp(16), dp(8), dp(16), 0);
                mList.addView(ft);
            }
        }
        mScroll.post(() -> mScroll.scrollTo(0, keep));
    }

    private void change(String key, int v) {
        nativeSet(key, v);
        if (key.equals("renderer") || key.equals("resolution") || key.equals("overlay") || key.equals("smooth")) {
            mList.post(this::render);
        }
    }

    private View rowView(final Row r) {
        final boolean on = enabled(r.key);
        LinearLayout row = new LinearLayout(mCtx);
        row.setOrientation(LinearLayout.HORIZONTAL);
        row.setGravity(Gravity.CENTER_VERTICAL);
        row.setMinimumHeight(dp(56));
        row.setPadding(dp(16), dp(4), dp(12), dp(4));
        if (r.key != null) {
            row.setContentDescription(r.key);
        }

        TextView t = text(r.title, 17, on ? TEXT : OFF);
        if (r.kind == ACTION) {
            t.setTextColor(BLUE);
        }
        row.addView(t, new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1));

        switch (r.kind) {
            case MENU: {
                int cur = nativeGet(r.key);
                String shown = "";
                for (int i = 0; i < r.options.length; i++) {
                    if (r.values[i] == cur) shown = r.options[i];
                }
                TextView b = text(shown + "  ▾", 17, on ? BLUE : OFF);
                b.setGravity(Gravity.CENTER_VERTICAL | Gravity.END);
                b.setMinHeight(dp(48));
                b.setPadding(dp(12), 0, dp(4), 0);
                b.setTag("menu:" + r.key);
                row.addView(b);
                if (on) {
                    row.setOnClickListener(v -> chooseFrom(r));
                }
                break;
            }
            case SWITCH: {
                Switch sw = new Switch(mCtx);
                sw.setChecked(nativeGet(r.key) != 0);
                sw.setEnabled(on);
                sw.setMinHeight(dp(48));
                sw.setShowText(false);
                sw.setTag("switch:" + r.key);
                sw.setOnCheckedChangeListener((b, checked) -> change(r.key, checked ? 1 : 0));
                row.addView(sw, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT));
                if (on) {
                    row.setOnClickListener(v -> sw.toggle());
                }
                break;
            }
            case SEGMENT: {
                int cur = nativeGet(r.key);
                LinearLayout seg = new LinearLayout(mCtx);
                seg.setOrientation(LinearLayout.HORIZONTAL);
                for (int i = 0; i < r.options.length; i++) {
                    final int val = r.values[i];
                    TextView b = text(r.options[i], 15, i == cur ? Color.BLACK : (on ? TEXT : OFF));
                    b.setGravity(Gravity.CENTER);
                    b.setMinHeight(dp(48));
                    b.setMinWidth(dp(84));
                    b.setPadding(dp(10), 0, dp(10), 0);
                    GradientDrawable g = new GradientDrawable();
                    g.setColor(i == cur ? (on ? BLUE : OFF) : 0xFF2C2C2E);
                    float rad = dp(8);
                    g.setCornerRadii(i == 0 ? new float[] {rad, rad, 0, 0, 0, 0, rad, rad} : new float[] {0, 0, rad, rad, rad, rad, 0, 0});
                    b.setBackground(g);
                    b.setTag("seg:" + r.key + ":" + val);
                    if (on) {
                        b.setOnClickListener(v -> {
                            change(r.key, val);
                            mList.post(this::render);
                        });
                    }
                    seg.addView(b);
                }
                row.addView(seg);
                break;
            }
            case SLIDER: {
                final TextView pct = text(nativeGet(r.key) + "%", 15, DIM);
                pct.setMinWidth(dp(44));
                pct.setGravity(Gravity.END);
                SeekBar sb = new SeekBar(mCtx);
                sb.setMax(100);
                sb.setProgress(nativeGet(r.key));
                sb.setTag("slider:" + r.key);
                // the volume is applied as the finger drags (volumelive: no ini write, no log line) and saved when it lifts, as the desktop does
                sb.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                    @Override
                    public void onProgressChanged(SeekBar s, int p, boolean user) {
                        pct.setText(p + "%");
                        if (user) nativeSet("volumelive", p);
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar s) {}

                    @Override
                    public void onStopTrackingTouch(SeekBar s) {
                        nativeSet(r.key, s.getProgress());
                    }
                });
                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(dp(160), dp(48));
                lp.leftMargin = dp(8);
                row.addView(sb, lp);
                row.addView(pct);
                break;
            }
            case INFO: {
                TextView d = text(r.detail, 15, DIM);
                d.setGravity(Gravity.END);
                d.setPadding(dp(12), 0, dp(4), 0);
                row.addView(d, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT));
                break;
            }
            case ACTION:
                row.setOnClickListener(v -> {
                    if ("perftest".equals(r.key)) perfTest();
                    else if ("restart".equals(r.key)) restartToTitle();
                    else if ("games".equals(r.key)) toGames();
                    else shareLog();
                });
                break;
            default:
                break;
        }
        return row;
    }

    /** A single-choice list for a menu row (ios_settings.m's pull-down): the chosen item ticked, subtitles under the titles. */
    private void chooseFrom(final Row r) {
        int cur = nativeGet(r.key);
        CharSequence[] items = new CharSequence[r.options.length];
        int checked = -1;
        for (int i = 0; i < items.length; i++) {
            if (r.subtitles != null) {
                SpannableString sp = new SpannableString(r.options[i] + "\n" + r.subtitles[i]);
                int a = r.options[i].length();
                sp.setSpan(new RelativeSizeSpan(0.8f), a, sp.length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
                sp.setSpan(new ForegroundColorSpan(DIM), a, sp.length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
                items[i] = sp;
            } else {
                items[i] = r.options[i];
            }
            if (r.values[i] == cur) checked = i;
        }
        final AlertDialog[] dlg = new AlertDialog[1];
        dlg[0] = new AlertDialog.Builder(mAct, android.R.style.Theme_Material_Dialog_Alert)
            .setTitle(r.title)
            .setSingleChoiceItems(items, checked, (dialog, which) -> {
                dialog.dismiss();
                change(r.key, r.values[which]);
                mList.post(this::render);
            })
            .setNegativeButton("Cancel", null)
            .create();
        dlg[0].show();
        dlg[0].getListView().setTag("choices:" + r.key);
    }

    // Restart to title: the game's own soft reset (L+R+START+SELECT) once the screen is closed. The app ends after the game's message:
    // port/src/reset.c; the next start is at the title.
    private void restartToTitle() {
        new AlertDialog.Builder(mAct, android.R.style.Theme_Material_Dialog_Alert)
            .setTitle("Restart to the title screen?")
            .setMessage("Unsaved progress is lost. The game starts again at the title.")
            .setNegativeButton("Cancel", null)
            .setPositiveButton("Restart", (dialog, which) -> {
                mDialog.dismiss();
                nativeRestart();
            })
            .show();
    }

    // Games: the launcher after the game's own soft reset (port/src/restart_press.c bwport_press_games; reset.c ends this process and
    // LauncherActivity comes up). Unsaved progress is lost, as with Restart to title.
    private void toGames() {
        new AlertDialog.Builder(mAct, android.R.style.Theme_Material_Dialog_Alert)
            .setTitle("Go to the games?")
            .setMessage("Unsaved progress is lost.")
            .setNegativeButton("Cancel", null)
            .setPositiveButton("Games", (dialog, which) -> {
                mDialog.dismiss();
                nativeGames();
            })
            .show();
    }

    // libntr-ios-pace.patch: a fixed list of settings, a few seconds each from where the player stands, a table in the log; the settings
    // come back at the end. It starts when the screen closes (the game stands still while it is open).
    private void perfTest() {
        new AlertDialog.Builder(mAct, android.R.style.Theme_Material_Dialog_Alert)
            .setTitle("Performance test")
            .setMessage("About 1.6 minutes: the game steps through renderers, resolutions and speeds from where you stand. Don't touch the "
                + "screen, and wait the full 1.6 minutes: opening Settings stops the test. Then open Settings again and tap Share latest log. "
                + "For numbers that compare, start with a cool phone (not right after playing).")
            .setNegativeButton("Cancel", null)
            .setPositiveButton("Start", (dialog, which) -> {
                nativePerfTest();
                mDialog.dismiss();
            })
            .show();
    }

    private void shareLog() {
        try {
            Class<?> c = Class.forName("com.bwport.app.LogShare");
            c.getMethod("share", Activity.class).invoke(null, mAct);
        } catch (ClassNotFoundException | NoSuchMethodException e) {
            Toast.makeText(mAct, "Sharing the log is not in this build", Toast.LENGTH_LONG).show();
        } catch (Exception e) {
            Log.w(TAG, "share log: " + e);
            Toast.makeText(mAct, "Could not share the log", Toast.LENGTH_LONG).show();
        }
    }
}
