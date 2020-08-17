package com.google.cloud.android.speech;

import android.util.Log;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class DebugLogger {

    public static void out(String where, String what){
        String currentTime = new SimpleDateFormat("yyyy.MM.dd 'at' HH:mm:ss z", Locale.TAIWAN).format(new Date());
        Log.d("[*] " + currentTime, "Happens in " + where + ", " + what);
    }

}
