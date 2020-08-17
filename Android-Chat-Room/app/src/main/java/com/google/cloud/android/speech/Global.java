package com.google.cloud.android.speech;


import android.app.AlertDialog;
import android.content.DialogInterface;
import android.util.Log;

import com.asus.robotframework.API.RobotAPI;
import com.asus.robotframework.API.RobotFace;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import com.google.cloud.android.speech.SpeechFragment;
import com.google.cloud.android.speech.MainActivityFragment;

public class Global {

    public static MainActivity mainAct;

    public static ArrayList wrongList = new ArrayList();
    // Robot speak
    public static RobotAPI api;
    public static boolean speak = true;
    private static Timer timer;

    public static void speakWithDiffFace(int milisec, Boolean isCorrect) {
        String text;
        api.robot.stopSpeak();
        if(isCorrect){
            //correct
            api.robot.setExpression(RobotFace.EXPECTING);
            text = "";//"答對囉!";
        }
        else{
            //not correct
            api.robot.setExpression(RobotFace.WORRIED);
            text = "";//"答錯囉!";
        }


        timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                api.robot.setExpression(RobotFace.HIDEFACE);
            }
        }, milisec);
        api.robot.speak(text);

    }

    public static void speakWithFace(int milisec, String text) {
        api.robot.stopSpeak();

        api.robot.setExpression(RobotFace.PLEASED);

        timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                api.robot.setExpression(RobotFace.HIDEFACE);
            }
        }, milisec);
        api.robot.speak(text);

    }

    // For CharacterDecision
    public static String character;

    // For line to get token id
    public static String charLineUuid ;

    // for 3 character
    public static String currentCharacter = "unknown"; // get in QRcode scan
    public static String character_token = ""; // get in option
    public static String char_prefix = "https://iir-rolemanager.azurewebsites.net/api/v1/";

    // for line api
    public static String current_user_id = "guest";
    public static String currentSession = "guest";
    public static String currentUserEmail = "guest@email.com";
    public static String line_prefix = "https://linemanager.azurewebsites.net/api/v2/";
    public static String line_login = Global.line_prefix+"Line-Nonce/Req/aeb53016-a9b7-4240-985c-21fbee73fa8f";
    public static String data_access_token = "";
    public static String current_field = "";
    public static Boolean ifScan = false;
    public static Boolean startCheck = false;
    public static Map<String, Integer> checkedItem = new HashMap<String, Integer>() {{
        put("pressureBox", 0);
        put("sleepBox", 0);
        put("drugBox", 0);
    }};

    // for Health Education
    public static String LineUserUUID = "b933046c-10c3-43ff-9ac8-e53ef1f5ddc4";//"0dac6971-a071-4817-8bc4-1325fcd7dcfa";
    public static String HEuserId;
    // diabetes_all = 31, high_pressure = 30, diabetes_nutrition = 32
    public static int HealthTopicId = 37;
    public static int questionNum = 0;
    public static String answerRecord_uuid = "unknown";

    public static Boolean startQuestionnaire = false;
    // for feedback
    public static int wrong_num = 0;
    public static String feedbackinfo = "";

    // clear character preference
    public static void clearCharacterPreference(){
        currentCharacter = "unknown"; // get in QRcode scan
        character_token = ""; // get in option
    }

    //clear Health Education preference
    public static void clearHEPreference(){
        HEuserId="unknown";
        // diabetes_all = 31, high_pressure = 30, diabetes_nutrition = 32
        HealthTopicId = 31;
        questionNum = 0;
        answerRecord_uuid = "unknown";
        startQuestionnaire = false;
    }

    // start logout & end dialog
    public static void openLogoutDialog(SpeechFragment frag) {
        Global.api.robot.speak("確定要登出結束嗎?");

        new AlertDialog.Builder(frag.getContext())
                .setTitle("登出並結束")
                .setMessage("確定要登出結束嗎?")
                .setPositiveButton("是",
                        new DialogInterface.OnClickListener() {

                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                //按下按鈕後執行的動作，沒寫則退出Dialog
                                //logout
                                clearCharacterPreference();
                                clearHEPreference();
//                                clearLinePreference();
                                mainAct.changeFragment(new MainActivityFragment());
                            }
                        }
                )
                .setNegativeButton("否",
                        new DialogInterface.OnClickListener() {

                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                //按下按鈕後執行的動作，沒寫則退出Dialog
                                //不動
                            }
                        }
                )
                .show();
        Log.i("pop out", "popping out");

    }

}