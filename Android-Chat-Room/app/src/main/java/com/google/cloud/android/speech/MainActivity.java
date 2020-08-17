package com.google.cloud.android.speech;

import android.Manifest;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.IBinder;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Toast;

import com.asus.robotframework.API.RobotAPI;

import java.util.ArrayList;
import java.util.HashMap;

import com.google.cloud.android.speech.R;
import com.google.cloud.android.speech.SpeechReaction;

import com.google.cloud.android.speech.SpeechService;
import com.google.cloud.android.speech.VoiceRecorder;
import com.google.cloud.android.speech.MainActivityFragment;
import com.google.cloud.android.speech.DebugLogger;
import com.google.cloud.android.speech.TextUtil;

import static com.google.cloud.android.speech.Global.wrongList;

public class MainActivity extends AppCompatActivity {
    private SpeechService mSpeechService;
    private SpeechReaction currentReaction;
    private VoiceRecorder mVoiceRecorder;
    private HashMap<String, HashMap<String, ArrayList<String>>> keywordTable;



    private final VoiceRecorder.Callback mVoiceCallback = new VoiceRecorder.Callback() {
        @Override
        public void onVoiceStart() {
            if (mSpeechService != null){
                mSpeechService.startRecognizing(mVoiceRecorder.getSampleRate());
            }
        }

        @Override
        public void onVoice(byte[] data, int size) {
            if (mSpeechService != null) {
                mSpeechService.recognize(data, size);
            }
        }

        @Override
        public void onVoiceEnd() {
            if (mSpeechService != null) {
                mSpeechService.finishRecognizing();
            }
        }
    };

    private final SpeechService.Listener mSpeechServiceListener = new SpeechService.Listener() {
        @Override
        public void onSpeechRecognized(final String text, final boolean isFinal) {
            if (isFinal) {
                mVoiceRecorder.dismiss();
            }
            if (!TextUtils.isEmpty(text)) {
                runOnUiThread(() -> {
                    if (isFinal) {
                        Toast.makeText(MainActivity.this, "偵測到 :" + text, Toast.LENGTH_SHORT).show();
                        if (currentReaction != null) {
                            currentReaction.onSentenceEnd(text);
                        }
                    }else {
                        //DebugLogger.out("MainActivity", "SpeechRecognized not yet Final");
                    }
                });
            }
        }
    };

    public final ServiceConnection mServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder binder) {
            DebugLogger.out("MainActivity,onServiceConnected", "");
            mSpeechService = SpeechService.from(binder);
            MainActivity.this.mSpeechService.addListener(mSpeechServiceListener);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            DebugLogger.out("MainActivity,onServiceDisconnected", "");
            mSpeechService = null;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.container);

        /*
         * Initialize Global parameters
         */
        Global.mainAct = this;
        if (Global.api == null){
            Global.api = new RobotAPI(this);
            keywordTable = TextUtil.readKeywordsTable(getApplicationContext());
        }

        /*
         * Avoid default behavior (Shy)
         */
        Global.api.robot.setPressOnHeadAction(false);

        /*
         * Avoid screen jump back to junior default face
         */
        this.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        /*
         * Avoid keyboard pop out in Fragment contain editText
         */
        this.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
        /*
         * Hide title bar
         * */
        //getSupportActionBar().hide();
        /*
         * Hide navigate bar
         * https://stackoverflow.com/questions/21724420/how-to-hide-navigation-bar-permanently-in-android-activity
         */
        final int flags = View.SYSTEM_UI_LAYOUT_FLAGS
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
        this.getWindow().getDecorView().setSystemUiVisibility(flags);


        /*
         * very important
         * https://zenbo.asus.com/developer/documents/Zenbo-SDK/DialogSystem
         * setTouchOnlySignal可以關掉語音hey zenbo觸發zenbo mode
         */
        Global.api.robot.setTouchOnlySignal(true);
        Global.api.robot.setVoiceTrigger(false);
    }

    @Override
    protected void onStart() {
        super.onStart();
        DebugLogger.out("MainActivity,onStart", "Bind Google Cloud Speech Service");

        // Prepare Cloud Speech API
        bindService(new Intent(this, SpeechService.class), mServiceConnection, BIND_AUTO_CREATE);

        int PERMISSION_ALL = 1;
        String[] PERMISSIONS = {
                Manifest.permission.RECORD_AUDIO,
                Manifest.permission.CAMERA,
                Manifest.permission.ACCESS_FINE_LOCATION
        };

        if (!hasPermissions(this, PERMISSIONS)) {
            ActivityCompat.requestPermissions(this, PERMISSIONS, PERMISSION_ALL);
        }

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO)
                == PackageManager.PERMISSION_GRANTED) {
            startVoiceRecorder();
            DebugLogger.out("MainActivity,onStart", "start voice recorder");
        }

        /*
         * AutoJump fragment
         */
        // System version
//        if (!Global.ifScan) {
//            changeFragment(new MainActivityFragment());
//        }
//        else{
//            changeFragment(new CharacterDetectFragment());
//        }

        // Demo version

        if(wrongList.size() > 0);
        else {
            changeFragment(new MainActivityFragment());
        }

        // check bluetooth permission
        checkBLEPermission();
    }

    private void checkBLEPermission(){
        /*
         * check bluetooth permission
         */
        if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)){
            Toast.makeText(this, "ble not supported", Toast.LENGTH_SHORT).show();
            finish();
        }

        /*
         * Ask for location permission if not already allowed
         */
        if(ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED)
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 1);

    }

    public static boolean hasPermissions(Context context, String... permissions) {
        if (context != null && permissions != null) {
            for (String permission : permissions) {
                if (ActivityCompat.checkSelfPermission(context, permission) != PackageManager.PERMISSION_GRANTED) {
                    return false;
                }
            }
        }
        return true;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        if (requestCode == 1) {
            if (permissions.length == 1 && grantResults.length == 1
                    && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                startVoiceRecorder();
            }

        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }

    // Hide status and bottom(navigate) bar
    @Override
    public void onResume() {
        super.onResume();
        // hide navigate bar
        final int flags = View.SYSTEM_UI_LAYOUT_FLAGS
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
        int currentAPIVersion = Build.VERSION.SDK_INT;
        if (currentAPIVersion >= Build.VERSION_CODES.KITKAT) {
            getWindow().getDecorView().setSystemUiVisibility(flags);
        }
    }


    // Hide status and bottom(navigate) bar
    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        // hide navigate bar
        final int flags = View.SYSTEM_UI_LAYOUT_FLAGS
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
        int currentAPIVersion = Build.VERSION.SDK_INT;
        if (currentAPIVersion >= Build.VERSION_CODES.KITKAT && hasFocus) {
            getWindow().getDecorView().setSystemUiVisibility(flags);
        }
    }

    @Override
    protected void onStop() {
        super.onStop();

        // Stop listening to voice
        stopVoiceRecorder();

        // Stop Cloud Speech API
        if (mSpeechService == null) {
            return;
        }
        mSpeechService.removeListener(mSpeechServiceListener);
        unbindService(mServiceConnection);
        mSpeechService = null;
        Log.d("MainActivity,onStop", "unbind service");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        // Stop listening to voice
        stopVoiceRecorder();

        // Stop Cloud Speech API
        if (mSpeechService == null) {
            return;
        }
        mSpeechService.removeListener(mSpeechServiceListener);
        unbindService(mServiceConnection);
        mSpeechService = null;
        Log.d("MainActivity,onDestroy", "unbind service");
    }

    private void startVoiceRecorder() {
        if (mVoiceRecorder != null) {
            mVoiceRecorder.stop();
        }
        mVoiceRecorder = new VoiceRecorder(mVoiceCallback);
        mVoiceRecorder.start();
    }

    private void stopVoiceRecorder() {
        if (mVoiceRecorder != null) {
            mVoiceRecorder.stop();
            mVoiceRecorder = null;
        }
    }

    public void setCurrentReaction(SpeechReaction reaction) {
        this.currentReaction = reaction;
    }

    public void changeFragment(Fragment f) {
        FragmentManager fm = getSupportFragmentManager();
        fm.beginTransaction().replace(R.id.main_container, f).commit();
    }

    public HashMap<String, HashMap<String, ArrayList<String>>> getKeywordTable() {
        return keywordTable;
    }
}
