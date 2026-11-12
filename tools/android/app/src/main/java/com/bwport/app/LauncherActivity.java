package com.bwport.app;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.RenderEffect;
import android.graphics.Shader;
import android.graphics.Typeface;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.GradientDrawable;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.ParcelFileDescriptor;
import android.text.format.DateUtils;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowInsets;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.HorizontalScrollView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupMenu;
import android.widget.ProgressBar;
import android.widget.ScrollView;
import android.widget.TextView;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.RandomAccessFile;
import java.text.DateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * The launcher (docs/launcher.md, "Android launcher"): the app's home screen, the Android version of port/src/ios_launcher.m over the
 * same C core (LauncherCore, port/src/android_launcher.c). The games in the library (Library layout: cards; Tabs layout: a tab per game
 * and one game's page), Add Game (the system file picker), Play, the saves (export, import), the backups (restore), the artwork, the
 * removal, the launcher's own menu. It runs in the app's main process; Play starts BwportActivity, the game, in the ":game" process,
 * which ends and comes back here for Games (BwportActivity.leaveGame). Views are built in code, as SettingsScreen's are.
 * Test aids (start-intent string extras): BWPORT_IMPORT_TEST=PATH adds that ROM without the picker, BWPORT_LAUNCHER=1 stays on the
 * library even when Start is "the last game". Every BWPORT_* / SDL_* extra is handed on to the game.
 */
public class LauncherActivity extends Activity {
    static final String TAG = "bwport";
    static final int PICK_ROM = 1, PICK_SAVE = 2, PICK_ART = 3, SAVE_AS = 4;

    static final int BG = 0xFF0E1015, SURFACE = 0xFF1B1F27, SURFACE2 = 0xFF262B35, TEXT = 0xFFF2F4F8, SUBTLE = 0xFF9AA3B2;

    private final Handler mUi = new Handler(Looper.getMainLooper());
    private List<LauncherCore.Game> mGames = new java.util.ArrayList<>();
    private String mSelected;    // the tabs layout's game
    private LinearLayout mRoot;
    private int mInsL, mInsT, mInsR, mInsB;
    private final Set<String> mFetching = new HashSet<>();
    private final HashMap<String, Bitmap> mBitmaps = new HashMap<>();
    private boolean mInitOk;
    private String mInitErr = "";
    private Dialog mBusy;
    private ProgressBar mBusyBar;
    private TextView mBusyText;
    private String mPendingCode;    // the game a picker or SAVE_AS answers for
    private String mPendingExport;    // the exported save waiting for SAVE_AS
    private boolean mFirst = true;

    // ------------------------------------------------------------------------------------------------------------- life

    @Override
    protected void onCreate(Bundle b) {
        super.onCreate(b);
        // test runs: the BWPORT_* extras (BWPORT_COVER_FAIL, BWPORT_LCTEST ...) are environment variables of this process too
        if (getIntent() != null && getIntent().getExtras() != null) {
            for (String k : getIntent().getExtras().keySet()) {
                Object v = getIntent().getExtras().get(k);
                if (v instanceof String && k.startsWith("BWPORT_")) {
                    try {
                        android.system.Os.setenv(k, (String) v, true);
                    } catch (Exception e) {
                        Log.w(TAG, "setenv " + k + ": " + e);
                    }
                }
            }
        }
        File dir = getExternalFilesDir(null);
        if (dir == null) {
            dir = getFilesDir();
        }
        try {
            mInitErr = LauncherCore.nativeInit(dir.getAbsolutePath());
            mInitOk = true;
        } catch (UnsatisfiedLinkError e) {
            Log.e(TAG, "launcher core: " + e);
            mInitErr = "The launcher's library did not load.";
        }
        if (Build.VERSION.SDK_INT >= 28) {
            getWindow().getAttributes().layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }
        getWindow().setStatusBarColor(Color.TRANSPARENT);
        getWindow().setNavigationBarColor(Color.TRANSPARENT);
        mRoot = new LinearLayout(this);
        mRoot.setOrientation(LinearLayout.VERTICAL);
        mRoot.setBackgroundColor(BG);
        setContentView(mRoot);
        mRoot.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION);
        mRoot.setOnApplyWindowInsetsListener((v, w) -> {
            int l = w.getSystemWindowInsetLeft(), t = w.getSystemWindowInsetTop(), r = w.getSystemWindowInsetRight(),
                bt = w.getSystemWindowInsetBottom();
            if (Build.VERSION.SDK_INT >= 28 && w.getDisplayCutout() != null) {
                l = Math.max(l, w.getDisplayCutout().getSafeInsetLeft());
                t = Math.max(t, w.getDisplayCutout().getSafeInsetTop());
                r = Math.max(r, w.getDisplayCutout().getSafeInsetRight());
                bt = Math.max(bt, w.getDisplayCutout().getSafeInsetBottom());
            }
            if (l != mInsL || t != mInsT || r != mInsR || bt != mInsB) {
                mInsL = l;
                mInsT = t;
                mInsR = r;
                mInsB = bt;
                build();
            }
            return w;
        });
        mRoot.requestApplyInsets();
        reload();
        handleIntent(getIntent());
        mFirst = false;
    }

    @Override
    protected void onNewIntent(Intent i) {
        super.onNewIntent(i);
        setIntent(i);
        reload();
        handleIntent(i);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (!mFirst) {
            reload();
        }
    }

    @Override
    public void onConfigurationChanged(Configuration c) {
        super.onConfigurationChanged(c);
        mRoot.requestApplyInsets();
        build();
    }

    /** From the game (BwportActivity.leaveGame): "bwport_play" = start that game again (Restart to title), "bwport_games" = the library. */
    private void handleIntent(Intent i) {
        if (!mInitOk) {
            return;
        }
        String play = i.getStringExtra("bwport_play");
        i.removeExtra("bwport_play");
        i.removeExtra("bwport_games");
        boolean stay = i.getStringExtra("BWPORT_LAUNCHER") != null || i.getBooleanExtra("bwport_stay", false);
        if (mInitErr != null && !mInitErr.isEmpty()) {
            note("The old files could not be moved", mInitErr + "\n\nThe game keeps using them where they are.");
            mInitErr = "";
        }
        String test = i.getStringExtra("BWPORT_IMPORT_TEST");
        if (test != null) {
            i.removeExtra("BWPORT_IMPORT_TEST");
            importFrom(null, new File(test));
            return;
        }
        if (play != null) {    // the game's process is ending: give it a moment before the next one starts
            mUi.postDelayed(() -> play(play), 700);
            return;
        }
        if (mFirst && !stay && LauncherCore.nativeGet(LauncherCore.START) == 0) {
            String last = LauncherCore.nativeLastGame();
            LauncherCore.Game g = find(last);
            if (g == null && mGames.size() == 1) {
                g = mGames.get(0);
            }
            if (g != null && (g.hasFiles || g.hasRom) && g.playable) {
                Log.i(TAG, "launcher: Start = the last game: " + g.title);
                play(g.code);
            }
        }
    }

    private LauncherCore.Game find(String code) {
        for (LauncherCore.Game g : mGames) {
            if (g.code.equals(code)) {
                return g;
            }
        }
        return null;
    }

    private void reload() {
        if (mInitOk) {
            mGames = LauncherCore.games();
            if (find(mSelected) == null) {
                mSelected = mGames.isEmpty() ? null : mGames.get(0).code;
            }
            for (LauncherCore.Game g : mGames) {
                if (LauncherCore.nativeCoverWanted(g.code)) {
                    fetchCover(g, false);
                }
            }
        }
        build();
    }

    // ------------------------------------------------------------------------------------------------------------- helpers

    private int dp(float v) {
        return Math.round(v * getResources().getDisplayMetrics().density);
    }

    private static int[] theme(String code) {    // accent, deep, glow, dark (ios_launcher.m's Theme)
        switch (code) {
            case "IRAO": return new int[] {0xE8590C, 0xF4F1EC, 0xFFB37A, 0};
            case "IREO": return new int[] {0xF2B705, 0x10131C, 0x2E6F9E, 1};
            case "IRDO": return new int[] {0xD6336C, 0xEEF3F7, 0x8FD3FF, 0};
            default: return new int[] {0x3D8BFF, 0x0B0F1A, 0x1E5BD8, 1};
        }
    }

    private static int accent(LauncherCore.Game g) {
        return 0xFF000000 | theme(g.code)[0];
    }

    private TextView text(String s, float sp, int color, boolean bold) {
        TextView t = new TextView(this);
        t.setText(s);
        t.setTextSize(sp);
        t.setTextColor(color);
        if (bold) {
            t.setTypeface(Typeface.DEFAULT_BOLD);
        }
        return t;
    }

    private GradientDrawable round(int color, float radiusDp) {
        GradientDrawable d = new GradientDrawable();
        d.setColor(color);
        d.setCornerRadius(dp(radiusDp));
        return d;
    }

    /** A 48 dp-high button: filled (the accent) or glass. */
    private TextView button(String label, int fill, int fg) {
        TextView t = text(label, 16, fg, true);
        t.setGravity(Gravity.CENTER);
        t.setMinHeight(dp(48));
        t.setMinWidth(dp(48));
        t.setPadding(dp(18), 0, dp(18), 0);
        t.setBackground(round(fill, 24));
        t.setClickable(true);
        t.setContentDescription(label);
        return t;
    }

    private String summary(LauncherCore.Game g) {
        if (!g.hasSave) {
            return "No save yet";
        }
        String when = DateUtils.getRelativeTimeSpanString(g.saved * 1000L, System.currentTimeMillis(), DateUtils.MINUTE_IN_MILLIS).toString();
        if (g.playSeconds < 0) {
            return "Saved " + when;
        }
        return "Saved " + when + "  ·  " + playText(g.playSeconds) + " played";
    }

    private static String playText(long secs) {
        long m = secs / 60, h = m / 60;
        m %= 60;
        return h > 0 ? h + " h " + m + " min" : m + " min";
    }

    private String dateText(long epoch) {
        return DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.SHORT).format(new Date(epoch * 1000L));
    }

    // ------------------------------------------------------------------------------------------------------------- images

    private File gameDir(LauncherCore.Game g) {
        return new File(LauncherCore.nativeGameDir(g.code));
    }

    /** The user's artwork.jpg, else the downloaded cover.jpg; sampled to at most ~1000 px; null when neither. */
    private Bitmap cover(LauncherCore.Game g) {
        File f = new File(gameDir(g), "artwork.jpg");
        if (!f.exists()) {
            f = new File(gameDir(g), "cover.jpg");
        }
        if (!f.exists()) {
            return null;
        }
        String key = f.getAbsolutePath() + f.lastModified();
        Bitmap b = mBitmaps.get(key);
        if (b == null) {
            BitmapFactory.Options o = new BitmapFactory.Options();
            o.inJustDecodeBounds = true;
            BitmapFactory.decodeFile(f.getAbsolutePath(), o);
            int s = 1;
            while (Math.max(o.outWidth, o.outHeight) / (s * 2) >= 1000) {
                s *= 2;
            }
            o = new BitmapFactory.Options();
            o.inSampleSize = s;
            b = BitmapFactory.decodeFile(f.getAbsolutePath(), o);
            if (b != null) {
                mBitmaps.put(key, b);
            }
        }
        return b;
    }

    /** The game's banner icon (32x32, the ROM's banner: header 0x68 -> bitmap at +0x20, palette at +0x220), or null. */
    private Bitmap icon(LauncherCore.Game g) {
        File rom = new File(gameDir(g), g.title + ".nds");
        if (!rom.exists()) {
            File[] l = gameDir(g).listFiles((d, n) -> n.endsWith(".nds"));
            if (l == null || l.length == 0) {
                return null;
            }
            rom = l[0];
        }
        String key = rom.getAbsolutePath() + "#icon";
        Bitmap b = mBitmaps.get(key);
        if (b != null) {
            return b;
        }
        try (RandomAccessFile f = new RandomAccessFile(rom, "r")) {
            byte[] h = new byte[4];
            f.seek(0x68);
            f.readFully(h);
            long off = (h[0] & 255) | (h[1] & 255) << 8 | (h[2] & 255) << 16 | (long) (h[3] & 255) << 24;
            if (off == 0 || off > f.length() - 0x240) {
                return null;
            }
            byte[] d = new byte[0x220], p = new byte[32];
            f.seek(off + 0x20);
            f.readFully(d);
            f.seek(off + 0x220);
            f.readFully(p);
            int[] pal = new int[16];
            for (int i = 0; i < 16; i++) {
                int c = (p[i * 2] & 255) | (p[i * 2 + 1] & 255) << 8;
                pal[i] = i == 0 ? 0 : 0xFF000000 | ((c & 31) * 255 / 31) << 16 | (((c >> 5) & 31) * 255 / 31) << 8 | ((c >> 10) & 31) * 255 / 31;
            }
            int[] px = new int[32 * 32];
            for (int ty = 0; ty < 4; ty++) {
                for (int tx = 0; tx < 4; tx++) {
                    for (int y = 0; y < 8; y++) {
                        for (int x = 0; x < 8; x++) {
                            int o = (ty * 4 + tx) * 32 + y * 4 + x / 2;
                            int idx = (x & 1) == 0 ? d[o] & 15 : (d[o] >> 4) & 15;
                            px[(ty * 8 + y) * 32 + tx * 8 + x] = pal[idx];
                        }
                    }
                }
            }
            b = Bitmap.createBitmap(px, 32, 32, Bitmap.Config.ARGB_8888);
            mBitmaps.put(key, b);
            return b;
        } catch (Exception e) {
            return null;
        }
    }

    private View iconView(LauncherCore.Game g, int sizeDp) {
        Bitmap b = icon(g);
        ImageView v = new ImageView(this);
        if (b != null) {
            BitmapDrawable d = new BitmapDrawable(getResources(), b);
            d.setFilterBitmap(false);    // pixel art, nearest
            v.setImageDrawable(d);
        } else {
            v.setImageDrawable(round(accent(g), 8));
        }
        v.setLayoutParams(new ViewGroup.LayoutParams(dp(sizeDp), dp(sizeDp)));
        return v;
    }

    /** The cover in a box: the game's gradient behind, the cover aspect-fit over a blurred copy of itself; "no cover": the icon. */
    private View showcase(LauncherCore.Game g, boolean cardTop) {
        int[] t = theme(g.code);
        FrameLayout f = new FrameLayout(this);
        GradientDrawable grad = new GradientDrawable(GradientDrawable.Orientation.TL_BR,
                new int[] {0xFF000000 | t[2], 0xFF000000 | (t[3] == 1 ? t[1] : 0x20252E)});
        f.setBackground(grad);
        Bitmap c = cover(g);
        if (c != null) {
            ImageView back = new ImageView(this);
            back.setScaleType(ImageView.ScaleType.CENTER_CROP);
            back.setImageBitmap(c);
            if (Build.VERSION.SDK_INT >= 31) {
                back.setRenderEffect(RenderEffect.createBlurEffect(40, 40, Shader.TileMode.CLAMP));
            }
            back.setColorFilter(0x88000000, android.graphics.PorterDuff.Mode.SRC_ATOP);
            f.addView(back, new FrameLayout.LayoutParams(-1, -1));
            ImageView fore = new ImageView(this);
            fore.setScaleType(ImageView.ScaleType.FIT_CENTER);
            fore.setImageBitmap(c);
            fore.setContentDescription(g.title + " cover");
            FrameLayout.LayoutParams lp = new FrameLayout.LayoutParams(-1, -1);
            lp.setMargins(dp(16), dp(16), dp(16), dp(16));
            f.addView(fore, lp);
        } else {
            View ic = iconView(g, 64);
            f.addView(ic, new FrameLayout.LayoutParams(dp(64), dp(64), Gravity.CENTER));
        }
        if (c != null && !g.coverSource.isEmpty() && !cardTop) {
            TextView credit = text("Cover art: " + g.coverSource, 11, 0xCCFFFFFF, false);
            credit.setPadding(dp(10), dp(4), dp(10), dp(4));
            credit.setBackground(round(0x66000000, 10));
            credit.setOnClickListener(v -> openLink(g.coverLink));
            FrameLayout.LayoutParams lp = new FrameLayout.LayoutParams(-2, -2, Gravity.BOTTOM | Gravity.END);
            lp.setMargins(dp(8), dp(8), dp(8), dp(8));
            f.addView(credit, lp);
        }
        return f;
    }

    private void openLink(String link) {
        try {
            startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(link)));
        } catch (Exception e) {
            Log.w(TAG, "link: " + e);
        }
    }

    // ------------------------------------------------------------------------------------------------------------- views

    private void build() {
        if (mRoot == null) {
            return;
        }
        mRoot.removeAllViews();
        mRoot.setPadding(mInsL, mInsT, mInsR, mInsB);
        boolean tabs = mInitOk && LauncherCore.nativeGet(LauncherCore.LAYOUT) == 1 && !mGames.isEmpty();
        LauncherCore.Game sel = tabs ? find(mSelected) : null;
        if (sel != null) {
            int[] t = theme(sel.code);
            GradientDrawable g = new GradientDrawable(GradientDrawable.Orientation.TOP_BOTTOM,
                    new int[] {0xFF000000 | t[1], t[3] == 1 ? 0xFF000000 | t[2] & 0x7F7F7F : 0xFFFFFFFF});
            mRoot.setBackground(t[3] == 1 ? g : new ColorDrawable(0xFF000000 | t[1]));
        } else {
            mRoot.setBackgroundColor(BG);
        }
        int fg = sel != null && theme(sel.code)[3] == 0 ? 0xFF1A1C22 : TEXT;
        // the top row: the title or the tabs, then Add and the menu
        LinearLayout top = new LinearLayout(this);
        top.setOrientation(LinearLayout.HORIZONTAL);
        top.setGravity(Gravity.CENTER_VERTICAL);
        top.setPadding(dp(16), dp(8), dp(8), dp(4));
        if (tabs) {
            HorizontalScrollView hs = new HorizontalScrollView(this);
            hs.setHorizontalScrollBarEnabled(false);
            LinearLayout row = new LinearLayout(this);
            row.setGravity(Gravity.CENTER_VERTICAL);
            for (LauncherCore.Game g : mGames) {
                boolean on = g == sel;
                TextView c = button(g.shortName, on ? accent(g) : 0x33808890, on ? Color.WHITE : fg);
                c.setContentDescription("Tab " + g.shortName);
                c.setOnClickListener(v -> {
                    mSelected = g.code;
                    build();
                });
                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(-2, -2);
                lp.rightMargin = dp(8);
                row.addView(c, lp);
            }
            hs.addView(row);
            top.addView(hs, new LinearLayout.LayoutParams(0, -2, 1));
        } else {
            top.addView(text("Games", 28, fg, true), new LinearLayout.LayoutParams(0, -2, 1));
        }
        if (mInitOk) {
            TextView add = button("+", 0x33808890, fg);
            add.setTextSize(24);
            add.setContentDescription("Add Game");
            add.setOnClickListener(v -> pick(PICK_ROM, null));
            top.addView(add, new LinearLayout.LayoutParams(dp(48), dp(48)));
            TextView menu = button("⋮", 0x33808890, fg);
            menu.setTextSize(22);
            menu.setContentDescription("Menu");
            menu.setOnClickListener(v -> globalMenu(v));
            LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(dp(48), dp(48));
            lp.leftMargin = dp(8);
            top.addView(menu, lp);
        }
        mRoot.addView(top, new LinearLayout.LayoutParams(-1, -2));
        if (!mInitOk || mGames.isEmpty()) {
            mRoot.addView(empty(), new LinearLayout.LayoutParams(-1, 0, 1));
        } else if (tabs) {
            mRoot.addView(page(sel != null ? sel : mGames.get(0), fg), new LinearLayout.LayoutParams(-1, 0, 1));
        } else {
            mRoot.addView(library(), new LinearLayout.LayoutParams(-1, 0, 1));
        }
    }

    private View empty() {
        LinearLayout l = new LinearLayout(this);
        l.setOrientation(LinearLayout.VERTICAL);
        l.setGravity(Gravity.CENTER);
        l.setPadding(dp(32), dp(16), dp(32), dp(16));
        TextView t = text("No games yet", 24, TEXT, true);
        t.setGravity(Gravity.CENTER);
        TextView b = text("Add your own copy of Pokemon Black (USA/Europe, the NDSi Enhanced 1.0 dump) as a .nds file.", 16, SUBTLE, false);
        b.setGravity(Gravity.CENTER);
        TextView add = button("Add Game", 0xFF3D8BFF, Color.WHITE);
        add.setOnClickListener(v -> pick(PICK_ROM, null));
        l.addView(t);
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(-2, -2);
        lp.topMargin = dp(8);
        l.addView(b, lp);
        lp = new LinearLayout.LayoutParams(-2, -2);
        lp.topMargin = dp(24);
        l.addView(add, lp);
        return l;
    }

    /** The library: every game as a card (a grid as wide as the window allows), then Add Game. */
    private View library() {
        int w = getResources().getDisplayMetrics().widthPixels - mInsL - mInsR;
        int cols = Math.max(1, Math.min(4, Math.round(w / dp(1) / 330f)));
        ScrollView sv = new ScrollView(this);
        sv.setClipToPadding(false);
        LinearLayout col = new LinearLayout(this);
        col.setOrientation(LinearLayout.VERTICAL);
        col.setPadding(dp(12), dp(4), dp(12), dp(16));
        int n = mGames.size() + 1;
        for (int i = 0; i < n; i += cols) {
            LinearLayout row = new LinearLayout(this);
            row.setOrientation(LinearLayout.HORIZONTAL);
            for (int j = 0; j < cols; j++) {
                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(0, -2, 1);
                lp.setMargins(dp(4), dp(4), dp(4), dp(4));
                if (i + j < mGames.size()) {
                    row.addView(card(mGames.get(i + j), w / cols), lp);
                } else if (i + j == mGames.size()) {
                    row.addView(addCard(), lp);
                } else {
                    row.addView(new View(this), lp);
                }
            }
            col.addView(row, new LinearLayout.LayoutParams(-1, -2));
        }
        sv.addView(col);
        return sv;
    }

    private View addCard() {
        LinearLayout c = new LinearLayout(this);
        c.setOrientation(LinearLayout.VERTICAL);
        c.setGravity(Gravity.CENTER);
        c.setBackground(round(SURFACE, 20));
        c.setMinimumHeight(dp(160));
        c.setClickable(true);
        c.setContentDescription("Add Game");
        TextView plus = text("+", 40, SUBTLE, false);
        plus.setGravity(Gravity.CENTER);
        TextView l = text("Add Game", 16, TEXT, true);
        l.setGravity(Gravity.CENTER);
        c.addView(plus);
        c.addView(l);
        c.setOnClickListener(v -> pick(PICK_ROM, null));
        return c;
    }

    private View card(LauncherCore.Game g, int widthPx) {
        LinearLayout c = new LinearLayout(this);
        c.setOrientation(LinearLayout.VERTICAL);
        c.setBackground(round(SURFACE, 20));
        c.setClipToOutline(true);
        View show = showcase(g, true);
        c.addView(show, new LinearLayout.LayoutParams(-1, Math.round(widthPx * 0.62f)));
        LinearLayout info = new LinearLayout(this);
        info.setOrientation(LinearLayout.VERTICAL);
        info.setPadding(dp(14), dp(10), dp(14), dp(10));
        LinearLayout head = new LinearLayout(this);
        head.setGravity(Gravity.CENTER_VERTICAL);
        head.addView(iconView(g, 28));
        TextView title = text(g.title, 18, TEXT, true);
        title.setPadding(dp(10), 0, 0, 0);
        title.setSingleLine(true);
        head.addView(title, new LinearLayout.LayoutParams(0, -2, 1));
        info.addView(head);
        TextView sum = text(g.playable ? summary(g) : "Not supported yet", 13, SUBTLE, false);
        info.addView(sum);
        LinearLayout actions = new LinearLayout(this);
        actions.setGravity(Gravity.CENTER_VERTICAL);
        TextView play = button(g.hasSave ? "Continue" : "Play", accent(g), Color.WHITE);
        play.setOnClickListener(v -> play(g.code));
        actions.addView(play, new LinearLayout.LayoutParams(0, dp(48), 1));
        TextView more = button("⋮", SURFACE2, TEXT);
        more.setContentDescription(g.title + " menu");
        more.setOnClickListener(v -> gameMenu(v, g));
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(dp(48), dp(48));
        lp.leftMargin = dp(8);
        actions.addView(more, lp);
        lp = new LinearLayout.LayoutParams(-1, -2);
        lp.topMargin = dp(10);
        info.addView(actions, lp);
        c.addView(info, new LinearLayout.LayoutParams(-1, -2));
        return c;
    }

    /** The tabs layout's page: the showcase and the game's details; side by side in landscape. */
    private View page(LauncherCore.Game g, int fg) {
        boolean wide = getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE;
        LinearLayout box = new LinearLayout(this);
        box.setOrientation(wide ? LinearLayout.HORIZONTAL : LinearLayout.VERTICAL);
        box.setPadding(dp(16), dp(8), dp(16), dp(16));
        View show = showcase(g, false);
        show.setClipToOutline(true);
        show.setBackground(show.getBackground());
        LinearLayout.LayoutParams sp = wide ? new LinearLayout.LayoutParams(0, -1, 1.2f) : new LinearLayout.LayoutParams(-1, 0, 1.4f);
        box.addView(show, sp);
        LinearLayout info = new LinearLayout(this);
        info.setOrientation(LinearLayout.VERTICAL);
        info.setGravity(wide ? Gravity.CENTER_VERTICAL : Gravity.TOP);
        info.setPadding(dp(wide ? 20 : 0), dp(wide ? 0 : 14), 0, 0);
        LinearLayout head = new LinearLayout(this);
        head.setGravity(Gravity.CENTER_VERTICAL);
        head.addView(iconView(g, 44));
        TextView title = text(g.shortName, 30, fg, true);
        title.setPadding(dp(12), 0, 0, 0);
        head.addView(title);
        info.addView(head);
        TextView sum = text(g.playable ? summary(g) : "Example entry", 15, fg == TEXT ? SUBTLE : 0xFF555B66, false);
        sum.setPadding(0, dp(6), 0, 0);
        info.addView(sum);
        TextView det = text(g.playable ? "USA / Europe  ·  NDSi Enhanced" : "Not supported yet", 13, fg == TEXT ? SUBTLE : 0xFF555B66, false);
        info.addView(det);
        TextView play = button(g.hasSave ? "Continue" : "Play", accent(g), Color.WHITE);
        play.setTextSize(20);
        play.setOnClickListener(v -> play(g.code));
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(-1, dp(56));
        lp.topMargin = dp(14);
        info.addView(play, lp);
        LinearLayout two = new LinearLayout(this);
        TextView save = button("Save", 0x33808890, fg);
        save.setOnClickListener(v -> saveMenu(v, g));
        TextView more = button("More", 0x33808890, fg);
        more.setOnClickListener(v -> moreMenu(v, g));
        two.addView(save, new LinearLayout.LayoutParams(0, dp(48), 1));
        LinearLayout.LayoutParams l2 = new LinearLayout.LayoutParams(0, dp(48), 1);
        l2.leftMargin = dp(8);
        two.addView(more, l2);
        lp = new LinearLayout.LayoutParams(-1, -2);
        lp.topMargin = dp(8);
        info.addView(two, lp);
        box.addView(info, wide ? new LinearLayout.LayoutParams(0, -1, 1) : new LinearLayout.LayoutParams(-1, -2));
        return box;
    }

    // ------------------------------------------------------------------------------------------------------------- menus

    private void globalMenu(View anchor) {
        PopupMenu m = new PopupMenu(this, anchor);
        Menu menu = m.getMenu();
        boolean last = LauncherCore.nativeGet(LauncherCore.START) == 0;
        boolean tabs = LauncherCore.nativeGet(LauncherCore.LAYOUT) == 1;
        menu.add(0, 1, 0, "Settings");
        menu.add(1, 2, 1, "On Start, Open: The Last Game").setCheckable(true).setChecked(last);
        menu.add(1, 3, 2, "On Start, Open: This Screen").setCheckable(true).setChecked(!last);
        menu.setGroupCheckable(1, true, true);
        menu.add(2, 4, 3, "Layout: Library").setCheckable(true).setChecked(!tabs);
        menu.add(2, 5, 4, "Layout: Tabs").setCheckable(true).setChecked(tabs);
        menu.setGroupCheckable(2, true, true);
        menu.add(3, 6, 5, "Download Artwork").setCheckable(true).setChecked(LauncherCore.nativeGet(LauncherCore.COVERS) != 0);
        menu.add(3, 7, 6, "Skip Title and Menu").setCheckable(true).setChecked(LauncherCore.nativeGet(LauncherCore.SKIPMENU) != 0);
        m.setOnMenuItemClickListener(it -> {
            switch (it.getItemId()) {
                case 1: settings(); break;
                case 2: LauncherCore.nativeSet(LauncherCore.START, 0); break;
                case 3: LauncherCore.nativeSet(LauncherCore.START, 1); break;
                case 4: LauncherCore.nativeSet(LauncherCore.LAYOUT, 0); reload(); break;
                case 5: LauncherCore.nativeSet(LauncherCore.LAYOUT, 1); reload(); break;
                case 6:
                    LauncherCore.nativeSet(LauncherCore.COVERS, LauncherCore.nativeGet(LauncherCore.COVERS) != 0 ? 0 : 1);
                    reload();
                    break;
                case 7:
                    LauncherCore.nativeSet(LauncherCore.SKIPMENU, LauncherCore.nativeGet(LauncherCore.SKIPMENU) != 0 ? 0 : 1);
                    break;
                default: break;
            }
            return true;
        });
        m.show();
    }

    private void saveMenu(View anchor, LauncherCore.Game g) {
        PopupMenu m = new PopupMenu(this, anchor);
        m.getMenu().add(0, 1, 0, "Export Save").setEnabled(g.hasSave);
        m.getMenu().add(0, 2, 1, "Import Save...");
        m.getMenu().add(0, 3, 2, "Backups");
        m.setOnMenuItemClickListener(it -> {
            switch (it.getItemId()) {
                case 1: exportSave(g); break;
                case 2: pick(PICK_SAVE, g); break;
                default: backups(g); break;
            }
            return true;
        });
        m.show();
    }

    private void moreMenu(View anchor, LauncherCore.Game g) {
        PopupMenu m = new PopupMenu(this, anchor);
        Menu menu = m.getMenu();
        menu.add(0, 1, 0, "Set Artwork...");
        boolean hasArt = new File(gameDir(g), "artwork.jpg").exists();
        if (hasArt) {
            menu.add(0, 2, 1, "Remove Artwork");
        }
        menu.add(0, 3, 2, "Download Cover Again").setEnabled(LauncherCore.nativeGet(LauncherCore.COVERS) != 0 && !mFetching.contains(g.code));
        if (!g.coverSource.isEmpty()) {
            menu.add(0, 4, 3, "Cover art: " + g.coverSource);
        }
        menu.add(0, 5, 4, "Remove Game...");
        m.setOnMenuItemClickListener(it -> {
            switch (it.getItemId()) {
                case 1: pick(PICK_ART, g); break;
                case 2: new File(gameDir(g), "artwork.jpg").delete(); reload(); break;
                case 3: fetchCover(g, true); break;
                case 4: openLink(g.coverLink); break;
                default: remove(g); break;
            }
            return true;
        });
        m.show();
    }

    /** The library card's menu: the save and the "more" entries together. */
    private void gameMenu(View anchor, LauncherCore.Game g) {
        PopupMenu m = new PopupMenu(this, anchor);
        Menu menu = m.getMenu();
        menu.add(0, 1, 0, "Export Save").setEnabled(g.hasSave);
        menu.add(0, 2, 1, "Import Save...");
        menu.add(0, 3, 2, "Backups");
        menu.add(1, 4, 3, "Set Artwork...");
        if (new File(gameDir(g), "artwork.jpg").exists()) {
            menu.add(1, 5, 4, "Remove Artwork");
        }
        menu.add(1, 6, 5, "Download Cover Again").setEnabled(LauncherCore.nativeGet(LauncherCore.COVERS) != 0 && !mFetching.contains(g.code));
        if (!g.coverSource.isEmpty()) {
            menu.add(1, 7, 6, "Cover art: " + g.coverSource);
        }
        menu.add(2, 8, 7, "Remove Game...");
        m.setOnMenuItemClickListener(it -> {
            switch (it.getItemId()) {
                case 1: exportSave(g); break;
                case 2: pick(PICK_SAVE, g); break;
                case 3: backups(g); break;
                case 4: pick(PICK_ART, g); break;
                case 5: new File(gameDir(g), "artwork.jpg").delete(); reload(); break;
                case 6: fetchCover(g, true); break;
                case 7: openLink(g.coverLink); break;
                default: remove(g); break;
            }
            return true;
        });
        m.show();
    }

    // ------------------------------------------------------------------------------------------------------------- dialogs

    private AlertDialog.Builder dialog() {
        return new AlertDialog.Builder(this, android.R.style.Theme_Material_Dialog_Alert);
    }

    private void note(String title, String text) {
        dialog().setTitle(title).setMessage(text).setPositiveButton("OK", null).show();
    }

    private void ask(String title, String text, String button, Runnable then) {
        dialog().setTitle(title).setMessage(text).setNegativeButton("Cancel", null)
                .setPositiveButton(button, (d, w) -> then.run()).show();
    }

    private interface Progress {
        int get();    // 0..100, or -1 for the second phase's label
    }

    private void showBusy(String text) {
        hideBusy();
        LinearLayout l = new LinearLayout(this);
        l.setOrientation(LinearLayout.VERTICAL);
        l.setPadding(dp(24), dp(20), dp(24), dp(20));
        mBusyText = text(text, 16, TEXT, false);
        mBusyBar = new ProgressBar(this, null, android.R.attr.progressBarStyleHorizontal);
        mBusyBar.setMax(100);
        l.addView(mBusyText);
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(-1, -2);
        lp.topMargin = dp(16);
        l.addView(mBusyBar, lp);
        mBusy = new AlertDialog.Builder(this, android.R.style.Theme_Material_Dialog_Alert).setView(l).setCancelable(false).create();
        mBusy.show();
    }

    private void hideBusy() {
        if (mBusy != null) {
            mBusy.dismiss();
            mBusy = null;
        }
    }

    /** Polls the core's two progresses while a worker thread runs: copying the ROM, then extracting its files. */
    private void pollBusy(boolean copy) {
        mUi.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (mBusy == null) {
                    return;
                }
                int c = LauncherCore.nativeCopyPercent();
                if (copy && c < 100) {
                    mBusyBar.setProgress(c);
                } else {
                    mBusyText.setText("Preparing game files...");
                    mBusyBar.setProgress(LauncherCore.nativeExtractPercent());
                }
                mUi.postDelayed(this, 150);
            }
        }, 150);
    }

    // ------------------------------------------------------------------------------------------------------------- actions

    private void pick(int what, LauncherCore.Game g) {
        mPendingCode = g != null ? g.code : null;
        Intent i = new Intent(what == PICK_ART ? Intent.ACTION_GET_CONTENT : Intent.ACTION_OPEN_DOCUMENT);
        i.addCategory(Intent.CATEGORY_OPENABLE);
        i.setType(what == PICK_ART ? "image/*" : "*/*");    // (.nds and .sav have no MIME type)
        startActivityForResult(i, what);
    }

    @Override
    protected void onActivityResult(int req, int res, Intent data) {
        if (res != RESULT_OK || data == null || data.getData() == null) {
            return;
        }
        Uri uri = data.getData();
        if (req == PICK_ROM) {
            importFrom(uri, null);
        } else if (req == PICK_SAVE) {
            importSave(uri);
        } else if (req == PICK_ART) {
            setArtwork(uri);
        } else if (req == SAVE_AS) {
            new Thread(() -> {
                String msg = null;
                try (InputStream in = new FileInputStream(mPendingExport); OutputStream out = getContentResolver().openOutputStream(uri)) {
                    byte[] buf = new byte[65536];
                    int n;
                    while ((n = in.read(buf)) > 0) {
                        out.write(buf, 0, n);
                    }
                } catch (Exception e) {
                    msg = "The save could not be written: " + e.getMessage();
                }
                new File(mPendingExport).delete();
                final String fail = msg;
                mUi.post(() -> {
                    if (fail != null) {
                        note("Export Save", fail);
                    }
                });
            }).start();
        }
    }

    /** Add Game: LC_ImportRom (header, copy with its sha1, extraction) on a thread. The picker's file is read through its descriptor. */
    private void importFrom(Uri uri, File file) {
        showBusy("Copying the ROM...");
        pollBusy(true);
        new Thread(() -> {
            String err;
            String[] out = new String[1];
            ParcelFileDescriptor pfd = null;
            try {
                String path;
                if (uri != null) {
                    pfd = getContentResolver().openFileDescriptor(uri, "r");
                    path = "/proc/self/fd/" + pfd.getFd();
                } else {
                    path = file.getAbsolutePath();
                }
                err = LauncherCore.nativeImportRom(path, out);
            } catch (Exception e) {
                err = "The file could not be read: " + e.getMessage();
            } finally {
                if (pfd != null) {
                    try {
                        pfd.close();
                    } catch (Exception ignored) {
                    }
                }
            }
            final String fail = err;
            mUi.post(() -> {
                hideBusy();
                if (fail != null && !fail.isEmpty()) {
                    note("Couldn't add the game", fail);
                    return;
                }
                if (out[0] != null) {
                    mSelected = out[0];
                }
                reload();
            });
        }).start();
    }

    private void play(String code) {
        reload();
        LauncherCore.Game g = find(code);
        if (g == null) {
            return;
        }
        if (!g.playable) {
            note(g.title, g.title + " is not supported yet. This build plays Pokemon Black.");
            return;
        }
        if (g.hasFiles) {
            enter(g);
            return;
        }
        if (!g.hasRom) {
            note(g.title, "The game's files are missing. Remove the game and add its ROM again.");
            return;
        }
        showBusy("Preparing game files...");
        pollBusy(false);
        new Thread(() -> {
            int files = LauncherCore.nativeExtract(g.code);
            mUi.post(() -> {
                hideBusy();
                if (files > 0) {
                    reload();
                    enter(g);
                } else {
                    note(g.title, "The ROM's files couldn't be extracted (is the phone out of space?).");
                }
            });
        }).start();
    }

    private void enter(LauncherCore.Game g) {
        Log.i(TAG, "launcher: play " + g.title);
        LauncherCore.nativeSetLastGame(g.code);
        Intent i = new Intent(this, BwportActivity.class);
        Bundle ex = getIntent() != null ? getIntent().getExtras() : null;
        if (ex != null) {
            for (String k : ex.keySet()) {
                Object v = ex.get(k);
                if (v instanceof String && (k.startsWith("BWPORT_") || k.startsWith("SDL_")) && !k.equals("BWPORT_LAUNCHER")) {
                    i.putExtra(k, (String) v);
                }
            }
        }
        i.putExtra("BWPORT_GAME", g.code);
        startActivity(i);
    }

    private void exportSave(LauncherCore.Game g) {
        File dir = new File(getCacheDir(), "export");
        dir.mkdirs();
        String[] err = new String[1];
        String path = LauncherCore.nativeExportSave(g.code, dir.getAbsolutePath(), err);
        if (path.equals("!")) {
            note("Export Save", err[0]);
            return;
        }
        mPendingExport = path;
        Intent i = new Intent(Intent.ACTION_CREATE_DOCUMENT);
        i.addCategory(Intent.CATEGORY_OPENABLE);
        i.setType("application/octet-stream");
        i.putExtra(Intent.EXTRA_TITLE, new File(path).getName());
        startActivityForResult(i, SAVE_AS);
    }

    private void importSave(Uri uri) {
        LauncherCore.Game g = find(mPendingCode);
        if (g == null) {
            return;
        }
        File tmp = new File(getCacheDir(), "import.sav");
        try (InputStream in = getContentResolver().openInputStream(uri); OutputStream out = new FileOutputStream(tmp)) {
            byte[] buf = new byte[65536];
            int n;
            long total = 0;
            while ((n = in.read(buf)) > 0) {
                total += n;
                if (total > (8 << 20)) {
                    note("Import Save", "That file is too large to be a save.");
                    tmp.delete();
                    return;
                }
                out.write(buf, 0, n);
            }
        } catch (Exception e) {
            note("Import Save", "The file could not be read: " + e.getMessage());
            return;
        }
        String info = LauncherCore.nativeSaveFileInfo(tmp.getAbsolutePath());
        String what = "an empty save (a new game)";
        if (!info.isEmpty()) {
            String[] f = info.split("\t");
            what = "saved " + dateText(Long.parseLong(f[1])) + ", " + playText(Long.parseLong(f[0])) + " played";
        }
        ask("Import this save?", g.title + "'s save is replaced by this one: " + what + ".\n\nThe current save is backed up first (Backups).",
                "Import", () -> {
                    String err = LauncherCore.nativeImportSave(g.code, tmp.getAbsolutePath());
                    tmp.delete();
                    if (!err.isEmpty()) {
                        note("Import Save", err);
                    }
                    reload();
                });
    }

    private void backups(LauncherCore.Game g) {
        List<LauncherCore.Backup> l = LauncherCore.backups(g.code);
        ScrollView sv = new ScrollView(this);
        LinearLayout col = new LinearLayout(this);
        col.setOrientation(LinearLayout.VERTICAL);
        col.setPadding(dp(16), dp(8), dp(16), dp(8));
        sv.addView(col);
        AlertDialog d = dialog().setTitle(g.title + " Backups").setView(sv).setPositiveButton("Done", null).create();
        if (l.isEmpty()) {
            col.addView(text("No backups yet", 16, SUBTLE, false));
        }
        for (LauncherCore.Backup b : l) {
            LinearLayout row = new LinearLayout(this);
            row.setGravity(Gravity.CENTER_VERTICAL);
            LinearLayout names = new LinearLayout(this);
            names.setOrientation(LinearLayout.VERTICAL);
            String when = b.when == 0 ? new File(gameDir(g), "backups/" + b.name).lastModified() / 1000 > 0
                    ? dateText(new File(gameDir(g), "backups/" + b.name).lastModified() / 1000) : b.name : dateText(b.when);
            names.addView(text(when, 15, TEXT, false));
            names.addView(text(b.kind == 0 ? "Before a change" : b.kind == 1 ? "Kept copy" : "Before the library", 12, SUBTLE, false));
            row.addView(names, new LinearLayout.LayoutParams(0, -2, 1));
            TextView r = button("Restore", SURFACE2, TEXT);
            r.setContentDescription("Restore " + b.name);
            r.setOnClickListener(v -> ask("Restore this backup?", g.title + "'s save goes back to the backup from " + when
                    + ". The current save is backed up first.", "Restore", () -> {
                String err = LauncherCore.nativeRestore(g.code, b.name);
                d.dismiss();
                if (!err.isEmpty()) {
                    note("Restore", err);
                } else {
                    reload();
                    backups(g);
                }
            }));
            row.addView(r, new LinearLayout.LayoutParams(-2, dp(48)));
            LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(-1, -2);
            lp.topMargin = dp(6);
            col.addView(row, lp);
        }
        d.show();
    }

    private void remove(LauncherCore.Game g) {
        ask("Remove " + g.title + "?", "The game's files are deleted. Its save and backups stay in the app's files folder, and come back "
                + "if you add the game again.", "Remove", () -> {
            String err = LauncherCore.nativeRemove(g.code);
            if (!err.isEmpty()) {
                note("Remove Game", err);
            }
            reload();
        });
    }

    /** artwork.jpg in the game's folder: JPEG 85, the long side at most 1500 px, drawn upright. */
    private void setArtwork(Uri uri) {
        LauncherCore.Game g = find(mPendingCode);
        if (g == null) {
            return;
        }
        try {
            int rot = 0;
            try (InputStream in = getContentResolver().openInputStream(uri)) {
                int o = new ExifInterface(in).getAttributeInt(ExifInterface.TAG_ORIENTATION, 1);
                rot = o == 6 ? 90 : o == 3 ? 180 : o == 8 ? 270 : 0;
            } catch (Exception ignored) {
            }
            Bitmap b;
            try (InputStream in = getContentResolver().openInputStream(uri)) {
                BitmapFactory.Options bo = new BitmapFactory.Options();
                bo.inSampleSize = 1;
                b = BitmapFactory.decodeStream(in, null, bo);
            }
            if (b == null) {
                note("Set Artwork", "That file is not a picture the phone can read.");
                return;
            }
            float s = Math.min(1f, 1500f / Math.max(b.getWidth(), b.getHeight()));
            android.graphics.Matrix m = new android.graphics.Matrix();
            m.postScale(s, s);
            m.postRotate(rot);
            Bitmap o = Bitmap.createBitmap(b, 0, 0, b.getWidth(), b.getHeight(), m, true);
            File dst = new File(gameDir(g), "artwork.jpg");
            File tmp = new File(gameDir(g), "artwork.jpg.tmp");
            try (OutputStream out = new FileOutputStream(tmp)) {
                o.compress(Bitmap.CompressFormat.JPEG, 85, out);
            }
            if (!tmp.renameTo(dst)) {
                note("Set Artwork", "The picture could not be stored.");
            }
            Log.i(TAG, "launcher: artwork of " + g.title + " set");
        } catch (Exception | OutOfMemoryError e) {
            note("Set Artwork", "The picture could not be used: " + e.getMessage());
        }
        reload();
    }

    private void fetchCover(LauncherCore.Game g, boolean manual) {
        if (!mFetching.add(g.code)) {
            return;
        }
        new Thread(() -> {
            String err = LauncherCore.nativeFetchCover(g.code);
            mUi.post(() -> {
                mFetching.remove(g.code);
                if (manual && !err.isEmpty()) {
                    note("Download Cover Again", "The cover couldn't be downloaded: " + err);
                }
                mGames = LauncherCore.games();
                build();
            });
        }).start();
    }

    private void settings() {
        note("Settings", "The settings are in the game: open its menu button while a game runs.");
    }
}
