package com.google.cloud.android.speech;

import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.app.Activity;
import android.os.Message;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.ProgressBar;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.Socket;

public class Welcome extends Activity {
    private Toolbar mToolBar;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_welcome);
        mToolBar = (Toolbar) findViewById(R.id.toolbar);
        final Resources resources = getResources();
        final Resources.Theme theme = getTheme();
        mToolBar.setTitle(R.string.app_name);


//        new SimpleAsyncTask(this,mBar).execute();
    }

    public void goback(View view) {
        Intent replyIntent = new Intent();
        setResult(RESULT_OK,replyIntent);
        finish();
    }
}