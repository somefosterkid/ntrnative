package com.bwport.app;

import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * The launcher's C core (port/src/launcher_core.c, launcher_cover.c) in the app's main process, through port/src/android_launcher.c:
 * the game table, import with validation, removal, saves, backups, covers, launcher.ini. The game itself runs in the ":game" process
 * (BwportActivity); this class loads the same library (libmain.so) for the core only. docs/launcher.md, "Android launcher".
 */
final class LauncherCore {
    static final String TAG = "bwport";

    static {
        System.loadLibrary("main");
    }

    private LauncherCore() {
    }

    /** One game of the library (a line of nativeGames). */
    static final class Game {
        String code, title, shortName, coverSource = "", coverLink = "";
        boolean playable, hasRom, hasFiles, hasSave;
        long playSeconds = -1, saved;    // saved: epoch seconds

        static Game parse(String line) {
            String[] f = line.split("\t", -1);
            if (f.length < 9) {
                return null;
            }
            Game g = new Game();
            g.code = f[0];
            g.title = f[1];
            g.shortName = f[2];
            g.playable = f[3].equals("1");
            g.hasRom = f[4].equals("1");
            g.hasFiles = f[5].equals("1");
            g.hasSave = f[6].equals("1");
            g.playSeconds = Long.parseLong(f[7]);
            g.saved = Long.parseLong(f[8]);
            if (f.length > 10) {
                g.coverSource = f[9];
                g.coverLink = f[10];
            }
            return g;
        }
    }

    /** One backup (a line of nativeBackups). */
    static final class Backup {
        String name;
        long when;
        int kind;
    }

    static native String nativeInit(String root);
    static native String[] nativeGames();
    static native String nativeGameDir(String code);
    static native String nativeImportRom(String path, String[] out);
    static native int nativeCopyPercent();
    static native int nativeExtractPercent();
    static native int nativeExtract(String code);
    static native String nativeRemove(String code);
    static native String nativeExportSave(String code, String dir, String[] err);
    static native String nativeImportSave(String code, String path);
    static native String nativeSaveFileInfo(String path);
    static native String[] nativeBackups(String code);
    static native String nativeRestore(String code, String name);
    static native void nativeSetLastGame(String code);
    static native String nativeLastGame();
    static native int nativeGet(int what);    // 0 start mode, 1 layout, 2 cover downloads, 3 skip title and menu
    static native void nativeSet(int what, int v);
    static native boolean nativeCoverWanted(String code);
    static native String nativeFetchCover(String code);

    static final int START = 0, LAYOUT = 1, COVERS = 2, SKIPMENU = 3;

    static java.util.List<Game> games() {
        java.util.ArrayList<Game> l = new java.util.ArrayList<>();
        String[] a = nativeGames();
        if (a != null) {
            for (String s : a) {
                if (s == null) {
                    break;
                }
                Game g = Game.parse(s);
                if (g != null) {
                    l.add(g);
                }
            }
        }
        return l;
    }

    static java.util.List<Backup> backups(String code) {
        java.util.ArrayList<Backup> l = new java.util.ArrayList<>();
        String[] a = nativeBackups(code);
        if (a != null) {
            for (String s : a) {
                String[] f = s.split("\t");
                if (f.length >= 3) {
                    Backup b = new Backup();
                    b.name = f[0];
                    b.when = Long.parseLong(f[1]);
                    b.kind = Integer.parseInt(f[2]);
                    l.add(b);
                }
            }
        }
        return l;
    }

    // ---- PORT_HttpGet (port/src/android_launcher.c calls these on the thread that called nativeFetchCover)
    private static String sWhy = "";

    static String httpWhy() {
        return sWhy;
    }

    /** One GET: https only (redirects too), 15 s, at most 10 MB, the launcher's user agent. 200 and the body in path, else the status or -1. */
    static int httpGet(String url, String path) {
        sWhy = "";
        HttpURLConnection c = null;
        try {
            if (!url.startsWith("https://")) {
                sWhy = "https only";
                return -1;
            }
            String cur = url;
            for (int hop = 0; hop < 5; hop++) {
                c = (HttpURLConnection) new URL(cur).openConnection();
                c.setConnectTimeout(15000);
                c.setReadTimeout(15000);
                c.setInstanceFollowRedirects(false);
                c.setRequestProperty("User-Agent", "bw-port-launcher/1.0 (a Pokemon Black/White port's game launcher; one cover per game)");
                int code = c.getResponseCode();
                if (code >= 300 && code < 400 && c.getHeaderField("Location") != null) {
                    String next = new URL(new URL(cur), c.getHeaderField("Location")).toString();
                    c.disconnect();
                    if (!next.startsWith("https://")) {
                        sWhy = "redirect to a non-https address";
                        return -1;
                    }
                    cur = next;
                    continue;
                }
                if (code != 200) {
                    return code;
                }
                File tmp = new File(path);
                long total = 0;
                try (InputStream in = c.getInputStream(); FileOutputStream out = new FileOutputStream(tmp)) {
                    byte[] buf = new byte[65536];
                    int n;
                    while ((n = in.read(buf)) > 0) {
                        total += n;
                        if (total > 10L * 1024 * 1024) {
                            sWhy = "the answer is over 10 MB";
                            new File(path).delete();
                            return -1;
                        }
                        out.write(buf, 0, n);
                    }
                }
                return 200;
            }
            sWhy = "too many redirects";
            return -1;
        } catch (Exception e) {
            sWhy = e.getClass().getSimpleName() + (e.getMessage() != null ? ": " + e.getMessage() : "");
            Log.i(TAG, "cover: " + sWhy);
            return -1;
        } finally {
            if (c != null) {
                c.disconnect();
            }
        }
    }
}
