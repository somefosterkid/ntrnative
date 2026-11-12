package com.bwport.app;

import android.app.Activity;
import android.content.ClipData;
import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.Intent;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.net.Uri;
import android.os.ParcelFileDescriptor;
import android.provider.OpenableColumns;
import android.util.Log;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;

/**
 * "Share latest log" (iOS's UIActivityViewController on the newest Documents/logs file): the share sheet with this run's log, and the run
 * before it when there is one (a crash's log is the previous run's). The files are the app's external files: log.txt (this run: port/src/
 * android_main.c) and logs/bwport-*.log (the earlier runs, the last ten). They reach the other app through {@link Provider}, a plain
 * ContentProvider (no androidx here) that serves only those files, read only, to whoever the share grants (FLAG_GRANT_READ_URI_PERMISSION).
 * SettingsScreen calls {@link #share} by reflection.
 */
public final class LogShare {
    private static final String TAG = "bwport";
    static final String AUTHORITY = "com.bwport.app.logs";

    private LogShare() {}

    /** The app's files directory (where android_main.c keeps log.txt). */
    static File dir(Activity a) {
        File d = a.getExternalFilesDir(null);
        return d != null ? d : a.getFilesDir();
    }

    /** The newest earlier run's log in logs/, or null. */
    static File previous(File dir) {
        File[] l = new File(dir, "logs").listFiles((d, n) -> n.startsWith("bwport-") && n.endsWith(".log"));
        File best = null;
        if (l != null) {
            for (File f : l) {
                if (best == null || f.getName().compareTo(best.getName()) > 0) {
                    best = f;
                }
            }
        }
        return best;
    }

    public static void share(Activity act) {
        File dir = dir(act);
        File cur = new File(dir, "log.txt"), prev = previous(dir);
        if (!cur.isFile() && prev == null) {
            Toast.makeText(act, "The app has not written a log yet", Toast.LENGTH_LONG).show();
            return;
        }
        ArrayList<Uri> uris = new ArrayList<>();
        if (cur.isFile()) {
            uris.add(Uri.parse("content://" + AUTHORITY + "/log.txt"));
        }
        if (prev != null) {
            uris.add(Uri.parse("content://" + AUTHORITY + "/logs/" + prev.getName()));
        }
        Intent send = new Intent(uris.size() > 1 ? Intent.ACTION_SEND_MULTIPLE : Intent.ACTION_SEND);
        send.setType("text/plain");
        if (uris.size() > 1) {
            send.putParcelableArrayListExtra(Intent.EXTRA_STREAM, uris);
        } else {
            send.putExtra(Intent.EXTRA_STREAM, uris.get(0));
        }
        send.putExtra(Intent.EXTRA_SUBJECT, "bwport log");
        ClipData clip = ClipData.newRawUri("log", uris.get(0));
        for (int i = 1; i < uris.size(); i++) {
            clip.addItem(new ClipData.Item(uris.get(i)));
        }
        send.setClipData(clip);
        send.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
        Log.i(TAG, "share log: " + uris);
        System.err.println("bwport: share log " + uris);
        try {
            act.startActivity(Intent.createChooser(send, "Share latest log"));
        } catch (Exception e) {
            Log.w(TAG, "share log: " + e);
            Toast.makeText(act, "Could not share the log", Toast.LENGTH_LONG).show();
        }
    }

    /** Serves log.txt and logs/bwport-*.log of the app's external files, read only. */
    public static final class Provider extends ContentProvider {
        private File resolve(Uri uri) throws FileNotFoundException {
            String p = uri.getPath() == null ? "" : uri.getPath();
            File dir = getContext().getExternalFilesDir(null);
            if (dir == null) {
                dir = getContext().getFilesDir();
            }
            File f;
            if (p.equals("/log.txt")) {
                f = new File(dir, "log.txt");
            } else if (p.startsWith("/logs/") && p.indexOf('/', 6) < 0 && p.startsWith("/logs/bwport-") && !p.contains("..")) {
                f = new File(new File(dir, "logs"), p.substring(6));
            } else {
                throw new FileNotFoundException(p);
            }
            if (!f.isFile()) {
                throw new FileNotFoundException(p);
            }
            return f;
        }

        @Override
        public boolean onCreate() {
            return true;
        }

        @Override
        public ParcelFileDescriptor openFile(Uri uri, String mode) throws FileNotFoundException {
            if (!"r".equals(mode)) {
                throw new SecurityException("read only");
            }
            return ParcelFileDescriptor.open(resolve(uri), ParcelFileDescriptor.MODE_READ_ONLY);
        }

        @Override
        public Cursor query(Uri uri, String[] projection, String sel, String[] args, String sort) {
            File f;
            try {
                f = resolve(uri);
            } catch (FileNotFoundException e) {
                return null;
            }
            String[] cols = projection != null ? projection : new String[] {OpenableColumns.DISPLAY_NAME, OpenableColumns.SIZE};
            MatrixCursor c = new MatrixCursor(cols);
            Object[] row = new Object[cols.length];
            for (int i = 0; i < cols.length; i++) {
                row[i] = OpenableColumns.DISPLAY_NAME.equals(cols[i]) ? (f.getName().equals("log.txt") ? "bwport-current.log" : f.getName())
                    : OpenableColumns.SIZE.equals(cols[i]) ? (Object) f.length() : null;
            }
            c.addRow(row);
            return c;
        }

        @Override
        public String getType(Uri uri) {
            return "text/plain";
        }

        @Override
        public Uri insert(Uri uri, ContentValues v) {
            throw new UnsupportedOperationException();
        }

        @Override
        public int delete(Uri uri, String s, String[] a) {
            throw new UnsupportedOperationException();
        }

        @Override
        public int update(Uri uri, ContentValues v, String s, String[] a) {
            throw new UnsupportedOperationException();
        }
    }
}
