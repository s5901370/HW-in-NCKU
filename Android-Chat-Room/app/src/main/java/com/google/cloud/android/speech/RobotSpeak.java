package com.google.cloud.android.speech;

import com.asus.robotframework.API.RobotAPI;

import com.google.cloud.android.speech.Global;


public class RobotSpeak {

    public static void speak(String text){
        if (Global.speak){
            if (Global.api == null){
                Global.api = new RobotAPI(Global.mainAct);
            }
            Global.api.robot.speak(text);
        }
    }

    public static void stopTalking(){
        if (Global.speak){
            if (Global.api == null){
                Global.api = new RobotAPI(Global.mainAct);
            }
            Global.api.robot.stopSpeak();
        }
    }
}
